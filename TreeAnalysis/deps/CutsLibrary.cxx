#include "QuadAnalysisCuts.h"

QuadAnalysisCuts* FindCut(string cutname){
    QuadAnalysisCuts* cut = new QuadAnalysisCuts();
    if (cutname == "debug1") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.16);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.65);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.6, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.6, 999);
    } 
    else if (cutname == "debug2") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.16);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.45);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.6, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.6, 999);
    }
    else if (cutname == "BasicAcc") {
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
    else if (cutname == "dataprefilter") {
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 1.5);
    }
    else if (cutname == "MLprefilter1") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.16);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.65);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.3, 999);
    }

    return cut;
};