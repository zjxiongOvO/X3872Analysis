#include "TreeMaker.h"

int main(int jobID) {
    TreeMaker *maker=new TreeMaker(jobID, "DECAY_Psi2S.DEC", "evt.pdl");
    maker->SetNEvents(100000);
    maker->ProcessPsi2SToJpsiPiPiToEEPiPiFromSpec();
    return 0;
}