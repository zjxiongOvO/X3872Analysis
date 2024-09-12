#include "../deps/HistoAndPlot.h"
#include "../deps/SignalExtraction_X3872.cxx"
#include "../deps/SignalExtraction_X3872_only.cxx"
#include "../deps/SignalExtraction_Psi2S_only.cxx"
#include "../deps/SignalExtraction_Psi2S_only_withBkg.cxx"
#include "../deps/SignalExtraction_X3872_only_withBkg.cxx"
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
    bool NeedSameSide = false;

    try {
        json config = readConfig(configpath);
        dataNameList = config["DataName"].get<std::vector<std::string>>();
        simNameList = config["SimName"].get<std::vector<std::string>>();
        CutNameList = config["CutName"].get<std::vector<std::string>>();
        NeedSameSide = config["NeedSameSide"].get<bool>();
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
    // QuadHistos::DrawMultHisto1d(list_QA, QAname, histoname::hPt, "p_{T} [GeV]", "Entries", "output/pT.pdf");
    // QuadHistos::DrawMultHisto1d(list_QA, QAname, histoname::hDeltaR, "#Delta R", "Entries", "output/DeltaR.pdf", 0, 1.5);
    // QuadHistos::DrawMultHisto1d(list_QA, QAname, histoname::hQ, "Q", "Entries", "output/Q.pdf", 0, 1.5);
    // QuadHistos::DrawMultHisto1d(list_QA, QAname, histoname::hDipionsMass, "Dipions Mass [GeV]", "Entries", "output/DipionsMass.pdf", 0, 1.5);
    // QuadHistos::DrawMultHisto1d(list_BeforeSelection, QAname, histoname::hDipionsMass, "#it{m}_{#pi#pi} [GeV/c^{2}]", "", "output/DipionsMass_bf.pdf", 0.3, 0.8);
    // 2D
    // QuadHistos::DrawMultHisto2d(list_QA, QAname, histoname::hDeltaR_Pt, "p_{T} [GeV]", "#Delta R", "output/DeltaR_Pt.pdf");
    // QuadHistos::DrawMultHisto2d(list_QA, QAname, histoname::hDeltaPhi_Pt, "p_{T} [GeV]", "#Delta #phi", "output/DeltaPhi_Pt.pdf");
    // QuadHistos::DrawMultHisto2d(list_QA, QAname, histoname::hDeltaEta_Pt, "p_{T} [GeV]", "#Delta #eta", "output/DeltaEta_Pt.pdf");


    // Signal Extraction for Data
    system("mkdir -p output/SigExtraction");
    // for (int i = 0; i < dataNameList.size(); i++){
    //     for (int j = 0; j < CutNameList.size(); j++){
    //         TH1F* hUnlikeSign = (TH1F*)list_QA[i*CutNameList.size() + j]->At(histoname::hMass)->Clone();
    //         // SignalExtraction_X3872(hUnlikeSign, 3.6, 4.0, Form("output/signalext_%s_%s.pdf", dataNameList[i].c_str(), CutNameList[j].c_str()));
    //         SignalExtraction_X3872_only(hUnlikeSign, 3.8, 4.0, Form("output/SigExtraction/signalextX3872_%s_%s.pdf", dataNameList[i].c_str(), CutNameList[j].c_str()));
    //         SignalExtraction_Psi2S_only(hUnlikeSign, 3.6, 3.8, Form("output/SigExtraction/signalextPsi2S_%s_%s.pdf", dataNameList[i].c_str(), CutNameList[j].c_str()));
    //     }
    // }

    // Signal Extraction in different pT bins
    std::vector<double> pTbin = {4, 35};
    for (int i = 0; i < dataNameList.size(); i++){
        for (int j = 0; j < CutNameList.size(); j++){
            TH2F* hMass_Pt = (TH2F*)list_QA[i*CutNameList.size() + j]->At(histoname::hMass_Pt)->Clone();
            TH2F* hMassPsi2S_Pt = (TH2F*)list_QA[i*CutNameList.size() + j]->At(histoname::hMassPsi2S_Pt)->Clone();
            for (int k = 0; k < pTbin.size()-1; k++){
                TH1F* hUnlikeSign = (TH1F*)hMass_Pt->ProjectionY(Form("hUnlikeSign_%s_%s_%d_%d", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]), hMass_Pt->GetXaxis()->FindBin(pTbin[k]), hMass_Pt->GetXaxis()->FindBin(pTbin[k+1]));
                hUnlikeSign->SetName(Form("hUnlikeSign_%s_%s_%d_%d", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]));
                // SignalExtraction_X3872(hUnlikeSign, 3.6, 4.0, Form("output/SigExtraction/signalextX3872_%s_%s_%d_%d_full.pdf", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]));
                // SignalExtraction_X3872_only(hUnlikeSign, 3.80, 3.96, Form("output/SigExtraction/signalextX3872_%s_%s_%d_%d.pdf", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]));
                
                TH1F* hUnlikeSignPsi2S = (TH1F*)hMassPsi2S_Pt->ProjectionY(Form("hUnlikeSignPsi2S_%s_%s_%d_%d", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]), hMassPsi2S_Pt->GetXaxis()->FindBin(pTbin[k]), hMassPsi2S_Pt->GetXaxis()->FindBin(pTbin[k+1]));
                hUnlikeSignPsi2S->SetName(Form("hUnlikeSignPsi2S_%s_%s_%d_%d", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]));
                SignalExtraction_Psi2S_only(hUnlikeSignPsi2S, 3.6, 3.76, Form("output/SigExtraction/signalextPsi2S_%s_%s_%d_%d.pdf", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]));
                // hUnlikeSign->Rebin(5);
                SignalExtraction_X3872_only(hUnlikeSign, 3.80, 3.92, Form("output/SigExtraction/signalextX3872_%s_%s_%d_%d.pdf", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]));
            }
            
        }
    }

    if (NeedSameSide == true){
        for (int i = 0; i < dataNameList.size(); i++){
            for (int j = 0; j < CutNameList.size(); j++){
                TList* listbkg = (TList*)myfile->Get(Form("%s_%s_bkg", dataNameList[i].c_str(), CutNameList[j].c_str()));
                TH2F* hMassPsi2S_Pt_bkg = (TH2F*)listbkg->At(histoname::hMassPsi2S_Pt)->Clone();
                hMassPsi2S_Pt_bkg->SetName(Form("hMassPsi2S_Pt_bkg_%s_%s", dataNameList[i].c_str(), CutNameList[j].c_str()));
                TH2F* hMassPsi2S_Pt = (TH2F*)list_QA[i*CutNameList.size() + j]->At(histoname::hMassPsi2S_Pt)->Clone();
                hMassPsi2S_Pt->SetName(Form("hMassPsi2S_Pt_%s_%s", dataNameList[i].c_str(), CutNameList[j].c_str()));
                TH2F* hMass_Pt_bkg = (TH2F*)listbkg->At(histoname::hMass_Pt)->Clone();
                hMass_Pt_bkg->SetName(Form("hMass_Pt_bkg_%s_%s", dataNameList[i].c_str(), CutNameList[j].c_str()));
                TH2F* hMass_Pt = (TH2F*)list_QA[i*CutNameList.size() + j]->At(histoname::hMass_Pt)->Clone();
                hMass_Pt->SetName(Form("hMass_Pt_%s_%s", dataNameList[i].c_str(), CutNameList[j].c_str()));

                for (int k = 0; k < pTbin.size()-1; k++){
                    // TH1F* hUnlikeSign_bkg = (TH1F*)hMass_Pt_bkg->ProjectionY(Form("hUnlikeSign_bkg_%s_%s_%d_%d", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]), hMass_Pt_bkg->GetXaxis()->FindBin(pTbin[k]), hMass_Pt_bkg->GetXaxis()->FindBin(pTbin[k+1]));
                    TH1F* hUnlikeSignPsi2S_bkg = (TH1F*)hMassPsi2S_Pt_bkg->ProjectionY(Form("hUnlikeSignPsi2S_bkg_%s_%s_%d_%d", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]), hMassPsi2S_Pt_bkg->GetXaxis()->FindBin(pTbin[k]), hMassPsi2S_Pt_bkg->GetXaxis()->FindBin(pTbin[k+1]));
                    TH1F* hUnlikeSignPsi2S = (TH1F*)hMassPsi2S_Pt->ProjectionY(Form("hUnlikeSignPsi2S_%s_%s_%d_%d", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]), hMassPsi2S_Pt->GetXaxis()->FindBin(pTbin[k]), hMassPsi2S_Pt->GetXaxis()->FindBin(pTbin[k+1]));
                    SignalExtraction_Psi2S_only_withBkg(hUnlikeSignPsi2S, hUnlikeSignPsi2S_bkg, 3.6, 3.76, Form("output/SigExtraction/signalextPsi2S_%s_%s_%d_%d_withBkg.pdf", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]));

                    TH1F* hUnlikeSign_bkg = (TH1F*)hMass_Pt_bkg->ProjectionY(Form("hUnlikeSign_bkg_%s_%s_%d_%d", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]), hMass_Pt_bkg->GetXaxis()->FindBin(pTbin[k]), hMass_Pt_bkg->GetXaxis()->FindBin(pTbin[k+1]));
                    TH1F* hUnlikeSign = (TH1F*)hMass_Pt->ProjectionY(Form("hUnlikeSign_%s_%s_%d_%d", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]), hMass_Pt->GetXaxis()->FindBin(pTbin[k]), hMass_Pt->GetXaxis()->FindBin(pTbin[k+1]));
                    SignalExtraction_X3872_only_withBkg(hUnlikeSign, hUnlikeSign_bkg, 3.80, 3.92, Form("output/SigExtraction/signalextX3872_%s_%s_%d_%d_withBkg.pdf", dataNameList[i].c_str(), CutNameList[j].c_str(), (int)pTbin[k], (int)pTbin[k+1]));
                }
            }
        }
    }
}