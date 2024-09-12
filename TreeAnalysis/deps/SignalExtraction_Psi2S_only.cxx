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

void SignalExtraction_Psi2S_only(TH1F* hCandidate, double massRangeLow, double massRangeHigh, string outputpath){
    // pre-defined functions for signal extraction
    TF1* gkf_global = nullptr;
    TF1* gkf_Psi2S = nullptr;
    TF1* gkf_background = nullptr;
    
    // Initialize
    gkf_Psi2S = new TF1("fPsi2S", "gaus", 3.6, 4);
    gkf_background = new TF1("fBackground", "pol3(0)", 3.6, 4);
    gkf_global = new TF1("fGlobal", "gaus(0) + pol3(3)", 3.6, 4);

    hCandidate->GetXaxis()->SetRangeUser(massRangeLow, massRangeHigh);

    double Psi2S_leftMargin = 3.686 - 0.008*3;
    double Psi2S_rightMargin = 3.686 + 0.008*3;
    
    // pre-fit, remove the signal region and only fit the background
    TH1F* hBackground = (TH1F*)hCandidate->Clone("hBackground");
    for (int i = 1; i <= hBackground->GetNbinsX(); i++){
        if (hBackground->GetBinCenter(i) > Psi2S_leftMargin && hBackground->GetBinCenter(i) < Psi2S_rightMargin){
            hBackground->SetBinContent(i, 0);
            hBackground->SetBinError(i, 0);
        }
    }
    hBackground->Fit(gkf_background, "RSI0", "", massRangeLow, massRangeHigh);

    // set initial parameters for Psi2S and Psi2S
    gkf_Psi2S->SetParameters(1, 3.686, 0.008);
    
    // set global function
    for (int i = 0; i < gkf_Psi2S->GetNpar(); i++){
        gkf_global->SetParameter(i, gkf_Psi2S->GetParameter(i));
    }


    for (int i = 0; i < gkf_background->GetNpar(); i++){
        gkf_global->SetParameter(gkf_Psi2S->GetNpar() + i, gkf_background->GetParameter(i));
    }

    // set global function limits
    gkf_global->SetParLimits(0, 0, 1e6);
    gkf_global->SetParLimits(1, 3.686 - 0.008*3, 3.686 + 0.008*3);
    gkf_global->SetParLimits(2, 0.004, 0.008*2);

    // fit
    TFitResultPtr r = hCandidate->Fit(gkf_global, "RSI0", "", massRangeLow, massRangeHigh);
    TMatrixDSym covGlobal = r->GetCovarianceMatrix();
    TMatrixDSym covPsi2S = covGlobal.GetSub(0, gkf_Psi2S->GetNpar() - 1, 0, gkf_Psi2S->GetNpar() - 1);
    TMatrixDSym covBackground = covGlobal.GetSub(gkf_Psi2S->GetNpar(), gkf_Psi2S->GetNpar() + gkf_background->GetNpar() - 1, gkf_Psi2S->GetNpar(), gkf_Psi2S->GetNpar() + gkf_background->GetNpar() - 1);

    // reset parameters
    for (int i = 0; i < gkf_Psi2S->GetNpar(); i++){
        gkf_Psi2S->SetParameter(i, gkf_global->GetParameter(i));
    }
    for (int i = 0; i < gkf_background->GetNpar(); i++){
        gkf_background->SetParameter(i, gkf_global->GetParameter(gkf_Psi2S->GetNpar() + i));
    }


    // calculate the variables
    double meanPsi2S = gkf_Psi2S->GetParameter(1);
    double sigmaPsi2S = gkf_Psi2S->GetParameter(2);

    // calculate the significance in 3 sigma region
    double N_Psi2S = gkf_Psi2S->Integral(meanPsi2S - 3*sigmaPsi2S, meanPsi2S + 3*sigmaPsi2S) / hCandidate->GetBinWidth(1);
    double N_Psi2S_err = gkf_Psi2S->IntegralError(meanPsi2S - 3*sigmaPsi2S, meanPsi2S + 3*sigmaPsi2S, gkf_Psi2S->GetParameters(), covPsi2S.GetMatrixArray()) / hCandidate->GetBinWidth(1);
    double N_background_Psi2S = gkf_background->Integral(meanPsi2S - 3*sigmaPsi2S, meanPsi2S + 3*sigmaPsi2S) / hCandidate->GetBinWidth(1);
    double N_background_Psi2S_err = gkf_background->IntegralError(meanPsi2S - 3*sigmaPsi2S, meanPsi2S + 3*sigmaPsi2S, gkf_background->GetParameters(), covBackground.GetMatrixArray()) / hCandidate->GetBinWidth(1);

    double S_over_B_Psi2S = N_Psi2S / N_background_Psi2S;
    double significance_Psi2S = N_Psi2S / TMath::Sqrt(N_Psi2S + N_background_Psi2S);

    double N_Psi2S_err_comb = TMath::Sqrt(N_Psi2S_err*N_Psi2S_err + N_background_Psi2S_err*N_background_Psi2S_err);


    TH1F* hRawCounts = (TH1F*)hCandidate->Clone("hRawCounts");
    hRawCounts->SetName("hRawCounts");
    for (int i = 1; i <= hRawCounts->GetNbinsX(); i++){
        hRawCounts->SetBinContent(i, hCandidate->GetBinContent(i) - gkf_background->Eval(hCandidate->GetBinCenter(i)));
        hRawCounts->SetBinError(i, hCandidate->GetBinError(i));
    }

    // draw
    gStyle->SetOptStat(0);
    TCanvas* temp = new TCanvas("temp","temp",800,600);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    gPad->SetRightMargin(0.04);
    gPad->SetTickx();
    gPad->SetTicky();
    hCandidate->Draw();
    MTool::StandardSet(hCandidate, "#it{m}_{J/#psi#pi^{+}#pi^{#font[122]{-}}} - #it{m}_{J/#psi} + #it{m}_{J/#psi}^{PDG} (GeV/#it{c}^{2})", Form("Entries per %.0f MeV/#it{c}^{2}", hCandidate->GetBinWidth(1)*1000));
    // MTool::StandardSet(hCandidate, "#it{m}_{e^{+}e^{-}#pi^{+}#pi^{-}} - #it{m}_{e^{+}e^{-}} + #it{m}_{J/#psi}^{PDG} (GeV/#it{c}^{2})", Form("Entries per %.0f MeV/#it{c}^{2}", hCandidate->GetBinWidth(1)*1000));
    MTool::SetMarkerLine(hCandidate, 1, 24, 1, 1, 1);
    hCandidate->GetXaxis()->SetNdivisions(505);
    // hCandidate->GetYaxis()->SetTitleOffset(1.25);
    hCandidate->GetYaxis()->SetMaxDigits(3);
    hCandidate->GetXaxis()->SetRangeUser(3.64, 3.72);
    // hCandidate->GetYaxis()->SetRangeUser(hCandidate->GetMinimum()*0.95, hCandidate->GetMaximum()*1.1);
    MTool::SetTF1(gkf_global, kRed, 1, 2, 2000);
    MTool::SetTF1(gkf_background, kBlue, 2, 2, 8000);
    // MTool::SetTF1(gkf_Psi2S, kRed, 1, 2, 8000);
    gkf_global->Draw("same");
    gkf_background->Draw("same");
    TLegend* leg = new TLegend(0.60,0.22,0.82,0.47);
    // TLegend* leg = new TLegend(0.18,0.35,0.42,0.62);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.04);
    leg->AddEntry(hCandidate,"Data","lep");
    leg->AddEntry(gkf_global,"Total fit","l");
    leg->AddEntry(gkf_background,"Comb. background","l");
    leg->Draw();
    MTool::SetLatex(0.18, 0.82, Form("ALICE Performance"), 42, 0.05, 1);
    MTool::SetLatex(0.18, 0.75, Form("pp, #sqrt{#it{s}} = 13.6 TeV, #psi(2S) #rightarrow J/#psi(#rightarrowe^{+}e^{#font[122]{-}})#pi^{+}#pi^{#font[122]{-}}"), 42, 0.045, 1);
    MTool::SetLatex(0.18, 0.68, Form("4 < #it{p}_{T} < 35 GeV/#it{c}, |#it{y}| < 0.9"), 42, 0.045, 1);
    MTool::SetLatex(0.18, 0.61, Form("Significance = %.1f", significance_Psi2S), 42, 0.045, 1);
    // MTool::SetLatex(0.18, 0.82, Form("N_{#psi(2S)} = %.0f #pm %.0f", N_Psi2S, N_Psi2S_err_comb), 42, 0.04, 1);
    // MTool::SetLatex(0.18, 0.77, Form("Significance = %.2f", significance_Psi2S), 42, 0.04, 1);
    // MTool::SetLatex(0.18, 0.72, Form("#mu = %.3f #pm %.3f GeV", gkf_global->GetParameter(1), gkf_global->GetParError(1)), 42, 0.04, 1);
    // MTool::SetLatex(0.18, 0.67, Form("#sigma = %.3f #pm %.3f GeV", gkf_global->GetParameter(2), gkf_global->GetParError(2)), 42, 0.04, 1);
    
    // TPad *pad = new TPad("pad", "Inset Pad", 0.18, 0.18, 0.55, 0.45);
    // pad->SetFillColor(0);
    // pad->Draw();
    // pad->cd();

    // hRawCounts->SetTitle("");
    // // MTool::SetMarkerLine(hRawCounts, 1, 24, 1, 1, 1);
    // hRawCounts->GetXaxis()->SetNdivisions(505);
    // hRawCounts->GetYaxis()->SetRangeUser(hRawCounts->GetMinimum(), hRawCounts->GetMaximum()*1.5);
    // hRawCounts->Draw();
    // MTool::SetTF1(gkf_Psi2S, kRed, 1, 2, 2000);
    // gkf_Psi2S->Draw("same");

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
    //  MTool::SetTF1(gkf_Psi2S, kRed, 1, 2, 2000);
    // gkf_Psi2S->Draw("same");
    // cRawCounts->SaveAs(Form("%s_rawCounts.pdf", outputpath.c_str()));
}