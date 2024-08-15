#ifndef TREEMAKER_H
#define TREEMAKER_H

//pythia
#include "Pythia8/Pythia.h"

//evtgen
#include "EvtGen/EvtGen.hh"
#include "EvtGenBase/EvtParticle.hh"

//root
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

//local
#include "VarManager.h"

using namespace std;

class TreeMaker {
public:
    TreeMaker(int jobID, string DecayFile, string PDLFile);
    ~TreeMaker();
    void SetNEvents(int nEvents) { NEvents = nEvents; }
    // process
    void ProcessX3872ToJpsiRhoToEEPiPi();
    void ProcessX3872ToJpsiRhoToEEPiPiFromSpec();
    void ProcessX3872ToJpsiPiPiToEEPiPiFromSpec();
    void ProcessPsi2SToJpsiPiPiToEEPiPiFromSpec();

private:
    Pythia8::Pythia pythia;
    EvtGen* myGenerator;
    TTree* tree;
    int NEvents;
};

#endif