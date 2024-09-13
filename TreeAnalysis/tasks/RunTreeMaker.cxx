// this code is for converting the tree to the skimmed tree with the cut applied.
// and save the efficiency histogram for the further analysis.

#include "../deps/QuadAnalysisCuts.h"
#include "../deps/QuadVarManager.h"
#include "../deps/CutsLibrary.cxx"
#include "../deps/json.hpp"
#include "../deps/HistoAndPlot.h"

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

// convert the EvtgenSimTree to SkimmedTree with a filter, and save the efficiency file
void RunSimTreeMakerWithFilter(TTree* tree, string tag, string signalname ,string Specialcutname, string preselcutname){
    string outputfileName = Form("%s/SkimmedTree/%s_%s_%s.root", tag.c_str(), signalname.c_str(), Specialcutname.c_str(), preselcutname.c_str());
    EvtgenSim* Sim = new EvtgenSim(tree);
    QuadVarManager* var = new QuadVarManager();
    QuadAnalysisCuts* Specialcut = FindCut(Specialcutname);
    QuadAnalysisCuts* Preselcut = FindCut(preselcutname);
    var->CreateTree(outputfileName);

    TObjArray* array_bf = new TObjArray();
    array_bf->SetOwner();
    QuadHistos::DefineHistograms(array_bf);

    TObjArray* array_af = new TObjArray();
    array_af->SetOwner();
    QuadHistos::DefineHistograms(array_af);

    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        var->FillSim(Sim);
        QuadHistos::FillHistograms(array_bf, var->value);
        if (!Specialcut->isInCut(var->value)) continue;
        if (!Preselcut->isInCut(var->value)) continue;
        QuadHistos::FillHistograms(array_af, var->value);
        var->FillToTree();
    }

    var->WriteToFile();

    TFile* file = new TFile(Form("%s/Analysis.root", tag.c_str()), "update");
    QuadHistos::WriteHistograms(array_bf, file, Form("%s_before", signalname.c_str()));
    QuadHistos::WriteHistograms(array_af, file, Form("%s_%s_%s", signalname.c_str(), Specialcutname.c_str(), preselcutname.c_str()));
    file->Close();
}

// convert the QuadpletTree to SkimmedTree with a filter
void RunDataTreeMakerWithFilter(TTree* tree, string tag, string cutname){
    string outputfileName = Form("%s/SkimmedTree/Data_%s.root", tag.c_str(), cutname.c_str());
    Quadplet* Data = new Quadplet(tree);
    QuadVarManager* var = new QuadVarManager();
    QuadAnalysisCuts* cut = FindCut(cutname);
    var->CreateTree(outputfileName);

    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        var->FillData(Data);
        if (!(var->value[QuadVarManager::kDileptonSign] == 0)) continue;
        if (!(var->value[QuadVarManager::kDihardonSign] == 0)) continue;
        if (!(var->value[QuadVarManager::kMass] > 3.6 && var->value[QuadVarManager::kMass] < 4.0)) continue;
        if (!cut->isInCut(var->value)) continue;
        var->FillToTree();
    }

    var->WriteToFile();
}

// convert the QuadpletTree to SkimmedTree with a filter for same side background
void RunDataTreeMakerWithFilterBkg(TTree* tree, string tag, string cutname){
    string outputfileName = Form("%s/SkimmedTree/Data_%s_sameside.root", tag.c_str(), cutname.c_str());
    Quadplet* Data = new Quadplet(tree);
    QuadVarManager* var = new QuadVarManager();
    QuadAnalysisCuts* cut = FindCut(cutname);
    var->CreateTree(outputfileName);

    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        var->FillData(Data);
        if (!(var->value[QuadVarManager::kDileptonSign] == 0)) continue;
        if (!(var->value[QuadVarManager::kDihardonSign] != 0)) continue;
        if (!(var->value[QuadVarManager::kMass] > 3.6 && var->value[QuadVarManager::kMass] < 4.0)) continue;
        if (!cut->isInCut(var->value)) continue;
        var->FillToTree();
    }

    var->WriteToFile();
}

void RunTreeMaker(string configpath) {
    // init
    string AnalysisName;
    string datafilepath;
    string datatreepath;
    string X3872ToyMCFilePath;
    string X3872ToyMCTreeName;
    string Psi2SToyMCFilePath;
    string Psi2SToyMCTreeName;
    string PreselCutName;
    string SpecialToyMCCutName;

    // read config
    try {
        json config = readConfig(configpath);
        AnalysisName = config["GlobalSetting"]["AnalysisName"];
        datafilepath = config["SkimmedTreeProduction"]["QuadletFilePath"];
        datatreepath = config["SkimmedTreeProduction"]["QuadletTreeName"];
        X3872ToyMCFilePath = config["SkimmedTreeProduction"]["X3872ToyMCFilePath"];
        X3872ToyMCTreeName = config["SkimmedTreeProduction"]["X3872ToyMCTreeName"];
        Psi2SToyMCFilePath = config["SkimmedTreeProduction"]["Psi2SToyMCFilePath"];
        Psi2SToyMCTreeName = config["SkimmedTreeProduction"]["Psi2SToyMCTreeName"];
        PreselCutName = config["SkimmedTreeProduction"]["PreselCutName"];
        SpecialToyMCCutName = config["SkimmedTreeProduction"]["SpecialToyMCCutName"];
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    system(Form("mkdir -p output/%s/SkimmedTree", AnalysisName.c_str()));

    // run data with presel cut, create data-tree and bkg-tree
    TFile* datafile = new TFile(datafilepath.c_str(), "read");
    TTree* datatree = (TTree*)datafile->Get(datatreepath.c_str());
    RunDataTreeMakerWithFilter(datatree, Form("output/%s", AnalysisName.c_str()), PreselCutName);
    RunDataTreeMakerWithFilterBkg(datatree, Form("output/%s", AnalysisName.c_str()), PreselCutName);
    datafile->Close();

    // run X3872 toy MC with special cut and presel cut, create X3872-toyMC-tree
    TFile* X3872ToyMCFile = new TFile(X3872ToyMCFilePath.c_str(), "read");
    TTree* X3872ToyMCTree = (TTree*)X3872ToyMCFile->Get(X3872ToyMCTreeName.c_str());
    RunSimTreeMakerWithFilter(X3872ToyMCTree, Form("output/%s", AnalysisName.c_str()), "X3872", SpecialToyMCCutName, PreselCutName);
    X3872ToyMCFile->Close();

    // run Psi2S toy MC with special cut and presel cut, create Psi2S-toyMC-tree
    TFile* Psi2SToyMCFile = new TFile(Psi2SToyMCFilePath.c_str(), "read");
    TTree* Psi2SToyMCTree = (TTree*)Psi2SToyMCFile->Get(Psi2SToyMCTreeName.c_str());
    RunSimTreeMakerWithFilter(Psi2SToyMCTree, Form("output/%s", AnalysisName.c_str()), "Psi2S", SpecialToyMCCutName, PreselCutName);
    Psi2SToyMCFile->Close();

    std::cout << "TreeMaker finished!" << std::endl;
}