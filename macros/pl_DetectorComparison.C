#include "anaConstEmbed.h"
void drawSort(TCanvas*, int, TH1F*, TH1F*, TH1F*, int);
void Normalize(TH1F*, TH1F*, TH1F*);


int pl_DetectorComparison(TString baseName = "test.root"){

  bool DEBUG = kFALSE;
  TString detector[3] = {"IST","PXL1","PXL2"};
  baseName.ReplaceAll(".root","");


  // Open Files
  TFile* f[3];
  TString fileName;
  if(DEBUG) cout << "at open file" << endl;
  for (int det=0; det<3; det++){
    fileName = "";
    fileName += baseName + "." + detector[det] + ".processed.root";
    f[det] = new TFile(fileName,"READ");
    if(!f[det]->IsOpen())
    {
      cout << "Failed to open " << detector[det] << " file. Aborting." << endl;
      cout << "Input file name should not have *.DETNAME.root, only *.root" << endl;
      exit(EXIT_FAILURE);
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


  TH1F* errorEta[3][nEtaBins];
  TH1F* errorPt[3][nPtBins];
  TH1F* resEta[3][nEtaBins];
  TH1F* resPt[3][nPtBins];
  TH1F* pullEta[3][nEtaBins];
  TH1F* pullPt[3][nPtBins];
  TH1F* RerrorEta[3][nEtaBins];
  TH1F* RerrorPt[3][nPtBins];
  TH1F* RresEta[3][nEtaBins];
  TH1F* RresPt[3][nPtBins];
  TH1F* RpullEta[3][nEtaBins];
  TH1F* RpullPt[3][nPtBins];

  if(DEBUG) cout << "at get hists" << endl;
  for(int det=0; det<3; det++)
  {
    for(int n=0; n<nEtaBins; n++)
    {
      errorEta[det][n] = (TH1F*)f[det]->Get(Form("errorEtaAcc_%i",n));
      resEta[det][n] = (TH1F*)f[det]->Get(Form("resEtaAcc_%i",n));
      pullEta[det][n] = (TH1F*)f[det]->Get(Form("pullEtaAcc_%i",n));

      RerrorEta[det][n] = (TH1F*)f[det]->Get(Form("errorEtaRej_%i",n));
      RresEta[det][n] = (TH1F*)f[det]->Get(Form("resEtaRej_%i",n));
      RpullEta[det][n] = (TH1F*)f[det]->Get(Form("pullEtaRej_%i",n));
    }

    for(int n=0; n<nPtBins; n++)
    {
      errorPt[det][n] = (TH1F*)f[det]->Get(Form("errorPtAcc_%i",n));
      resPt[det][n] = (TH1F*)f[det]->Get(Form("resPtAcc_%i",n));
      pullPt[det][n] = (TH1F*)f[det]->Get(Form("pullPtAcc_%i",n));

      RerrorPt[det][n] = (TH1F*)f[det]->Get(Form("errorPtRej_%i",n));
      RresPt[det][n] = (TH1F*)f[det]->Get(Form("resPtRej_%i",n));
      RpullPt[det][n] = (TH1F*)f[det]->Get(Form("pullPtRej_%i",n));
    }
  }

  // Make Canvii
  const int numPtCanvas = nPtBins/9 + 1;
  const int numEtaCanvas = nEtaBins/9 + 1;
  TCanvas* cErrEta[numEtaCanvas];
  TCanvas* cResEta[numEtaCanvas];
  TCanvas* cPullEta[numEtaCanvas];
  TCanvas* cErrPt[numPtCanvas];
  TCanvas* cResPt[numPtCanvas];
  TCanvas* cPullPt[numPtCanvas];
  TCanvas* cRErrEta[numEtaCanvas];
  TCanvas* cRResEta[numEtaCanvas];
  TCanvas* cRPullEta[numEtaCanvas];
  TCanvas* cRErrPt[numPtCanvas];
  TCanvas* cRResPt[numPtCanvas];
  TCanvas* cRPullPt[numPtCanvas];
  if(DEBUG) cout << "at make canvas" << endl;
  for(int i = 0; i < numEtaCanvas; i++)
  {
    cErrEta[i] = new TCanvas(Form("cErrEta_%i",i),"Error Eta Dependence",0,0,1050,1050);
    cErrEta[i]->Divide(3,3);
    cPullEta[i] = new TCanvas(Form("cPullEta_%i",i),"Pull Eta Dependence",0,0,1050,1050);
    cPullEta[i]->Divide(3,3);
    cResEta[i] = new TCanvas(Form("cResEta_%i",i),"Res Eta Dependence",0,0,1050,1050);
    cResEta[i]->Divide(3,3);

    cRErrEta[i] = new TCanvas(Form("cRErrEta_%i",i),"Error Eta Dependence",0,0,1050,1050);
    cRErrEta[i]->Divide(3,3);
    cRPullEta[i] = new TCanvas(Form("cRPullEta_%i",i),"Pull Eta Dependence",0,0,1050,1050);
    cRPullEta[i]->Divide(3,3);
    cRResEta[i] = new TCanvas(Form("cRResEta_%i",i),"Res Eta Dependence",0,0,1050,1050);
    cRResEta[i]->Divide(3,3);
  }
  for(int i = 0; i < numPtCanvas; i++)
  {
    cErrPt[i] = new TCanvas(Form("cErrPt_%i",i),"Error Pt Dependence",0,0,1050,1050);
    cErrPt[i]->Divide(3,3);
    cPullPt[i] = new TCanvas(Form("cPullPt_%i",i),"Pull Pt Dependence",0,0,1050,1050);
    cPullPt[i]->Divide(3,3);
    cResPt[i] = new TCanvas(Form("cResPt_%i",i),"Res Pt Dependence",0,0,1050,1050);
    cResPt[i]->Divide(3,3);

    cRErrPt[i] = new TCanvas(Form("cRErrPt_%i",i),"Error Pt Dependence",0,0,1050,1050);
    cRErrPt[i]->Divide(3,3);
    cRPullPt[i] = new TCanvas(Form("cRPullPt_%i",i),"Pull Pt Dependence",0,0,1050,1050);
    cRPullPt[i]->Divide(3,3);
    cRResPt[i] = new TCanvas(Form("cRResPt_%i",i),"Res Pt Dependence",0,0,1050,1050);
    cRResPt[i]->Divide(3,3);
  }


  // Draw Hists to Canvii
  TLegend* leg = new TLegend(.65,.65,.87,.90);
  leg->AddEntry(errorEta[0][0],"IST","l");
  leg->AddEntry(errorEta[2][0],"PXL2","l");
  leg->AddEntry(errorEta[1][0],"PXL1","l");
  TPaveText* lblE[nEtaBins]; 
  if(DEBUG) cout << "at draw eta canvas" << endl;
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
    cErrEta[activeCanvas]->cd(activeBin+1);
    errorEta[0][etabin]->SetLineColor(kBlack);
    errorEta[1][etabin]->SetLineColor(kBlue);
    errorEta[2][etabin]->SetLineColor(kRed);
    Normalize(errorEta[0][etabin],errorEta[1][etabin],errorEta[2][etabin]);
    drawSort(cErrEta[activeCanvas],activeBin,errorEta[0][etabin],errorEta[1][etabin],errorEta[2][etabin],0);
    leg->Draw("same");

    //Rejected
    cRErrEta[activeCanvas]->cd(activeBin+1);
    RerrorEta[0][etabin]->SetLineColor(kBlack);
    RerrorEta[1][etabin]->SetLineColor(kBlue);
    RerrorEta[2][etabin]->SetLineColor(kRed);
    Normalize(RerrorEta[0][etabin],RerrorEta[1][etabin],RerrorEta[2][etabin]);
    drawSort(cRErrEta[activeCanvas],activeBin,RerrorEta[0][etabin],RerrorEta[1][etabin],RerrorEta[2][etabin],1);
    leg->Draw("same");

    //Accepted
    if(DEBUG) cout << "at eta res canvas" << endl;
    cResEta[activeCanvas]->cd(activeBin+1);
    resEta[0][etabin]->SetLineColor(kBlack);
    resEta[1][etabin]->SetLineColor(kBlue);
    resEta[2][etabin]->SetLineColor(kRed);
    Normalize(resEta[0][etabin],resEta[1][etabin],resEta[2][etabin]);
    drawSort(cResEta[activeCanvas],activeBin,resEta[0][etabin],resEta[1][etabin],resEta[2][etabin],0);
    leg->Draw("same");

    //Rejected
    cRResEta[activeCanvas]->cd(activeBin+1);
    RresEta[0][etabin]->SetLineColor(kBlack);
    RresEta[1][etabin]->SetLineColor(kBlue);
    RresEta[2][etabin]->SetLineColor(kRed);
    Normalize(RresEta[0][etabin],RresEta[1][etabin],RresEta[2][etabin]);
    drawSort(cRResEta[activeCanvas],activeBin,RresEta[0][etabin],RresEta[1][etabin],RresEta[2][etabin],1);
    leg->Draw("same");

    //Accepted
    if(DEBUG) cout << "at eta pull canvas" << endl;
    cPullEta[activeCanvas]->cd(activeBin+1);
    pullEta[0][etabin]->SetLineColor(kBlack);
    pullEta[1][etabin]->SetLineColor(kBlue);
    pullEta[2][etabin]->SetLineColor(kRed);
    Normalize(pullEta[0][etabin],pullEta[1][etabin],pullEta[2][etabin]);
    drawSort(cPullEta[activeCanvas],activeBin,pullEta[0][etabin],pullEta[1][etabin],pullEta[2][etabin],0);
    leg->Draw("same");
    
    //Rejected
    cRPullEta[activeCanvas]->cd(activeBin+1);
    RpullEta[0][etabin]->SetLineColor(kBlack);
    RpullEta[1][etabin]->SetLineColor(kBlue);
    RpullEta[2][etabin]->SetLineColor(kRed);
    Normalize(RpullEta[0][etabin],RpullEta[1][etabin],RpullEta[2][etabin]);
    drawSort(cRPullEta[activeCanvas],activeBin,RpullEta[0][etabin],RpullEta[1][etabin],RpullEta[2][etabin],1);
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
    cErrPt[activeCanvas]->cd(activeBin+1);
    errorPt[0][ptbin]->SetLineColor(kBlack);
    errorPt[1][ptbin]->SetLineColor(kBlue);
    errorPt[2][ptbin]->SetLineColor(kRed);
    Normalize(errorPt[0][ptbin],errorPt[1][ptbin],errorPt[2][ptbin]);
    drawSort(cErrPt[activeCanvas],activeBin,errorPt[0][ptbin],errorPt[1][ptbin],errorPt[2][ptbin],0);
    leg->Draw("same");

    //Rejected
    cRErrPt[activeCanvas]->cd(activeBin+1);
    RerrorPt[0][ptbin]->SetLineColor(kBlack);
    RerrorPt[1][ptbin]->SetLineColor(kBlue);
    RerrorPt[2][ptbin]->SetLineColor(kRed);
    Normalize(RerrorPt[0][ptbin],RerrorPt[1][ptbin],RerrorPt[2][ptbin]);
    drawSort(cRErrPt[activeCanvas],activeBin,RerrorPt[0][ptbin],RerrorPt[1][ptbin],RerrorPt[2][ptbin],1);
    leg->Draw("same");

    //Accepted
    cResPt[activeCanvas]->cd(activeBin+1);
    resPt[0][ptbin]->SetLineColor(kBlack);
    resPt[1][ptbin]->SetLineColor(kBlue);
    resPt[2][ptbin]->SetLineColor(kRed);
    Normalize(resPt[0][ptbin],resPt[1][ptbin],resPt[2][ptbin]);
    drawSort(cResPt[activeCanvas],activeBin,resPt[0][ptbin],resPt[1][ptbin],resPt[2][ptbin],0);
    leg->Draw("same");

    //Rejected
    cRResPt[activeCanvas]->cd(activeBin+1);
    RresPt[0][ptbin]->SetLineColor(kBlack);
    RresPt[1][ptbin]->SetLineColor(kBlue);
    RresPt[2][ptbin]->SetLineColor(kRed);
    Normalize(RresPt[0][ptbin],RresPt[1][ptbin],RresPt[2][ptbin]);
    drawSort(cRResPt[activeCanvas],activeBin,RresPt[0][ptbin],RresPt[1][ptbin],RresPt[2][ptbin],1);
    leg->Draw("same");

    //Accepted
    cPullPt[activeCanvas]->cd(activeBin+1);
    pullPt[0][ptbin]->SetLineColor(kBlack);
    pullPt[1][ptbin]->SetLineColor(kBlue);
    pullPt[2][ptbin]->SetLineColor(kRed);
    Normalize(pullPt[0][ptbin],pullPt[1][ptbin],pullPt[2][ptbin]);
    drawSort(cPullPt[activeCanvas],activeBin,pullPt[0][ptbin],pullPt[1][ptbin],pullPt[2][ptbin],0);
    leg->Draw("same");

    //Rejected
    cRPullPt[activeCanvas]->cd(activeBin+1);
    RpullPt[0][ptbin]->SetLineColor(kBlack);
    RpullPt[1][ptbin]->SetLineColor(kBlue);
    RpullPt[2][ptbin]->SetLineColor(kRed);
    Normalize(RpullPt[0][ptbin],RpullPt[1][ptbin],RpullPt[2][ptbin]);
    drawSort(cRPullPt[activeCanvas],activeBin,RpullPt[0][ptbin],RpullPt[1][ptbin],RpullPt[2][ptbin],1);
    leg->Draw("same");
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
  sprintf(FileName2,baseName);
  sprintf(FileName,"%s_Combined",FileName2); //convert string to char for later

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
  for(int q=0; q<numEtaCanvas; q++)
  {
    temp = cResEta[q];
    temp->Print(name);
    temp = cErrEta[q];
    temp->Print(name);
    temp = cPullEta[q];
    temp->Print(name);
  }
  for(int q=0; q<numPtCanvas; q++)
  {
    temp = cResPt[q];
    temp->Print(name);
    temp = cErrPt[q];
    temp->Print(name);
    temp = cPullPt[q];
    temp->Print(name);
  }
  for(int q=0; q<numEtaCanvas; q++)
  {
    temp = cRResEta[q];
    temp->Print(name);
    temp = cRErrEta[q];
    temp->Print(name);
    temp = cRPullEta[q];
    temp->Print(name);
  }
  for(int q=0; q<numPtCanvas; q++)
  {
    temp = cRResPt[q];
    temp->Print(name);
    temp = cRErrPt[q];
    temp->Print(name);
    temp = cRPullPt[q];
    temp->Print(name);
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
  else if(c >= b && c >= a)
  {
    c->SetTitle(Label);
    c->Draw();
    b->Draw("same");
    a->Draw("same");
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
