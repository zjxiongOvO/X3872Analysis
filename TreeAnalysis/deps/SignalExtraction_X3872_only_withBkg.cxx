#include <TF1.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TMatrixDSym.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TStyle.h>

#include "Mtool.h"

void SignalExtraction_X3872_only_withBkg(TH1F* hCandidate, TH1F*hBkg, double massRangeLow, double massRangeHigh, string outputpath){
    // pre-defined functions for signal extraction
    TF1* gkf_global = nullptr;
    TF1* gkf_X3872 = nullptr;
    TF1* gkf_background = nullptr;
    
    // Initialize
    gkf_X3872 = new TF1("fX3872", "gaus", 3.6, 4);
    gkf_background = new TF1("fBackground", "pol3(0)", 3.6, 4);
    gkf_global = new TF1("fGlobal", "gaus(0) + pol3(3)", 3.6, 4);

    hCandidate->GetXaxis()->SetRangeUser(massRangeLow, massRangeHigh);
    hBkg->GetXaxis()->SetRangeUser(massRangeLow, massRangeHigh);
    
    TH1F* hRawCounts = (TH1F*)hCandidate->Clone("hRawCounts");
    hRawCounts->SetName("hRawCounts");
    for (int i = 1; i <= hRawCounts->GetNbinsX(); i++){
        hRawCounts->SetBinContent(i, hCandidate->GetBinContent(i) - hBkg->GetBinContent(i));
        double err = TMath::Sqrt(hCandidate->GetBinError(i)*hCandidate->GetBinError(i) + hBkg->GetBinError(i)*hBkg->GetBinError(i));
        hRawCounts->SetBinError(i, err);
    }

    double X3872_leftMargin = 3.872 - 0.008*3;
    double X3872_rightMargin = 3.872 + 0.008*3;
    
    // pre-fit, remove the signal region and only fit the background
    TH1F* hBackground = (TH1F*)hRawCounts->Clone("hBackground");
    for (int i = 1; i <= hBackground->GetNbinsX(); i++){
        if (hBackground->GetBinCenter(i) > X3872_leftMargin && hBackground->GetBinCenter(i) < X3872_rightMargin){
            hBackground->SetBinContent(i, 0);
            hBackground->SetBinError(i, 0);
        }
    }
    hBackground->Fit(gkf_background, "RSI0", "", massRangeLow, massRangeHigh);

    // set initial parameters for X3872 and X3872
    gkf_X3872->SetParameters(1, 3.872, 0.008);
    
    // set global function
    for (int i = 0; i < gkf_X3872->GetNpar(); i++){
        gkf_global->SetParameter(i, gkf_X3872->GetParameter(i));
    }


    for (int i = 0; i < gkf_background->GetNpar(); i++){
        gkf_global->SetParameter(gkf_X3872->GetNpar() + i, gkf_background->GetParameter(i));
    }

    // set global function limits
    gkf_global->SetParLimits(0, 0, 1e6);
    gkf_global->SetParLimits(1, 3.872 - 0.008*3, 3.872 + 0.008*3);
    gkf_global->SetParLimits(2, 0.004, 0.008*2);

    // fit
    TFitResultPtr r = hRawCounts->Fit(gkf_global, "RSI0", "", massRangeLow, massRangeHigh);
    TMatrixDSym covGlobal = r->GetCovarianceMatrix();
    TMatrixDSym covX3872 = covGlobal.GetSub(0, gkf_X3872->GetNpar() - 1, 0, gkf_X3872->GetNpar() - 1);
    TMatrixDSym covBackground = covGlobal.GetSub(gkf_X3872->GetNpar(), gkf_X3872->GetNpar() + gkf_background->GetNpar() - 1, gkf_X3872->GetNpar(), gkf_X3872->GetNpar() + gkf_background->GetNpar() - 1);

    // reset parameters
    for (int i = 0; i < gkf_X3872->GetNpar(); i++){
        gkf_X3872->SetParameter(i, gkf_global->GetParameter(i));
    }
    for (int i = 0; i < gkf_background->GetNpar(); i++){
        gkf_background->SetParameter(i, gkf_global->GetParameter(gkf_X3872->GetNpar() + i));
    }


    // calculate the variables
    double meanX3872 = gkf_X3872->GetParameter(1);
    double sigmaX3872 = gkf_X3872->GetParameter(2);

    // calculate the significance in 3 sigma region
    double N_X3872 = gkf_X3872->Integral(meanX3872 - 3*sigmaX3872, meanX3872 + 3*sigmaX3872) / hRawCounts->GetBinWidth(1);
    double N_X3872_err = gkf_X3872->IntegralError(meanX3872 - 3*sigmaX3872, meanX3872 + 3*sigmaX3872, gkf_X3872->GetParameters(), covX3872.GetMatrixArray()) / hRawCounts->GetBinWidth(1);
    double N_background_X3872 = gkf_background->Integral(meanX3872 - 3*sigmaX3872, meanX3872 + 3*sigmaX3872) / hRawCounts->GetBinWidth(1);
    double N_background_X3872_err = gkf_background->IntegralError(meanX3872 - 3*sigmaX3872, meanX3872 + 3*sigmaX3872, gkf_background->GetParameters(), covBackground.GetMatrixArray()) / hRawCounts->GetBinWidth(1);
    double N_comb_X3872 = hBkg->Integral(hBkg->FindBin(meanX3872 - 3*sigmaX3872), hBkg->FindBin(meanX3872 + 3*sigmaX3872));
    double N_comb_X3872_err = TMath::Sqrt(N_comb_X3872);

    double S_over_B_X3872 = N_X3872 / (N_background_X3872 + N_comb_X3872);
    double significance_X3872 = N_X3872 / TMath::Sqrt(N_X3872 + (N_background_X3872 + N_comb_X3872)*2);

    double N_X3872_err_comb = TMath::Sqrt(N_X3872_err*N_X3872_err + N_background_X3872_err*N_background_X3872_err);


    // draw
    gStyle->SetOptStat(0);
    TCanvas* temp = new TCanvas("temp","temp",800,600);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    hCandidate->Draw();
    MTool::StandardSet(hCandidate, "#it{m}_{J/#psi#pi#pi} - #it{m}_{J/#psi} + #it{m}_{J/#psi}^{PDG} (GeV/#it{c}^{2})", Form("Entries / %.0f MeV/#it{c}^{2}", hCandidate->GetBinWidth(1)*1000));
    MTool::SetMarkerLine(hCandidate, 1, 24, 1, 1, 1);
    hCandidate->GetXaxis()->SetNdivisions(505);
    hCandidate->GetYaxis()->SetTitleOffset(1.25);
    MTool::StandardSet(hBkg, "#it{m}_{J/#psi#pi#pi} - #it{m}_{J/#psi} + #it{m}_{J/#psi}^{PDG} (GeV/#it{c}^{2})", Form("Entries / %.0f MeV/#it{c}^{2}", hCandidate->GetBinWidth(1)*1000));
    MTool::SetMarkerLine(hBkg, 4, 24, 1, 1, 1);
    hBkg->Draw("same");
    MTool::SetTF1(gkf_global, kRed, 1, 2, 2000);
    // MTool::SetTF1(gkf_X3872, kBlue, 1, 2, 8000);
    // MTool::SetTF1(gkf_X3872, kGreen, 1, 2, 8000);
    MTool::SetTF1(gkf_background, kBlue, 1, 2, 8000);
    // gkf_global->Draw("same");
    // gkf_X3872->Draw("same");
    // gkf_background->Draw("same");
    TLegend* leg = new TLegend(0.60,0.22,0.82,0.47);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.04);
    leg->AddEntry(hCandidate,"Unlike sign","lep");
    leg->AddEntry(gkf_global,"Total fit","l");
    leg->AddEntry(gkf_background,"Residual bkg","l");
    leg->Draw();
    MTool::SetLatex(0.18, 0.82, Form("N_{X(3872)} = %.0f #pm %.0f", N_X3872, N_X3872_err_comb), 42, 0.04, 1);
    MTool::SetLatex(0.18, 0.77, Form("Significance = %.2f", significance_X3872), 42, 0.04, 1);
    MTool::SetLatex(0.18, 0.72, Form("#mu = %.3f #pm %.3f GeV", gkf_global->GetParameter(1), gkf_global->GetParError(1)), 42, 0.04, 1);
    MTool::SetLatex(0.18, 0.67, Form("#sigma = %.3f #pm %.3f GeV", gkf_global->GetParameter(2), gkf_global->GetParError(2)), 42, 0.04, 1);
    
    TPad *pad = new TPad("pad", "Inset Pad", 0.18, 0.18, 0.55, 0.45);
    pad->SetFillColor(0);
    pad->Draw();
    pad->cd();

    hRawCounts->SetTitle("");
    hRawCounts->GetXaxis()->SetNdivisions(505);
    hRawCounts->GetYaxis()->SetRangeUser(hRawCounts->GetMinimum(), hRawCounts->GetMaximum()*1.5);
    hRawCounts->Draw();
    gkf_global->Draw("same");
    gkf_background->Draw("same");

    temp->cd();
    temp->SaveAs(outputpath.c_str());

    // TCanvas* cRawCounts = new TCanvas("cRawCounts", "cRawCounts", 800, 600);
    // gPad->SetLeftMargin(0.15);
    // gPad->SetBottomMargin(0.15);
    // hRawCounts->Draw();
    // MTool::StandardSet(hRawCounts, "#it{m}_{J/#psi#pi#pi} - #it{m}_{J/#psi} + #it{m}_{J/#psi}^{PDG} (GeV/#it{c}^{2})", Form("Entries / %.0f MeV/#it{c}^{2}", hRawCounts->GetBinWidth(1)*1000));
    // MTool::SetMarkerLine(hRawCounts, 1, 24, 1, 1, 1);
    // hRawCounts->GetXaxis()->SetNdivisions(505);
    // hRawCounts->GetYaxis()->SetTitleOffset(1.25);
    // hRawCounts->GetYaxis()->SetRangeUser(hRawCounts->GetMinimum(), hRawCounts->GetMaximum()*1.5);
    //  MTool::SetTF1(gkf_X3872, kRed, 1, 2, 2000);
    // gkf_X3872->Draw("same");
    // cRawCounts->SaveAs(Form("%s_rawCounts.pdf", outputpath.c_str()));
}