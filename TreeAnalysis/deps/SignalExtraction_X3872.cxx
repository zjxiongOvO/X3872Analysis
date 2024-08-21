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

// pre-defined functions for signal extraction
TF1* gkf_global = nullptr;
TF1* gkf_Psi2S = nullptr;
TF1* gkf_X3872 = nullptr;
TF1* gkf_background = nullptr;

void ResetParameters(){
    // once we fit the global function, we need to reset the parameters of Psi2S, X3872 and background
    if (gkf_Psi2S != nullptr && gkf_X3872 != nullptr && gkf_background != nullptr) {
        for (int i = 0; i < gkf_X3872->GetNpar(); i++){
            gkf_X3872->SetParameter(i, gkf_global->GetParameter(i));
        }
        for (int i = 0; i < gkf_Psi2S->GetNpar(); i++){
            gkf_Psi2S->SetParameter(i, gkf_global->GetParameter(gkf_X3872->GetNpar() + i));
        }
        for (int i = 0; i < gkf_background->GetNpar(); i++){
            gkf_background->SetParameter(i, gkf_global->GetParameter(gkf_X3872->GetNpar() + gkf_Psi2S->GetNpar() + i));
        }
    } else {
        std::cerr << "Error: gkf_Psi2S, gkf_X3872, gkf_background should be set before calling ResetParameters()" << std::endl;
        return;
    }
}

void SignalExtraction_X3872(TH1F* hCandidate, double massRangeLow, double massRangeHigh, string outputpath){
    // Initialize
    gkf_Psi2S = new TF1("fPsi2S", "gaus", 0, 15);
    gkf_X3872 = new TF1("fX3872", "gaus", 0, 15);
    gkf_background = new TF1("fBackground", "pol3(0)", 0, 15);
    gkf_global = new TF1("fGlobal", "gaus(0) + gaus(3) + pol3(6)", 0, 15);

    double X3872_leftMargin = 3.872 - 0.008*3;
    double X3872_rightMargin = 3.872 + 0.008*3;
    double Psi2S_leftMargin = 3.686 - 0.008*3;
    double Psi2S_rightMargin = 3.686 + 0.008*3;
    
    // pre-fit, remove the signal region and only fit the background
    TH1F* hBackground = (TH1F*)hCandidate->Clone("hBackground");
    for (int i = 1; i <= hBackground->GetNbinsX(); i++){
        if (hBackground->GetBinCenter(i) > X3872_leftMargin && hBackground->GetBinCenter(i) < X3872_rightMargin){
            hBackground->SetBinContent(i, 0);
            hBackground->SetBinError(i, 0);
        }
        if (hBackground->GetBinCenter(i) > Psi2S_leftMargin && hBackground->GetBinCenter(i) < Psi2S_rightMargin){
            hBackground->SetBinContent(i, 0);
            hBackground->SetBinError(i, 0);
        }
    }
    hBackground->Fit(gkf_background, "RSI0", "", massRangeLow, massRangeHigh);

    // set initial parameters for X3872 and Psi2S
    gkf_X3872->SetParameters(1, 3.872, 0.008);
    gkf_Psi2S->SetParameters(1, 3.686, 0.008);
    
    // set global function
    for (int i = 0; i < gkf_X3872->GetNpar(); i++){
        gkf_global->SetParameter(i, gkf_X3872->GetParameter(i));
    }

    for (int i = 0; i < gkf_Psi2S->GetNpar(); i++){
        gkf_global->SetParameter(gkf_X3872->GetNpar() + i, gkf_Psi2S->GetParameter(i));
    }

    for (int i = 0; i < gkf_background->GetNpar(); i++){
        gkf_global->SetParameter(gkf_X3872->GetNpar() + gkf_Psi2S->GetNpar() + i, gkf_background->GetParameter(i));
    }

    // set global function limits
    gkf_global->SetParLimits(0, 0, 1e6);
    gkf_global->SetParLimits(1, 3.872 - 0.008*3, 3.872 + 0.008*3);
    gkf_global->SetParLimits(2, 0.004, 0.008*2);
    gkf_global->SetParLimits(3, 0, 1e6);
    gkf_global->SetParLimits(4, 3.686 - 0.008*3, 3.686 + 0.008*3);
    gkf_global->SetParLimits(5, 0.004, 0.008*2);

    // fit
    TFitResultPtr r = hCandidate->Fit(gkf_global, "RSI0", "", massRangeLow, massRangeHigh);
    TMatrixDSym covGlobal = r->GetCovarianceMatrix();
    TMatrixDSym covX3872 = covGlobal.GetSub(0, gkf_X3872->GetNpar() - 1, 0, gkf_X3872->GetNpar() - 1);
    TMatrixDSym covPsi2S = covGlobal.GetSub(gkf_X3872->GetNpar(), gkf_X3872->GetNpar() + gkf_Psi2S->GetNpar() - 1, gkf_X3872->GetNpar(), gkf_X3872->GetNpar() + gkf_Psi2S->GetNpar() - 1);
    TMatrixDSym covBackground = covGlobal.GetSub(gkf_X3872->GetNpar() + gkf_Psi2S->GetNpar(), gkf_X3872->GetNpar() + gkf_Psi2S->GetNpar() + gkf_background->GetNpar() - 1, gkf_X3872->GetNpar() + gkf_Psi2S->GetNpar(), gkf_X3872->GetNpar() + gkf_Psi2S->GetNpar() + gkf_background->GetNpar() - 1);

    // reset parameters
    ResetParameters();

    // calculate the variables
    double meanX3872 = gkf_X3872->GetParameter(1);
    double sigmaX3872 = gkf_X3872->GetParameter(2);
    double meanPsi2S = gkf_Psi2S->GetParameter(1);
    double sigmaPsi2S = gkf_Psi2S->GetParameter(2);

    // calculate the significance in 3 sigma region
    double N_X3872 = gkf_X3872->Integral(meanX3872 - 3*sigmaX3872, meanX3872 + 3*sigmaX3872) / hCandidate->GetBinWidth(1);
    double N_X3872_err = gkf_X3872->IntegralError(meanX3872 - 3*sigmaX3872, meanX3872 + 3*sigmaX3872, gkf_X3872->GetParameters(), covX3872.GetMatrixArray()) / hCandidate->GetBinWidth(1);
    double N_Psi2S = gkf_Psi2S->Integral(meanPsi2S - 3*sigmaPsi2S, meanPsi2S + 3*sigmaPsi2S) / hCandidate->GetBinWidth(1);
    double N_Psi2S_err = gkf_Psi2S->IntegralError(meanPsi2S - 3*sigmaPsi2S, meanPsi2S + 3*sigmaPsi2S, gkf_Psi2S->GetParameters(), covPsi2S.GetMatrixArray()) / hCandidate->GetBinWidth(1);
    double N_background_X3872 = gkf_background->Integral(meanX3872 - 3*sigmaX3872, meanX3872 + 3*sigmaX3872) / hCandidate->GetBinWidth(1);
    // double N_background_X3872_err = gkf_background->IntegralError(meanX3872 - 3*sigmaX3872, meanX3872 + 3*sigmaX3872, r->GetParams(), covBackground.GetMatrixArray()) / hCandidate->GetBinWidth(1);
    double N_background_X3872_err = gkf_background->IntegralError(meanX3872 - 3*sigmaX3872, meanX3872 + 3*sigmaX3872, gkf_background->GetParameters(), covBackground.GetMatrixArray()) / hCandidate->GetBinWidth(1);
    double N_background_Psi2S = gkf_background->Integral(meanPsi2S - 3*sigmaPsi2S, meanPsi2S + 3*sigmaPsi2S) / hCandidate->GetBinWidth(1);
    // double N_background_Psi2S_err = gkf_background->IntegralError(meanPsi2S - 3*sigmaPsi2S, meanPsi2S + 3*sigmaPsi2S, r->GetParams(), covBackground.GetMatrixArray()) / hCandidate->GetBinWidth(1);
    double N_background_Psi2S_err = gkf_background->IntegralError(meanPsi2S - 3*sigmaPsi2S, meanPsi2S + 3*sigmaPsi2S, gkf_background->GetParameters(), covBackground.GetMatrixArray()) / hCandidate->GetBinWidth(1);

    double S_over_B_X3872 = N_X3872 / N_background_X3872;
    double S_over_B_Psi2S = N_Psi2S / N_background_Psi2S;
    double significance_X3872 = N_X3872 / TMath::Sqrt(N_X3872 + N_background_X3872);
    double significance_Psi2S = N_Psi2S / TMath::Sqrt(N_Psi2S + N_background_Psi2S);

    double N_X3872_err_comb = TMath::Sqrt(N_X3872_err*N_X3872_err + N_background_X3872_err*N_background_X3872_err);
    double N_Psi2S_err_comb = TMath::Sqrt(N_Psi2S_err*N_Psi2S_err + N_background_Psi2S_err*N_background_Psi2S_err);

    // draw
    gStyle->SetOptStat(0);
    TCanvas* temp = new TCanvas("temp","temp",800,600);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    hCandidate->Draw();
    MTool::StandardSet(hCandidate, "#it{m}_{J/#psi#pi#pi} (GeV/#it{c}^{2})", hCandidate->GetYaxis()->GetTitle());
    MTool::SetMarkerLine(hCandidate, 1, 24, 1, 1, 1);
    hCandidate->GetXaxis()->SetNdivisions(505);
    MTool::SetTF1(gkf_global, kRed, 1, 2, 20000);
    // MTool::SetTF1(gkf_X3872, kBlue, 1, 2, 8000);
    // MTool::SetTF1(gkf_Psi2S, kGreen, 1, 2, 8000);
    MTool::SetTF1(gkf_background, kBlack, 1, 2, 8000);
    gkf_global->Draw("same");
    gkf_X3872->Draw("same");
    gkf_Psi2S->Draw("same");
    gkf_background->Draw("same");
    TLegend* leg = new TLegend(0.60,0.62,0.82,0.87);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.04);
    leg->AddEntry(hCandidate,"Unlike sign","lep");
    leg->AddEntry(gkf_global,"Total fit","l");
    // leg->AddEntry(gkf_X3872,"X(3872)","l");
    // leg->AddEntry(gkf_Psi2S,"#Psi(2S)","l");
    leg->AddEntry(gkf_background,"Combination bkg","l");
    leg->Draw();
    MTool::SetLatex(0.18, 0.45, Form("N_{X(3872)} = %.0f #pm %.0f", N_X3872, N_X3872_err_comb), 42, 0.04, 1);
    MTool::SetLatex(0.18, 0.40, Form("N_{#Psi(2S)} = %.0f #pm %.0f", N_Psi2S, N_Psi2S_err_comb), 42, 0.04, 1);
    MTool::SetLatex(0.18, 0.35, Form("Significance_{X(3872)} = %.2f", significance_X3872), 42, 0.04, 1);
    MTool::SetLatex(0.18, 0.30, Form("Significance_{#Psi(2S)} = %.2f", significance_Psi2S), 42, 0.04, 1);
    temp->SaveAs(outputpath.c_str());

    // temp->Clear();
    // TH1F* hSignal = (TH1F*)hCandidate->Clone("hSignal");
    // // remove the background
    // for (int i = 1; i <= hSignal->GetNbinsX(); i++){
    //     double X = hSignal->GetBinCenter(i);
    //     double Y = hSignal->GetBinContent(i);
    //     double background = gkf_background->Eval(X);
    //     hSignal->SetBinContent(i, Y - background);
    // }
    // hSignal->Draw();
    // MTool::StandardSet(hSignal, "#it{m}_{J/#psi#pi#pi} (GeV/#it{c}^{2})", hSignal->GetYaxis()->GetTitle());
    // MTool::SetMarkerLine(hSignal, 1, 24, 1, 1, 1);
    // hSignal->GetXaxis()->SetNdivisions(505);
    // hSignal->GetYaxis()->SetRangeUser(hSignal->GetMinimum(), hSignal->GetMaximum()*1.2);
    // gkf_X3872->Draw("same");
    // gkf_Psi2S->Draw("same");
    // temp->SaveAs("output/SignalExtraction_X3872_Signal.pdf");
}