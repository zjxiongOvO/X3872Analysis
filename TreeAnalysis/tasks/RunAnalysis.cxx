// this code is for applying the cut on the data and simulation,
// calculate the Acceptance,
// show the QA plots (distribution and comparison),
// Run Signal Extraction
// and Create the skimmed tree for the further analysis.

#include "../deps/HistoAndPlot.h"
#include "../deps/QuadAnalysisCuts.h"
#include "../deps/QuadVarManager.h"
#include "../deps/json.hpp"
#include <TF1.h>

#include "../deps/CutsLibrary.cxx"

using json = nlohmann::json;

json readConfig(const std::string& configFilePath) {
    std::ifstream configFile(configFilePath);
    if (!configFile) {
        throw std::runtime_error("Cannot open config file: " + configFilePath);
    }

    json config;
    configFile >> config;
    return config;
}

void RunAnalysis(string configpath)
{
    // init 
    std::vector<string> dataPathList;
    std::vector<string> SameSidePathList;
    std::vector<string> dataNameList;
    std::vector<string> simPathList;
    std::vector<string> simNameList;
    std::vector<string> CutNameList;
    bool NeedSaveTree = false;
    string SaveTreePath = "";
    bool NeedUpdateFile = false;
    bool NeedSameSide = false;

    try {
        json config = readConfig(configpath);
        dataPathList = config["DataPath"].get<std::vector<std::string>>();
        SameSidePathList = config["SameSidePath"].get<std::vector<std::string>>();
        dataNameList = config["DataName"].get<std::vector<std::string>>();
        simPathList = config["SimPath"].get<std::vector<std::string>>();
        simNameList = config["SimName"].get<std::vector<std::string>>();
        CutNameList = config["CutName"].get<std::vector<std::string>>();
        NeedSaveTree = config["NeedSaveTree"].get<bool>();
        SaveTreePath = config["SaveTreePath"].get<std::string>();
        NeedUpdateFile = config["NeedUpdateFile"].get<bool>();
        NeedSameSide = config["NeedSameSide"].get<bool>();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    TFile* myfile;
    if (NeedUpdateFile) myfile = new TFile("output/Analysis.root", "update");
    else myfile = new TFile("output/Analysis.root", "recreate");

    system(Form("mkdir -p %s", SaveTreePath.c_str()));

    // read data
    for (int i = 0; i < dataPathList.size(); i++){
        TFile* file = new TFile(dataPathList[i].c_str());
        SkimmedTree* tree = new SkimmedTree((TTree*)file->Get("tree"));
        QuadVarManager* var = new QuadVarManager();
        
        for (int j = 0; j < CutNameList.size(); j++){
            if (NeedSaveTree) var->CreateTree(Form("%s/%s_%s.root", SaveTreePath.c_str(), dataNameList[i].c_str(), CutNameList[j].c_str()));
            QuadAnalysisCuts* cut = FindCut(CutNameList[j]);
            TObjArray* array = new TObjArray();
            QuadHistos::DefineHistograms(array);
            for (int k = 0; k < tree->fChain->GetEntries(); k++) {
                tree->fChain->GetEntry(k);
                var->FillSkimmedTree(tree);
                if (cut->isInCut(var->value)){
                    QuadHistos::FillHistograms(array, var->value);
                    if (NeedSaveTree) var->FillToTree();
                }
            }
            QuadHistos::WriteHistograms(array, myfile, Form("%s_%s", dataNameList[i].c_str(), CutNameList[j].c_str()));
            array->Clear("C");
        }
    }

    // read SameSide bkg
    if (NeedSameSide == true) {
        for (int i = 0; i < SameSidePathList.size(); i++){
            TFile* file = new TFile(SameSidePathList[i].c_str());
            SkimmedTree* tree = new SkimmedTree((TTree*)file->Get("tree"));
            QuadVarManager* var = new QuadVarManager();
            
            for (int j = 0; j < CutNameList.size(); j++){
                if (NeedSaveTree) var->CreateTree(Form("%s/%s_%s.root", SaveTreePath.c_str(), dataNameList[i].c_str(), CutNameList[j].c_str()));
                QuadAnalysisCuts* cut = FindCut(CutNameList[j]);
                TObjArray* array = new TObjArray();
                QuadHistos::DefineHistograms(array);
                for (int k = 0; k < tree->fChain->GetEntries(); k++) {
                    tree->fChain->GetEntry(k);
                    var->FillSkimmedTree(tree);
                    if (cut->isInCut(var->value)){
                        QuadHistos::FillHistograms(array, var->value);
                        if (NeedSaveTree) var->FillToTree();
                    }
                }
                QuadHistos::WriteHistograms(array, myfile, Form("%s_%s_bkg", dataNameList[i].c_str(), CutNameList[j].c_str()));
                array->Clear("C");
            }
        }
    }

    // read simulation
    for (int i = 0; i < simPathList.size(); i++){
        TFile* file = new TFile(simPathList[i].c_str());
        SkimmedTree* tree = new SkimmedTree((TTree*)file->Get("tree"));
        QuadVarManager* var = new QuadVarManager();

        // before selection
        TObjArray* arraybf = new TObjArray();
        QuadHistos::DefineHistograms(arraybf);
        for (int k = 0; k < tree->fChain->GetEntries(); k++) {
            tree->fChain->GetEntry(k);
            var->FillSkimmedTree(tree);
            QuadHistos::FillHistograms(arraybf, var->value);
        }
        QuadHistos::WriteHistograms(arraybf, myfile, Form("%s_before", simNameList[i].c_str()));
        arraybf->Clear("C");
        
        for (int j = 0; j < CutNameList.size(); j++){
            if (NeedSaveTree) var->CreateTree(Form("%s/%s_%s.root", SaveTreePath.c_str(), simNameList[i].c_str(), CutNameList[j].c_str()));
            QuadAnalysisCuts* cut = FindCut(CutNameList[j]);
            TObjArray* array = new TObjArray();
            QuadHistos::DefineHistograms(array);
            for (int k = 0; k < tree->fChain->GetEntries(); k++) {
                tree->fChain->GetEntry(k);
                var->FillSkimmedTree(tree);
                if (cut->isInCut(var->value)){
                    QuadHistos::FillHistograms(array, var->value);
                    if (NeedSaveTree) var->FillToTree();
                }
            }
            QuadHistos::WriteHistograms(array, myfile, Form("%s_%s", simNameList[i].c_str(), CutNameList[j].c_str()));
            array->Clear("C");
        }
    }
}