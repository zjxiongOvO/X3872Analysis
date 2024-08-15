//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug  8 16:32:23 2024 by ROOT version 6.30/01
// from TTree tree/tree
// found on file: X3872_JpsiRho.root
//////////////////////////////////////////////////////////

#ifndef EvtgenSim_h
#define EvtgenSim_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class EvtgenSim {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        X3872Pt;
   Double_t        X3872Eta;
   Double_t        X3872Phi;
   Double_t        X3872Mass;
   Double_t        DeltaR1;
   Double_t        DeltaR2;
   Double_t        Q;
   Double_t        CosMotherAndDaughter_Lab;
   Double_t        CosMotherAndDaughter_CMS;
   Double_t        JpsiPt;
   Double_t        JpsiEta;
   Double_t        JpsiPhi;
   Double_t        JpsiMass;
   Double_t        PiPlusPt;
   Double_t        PiPlusEta;
   Double_t        PiPlusPhi;
   Double_t        PiPlusMass;
   Double_t        PiMinusPt;
   Double_t        PiMinusEta;
   Double_t        PiMinusPhi;
   Double_t        PiMinusMass;
   Double_t        EPlusPt;
   Double_t        EPlusEta;
   Double_t        EPlusPhi;
   Double_t        EPlusMass;
   Double_t        EMinusPt;
   Double_t        EMinusEta;
   Double_t        EMinusPhi;
   Double_t        EMinusMass;

   // List of branches
   TBranch        *b_X3872Pt;   //!
   TBranch        *b_X3872Eta;   //!
   TBranch        *b_X3872Phi;   //!
   TBranch        *b_X3872Mass;   //!
   TBranch        *b_DeltaR1;   //!
   TBranch        *b_DeltaR2;   //!
   TBranch        *b_Q;   //!
   TBranch        *b_CosMotherAndDaughter_Lab;   //!
   TBranch        *b_CosMotherAndDaughter_CMS;   //!
   TBranch        *b_JpsiPt;   //!
   TBranch        *b_JpsiEta;   //!
   TBranch        *b_JpsiPhi;   //!
   TBranch        *b_JpsiMass;   //!
   TBranch        *b_PiPlusPt;   //!
   TBranch        *b_PiPlusEta;   //!
   TBranch        *b_PiPlusPhi;   //!
   TBranch        *b_PiPlusMass;   //!
   TBranch        *b_PiMinusPt;   //!
   TBranch        *b_PiMinusEta;   //!
   TBranch        *b_PiMinusPhi;   //!
   TBranch        *b_PiMinusMass;   //!
   TBranch        *b_EPlusPt;   //!
   TBranch        *b_EPlusEta;   //!
   TBranch        *b_EPlusPhi;   //!
   TBranch        *b_EPlusMass;   //!
   TBranch        *b_EMinusPt;   //!
   TBranch        *b_EMinusEta;   //!
   TBranch        *b_EMinusPhi;   //!
   TBranch        *b_EMinusMass;   //!

   EvtgenSim(TTree *tree=0);
   virtual ~EvtgenSim();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef EvtgenSim_cxx
EvtgenSim::EvtgenSim(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("X3872_JpsiRho.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("X3872_JpsiRho.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

EvtgenSim::~EvtgenSim()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t EvtgenSim::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t EvtgenSim::LoadTree(Long64_t entry)
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

void EvtgenSim::Init(TTree *tree)
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

   fChain->SetBranchAddress("X3872Pt", &X3872Pt, &b_X3872Pt);
   fChain->SetBranchAddress("X3872Eta", &X3872Eta, &b_X3872Eta);
   fChain->SetBranchAddress("X3872Phi", &X3872Phi, &b_X3872Phi);
   fChain->SetBranchAddress("X3872Mass", &X3872Mass, &b_X3872Mass);
   fChain->SetBranchAddress("DeltaR1", &DeltaR1, &b_DeltaR1);
   fChain->SetBranchAddress("DeltaR2", &DeltaR2, &b_DeltaR2);
   fChain->SetBranchAddress("Q", &Q, &b_Q);
   fChain->SetBranchAddress("CosMotherAndDaughter_Lab", &CosMotherAndDaughter_Lab, &b_CosMotherAndDaughter_Lab);
   fChain->SetBranchAddress("CosMotherAndDaughter_CMS", &CosMotherAndDaughter_CMS, &b_CosMotherAndDaughter_CMS);
   fChain->SetBranchAddress("JpsiPt", &JpsiPt, &b_JpsiPt);
   fChain->SetBranchAddress("JpsiEta", &JpsiEta, &b_JpsiEta);
   fChain->SetBranchAddress("JpsiPhi", &JpsiPhi, &b_JpsiPhi);
   fChain->SetBranchAddress("JpsiMass", &JpsiMass, &b_JpsiMass);
   fChain->SetBranchAddress("PiPlusPt", &PiPlusPt, &b_PiPlusPt);
   fChain->SetBranchAddress("PiPlusEta", &PiPlusEta, &b_PiPlusEta);
   fChain->SetBranchAddress("PiPlusPhi", &PiPlusPhi, &b_PiPlusPhi);
   fChain->SetBranchAddress("PiPlusMass", &PiPlusMass, &b_PiPlusMass);
   fChain->SetBranchAddress("PiMinusPt", &PiMinusPt, &b_PiMinusPt);
   fChain->SetBranchAddress("PiMinusEta", &PiMinusEta, &b_PiMinusEta);
   fChain->SetBranchAddress("PiMinusPhi", &PiMinusPhi, &b_PiMinusPhi);
   fChain->SetBranchAddress("PiMinusMass", &PiMinusMass, &b_PiMinusMass);
   fChain->SetBranchAddress("EPlusPt", &EPlusPt, &b_EPlusPt);
   fChain->SetBranchAddress("EPlusEta", &EPlusEta, &b_EPlusEta);
   fChain->SetBranchAddress("EPlusPhi", &EPlusPhi, &b_EPlusPhi);
   fChain->SetBranchAddress("EPlusMass", &EPlusMass, &b_EPlusMass);
   fChain->SetBranchAddress("EMinusPt", &EMinusPt, &b_EMinusPt);
   fChain->SetBranchAddress("EMinusEta", &EMinusEta, &b_EMinusEta);
   fChain->SetBranchAddress("EMinusPhi", &EMinusPhi, &b_EMinusPhi);
   fChain->SetBranchAddress("EMinusMass", &EMinusMass, &b_EMinusMass);
   Notify();
}

Bool_t EvtgenSim::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EvtgenSim::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t EvtgenSim::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef EvtgenSim_cxx
