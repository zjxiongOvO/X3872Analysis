#ifndef VARMANAGER_H
#define VARMANAGER_H

//evtgen
#include "EvtGen/EvtGen.hh"
#include "EvtGenBase/EvtParticle.hh"

//root
#include "TLorentzVector.h"

enum ProngType {
    kJpsiToEE = 0, // J/psi -> e+ e-
    KX3872ToJpsiRhoToEEPiPi, // X(3872) -> J/psi rho -> e+ e- pi+ pi-
    KX3872ToJpsiPiPiToEEPiPi, // X(3872) -> J/psi pi+ pi- -> e+ e- pi+ pi-
    kBToDstarPlusToD0PiToKPiPi, // B -> D*+ -> D0 pi+ -> K- pi+ pi+
    kPsi2SToJpsiPiPiToEEPiPi, // psi(2S) -> J/psi pi+ pi- -> e+ e- pi+ pi-
    kNProngTypes
};

class VarManager {
public:
    enum Variables {
        kNothing = -1,
        // J/psi
        kJpsiPt,
        kJpsiEta,
        kJpsiPhi,
        kJpsiMass,
        // X(3872)
        kX3872Pt,
        kX3872Eta,
        kX3872Phi,
        kX3872Mass,
        kDeltaR1,
        kDeltaR2,
        kQ,
        kCosMotherAndDaughter_Lab,
        kCosMotherAndDaughter_CMS,
        // rho
        kRhoPt,
        kRhoEta,
        kRhoPhi,
        kRhoMass,
        // D*+
        kDstarPlusPt,
        kDstarPlusEta,
        kDstarPlusPhi,
        kDstarPlusMass,
        // D0
        kD0Pt,
        kD0Eta,
        kD0Phi,
        kD0Mass,
        // pi+
        kPiPlusPt,
        kPiPlusEta,
        kPiPlusPhi,
        kPiPlusMass,
        // pi-
        kPiMinusPt,
        kPiMinusEta,
        kPiMinusPhi,
        kPiMinusMass,
        // e+
        kEPlusPt,
        kEPlusEta,
        kEPlusPhi,
        kEPlusMass,
        // e-
        kEMinusPt,
        kEMinusEta,
        kEMinusPhi,
        kEMinusMass,
        kNVariables
    };

    static void FillX3872(double* variable, EvtParticle* X3872, EvtParticle* Jpsi, EvtParticle* ePlus, EvtParticle* eMinus, EvtParticle* rho, EvtParticle* piPlus, EvtParticle* piMinus) {
        EvtVector4R v_X3872 = X3872->getP4Lab();
        EvtVector4R v_Jpsi = Jpsi->getP4Lab();
        EvtVector4R v_ePlus = ePlus->getP4Lab();
        EvtVector4R v_eMinus = eMinus->getP4Lab();
        EvtVector4R v_rho = rho->getP4Lab();
        EvtVector4R v_piPlus = piPlus->getP4Lab();
        EvtVector4R v_piMinus = piMinus->getP4Lab();

        TLorentzVector X3872Vec(v_X3872.get(1), v_X3872.get(2), v_X3872.get(3), v_X3872.get(0));
        TLorentzVector JpsiVec(v_Jpsi.get(1), v_Jpsi.get(2), v_Jpsi.get(3), v_Jpsi.get(0));
        TLorentzVector ePlusVec(v_ePlus.get(1), v_ePlus.get(2), v_ePlus.get(3), v_ePlus.get(0));
        TLorentzVector eMinusVec(v_eMinus.get(1), v_eMinus.get(2), v_eMinus.get(3), v_eMinus.get(0));
        TLorentzVector rhoVec(v_rho.get(1), v_rho.get(2), v_rho.get(3), v_rho.get(0));
        TLorentzVector piPlusVec(v_piPlus.get(1), v_piPlus.get(2), v_piPlus.get(3), v_piPlus.get(0));
        TLorentzVector piMinusVec(v_piMinus.get(1), v_piMinus.get(2), v_piMinus.get(3), v_piMinus.get(0));

        variable[kX3872Pt] = X3872Vec.Pt();
        variable[kX3872Eta] = X3872Vec.Eta();
        variable[kX3872Phi] = X3872Vec.Phi();
        variable[kX3872Mass] = X3872Vec.M();
        variable[kQ] = X3872Vec.M() - (piPlusVec + piMinusVec).M() - 3.096916;

        double deltaPhi1 = JpsiVec.Phi() - piPlusVec.Phi();
        double deltaPhi2 = JpsiVec.Phi() - piMinusVec.Phi();
        double deltaPhi1_1pi = deltaPhi1 > TMath::Pi() ? deltaPhi1 - 2 * TMath::Pi() : deltaPhi1;
        deltaPhi1_1pi = deltaPhi1_1pi < -TMath::Pi() ? deltaPhi1_1pi + 2 * TMath::Pi() : deltaPhi1_1pi;
        double deltaPhi2_1pi = deltaPhi2 > TMath::Pi() ? deltaPhi2 - 2 * TMath::Pi() : deltaPhi2;
        deltaPhi2_1pi = deltaPhi2_1pi < -TMath::Pi() ? deltaPhi2_1pi + 2 * TMath::Pi() : deltaPhi2_1pi;
        // Variable[kDeltaR1] = sqrt((X3872Vec.Eta() - piPlusVec.Eta()) * (X3872Vec.Eta() - piPlusVec.Eta()) + deltaPhi1_1pi * deltaPhi1_1pi);
        // Variable[kDeltaR2] = sqrt((X3872Vec.Eta() - piMinusVec.Eta()) * (X3872Vec.Eta() - piMinusVec.Eta()) + deltaPhi2_1pi * deltaPhi2_1pi);
        variable[kDeltaR1] = sqrt((JpsiVec.Eta() - piPlusVec.Eta()) * (JpsiVec.Eta() - piPlusVec.Eta()) + deltaPhi1_1pi * deltaPhi1_1pi);
        variable[kDeltaR2] = sqrt((JpsiVec.Eta() - piMinusVec.Eta()) * (JpsiVec.Eta() - piMinusVec.Eta()) + deltaPhi2_1pi * deltaPhi2_1pi);
        variable[kCosMotherAndDaughter_Lab] = (X3872Vec.Vect().Dot(JpsiVec.Vect())) / (X3872Vec.Vect().Mag() * JpsiVec.Vect().Mag());
        // boost to the rest frame of the mother particle
        TLorentzVector JpsiVecRest = JpsiVec;
        JpsiVecRest.Boost(-X3872Vec.BoostVector());
        variable[kCosMotherAndDaughter_CMS] = JpsiVecRest.Vect().Dot(X3872Vec.Vect()) / (JpsiVecRest.Vect().Mag() * X3872Vec.Vect().Mag());

        variable[kJpsiPt] = JpsiVec.Pt();
        variable[kJpsiEta] = JpsiVec.Eta();
        variable[kJpsiPhi] = JpsiVec.Phi();
        variable[kJpsiMass] = JpsiVec.M();
        variable[kEPlusPt] = ePlusVec.Pt();
        variable[kEPlusEta] = ePlusVec.Eta();
        variable[kEPlusPhi] = ePlusVec.Phi();
        variable[kEPlusMass] = ePlusVec.M();
        variable[kEMinusPt] = eMinusVec.Pt();
        variable[kEMinusEta] = eMinusVec.Eta();
        variable[kEMinusPhi] = eMinusVec.Phi();
        variable[kEMinusMass] = eMinusVec.M();
        variable[kPiPlusPt] = piPlusVec.Pt();
        variable[kPiPlusEta] = piPlusVec.Eta();
        variable[kPiPlusPhi] = piPlusVec.Phi();
        variable[kPiPlusMass] = piPlusVec.M();
        variable[kPiMinusPt] = piMinusVec.Pt();
        variable[kPiMinusEta] = piMinusVec.Eta();
        variable[kPiMinusPhi] = piMinusVec.Phi();
        variable[kPiMinusMass] = piMinusVec.M();
        variable[kRhoPt] = rhoVec.Pt();
        variable[kRhoEta] = rhoVec.Eta();
        variable[kRhoPhi] = rhoVec.Phi();
        variable[kRhoMass] = rhoVec.M();
    }

    static void FillDstarPlus(double* variable, EvtParticle* DstarPlus, EvtParticle* D0, EvtParticle* piPlus) {
        EvtVector4R v_DstarPlus = DstarPlus->getP4Lab();
        EvtVector4R v_D0 = D0->getP4Lab();
        EvtVector4R v_piPlus = piPlus->getP4Lab();

        TLorentzVector DstarPlusVec(v_DstarPlus.get(1), v_DstarPlus.get(2), v_DstarPlus.get(3), v_DstarPlus.get(0));
        TLorentzVector D0Vec(v_D0.get(1), v_D0.get(2), v_D0.get(3), v_D0.get(0));
        TLorentzVector piPlusVec(v_piPlus.get(1), v_piPlus.get(2), v_piPlus.get(3), v_piPlus.get(0));

        variable[kDstarPlusPt] = DstarPlusVec.Pt();
        variable[kDstarPlusEta] = DstarPlusVec.Eta();
        variable[kDstarPlusPhi] = DstarPlusVec.Phi();
        variable[kDstarPlusMass] = DstarPlusVec.M();
        variable[kD0Pt] = D0Vec.Pt();
        variable[kD0Eta] = D0Vec.Eta();
        variable[kD0Phi] = D0Vec.Phi();
        variable[kD0Mass] = D0Vec.M();
        variable[kPiPlusPt] = piPlusVec.Pt();
        variable[kPiPlusEta] = piPlusVec.Eta();
        variable[kPiPlusPhi] = piPlusVec.Phi();
        variable[kPiPlusMass] = piPlusVec.M();
    }

    static void FillJpsi(double* variable, EvtParticle* Jpsi, EvtParticle* ePlus, EvtParticle* eMinus) {
        EvtVector4R v_Jpsi = Jpsi->getP4Lab();
        EvtVector4R v_ePlus = ePlus->getP4Lab();
        EvtVector4R v_eMinus = eMinus->getP4Lab();

        TLorentzVector JpsiVec(v_Jpsi.get(1), v_Jpsi.get(2), v_Jpsi.get(3), v_Jpsi.get(0));
        TLorentzVector ePlusVec(v_ePlus.get(1), v_ePlus.get(2), v_ePlus.get(3), v_ePlus.get(0));
        TLorentzVector eMinusVec(v_eMinus.get(1), v_eMinus.get(2), v_eMinus.get(3), v_eMinus.get(0));

        variable[kJpsiPt] = JpsiVec.Pt();
        variable[kJpsiEta] = JpsiVec.Eta();
        variable[kJpsiPhi] = JpsiVec.Phi();
        variable[kJpsiMass] = JpsiVec.M();
        variable[kEPlusPt] = ePlusVec.Pt();
        variable[kEPlusEta] = ePlusVec.Eta();
        variable[kEPlusPhi] = ePlusVec.Phi();
        variable[kEPlusMass] = ePlusVec.M();
        variable[kEMinusPt] = eMinusVec.Pt();
        variable[kEMinusEta] = eMinusVec.Eta();
        variable[kEMinusPhi] = eMinusVec.Phi();
        variable[kEMinusMass] = eMinusVec.M();
    }

private:
};

#endif