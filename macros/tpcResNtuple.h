//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul 31 10:12:49 2015 by ROOT version 5.34/30
// from TTree nt/nt
// found on file: piplus_160.tpcRes.root
//////////////////////////////////////////////////////////

#define tpcResNtuple_cxx
#ifndef tpcResNtuple_h
#define tpcResNtuple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class tpcResNtuple {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         pt;
   Float_t         phi;
   Float_t         eta;
   Float_t         geantId;
   Float_t         svx;
   Float_t         svy;
   Float_t         svz;
   Float_t         nCommon;
   Float_t         nFit;
   Float_t         nMax;
   Float_t         gPt;
   Float_t         gPhi;
   Float_t         gEta;
   Float_t         dca;
   Float_t         dcaXY;
   Float_t         dcaZ;
   Float_t         hftTopo;
   Float_t         isTrueHft;

   // List of branches
   TBranch        *b_pt;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_geantId;   //!
   TBranch        *b_svx;   //!
   TBranch        *b_svy;   //!
   TBranch        *b_svz;   //!
   TBranch        *b_nCommon;   //!
   TBranch        *b_nFit;   //!
   TBranch        *b_nMax;   //!
   TBranch        *b_gPt;   //!
   TBranch        *b_gPhi;   //!
   TBranch        *b_gEta;   //!
   TBranch        *b_dca;   //!
   TBranch        *b_dcaXY;   //!
   TBranch        *b_dcaZ;   //!
   TBranch        *b_hftTopo;   //!
   TBranch        *b_isTrueHft;   //!

   tpcResNtuple(TTree *tree=0);
   virtual ~tpcResNtuple();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t GetEntries() { return fChain->GetEntries();}
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tpcResNtuple_cxx
tpcResNtuple::tpcResNtuple(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("piplus_160.tpcRes.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("piplus_160.tpcRes.root");
      }
      f->GetObject("nt",tree);

   }
   Init(tree);
}

tpcResNtuple::~tpcResNtuple()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tpcResNtuple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tpcResNtuple::LoadTree(Long64_t entry)
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

void tpcResNtuple::Init(TTree *tree)
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

   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("phi", &phi, &b_phi);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("geantId", &geantId, &b_geantId);
   fChain->SetBranchAddress("svx", &svx, &b_svx);
   fChain->SetBranchAddress("svy", &svy, &b_svy);
   fChain->SetBranchAddress("svz", &svz, &b_svz);
   fChain->SetBranchAddress("nCommon", &nCommon, &b_nCommon);
   fChain->SetBranchAddress("nFit", &nFit, &b_nFit);
   fChain->SetBranchAddress("nMax", &nMax, &b_nMax);
   fChain->SetBranchAddress("gPt", &gPt, &b_gPt);
   fChain->SetBranchAddress("gPhi", &gPhi, &b_gPhi);
   fChain->SetBranchAddress("gEta", &gEta, &b_gEta);
   fChain->SetBranchAddress("dca", &dca, &b_dca);
   fChain->SetBranchAddress("dcaXY", &dcaXY, &b_dcaXY);
   fChain->SetBranchAddress("dcaZ", &dcaZ, &b_dcaZ);
   fChain->SetBranchAddress("hftTopo", &hftTopo, &b_hftTopo);
   fChain->SetBranchAddress("isTrueHft", &isTrueHft, &b_isTrueHft);
   Notify();
}

Bool_t tpcResNtuple::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tpcResNtuple::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tpcResNtuple::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tpcResNtuple_cxx
