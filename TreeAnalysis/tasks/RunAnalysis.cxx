// this code is for applying the cut on the data and simulation,
// calculate the Acceptance,
// show the QA plots (distribution and comparison),
// Run Signal Extraction
// and Create the skimmed tree for the further analysis.

#include "../deps/HistoAndPlot.h"
#include "../deps/QaudVarManager.h"
#include "../deps/QuadAnalysisCuts.h"
#include <TF1.h>

#include "../deps/CutsLibrary.cxx"

// void RunAnalysis(TString dataPath, TString dataTag, TString simPath, TString simTag, TString cutName)
void RunAnalysis()
{
    // init 
    std::vector<string> dataPathList = {"SkimmedTree/Data_2022.root", "SkimmedTree/Data_2023.root", "SkimmedTree/Data_2024.root"};
    std::vector<string> dataNameList = {"2022", "2023", "2024"};
    std::vector<string> simPathList = {"SkimmedTree/X3872_JpsiRho_Swave.root", "SkimmedTree/Psi2S_VVPIPI.root"};
    std::vector<string> simNameList = {"X(3872)", "#Psi(2S)"};
    std::vector<string> CutNameList = {"debug1", "debug2"};
    TFile* myfile = new TFile("output/Analysis.root", "recreate");

    // read data
    for (int i = 0; i < dataPathList.size(); i++){
        TFile* file = new TFile(dataPathList[i].c_str());
        SkimmedTree* tree = new SkimmedTree((TTree*)file->Get("tree"));
        QaudVarManager* var = new QaudVarManager();
        
        for (int j = 0; j < CutNameList.size(); j++){
            ClassicalCuts* cut = FindCut(CutNameList[j]);
            TObjArray* array = new TObjArray();
            DefineHistograms(array);
            for (int k = 0; k < tree->fChain->GetEntries(); k++) {
                tree->fChain->GetEntry(k);
                var->FillSkimmedTree(tree);
                if (cut->isInCut(var->value)){
                    FillHistograms(array, var->value);
                }
            }
            WriteHistograms(array, myfile, Form("%s_%s", dataNameList[i].c_str(), CutNameList[j].c_str()));
            // clear array
            array->Clear("C");
        }
    }

    // read simulation
    for (int i = 0; i < simPathList.size(); i++){
        TFile* file = new TFile(simPathList[i].c_str());
        SkimmedTree* tree = new SkimmedTree((TTree*)file->Get("tree"));
        QaudVarManager* var = new QaudVarManager();

        // before selection
        TObjArray* arraybf = new TObjArray();
        DefineHistograms(arraybf);
        for (int k = 0; k < tree->fChain->GetEntries(); k++) {
            tree->fChain->GetEntry(k);
            var->FillSkimmedTree(tree);
            FillHistograms(arraybf, var->value);
        }
        WriteHistograms(arraybf, myfile, Form("%s_before", simNameList[i].c_str()));
        arraybf->Clear("C");
        
        for (int j = 0; j < CutNameList.size(); j++){
            ClassicalCuts* cut = FindCut(CutNameList[j]);
            TObjArray* array = new TObjArray();
            DefineHistograms(array);
            for (int k = 0; k < tree->fChain->GetEntries(); k++) {
                tree->fChain->GetEntry(k);
                var->FillSkimmedTree(tree);
                if (cut->isInCut(var->value)){
                    FillHistograms(array, var->value);
                }
            }
            WriteHistograms(array, myfile, Form("%s_%s", simNameList[i].c_str(), CutNameList[j].c_str()));
            array->Clear("C");
        }
    }
}