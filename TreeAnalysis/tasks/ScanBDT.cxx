#include <TTree.h>
#include <TCanvas.h>
#include "../deps/Mtool.h"
#include "../deps/QuadVarManager.h"
#include "../deps/SignalExtraction_X3872_only.cxx"
#include "../deps/SignalExtraction_Psi2S_only.cxx"

void ScanBDT(string inputfile, double start, double end, int NBDT){
    // load the BDT output tree
    TFile* myfile = new TFile(inputfile.c_str(), "read");
    TTree* tree = (TTree*)myfile->Get("fTreePairsSPM");
    BDTtree* bdt = new BDTtree(tree);
    QuadVarManager* var = new QuadVarManager();

    double step = (end-start)/NBDT;
    TH1F* hMass_Psi2S[NBDT];
    TH1F* hMass_X3872[NBDT];
    // loop over the BDT output
    for (int i = 0; i < NBDT; i++){
        hMass_Psi2S[i] = new TH1F(Form("hMass_Psi2S_%d", i), Form("hMass_Psi2S_%d", i), 36, 3.6, 3.76);
        hMass_X3872[i] = new TH1F(Form("hMass_X3872_%d", i), Form("hMass_X3872_%d", i), 36, 3.76, 4.0);
        hMass_Psi2S[i]->Sumw2();
        hMass_X3872[i]->Sumw2();
    }

    for (int i = 0; i < tree->GetEntries(); i++){
        tree->GetEntry(i);
        var->FillBDTtree(bdt);
        for (int j = 0; j < NBDT; j++){
            if (var->value[QuadVarManager::Variables::kBDT] > (j*step + start)){
                hMass_Psi2S[j]->Fill(var->value[QuadVarManager::Variables::kMass]);
                hMass_X3872[j]->Fill(var->value[QuadVarManager::Variables::kMass]);
            }
        }
    }

    system("mkdir -p output/BDTScan");
    for (int j = 0; j < NBDT; j++){
        string outputpath = Form("output/BDTScan/BDT_%.2f_Psi2S.pdf", j*step + start);
        SignalExtraction_Psi2S_only(hMass_Psi2S[j], 3.6, 3.76, outputpath);
        outputpath = Form("output/BDTScan/BDT_%.2f_X3872.pdf", j*step + start);
        SignalExtraction_X3872_only(hMass_X3872[j], 3.76, 4.0, outputpath);
    }
}