#include "../deps/QuadAnalysisCuts.h"
#include "../deps/QuadVarManager.h"
#include "../deps/CutsLibrary.cxx"

// convert the EvtgenSimTree to SkimmedTree
void RunSimTreeMaker(TTree* tree, string tag){
    string outputfileName = Form("SkimmedTree/%s.root", tag.c_str());
    EvtgenSim* Sim = new EvtgenSim(tree);
    QuadVarManager* var = new QuadVarManager(outputfileName);

    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        var->FillSim(Sim);
        var->FillToTree();
    }

    var->WriteToFile();
}

// convert the QuadpletTree to SkimmedTree
void RunDataTreeMakerWithFilter(TTree* tree, string tag, string cutname){
    string outputfileName = Form("SkimmedTree/%s_%s.root", tag.c_str(), cutname.c_str());
    Quadplet* Data = new Quadplet(tree);
    QuadVarManager* var = new QuadVarManager(outputfileName);
    QuadAnalysisCuts* cut = FindCut(cutname);

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

void RunTreeMaker() {
    system("mkdir -p SkimmedTree");

    TFile* file_Data_2022 = new TFile("input/LHC22_pass4.root");
    TTree* tree_Data_2022 = (TTree*)file_Data_2022->Get("DF_2271170122563072/O2rtdquadplet");
    RunDataTreeMakerWithFilter(tree_Data_2022, "Data_2022", "data_prefilter");
    file_Data_2022->Close();

    TFile* file_Data_2023 = new TFile("input/LHC23_pass4.root");
    TTree* tree_Data_2023 = (TTree*)file_Data_2023->Get("DF_2298103926612928/O2rtdquadplet");
    RunDataTreeMakerWithFilter(tree_Data_2023, "Data_2023", "data_prefilter");
    file_Data_2023->Close();

    TFile* file_Data_2024 = new TFile("input/LHC24_pass1.root");
    TTree* tree_Data_2024 = (TTree*)file_Data_2024->Get("DF_2372458388316160/O2rtdquadplet");
    RunDataTreeMakerWithFilter(tree_Data_2024, "Data_2024", "data_prefilter");
    file_Data_2024->Close();

    TFile* file_JpsiPiPi_mydef = new TFile("input/X3872_JpsiRho_Swave.root");
    TTree* tree_JpsiPiPi_mydef = (TTree*)file_JpsiPiPi_mydef->Get("tree");
    RunSimTreeMaker(tree_JpsiPiPi_mydef, "X3872_JpsiRho_Swave");
    file_JpsiPiPi_mydef->Close();

    TFile* file_Psi2S = new TFile("input/Psi2S_VVPIPI.root");
    TTree* tree_Psi2S = (TTree*)file_Psi2S->Get("tree");
    RunSimTreeMaker(tree_Psi2S, "Psi2S_VVPIPI");
    file_Psi2S->Close();
}