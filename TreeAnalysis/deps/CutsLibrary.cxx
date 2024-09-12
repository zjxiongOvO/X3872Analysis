#include "QuadAnalysisCuts.h"
#include <TCutG.h>

QuadAnalysisCuts* FindCut(string cutname){
    QuadAnalysisCuts* cut = new QuadAnalysisCuts();
    // read cut from file
    TFile* file = new TFile("output/CutG.root");
    // std::vector<double> SavedRatios = {0.98, 0.96, 0.94, 0.92, 0.90, 0.88, 0.86, 0.84, 0.82, 0.80};
    std::vector<double> SavedRatios = {0.98, 0.96, 0.94, 0.92, 0.90, 0.88, 0.86, 0.84, 0.82, 0.80, 0.70, 0.60, 0.50, 0.40, 0.30, 0.20, 0.10};

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
    else if (cutname == "TunedAcc") {
        cut->SetCut(QuadVarManager::kElectronPt1, 1.0, 999);
        cut->SetCut(QuadVarManager::kElectronPt2, 1.0, 999);
        cut->SetCut(QuadVarManager::kElectronEta1, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kElectronEta2, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kDileptonEta, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kPionPt1, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionEta1, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kPionEta2, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kDihardonEta, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.2);
    }
    else if (cutname == "dataprefilter") {
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 2.5);
    }
    else if (cutname == "MLprefilter1") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.16);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.65);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.3, 999);
    }
    else if (cutname == "BasicAccAndSpecial") {
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.45);
        cut->SetCut(QuadVarManager::kDihardonPt, 1.0, 999);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        cut->SetCut(QuadVarManager::kElectronPt1, 1.0, 999);
        cut->SetCut(QuadVarManager::kElectronPt2, 1.0, 999);
        cut->SetCut(QuadVarManager::kElectronEta1, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kElectronEta2, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kDileptonEta, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kPionPt1, 0.6, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.6, 999);
        cut->SetCut(QuadVarManager::kPionEta1, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kPionEta2, -0.9, 0.9);
        cut->SetCut(QuadVarManager::kDihardonEta, -0.9, 0.9);
    }
    else if (cutname == "Special") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.92, 3.14);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.28);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.45);
        cut->SetCut(QuadVarManager::kDihardonPt, 1.0, 999);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.6, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.6, 999);
    }
    else if (cutname == "Special2") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.20);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.28);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.45);
        cut->SetCut(QuadVarManager::kDihardonPt, 1.0, 999);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.6, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.6, 999);
    }
    else if (cutname == "MLprefilter2_X3872") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.16);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.6);
        cut->SetCut(QuadVarManager::kDeltaR1, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDeltaR2, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.3, 999);
    }
    else if (cutname == "MLprefilter2_Psi2S") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.16);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR1, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDeltaR2, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.0, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.15, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.15, 999);
    }
    else if (cutname == "MLprefilter2_X3872_Sim") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.16);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR1, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDeltaR2, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
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
    else if (cutname == "MLprefilter2_Psi2S_Sim") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.16);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR1, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDeltaR2, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.0, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.3, 999);
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
    else if (cutname == "MLprefilter3_X3872") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.16);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR1, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDeltaR2, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.3, 999);
    }
    else if (cutname == "MLprefilter3_X3872_Sim") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.16);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        cut->SetCut(QuadVarManager::kDeltaR1, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDeltaR2, 0.0, 1.0);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.3, 999);
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
    else if (cutname == "MLprefilter4_X3872_Sim") {
        cut->SetCut(QuadVarManager::kDeltaR1, 0.0, 1.5);
        cut->SetCut(QuadVarManager::kDeltaR2, 0.0, 1.5);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.0, 999);
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
    else if (cutname == "CutG_debug1") {
        cut->SetCut(QuadVarManager::kPt, 5, 999);
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.20);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.3);
        // cut->SetCut(QuadVarManager::kDihardonMass, 0.0, 999);
        // cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.65);
        // cut->SetCutG(cutg_DeltaR_Pt, QuadVarManager::kPt, QuadVarManager::kDeltaR);
        // cut->SetCutG(cutg_DeltaEta_Pt, QuadVarManager::kPt, QuadVarManager::kDeltaEta);
        // cut->SetCutG(cutg_DeltaPhi_Pt, QuadVarManager::kPt, QuadVarManager::kDeltaPhi);
    }
    else if (cutname == "ImprovedCut1") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.20);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.25);
        // TCutG* cutg_DeltaR_Pt = (TCutG*)file->Get("cutg_DeltaR_Pt_0.95");
        // cut->SetCutG(cutg_DeltaR_Pt, QuadVarManager::kPt, QuadVarManager::kDeltaR);
        // TCutG* cutg_DeltaEta_Pt = (TCutG*)file->Get("cutg_DeltaEta_Pt_0.90");
        // cut->SetCutG(cutg_DeltaEta_Pt, QuadVarManager::kPt, QuadVarManager::kDeltaEta);
        // TCutG* cutg_DeltaPhi_Pt = (TCutG*)file->Get("cutg_DeltaPhi_Pt_0.95");
        // cut->SetCutG(cutg_DeltaPhi_Pt, QuadVarManager::kPt, QuadVarManager::kDeltaPhi);
        // cut->SetCutG(cutg_DeltaR_Pt, QuadVarManager::kPt, QuadVarManager::kDeltaR);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.3, 999);
    }
    else if (cutname == "ImprovedCut2") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.20);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.20);
        // cut->SetCutG(cutg_DeltaR_Pt, QuadVarManager::kPt, QuadVarManager::kDeltaR);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.45);
    }
    else if (cutname == "ImprovedCut3") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.20);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.20);
        // cut->SetCutG(cutg_DeltaR_Pt, QuadVarManager::kPt, QuadVarManager::kDeltaR);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.45);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.3, 999);
    }
    else if (cutname == "nocut") {
        // no cut
    }

    // cut-scan method 
    // for (int i = 0; i < 10; i++) {
    //     if (cutname == ("Scan"+to_string(i))) {
    //         cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.20);
    //         cut->SetCut(QuadVarManager::kQ, 0.0, 0.20);
    //         TCutG* cutg_DeltaR_Pt_Psi2S = (TCutG*)file->Get(Form("cutg_DeltaR_Pt_Psi2S_%.2f", SavedRatios[i]));
    //         cut->SetCutG(cutg_DeltaR_Pt_Psi2S, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 19);
    //     }
    // }

    for (int i = 0; i < SavedRatios.size(); i++) {
        if (cutname == ("ScanX"+to_string(i))) {
            cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.20);
            cut->SetCut(QuadVarManager::kQ, 0.0, 0.20);
            cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
            TCutG* cutg_DeltaR_Pt_Psi2S = (TCutG*)file->Get(Form("cutg_Tuned_DeltaR_Pt_X3872_%.2f", SavedRatios[i]));
            cut->SetCutG(cutg_DeltaR_Pt_Psi2S, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 39);
            // cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.45);
            cut->SetCut(QuadVarManager::kPionPt1, 0.3, 999);
            cut->SetCut(QuadVarManager::kPionPt2, 0.3, 999);
        }
    }

    if (cutname == "FinaltuneX") {
        cut->SetCut(QuadVarManager::kPt, 2, 40);
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.20);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.2);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        TCutG* cutg_DeltaR_Pt_X3872 = (TCutG*)file->Get("cutg_Tuned_DeltaR_Pt_X3872_0.60");
        cut->SetCutG(cutg_DeltaR_Pt_X3872, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 39); 
        cut->SetCut(QuadVarManager::kPionPt1, 0.3, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.3, 999);
    }

    if (cutname == "Finaltune") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.2);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.30);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.50, 999);
        // TCutG* cutg_DeltaR_Pt_Psi2S = (TCutG*)file->Get("cutg_DeltaR_Pt_Psi2S_0.80");
        // cut->SetCutG(cutg_DeltaR_Pt_Psi2S, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 39);
        TCutG* cutg_DeltaR_Pt_X3872 = (TCutG*)file->Get("cutg_Tuned_DeltaR_Pt_X3872_0.80");
        cut->SetCutG(cutg_DeltaR_Pt_X3872, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 39); 
        cut->SetCut(QuadVarManager::kPionPt1, 0.40, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.40, 999);
        // cut->SetCut(QuadVarManager::kDihardonPt, 1.0, 999);
    }

    if (cutname == "Finaltune2S") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.2);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.25);
        // cut->SetCut(QuadVarManager::kDihardonMass, 0.50, 999);
        TCutG* cutg_DeltaR_Pt_Psi2S = (TCutG*)file->Get("cutg_DeltaR_Pt_Psi2S_0.80");
        cut->SetCutG(cutg_DeltaR_Pt_Psi2S, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 39);
        // TCutG* cutg_DeltaR_Pt_X3872 = (TCutG*)file->Get("cutg_Tuned_DeltaR_Pt_X3872_0.80");
        // cut->SetCutG(cutg_DeltaR_Pt_X3872, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 39); 
        cut->SetCut(QuadVarManager::kPionPt1, 0.30, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.30, 999);
        // cut->SetCut(QuadVarManager::kDihardonPt, 1.0, 999);
    }

    if (cutname == "FinaltuneX3872") {
        cut->SetCut(QuadVarManager::kPt, 4, 35);
        cut->SetCut(QuadVarManager::kDileptonMass, 2.72, 3.2);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.25);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.50, 999);
        // TCutG* cutg_DeltaR_Pt_Psi2S = (TCutG*)file->Get("cutg_DeltaR_Pt_Psi2S_0.80");
        // cut->SetCutG(cutg_DeltaR_Pt_Psi2S, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 39);
        TCutG* cutg_DeltaR_Pt_X3872 = (TCutG*)file->Get("cutg_Tuned_DeltaR_Pt_X3872_0.80");
        cut->SetCutG(cutg_DeltaR_Pt_X3872, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 39); 
        cut->SetCut(QuadVarManager::kPionPt1, 0.40, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.40, 999);
        // cut->SetCut(QuadVarManager::kDihardonPt, 1.0, 999);
    }

    if (cutname == "Specialtuned") {
        cut->SetCut(QuadVarManager::kDileptonMass, 2.92, 3.14);
        cut->SetCut(QuadVarManager::kQ, 0.0, 0.28);
        cut->SetCut(QuadVarManager::kDeltaR, 0.0, 0.45);
        // TCutG* cutg_DeltaR_Pt_X3872 = (TCutG*)file->Get("cutg_Tuned_DeltaR_Pt_X3872_0.80");
        // cut->SetCutG(cutg_DeltaR_Pt_X3872, QuadVarManager::kPt, QuadVarManager::kDeltaR, 1, 39); 
        cut->SetCut(QuadVarManager::kDihardonPt, 1.0, 999);
        cut->SetCut(QuadVarManager::kDihardonMass, 0.5, 999);
        cut->SetCut(QuadVarManager::kPionPt1, 0.6, 999);
        cut->SetCut(QuadVarManager::kPionPt2, 0.6, 999);
    }
    return cut;
};