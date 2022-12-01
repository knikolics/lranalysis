#ifndef Ar_ArOutput
#define Ar_ArOutput

#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <TROOT.h>
#include <TArrayF.h>

#include "ArRun.hh"
#include "ArRecoPMT.hh"
#include "ArDMData.hh"

class ArOutput : public TObject {

private:

  TString  eFileName;
  TFile*   eFile;
  TTree*   eTreeData;
  TTree*   eTreeMeanTrace;
  //  TTree*   eTreeHeader;

  // Header tree informations
  Int_t eRunNumber;
  //Int_t eFileNumber;
  Int_t eEventNumber;
  Int_t eBoardEventNumber[NBOARD];
  Int_t eNPMTs;
  //Int_t eAcquisitionTime;

  uint32_t eTTT[NBOARD];
  //Int_t eDelay[16];
  //Int_t eDelay[24];
  //Int_t eDelay[NBOARD*NCHANNEL_IN_ONE_BOARD_STORED_IN_OUTPUT_TREE];
  Int_t eNSamples;

  // Data tree informations
  TClonesArray *eRecoPMTs;
  ArRecoPMT *eRecoVPMT;

  // Mean trace tree informations
  Int_t ePMT;
  Int_t eTime;
  Double_t eData;
  Double_t eDataError;


  //test (khoi nguyen)
  //retrieve unixTime of event
  UInt_t eUnixTime;
  double eSumPeakIntegralTop;
  double eSumPeakIntegralBottom;
  double eSumPeakIntegralTotal;
  //end test

public:

  ArOutput(const char *filename);
  ~ArOutput();

  Int_t Open();
  Int_t Cd();
  Int_t FillData(ArRun *run);
  Int_t FillMeanTrace(ArAnalysis *analysis);
  //  Int_t FillHeader(ArRun *run);
  Int_t Close();

  ClassDef(ArOutput,1);
};

#endif /* Ar_ArOutput */
