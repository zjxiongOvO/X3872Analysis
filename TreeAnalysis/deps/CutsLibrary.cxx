#include "QuadAnalysisCuts.h"
#include <TCutG.h>

QuadAnalysisCuts* FindCut(string cutname){
    QuadAnalysisCuts* cut = new QuadAnalysisCuts();
    // read 2D-cut from file
    TFile* file = new TFile("output/CutG.root");
    std::vector<double> SavedRatios = {0.98, 0.96, 0.94, 0.92, 0.90, 0.88, 0.86, 0.84, 0.82, 0.80, 0.70, 0.60, 0.50, 0.40, 0.30, 0.20, 0.10};

    if (cutname == "BasicAcc") {
        cut->SetCut(QuadVarManager::kElectronPt1, 1.0, 999);
        cut->SetCut(QuadVarManager::kElectronPt2, 1.0, 999);
        cut->SetCut(QuadVarManager::kElectronEta1, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kElectronEta2, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kDileptonEta, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kPionPt1, 0.15, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.15, 999);
        cut->SetCut(QuadVarManager::kPionEta1, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kPionEta2, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kDihardonEta, -0.9, 0.9);
    }
    if (cutname == "dataprefilter") {
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 2.5);
    }
    if (cutname == "MLprefilter") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.20);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR1, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDeltaR2, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.3, 999);
    }
    if (cutname == "nocut") {
        // no cut
    }
    // special cut for Psi2S (4 - 35 GeV, performance plot)
    if (cutname == "Finaltune2S") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.2);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.25);
        TCutG* cutg_DeltaR_Pt_Psi2S = (TCutG*)file->Get("cutg_DeltaR_Pt_Psi2S_0.80");
        cut->SetCutG(cutg_DeltaR_Pt_Psi2S, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 39);
        cut->SetCut(QuadVarManager::kPionPt1, 0.30, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.30, 999);
    }
    // special cut for X3872 (4 - 35 GeV, performance plot)
    if (cutname == "FinaltuneX3872") {
        cut->SetCut(QuadVarManager::kPt, 4, 35);
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.2);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.25);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.50, 999);
        TCutG* cutg_DeltaR_Pt_X3872 = (TCutG*)file->Get("cutg_Tuned_DeltaR_Pt_X3872_0.80");
        cut->SetCutG(cutg_DeltaR_Pt_X3872, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 39); 
        cut->SetCut(QuadVarManager::kPionPt1, 0.40, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.40, 999);
    }
    // the cut we can find the fake-peak in 2022 data
    if (cutname == "Specialtuned") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.92, 3.14);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.28);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.45);
        cut->SetCut(QuadVarManager::kDihardonPt, 1.0, 999);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.6, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.6, 999);
    }
    return cut;
};