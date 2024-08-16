#include "../deps/HistoAndPlot.h"
// #include "/data3/xzj/Hep4USTC/Public/src/HistogramProjectionFitter.cxx"

void DrawAnalysisResults(){
    TFile* myfile = new TFile("output/Analysis.root", "read");

    std::vector<string> dataNameList = {"2022", "2023", "2024"};
    std::vector<string> simNameList = {"X(3872)", "#Psi(2S)"};
    std::vector<string> CutNameList = {"debug1", "debug2"};

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
    DrawEfficiency(list_AfterSelection, list_BeforeSelection, effname);

    // QA

    // Signal Extraction for Data
}