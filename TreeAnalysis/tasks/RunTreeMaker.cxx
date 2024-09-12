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

// convert the EvtgenSimTree to SkimmedTree
void RunSimTreeMaker(TTree* tree, string tag){
    string outputfileName = Form("SkimmedTree/%s.root", tag.c_str());
    EvtgenSim* Sim = new EvtgenSim(tree);
    QuadVarManager* var = new QuadVarManager();
    var->CreateTree(outputfileName);

    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        var->FillSim(Sim);
        var->FillToTree();
    }

    var->WriteToFile();
}

// convert the EvtgenSimTree to SkimmedTree
void RunSimTreeMakerWithFilter(TTree* tree, string tag, string cutname){
    string outputfileName = Form("SkimmedTree/%s_%s.root", tag.c_str(), cutname.c_str());
    EvtgenSim* Sim = new EvtgenSim(tree);
    QuadVarManager* var = new QuadVarManager();
    QuadAnalysisCuts* cut = FindCut(cutname);
    var->CreateTree(outputfileName);

    // histo
    TObjArray* array_bf = new TObjArray();
    QuadHistos::DefineHistograms(array_bf);

    TObjArray* array_af = new TObjArray();
    QuadHistos::DefineHistograms(array_af);

    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        var->FillSim(Sim);
        QuadHistos::FillHistograms(array_bf, var->value);
        if (!cut->isInCut(var->value)) continue;
        QuadHistos::FillHistograms(array_af, var->value);
        var->FillToTree();
    }

    var->WriteToFile();

    TFile* file = new TFile("output/Analysis.root", "update");
    QuadHistos::WriteHistograms(array_bf, file, Form("%s_before", tag.c_str()));
    QuadHistos::WriteHistograms(array_af, file, Form("%s_%s", tag.c_str(), cutname.c_str()));
    file->Close();
}

// convert the QuadpletTree to SkimmedTree
void RunDataTreeMakerWithFilter(TTree* tree, string tag, string cutname){
    string outputfileName = Form("SkimmedTree/%s_%s.root", tag.c_str(), cutname.c_str());
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

// convert the QuadpletTree to SkimmedTree (Bkg)
void RunDataTreeMakerWithFilterBkg(TTree* tree, string tag, string cutname){
    string outputfileName = Form("SkimmedTree/%s_%s_sameside.root", tag.c_str(), cutname.c_str());
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
    system("mkdir -p SkimmedTree");

    // init
    string filepath;
    string treepath;
    string tag;
    string cutname;
    int RunType;

    // read config
    try {
        json config = readConfig(configpath);
        filepath = config["FilePath"].get<std::string>();
        treepath = config["TreePath"].get<std::string>();
        tag = config["Tag"].get<std::string>();
        cutname = config["CutName"].get<std::string>();
        RunType = config["RunType"].get<int>();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    TFile* file = new TFile(filepath.c_str(), "read");
    TTree* tree = (TTree*)file->Get(treepath.c_str());
    // run
    if (RunType == 0) RunSimTreeMaker(tree, tag);
    else if (RunType == 1) RunSimTreeMakerWithFilter(tree, tag, cutname);
    else if (RunType == 2) RunDataTreeMakerWithFilter(tree, tag, cutname);
    else if (RunType == 3) RunDataTreeMakerWithFilterBkg(tree, tag, cutname);
    else std::cerr << "Invalid RunType" << std::endl;
    file->Close();

    std::cout << "Finished!" << std::endl;

    // TFile* outputfile = new TFile("output/Analysis.root", "read");
    // TList* list_bf = (TList*)outputfile->Get(Form("%s_before", tag.c_str()));
    // TList* list_af = (TList*)outputfile->Get(Form("%s_%s", tag.c_str(), cutname.c_str()));
    // std::vector<TList*> list_before;
    // list_before.push_back(list_bf);
    // std::vector<TList*> list_after;
    // list_after.push_back(list_af);
    // std::vector<string> ListName;
    // ListName.push_back(tag);
    // QuadHistos::DrawEfficiency(list_after, list_before, ListName);
}