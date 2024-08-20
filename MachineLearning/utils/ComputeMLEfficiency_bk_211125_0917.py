# script by lupz to compute ML efficiency at any given BDT cut
import sys
import argparse
import time
import itertools
import numpy as np
import yaml
import ctypes
from ROOT import TFile, TH1F, TH2F, TF1, TCanvas, TNtuple, TDirectoryFile  # pylint: disable=import-error,no-name-in-module
from ROOT import gROOT, kRainBow, kBlack, kFullCircle  # pylint: disable=import-error,no-name-in-module
from .AnalysisUtils import ComputeEfficiency #pylint: disable=wrong-import-position,import-error



def ComputeMLEfficiency(RootFileNameGen=[],TreeName=[],MassCut=[0,0],BDTBkgCut=[0,1],BDTPromptCut=[0,1],BDTFDCut=[0,1]):

    hInput = TH1F("hInput" ,";M_{e^+e^-} (GeV/c^2);N",250,0.0,10)
    hInput.Sumw2()
    hRec = TH1F("hRec" ,";M_{e^+e^-} (GeV/c^2);N",250,0.0,10)
    hRec.Sumw2()
    
    for ifile,(InputFileName,InputTreeName) in enumerate(zip(RootFileNameGen,TreeName)):

        print("InFile: ",InputFileName,":",InputTreeName)
        print("Mass window: ",MassCut)
        print("BDT Bkg cuts: ",BDTBkgCut," && ","BDT Prompt cuts: ",BDTPromptCut," && ","BDT FD cuts: ",BDTFDCut)

        InputFile = TFile.Open(InputFileName,"READ")
        tree = InputFile.Get(InputTreeName)
        for entry in tree:
            if(MassCut[0]<entry.pairMass<MassCut[1]):
                hInput.Fill(entry.pairMass)
                if(BDTBkgCut[0]<entry.model_output_0<BDTBkgCut[1] and BDTPromptCut[0]<entry.model_output_1<BDTPromptCut[1] and BDTFDCut[0]<entry.model_output_2<BDTFDCut[1]):
                    hRec.Fill(entry.pairMass)
        
        hInput.SetDirectory(0)
        hRec.SetDirectory(0)
        InputFile.Close()
    
    NInputUnc,NRecUnc = (ctypes.c_double() for i in range(2))
    
    NInput = hInput.IntegralAndError(0,hInput.GetNbinsX()+1,NInputUnc)
    NRec = hRec.IntegralAndError(0,hRec.GetNbinsX()+1,NRecUnc)
    
    
    eff,effUnc = ComputeEfficiency(NRec,NInput,NRecUnc.value,NInputUnc.value)
    
    return NRec, eff, effUnc
    
    
    
