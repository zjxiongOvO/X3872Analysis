#include <TTree.h>
#include <TCanvas.h>
#include "../deps/Mtool.h"
#include "../deps/QuadVarManager.h"
#include "../deps/SignalExtraction_X3872.cxx"

void ScanBDT(string inputfile, double start, double end, int NBDT){
    // load the BDT output tree
    TFile* myfile = new TFile(inputfile.c_str(), "read");
    TTree* tree = (TTree*)myfile->Get("fTreePairsSPM");
    BDTtree* bdt = new BDTtree(tree);
    QuadVarManager* var = new QuadVarManager();

    double step = (end-start)/NBDT;
    TH1F* hMass[NBDT];
    // loop over the BDT output
    for (int i = 0; i < NBDT; i++){
        hMass[i] = new TH1F(Form("hMass_%d", i), Form("hMass_%d", i), 40, 3.6, 4.0);
        hMass[i]->Sumw2();
    }

    for (int i = 0; i < tree->GetEntries(); i++){
        tree->GetEntry(i);
        var->FillBDTtree(bdt);
        for (int j = 0; j < NBDT; j++){
            if (var->value[QuadVarManager::Variables::kBDT] > (j*step + start)){
                hMass[j]->Fill(var->value[QuadVarManager::Variables::kMass]);
            }
        }
    }

    system("mkdir -p output/BDTScan");
    for (int j = 0; j < NBDT; j++){
        string outputpath = Form("output/BDTScan/BDT_%.2f.pdf", j*step + start);
        SignalExtraction_X3872(hMass[j], 3.6, 4.0, outputpath);
    }
}