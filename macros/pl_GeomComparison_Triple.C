#include "anaConstEmbed.h"
void drawSort(TCanvas*, int, TH1F*, TH1F*,TH1F*, int);
void Normalize(TH1F*, TH1F*, TH1F*);


int pl_GeomComparison_Triple(TString baseName = "junk.root", TString baseName2 = "junk2.root", TString baseName3 = "junk3.root"){

  bool DEBUG = kFALSE;
  TString detector[3] = {"IST","PXL1","PXL2"};
  baseName.ReplaceAll(".root","");
  baseName2.ReplaceAll(".root","");
  baseName3.ReplaceAll(".root","");
  TString bName[3] = {baseName,baseName2,baseName3};
  TString legName[3] = {baseName,baseName2,baseName3};
  legName[0].ReplaceAll(".stihify.hist","");
  legName[1].ReplaceAll(".stihify.hist","");
  legName[2].ReplaceAll(".stihify.hist","");
  legName[0].ReplaceAll("piplus_","");
  legName[1].ReplaceAll("piplus_","");
  legName[2].ReplaceAll("piplus_","");
  for(int ll=0;ll<3;ll++)
  {
    legName[ll].ReplaceAll("7130","Database");
    legName[ll].ReplaceAll("7131","Ideal");
  }

  // Open Files
  TFile* f[3][3];
  TString fileName;
  if(DEBUG) cout << "at open file" << endl;
  for (int i=0; i<3; i++){
    for (int det=0; det<3; det++){
      fileName = "";
      fileName += bName[i] + "." + detector[det] + ".processed.root";
      f[i][det] = new TFile(fileName,"READ");
      if(!f[i][det]->IsOpen())
      {
        cout << "Failed to open " << fileName << ". Aborting." << endl;
        cout << "Input file name should not have *.DETNAME.root, only *.root" << endl;
        exit(EXIT_FAILURE);
      }
    }
  }


  // Get Histos
  if(DEBUG) cout << "at get anaConst" << endl;
  const Int_t nPtBins  = anaConst::numPtBins;
  const Int_t nEtaBins = anaConst::numEtaBins;
  Float_t ptBinLow[nPtBins],ptBinHigh[nPtBins];
  Float_t etaBinLow[nEtaBins],etaBinHigh[nEtaBins];
  for(Int_t c=0; c< nPtBins; c++){
    ptBinLow[c] = anaConst::ptBinLow[c];
    ptBinHigh[c] = anaConst::ptBinHigh[c];
  }
  for(Int_t c=0; c< nEtaBins; c++){
    etaBinLow[c] = anaConst::etaBinLow[c];
    etaBinHigh[c] = anaConst::etaBinHigh[c];
  }


  TH1F* errorEta[3][3][nEtaBins];
  TH1F* errorPt[3][3][nPtBins];
  TH1F* resEta[3][3][nEtaBins];
  TH1F* resPt[3][3][nPtBins];
  TH1F* pullEta[3][3][nEtaBins];
  TH1F* pullPt[3][3][nPtBins];
  TH1F* RerrorEta[3][3][nEtaBins];
  TH1F* RerrorPt[3][3][nPtBins];
  TH1F* RresEta[3][3][nEtaBins];
  TH1F* RresPt[3][3][nPtBins];
  TH1F* RpullEta[3][3][nEtaBins];
  TH1F* RpullPt[3][3][nPtBins];

  if(DEBUG) cout << "at get hists" << endl;
  for (int i=0; i<3; i++){
    for(int det=0; det<3; det++)
    {
      for(int n=0; n<nEtaBins; n++)
      {
        errorEta[i][det][n] = (TH1F*)f[i][det]->Get(Form("errorEtaAcc_%i",n));
        resEta[i][det][n] = (TH1F*)f[i][det]->Get(Form("resEtaAcc_%i",n));
        pullEta[i][det][n] = (TH1F*)f[i][det]->Get(Form("pullEtaAcc_%i",n));

        RerrorEta[i][det][n] = (TH1F*)f[i][det]->Get(Form("errorEtaRej_%i",n));
        RresEta[i][det][n] = (TH1F*)f[i][det]->Get(Form("resEtaRej_%i",n));
        RpullEta[i][det][n] = (TH1F*)f[i][det]->Get(Form("pullEtaRej_%i",n));
      }

      for(int n=0; n<nPtBins; n++)
      {
        errorPt[i][det][n] = (TH1F*)f[i][det]->Get(Form("errorPtAcc_%i",n));
        resPt[i][det][n] = (TH1F*)f[i][det]->Get(Form("resPtAcc_%i",n));
        pullPt[i][det][n] = (TH1F*)f[i][det]->Get(Form("pullPtAcc_%i",n));

        RerrorPt[i][det][n] = (TH1F*)f[i][det]->Get(Form("errorPtRej_%i",n));
        RresPt[i][det][n] = (TH1F*)f[i][det]->Get(Form("resPtRej_%i",n));
        RpullPt[i][det][n] = (TH1F*)f[i][det]->Get(Form("pullPtRej_%i",n));
      }
    }
  }

  // Make Canvii
  const int numPtCanvas = nPtBins/9 + 1;
  const int numEtaCanvas = nEtaBins/9 + 1;
  TCanvas* cErrEta[3][numEtaCanvas];
  TCanvas* cResEta[3][numEtaCanvas];
  TCanvas* cPullEta[3][numEtaCanvas];
  TCanvas* cErrPt[3][numPtCanvas];
  TCanvas* cResPt[3][numPtCanvas];
  TCanvas* cPullPt[3][numPtCanvas];
  TCanvas* cRErrEta[3][numEtaCanvas];
  TCanvas* cRResEta[3][numEtaCanvas];
  TCanvas* cRPullEta[3][numEtaCanvas];
  TCanvas* cRErrPt[3][numPtCanvas];
  TCanvas* cRResPt[3][numPtCanvas];
  TCanvas* cRPullPt[3][numPtCanvas];
  if(DEBUG) cout << "at make canvas" << endl;
  for(int d = 0; d < 3; d++)
  {
    for(int i = 0; i < numEtaCanvas; i++)
    {
      cErrEta[d][i] = new TCanvas(Form("cErrEta_%i_%i",i,d),"Error Eta Dependence",0,0,1050,1050);
      cErrEta[d][i]->Divide(3,3);
      cPullEta[d][i] = new TCanvas(Form("cPullEta_%i_%i",i,d),"Pull Eta Dependence",0,0,1050,1050);
      cPullEta[d][i]->Divide(3,3);
      cResEta[d][i] = new TCanvas(Form("cResEta_%i_%i",i,d),"Res Eta Dependence",0,0,1050,1050);
      cResEta[d][i]->Divide(3,3);

      cRErrEta[d][i] = new TCanvas(Form("cRErrEta_%i_%i",i,d),"Error Eta Dependence",0,0,1050,1050);
      cRErrEta[d][i]->Divide(3,3);
      cRPullEta[d][i] = new TCanvas(Form("cRPullEta_%i_%i",i,d),"Pull Eta Dependence",0,0,1050,1050);
      cRPullEta[d][i]->Divide(3,3);
      cRResEta[d][i] = new TCanvas(Form("cRResEta_%i_%i",i,d),"Res Eta Dependence",0,0,1050,1050);
      cRResEta[d][i]->Divide(3,3);
    }
    for(int i = 0; i < numPtCanvas; i++)
    {
      cErrPt[d][i] = new TCanvas(Form("cErrPt_%i_%i",i,d),"Error Pt Dependence",0,0,1050,1050);
      cErrPt[d][i]->Divide(3,3);
      cPullPt[d][i] = new TCanvas(Form("cPullPt_%i_%i",i,d),"Pull Pt Dependence",0,0,1050,1050);
      cPullPt[d][i]->Divide(3,3);
      cResPt[d][i] = new TCanvas(Form("cResPt_%i_%i",i,d),"Res Pt Dependence",0,0,1050,1050);
      cResPt[d][i]->Divide(3,3);

      cRErrPt[d][i] = new TCanvas(Form("cRErrPt_%i_%i",i,d),"Error Pt Dependence",0,0,1050,1050);
      cRErrPt[d][i]->Divide(3,3);
      cRPullPt[d][i] = new TCanvas(Form("cRPullPt_%i_%i",i,d),"Pull Pt Dependence",0,0,1050,1050);
      cRPullPt[d][i]->Divide(3,3);
      cRResPt[d][i] = new TCanvas(Form("cRResPt_%i_%i",i,d),"Res Pt Dependence",0,0,1050,1050);
      cRResPt[d][i]->Divide(3,3);
    }
  }

  // Draw Hists to Canvii
  TLegend* leg = new TLegend(.65,.65,.87,.87);
  leg->AddEntry(errorEta[0][0][0],legName[0],"l");
  leg->AddEntry(errorEta[1][0][0],legName[1],"l");
  leg->AddEntry(errorEta[2][0][0],legName[2],"l");
  TPaveText* lblE[nEtaBins]; 
  TPaveText* lblDet[3]; 
  if(DEBUG) cout << "at draw eta canvas" << endl;
  for(int det=0; det<3; det++)
  {
    char detLabel[100];
    char temp[10];
    lblDet[det] = new TPaveText(.48,.8,.63,.87,Form("NB NDC%i",det));
    sprintf(temp,detector[det]);
    sprintf(detLabel,"%s",temp);
    lblDet[det]->AddText(detLabel);
    lblDet[det]->SetFillColor(kWhite);

    for(int etabin=0; etabin<nEtaBins; etabin++)
    {
      // Init necessary plotting tools
      char textLabel[100];
      lblE[etabin] = new TPaveText(.67,.25,.85,.3,Form("NB NDC%i",etabin));
      sprintf(textLabel,"%.2f < #eta < %.2f",etaBinLow[etabin],etaBinHigh[etabin]);
      lblE[etabin]->AddText(textLabel);
      lblE[etabin]->SetFillColor(kWhite);

      int activeCanvas = (int)etabin/9;
      int activeBin = etabin - activeCanvas*9;
      //Accepted
      if(DEBUG) cout << "at eta err canvas" << endl;
      cErrEta[det][activeCanvas]->cd(activeBin+1);
      errorEta[0][det][etabin]->SetLineColor(kBlack);
      errorEta[1][det][etabin]->SetLineColor(kRed);
      errorEta[2][det][etabin]->SetLineColor(kBlue);
      Normalize(errorEta[0][det][etabin],errorEta[1][det][etabin],errorEta[2][det][etabin]);
      drawSort(cErrEta[det][activeCanvas],activeBin,errorEta[0][det][etabin],errorEta[1][det][etabin],errorEta[2][det][etabin],0);
      lblDet[det]->Draw("same");
      leg->Draw("same");

      //Rejected
      cRErrEta[det][activeCanvas]->cd(activeBin+1);
      RerrorEta[0][det][etabin]->SetLineColor(kBlack);
      RerrorEta[1][det][etabin]->SetLineColor(kRed);
      RerrorEta[2][det][etabin]->SetLineColor(kBlue);
      Normalize(RerrorEta[0][det][etabin],RerrorEta[1][det][etabin],RerrorEta[2][det][etabin]);
      drawSort(cRErrEta[det][activeCanvas],activeBin,RerrorEta[0][det][etabin],RerrorEta[1][det][etabin],RerrorEta[2][det][etabin],1);
      lblDet[det]->Draw("same");
      leg->Draw("same");

       //Accepted
      cResEta[det][activeCanvas]->cd(activeBin+1);
      resEta[0][det][etabin]->SetLineColor(kBlack);
      resEta[1][det][etabin]->SetLineColor(kRed);
      resEta[2][det][etabin]->SetLineColor(kBlue);
      Normalize(resEta[0][det][etabin],resEta[1][det][etabin],resEta[2][det][etabin]);
      drawSort(cResEta[det][activeCanvas],activeBin,resEta[0][det][etabin],resEta[1][det][etabin],resEta[2][det][etabin],0);
      lblDet[det]->Draw("same");
      leg->Draw("same");

      //Rejected
      cRResEta[det][activeCanvas]->cd(activeBin+1);
      RresEta[0][det][etabin]->SetLineColor(kBlack);
      RresEta[1][det][etabin]->SetLineColor(kRed);
      RresEta[2][det][etabin]->SetLineColor(kBlue);
      Normalize(RresEta[0][det][etabin],RresEta[1][det][etabin],RresEta[2][det][etabin]);
      drawSort(cRResEta[det][activeCanvas],activeBin,RresEta[0][det][etabin],RresEta[1][det][etabin],RresEta[2][det][etabin],1);
      lblDet[det]->Draw("same");
      leg->Draw("same");

      //Accepted
      cPullEta[det][activeCanvas]->cd(activeBin+1);
      pullEta[0][det][etabin]->SetLineColor(kBlack);
      pullEta[1][det][etabin]->SetLineColor(kRed);
      pullEta[2][det][etabin]->SetLineColor(kBlue);
      Normalize(pullEta[0][det][etabin],pullEta[1][det][etabin],pullEta[2][det][etabin]);
      drawSort(cPullEta[det][activeCanvas],activeBin,pullEta[0][det][etabin],pullEta[1][det][etabin],pullEta[2][det][etabin],0);
      lblDet[det]->Draw("same");
      leg->Draw("same");

      //Rejected
      cRPullEta[det][activeCanvas]->cd(activeBin+1);
      RpullEta[0][det][etabin]->SetLineColor(kBlack);
      RpullEta[1][det][etabin]->SetLineColor(kRed);
      RpullEta[2][det][etabin]->SetLineColor(kBlue);
      Normalize(RpullEta[0][det][etabin],RpullEta[1][det][etabin],RpullEta[2][det][etabin]);
      drawSort(cRPullEta[det][activeCanvas],activeBin,RpullEta[0][det][etabin],RpullEta[1][det][etabin],RpullEta[2][det][etabin],1);
      lblDet[det]->Draw("same");
      leg->Draw("same");
    }
  

    TPaveText* lblP[nPtBins]; 
    if(DEBUG) cout << "at draw pt canvas" << endl;
    for(int ptbin=0; ptbin<nPtBins; ptbin++)
    {
      // Init necessary plotting tools
      char textLabel[100];
      lblP[ptbin] = new TPaveText(.67,.25,.85,.3,Form("NB NDC%i",ptbin));
      sprintf(textLabel,"%.2f < #pt < %.2f",ptBinLow[ptbin],ptBinHigh[ptbin]);
      lblP[ptbin]->AddText(textLabel);
      lblP[ptbin]->SetFillColor(kWhite);

      int activeCanvas = (int)ptbin/9;
      int activeBin = ptbin - activeCanvas*9;

      //Accepted
      if(DEBUG) cout << "at eta err canvas" << endl;
      cErrPt[det][activeCanvas]->cd(activeBin+1);
      errorPt[0][det][ptbin]->SetLineColor(kBlack);
      errorPt[1][det][ptbin]->SetLineColor(kRed);
      errorPt[2][det][ptbin]->SetLineColor(kBlue);
      Normalize(errorPt[0][det][ptbin],errorPt[1][det][ptbin],errorPt[2][det][ptbin]);
      drawSort(cErrPt[det][activeCanvas],activeBin,errorPt[0][det][ptbin],errorPt[1][det][ptbin],errorPt[2][det][ptbin],0);
      lblDet[det]->Draw("same");
      leg->Draw("same");

      //Rejected
      cRErrPt[det][activeCanvas]->cd(activeBin+1);
      RerrorPt[0][det][ptbin]->SetLineColor(kBlack);
      RerrorPt[1][det][ptbin]->SetLineColor(kRed);
      RerrorPt[2][det][ptbin]->SetLineColor(kBlue);
      Normalize(RerrorPt[0][det][ptbin],RerrorPt[1][det][ptbin],RerrorPt[2][det][ptbin]);
      drawSort(cRErrPt[det][activeCanvas],activeBin,RerrorPt[0][det][ptbin],RerrorPt[1][det][ptbin],RerrorPt[2][det][ptbin],1);
      lblDet[det]->Draw("same");
      leg->Draw("same");

       //Accepted
      cResPt[det][activeCanvas]->cd(activeBin+1);
      resPt[0][det][ptbin]->SetLineColor(kBlack);
      resPt[1][det][ptbin]->SetLineColor(kRed);
      resPt[2][det][ptbin]->SetLineColor(kBlue);
      Normalize(resPt[0][det][ptbin],resPt[1][det][ptbin],resPt[2][det][ptbin]);
      drawSort(cResPt[det][activeCanvas],activeBin,resPt[0][det][ptbin],resPt[1][det][ptbin],resPt[2][det][ptbin],0);
      lblDet[det]->Draw("same");
      leg->Draw("same");

      //Rejected
      cRResPt[det][activeCanvas]->cd(activeBin+1);
      RresPt[0][det][ptbin]->SetLineColor(kBlack);
      RresPt[1][det][ptbin]->SetLineColor(kRed);
      RresPt[2][det][ptbin]->SetLineColor(kBlue);
      Normalize(RresPt[0][det][ptbin],RresPt[1][det][ptbin],RresPt[2][det][ptbin]);
      drawSort(cRResPt[det][activeCanvas],activeBin,RresPt[0][det][ptbin],RresPt[1][det][ptbin],RresPt[2][det][ptbin],1);
      lblDet[det]->Draw("same");
      leg->Draw("same");

      //Accepted
      cPullPt[det][activeCanvas]->cd(activeBin+1);
      pullPt[0][det][ptbin]->SetLineColor(kBlack);
      pullPt[1][det][ptbin]->SetLineColor(kRed);
      pullPt[2][det][ptbin]->SetLineColor(kBlue);
      Normalize(pullPt[0][det][ptbin],pullPt[1][det][ptbin],pullPt[2][det][ptbin]);
      drawSort(cPullPt[det][activeCanvas],activeBin,pullPt[0][det][ptbin],pullPt[1][det][ptbin],pullPt[2][det][ptbin],0);
      lblDet[det]->Draw("same");
      leg->Draw("same");

      //Rejected
      cRPullPt[det][activeCanvas]->cd(activeBin+1);
      RpullPt[0][det][ptbin]->SetLineColor(kBlack);
      RpullPt[1][det][ptbin]->SetLineColor(kRed);
      RpullPt[2][det][ptbin]->SetLineColor(kBlue);
      Normalize(RpullPt[0][det][ptbin],RpullPt[1][det][ptbin],RpullPt[2][det][ptbin]);
      drawSort(cRPullPt[det][activeCanvas],activeBin,RpullPt[0][det][ptbin],RpullPt[1][det][ptbin],RpullPt[2][det][ptbin],1);
      lblDet[det]->Draw("same");
      leg->Draw("same");
    }
  }

  if(DEBUG) cout << "In makePDF()" << endl;
  //Set front page
  TCanvas* fp = new TCanvas("fp","Front Page",0,0,1050,1050);
  fp->cd();
  TBox *bLabel = new TBox(0.01, 0.88, 0.99, 0.99);
  bLabel->SetFillColor(38);
  bLabel->Draw();
  TLatex tl;
  tl.SetNDC();
  tl.SetTextColor(kWhite);
  tl.SetTextSize(0.033);
  char tlName[100];
  char tlName2[100];
  char FileName[100];
  char FileName2[100];
  char FileName3[100];
  char FileName4[100];
  sprintf(FileName2,legName[0]);
  sprintf(FileName3,legName[1]);
  sprintf(FileName4,legName[2]);
  sprintf(FileName,"%s_vs_%s_vs_%s",FileName2,FileName3,FileName4); //convert string to char for later

  TString titlename = FileName;
  int found = titlename.Last('/');
  if(found >= 0){
    titlename.Replace(0, found+1, "");
  } 
  sprintf(tlName, "Sti Error Analyzer");
  tl.SetTextSize(0.05);
  tl.SetTextColor(kWhite);
  tl.DrawLatex(0.05, 0.92,tlName);

  TBox *bFoot = new TBox(0.01, 0.01, 0.99, 0.12);
  bFoot->SetFillColor(38);
  bFoot->Draw();
  tl.SetTextColor(kWhite);
  tl.SetTextSize(0.05);
  tl.DrawLatex(0.05, 0.05, (new TDatime())->AsString());
  tl.SetTextColor(kBlack);
  tl.SetTextSize(0.03);
  tl.DrawLatex(0.1, 0.14, titlename);
  sprintf(tlName,"STI Tracking Error Analysis");
  tl.DrawLatex(0.1, 0.75,tlName);
  sprintf(tlName,"Z.W. Miller - UIC");
  tl.DrawLatex(0.1, 0.67,tlName);


  // Place canvases in order
  TCanvas* temp = new TCanvas();
  char name[100];
  sprintf(name, "%s.pdf[", FileName);
  temp->Print(name);
  sprintf(name, "%s.pdf", FileName);
  temp = fp; // print front page
  temp->Print(name);
  for(int det=0; det<3; det++)
  {
   /* for(int q=0; q<numEtaCanvas; q++)
    {
      temp = cResEta[det][q];
      temp->Print(name);
      temp = cErrEta[det][q];
      temp->Print(name);
      temp = cPullEta[det][q];
      temp->Print(name);
    }
    for(int q=0; q<numEtaCanvas; q++)
    {
      // temp = cRResEta[det][q];
      // temp->Print(name);
      temp = cRErrEta[det][q];
      temp->Print(name);
      // temp = cRPullEta[det][q];
      // temp->Print(name);
    }*/

    for(int q=0; q<numPtCanvas; q++)
    {
      temp = cResPt[det][q];
      temp->Print(name);
      temp = cErrPt[det][q];
      temp->Print(name);
      temp = cPullPt[det][q];
      temp->Print(name);
    }
    for(int q=0; q<numPtCanvas; q++)
    {
      //temp = cRResPt[det][q];
      //temp->Print(name);
      temp = cRErrPt[det][q];
      temp->Print(name);
      //temp = cRPullPt[det][q];
      //temp->Print(name);
    }
  }
  sprintf(name, "%s.pdf]", FileName);
  temp->Print(name);


  return 1;
}

void drawSort(TCanvas* can, int activeB, TH1F* a, TH1F* b, TH1F* c, int AccOrRej)
{
  TString Label;
  Label += a->GetTitle();
  if(AccOrRej)
    Label += ": Rejected";
  if(!AccOrRej)
    Label += ": Accepted";

  int ai = a->GetMaximum();
  int bi = b->GetMaximum();
  int ci = c->GetMaximum();
  int isLogY = 1;
  can->cd(activeB+1);
  gPad->SetLogy(isLogY);
  if(a >= b && a >= c)
  {
    a->SetTitle(Label);
    a->Draw();
    b->Draw("same");
    c->Draw("same");
  }
  else if(b >= a && b >= c)
  {
    b->SetTitle(Label);
    b->Draw();
    a->Draw("same");
    c->Draw("same");
  }
  else if(c >= a && c >= b)
  {
    c->SetTitle(Label);
    c->Draw();
    a->Draw("same");
    b->Draw("same");
  }
}

void Normalize(TH1F* hist0, TH1F* hist1, TH1F* hist2)
{
  double norm[3] = {0.};
  norm[0] = hist0->Integral();
  norm[1] = hist1->Integral();
  norm[2] = hist2->Integral();
  hist0->Scale(1./norm[0]);
  hist1->Scale(1./norm[1]);
  hist2->Scale(1./norm[2]);
  // Change Y-axis label to reflect
  hist0->GetYaxis()->SetTitle("1/N_{tot} * Counts");
  hist1->GetYaxis()->SetTitle("1/N_{tot} * Counts");
  hist2->GetYaxis()->SetTitle("1/N_{tot} * Counts");
}
