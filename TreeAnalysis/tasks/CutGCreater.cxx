// fit 2 function for 2-d bond cut form a TH2F
#include "../deps/Mtool.h"
#include <TCanvas.h>
#include <TCutG.h>
#include <TSpline.h>

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
    TCanvas* c = new TCanvas("c", "c", 800, 600);
    gPad->SetGrid();
    gPad->SetLogz();
    h2D->SetTitle("");
    h2D->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    h2D->GetYaxis()->SetTitle("#DeltaR");
    h2D->Draw("colz");
    spline_low->Draw("same");
    spline_high->Draw("same");
    cutg->Draw("same");
    cutg->SetName(h2D->GetName());
    // c->SaveAs(Form("output/CutG_%s.pdf", h2D->GetName()));
    c->SaveAs(Form("output/CutG/CutG_%s_%.2f.pdf", h2D->GetName(), percent));
    return cutg;
}

void CutGCreater(){
    TFile* file = new TFile("output/Analysis_bk.root");
    gStyle->SetOptStat(0);
    system("mkdir -p output/CutG");
    // TList* list = (TList*)file->Get("X3872_BasicAcc");
    // TH2F* h2_DeltaR_Pt_X3872 = (TH2F*)list->FindObject("h_DeltaR_Pt");
    // TH2F* h2_DeltaEta_Pt_X3872 = (TH2F*)list->FindObject("h_DeltaEta_Pt");
    // TH2F* h2_DeltaPhi_Pt_X3872 = (TH2F*)list->FindObject("h_DeltaPhi_Pt");
    // h2_DeltaR_Pt_X3872->SetName("h2_DeltaR_Pt_X3872");
    // h2_DeltaEta_Pt_X3872->SetName("h2_DeltaEta_Pt_X3872");
    // h2_DeltaPhi_Pt_X3872->SetName("h2_DeltaPhi_Pt_X3872");

 
    // TList* list = (TList*)file->Get("Psi2S_BasicAcc");
    // TH2F* h2_DeltaR_Pt_Psi2S = (TH2F*)list->FindObject("h_DeltaR_Pt");
    // TH2F* h2_DeltaEta_Pt_Psi2S = (TH2F*)list->FindObject("h_DeltaEta_Pt");
    // TH2F* h2_DeltaPhi_Pt_Psi2S = (TH2F*)list->FindObject("h_DeltaPhi_Pt");
    // h2_DeltaR_Pt_Psi2S->SetName("h2_DeltaR_Pt_Psi2S");
    // h2_DeltaEta_Pt_Psi2S->SetName("h2_DeltaEta_Pt_Psi2S");
    // h2_DeltaPhi_Pt_Psi2S->SetName("h2_DeltaPhi_Pt_Psi2S");

    TList* list = (TList*)file->Get("X3872_TunedAcc");
    TH2F* h2_DeltaR_Pt_X3872 = (TH2F*)list->FindObject("h_DeltaR_Pt");
    h2_DeltaR_Pt_X3872->SetName("h2_DeltaR_Pt_X3872");

    TFile* ouput = new TFile("output/CutG.root", "update");
    ouput->cd();
    std::vector<double> SavedRatios = {0.98, 0.96, 0.94, 0.92, 0.90, 0.88, 0.86, 0.84, 0.82, 0.80, 0.70, 0.60, 0.50, 0.40, 0.30, 0.20, 0.10};
    // std::vector<double> SavedRatios = {0.90, 0.80, 0.70, 0.60, 0.50, 0.40, 0.30, 0.20, 0.10};
    for (int i = 0; i < SavedRatios.size(); i++){
        TCutG* cutg_DeltaR_Pt_X3872 = CreateCutG(h2_DeltaR_Pt_X3872, SavedRatios[i]);
        // TCutG* cutg_DeltaEta_Pt_X3872 = CreateCutG(h2_DeltaEta_Pt_X3872, SavedRatios[i]);
        // TCutG* cutg_DeltaPhi_Pt_X3872 = CreateCutG(h2_DeltaPhi_Pt_X3872, SavedRatios[i]);
        cutg_DeltaR_Pt_X3872->Write(Form("cutg_Tuned_DeltaR_Pt_X3872_%.2f", SavedRatios[i]));
        // cutg_DeltaEta_Pt_X3872->Write(Form("cutg_DeltaEta_Pt_X3872_%.2f", SavedRatios[i]));
        // cutg_DeltaPhi_Pt_X3872->Write(Form("cutg_DeltaPhi_Pt_X3872_%.2f", SavedRatios[i]));

        // TCutG* cutg_DeltaR_Pt_Psi2S = CreateCutG(h2_DeltaR_Pt_Psi2S, SavedRatios[i]);
        // TCutG* cutg_DeltaEta_Pt_Psi2S = CreateCutG(h2_DeltaEta_Pt_Psi2S, SavedRatios[i]);
        // TCutG* cutg_DeltaPhi_Pt_Psi2S = CreateCutG(h2_DeltaPhi_Pt_Psi2S, SavedRatios[i]);
        // cutg_DeltaR_Pt_Psi2S->Write(Form("cutg_DeltaR_Pt_Psi2S_%.2f", SavedRatios[i]));
        // cutg_DeltaEta_Pt_Psi2S->Write(Form("cutg_DeltaEta_Pt_Psi2S_%.2f", SavedRatios[i]));
        // cutg_DeltaPhi_Pt_Psi2S->Write(Form("cutg_DeltaPhi_Pt_Psi2S_%.2f", SavedRatios[i]));
    }
    ouput->Close();
}