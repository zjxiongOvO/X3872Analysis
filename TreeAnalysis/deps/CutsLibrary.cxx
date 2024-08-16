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
    // else if (cutname == "BasicAcc") {
    // }

    return cut;
};