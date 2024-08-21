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

void DrawAnalysisResults(string configpath){
    TFile* myfile = new TFile("output/Analysis.root", "read");

    std::vector<string> dataNameList;
    std::vector<string> simNameList;
    std::vector<string> CutNameList;

    try {
        json config = readConfig(configpath);
        dataNameList = config["DataName"].get<std::vector<std::string>>();
        simNameList = config["SimName"].get<std::vector<std::string>>();
        CutNameList = config["CutName"].get<std::vector<std::string>>();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    // calculate efficiency
    std::vector<TList*> list_AfterSelection;
    std::vector<TList*> list_BeforeSelection;
    std::vector<string> effname;
    for (int i = 0; i < CutNameList.size(); i++){
        for (int j = 0; j < simNameList.size(); j++){
            list_AfterSelection.push_back((TList*)myfile->Get(Form("%s_%s", simNameList[j].c_str(), CutNameList[i].c_str())));
            list_BeforeSelection.push_back((TList*)myfile->Get(Form("%s_before", simNameList[j].c_str())));
            effname.push_back(Form("%s_%s", simNameList[j].c_str(), CutNameList[i].c_str()));
        }
    }
    QuadHistos::DrawEfficiency(list_AfterSelection, list_BeforeSelection, effname);

    // QA
    std::vector<TList*> list_QA;
    std::vector<string> QAname;
    for (int i = 0; i < CutNameList.size(); i++){
        for (int j = 0; j < dataNameList.size(); j++){
            list_QA.push_back((TList*)myfile->Get(Form("%s_%s", dataNameList[j].c_str(), CutNameList[i].c_str())));
            QAname.push_back(Form("%s_%s", dataNameList[j].c_str(), CutNameList[i].c_str()));
        }
        for (int j = 0; j < simNameList.size(); j++){
            list_QA.push_back((TList*)myfile->Get(Form("%s_%s", simNameList[j].c_str(), CutNameList[i].c_str())));
            QAname.push_back(Form("%s_%s", simNameList[j].c_str(), CutNameList[i].c_str()));
        }
    }

    // 1D
    QuadHistos::DrawMultHisto1d(list_QA, QAname, histoname::hPt, "p_{T} [GeV]", "Entries", "output/pT.pdf");
    QuadHistos::DrawMultHisto1d(list_QA, QAname, histoname::hDeltaR, "#Delta R", "Entries", "output/DeltaR.pdf", 0, 1.5);
    QuadHistos::DrawMultHisto1d(list_QA, QAname, histoname::hQ, "Q", "Entries", "output/Q.pdf", 0, 1.5);
    QuadHistos::DrawMultHisto1d(list_QA, QAname, histoname::hDipionsMass, "Dipions Mass [GeV]", "Entries", "output/DipionsMass.pdf", 0, 1.5);
    // 2D
    QuadHistos::DrawMultHisto2d(list_QA, QAname, histoname::hDeltaR_Pt, "p_{T} [GeV]", "#Delta R", "output/DeltaR_Pt.pdf");

    // Signal Extraction for Data
    for (int i = 0; i < dataNameList.size(); i++){
        for (int j = 0; j < CutNameList.size(); j++){
            TH1F* hUnlikeSign = (TH1F*)list_QA[i*CutNameList.size() + j]->At(histoname::hMass)->Clone();
            SignalExtraction_X3872(hUnlikeSign, 3.6, 4.0, Form("output/signalext_%s_%s.pdf", dataNameList[i].c_str(), CutNameList[j].c_str()));
        }
    }
}