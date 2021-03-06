//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb 17 09:11:47 2016 by ROOT version 6.04/00
// from TTree pullAnaTree/Tree for Pull Error Analysis
// found on file: st_physics_adc_15107089_raw_5000010.daq.stihify.hist.root
//////////////////////////////////////////////////////////

#ifndef StiAnalyzer_h
#define StiAnalyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string>

// Header file for the classes stored in the TTree if any.

class StiAnalyzer {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         AnyHit_errorMag;
   Float_t         AnyHit_pull;
   Float_t         AnyHit_residual;
   Float_t         AnyHit_eta;
   Float_t         AnyHit_pT;
   Float_t         AnyHit_phi;
   Float_t         AnyHit_z;
   Float_t         AcceptedHit_errorMag;
   Float_t         AcceptedHit_pull;
   Float_t         AcceptedHit_residual;
   Float_t         AcceptedHit_eta;
   Float_t         AcceptedHit_pT;
   Float_t         AcceptedHit_phi;
   Float_t         AcceptedHit_z;
   Float_t         RejectedHit_errorMag;
   Float_t         RejectedHit_pull;
   Float_t         RejectedHit_residual;
   Float_t         RejectedHit_eta;
   Float_t         RejectedHit_pT;
   Float_t         RejectedHit_phi;
   Float_t         RejectedHit_z;

   // List of branches
   TBranch        *b_AnyHit;   //!
   TBranch        *b_AcceptedHit;   //!
   TBranch        *b_RejectedHit;   //!

   StiAnalyzer(TTree *tree=0);
   virtual ~StiAnalyzer();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   // Set Cut Values
   void setBinningValues();
   int numEtaBins, numPtBins;
   float etaBinLow[50], etaBinHigh[50];
   float ptBinLow[50], ptBinHigh[50];

   // TCanvas 
   int numPtCanvas;
   int numEtaCanvas;
   TCanvas* errorSumTest;
   TCanvas* cEffPlots;
   TCanvas* cErrEta[10];
   TCanvas* cErrPt[10];
   TCanvas* cPullEta[10];
   TCanvas* cPullPt[10];
   TCanvas* cResEta[10];
   TCanvas* cResPt[10];

   // Labels
   TPaveText* lblE[20];
   TPaveText* lblP[20];

   // Histograms (use vectors for pt/eta binning)
   TH1F* errorAcc;
   TH1F* errorRej;
   TH1F* errorAny;
   TH1F* errorEtaAcc[20];
   TH1F* errorPtAcc[20];
   TH1F* errorEtaRej[20];
   TH1F* errorPtRej[20];
   TH1F* pullEtaAcc[20];
   TH1F* pullPtAcc[20];
   TH1F* pullEtaRej[20];
   TH1F* pullPtRej[20];
   TH1F* resEtaAcc[20];
   TH1F* resPtAcc[20];
   TH1F* resEtaRej[20];
   TH1F* resPtRej[20];

   //TGraphObjects
   int ptCounter,etaCounter;
   float etaEff[50], ptEff[50], etaVal[50], ptVal[50];
   TGraphErrors* efficiency_pt;
   TGraphErrors* efficiency_eta;

   // Functions for managing objects
   void BookCanvas();
   void BookHistograms();
   void BookTGraphs();

   // Functions for Filling Histograms
   void FillHistograms();
   void etaBinLoop(); // Loop through eta Bins and fill binned histos
   void ptBinLoop();  // Loop through pT Bins and fill binned histos
   void sumTest();    // Make sure Accepted and Rejected Branches Sum to "Any" Branc
   void makeEfficiencyPlots(); // Integrate objects to see what I have
   void makeEtaEfficiency();
   void makePtEfficiency();

   // Functions for Drawing on Canvii
   void drawToCanvas();
   void makeErrorSumTest();
   void drawEtaHistograms();
   void drawPtHistograms();
   void drawEfficiency();

   // Functional Procedures
   void MakePDF();

   // Input/Output
   void makeOutFile();
   void writeOutFile();
   void GetHifyFile(TString);
   
private:

   TFile* outFile;
   string fileName;
};

#endif

#ifdef StiAnalyzer_cxx
StiAnalyzer::StiAnalyzer(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
 /*  if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("st_physics_adc_15107089_raw_5000010.daq.stihify.hist.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("st_physics_adc_15107089_raw_5000010.daq.stihify.hist.root");
      }
      f->GetObject("pullAnaTree",tree);

   }
   Init(tree);*/
}

StiAnalyzer::~StiAnalyzer()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t StiAnalyzer::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

void StiAnalyzer::GetHifyFile(TString name){

// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  TTree* tree;
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(name);
  if (!f || !f->IsOpen()) {
    f = new TFile(name);
  }
  fileName = name;
  f->GetObject("pullAnaTree",tree);
  Init(tree);
}

Long64_t StiAnalyzer::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void StiAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("AnyHit", &AnyHit_errorMag, &b_AnyHit);
   fChain->SetBranchAddress("AcceptedHit", &AcceptedHit_errorMag, &b_AcceptedHit);
   fChain->SetBranchAddress("RejectedHit", &RejectedHit_errorMag, &b_RejectedHit);
   Notify();
}

Bool_t StiAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void StiAnalyzer::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t StiAnalyzer::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef StiAnalyzer_cxx
