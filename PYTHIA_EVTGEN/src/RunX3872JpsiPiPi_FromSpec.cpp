#include "TreeMaker.h"

int main(int jobID) {
    TreeMaker *maker=new TreeMaker(jobID, "DECAY_X3872_PHSP.DEC", "evt.pdl");
    maker->SetNEvents(100000);
    maker->ProcessX3872ToJpsiPiPiToEEPiPiFromSpec();
    return 0;
}