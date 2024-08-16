#ifndef QUADANALYSISCUTS_H
#define QUADANALYSISCUTS_H

#include "QuadVarManager.h"

class QuadAnalysisCuts {
public:
    QuadAnalysisCuts();
    ~QuadAnalysisCuts();
    // traditional cuts
    bool NeedSelection[QuadVarManager::Variables::kNall];
    double MinValue[QuadVarManager::Variables::kNall];
    double MaxValue[QuadVarManager::Variables::kNall];
    void SetCut(int index, double min, double max);
    bool isInCut(const double* value);

    // defined specific cuts

private:
    TF1* f_cutlow;
    TF1* f_cuthigh;
};

// 

QuadAnalysisCuts::QuadAnalysisCuts() {
    for (int i = 0; i < QuadVarManager::Variables::kNall; i++) {
        NeedSelection[i] = false;
        MinValue[i] = -999;
        MaxValue[i] = -999;
    }
}

QuadAnalysisCuts::~QuadAnalysisCuts() {}

void QuadAnalysisCuts::SetCut(int index, double min, double max) {
    NeedSelection[index] = true;
    MinValue[index] = min;
    MaxValue[index] = max;
}

bool QuadAnalysisCuts::isInCut(const double* value) {
    for (int i = 0; i < QuadVarManager::Variables::kNall; i++) {
        if (NeedSelection[i] && (value[i] < MinValue[i] || value[i] > MaxValue[i])) return false;
    }
    return true;
}

#endif