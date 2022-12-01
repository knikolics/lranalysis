#ifndef Ar_ArConfiguration
#define Ar_ArConfiguration

#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <TROOT.h>
#include <TClonesArray.h>
#include <TArrayI.h>

using namespace std;

#include "ArDetector.hh"
#include "ArRun.hh"

class ArDetector;
class ArRun;



class ArConfiguration : public TObject {
  
public:

  TString ePath;
  Int_t eRunNumber;
  TString eCurrentFileName;

  Int_t BitResolution;
  Double_t SampleSize;
  Int_t EventsInTheMask;
  Int_t FileNumber;
  Long64_t FirstEvent;
  Long64_t LastEvent;
  TString EventsMaskFile;
  TArrayI FileList;
  TArrayI EventList;

  TString CalibrationFile;
  TArrayD Calibration;
  TArrayI Delay;
  Int_t ApplyDelayCorrectionFromDAQInfo;
  Int_t EvaluateChannelsFromVirtualChannel;
  Int_t FakeSignalTime;
  Double_t SingletIntegrationTime;
  Double_t PedPeakFindingTimeMargin;
  Double_t PeakFindingTimeMargin;
  Double_t PedPeakFindingDisentangleTime;
  Double_t PeakFindingDisentangleTime;
  Double_t PedPeakFindingSigmaThreshold;
  Double_t PeakFindingSigmaThreshold;
  TArrayD Pedestal;
  Int_t VirtualChannelMask;
  TArrayI VirtualChannelMaskList;

  Int_t StartPedestal;
  Int_t StopPedestal;
  Int_t StartPeakSignal;
  Int_t StopPeakSignal;
  Double_t ReferenceConversionFactor;
  Double_t MaximumIntegrationTime;

public:
  ArConfiguration();
  ArConfiguration(const char *path, Int_t runnumber);
  ~ArConfiguration();
  
  // Reset private variables
  void Reset();

  TString Path()                { return ePath; }
  Int_t   RunNumber()           { return eRunNumber; }

  void SetPath(const char *path)    { ePath = path; }
  void SetRunNumber(Int_t i)        { eRunNumber = i; }

  void Print(FILE *fp = stdout);

  Int_t Load();
  Int_t ReadMask(ifstream &s);
  Int_t LoadEventsMaskFile();
  Int_t PrintMask();
  Int_t ReadCalibration(ifstream &s);
  Int_t ReadDelay(ifstream &s);
  Int_t ReadPedestal(ifstream &s);
  Int_t ReadVirtualChannelMask(ifstream &s);

  //ClassDef(ArConfiguration,1);
  ClassDef(ArConfiguration,1)
};


#endif /* Ar_ArConfiguration */
