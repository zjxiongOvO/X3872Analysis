#include "TreeMaker.h"

int main(int jobID) {
    TreeMaker *maker=new TreeMaker(jobID, "DECAY_X3872_rhoJpsi.DEC", "evt.pdl");
    maker->SetNEvents(10000);
    maker->ProcessX3872ToJpsiRhoToEEPiPi();
    return 0;
}