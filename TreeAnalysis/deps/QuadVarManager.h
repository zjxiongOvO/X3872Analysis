#ifndef QUADVARMANAGER_H
#define QUADVARMANAGER_H

#include "../header/Quadplet.h"
#include "../header/Quadplet.C"
#include "../header/EvtgenSim.h"
#include "../header/EvtgenSim.C"
#include "../header/SkimmedTree.h"
#include "../header/SkimmedTree.C"
#include "../header/BDTtree.h"
#include "../header/BDTtree.C"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TVector.h"
#include "TLorentzVector.h"
#include "Math/Vector4D.h"
#include "Math/Vector3D.h"
#include "Math/GenVector/Boost.h"

using namespace std;
class QuadVarManager {
public:
    enum Variables {
        kNothing = -1,
        kMass,
        kPt,
        kEta,
        kPhi,
        kQ,
        kDeltaR1,
        kDeltaR2,
        kDeltaR,
        kDeltaPhi, //delta phi bewteen two pions
        kDeltaEta, //delta eta bewteen two pions
        kCosDileptonDiTracks, // cosine of the angle between the dilepton and di-tracks in the CMS
        kDileptonMass,
        kDileptonPt,
        kDileptonEta,
        kDileptonPhi,
        kDileptonSign,
        kDihardonMass,
        kDihardonPt,
        kDihardonEta,
        kDihardonSign,
        kElectronPt1,
        kElectronEta1,
        kElectronPhi1,
        kElectronPt2,
        kElectronEta2,
        kElectronPhi2,
        kPionPt1,
        kPionEta1,
        kPionPhi1,
        kPionPt2,
        kPionEta2,
        kPionPhi2,
        // TPC PID nSigma
        kTPCPIDNSigmaEl_electron1,
        kTPCPIDNSigmaEl_electron2,
        kTPCPIDNSigmaPi_electron1,
        kTPCPIDNSigmaPi_electron2,
        kTPCPIDNSigmaPr_electron1,
        kTPCPIDNSigmaPr_electron2,
        kTPCPIDNSigmaEl_pion1,
        kTPCPIDNSigmaEl_pion2,
        kTPCPIDNSigmaPi_pion1,
        kTPCPIDNSigmaPi_pion2,
        kTPCPIDNSigmaPr_pion1,
        kTPCPIDNSigmaPr_pion2,
        // Track Pt Resolution
        kRecoPt_electron1,
        kRecoPt_electron2,
        kRecoPt_pion1,
        kRecoPt_pion2,
        //
        kBDT,
        //
        kNall,
    };
    
    // QuadVarManager(string filename);
    QuadVarManager();
    ~QuadVarManager();
    double* value = nullptr;
    TTree* tree = nullptr;
    TFile* file = nullptr;
    void FillSim(EvtgenSim* sim);
    void FillData(Quadplet* data);
    void FillSkimmedTree(SkimmedTree* skimmedTree);
    void FillBDTtree(BDTtree* bdtTree);
    void CreateTree(string filename);
    void FillToTree();
    void WriteToFile() {
        this->file->cd();
        this->tree->Write();
    }
    // propagate methods
    void FillTPCPIDNSigma(EvtgenSim* sim, int i, TH2F* hTPCPIDNSigma_Pt);
    void FillTrackPtResolution(EvtgenSim* sim, int i, TH2F* hTrackPtResolution_Pt);
};

// QuadVarManager::QuadVarManager(string filename) {
//     double* value_temp = new double[Variables::kNall];
//     value = value_temp;
//     // this->value = new double[Variables::kNall];
//     for (int i = 0; i < Variables::kNall; i++) {
//         value[i] = -999;
//     }
//     file = new TFile(filename.c_str(), "recreate");
//     // tree initialization
//     tree = new TTree("tree", "tree");
//     tree->Branch("Mass", &value[Variables::kMass], "Mass/D");
//     tree->Branch("Pt", &value[Variables::kPt], "Pt/D");
//     tree->Branch("Eta", &value[Variables::kEta], "Eta/D");
//     // tree->Branch("Phi", &value[Variables::kPhi], "Phi/D");
//     // tree->Branch("kCosDileptonDiTracks", &value[Variables::kCosDileptonDiTracks], "kCosDileptonDiTracks/D");
//     tree->Branch("Q", &value[Variables::kQ], "Q/D");
//     tree->Branch("DeltaR", &value[Variables::kDeltaR], "DeltaR/D");
//     tree->Branch("kDileptonMass", &value[Variables::kDileptonMass], "kDileptonMass/D");
//     tree->Branch("kDileptonPt", &value[Variables::kDileptonPt], "kDileptonPt/D");
//     tree->Branch("kDileptonEta", &value[Variables::kDileptonEta], "kDileptonEta/D");
//     // tree->Branch("fDileptonPhi", &value[Variables::kPhi], "fDileptonPhi/D");
//     tree->Branch("fDileptonSign", &value[Variables::kDileptonSign], "fDileptonSign/D");
//     tree->Branch("fDiTracksMass", &value[Variables::kDihardonMass], "fDiTracksMass/D");
//     tree->Branch("fDiTracksPt", &value[Variables::kDihardonPt], "fDiTracksPt/D");
//     tree->Branch("fDiTracksSign", &value[Variables::kDihardonSign], "fDiTracksSign/D");
//     tree->Branch("fTrackPt1", &value[Variables::kPionPt1], "fTrackPt1/D");
//     tree->Branch("fTrackPt2", &value[Variables::kPionPt2], "fTrackPt2/D");
//     tree->Branch("fkDeltaPhiPiPi", &value[Variables::kDeltaPhi], "fkDeltaPhiPiPi/D");
//     tree->Branch("fkDeltaEtaPiPi", &value[Variables::kDeltaEta], "fkDeltaEtaPiPi/D");
// }

QuadVarManager::QuadVarManager() {
    double* value_temp = new double[Variables::kNall];
    value = value_temp;
    // this->value = new double[Variables::kNall];
    for (int i = 0; i < Variables::kNall; i++) {
        value[i] = -999;
    }
    file = nullptr;
}

QuadVarManager::~QuadVarManager() {
    delete[] value;
    file->Close();
}

void QuadVarManager::FillSim(EvtgenSim* sim) {
    value[kMass] = sim->X3872Mass;
    value[kPt] = sim->X3872Pt;
    value[kEta] = sim->X3872Eta;
    value[kDeltaR1] = sim->DeltaR1;
    value[kDeltaR2] = sim->DeltaR2;
    value[kDeltaR] = sqrt(sim->DeltaR1*sim->DeltaR1 + sim->DeltaR2*sim->DeltaR2)/sqrt(2);
    double deltaPhi = sim->PiPlusPhi - sim->PiMinusPhi;
    if (deltaPhi > TMath::Pi()) deltaPhi -= 2*TMath::Pi();
    if (deltaPhi < -TMath::Pi()) deltaPhi += 2*TMath::Pi();
    value[kDeltaPhi] = deltaPhi;
    double deltaEta = sim->PiPlusEta - sim->PiMinusEta;
    value[kDeltaEta] = deltaEta;
    value[kQ] = sim->Q;
    value[kCosDileptonDiTracks] = sim->CosMotherAndDaughter_CMS;
    value[kDileptonMass] = sim->JpsiMass;
    value[kDileptonPt] = sim->JpsiPt;
    value[kDileptonEta] = sim->JpsiEta;
    value[kDileptonSign] = 0;
    ROOT::Math::PtEtaPhiMVector Pionplus(sim->PiPlusPt, sim->PiPlusEta, sim->PiPlusPhi, 0.13957);
    ROOT::Math::PtEtaPhiMVector Pionminus(sim->PiMinusPt, sim->PiMinusEta, sim->PiMinusPhi, 0.13957);
    ROOT::Math::PtEtaPhiMVector DiPion = Pionplus + Pionminus;
    value[kDihardonMass] = DiPion.M();
    value[kDihardonPt] = DiPion.Pt();
    value[kDihardonEta] = DiPion.Eta();
    value[kDihardonSign] = 0;
    value[kElectronPt1] = sim->EPlusPt;
    value[kElectronEta1] = sim->EPlusEta;
    value[kElectronPhi1] = sim->EPlusPhi;
    value[kElectronPt2] = sim->EMinusPt;
    value[kElectronEta2] = sim->EMinusEta;
    value[kElectronPhi2] = sim->EMinusPhi;
    value[kPionPt1] = sim->PiPlusPt;
    value[kPionEta1] = sim->PiPlusEta;
    value[kPionPhi1] = sim->PiPlusPhi;
    value[kPionPt2] = sim->PiMinusPt;
    value[kPionEta2] = sim->PiMinusEta;
    value[kPionPhi2] = sim->PiMinusPhi;
}

void QuadVarManager::FillData(Quadplet* data) {
    value[kMass] = data->fMass;
    value[kPt] = data->fPt;
    value[kEta] = data->fEta;
    value[kPhi] = data->fPhi;
    value[kQ] = data->fDeltaQ;
    value[kDeltaR1] = data->fR1;
    value[kDeltaR2] = data->fR2;
    value[kDeltaR] = sqrt(data->fR1*data->fR1 + data->fR2*data->fR2)/sqrt(2);
    value[kDileptonMass] = data->fDileptonMass;
    value[kDileptonPt] = data->fDileptonPt;
    value[kDileptonEta] = data->fDileptonEta;
    value[kDileptonPhi] = data->fDileptonPhi;
    // ROOT::Math::PtEtaPhiMVector X(data->fDileptonPt, data->fDileptonEta, data->fDileptonPhi, 3.8722);
    // ROOT::Math::PtEtaPhiMVector Jpsi(data->fDileptonPt, data->fDileptonEta, data->fDileptonPhi, 3.0969);
    // ROOT::Math::Boost boostX{X.BoostToCM()};
    // ROOT::Math::XYZVectorF JpsiInX = boostX(Jpsi.Vect());
    // value[kCosDileptonDiTracks] = JpsiInX.
    value[kDileptonSign] = data->fDileptonSign;
    value[kDihardonMass] = data->fDiTracksMass;
    value[kDihardonPt] = data->fDiTracksPt;
    value[kDihardonSign] = data->fDiTracksSign;
    value[kPionPt1] = data->fTrackPt1;
    value[kPionPt2] = data->fTrackPt2;
    value[kPionEta1] = data->fTrackEta1;
    value[kPionEta2] = data->fTrackEta2;
    value[kPionPhi1] = data->fTrackPhi1;
    value[kPionPhi2] = data->fTrackPhi2;
    ROOT::Math::PtEtaPhiMVector Pionplus(data->fTrackPt1, data->fTrackEta1, data->fTrackPhi1, 0.13957);
    ROOT::Math::PtEtaPhiMVector Pionminus(data->fTrackPt2, data->fTrackEta2, data->fTrackPhi2, 0.13957);
    ROOT::Math::PtEtaPhiMVector DiPion = Pionplus + Pionminus;
    value[kDeltaPhi] = Pionplus.Phi() - Pionminus.Phi();
    value[kDeltaEta] = Pionplus.Eta() - Pionminus.Eta();
    value[kDihardonEta] = DiPion.Eta();
}

void QuadVarManager::FillSkimmedTree(SkimmedTree* skimmedTree) {
    // the structure of the skimmed tree should be the same as the tree we defined in the constructor
    value[kMass] = skimmedTree->Mass;
    value[kPt] = skimmedTree->Pt;
    value[kEta] = skimmedTree->Eta;
    value[kQ] = skimmedTree->Q;
    value[kDeltaR] = skimmedTree->DeltaR;
    value[kDileptonMass] = skimmedTree->kDileptonMass;
    value[kDileptonPt] = skimmedTree->kDileptonPt;
    value[kDileptonEta] = skimmedTree->kDileptonEta;
    value[kDileptonSign] = skimmedTree->fDileptonSign;
    value[kDihardonMass] = skimmedTree->fDiTracksMass;
    value[kDihardonPt] = skimmedTree->fDiTracksPt;
    value[kDihardonSign] = skimmedTree->fDiTracksSign;
    value[kPionPt1] = skimmedTree->fTrackPt1;
    value[kPionPt2] = skimmedTree->fTrackPt2;
    value[kDeltaPhi] = skimmedTree->fkDeltaPhiPiPi;
    value[kDeltaEta] = skimmedTree->fkDeltaEtaPiPi;
}

void QuadVarManager::FillBDTtree(BDTtree* bdtTree) {
    // the structure of the BDT tree should be the same as the tree we defined in the constructor
    value[kMass] = bdtTree->Mass;
    value[kBDT] = bdtTree->model_output;
}

void QuadVarManager::CreateTree(string filename){
    // tree->Reset();
    if (tree != nullptr) delete tree;
    if (file != nullptr) file->Close();
    file = new TFile(filename.c_str(), "recreate");
    // tree initialization
    tree = new TTree("tree", "tree");
    tree->Branch("Mass", &value[Variables::kMass], "Mass/D");
    tree->Branch("Pt", &value[Variables::kPt], "Pt/D");
    tree->Branch("Eta", &value[Variables::kEta], "Eta/D");
    // tree->Branch("kCosDileptonDiTracks", &value[Variables::kCosDileptonDiTracks], "kCosDileptonDiTracks/D");
    tree->Branch("Q", &value[Variables::kQ], "Q/D");
    tree->Branch("DeltaR", &value[Variables::kDeltaR], "DeltaR/D");
    tree->Branch("kDileptonMass", &value[Variables::kDileptonMass], "kDileptonMass/D");
    tree->Branch("kDileptonPt", &value[Variables::kDileptonPt], "kDileptonPt/D");
    tree->Branch("kDileptonEta", &value[Variables::kDileptonEta], "kDileptonEta/D");
    tree->Branch("fDileptonSign", &value[Variables::kDileptonSign], "fDileptonSign/D");
    tree->Branch("fDiTracksMass", &value[Variables::kDihardonMass], "fDiTracksMass/D");
    tree->Branch("fDiTracksPt", &value[Variables::kDihardonPt], "fDiTracksPt/D");
    tree->Branch("fDiTracksSign", &value[Variables::kDihardonSign], "fDiTracksSign/D");
    tree->Branch("fTrackPt1", &value[Variables::kPionPt1], "fTrackPt1/D");
    tree->Branch("fTrackPt2", &value[Variables::kPionPt2], "fTrackPt2/D");
    tree->Branch("fkDeltaPhiPiPi", &value[Variables::kDeltaPhi], "fkDeltaPhiPiPi/D");
    tree->Branch("fkDeltaEtaPiPi", &value[Variables::kDeltaEta], "fkDeltaEtaPiPi/D");
}


void QuadVarManager::FillToTree() {
    tree->Fill();
}

void QuadVarManager::FillTPCPIDNSigma(EvtgenSim* sim, int i, TH2F* hTPCPIDNSigma_Pt){
    double pT = sim->X3872Pt;
    // find the binx, if pT is larger than the last bin edge, set binx to the last bin
    int binx = 0;
    if (pT > hTPCPIDNSigma_Pt->GetXaxis()->GetBinUpEdge(hTPCPIDNSigma_Pt->GetNbinsX())) {
        binx = hTPCPIDNSigma_Pt->GetNbinsX();
    } else {
        binx = hTPCPIDNSigma_Pt->GetXaxis()->FindBin(pT);
    }
    // slice the histogram along the x-axis
    TH1D* hSlice = hTPCPIDNSigma_Pt->ProjectionY("hSlice", binx, binx);
    // fill the value randomly
    value[i] = hSlice->GetRandom();
    delete hSlice;
}

void QuadVarManager::FillTrackPtResolution(EvtgenSim* sim, int i, TH2F* hTrackPtResolution_Pt){
    double pT = sim->X3872Pt;
    // find the binx, if pT is larger than the last bin edge, set binx to the last bin
    int binx = 0;
    if (pT > hTrackPtResolution_Pt->GetXaxis()->GetBinUpEdge(hTrackPtResolution_Pt->GetNbinsX())) {
        binx = hTrackPtResolution_Pt->GetNbinsX();
    } else {
        binx = hTrackPtResolution_Pt->GetXaxis()->FindBin(pT);
    }
    // slice the histogram along the x-axis
    TH1D* hSlice = hTrackPtResolution_Pt->ProjectionY("hSlice", binx, binx);
    // fill the value randomly
    value[i] = hSlice->GetRandom();
    delete hSlice;
}

#endif // VARMANAGER_H