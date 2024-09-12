#include "../deps/HistoAndPlot.h"
// #include "../deps/SignalExtraction_X3872.cxx"

void PerformanceMaker(){
    TFile* myfile = new TFile("output/Performance.root", "read");
    TList* data = (TList*)myfile->Get("merged_FinaltuneX3872");
    TList* sameside = (TList*)myfile->Get("merged_FinaltuneX3872_bkg");

    TH1F* hUnlikeSign = (TH1F*)data->At(histoname::hMassX3872)->Clone();
    hUnlikeSign->SetName("hUnlikeSign");
    TH1F* hLikeSign = (TH1F*)sameside->At(histoname::hMassX3872)->Clone();
    hLikeSign->SetName("hLikeSign");

    // pre-defined function
    TF1* gkf_bkg = new TF1("gkf_bkg", "[0] + [1]*x + [2]*x*x", 3.6, 4.0);
    MTool::SetTF1(gkf_bkg, 3, 1, 1, 4000);
    // gkf_bkg->SetParameters(0, 0, 0);

    // setting the style
    gStyle->SetOptStat(0);
    double binWidth = 0.008; //GeV
    double massrangelow = 3.6;
    double massrangehigh = 3.92;
    bool isLogy = false;


    double ratioofbin = binWidth/hUnlikeSign->GetBinWidth(1);
    int rebin = (ratioofbin < 1) ? 1 : (int)ratioofbin;
    hUnlikeSign->Rebin(rebin);
    hLikeSign->Rebin(rebin);

    TCanvas* c = new TCanvas("c", "c", 1600, 1200);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    gPad->SetRightMargin(0.04);
    gPad->SetTickx();
    gPad->SetTicky();
    gPad->SetLogy(isLogy);

    // unlike sign
    MTool::StandardSet(hUnlikeSign, "#it{m}_{J/#psi#pi#pi} - #it{m}_{J/#psi} + #it{m}_{J/#psi}^{PDG} (GeV/#it{c}^{2})", Form("Entries per %.0f MeV/#it{c}^{2}", hUnlikeSign->GetBinWidth(1)*1000));
    MTool::SetMarkerLine(hUnlikeSign, 1, 24, 1, 1, 1);
    hUnlikeSign->GetXaxis()->SetNdivisions(505);
    // hUnlikeSign->GetYaxis()->SetTitleOffset(1.3);
    hUnlikeSign->GetYaxis()->SetMaxDigits(3);
    hUnlikeSign->GetXaxis()->SetRangeUser(massrangelow, massrangehigh);
    if (isLogy) {
        hUnlikeSign->SetMinimum(1);
        hUnlikeSign->SetMaximum(1e5);
    } else {
        hUnlikeSign->SetMinimum(0);
        hUnlikeSign->SetMaximum(1.1*hUnlikeSign->GetMaximum());
    }
    hUnlikeSign->Draw();
    // hUnlikeSign->Fit(gkf_bkg, "RSI", "", massrangelow, massrangehigh);

    // like sign
    MTool::StandardSet(hLikeSign, "#it{m}_{J/#psi#pi#pi} - #it{m}_{J/#psi} + #it{m}_{J/#psi}^{PDG} (GeV/#it{c}^{2})", Form("Entries per %.0f MeV/#it{c}^{2}", hLikeSign->GetBinWidth(1)*1000));
    MTool::SetMarkerLine(hLikeSign, 4, 24, 1, 1, 1);
    hLikeSign->GetXaxis()->SetNdivisions(505);
    hLikeSign->Draw("same");

    // Raw counts
    TH1F* hRawCounts = (TH1F*)hUnlikeSign->Clone("hRawCounts");
    hRawCounts->SetName("hRawCounts");
    for (int i = 1; i <= hRawCounts->GetNbinsX(); i++){
        hRawCounts->SetBinContent(i, hUnlikeSign->GetBinContent(i) - hLikeSign->GetBinContent(i));
        hRawCounts->SetBinError(i, sqrt(pow(hUnlikeSign->GetBinError(i), 2) + pow(hLikeSign->GetBinError(i), 2)));
    }
    MTool::StandardSet(hRawCounts, "#it{m}_{J/#psi#pi#pi} - #it{m}_{J/#psi} + #it{m}_{J/#psi}^{PDG} (GeV/#it{c}^{2})", Form("Entries per %.0f MeV/#it{c}^{2}", hRawCounts->GetBinWidth(1)*1000));
    MTool::SetMarkerLine(hRawCounts, 2, 24, 1, 1, 1);
    hRawCounts->Draw("same");
    // hRawCounts->Fit(gkf_bkg, "RSI", "", massrangelow, massrangehigh);


    // TLegend and Latex
    MTool::SetLatex(0.18, 0.82, Form("ALICE performance"), 42, 0.05, 1);
    MTool::SetLatex(0.18, 0.75, Form("pp, #sqrt{s} = 13.6 TeV"), 42, 0.05, 1);
    MTool::SetLatex(0.18, 0.68, Form("4 < #it{p}_{T} < 35 GeV/#it{c}, |y| < 0.9"), 42, 0.04, 1);

    // save
    c->SaveAs("output/Performance/Performance.pdf");



}