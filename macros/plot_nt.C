#include <iostream>
#include <string>

#include "TH1.h"
#include "TH1F.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"

#include "plotStyle.h"
#include "tpcResNtuple.h"


struct Hists
{
  std::string title;
  TH1F* h1McPt;
  TH1F* h1HftMatchedMcPt;
  TH1F* h1TpcMcPt;
  TH1F* h1DcaZPosEta;
  TH1F* h1DcaZNegEta;
  TH1F* h1DcaZ;

  TGraphAsymmErrors* gTpcEff;
  TGraphAsymmErrors* gHftEff;

  Hists(std::string name)
  {
    title            = name;
    h1McPt           = new TH1F(Form("h1McPt_%s",name.c_str()),";mcP_{T}(GeV/c)",40,1,5);
    h1HftMatchedMcPt = new TH1F(Form("h1HftMatchedMcPt_%s",name.c_str()),";mcP_{T}(GeV/c)",40,1,5);
    h1TpcMcPt        = new TH1F(Form("h1TpcMcPt_%s",name.c_str()),";mcP_{T}(GeV/c)",40,1,5);
    gTpcEff          = new TGraphAsymmErrors; gTpcEff->SetName(Form("gTpcEff_%s",name.c_str()));
    gHftEff          = new TGraphAsymmErrors; gHftEff->SetName(Form("gHftEff_%s",name.c_str()));
    h1DcaZPosEta     = new TH1F(Form("h1DcaZPosEta_%s",name.c_str()),";dcaZ(cm)",400,-1,1);
    h1DcaZNegEta     = new TH1F(Form("h1DcaZNegEta_%s",name.c_str()),";dcaZ(cm)",400,-1,1);
    h1DcaZ           = new TH1F(Form("h1DcaZ_%s",name.c_str()),";dcaZ(cm)",400,-1,1);

    h1McPt->SetDirectory(0);
    h1HftMatchedMcPt->SetDirectory(0);
    h1TpcMcPt->SetDirectory(0);
    h1DcaZ->SetDirectory(0);
  }
};

Hists getHists(std::string inFileName,std::string title,bool istOnly = false,bool istPxl2Only = false)
{
  TFile file(inFileName.c_str());
  tpcResNtuple nt((TTree*)file.Get("nt"));

  Hists hists(title);

  for(int ii=0;ii<nt.GetEntries();++ii)
  {
    nt.GetEntry(ii);

    hists.h1McPt->Fill(nt.pt);

    if(nt.geantId!=8 || fabs(nt.eta)>0.5) continue;

    if(nt.gPt>1 && nt.gPt<5. && nt.nFit>20 && nt.nFit/nt.nMax>0.52)
    {
      hists.h1TpcMcPt->Fill(nt.pt);
      
      int hftTopo = static_cast<int>(nt.hftTopo);

      if(istOnly)
      {
        if(nt.isTrueHft && (hftTopo>>3 & 0x3))
        {
          hists.h1HftMatchedMcPt->Fill(nt.pt);
        }
      }
      else if(istPxl2Only)
      {
        if(nt.isTrueHft && (hftTopo>>1 & 0x3) && (hftTopo>>3 & 0x3))
        {
          hists.h1HftMatchedMcPt->Fill(nt.pt);
        }
      }
      else
      {
        if(nt.isTrueHft && ((hftTopo>>0 & 0x1) && (hftTopo>>1 & 0x3) && (hftTopo>>3 & 0x3)))
        {
          hists.h1HftMatchedMcPt->Fill(nt.pt);
        }
      }

      if(nt.gPt>2. && fabs(nt.eta)>0.1)
      {
        hists.h1DcaZ->Fill(nt.dcaZ);
        if(nt.eta>0) hists.h1DcaZPosEta->Fill(nt.dcaZ);
        if(nt.eta<0) hists.h1DcaZNegEta->Fill(nt.dcaZ);
      }
    }
  }

  hists.gTpcEff->Divide(hists.h1TpcMcPt,hists.h1McPt);
  hists.gHftEff->Divide(hists.h1HftMatchedMcPt,hists.h1TpcMcPt);

  file.Close();

  return hists;
}

void plot_eff(std::vector<Hists*> hists, std::string outBaseName)
{
  if(hists.empty()) return;

  TCanvas* cv = canvas();
  cv->SetName("cv_eff");
  cv->cd();

  hists[0]->gHftEff->Draw("AP");
  setStyle(hists[0]->gHftEff,20,2);
  hists[0]->gHftEff->SetMinimum(0);
  hists[0]->gHftEff->SetMaximum(2.0);
  hists[0]->gHftEff->GetXaxis()->SetTitle("mcP_{T}(GeV/c)");

  TLegend* leg = legend("HFT to TPC Eff.");
  leg->SetY1NDC(0.8);
  leg->SetY2NDC(0.95);
  leg->AddEntry(hists[0]->gHftEff,hists[0]->title.c_str(),"LP");

  int const COLOR[] = {1,4,6,8};
  for(size_t ih=1; ih<hists.size(); ++ih)
  {
    hists[ih]->gHftEff->Draw("P:SAME");
    setStyle(hists[ih]->gHftEff,20,COLOR[(ih-1)%4]);
    leg->AddEntry(hists[ih]->gHftEff,hists[ih]->title.c_str(),"LP");
  }

  leg->Draw();

  cv->SaveAs(Form("%s.HftToTpcEff.png",outBaseName.c_str()));
}

void plot_dca(Hists& hists0,std::string outBaseName)
{
  // gStyle->SetOptStat("2200");

  TCanvas* cv = canvas();
  cv->SetName(Form("cv_dca_%s",hists0.title.c_str()));
  cv->cd();
  setStyle(hists0.h1DcaZ,20,kGreen+2);
  setStyle(hists0.h1DcaZPosEta,20,2);
  setStyle(hists0.h1DcaZNegEta,20,4);
  hists0.h1DcaZ->Draw("CL:P");
  hists0.h1DcaZPosEta->Draw("CL:P:sames");
  hists0.h1DcaZNegEta->Draw("CL:P:sames");
  gPad->Update();

  TPaveStats* stDcaZ = statBox(hists0.h1DcaZ);
  TPaveStats* stDcaZPos = statBox(hists0.h1DcaZPosEta);
  TPaveStats* stDcaZNeg = statBox(hists0.h1DcaZNegEta);
  alignStatBox(stDcaZ,stDcaZPos);
  alignStatBox(stDcaZPos,stDcaZNeg);

  TLegend* leg = legend(Form("dcaZ - %s",hists0.title.c_str()),0.4,0.85);
  leg->AddEntry(hists0.h1DcaZ,"#left|#eta#right|< 0.5","LP");
  leg->AddEntry(hists0.h1DcaZPosEta,"#eta > 0.1","LP");
  leg->AddEntry(hists0.h1DcaZNegEta,"#eta < -0.1","LP");
  leg->Draw();

  cv->SaveAs(Form("%s.%s.dcaZ.png",outBaseName.c_str(),hists0.title.c_str()));
}

using namespace std;

void plot_nt()
{
  TH1::AddDirectory(false);
  TFile::AddDirectory(false);

  string outBaseName = "piplus_7130_vs_7128_vs_612";
  Hists hists0 = getHists("../2015-10-19/piplus_612.tpcRes.root","ideal");
  Hists hists1 = getHists("../2015-10-19/piplus_7128.tpcRes.root","7128 - SL15c");
  Hists hists2 = getHists("zach_7130_SL16c.tpcRes.root","7130 - SL16c");

  vector<Hists*> hists;
  hists.push_back(&hists0);
  hists.push_back(&hists1);
  hists.push_back(&hists2);

  setGraphicsStyle();
  plot_eff(hists,outBaseName);
}
