#include "TreeMaker.h"

//evtgen
#include "EvtGen/EvtGen.hh"

#include "EvtGenBase/EvtAbsRadCorr.hh"
#include "EvtGenBase/EvtComplex.hh"
#include "EvtGenBase/EvtConst.hh"
#include "EvtGenBase/EvtDecayBase.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenBase/EvtDiracSpinor.hh"
#include "EvtGenBase/EvtGammaMatrix.hh"
#include "EvtGenBase/EvtIdSet.hh"
#include "EvtGenBase/EvtKine.hh"
#include "EvtGenBase/EvtMTRandomEngine.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtParser.hh"
#include "EvtGenBase/EvtParticle.hh"
//#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtParticleFactory.hh"
#include "EvtGenBase/EvtRadCorr.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtRandomEngine.hh"
#include "EvtGenBase/EvtReport.hh"
//#include "EvtGenBase/EvtGenEnums.h"
#include "EvtGenBase/EvtSecondary.hh"
#include "EvtGenBase/EvtSimpleRandomEngine.hh"
#include "EvtGenBase/EvtStdHep.hh"
#include "EvtGenBase/EvtTensor4C.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtVectorParticle.hh"
#include "EvtGenBase/EvtSpinDensity.hh"
#include "EvtGenBase/EvtAmp.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtPatches.hh"


#ifdef EVTGEN_EXTERNAL
#include "EvtGenExternal/EvtExternalGenList.hh"
#endif

//root
#include "TApplication.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TROOT.h"
#include "TTree.h"
#include "TString.h"

//other
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//______________________________________________________________________________
TreeMaker::TreeMaker(int jobID, string DecayFile, string PDLFile) {
    // Initialize Pythia
    //set pp collision at 13.6 TeV
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 13600.");
    pythia.readString("HardQCD:all = on");
    pythia.readString("PhaseSpace:pTHatMin = 20.");
    pythia.readString("PhaseSpace:bias2Selection = on");
    pythia.readString("PhaseSpace:bias2SelectionPow = 4.");
    pythia.readString("PhaseSpace:bias2SelectionRef = 1.");

    pythia.init();

    //   Initialize evtgen
    EvtGenReport(EVTGEN_INFO, "EvtGen") << "Starting EvtGen" << endl;
    // Define the random number generator
    time_t t=time(nullptr);
    int seed = static_cast<int>(t);
    EvtRandomEngine* eng =  new EvtMTRandomEngine(seed+jobID);
    EvtRandom::setRandomEngine( eng );
    EvtAbsRadCorr* radCorrEngine = 0;
    std::list<EvtDecayBase*> extraModels;
    // Define the decay table
    myGenerator=new EvtGen(Form("../Basefile/%s",DecayFile.c_str()), Form("../Basefile/%s",PDLFile.c_str()), eng, radCorrEngine,&extraModels);
    // Initialize the tree
    tree = nullptr;
};

//______________________________________________________________________________
TreeMaker::~TreeMaker() {
    delete myGenerator;
};

//______________________________________________________________________________
//find chi_c1 in pythia, then use it momentum as X3872's momentum, then decay it by using Evtgen.
void TreeMaker::ProcessX3872ToJpsiRhoToEEPiPi() {
    // init
    EvtParticle *X3872;
    EvtParticle *Jpsi;
    EvtParticle *ePlus;
    EvtParticle *eMinus;
    EvtParticle *rho;
    EvtParticle *piPlus;
    EvtParticle *piMinus;
    double* variable = new double[VarManager::kNVariables];
    tree = new TTree("tree", "tree");
    //define the structure of these trees
    tree->Branch("X3872Pt", &variable[VarManager::kX3872Pt], "X3872Pt/D");
    tree->Branch("X3872Eta", &variable[VarManager::kX3872Eta], "X3872Eta/D");
    tree->Branch("X3872Phi", &variable[VarManager::kX3872Phi], "X3872Phi/D");
    tree->Branch("X3872Mass", &variable[VarManager::kX3872Mass], "X3872Mass/D");
    tree->Branch("DeltaR1", &variable[VarManager::kDeltaR1], "DeltaR1/D");
    tree->Branch("DeltaR2", &variable[VarManager::kDeltaR2], "DeltaR2/D");
    tree->Branch("Q", &variable[VarManager::kQ], "Q/D");
    tree->Branch("CosMotherAndDaughter_Lab", &variable[VarManager::kCosMotherAndDaughter_Lab], "CosMotherAndDaughter_Lab/D");
    tree->Branch("CosMotherAndDaughter_CMS", &variable[VarManager::kCosMotherAndDaughter_CMS], "CosMotherAndDaughter_CMS/D");
    tree->Branch("JpsiPt", &variable[VarManager::kJpsiPt], "JpsiPt/D");
    tree->Branch("JpsiEta", &variable[VarManager::kJpsiEta], "JpsiEta/D");
    tree->Branch("JpsiPhi", &variable[VarManager::kJpsiPhi], "JpsiPhi/D");
    tree->Branch("JpsiMass", &variable[VarManager::kJpsiMass], "JpsiMass/D");
    tree->Branch("PiPlusPt", &variable[VarManager::kPiPlusPt], "PiPlusPt/D");
    tree->Branch("PiPlusEta", &variable[VarManager::kPiPlusEta], "PiPlusEta/D");
    tree->Branch("PiPlusPhi", &variable[VarManager::kPiPlusPhi], "PiPlusPhi/D");
    tree->Branch("PiPlusMass", &variable[VarManager::kPiPlusMass], "PiPlusMass/D");
    tree->Branch("PiMinusPt", &variable[VarManager::kPiMinusPt], "PiMinusPt/D");
    tree->Branch("PiMinusEta", &variable[VarManager::kPiMinusEta], "PiMinusEta/D");
    tree->Branch("PiMinusPhi", &variable[VarManager::kPiMinusPhi], "PiMinusPhi/D");
    tree->Branch("PiMinusMass", &variable[VarManager::kPiMinusMass], "PiMinusMass/D");
    tree->Branch("EPlusPt", &variable[VarManager::kEPlusPt], "EPlusPt/D");
    tree->Branch("EPlusEta", &variable[VarManager::kEPlusEta], "EPlusEta/D");
    tree->Branch("EPlusPhi", &variable[VarManager::kEPlusPhi], "EPlusPhi/D");
    tree->Branch("EPlusMass", &variable[VarManager::kEPlusMass], "EPlusMass/D");
    tree->Branch("EMinusPt", &variable[VarManager::kEMinusPt], "EMinusPt/D");
    tree->Branch("EMinusEta", &variable[VarManager::kEMinusEta], "EMinusEta/D");
    tree->Branch("EMinusPhi", &variable[VarManager::kEMinusPhi], "EMinusPhi/D");
    tree->Branch("EMinusMass", &variable[VarManager::kEMinusMass], "EMinusMass/D");
     //loop over pythia events
    for (int iEvent = 0; iEvent < NEvents; ++iEvent) {
        if (!pythia.next()) continue;
        // Loop over particles in event
        for (int i = 0; i < pythia.event.size(); ++i) {
            Pythia8::Particle& particle = pythia.event[i];
            // find chi_c1, and get it momentum
            if (particle.id() == 20443) {

				double energy = sqrt(3.8722*3.8722 + particle.px()*particle.px() + particle.py()*particle.py() + particle.pz()*particle.pz());
                EvtVector4R pInit(energy, particle.px(), particle.py(), particle.pz());
                //use particle id to define the parent particle
                X3872 = EvtParticleFactory::particleFactory(EvtPDL::getId("X_1(3872)"), pInit);
                myGenerator->generateDecay(X3872);
                //get the final state particles
                Jpsi = X3872->getDaug(0);
                rho = X3872->getDaug(1);
                ePlus = Jpsi->getDaug(0);
                eMinus = Jpsi->getDaug(1);
                piPlus = rho->getDaug(0);
                piMinus = rho->getDaug(1);
                //fill the tree
                VarManager::FillX3872(variable, X3872, Jpsi, ePlus, eMinus, rho, piPlus, piMinus);
                tree->Fill();
            }
        }//loop over particles
    }//loop over events
    TFile* file = new TFile("X3872ToJpsiRhoToEEPiPi.root", "RECREATE");
    tree->Write();
    file->Close();
}

//______________________________________________________________________________
//use the momentum of X(3872) from the a given spectrum, then decay it by using Evtgen.
void TreeMaker::ProcessX3872ToJpsiRhoToEEPiPiFromSpec() {
    // init
    EvtParticle *X3872;
    EvtParticle *Jpsi;
    EvtParticle *ePlus;
    EvtParticle *eMinus;
    EvtParticle *rho;
    EvtParticle *piPlus;
    EvtParticle *piMinus;
    double* variable = new double[VarManager::kNVariables];
    tree = new TTree("tree", "tree");
    //define the structure of these trees
    tree->Branch("X3872Pt", &variable[VarManager::kX3872Pt], "X3872Pt/D");
    tree->Branch("X3872Eta", &variable[VarManager::kX3872Eta], "X3872Eta/D");
    tree->Branch("X3872Phi", &variable[VarManager::kX3872Phi], "X3872Phi/D");
    tree->Branch("X3872Mass", &variable[VarManager::kX3872Mass], "X3872Mass/D");
    tree->Branch("DeltaR1", &variable[VarManager::kDeltaR1], "DeltaR1/D");
    tree->Branch("DeltaR2", &variable[VarManager::kDeltaR2], "DeltaR2/D");
    tree->Branch("Q", &variable[VarManager::kQ], "Q/D");
    tree->Branch("CosMotherAndDaughter_Lab", &variable[VarManager::kCosMotherAndDaughter_Lab], "CosMotherAndDaughter_Lab/D");
    tree->Branch("CosMotherAndDaughter_CMS", &variable[VarManager::kCosMotherAndDaughter_CMS], "CosMotherAndDaughter_CMS/D");
    tree->Branch("JpsiPt", &variable[VarManager::kJpsiPt], "JpsiPt/D");
    tree->Branch("JpsiEta", &variable[VarManager::kJpsiEta], "JpsiEta/D");
    tree->Branch("JpsiPhi", &variable[VarManager::kJpsiPhi], "JpsiPhi/D");
    tree->Branch("JpsiMass", &variable[VarManager::kJpsiMass], "JpsiMass/D");
    tree->Branch("PiPlusPt", &variable[VarManager::kPiPlusPt], "PiPlusPt/D");
    tree->Branch("PiPlusEta", &variable[VarManager::kPiPlusEta], "PiPlusEta/D");
    tree->Branch("PiPlusPhi", &variable[VarManager::kPiPlusPhi], "PiPlusPhi/D");
    tree->Branch("PiPlusMass", &variable[VarManager::kPiPlusMass], "PiPlusMass/D");
    tree->Branch("PiMinusPt", &variable[VarManager::kPiMinusPt], "PiMinusPt/D");
    tree->Branch("PiMinusEta", &variable[VarManager::kPiMinusEta], "PiMinusEta/D");
    tree->Branch("PiMinusPhi", &variable[VarManager::kPiMinusPhi], "PiMinusPhi/D");
    tree->Branch("PiMinusMass", &variable[VarManager::kPiMinusMass], "PiMinusMass/D");
    tree->Branch("EPlusPt", &variable[VarManager::kEPlusPt], "EPlusPt/D");
    tree->Branch("EPlusEta", &variable[VarManager::kEPlusEta], "EPlusEta/D");
    tree->Branch("EPlusPhi", &variable[VarManager::kEPlusPhi], "EPlusPhi/D");
    tree->Branch("EPlusMass", &variable[VarManager::kEPlusMass], "EPlusMass/D");
    tree->Branch("EMinusPt", &variable[VarManager::kEMinusPt], "EMinusPt/D");
    tree->Branch("EMinusEta", &variable[VarManager::kEMinusEta], "EMinusEta/D");
    tree->Branch("EMinusPhi", &variable[VarManager::kEMinusPhi], "EMinusPhi/D");
    tree->Branch("EMinusMass", &variable[VarManager::kEMinusMass], "EMinusMass/D");

    double pTmin = 0.0;
    double pTmax = 20.0;
    double etamin = -0.9;
    double etamax = 0.9;
    double phimin = -1.0 * TMath::Pi();
    double phimax = TMath::Pi();
    // loop over events
    for (int iEvent = 0; iEvent < NEvents; ++iEvent) {
        // get pT, eta, phi, mass from flat spectrum
        // using evtgen random number generator
        double pT = EvtRandom::Flat(pTmin, pTmax);
        double eta = EvtRandom::Flat(etamin, etamax);
        double phi = EvtRandom::Flat(phimin, phimax);
        double mass = 3.8722000e+00;
        // get momentum
        double px = pT * cos(phi);
        double py = pT * sin(phi);
        double pz = pT * sinh(eta);
        double energy = sqrt(mass*mass + px*px + py*py + pz*pz);
        EvtVector4R pInit(energy, px, py, pz);
        //use particle id to define the parent particle
        X3872 = EvtParticleFactory::particleFactory(EvtPDL::getId("X_1(3872)"), pInit);
        myGenerator->generateDecay(X3872);
        //get the final state particles
        Jpsi = X3872->getDaug(0);
        rho = X3872->getDaug(1);
        ePlus = Jpsi->getDaug(0);
        eMinus = Jpsi->getDaug(1);
        piPlus = rho->getDaug(0);
        piMinus = rho->getDaug(1);
        //fill the tree
        VarManager::FillX3872(variable, X3872, Jpsi, ePlus, eMinus, rho, piPlus, piMinus);
        tree->Fill();
    }//loop over event
    TFile* file = new TFile("X3872ToJpsiRhoToEEPiPiFromSpec.root", "RECREATE");
    tree->Write();
    file->Close();
}

//______________________________________________________________________________
//use the momentum of X(3872) from the a given spectrum, then decay it by using Evtgen.
void TreeMaker::ProcessX3872ToJpsiPiPiToEEPiPiFromSpec() {
    // init
    EvtParticle *X3872;
    EvtParticle *Jpsi;
    EvtParticle *ePlus;
    EvtParticle *eMinus;
    EvtParticle *rho;
    EvtParticle *piPlus;
    EvtParticle *piMinus;
    double* variable = new double[VarManager::kNVariables];
    tree = new TTree("tree", "tree");
    //define the structure of these trees
    tree->Branch("X3872Pt", &variable[VarManager::kX3872Pt], "X3872Pt/D");
    tree->Branch("X3872Eta", &variable[VarManager::kX3872Eta], "X3872Eta/D");
    tree->Branch("X3872Phi", &variable[VarManager::kX3872Phi], "X3872Phi/D");
    tree->Branch("X3872Mass", &variable[VarManager::kX3872Mass], "X3872Mass/D");
    tree->Branch("DeltaR1", &variable[VarManager::kDeltaR1], "DeltaR1/D");
    tree->Branch("DeltaR2", &variable[VarManager::kDeltaR2], "DeltaR2/D");
    tree->Branch("Q", &variable[VarManager::kQ], "Q/D");
    tree->Branch("CosMotherAndDaughter_Lab", &variable[VarManager::kCosMotherAndDaughter_Lab], "CosMotherAndDaughter_Lab/D");
    tree->Branch("CosMotherAndDaughter_CMS", &variable[VarManager::kCosMotherAndDaughter_CMS], "CosMotherAndDaughter_CMS/D");
    tree->Branch("JpsiPt", &variable[VarManager::kJpsiPt], "JpsiPt/D");
    tree->Branch("JpsiEta", &variable[VarManager::kJpsiEta], "JpsiEta/D");
    tree->Branch("JpsiPhi", &variable[VarManager::kJpsiPhi], "JpsiPhi/D");
    tree->Branch("JpsiMass", &variable[VarManager::kJpsiMass], "JpsiMass/D");
    tree->Branch("PiPlusPt", &variable[VarManager::kPiPlusPt], "PiPlusPt/D");
    tree->Branch("PiPlusEta", &variable[VarManager::kPiPlusEta], "PiPlusEta/D");
    tree->Branch("PiPlusPhi", &variable[VarManager::kPiPlusPhi], "PiPlusPhi/D");
    tree->Branch("PiPlusMass", &variable[VarManager::kPiPlusMass], "PiPlusMass/D");
    tree->Branch("PiMinusPt", &variable[VarManager::kPiMinusPt], "PiMinusPt/D");
    tree->Branch("PiMinusEta", &variable[VarManager::kPiMinusEta], "PiMinusEta/D");
    tree->Branch("PiMinusPhi", &variable[VarManager::kPiMinusPhi], "PiMinusPhi/D");
    tree->Branch("PiMinusMass", &variable[VarManager::kPiMinusMass], "PiMinusMass/D");
    tree->Branch("EPlusPt", &variable[VarManager::kEPlusPt], "EPlusPt/D");
    tree->Branch("EPlusEta", &variable[VarManager::kEPlusEta], "EPlusEta/D");
    tree->Branch("EPlusPhi", &variable[VarManager::kEPlusPhi], "EPlusPhi/D");
    tree->Branch("EPlusMass", &variable[VarManager::kEPlusMass], "EPlusMass/D");
    tree->Branch("EMinusPt", &variable[VarManager::kEMinusPt], "EMinusPt/D");
    tree->Branch("EMinusEta", &variable[VarManager::kEMinusEta], "EMinusEta/D");
    tree->Branch("EMinusPhi", &variable[VarManager::kEMinusPhi], "EMinusPhi/D");
    tree->Branch("EMinusMass", &variable[VarManager::kEMinusMass], "EMinusMass/D");

    double pTmin = 0.0;
    double pTmax = 20.0;
    double etamin = -0.9;
    double etamax = 0.9;
    double phimin = -1.0 * TMath::Pi();
    double phimax = TMath::Pi();
    rho = EvtParticleFactory::particleFactory(EvtPDL::getId("rho0"), EvtVector4R(0.77549, 0.0, 0.0, 0.0));
    // loop over events
    for (int iEvent = 0; iEvent < NEvents; ++iEvent) {
        // get pT, eta, phi, mass from flat spectrum
        // using evtgen random number generator
        double pT = EvtRandom::Flat(pTmin, pTmax);
        double eta = EvtRandom::Flat(etamin, etamax);
        double phi = EvtRandom::Flat(phimin, phimax);
        double mass = 3.8722000e+00;
        // get momentum
        double px = pT * cos(phi);
        double py = pT * sin(phi);
        double pz = pT * sinh(eta);
        double energy = sqrt(mass*mass + px*px + py*py + pz*pz);
        EvtVector4R pInit(energy, px, py, pz);
        //use particle id to define the parent particle
        X3872 = EvtParticleFactory::particleFactory(EvtPDL::getId("X_1(3872)"), pInit);
        myGenerator->generateDecay(X3872);
        //get the final state particles
        Jpsi = X3872->getDaug(0);
        piPlus = X3872->getDaug(1);
        piMinus = X3872->getDaug(2); 
        ePlus = Jpsi->getDaug(0);
        eMinus = Jpsi->getDaug(1);
        //fill the tree
        VarManager::FillX3872(variable, X3872, Jpsi, ePlus, eMinus, rho, piPlus, piMinus);
        tree->Fill();
    }//loop over event
    TFile* file = new TFile("X3872ToJpsiPiPiToEEPiPiFromSpec.root", "RECREATE");
    tree->Write();
    file->Close();
}

//______________________________________________________________________________
//use the momentum of psi(2S) from the a given spectrum, then decay it by using Evtgen.
void TreeMaker::ProcessPsi2SToJpsiPiPiToEEPiPiFromSpec() {
    // init
    EvtParticle *psi2S;
    EvtParticle *Jpsi;
    EvtParticle *ePlus;
    EvtParticle *eMinus;
    EvtParticle *rho;
    EvtParticle *piPlus;
    EvtParticle *piMinus;
    double* variable = new double[VarManager::kNVariables];
    tree = new TTree("tree", "tree");
    //define the structure of these trees
    tree->Branch("X3872Pt", &variable[VarManager::kX3872Pt], "X3872Pt/D");
    tree->Branch("X3872Eta", &variable[VarManager::kX3872Eta], "X3872Eta/D");
    tree->Branch("X3872Phi", &variable[VarManager::kX3872Phi], "X3872Phi/D");
    tree->Branch("X3872Mass", &variable[VarManager::kX3872Mass], "X3872Mass/D");
    tree->Branch("DeltaR1", &variable[VarManager::kDeltaR1], "DeltaR1/D");
    tree->Branch("DeltaR2", &variable[VarManager::kDeltaR2], "DeltaR2/D");
    tree->Branch("Q", &variable[VarManager::kQ], "Q/D");
    tree->Branch("CosMotherAndDaughter_Lab", &variable[VarManager::kCosMotherAndDaughter_Lab], "CosMotherAndDaughter_Lab/D");
    tree->Branch("CosMotherAndDaughter_CMS", &variable[VarManager::kCosMotherAndDaughter_CMS], "CosMotherAndDaughter_CMS/D");
    tree->Branch("JpsiPt", &variable[VarManager::kJpsiPt], "JpsiPt/D");
    tree->Branch("JpsiEta", &variable[VarManager::kJpsiEta], "JpsiEta/D");
    tree->Branch("JpsiPhi", &variable[VarManager::kJpsiPhi], "JpsiPhi/D");
    tree->Branch("JpsiMass", &variable[VarManager::kJpsiMass], "JpsiMass/D");
    tree->Branch("PiPlusPt", &variable[VarManager::kPiPlusPt], "PiPlusPt/D");
    tree->Branch("PiPlusEta", &variable[VarManager::kPiPlusEta], "PiPlusEta/D");
    tree->Branch("PiPlusPhi", &variable[VarManager::kPiPlusPhi], "PiPlusPhi/D");
    tree->Branch("PiPlusMass", &variable[VarManager::kPiPlusMass], "PiPlusMass/D");
    tree->Branch("PiMinusPt", &variable[VarManager::kPiMinusPt], "PiMinusPt/D");
    tree->Branch("PiMinusEta", &variable[VarManager::kPiMinusEta], "PiMinusEta/D");
    tree->Branch("PiMinusPhi", &variable[VarManager::kPiMinusPhi], "PiMinusPhi/D");
    tree->Branch("PiMinusMass", &variable[VarManager::kPiMinusMass], "PiMinusMass/D");
    tree->Branch("EPlusPt", &variable[VarManager::kEPlusPt], "EPlusPt/D");
    tree->Branch("EPlusEta", &variable[VarManager::kEPlusEta], "EPlusEta/D");
    tree->Branch("EPlusPhi", &variable[VarManager::kEPlusPhi], "EPlusPhi/D");
    tree->Branch("EPlusMass", &variable[VarManager::kEPlusMass], "EPlusMass/D");
    tree->Branch("EMinusPt", &variable[VarManager::kEMinusPt], "EMinusPt/D");
    tree->Branch("EMinusEta", &variable[VarManager::kEMinusEta], "EMinusEta/D");
    tree->Branch("EMinusPhi", &variable[VarManager::kEMinusPhi], "EMinusPhi/D");
    tree->Branch("EMinusMass", &variable[VarManager::kEMinusMass], "EMinusMass/D");

    double pTmin = 0.0;
    double pTmax = 20.0;
    double etamin = -0.9;
    double etamax = 0.9;
    double phimin = -1.0 * TMath::Pi();
    double phimax = TMath::Pi();
    rho = EvtParticleFactory::particleFactory(EvtPDL::getId("rho0"), EvtVector4R(0.77549, 0.0, 0.0, 0.0));
    // loop over events
    for (int iEvent = 0; iEvent < NEvents; ++iEvent) {
        // get pT, eta, phi, mass from flat spectrum
        // using evtgen random number generator
        double pT = EvtRandom::Flat(pTmin, pTmax);
        double eta = EvtRandom::Flat(etamin, etamax);
        double phi = EvtRandom::Flat(phimin, phimax);
        double mass = 3.6860970;
        // get momentum
        double px = pT * cos(phi);
        double py = pT * sin(phi);
        double pz = pT * sinh(eta);
        double energy = sqrt(mass*mass + px*px + py*py + pz*pz);
        EvtVector4R pInit(energy, px, py, pz);
        //use particle id to define the parent particle
        psi2S = EvtParticleFactory::particleFactory(EvtPDL::getId("psi(2S)"), pInit);
        myGenerator->generateDecay(psi2S);
        //get the final state particles
        Jpsi = psi2S->getDaug(0);
        piPlus = psi2S->getDaug(1);
        piMinus = psi2S->getDaug(2); 
        ePlus = Jpsi->getDaug(0);
        eMinus = Jpsi->getDaug(1);
        //fill the tree
        VarManager::FillX3872(variable, psi2S, Jpsi, ePlus, eMinus, rho, piPlus, piMinus);
        tree->Fill();
    }//loop over event
    TFile* file = new TFile("Psi2SToJpsiPiPiToEEPiPiFromSpec.root", "RECREATE");
    tree->Write();
    file->Close();
}