// this is a task to run the signal extraction for the X(3872) analysis

#include "../deps/HistoAndPlot.h"
#include "../deps/SignalExtraction_X3872.cxx"
#include "../deps/json.hpp"

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

void RunSignalExtraction(const std::string& configFilePath) {
    string outputpath;
    string AnalysisName;
    std::vector<string> AnalysisCutsName;
    std::vector<double> Ptbins;
    double BinWidth;
    std::vector<double> MassRangeX3872;
    std::vector<double> MassRangePsi2S;

    try {
        json config = readConfig(configFilePath);
        AnalysisName = config["GlobalSetting"]["AnalysisName"].get<std::string>();
        AnalysisCutsName = config["SignalExtraction"]["AnalysisCutsName"].get<std::vector<std::string>>();
        Ptbins = config["SignalExtraction"]["Ptbins"].get<std::vector<double>>();
        BinWidth = config["SignalExtraction"]["BinWidth"].get<double>();
        MassRangeX3872 = config["SignalExtraction"]["MassRangeX3872"].get<std::vector<double>>();
        MassRangePsi2S = config["SignalExtraction"]["MassRangePsi2S"].get<std::vector<double>>();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    TFile* myfile = new TFile(Form("output/%s/Analysis.root", AnalysisName.c_str()), "read");
    system(Form("mkdir -p output/%s/SigExtraction", AnalysisName.c_str()));

    // get histo
    for (int i = 0; i < AnalysisCutsName.size(); i++){
        TList* list = (TList*)myfile->Get(Form("Data_%s", AnalysisCutsName[i].c_str()));
        TH2F* hMass_Pt = (TH2F*)list->At(histoname::hMass_Pt)->Clone();
        hMass_Pt->SetName(Form("hMass_Pt_%s", AnalysisCutsName[i].c_str()));

        TList* list_sameside = (TList*)myfile->Get(Form("SameSide_%s", AnalysisCutsName[i].c_str()));
        TH2F* hMass_Pt_sameside = (TH2F*)list_sameside->At(histoname::hMass_Pt)->Clone();
        hMass_Pt_sameside->SetName(Form("hMass_Pt_sameside_%s", AnalysisCutsName[i].c_str()));

        // project to pT bins
        for (int j = 0; j < Ptbins.size()-1; j++){
            TH1F* hUnlikeSign = (TH1F*)hMass_Pt->ProjectionY(Form("hUnlikeSign_%s_%d_%d", AnalysisCutsName[i].c_str(), (int)Ptbins[j], (int)Ptbins[j+1]), hMass_Pt->GetXaxis()->FindBin(Ptbins[j]), hMass_Pt->GetXaxis()->FindBin(Ptbins[j+1]));
            hUnlikeSign->SetName(Form("hUnlikeSign_%s_%d_%d", AnalysisCutsName[i].c_str(), (int)Ptbins[j], (int)Ptbins[j+1]));
            TH1F* hUnlikeSign_sameside = (TH1F*)hMass_Pt_sameside->ProjectionY(Form("hUnlikeSign_sameside_%s_%d_%d", AnalysisCutsName[i].c_str(), (int)Ptbins[j], (int)Ptbins[j+1]), hMass_Pt_sameside->GetXaxis()->FindBin(Ptbins[j]), hMass_Pt_sameside->GetXaxis()->FindBin(Ptbins[j+1]));
            hUnlikeSign_sameside->SetName(Form("hUnlikeSign_sameside_%s_%d_%d", AnalysisCutsName[i].c_str(), (int)Ptbins[j], (int)Ptbins[j+1]));
        
            // signal extraction
            SignalExtraction_X3872(hUnlikeSign, hUnlikeSign_sameside, MassRangeX3872[0], MassRangeX3872[1], BinWidth, Ptbins[j], Ptbins[j+1], "X3872", "fitdirect", Form("output/%s/SigExtraction", AnalysisName.c_str()));
            SignalExtraction_X3872(hUnlikeSign, hUnlikeSign_sameside, MassRangeX3872[0], MassRangeX3872[1], BinWidth, Ptbins[j], Ptbins[j+1], "X3872", "withbkg", Form("output/%s/SigExtraction", AnalysisName.c_str()));
            SignalExtraction_X3872(hUnlikeSign, hUnlikeSign_sameside, MassRangePsi2S[0], MassRangePsi2S[1], BinWidth, Ptbins[j], Ptbins[j+1], "Psi2S", "fitdirect", Form("output/%s/SigExtraction", AnalysisName.c_str()));
            SignalExtraction_X3872(hUnlikeSign, hUnlikeSign_sameside, MassRangePsi2S[0], MassRangePsi2S[1], BinWidth, Ptbins[j], Ptbins[j+1], "Psi2S", "withbkg", Form("output/%s/SigExtraction", AnalysisName.c_str()));
        }
    
    }


}