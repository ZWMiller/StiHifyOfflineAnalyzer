#include <string>

#include "TStyle.h"
#include "TGaxis.h"
#include "TColor.h"
#include "TH1.h"
#include "TH2.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TList.h"
#include "TGraph.h"

void setGraphicsStyle()
{
    // **************************** Set graphic style ***************************************
    gStyle->SetEndErrorSize(5);
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(2200);
    gStyle->SetOptFit(111);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetPalette(1);
    gStyle->SetCanvasColor(10);
    gStyle->SetFrameFillColor(10);
    gStyle->SetFillColor(4);
    TGaxis::SetMaxDigits(4);
    gStyle->SetPadTopMargin(0.055);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadRightMargin(0.115);
    gStyle->SetPadLeftMargin(0.115);
    gStyle->SetLabelSize(0.056,"X");
    gStyle->SetLabelSize(0.056,"Y");
    gStyle->SetTitleSize(0.056,"X");
    gStyle->SetTitleSize(0.056,"Y");
    gStyle->SetTitleOffset(1.0,"X");
    gStyle->SetTitleOffset(1.0,"Y");
    gStyle->SetTitleFontSize(0.1);
    gStyle->SetTitleFont(42, "X");
    gStyle->SetTitleFont(42, "Y");
    gStyle->SetTitleFont(42, "Z");
    gStyle->SetTitleOffset(1.0, "Y");

    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t reds[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t greens[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blues[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    Int_t  FI = TColor::CreateGradientColorTable(NRGBs, stops, reds, greens, blues, NCont);
    gStyle->SetNumberContours(NCont);
    gStyle->UseCurrentStyle();
    // **************************************************************************************
}

void setStyle(TH1* h,int marker,Color_t color,std::string title="")
{
  h->GetYaxis()->SetTitle(title.c_str());
  h->SetMarkerStyle(marker);
  h->SetLineColor(color);
  h->SetMarkerColor(color);
  h->SetLineWidth(1.2);
  h->SetMarkerSize(1.2);

  h->SetTitleFont(42, "X");
  h->SetTitleFont(42, "Y");
  h->SetLabelFont(42, "X");
  h->SetLabelFont(42, "Y");
  h->SetTitleSize(.056, "X");
  h->SetTitleSize(.056, "Y");
  h->SetLabelSize(.056, "X");
  h->SetLabelSize(.056, "Y");
  h->SetTitleOffset(1.1, "X");
  h->SetTitleOffset(0.7, "Y");
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();
}

void setStyle(TH2* h,int marker=0,Color_t color=0)
{
  if(marker) 
  {
    h->SetMarkerStyle(marker);
    h->SetMarkerSize(1.2);
  }

  if(color)
  {
    h->SetLineColor(color);
    h->SetMarkerColor(color);
  }

  h->SetLineWidth(1.2);

  h->SetTitleFont(42, "X");
  h->SetTitleFont(42, "Y");
  h->SetLabelFont(42, "X");
  h->SetLabelFont(42, "Y");
  h->SetTitleSize(.053, "X");
  h->SetTitleSize(.053, "Y");
  h->SetLabelSize(.053, "X");
  h->SetLabelSize(.053, "Y");
  h->SetTitleOffset(1., "X");
  h->SetTitleOffset(1.0, "Y");
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();
}

void setStyle(TGraph* gr,int marker,Color_t color)
{
  gr->SetMarkerStyle(marker);
  gr->SetLineColor(color);
  gr->SetMarkerColor(color);
  gr->SetLineWidth(1.2);
  gr->SetMarkerSize(1.2);
  if(gr->GetHistogram()) setStyle(gr->GetHistogram(),marker,color);
}

TLegend* legend(std::string header="",float y1Ndc=0.6,float y2Ndc=0.85,float x1Ndc=0.15,float x2Ndc=0.4)
{
  static int nLegends = 0;
  ++nLegends;
  TLegend* leg = new TLegend(x1Ndc,y1Ndc,x2Ndc,y2Ndc);
  leg->SetName(Form("LEGEND_%i",nLegends));
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.08);
  if(!header.empty()) leg->SetHeader(header.c_str());

  return leg;
}

TCanvas* canvas(bool square=false)
{
  static int nCvs = 0;
  ++nCvs;
  TCanvas* cv = NULL;
  // if(!square) cv = new TCanvas(Form("cv%i",nCvs),Form("cv%i",nCvs),2*700,2*220);//338);
  if(!square) cv = new TCanvas(Form("cv%i",nCvs),Form("cv%i",nCvs),2*600,2*338);
  else cv = new TCanvas(Form("cv%i",nCvs),Form("cv%i",nCvs),1000,1000);

  return cv;
}

TPaveStats* statBox(TH1* h)
{
  gPad->Update(); 
  TPaveStats* st0 = (TPaveStats*)h->GetListOfFunctions()->FindObject("stats");

  st0->SetName(Form("st_%s",h->GetName()));
  st0->SetTextColor(h->GetMarkerColor());
  st0->SetTextSize(0.035);
  st0->SetX1NDC(0.55);
  st0->SetX2NDC(0.98);

  return st0;
}

void alignStatBox(TPaveStats* const st0,TPaveStats* const st1)
{
  st1->SetX1NDC(st0->GetX1NDC());
  st1->SetX2NDC(st0->GetX2NDC());
  st1->SetY2NDC(st0->GetY1NDC());
  st1->SetY1NDC(st0->GetY1NDC()-(st0->GetY2NDC()-st0->GetY1NDC()));
}
