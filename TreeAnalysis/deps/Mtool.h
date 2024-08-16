#ifndef _MTOOL_H_
#define _MTOOL_H_
using namespace std;

#include <iostream>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TFile.h>
#include <TList.h>
#include <THashList.h>
#include <THn.h>
#include <TLatex.h>
#include <TLine.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TLegend.h>

namespace MTool {
    //project the nD histogram to 1D histogram
    TH1* Project(THn* hn, int dim, std::vector<double> low, std::vector<double> high)
    {
        if (!hn || dim <= 0 || low.size() < dim || high.size() < dim) {
            cout<< "Error: invalid input in MTool::Project" << endl;
            return nullptr;
        }
        THn* ndimhisto = dynamic_cast<THn*>(hn->Clone());
        if (!ndimhisto) {
            cout<< "Error: invalid input in MTool::Project" << endl;
            return nullptr;
        }
        for (int i = 0; i < dim; ++i) {
            ndimhisto->GetAxis(i)->SetRangeUser(low[i], high[i]);
        }
        TH1* h1 = ndimhisto->Projection(0);
        TH1* h1Clone = static_cast<TH1*>(h1->Clone());
        delete ndimhisto;
        delete h1;
        return h1Clone;
    }

    //project the 3D histogram to 1D histogram
    TH1* Project(TH3* h3, double ylow, double yhigh, double zlow, double zhigh)
    {
        int yBinLow = h3->GetYaxis()->FindBin(ylow + 10e-6);
        int yBinHigh = h3->GetYaxis()->FindBin(yhigh - 10e-6);
        int zBinLow = h3->GetZaxis()->FindBin(zlow + 10e-6);
        int zBinHigh = h3->GetZaxis()->FindBin(zhigh - 10e-6);
        // Ensure proper bin order.
        if (yBinLow > yBinHigh) {
            std::swap(yBinLow, yBinHigh);
        }
        if (zBinLow > zBinHigh) {
            std::swap(zBinLow, zBinHigh);
        }
        // Project onto the X-axis within the specified y and z ranges.
        TH1* h1 = h3->ProjectionX("h1", yBinLow, yBinHigh, zBinLow, zBinHigh);
        // Clone the projected histogram for safe memory management.
        TH1* h1Clone = dynamic_cast<TH1*>(h1->Clone("h1Clone"));
        return h1Clone;
    }

    //get a histo from a list
    template <class HISTO>
    HISTO* GetHisto(TList* list, string histoname)
    {
        if (!list) {
            cout <<"Error: list is nullptr!!!!" << endl;
            return nullptr;
        }
        HISTO* histo = dynamic_cast<HISTO*>(list->FindObject(histoname.c_str()));
        if (!histo) {
            return nullptr;
        }
        return histo;
    };

    //get a histo from a list and set name
    template <class HISTO>
    HISTO* GetHisto(TList* list, string histoname, string newname)
    {
        if (!list) {
            cout <<"Error: list is nullptr!!!!" << endl;
            return nullptr;
        }
        HISTO* histo = dynamic_cast<HISTO*>(list->FindObject(histoname.c_str()));
        if (!histo) {
            return nullptr;
        }
        histo->SetName(newname.c_str());
        return histo;
    };

    //get a list from a file
    TList* GetList(TFile* mfile, const string& hashlistname, const string& listname)
    {
        if (!mfile) {
            cout << "Error: file is nullptr!!!!" << endl;
            return nullptr;
        }

        THashList* HashList = dynamic_cast<THashList*>(mfile->Get(hashlistname.c_str()));
        if (!HashList) {
            cout << "Error: HashList is nullptr!!!!" << endl; 
            return nullptr;
        }

        TList* List = dynamic_cast<TList*>(HashList->FindObject(listname.c_str()));
        if (!List) {
            cout << "Error: List is nullptr!!!!" << endl;
            return nullptr;
        }

        return List;
    }

    //______________________________________________________________________________
    TList* GetList(TFile* mfile, const string& listname)
    {
        if (!mfile) {
            cout << "Error: file is nullptr!!!!" << endl;
            return nullptr;
        }

        TList* List = dynamic_cast<TList*>(mfile->Get(listname.c_str()));

        if (!List) {
            cout << "Error: List is nullptr!!!!" << endl;
            return nullptr;
        }
        return List;
    }

    //histogram setting
    void StandardSet(TH1* h1, string xaxisname, string yaxisname){
        h1->GetXaxis()->SetTitleOffset(1.1);
        h1->GetYaxis()->SetTitleOffset(1.1);
        h1->GetXaxis()->SetTitleSize(0.06);
        h1->GetYaxis()->SetTitleSize(0.06);
        h1->GetXaxis()->SetLabelSize(0.06);
        h1->GetXaxis()->SetLabelOffset(0.015);
        h1->GetYaxis()->SetLabelSize(0.06);
        h1->GetYaxis()->SetLabelOffset(0.015);
        h1->GetXaxis()->SetTitle(xaxisname.c_str());
        h1->GetYaxis()->SetTitle(yaxisname.c_str());
        h1->SetLineColor(1);
        h1->SetLineWidth(1);
        h1->SetMarkerStyle(20);
        h1->SetMarkerSize(1);
        h1->SetMarkerColor(1);
        h1->SetLineStyle(1);
        h1->SetStats(0);
        h1->SetTitle("");
    }

    //histogram setting
    void SetMarkerLine(TH1* h, int color, int markerstyle, int markersize, int linestyle, int linewidth){
        h->SetMarkerColor(color);
        h->SetMarkerStyle(markerstyle);
        h->SetMarkerSize(markersize);
        h->SetLineColor(color);
        h->SetLineStyle(linestyle);
        h->SetLineWidth(linewidth);
    }

    //TF1 setting
    void SetTF1(TF1* f, int color, int linestyle, int linewidth, double Npx=1000){
        f->SetLineColor(color);
        f->SetLineStyle(linestyle);
        f->SetLineWidth(linewidth);
        f->SetNpx(4000);
    }

    //calculate the correlation coefficient of a 2D histogram
    double CalculateCorrelationCoefficient(TH2* hist) 
    {
        int nbinsX = hist->GetNbinsX();
        int nbinsY = hist->GetNbinsY();
        double meanX = hist->GetMean(1);
        double meanY = hist->GetMean(2);

        double numerator = 0.0;
        double denominatorX = 0.0;
        double denominatorY = 0.0;

        for (int binX = 1; binX <= nbinsX; ++binX) {
            for (int binY = 1; binY <= nbinsY; ++binY) {
                double value = hist->GetBinContent(binX, binY);
                double x = hist->GetXaxis()->GetBinCenter(binX);
                double y = hist->GetYaxis()->GetBinCenter(binY);

                numerator += (x - meanX) * (y - meanY) * value;
                denominatorX += TMath::Power(x - meanX, 2) * value;
                denominatorY += TMath::Power(y - meanY, 2) * value;
            }
        }

        double correlationCoefficient = numerator / (TMath::Sqrt(denominatorX) * TMath::Sqrt(denominatorY));
        return correlationCoefficient;
    }

    //set latex
    TLatex* SetLatex(double x, double y, char* text, int textFont,double textsize, int textcolor)
    {
        TLatex* tex = new TLatex(x, y, text);
        tex->SetTextFont(textFont);
        tex->SetTextSize(textsize);
        tex->SetTextColor(textcolor);
        tex->SetNDC();
        tex->Draw();
        return tex;
    };

    //set line
    TLine* SetLine(double x1, double y1, double x2, double y2, int linecolor, int linestyle, int linewidth)
    {
        TLine* line = new TLine(x1, y1, x2, y2);
        line->SetLineColor(linecolor);
        line->SetLineStyle(linestyle);
        line->SetLineWidth(linewidth);
        line->Draw();
        return line;
    };

    template <class HistType>
    void SaveHistogramInTList(TFile* file, const char* listName, HistType* hist) {
        // Check if the file is open
        if (!file) {
            std::cerr << "Error: ROOT file is nullptr!" << std::endl;
            return;
        }

        const char* histName = hist->GetName();

        // Get the TList from the ROOT file
        TList* list = dynamic_cast<TList*>(file->Get(listName));

        // Create a new TList if it doesn't exist
        if (!list) {
            list = new TList();
            std::cout << "Created new TList '" << listName << "' in ROOT file." << std::endl;
        }

        // Check if the histogram already exists in the TList
        TObject* obj = list->FindObject(histName);
        if (obj && obj->IsA() == HistType::Class()) {
            // std::cout << "Histogram '" << histName << "' already exists in TList '" << listName << "'. Skipping." << std::endl;
            std::cout << "Histogram '" << histName << "' already exists in TList '" << listName << "'. Updating." << std::endl;
            // return;
            // Remove the existing histogram from the TList
            list->Remove(obj);
        }

        // Add the histogram to the TList
        list->Add(hist);
        file->cd(); // Move to the top directory of the ROOT file
        list->Write(listName, TObject::kSingleKey | TObject::kWriteDelete); // Write the TList to the ROOT file
        std::cout << "Histogram '" << histName << "' added to TList '" << listName << "' in ROOT file." << std::endl;
    }

    std::vector<TString> CollectNamesFromTHashList(THashList* hashList) {
        std::vector<TString> names;
        TIterator* iter = hashList->MakeIterator();

        TObject* obj;
        while ((obj = iter->Next()) != nullptr) {
            TList* list = dynamic_cast<TList*>(obj);
            if (list) {
                names.push_back(list->GetName());
            }
        }

        delete iter;
        return names;
    }

    std::string extractCut(const std::string& name, const std::string& prefix) {
        if (name.find(prefix) == 0) {
            return name.substr(prefix.length());
        }
        return "";
    }
}


#endif