//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Aug 11 21:57:30 2024 by ROOT version 6.30/01
// from TTree O2rtdquadplet/O2rtdquadplet
// found on file: LHC22_23_24.root
//////////////////////////////////////////////////////////

#ifndef Quadplet_h
#define Quadplet_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class Quadplet {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         fMass;
   Float_t         fPt;
   Float_t         fEta;
   Float_t         fPhi;
   Float_t         fDeltaQ;
   Float_t         fR1;
   Float_t         fR2;
   Float_t         fDileptonMass;
   Float_t         fDileptonPt;
   Float_t         fDileptonEta;
   Float_t         fDileptonPhi;
   Int_t           fDileptonSign;
   Float_t         fDiTracksMass;
   Float_t         fDiTracksPt;
   Int_t           fDiTracksSign;
   Float_t         fTrackPt1;
   Float_t         fTrackPt2;
   Float_t         fTrackEta1;
   Float_t         fTrackEta2;
   Float_t         fTrackPhi1;
   Float_t         fTrackPhi2;
   Float_t         fTrackDcaXY1;
   Float_t         fTrackDcaZ1;
   Float_t         fTrackDcaXY2;
   Float_t         fTrackDcaZ2;
   Float_t         fDileptonTPCPIDChi2El;
   Float_t         fDileptonTPCPIDChi2Pi;
   Float_t         fDileptonTPCPIDChi2Pr;
   Float_t         fDiTracksTPCPIDChi2El;
   Float_t         fDiTracksTPCPIDChi2Pi;
   Float_t         fDiTracksTPCPIDChi2Pr;

   // List of branches
   TBranch        *b_fMass;   //!
   TBranch        *b_fPt;   //!
   TBranch        *b_fEta;   //!
   TBranch        *b_fPhi;   //!
   TBranch        *b_fDeltaQ;   //!
   TBranch        *b_fR1;   //!
   TBranch        *b_fR2;   //!
   TBranch        *b_fDileptonMass;   //!
   TBranch        *b_fDileptonPt;   //!
   TBranch        *b_fDileptonEta;   //!
   TBranch        *b_fDileptonPhi;   //!
   TBranch        *b_fDileptonSign;   //!
   TBranch        *b_fDiTracksMass;   //!
   TBranch        *b_fDiTracksPt;   //!
   TBranch        *b_fDiTracksSign;   //!
   TBranch        *b_fTrackPt1;   //!
   TBranch        *b_fTrackPt2;   //!
   TBranch        *b_fTrackEta1;   //!
   TBranch        *b_fTrackEta2;   //!
   TBranch        *b_fTrackPhi1;   //!
   TBranch        *b_fTrackPhi2;   //!
   TBranch        *b_fTrackDcaXY1;   //!
   TBranch        *b_fTrackDcaZ1;   //!
   TBranch        *b_fTrackDcaXY2;   //!
   TBranch        *b_fTrackDcaZ2;   //!
   TBranch        *b_fDileptonTPCPIDChi2El;   //!
   TBranch        *b_fDileptonTPCPIDChi2Pi;   //!
   TBranch        *b_fDileptonTPCPIDChi2Pr;   //!
   TBranch        *b_fDiTracksTPCPIDChi2El;   //!
   TBranch        *b_fDiTracksTPCPIDChi2Pi;   //!
   TBranch        *b_fDiTracksTPCPIDChi2Pr;   //!

   Quadplet(TTree *tree=0);
   virtual ~Quadplet();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Quadplet_cxx
Quadplet::Quadplet(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("LHC22_23_24.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("LHC22_23_24.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("LHC22_23_24.root:/DF_2271170122563072");
      dir->GetObject("O2rtdquadplet",tree);

   }
   Init(tree);
}

Quadplet::~Quadplet()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Quadplet::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Quadplet::LoadTree(Long64_t entry)
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

void Quadplet::Init(TTree *tree)
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

   fChain->SetBranchAddress("fMass", &fMass, &b_fMass);
   fChain->SetBranchAddress("fPt", &fPt, &b_fPt);
   fChain->SetBranchAddress("fEta", &fEta, &b_fEta);
   fChain->SetBranchAddress("fPhi", &fPhi, &b_fPhi);
   fChain->SetBranchAddress("fDeltaQ", &fDeltaQ, &b_fDeltaQ);
   fChain->SetBranchAddress("fR1", &fR1, &b_fR1);
   fChain->SetBranchAddress("fR2", &fR2, &b_fR2);
   fChain->SetBranchAddress("fDileptonMass", &fDileptonMass, &b_fDileptonMass);
   fChain->SetBranchAddress("fDileptonPt", &fDileptonPt, &b_fDileptonPt);
   fChain->SetBranchAddress("fDileptonEta", &fDileptonEta, &b_fDileptonEta);
   fChain->SetBranchAddress("fDileptonPhi", &fDileptonPhi, &b_fDileptonPhi);
   fChain->SetBranchAddress("fDileptonSign", &fDileptonSign, &b_fDileptonSign);
   fChain->SetBranchAddress("fDiTracksMass", &fDiTracksMass, &b_fDiTracksMass);
   fChain->SetBranchAddress("fDiTracksPt", &fDiTracksPt, &b_fDiTracksPt);
   fChain->SetBranchAddress("fDiTracksSign", &fDiTracksSign, &b_fDiTracksSign);
   fChain->SetBranchAddress("fTrackPt1", &fTrackPt1, &b_fTrackPt1);
   fChain->SetBranchAddress("fTrackPt2", &fTrackPt2, &b_fTrackPt2);
   fChain->SetBranchAddress("fTrackEta1", &fTrackEta1, &b_fTrackEta1);
   fChain->SetBranchAddress("fTrackEta2", &fTrackEta2, &b_fTrackEta2);
   fChain->SetBranchAddress("fTrackPhi1", &fTrackPhi1, &b_fTrackPhi1);
   fChain->SetBranchAddress("fTrackPhi2", &fTrackPhi2, &b_fTrackPhi2);
   fChain->SetBranchAddress("fTrackDcaXY1", &fTrackDcaXY1, &b_fTrackDcaXY1);
   fChain->SetBranchAddress("fTrackDcaZ1", &fTrackDcaZ1, &b_fTrackDcaZ1);
   fChain->SetBranchAddress("fTrackDcaXY2", &fTrackDcaXY2, &b_fTrackDcaXY2);
   fChain->SetBranchAddress("fTrackDcaZ2", &fTrackDcaZ2, &b_fTrackDcaZ2);
   fChain->SetBranchAddress("fDileptonTPCPIDChi2El", &fDileptonTPCPIDChi2El, &b_fDileptonTPCPIDChi2El);
   fChain->SetBranchAddress("fDileptonTPCPIDChi2Pi", &fDileptonTPCPIDChi2Pi, &b_fDileptonTPCPIDChi2Pi);
   fChain->SetBranchAddress("fDileptonTPCPIDChi2Pr", &fDileptonTPCPIDChi2Pr, &b_fDileptonTPCPIDChi2Pr);
   fChain->SetBranchAddress("fDiTracksTPCPIDChi2El", &fDiTracksTPCPIDChi2El, &b_fDiTracksTPCPIDChi2El);
   fChain->SetBranchAddress("fDiTracksTPCPIDChi2Pi", &fDiTracksTPCPIDChi2Pi, &b_fDiTracksTPCPIDChi2Pi);
   fChain->SetBranchAddress("fDiTracksTPCPIDChi2Pr", &fDiTracksTPCPIDChi2Pr, &b_fDiTracksTPCPIDChi2Pr);
   Notify();
}

Bool_t Quadplet::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Quadplet::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Quadplet::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Quadplet_cxx
