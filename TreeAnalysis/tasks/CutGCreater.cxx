// this code is for creating the 2-D cut from a 2-D histogram

#include "../deps/Mtool.h"
#include <TCanvas.h>
#include <TCutG.h>
#include <TSpline.h>
#include <TStyle.h>
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

TCutG* CreateCutG(TH2F* h2D, double percent=0.9){
    if (percent > 1 || percent < 0){
        cout << "Error: percent should be in [0, 1]" << endl;
        return nullptr;
    }
    int nBinsX = h2D->GetNbinsX();
    double xValues[nBinsX];
    double yValues_low[nBinsX];
    double yValues_high[nBinsX];
    double side = (1-percent)/2;
    for (int i = 1; i <= nBinsX; i++){
        xValues[i-1] = h2D->GetXaxis()->GetBinCenter(i);
        TH1F* h1D = (TH1F*)h2D->ProjectionY(Form("h1D_%d", i), i, i);
        double integral = h1D->Integral();
        double side_integral = integral*side;
        double nCounts = 0;
        double nCountsbf = 0;
        for (int j = 1; j <= h1D->GetNbinsX(); j++){
            nCountsbf = nCounts;
            nCounts += h1D->GetBinContent(j);
            if (nCountsbf < side_integral && nCounts >= side_integral){
                yValues_low[i-1] = h1D->GetBinCenter(j);
            }
            if (nCountsbf < integral-side_integral && nCounts >= integral-side_integral){
                yValues_high[i-1] = h1D->GetBinCenter(j);
            }
        }
    }
    TSpline3* spline_low = new TSpline3("spline", xValues, yValues_low, nBinsX);
    TSpline3* spline_high = new TSpline3("spline", xValues, yValues_high, nBinsX);
    spline_low->SetLineColor(kRed);
    spline_high->SetLineColor(kRed);
    TCutG* cutg = new TCutG("cutg", nBinsX*2);
    for (int i = 0; i < nBinsX; i++){
        cutg->SetPoint(i, xValues[i], yValues_low[i]);
        cutg->SetPoint(nBinsX*2-1-i, xValues[i], yValues_high[i]);
    }
    cutg->SetName(Form("cutg_%s_%.0f", h2D->GetName(), percent*100));
    return cutg;
}

void CutGCreater(string configFilePath){
    system("mkdir -p CutLibrary");
    // init config
    string AnalysisName;
    string CutGFilePath;
    string CutBasedOn;
    std::vector <double> CutGPercents;

    // read config
    try {
        json config = readConfig(configFilePath);
        AnalysisName = config["GlobalSetting"]["AnalysisName"];
        CutGFilePath = config["CutGCreater"]["CutGFilePath"];
        CutBasedOn = config["CutGCreater"]["CutBasedOn"];
        CutGPercents = config["CutGCreater"]["CutGPercents"].get<std::vector<double>>();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    // read file
    TFile* file = new TFile(Form("output/%s/Analysis.root", AnalysisName.c_str()), "read");
    gStyle->SetOptStat(0);

    // get histograms
    // X3872
    TList* list_X3872 = (TList*)file->Get(Form("X3872_%s", CutBasedOn.c_str()));
    TH2F* h2_DeltaR_Pt_X3872 = (TH2F*)list_X3872->FindObject("h_DeltaR_Pt");
    TH2F* h2_DeltaEta_Pt_X3872 = (TH2F*)list_X3872->FindObject("h_DeltaEta_Pt");
    TH2F* h2_DeltaPhi_Pt_X3872 = (TH2F*)list_X3872->FindObject("h_DeltaPhi_Pt");
    h2_DeltaEta_Pt_X3872->SetName("h_DeltaEta_Pt_X3872");
    h2_DeltaPhi_Pt_X3872->SetName("h_DeltaPhi_Pt_X3872");
    h2_DeltaR_Pt_X3872->SetName("h_DeltaR_Pt_X3872");

    // Psi2S
    TList* list_Psi2S = (TList*)file->Get(Form("Psi2S_%s", CutBasedOn.c_str()));
    TH2F* h2_DeltaR_Pt_Psi2S = (TH2F*)list_Psi2S->FindObject("h_DeltaR_Pt");
    TH2F* h2_DeltaEta_Pt_Psi2S = (TH2F*)list_Psi2S->FindObject("h_DeltaEta_Pt");
    TH2F* h2_DeltaPhi_Pt_Psi2S = (TH2F*)list_Psi2S->FindObject("h_DeltaPhi_Pt");
    h2_DeltaEta_Pt_Psi2S->SetName("h_DeltaEta_Pt_Psi2S");
    h2_DeltaPhi_Pt_Psi2S->SetName("h_DeltaPhi_Pt_Psi2S");
    h2_DeltaR_Pt_Psi2S->SetName("h_DeltaR_Pt_Psi2S");

    // create cutg
    TFile* output = new TFile(Form("CutLibrary/%s_CutG.root", AnalysisName.c_str()), "recreate");
    output->cd();
    for (int i = 0; i < CutGPercents.size(); i++){
        TCutG* cutg_DeltaR_Pt_X3872 = CreateCutG(h2_DeltaR_Pt_X3872, CutGPercents[i]);
        cutg_DeltaR_Pt_X3872->Write(Form("cutg_%s_DeltaR_Pt_X3872_%.0f", CutBasedOn.c_str(), CutGPercents[i]*100));
        TCutG* cutg_DeltaEta_Pt_X3872 = CreateCutG(h2_DeltaEta_Pt_X3872, CutGPercents[i]);
        cutg_DeltaEta_Pt_X3872->Write(Form("cutg_%s_DeltaEta_Pt_X3872_%.0f", CutBasedOn.c_str(), CutGPercents[i]*100));
        TCutG* cutg_DeltaPhi_Pt_X3872 = CreateCutG(h2_DeltaPhi_Pt_X3872, CutGPercents[i]);
        cutg_DeltaPhi_Pt_X3872->Write(Form("cutg_%s_DeltaPhi_Pt_X3872_%.0f", CutBasedOn.c_str(), CutGPercents[i]*100));

        TCutG* cutg_DeltaR_Pt_Psi2S = CreateCutG(h2_DeltaR_Pt_Psi2S, CutGPercents[i]);
        cutg_DeltaR_Pt_Psi2S->Write(Form("cutg_%s_DeltaR_Pt_Psi2S_%.0f", CutBasedOn.c_str(), CutGPercents[i]*100));
        TCutG* cutg_DeltaEta_Pt_Psi2S = CreateCutG(h2_DeltaEta_Pt_Psi2S, CutGPercents[i]);
        cutg_DeltaEta_Pt_Psi2S->Write(Form("cutg_%s_DeltaEta_Pt_Psi2S_%.0f", CutBasedOn.c_str(), CutGPercents[i]*100));
        TCutG* cutg_DeltaPhi_Pt_Psi2S = CreateCutG(h2_DeltaPhi_Pt_Psi2S, CutGPercents[i]);
        cutg_DeltaPhi_Pt_Psi2S->Write(Form("cutg_%s_DeltaPhi_Pt_Psi2S_%.0f", CutBasedOn.c_str(), CutGPercents[i]*100));
    }

    output->Close();
    file->Close();

    cout << "CutG created successfully!" << endl;
}