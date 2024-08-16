#include "QuadVarManager.h"
#include "Mtool.h"
#include <TObjArray.h>

enum histoname{
    hQ,
    hDeltaR1,
    hDeltaR2,
    hDeltaR,
    hCosMotherAndDaughter_CMS,
    hDeltaPhi_Pt,
    hDeltaEta_Pt,
    hDeltaR_Pt,
    hDipionsMass,
    hPt,
    hMass,
    hMassX3872,
    HMass_Pt,
};

namespace QuadHistos{
    void DefineHistograms(TObjArray* array){
        // define histograms
        TH1F* h_Q = new TH1F("h_Q", "Q", 100, 0, 1);
        array->AddAt(h_Q, hQ);
        TH1F* h_DeltaR1 = new TH1F("h_DeltaR1", "DeltaR1", 600, 0, sqrt(4 + pow(TMath::Pi(), 2)));
        array->AddAt(h_DeltaR1, hDeltaR1);
        TH1F* h_DeltaR2 = new TH1F("h_DeltaR2", "DeltaR2", 600, 0, sqrt(4 + pow(TMath::Pi(), 2)));
        array->AddAt(h_DeltaR2, hDeltaR2);
        TH1F* h_DeltaR = new TH1F("h_DeltaR", "DeltaR", 600, 0, sqrt(4 + pow(TMath::Pi(), 2)));
        array->AddAt(h_DeltaR, hDeltaR);
        TH1F* h_CosMotherAndDaughter_CMS = new TH1F("h_CosMotherAndDaughter_CMS", "CosMotherAndDaughter_CMS", 100, -1, 1);
        array->AddAt(h_CosMotherAndDaughter_CMS, hCosMotherAndDaughter_CMS);
        TH2F* h_DeltaPhi_Pt = new TH2F("h_DeltaPhi_Pt", "DeltaPhi_Pt", 200, 0, 20, 100, 0, TMath::Pi());
        array->AddAt(h_DeltaPhi_Pt, hDeltaPhi_Pt);
        TH2F* h_DeltaEta_Pt = new TH2F("h_DeltaEta_Pt", "DeltaEta_Pt", 200, 0, 20, 100, 0, 2);
        array->AddAt(h_DeltaEta_Pt, hDeltaEta_Pt);
        TH2F* h_DeltaR_Pt = new TH2F("h_DeltaR_Pt", "DeltaR_Pt", 200, 0, 20, 600, 0, sqrt(4 + pow(TMath::Pi(), 2)));
        array->AddAt(h_DeltaR_Pt, hDeltaR_Pt);
        TH1F* h_DipionsMass = new TH1F("h_DipionsMass", "DipionsMass", 100, 0, 1.0);
        array->AddAt(h_DipionsMass, hDipionsMass);
        TH1F* hPt = new TH1F("hPt", "Pt", 20, 0, 20);
        hPt->Sumw2();
        array->AddAt(hPt, histoname::hPt);
        TH1F* hMass = new TH1F("hMass", "Mass", 80, 3.6, 4.0);
        hMass->Sumw2();
        array->AddAt(hMass, histoname::hMass);
        TH1F* hMassX3872 = new TH1F("hMassX3872", "MassX3872", 20, 3.8, 4.0);
        hMassX3872->Sumw2();
        array->AddAt(hMassX3872, histoname::hMassX3872);
        TH2F* hMass_Pt = new TH2F("hMass_Pt", "Mass_Pt", 20, 0, 20, 80, 3.6, 4.0);
        hMass_Pt->Sumw2();
        array->AddAt(hMass_Pt, histoname::HMass_Pt);
    }

    //---------------------------------------------------------------------------------------------------
    void FillHistograms(TObjArray* array, double* values){
        ((TH1F*)array->At(hQ))->Fill(values[QuadVarManager::Variables::kQ]);
        ((TH1F*)array->At(hDeltaR1))->Fill(values[QuadVarManager::Variables::kDeltaR1]);
        ((TH1F*)array->At(hDeltaR2))->Fill(values[QuadVarManager::Variables::kDeltaR2]);
        ((TH1F*)array->At(hDeltaR))->Fill(values[QuadVarManager::Variables::kDeltaR]);
        ((TH1F*)array->At(hCosMotherAndDaughter_CMS))->Fill(values[QuadVarManager::Variables::kCosDileptonDiTracks]);
        ((TH2F*)array->At(hDeltaPhi_Pt))->Fill(values[QuadVarManager::Variables::kPt], values[QuadVarManager::Variables::kDeltaPhi]);
        ((TH2F*)array->At(hDeltaEta_Pt))->Fill(values[QuadVarManager::Variables::kPt], values[QuadVarManager::Variables::kDeltaEta]);
        ((TH2F*)array->At(hDeltaR_Pt))->Fill(values[QuadVarManager::Variables::kPt], values[QuadVarManager::Variables::kDeltaR]);
        ((TH1F*)array->At(hDipionsMass))->Fill(values[QuadVarManager::Variables::kDihardonMass]);
        ((TH1F*)array->At(hPt))->Fill(values[QuadVarManager::Variables::kPt]);
        ((TH1F*)array->At(hMass))->Fill(values[QuadVarManager::Variables::kMass]);
        ((TH1F*)array->At(hMassX3872))->Fill(values[QuadVarManager::Variables::kMass]);
        ((TH2F*)array->At(HMass_Pt))->Fill(values[QuadVarManager::Variables::kPt], values[QuadVarManager::Variables::kMass]);
    }

    //---------------------------------------------------------------------------------------------------
    void WriteHistograms(TObjArray* array, TFile* file, char* ListName){
        file->cd();
        MTool::SaveHistogramInTList<TH1F>(file, ListName, (TH1F*)array->At(hQ));
        MTool::SaveHistogramInTList<TH1F>(file, ListName, (TH1F*)array->At(hDeltaR1));
        MTool::SaveHistogramInTList<TH1F>(file, ListName, (TH1F*)array->At(hDeltaR2));
        MTool::SaveHistogramInTList<TH1F>(file, ListName, (TH1F*)array->At(hDeltaR));
        MTool::SaveHistogramInTList<TH1F>(file, ListName, (TH1F*)array->At(hCosMotherAndDaughter_CMS));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(hDeltaPhi_Pt));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(hDeltaEta_Pt));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(hDeltaR_Pt));
        MTool::SaveHistogramInTList<TH1F>(file, ListName, (TH1F*)array->At(hDipionsMass));
        MTool::SaveHistogramInTList<TH1F>(file, ListName, (TH1F*)array->At(hPt));
        MTool::SaveHistogramInTList<TH1F>(file, ListName, (TH1F*)array->At(hMass));
        MTool::SaveHistogramInTList<TH1F>(file, ListName, (TH1F*)array->At(hMassX3872));
        MTool::SaveHistogramInTList<TH2F>(file, ListName, (TH2F*)array->At(HMass_Pt));
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
        c->SaveAs("output/Efficiency_pT.pdf");
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
        TLegend* leg = new TLegend(0.6, 0.8-h_temp.size()*0.055, 0.85, 0.8);
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