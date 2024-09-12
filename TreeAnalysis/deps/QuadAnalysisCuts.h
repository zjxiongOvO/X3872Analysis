#ifndef QUADANALYSISCUTS_H
#define QUADANALYSISCUTS_H

#include "QuadVarManager.h"
#include <TCutG.h>

struct CutGInfo{
    TCutG* cutg;
    int xindex;
    int yIndex;
    double Xmin;
    double Xmax;
};

class QuadAnalysisCuts {
public:
    QuadAnalysisCuts();
    ~QuadAnalysisCuts();
    // traditional cuts
    bool NeedSelection[QuadVarManager::Variables::kNall];
    double MinValue[QuadVarManager::Variables::kNall];
    double MaxValue[QuadVarManager::Variables::kNall];
    void SetCut(int index, double min, double max);
    // void SetCutG(TCutG* cutg, int xindex, int yIndex);
    void SetCutG(TCutG* cutg, int xindex, int yIndex, double Xmin, double Xmax);
    bool isInCut(const double* value);

    // defined specific cuts

private:
    std::vector<CutGInfo> CutGList;
};

// 

QuadAnalysisCuts::QuadAnalysisCuts() {
    for (int i = 0; i < QuadVarManager::Variables::kNall; i++) {
        NeedSelection[i] = false;
        MinValue[i] = -999;
        MaxValue[i] = -999;
    }
    CutGList.clear();
}

QuadAnalysisCuts::~QuadAnalysisCuts() {}

void QuadAnalysisCuts::SetCut(int index, double min, double max) {
    NeedSelection[index] = true;
    MinValue[index] = min;
    MaxValue[index] = max;
}

// void QuadAnalysisCuts::SetCutG(TCutG* cutg, int xindex, int yIndex) {
//     CutGInfo cutginfo;
//     cutginfo.cutg = cutg;
//     cutginfo.xindex = xindex;
//     cutginfo.yIndex = yIndex;
//     CutGList.push_back(cutginfo);
// }

void QuadAnalysisCuts::SetCutG(TCutG* cutg, int xindex, int yIndex, double Xmin, double Xmax) {
    CutGInfo cutginfo;
    cutginfo.cutg = cutg;
    cutginfo.xindex = xindex;
    cutginfo.yIndex = yIndex;
    cutginfo.Xmin = Xmin;
    cutginfo.Xmax = Xmax;
    CutGList.push_back(cutginfo);
}

bool QuadAnalysisCuts::isInCut(const double* value) {
    // apply traditional cuts
    for (int i = 0; i < QuadVarManager::Variables::kNall; i++) {
        if (NeedSelection[i] && (value[i] < MinValue[i] || value[i] > MaxValue[i])) return false;
    }
    // apply cutg
    for (int i = 0; i < CutGList.size(); i++) {
        if ((value[CutGList[i].xindex] < CutGList[i].Xmin || value[CutGList[i].xindex] > CutGList[i].Xmax)) return true;
        if (!CutGList[i].cutg->IsInside(value[CutGList[i].xindex], value[CutGList[i].yIndex])) return false;
    }
    return true;
}

#endif