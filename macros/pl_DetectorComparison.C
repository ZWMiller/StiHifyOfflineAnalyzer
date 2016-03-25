#include "anaConstEmbed.h"
void drawSort(TCanvas*, int, TH1F*, TH1F*, TH1F*);


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

  if(DEBUG) cout << "at get hists" << endl;
  for(int det=0; det<3; det++)
  {
    for(int n=0; n<nEtaBins; n++)
    {
      errorEta[det][n] = (TH1F*)f[det]->Get(Form("errorEtaAcc_%i",n));
      resEta[det][n] = (TH1F*)f[det]->Get(Form("resEtaAcc_%i",n));
      pullEta[det][n] = (TH1F*)f[det]->Get(Form("pullEtaAcc_%i",n));
    }

    for(int n=0; n<nPtBins; n++)
    {
      errorPt[det][n] = (TH1F*)f[det]->Get(Form("errorPtAcc_%i",n));
      resPt[det][n] = (TH1F*)f[det]->Get(Form("resPtAcc_%i",n));
      pullPt[det][n] = (TH1F*)f[det]->Get(Form("pullPtAcc_%i",n));
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
  if(DEBUG) cout << "at make canvas" << endl;
  for(int i = 0; i < numEtaCanvas; i++)
  {
    cErrEta[i] = new TCanvas(Form("cErrEta_%i",i),"Error Eta Dependence",0,0,1050,1050);
    cErrEta[i]->Divide(3,3);
    cPullEta[i] = new TCanvas(Form("cPullEta_%i",i),"Pull Eta Dependence",0,0,1050,1050);
    cPullEta[i]->Divide(3,3);
    cResEta[i] = new TCanvas(Form("cResEta_%i",i),"Res Eta Dependence",0,0,1050,1050);
    cResEta[i]->Divide(3,3);
  }
  for(int i = 0; i < numPtCanvas; i++)
  {
    cErrPt[i] = new TCanvas(Form("cErrPt_%i",i),"Error Pt Dependence",0,0,1050,1050);
    cErrPt[i]->Divide(3,3);
    cPullPt[i] = new TCanvas(Form("cPullPt_%i",i),"Pull Pt Dependence",0,0,1050,1050);
    cPullPt[i]->Divide(3,3);
    cResPt[i] = new TCanvas(Form("cResPt_%i",i),"Res Pt Dependence",0,0,1050,1050);
    cResPt[i]->Divide(3,3);
  }


  // Draw Hists to Canvii
  TLegend* leg = new TLegend(.55,.55,.87,.85);
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

    if(DEBUG) cout << "at eta err canvas" << endl;
    cErrEta[activeCanvas]->cd(activeBin+1);
    errorEta[0][etabin]->SetLineColor(kBlack);
    errorEta[1][etabin]->SetLineColor(kBlue);
    errorEta[2][etabin]->SetLineColor(kRed);
    drawSort(cErrEta[activeCanvas],activeBin,errorEta[0][etabin],errorEta[1][etabin],errorEta[2][etabin]);
    leg->Draw("same");

    if(DEBUG) cout << "at eta res canvas" << endl;
    cResEta[activeCanvas]->cd(activeBin+1);
    resEta[0][etabin]->SetLineColor(kBlack);
    resEta[1][etabin]->SetLineColor(kBlue);
    resEta[2][etabin]->SetLineColor(kRed);
    drawSort(cResEta[activeCanvas],activeBin,resEta[0][etabin],resEta[1][etabin],resEta[2][etabin]);
    leg->Draw("same");

    if(DEBUG) cout << "at eta pull canvas" << endl;
    cPullEta[activeCanvas]->cd(activeBin+1);
    pullEta[0][etabin]->SetLineColor(kBlack);
    pullEta[1][etabin]->SetLineColor(kBlue);
    pullEta[2][etabin]->SetLineColor(kRed);
    drawSort(cPullEta[activeCanvas],activeBin,pullEta[0][etabin],pullEta[1][etabin],pullEta[2][etabin]);
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

    cErrPt[activeCanvas]->cd(activeBin+1);
    errorPt[0][ptbin]->SetLineColor(kBlack);
    errorPt[1][ptbin]->SetLineColor(kBlue);
    errorPt[2][ptbin]->SetLineColor(kRed);
    drawSort(cErrPt[activeCanvas],activeBin,errorPt[0][ptbin],errorPt[1][ptbin],errorPt[2][ptbin]);
    leg->Draw("same");

    cResPt[activeCanvas]->cd(activeBin+1);
    resPt[0][ptbin]->SetLineColor(kBlack);
    resPt[1][ptbin]->SetLineColor(kBlue);
    resPt[2][ptbin]->SetLineColor(kRed);
    drawSort(cResPt[activeCanvas],activeBin,resPt[0][ptbin],resPt[1][ptbin],resPt[2][ptbin]);
    leg->Draw("same");

    cPullPt[activeCanvas]->cd(activeBin+1);
    pullPt[0][ptbin]->SetLineColor(kBlack);
    pullPt[1][ptbin]->SetLineColor(kBlue);
    pullPt[2][ptbin]->SetLineColor(kRed);
    drawSort(cPullPt[activeCanvas],activeBin,pullPt[0][ptbin],pullPt[1][ptbin],pullPt[2][ptbin]);
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
  sprintf(name, "%s.pdf]", FileName);
  temp->Print(name);


  return 1;
}

void drawSort(TCanvas* can, int activeB, TH1F* a, TH1F* b, TH1F* c)
{
  int ai = a->GetMaximum();
  int bi = b->GetMaximum();
  int ci = c->GetMaximum();
  int isLogY = 1;
  can->cd(activeB+1);
  gPad->SetLogy(isLogY);
  if(a >= b && a >= c)
  {
    a->Draw();
    b->Draw("same");
    c->Draw("same");
  }
  else if(b >= a && b >= c)
  {
    b->Draw();
    a->Draw("same");
    c->Draw("same");
  }
  else if(c >= b && c >= a)
  {
    c->Draw();
    b->Draw("same");
    a->Draw("same");
  }
}
