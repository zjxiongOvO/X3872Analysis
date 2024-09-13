// the minimum example for signal extraction of X3872 and Psi2S
// by using a gaussian function for signal and a polynomial function for background

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



void SignalExtraction_X3872(TH1F* hData, // unlike-sign data
                            TH1F* hCombBkg, // estimated combinatorial background, e.g. from like-sign, rotation, or mixed events
                            double massRangeLow, double massRangeHigh, // mass range for fitting and drawing
                            double binWidth, // bin width of the histogram
                            double pTlow, double pThigh, // pT range for the histogram
                            string signalname, // name of the signal, e.g. X3872, Psi2S, etc.
                            string method, // method for signal extraction, e.g. fitdirect, withbkg, etc.
                            string outputpath) // output path for saving the results

{
    // check if signal name is X3872 or Psi2S
    if (signalname != "X3872" && signalname != "Psi2S"){
        cout << "Error: invalid signal name!" << endl;
        return;
    }
    // check if method is fitdirect or withbkg
    if (method != "fitdirect" && method != "withbkg"){
        cout << "Error: invalid method!" << endl;
        return;
    }
    // pre-defined values to control the fitting
    const double signal_mean_X3872 = 3.872;
    const double signal_mean_Psi2S = 3.686;
    const double signal_width = 0.005;
    // pre-defined latex texts for drawing
    string decaychannel = (signalname == "X3872") ? "X(3872) #rightarrow J/#psi(#rightarrowe^{+}e^{#font[122]{-}})#pi^{+}#pi^{#font[122]{-}}" : "#psi(2S) #rightarrow J/#psi(#rightarrowe^{+}e^{#font[122]{-}})#pi^{+}#pi^{#font[122]{-}}";
    // pre-defined functions for signal extraction
    TF1* gkf_signal = new TF1("fSignal", "gaus", 3.6, 4);
    TF1* gkf_background = new TF1("fBackground", "pol2(0)", 3.6, 4);
    TF1* gkf_global = new TF1("fGlobal", "gaus(0) + pol2(3)", 3.6, 4);

    // clone the histograms
    TH1F* hCandidates = (TH1F*)hData->Clone("hCandidates");
    hCandidates->SetName("hCandidates");

    // rebin the histograms
    hCandidates->Rebin((int)(binWidth / hCandidates->GetBinWidth(1)));
    hCombBkg->Rebin((int)(binWidth / hCombBkg->GetBinWidth(1)));

    hCandidates->GetXaxis()->SetRangeUser(massRangeLow, massRangeHigh);
    TH1F* hRawCounts = (TH1F*)hCandidates->Clone("hRawCounts");
    hRawCounts->SetName("hRawCounts");
    for (int i = 1; i <= hRawCounts->GetNbinsX(); i++){
        hRawCounts->SetBinContent(i, hCandidates->GetBinContent(i) - hCombBkg->GetBinContent(i));
        hRawCounts->SetBinError(i, sqrt(pow(hCandidates->GetBinError(i), 2) + pow(hCombBkg->GetBinError(i), 2)));
    }
    double signal_leftMargin = (signalname == "X3872") ? signal_mean_X3872 - signal_width*3 : signal_mean_Psi2S - signal_width*3;
    double signal_rightMargin = (signalname == "X3872") ? signal_mean_X3872 + signal_width*3 : signal_mean_Psi2S + signal_width*3;


    //------------------------------------------------  
    // set initial parameters for signal and background
    gkf_signal->SetParameters(1, (signalname == "X3872") ? signal_mean_X3872 : signal_mean_Psi2S, signal_width);

    // pre-fit, remove the signal region and only fit the background
    TH1F* hBackground;
    if (method == "fitdirect"){
        hBackground = (TH1F*)hCandidates->Clone("hBackground");
    }
    else if (method == "withbkg"){
        hBackground = (TH1F*)hRawCounts->Clone("hBackground");
    }
    else{
        cout << "Error: invalid method!" << endl;
        return;
    }
    for (int i = 1; i <= hBackground->GetNbinsX(); i++){
        if (hBackground->GetBinCenter(i) > signal_leftMargin && hBackground->GetBinCenter(i) < signal_rightMargin){
            hBackground->SetBinContent(i, 0);
            hBackground->SetBinError(i, 0);
        }
    }
    hBackground->Fit(gkf_background, "RSI0", "", massRangeLow, massRangeHigh);

    // set global function
    for (int i = 0; i < gkf_signal->GetNpar(); i++){
        gkf_global->SetParameter(i, gkf_signal->GetParameter(i));
    }
    for (int i = 0; i < gkf_background->GetNpar(); i++){
        gkf_global->SetParameter(gkf_signal->GetNpar() + i, gkf_background->GetParameter(i));
    }

    // set global function limits
    gkf_global->SetParLimits(0, 0, 1e6);
    gkf_global->SetParLimits(1, signal_leftMargin, signal_rightMargin);
    gkf_global->SetParLimits(2, 0, signal_width*3);

    // fit
    TFitResultPtr r;
    if (method == "fitdirect"){
        r = hCandidates->Fit(gkf_global, "RSI0", "", massRangeLow, massRangeHigh);
    }
    else if (method == "withbkg"){
        r = hRawCounts->Fit(gkf_global, "RSI0", "", massRangeLow, massRangeHigh);
    }
    else{
        cout << "Error: invalid method!" << endl;
        return;
    }
    TMatrixDSym covGlobal = r->GetCovarianceMatrix();
    TMatrixDSym covSignal = covGlobal.GetSub(0, gkf_signal->GetNpar() - 1, 0, gkf_signal->GetNpar() - 1);
    TMatrixDSym covBackground = covGlobal.GetSub(gkf_signal->GetNpar(), gkf_signal->GetNpar() + gkf_background->GetNpar() - 1, gkf_signal->GetNpar(), gkf_signal->GetNpar() + gkf_background->GetNpar() - 1);

    // reset parameters
    for (int i = 0; i < gkf_signal->GetNpar(); i++){
        gkf_signal->SetParameter(i, gkf_global->GetParameter(i));
    }
    for (int i = 0; i < gkf_background->GetNpar(); i++){
        gkf_background->SetParameter(i, gkf_global->GetParameter(gkf_signal->GetNpar() + i));
    }

    // calculate the variables
    double meanSignal = gkf_global->GetParameter(1);
    double meanSignalErr = gkf_global->GetParError(1);
    double sigmaSignal = gkf_global->GetParameter(2);
    double sigmaSignalErr = gkf_global->GetParError(2);

    double N_signal = gkf_signal->Integral(meanSignal - 3*sigmaSignal, meanSignal + 3*sigmaSignal) / hCandidates->GetBinWidth(1);
    double N_signal_err = gkf_signal->IntegralError(meanSignal - 3*sigmaSignal, meanSignal + 3*sigmaSignal, gkf_signal->GetParameters(), covSignal.GetMatrixArray()) / hCandidates->GetBinWidth(1);

    double N_background_fromFit = gkf_background->Integral(meanSignal - 3*sigmaSignal, meanSignal + 3*sigmaSignal) / hCandidates->GetBinWidth(1);
    double N_background_fromFit_err = gkf_background->IntegralError(meanSignal - 3*sigmaSignal, meanSignal + 3*sigmaSignal, gkf_background->GetParameters(), covBackground.GetMatrixArray()) / hCandidates->GetBinWidth(1);

    double N_background_fromCount = 0;
    N_background_fromCount = hCombBkg->Integral(hCombBkg->FindBin(meanSignal - 3*sigmaSignal) + 1, hCombBkg->FindBin(meanSignal + 3*sigmaSignal) -1);
    double N_background_fromCount_Leftbin = hCombBkg->GetBinContent(hCombBkg->FindBin(meanSignal - 3*sigmaSignal)) * (1 - ((meanSignal - 3*sigmaSignal) - hCombBkg->GetBinLowEdge(hCombBkg->FindBin(meanSignal - 3*sigmaSignal))) / hCombBkg->GetBinWidth(hCombBkg->FindBin(meanSignal - 3*sigmaSignal)));
    double N_background_fromCount_Rightbin = hCombBkg->GetBinContent(hCombBkg->FindBin(meanSignal + 3*sigmaSignal)) * ((meanSignal + 3*sigmaSignal) - hCombBkg->GetBinLowEdge(hCombBkg->FindBin(meanSignal + 3*sigmaSignal))) / hCombBkg->GetBinWidth(hCombBkg->FindBin(meanSignal + 3*sigmaSignal));
    N_background_fromCount += N_background_fromCount_Leftbin + N_background_fromCount_Rightbin;

    double S_over_B=-999;
    double significance=-999;

    if (method == "fitdirect"){
        S_over_B = N_signal / N_background_fromFit;
        significance = N_signal / sqrt(N_signal + N_background_fromFit);
    }
    else if (method == "withbkg"){
        S_over_B = N_signal / (N_background_fromCount + N_background_fromFit);
        significance = N_signal / sqrt(N_signal + (N_background_fromCount + N_background_fromFit)*2);
    }
    else{
        cout << "Error: invalid method!" << endl;
        return;
    }

    // draw
    gStyle->SetOptStat(0);
    TCanvas* temp = new TCanvas("temp","temp",800,600);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    gPad->SetRightMargin(0.04);
    gPad->SetTickx();
    gPad->SetTicky();
    hCandidates->Draw();
    MTool::StandardSet(hCandidates, "#it{m}_{J/#psi#pi^{+}#pi^{#font[122]{-}}} - #it{m}_{J/#psi} + #it{m}_{J/#psi}^{PDG} (GeV/#it{c}^{2})", Form("Entries per %.0f MeV/#it{c}^{2}", hCandidates->GetBinWidth(1)*1000));
    MTool::SetMarkerLine(hCandidates, 1, 24, 1, 1, 1);
    hCandidates->GetXaxis()->SetNdivisions(505);
    hCandidates->GetYaxis()->SetMaxDigits(3);
    MTool::SetTF1(gkf_global, kRed, 1, 2, 20000);
    MTool::SetTF1(gkf_background, kBlue, 2, 2, 20000);  
    if (method == "fitdirect"){
        gkf_global->Draw("same");
        gkf_background->Draw("same");
    }
    if (method == "withbkg"){
        MTool::StandardSet(hCombBkg, "#it{m}_{J/#psi#pi^{+}#pi^{#font[122]{-}}} - #it{m}_{J/#psi} + #it{m}_{J/#psi}^{PDG} (GeV/#it{c}^{2})", Form("Entries per %.0f MeV/#it{c}^{2}", hCombBkg->GetBinWidth(1)*1000));
        MTool::SetMarkerLine(hCombBkg, 4, 24, 1, 1, 1);
        hCombBkg->Draw("same");
        MTool::StandardSet(hRawCounts, "#it{m}_{J/#psi#pi^{+}#pi^{#font[122]{-}}} - #it{m}_{J/#psi} + #it{m}_{J/#psi}^{PDG} (GeV/#it{c}^{2})", Form("Entries per %.0f MeV/#it{c}^{2}", hRawCounts->GetBinWidth(1)*1000));
        MTool::SetMarkerLine(hRawCounts, 2, 24, 1, 1, 1);
        hRawCounts->Draw("same");
        double max = hCandidates->GetMaximum();
        double min = hRawCounts->GetMinimum();
        hCandidates->GetYaxis()->SetRangeUser(min, max*1.2);
        gkf_global->Draw("same");
        gkf_background->Draw("same");
    }
    TLegend* leg = new TLegend(0.60,0.22,0.82,0.47);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.04);
    leg->AddEntry(hCandidates,"Data","lep");
    leg->AddEntry(gkf_global,"Total fit","l");
    if (method == "withbkg"){
        leg->AddEntry(hCombBkg,"Comb. background","lep");
        leg->AddEntry(hRawCounts,"Data - comb. background","lep");
        leg->AddEntry(gkf_background,"Residual background","l");
    }
    if (method == "fitdirect"){
        leg->AddEntry(gkf_background,"Comb. background","l");
    }
    leg->Draw();
    MTool::SetLatex(0.18, 0.82, Form("pp, #sqrt{#it{s}} = 13.6 TeV, %s", decaychannel.c_str()), 42, 0.04, 1);
    MTool::SetLatex(0.18, 0.76, Form("%.1f < #it{p}_{T} < %.1f GeV/#it{c}, |#it{y}| < 0.9", pTlow, pThigh), 42, 0.04, 1);
    MTool::SetLatex(0.18, 0.70, Form("N_{fit} = %.0f #pm %.0f, S/B = %.3f", N_signal, N_signal_err, S_over_B), 42, 0.04, 1);
    MTool::SetLatex(0.18, 0.64, Form("Significance = %.1f", significance), 42, 0.04, 1);
    
    temp->SaveAs(Form("%s/%s_%.1f_%.1f_%s.pdf", outputpath.c_str(), signalname.c_str(), pTlow, pThigh, method.c_str()));

    // save the results to a pre-defined file
}