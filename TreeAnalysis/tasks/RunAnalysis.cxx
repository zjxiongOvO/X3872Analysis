// this code is for applying the cut on the data and simulation,
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
    string AnalysisName;
    string dataPath;
    string SameSidePath;
    string X3872ToyMCFilePath;
    string Psi2SToyMCFilePath;
    std::vector<string> AnalysisCutsName;
    bool NeedSaveTree = false;
    string commonpath;

    try {
        json config = readConfig(configpath);
        AnalysisName = config["GlobalSetting"]["AnalysisName"];
        dataPath = config["Analysis"]["DataPath"];
        SameSidePath = config["Analysis"]["SameSidePath"];
        X3872ToyMCFilePath = config["Analysis"]["X3872ToyMCFilePath"];
        Psi2SToyMCFilePath = config["Analysis"]["Psi2SToyMCFilePath"];
        AnalysisCutsName = config["Analysis"]["AnalysisCutsName"].get<std::vector<string>>();
        NeedSaveTree = config["Analysis"]["NeedSaveTree"].get<bool>();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    commonpath = Form("output/%s", AnalysisName.c_str());

    TFile* myfile = new TFile(Form("%s/Analysis.root", commonpath.c_str()), "update");

    // read data
    TFile* datafile = new TFile(Form("%s/%s", commonpath.c_str(), dataPath.c_str()));
    SkimmedTree* datatree = new SkimmedTree((TTree*)datafile->Get("tree"));
    QuadVarManager* datavar = new QuadVarManager();

    for (int i = 0; i < AnalysisCutsName.size(); i++){
        if (NeedSaveTree) datavar->CreateTree(Form("%s/SkimmedTree/Data_%s.root", commonpath.c_str(), AnalysisCutsName[i].c_str()));
        QuadAnalysisCuts* cut = FindCut(AnalysisCutsName[i]);
        TObjArray* array = new TObjArray();
        QuadHistos::DefineHistograms(array);
        for (int k = 0; k < datatree->fChain->GetEntries(); k++) {
            datatree->fChain->GetEntry(k);
            datavar->FillSkimmedTree(datatree);
            if (cut->isInCut(datavar->value)){
                QuadHistos::FillHistograms(array, datavar->value);
                if (NeedSaveTree) datavar->FillToTree();
            }
        }
        QuadHistos::WriteHistograms(array, myfile, Form("Data_%s", AnalysisCutsName[i].c_str()));
        array->Clear("C");
    }
    datafile->Close();

    // read SameSide bkg
    TFile* samesidefile = new TFile(Form("%s/%s", commonpath.c_str(), SameSidePath.c_str()));
    SkimmedTree* samesidetree = new SkimmedTree((TTree*)samesidefile->Get("tree"));
    QuadVarManager* samesidevar = new QuadVarManager();

    for (int i = 0; i < AnalysisCutsName.size(); i++){
        if (NeedSaveTree) samesidevar->CreateTree(Form("%s/SkimmedTree/SameSide_%s.root", commonpath.c_str(), AnalysisCutsName[i].c_str()));
        QuadAnalysisCuts* cut = FindCut(AnalysisCutsName[i]);
        TObjArray* array = new TObjArray();
        QuadHistos::DefineHistograms(array);
        for (int k = 0; k < samesidetree->fChain->GetEntries(); k++) {
            samesidetree->fChain->GetEntry(k);
            samesidevar->FillSkimmedTree(samesidetree);
            if (cut->isInCut(samesidevar->value)){
                QuadHistos::FillHistograms(array, samesidevar->value);
                if (NeedSaveTree) samesidevar->FillToTree();
            }
        }
        QuadHistos::WriteHistograms(array, myfile, Form("SameSide_%s", AnalysisCutsName[i].c_str()));
        array->Clear("C");
    }
    samesidefile->Close();

    // read simulation
    // X3872
    TFile* X3872file = new TFile(Form("%s/%s", commonpath.c_str(), X3872ToyMCFilePath.c_str()));
    SkimmedTree* X3872tree = new SkimmedTree((TTree*)X3872file->Get("tree"));
    QuadVarManager* X3872var = new QuadVarManager();

    for (int i = 0; i < AnalysisCutsName.size(); i++){
        if (NeedSaveTree) X3872var->CreateTree(Form("%s/SkimmedTree/X3872_%s.root", commonpath.c_str(), AnalysisCutsName[i].c_str()));
        QuadAnalysisCuts* cut = FindCut(AnalysisCutsName[i]);
        TObjArray* array = new TObjArray();
        QuadHistos::DefineHistograms(array);
        for (int k = 0; k < X3872tree->fChain->GetEntries(); k++) {
            X3872tree->fChain->GetEntry(k);
            X3872var->FillSkimmedTree(X3872tree);
            if (cut->isInCut(X3872var->value)){
                QuadHistos::FillHistograms(array, X3872var->value);
                if (NeedSaveTree) X3872var->FillToTree();
            }
        }
        QuadHistos::WriteHistograms(array, myfile, Form("X3872_%s", AnalysisCutsName[i].c_str()));
        array->Clear("C");
    }
    X3872file->Close();

    // Psi2S
    TFile* Psi2Sfile = new TFile(Form("%s/%s", commonpath.c_str(), Psi2SToyMCFilePath.c_str()));
    SkimmedTree* Psi2Stree = new SkimmedTree((TTree*)Psi2Sfile->Get("tree"));
    QuadVarManager* Psi2Svar = new QuadVarManager();

    for (int i = 0; i < AnalysisCutsName.size(); i++){
        if (NeedSaveTree) Psi2Svar->CreateTree(Form("%s/SkimmedTree/Psi2S_%s.root", commonpath.c_str(), AnalysisCutsName[i].c_str()));
        QuadAnalysisCuts* cut = FindCut(AnalysisCutsName[i]);
        TObjArray* array = new TObjArray();
        QuadHistos::DefineHistograms(array);
        for (int k = 0; k < Psi2Stree->fChain->GetEntries(); k++) {
            Psi2Stree->fChain->GetEntry(k);
            Psi2Svar->FillSkimmedTree(Psi2Stree);
            if (cut->isInCut(Psi2Svar->value)){
                QuadHistos::FillHistograms(array, Psi2Svar->value);
                if (NeedSaveTree) Psi2Svar->FillToTree();
            }
        }
        QuadHistos::WriteHistograms(array, myfile, Form("Psi2S_%s", AnalysisCutsName[i].c_str()));
        array->Clear("C");
    }
    Psi2Sfile->Close();

    cout<<"Analysis finished!"<<endl;
}