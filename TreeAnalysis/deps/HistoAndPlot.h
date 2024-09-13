#include "QuadVarManager.h"
#include "Mtool.h"
#include <TObjArray.h>

enum histoname{
    hQ,
    hDeltaPhi_Pt,
    hDeltaEta_Pt,
    hDeltaR_Pt,
    hDipionsMass_Pt,
    hQ_Pt,
    hDipionsMass_DeltaR,
    hPt,
    hMassPsi2S_Pt,
    hMass_Pt,
};

namespace QuadHistos{
    void DefineHistograms(TObjArray* array){
        // define histograms
        TH1F* h_Q = new TH1F("h_Q", "Q", 100, 0, 1);
        array->AddAt(h_Q, hQ);
        TH2F* h_DeltaPhi_Pt = new TH2F("h_DeltaPhi_Pt", "DeltaPhi_Pt", 40, 0, 20, 100, 0, TMath::Pi());
        array->AddAt(h_DeltaPhi_Pt, hDeltaPhi_Pt);
        TH2F* h_DeltaEta_Pt = new TH2F("h_DeltaEta_Pt", "DeltaEta_Pt", 40, 0, 20, 100, 0, 2);
        array->AddAt(h_DeltaEta_Pt, hDeltaEta_Pt);
        TH2F* h_DeltaR_Pt = new TH2F("h_DeltaR_Pt", "DeltaR_Pt", 80, 0, 40, 600, 0, sqrt(4 + pow(TMath::Pi(), 2)));
        array->AddAt(h_DeltaR_Pt, hDeltaR_Pt);
        TH2F* h_DipionsMass_Pt = new TH2F("h_DipionsMass_Pt", "DipionsMass_Pt", 200, 0, 20, 100, 0, 1.0);
        array->AddAt(h_DipionsMass_Pt, hDipionsMass_Pt);
        TH2F* h_Q_Pt = new TH2F("h_Q_Pt", "Q_Pt", 200, 0, 20, 100, 0, 1);
        array->AddAt(h_Q_Pt, hQ_Pt);
        TH2F* h_DipionsMass_DeltaR = new TH2F("h_DipionsMass_DeltaR", "DipionsMass_DeltaR", 600, 0, sqrt(4 + pow(TMath::Pi(), 2)), 100, 0, 1.0);
        array->AddAt(h_DipionsMass_DeltaR, hDipionsMass_DeltaR);
        TH1F* hPt = new TH1F("hPt", "Pt", 20, 0, 20);
        hPt->Sumw2();
        array->AddAt(hPt, histoname::hPt);
        TH2F* hMassPsi2S_Pt = new TH2F("hMassPsi2S_Pt", "MassPsi2S_Pt", 40, 0, 40, 50, 3.6, 3.8);
        hMassPsi2S_Pt->Sumw2();
        array->AddAt(hMassPsi2S_Pt, histoname::hMassPsi2S_Pt);
        TH2F* hMass_Pt = new TH2F("hMass_Pt", "Mass_Pt", 100, 0, 100, 100, 3.6, 4.0);
        hMass_Pt->Sumw2();
        array->AddAt(hMass_Pt, histoname::hMass_Pt);
    }

    //---------------------------------------------------------------------------------------------------
    void FillHistograms(TObjArray* array, double* values){
        ((TH1F*)array->At(hQ))->Fill(values[QuadVarManager::Variables::kQ]);
        ((TH2F*)array->At(hDeltaPhi_Pt))->Fill(values[QuadVarManager::Variables::kPt], values[QuadVarManager::Variables::kDeltaPhi]);
        ((TH2F*)array->At(hDeltaEta_Pt))->Fill(values[QuadVarManager::Variables::kPt], values[QuadVarManager::Variables::kDeltaEta]);
        ((TH2F*)array->At(hDeltaR_Pt))->Fill(values[QuadVarManager::Variables::kPt], values[QuadVarManager::Variables::kDeltaR]);
        ((TH2F*)array->At(hDipionsMass_Pt))->Fill(values[QuadVarManager::Variables::kPt], values[QuadVarManager::Variables::kDihardonMass]);
        ((TH2F*)array->At(hQ_Pt))->Fill(values[QuadVarManager::Variables::kPt], values[QuadVarManager::Variables::kQ]);
        ((TH2F*)array->At(hDipionsMass_DeltaR))->Fill(values[QuadVarManager::Variables::kDeltaR], values[QuadVarManager::Variables::kDihardonMass]);
        ((TH1F*)array->At(hPt))->Fill(values[QuadVarManager::Variables::kPt]);
        ((TH2F*)array->At(hMassPsi2S_Pt))->Fill(values[QuadVarManager::Variables::kPt], values[QuadVarManager::Variables::kMass]);
        ((TH2F*)array->At(hMass_Pt))->Fill(values[QuadVarManager::Variables::kPt], values[QuadVarManager::Variables::kMass]);
    }

    //---------------------------------------------------------------------------------------------------
    void WriteHistograms(TObjArray* array, TFile* file, char* ListName){
        file->cd();
        MTool::SaveHistogramInTList<TH1F>(file, ListName, (TH1F*)array->At(hQ));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(hDeltaPhi_Pt));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(hDeltaEta_Pt));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(hDeltaR_Pt));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(hDipionsMass_Pt));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(hQ_Pt));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(hDipionsMass_DeltaR));
        MTool::SaveHistogramInTList<TH1F>(file, ListName, (TH1F*)array->At(hPt));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(hMassPsi2S_Pt));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(hMass_Pt));
    }

    void DrawEfficiency(std::vector<TList*> list, std::vector<TList*> listbf, std::vector<string> ListName)
    {
        gStyle->SetOptStat(0);
        std::vector<TH1F*> hEff_Pt;
        for (int i = 0; i < list.size(); i++){
            TH1F* hEff = (TH1F*)list[i]->At(histoname::hPt)->Clone();
            hEff->Sumw2();
            hEff->SetName(Form("hEff_%s", ListName[i].c_str()));
            TH1F* hAll = (TH1F*)listbf[i]->At(histoname::hPt)->Clone();
            hAll->Sumw2();
            hAll->SetName(Form("hAll_%s", ListName[i].c_str()));
            hEff->Divide(hAll);
            hEff_Pt.push_back(hEff);
        }
        TCanvas* c = new TCanvas("c", "c", 800, 600);
        gPad->SetGrid();
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.15);
        for (int i = 0; i < hEff_Pt.size(); i++){
            MTool::StandardSet(hEff_Pt[i], "p_{T} [GeV]", "Acceptance");
            MTool::SetMarkerLine(hEff_Pt[i], i+1, 20, 1, 1, 1);
            if (i == 0){
                hEff_Pt[i]->Draw("E");
                hEff_Pt[i]->GetYaxis()->SetRangeUser(0, 1);
            }else{
                hEff_Pt[i]->Draw("Esame");
            }
        }
        TLegend* leg = new TLegend(0.2, 0.8-hEff_Pt.size()*0.06, 0.45, 0.8);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);
        for (int i = 0; i < ListName.size(); i++){
            leg->AddEntry(hEff_Pt[i], ListName[i].c_str(), "lep");
        }
        leg->Draw();
        c->SaveAs("output/Efficiency.pdf");

        // estimate signal distribution
        TF1* f_pTspectrum = new TF1("f_pTspectrum","[0]*x/TMath::Power((1+(x/[1])*(x/[1])),[2])",0.,30.);
        f_pTspectrum->SetParameters(7.64519*1.44, 5.30628, 3.30887);
        double Lumi = 20.0; // nb^-1
        double average_eff = 0.8*0.8*0.85*0.8*0.8*0.6;
        double BR_Jpsiee = 5.96e-2;
        // std::vector<double> pTbin = {0, 1, 2, 3, 4, 6, 8, 10, 15, 20};
        TH1F* h_pTspectrum = new TH1F("h_pTspectrum", "p_{T} Spectrum", 20, 0, 20);
        // TH1F* h_pTspectrum = new TH1F("h_pTspectrum", "p_{T} Spectrum", pTbin.size()-1, pTbin.data());
        h_pTspectrum->Sumw2();
        for (int ibinx = 1; ibinx <= h_pTspectrum->GetNbinsX(); ibinx++){
            double pT_low = h_pTspectrum->GetBinLowEdge(ibinx);
            double pT_high = h_pTspectrum->GetBinLowEdge(ibinx+1);
            double Nsignal = f_pTspectrum->Integral(pT_low, pT_high) * Lumi * average_eff * BR_Jpsiee * 1000;
            h_pTspectrum->SetBinContent(ibinx, Nsignal);
        }
        TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
        gPad->SetGrid();
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.15);
        // h_pTspectrum times Acceptance
        for (int i = 0; i < hEff_Pt.size(); i++){
            TH1F* hSignal = (TH1F*)h_pTspectrum->Clone();
            hSignal->SetName(Form("hSignal_%s", ListName[i].c_str()));
            hSignal->Multiply(hEff_Pt[i]);
            MTool::StandardSet(hSignal, "p_{T} [GeV]", "Counts");
            MTool::SetMarkerLine(hSignal, i+1, 20, 1, 1, 1);
            if (i == 0){
                hSignal->Draw("E");
                hSignal->GetYaxis()->SetRangeUser(hSignal->GetMinimum()*0.8, hSignal->GetMaximum()*1.3);
            }else{
                hSignal->Draw("Esame");
            }
            double N_10_20 = hSignal->Integral(hSignal->FindBin(10+1e-6), hSignal->FindBin(20-1e-6));
            cout<<ListName[i]<<": "<<N_10_20<<endl;
        }
        TLegend* leg2 = new TLegend(0.2, 0.8-hEff_Pt.size()*0.06, 0.45, 0.8);
        leg2->SetBorderSize(0);
        leg2->SetFillStyle(0);
        for (int i = 0; i < ListName.size(); i++){
            leg2->AddEntry(hEff_Pt[i], ListName[i].c_str(), "lep");
        }
        leg2->Draw();
        MTool::SetLatex(0.56, 0.8-hEff_Pt.size()*0.06, Form("Luminosity = %.1f pb^{-1}", Lumi), 42, 0.04, 1);
        MTool::SetLatex(0.56, 0.72-hEff_Pt.size()*0.06, Form("Average Efficiency = %.2f", average_eff), 42, 0.04, 1);
        c2->SaveAs("output/SignalDistribution.pdf");
    }

    void DrawMultHisto1d(std::vector<TList*> list, std::vector<string> ListName, int histoname, string xtitle, string ytitle, string outputname, double xmin = -999, double xmax = 999){
        gStyle->SetOptStat(0);
        std::vector<TH1F*> h_temp;
        for (int i = 0; i < list.size(); i++){
            TH1F* h = (TH1F*)list[i]->At(histoname)->Clone();
            h->SetName(Form("temp_%s", ListName[i].c_str()));
            h_temp.push_back(h);
        }
        TCanvas* c = new TCanvas("c", "c", 800, 600);
        gPad->SetGrid();
        gPad->SetLeftMargin(0.18);
        gPad->SetBottomMargin(0.15);
        double ymax = 0;
        for (int i = 0; i < h_temp.size(); i++){
            MTool::StandardSet(h_temp[i], xtitle.c_str(), ytitle.c_str());
            MTool::SetMarkerLine(h_temp[i], i+1, 20, 1, 1, 1);
            h_temp[i]->Scale(1/h_temp[i]->Integral());
            if (h_temp[i]->GetMaximum() > ymax) ymax = h_temp[i]->GetMaximum();
            if (i == 0){
                h_temp[i]->GetXaxis()->SetRangeUser(xmin, xmax);
                h_temp[i]->Draw("LEP");
            }else{
                h_temp[i]->Draw("LEPsame");
            }
        }
        h_temp[0]->GetYaxis()->SetRangeUser(0, ymax*1.2);
        c->Update();
        TLegend* leg = new TLegend(0.23, 0.8-h_temp.size()*0.055, 0.45, 0.8);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);
        for (int i = 0; i < ListName.size(); i++){
            leg->AddEntry(h_temp[i], ListName[i].c_str(), "lep");
        }
        leg->Draw();
        c->SaveAs(outputname.c_str());

    }


    void DrawMultHisto2d(std::vector<TList*> list, std::vector<string> ListName, int histoname, string xtitle, string ytitle, string outputname){
        gStyle->SetOptStat(0);
        std::vector<TH2F*> h_temp;
        for (int i = 0; i < list.size(); i++){
            TH2F* h = (TH2F*)list[i]->At(histoname)->Clone();
            h->SetName(Form("temp_%s", ListName[i].c_str()));
            h_temp.push_back(h);
        }
        TCanvas* c = new TCanvas("c", "c", 800, 600);
        gPad->SetLeftMargin(0.18);
        gPad->SetBottomMargin(0.15);
        gPad->SetLogz();
        for (int i = 0; i < h_temp.size(); i++){
            MTool::StandardSet(h_temp[i], xtitle.c_str(), ytitle.c_str());
            h_temp[i]->Draw("colz");
            c->SaveAs(Form("%s_%s.pdf", outputname.c_str(), ListName[i].c_str()));
            c->Clear();
        }
    }

};