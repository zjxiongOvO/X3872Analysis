//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Aug 13 18:14:11 2024 by ROOT version 6.30/01
// from TTree tree/tree
// found on file: X3872_JpsiRho_Swave.root
//////////////////////////////////////////////////////////

#ifndef SkimmedTree_h
#define SkimmedTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class SkimmedTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        Mass;
   Double_t        Pt;
   Double_t        Eta;
   Double_t        Q;
   Double_t        DeltaR;
   Double_t        kDileptonMass;
   Double_t        kDileptonPt;
   Double_t        kDileptonEta;
   Double_t        fDileptonSign;
   Double_t        fDiTracksMass;
   Double_t        fDiTracksPt;
   Double_t        fDiTracksSign;
   Double_t        fTrackPt1;
   Double_t        fTrackPt2;
   Double_t        fkDeltaPhiPiPi;
   Double_t        fkDeltaEtaPiPi;

   // List of branches
   TBranch        *b_Mass;   //!
   TBranch        *b_Pt;   //!
   TBranch        *b_Eta;   //!
   TBranch        *b_Q;   //!
   TBranch        *b_DeltaR;   //!
   TBranch        *b_kDileptonMass;   //!
   TBranch        *b_kDileptonPt;   //!
   TBranch        *b_kDileptonEta;   //!
   TBranch        *b_fDileptonSign;   //!
   TBranch        *b_fDiTracksMass;   //!
   TBranch        *b_fDiTracksPt;   //!
   TBranch        *b_fDiTracksSign;   //!
   TBranch        *b_fTrackPt1;   //!
   TBranch        *b_fTrackPt2;   //!
   TBranch        *b_fkDeltaPhiPiPi;   //!
   TBranch        *b_fkDeltaEtaPiPi;   //!

   SkimmedTree(TTree *tree=0);
   virtual ~SkimmedTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef SkimmedTree_cxx
SkimmedTree::SkimmedTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("X3872_JpsiRho_Swave.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("X3872_JpsiRho_Swave.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

SkimmedTree::~SkimmedTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t SkimmedTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t SkimmedTree::LoadTree(Long64_t entry)
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

void SkimmedTree::Init(TTree *tree)
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

   fChain->SetBranchAddress("Mass", &Mass, &b_Mass);
   fChain->SetBranchAddress("Pt", &Pt, &b_Pt);
   fChain->SetBranchAddress("Eta", &Eta, &b_Eta);
   fChain->SetBranchAddress("Q", &Q, &b_Q);
   fChain->SetBranchAddress("DeltaR", &DeltaR, &b_DeltaR);
   fChain->SetBranchAddress("kDileptonMass", &kDileptonMass, &b_kDileptonMass);
   fChain->SetBranchAddress("kDileptonPt", &kDileptonPt, &b_kDileptonPt);
   fChain->SetBranchAddress("kDileptonEta", &kDileptonEta, &b_kDileptonEta);
   fChain->SetBranchAddress("fDileptonSign", &fDileptonSign, &b_fDileptonSign);
   fChain->SetBranchAddress("fDiTracksMass", &fDiTracksMass, &b_fDiTracksMass);
   fChain->SetBranchAddress("fDiTracksPt", &fDiTracksPt, &b_fDiTracksPt);
   fChain->SetBranchAddress("fDiTracksSign", &fDiTracksSign, &b_fDiTracksSign);
   fChain->SetBranchAddress("fTrackPt1", &fTrackPt1, &b_fTrackPt1);
   fChain->SetBranchAddress("fTrackPt2", &fTrackPt2, &b_fTrackPt2);
   fChain->SetBranchAddress("fkDeltaPhiPiPi", &fkDeltaPhiPiPi, &b_fkDeltaPhiPiPi);
   fChain->SetBranchAddress("fkDeltaEtaPiPi", &fkDeltaEtaPiPi, &b_fkDeltaEtaPiPi);
   Notify();
}

Bool_t SkimmedTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void SkimmedTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t SkimmedTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef SkimmedTree_cxx
