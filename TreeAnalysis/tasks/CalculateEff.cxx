#include "../deps/json.hpp"
#include "../deps/HistoAndPlot.h"
#include <TEfficiency.h>

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

void AddPreselEfficiencyFromToyMC(TFile* AnalysisResult, string SpecialToyMCCutName, string PreselCutName, TFile* EfficiencyResult){
    // get histograms
    // check if the histograms exist
    if (!AnalysisResult->Get(Form("X3872_%s_%s", SpecialToyMCCutName.c_str(), PreselCutName.c_str()))) {
        return;
    }
    //  X3872
    TList* list_X3872_before = (TList*)AnalysisResult->Get(Form("X3872_before"));
    TList* list_X3872_after = (TList*)AnalysisResult->Get(Form("X3872_%s_%s", SpecialToyMCCutName.c_str(), PreselCutName.c_str()));
    TH1F* hTotal_pT_X3872 = (TH1F*)list_X3872_before->At(hPt);
    TH1F* hPass_pT_X3872 = (TH1F*)list_X3872_after->At(hPt);
    TEfficiency* eff_pT_X3873 = new TEfficiency(*hPass_pT_X3872, *hTotal_pT_X3872);
    eff_pT_X3873->SetName("PreselEfficiency_X3872_Pt");
    //  Psi2S
    TList* list_Psi2S_before = (TList*)AnalysisResult->Get(Form("Psi2S_before"));
    TList* list_Psi2S_after = (TList*)AnalysisResult->Get(Form("Psi2S_%s_%s", SpecialToyMCCutName.c_str(), PreselCutName.c_str()));
    TH1F* hTotal_pT_Psi2S = (TH1F*)list_Psi2S_before->At(hPt);
    TH1F* hPass_pT_Psi2S = (TH1F*)list_Psi2S_after->At(hPt);
    TEfficiency* eff_pT_Psi2S = new TEfficiency(*hPass_pT_Psi2S, *hTotal_pT_Psi2S);
    eff_pT_Psi2S->SetName("PreselEfficiency_Psi2S_Pt");

    // save efficiency
    EfficiencyResult->cd();
    eff_pT_X3873->Write();
    eff_pT_Psi2S->Write();
}

void AddAnalysisEfficiencyFromToyMC(TFile* AnalysisResult, string AnalysisCutName,  TFile* EfficiencyResult){
    // get histograms
    // check if the histograms exist
    if (!AnalysisResult->Get(Form("X3872_%s", AnalysisCutName.c_str()))) {
        return;
    }
    //  X3872
    TList* list_X3872_before = (TList*)AnalysisResult->Get(Form("X3872_before"));
    TList* list_X3872_after = (TList*)AnalysisResult->Get(Form("X3872_%s", AnalysisCutName.c_str()));
    TH1F* hTotal_pT_X3872 = (TH1F*)list_X3872_before->At(hPt);
    TH1F* hPass_pT_X3872 = (TH1F*)list_X3872_after->At(hPt);
    TEfficiency* eff_pT_X3873 = new TEfficiency(*hPass_pT_X3872, *hTotal_pT_X3872);
    eff_pT_X3873->SetName(Form("AnalysisEfficiency_X3872_%s_Pt", AnalysisCutName.c_str()));
    //  Psi2S
    TList* list_Psi2S_before = (TList*)AnalysisResult->Get(Form("Psi2S_before"));
    TList* list_Psi2S_after = (TList*)AnalysisResult->Get(Form("Psi2S_%s", AnalysisCutName.c_str()));
    TH1F* hTotal_pT_Psi2S = (TH1F*)list_Psi2S_before->At(hPt);
    TH1F* hPass_pT_Psi2S = (TH1F*)list_Psi2S_after->At(hPt);
    TEfficiency* eff_pT_Psi2S = new TEfficiency(*hPass_pT_Psi2S, *hTotal_pT_Psi2S);
    eff_pT_Psi2S->SetName(Form("AnalysisEfficiency_Psi2S_%s_Pt", AnalysisCutName.c_str()));

    // save
    EfficiencyResult->cd();
    eff_pT_X3873->Write();
    eff_pT_Psi2S->Write();
}


void CalculateEff(string configpath) {
     // init
    string AnalysisName;
    string PreselCutName;
    string SpecialToyMCCutName;
    std::vector<string> AnalysisCutsName;

    // read config
    try {
        json config = readConfig(configpath);
        AnalysisName = config["GlobalSetting"]["AnalysisName"];
        PreselCutName = config["SkimmedTreeProduction"]["PreselCutName"];
        SpecialToyMCCutName = config["SkimmedTreeProduction"]["SpecialToyMCCutName"];
        AnalysisCutsName = config["Analysis"]["AnalysisCutsName"].get<std::vector<string>>();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    TFile* file = new TFile(Form("output/%s/Analysis.root", AnalysisName.c_str()), "read");
    TFile* file_eff = new TFile(Form("output/%s/Efficiency.root", AnalysisName.c_str()), "recreate");
    
    // add efficiency
    AddPreselEfficiencyFromToyMC(file, SpecialToyMCCutName, PreselCutName, file_eff);

    // add analysis efficiency
    for (int i = 0; i < AnalysisCutsName.size(); i++){
        AddAnalysisEfficiencyFromToyMC(file, AnalysisCutsName[i], file_eff);
    }

    file->Close();
    file_eff->Close();

}