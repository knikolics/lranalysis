#ifndef Ar_ArRun
#define Ar_ArRun

#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <TROOT.h>
#include <TClonesArray.h>
#include <TArrayI.h>



using namespace std;

#include "ArPMT.hh"
#include "ArConfiguration.hh"
//#include "/home/ardm/analysis/LRAnalysis/LRAnalysis/src/ArRun/ArDMData.hh"
#include "/home/knguyen/analysis/LRAnalysis/LRAnalysis/src/ArRun/ArDMData.hh"

#define NBOARD C_NBMAX_PMT_BOARD //=4
#define NCHANNEL_ON_BOARD C_LIGHT_NBMAX_CHANNEL_PER_BOARD //=8 
#define NCHANNEL_IN_ONE_BOARD_STORED_IN_RAW_DATA_FILE 7
#define NCHANNEL_IN_ONE_BOARD_STORED_IN_OUTPUT_TREE 6   // <= NCHANNEL_IN_ONE_BOARD_STORED_IN_RAW_DATA_FILE !!!
//#define NCHANNEL_USED_FOR_PMT_ON_ONE_BOARD 6

class ArDetector;
class ArConfiguration;


class ArRun : public TObject {

private:

  TString  ePath;
  Int_t    eRunNumber;
  Int_t    eFileNumber;
  Int_t    eEventNumber;
  UInt_t   eUnixTime;
  Int_t    eNPMTs;
  uint32_t eTTT[NBOARD];

  //Int_t    eTotalNumberOfEvents;

  //there are 8 channels on each board
  //6 first channels on each board are used for 6 pmts
  //the 7th channel is for trigger
  //the 8th channel is not used, records only physical noise of its own
  //but the 8th channel is also stored in the rawDataFile
  //so in the rawDataFile, all 4*8 channels are there !!! 
  //and not only 4*6 for 24 pmts or 4*7 for 24 pmts + 4 triggers !
  //
  //--> in order to get the correct value for the variable ArRun::eNumberOfSamples, 
  //which is the number of samples for each PMT,
  //do this : eNumberOfSamples = ArDMData::getNbData()/NCHANNEL_ON_BOARD ; 
  //in this case, since all 8 channels are stored to the rawDataFile, NCHANNEL_ON_BOARD = 8 !!
  //
  //the variable ArDMData::m_sizeData, which is returned by the method ArDMData::getNbData(),
  //is the total number of samples for 1 board, i.e. for all 8 channels on that board !!

  //Int_t    eDelay[NBOARD*NCHANNEL_ON_BOARD]; //nentries = eNPMTs
  Int_t    eDelay[NBOARD*NCHANNEL_IN_ONE_BOARD_STORED_IN_OUTPUT_TREE]; //delay of each pmt. trigger channels are not interesting --> ignore them !
  Int_t    eNumberOfSamples;
  Int_t    eTotalNumberOfFiles;
  Int_t    eNumberOfEventsInFile;
  //UInt_t   eAcquisitionTime;


//   TClonesArray* eData; //4 boards --> 4 entries, 6 PMTs on one board
  TClonesArray* ePMTs; 
  ArPMT*        eVPMT;

public:

  ArRun();
  ArRun(const char *path, Int_t runnumber);
  ~ArRun();


  Int_t    RunNumber(){ return eRunNumber; };
  Int_t    FileNumber(){ return eFileNumber; };
  Int_t    EventNumber(){ return eEventNumber; };
  UInt_t   unixTime(){ return eUnixTime; };
  Int_t    NPMTs(){  return ePMTs ? ePMTs->GetEntriesFast() : 0; };
  uint32_t* TTT(){ return eTTT; };
  Int_t    Delay(int i){ return eDelay[i]; };
  Int_t    NumberOfSamples(){ return eNumberOfSamples; };
  ArPMT*   GetPMT(int pmtid){ return (ArPMT*)ePMTs->At(pmtid); };
  ArPMT*   GetVPMT(){ return eVPMT;};
  Int_t    TotalNumberOfFiles(){ return eTotalNumberOfFiles; };
  Int_t    NumberOfEventsInFile(){ return eNumberOfEventsInFile; };

  Int_t    CurrentFileNumber(){ return eFileNumber;};
  Int_t    CurrentEventNumber(){return eEventNumber;};
  //UInt_t   AcquisitionTime(){ return eAcquisitionTime;};
  Int_t    NSamples(){ return eNumberOfSamples;};


  Int_t LoadEvent(Int_t evtid);
  Int_t LoadEvent(Long64_t evtid);
  void  LoadConfig();
  void  Reset();

  void  BeginningOfEvent();

  ArPMT* AddPMT();

  ArDMData* eData;


  //test //khoi
  Int_t    eBoardEventNumber[NBOARD];
  double eSumPeakIntegralTop;
  double eSumPeakIntegralBottom;
  //end test


  /*





private:
  TTree   *eTree;
  TBranch *eBranchTTT;
  TBranch *eBranchDelay;
  TBranch *eBranchNSamples;

  TClonesArray *ePMTs;

  ArPMT *eVPMT;

  TString ePath;
  Int_t eRunNumber;
  Float_t eTime;
  TString eCurrentFileName;
  TFile *eCurrentFile;

  Int_t eTotalNumberOfFiles;
  Int_t eNumberOfEventsInFile;

  Int_t eCurrentFileNumber;  // Number of current file in the files set
  Int_t eCurrentEventNumber; // Number of current event inside the current file

  Int_t eNumberOfSamples;    // Number of samples for each channel

  Int_t eDisMajThresholds;   // ???
  Int_t eTriggerDelay;       // ???
  Int_t eTriggerType;        // 0 - Calibration, trigger on the pulser
                             // 1 - Internal Trigger on Majority of [n] (PMTs over threshold)
                             // 2 - External trigger

  time_t eAcquisitionTime;         // Aquisition time of the run

  Long64_t eTTT;         // Event timestamp
  //Int_t eDelay[24];      // Delay among PMTs for each event
  Int_t eDelay[16];      // Delay among PMTs for each event
  Int_t eNSamples;       // Number of samples per event in each PMT


  //test (khoi nguyen)
  //retrieve unixTime of event
  TBranch* eBranchUnixTime;
  UInt_t   eUnixTime;

  //end test

public:

  ArRun();
  ArRun(const char *path, Int_t runnumber);
  ~ArRun();

  // Reset private variables
  void Reset();

  TString Path()                { return ePath; }
  Int_t   RunNumber()           { return eRunNumber; }
  Float_t Time()                { return eTime; }
  Int_t   TotalNumberOfFiles()  { return eTotalNumberOfFiles; }
  Int_t   NumberOfEventsInFile(){ return eNumberOfEventsInFile; }
  Int_t   CurrentFileNumber()   { return eCurrentFileNumber; }
  Int_t   CurrentEventNumber()  { return eCurrentEventNumber; }
  Int_t   NumberOfSamples()     { return eNumberOfSamples; }
  Int_t   DisMajThresholds()    { return eDisMajThresholds; }
  Int_t   TriggerDelay()        { return eTriggerDelay; }
  Int_t   TriggerType()         { return eTriggerType; }
  Int_t   AcquisitionTime()     { return eAcquisitionTime; }

  Long64_t TTT()                { return eTTT; }
  Int_t   Delay(Int_t i)        { return eDelay[i]; }
  Int_t   NSamples()            { return eNSamples; }

  void SetPath(const char *path)    { ePath = path; }
  void SetRunNumber(Int_t i)        { eRunNumber = i; }
  void SetTime(Float_t f)           { eTime = f; }
  void SetTotalNumberOfFiles(Int_t i)  { eTotalNumberOfFiles=i; }
  void SetNumberOfEventsInFile(Int_t i){ eNumberOfEventsInFile=i; }
  void SetCurrentFileNumber(Int_t i)   { eCurrentFileNumber=i; }
  void SetCurrentEventNumber(Int_t i)  { eCurrentEventNumber=i; }
  void SetNumberOfSamples(Int_t i)     { eNumberOfSamples=i; }
  void SetDisMajThresholds(Int_t i)    { eDisMajThresholds=i; }
  void SetTriggerDelay(Int_t i)        { eTriggerDelay=i; }
  void SetTriggerType(Int_t i)         { eTriggerType=i; }
  void SetAcquisitionTime(Int_t i)     { eAcquisitionTime=i; }

  void SetTTT(Long64_t i)              { eTTT=i; }
  void SetDelay(Int_t ipmt, Int_t i)   { eDelay[ipmt]=i; }
  void SetNSamples(Int_t i)            { eNSamples=i; }

  void Print(FILE *fp = stdout);

  // Adds a detector
  ArPMT *AddPMT();
  
  // Returns the i-th detector
  ArPMT *GetPMT(int i) const { return (ArPMT *)ePMTs->At(i); }

  // Returns the detector array
  TClonesArray *GetPMTs() const { return ePMTs; }

  // Returns the number of detectors
  Int_t NPMTs() const { return ePMTs ? ePMTs->GetEntriesFast() : 0; }
  
  // Returns the Virtual PMT
  ArPMT *GetVPMT() { return eVPMT; }

  // Set the Virtual PMT
  void SetVPMT(ArPMT *pmt) { eVPMT = pmt; }

  // Opens next file, accordind to eCurrentFileNumber
  Int_t OpenNextFile();

  // Opens file with number given as input
  Int_t OpenFile(Int_t ifile);

  // Closes current file
  Int_t CloseFile();

  // Reads data from next event
  Int_t LoadNextEvent();

  // Reads data from event given as input
  Int_t LoadEvent(Int_t ievent);

  // Reads data from file and event given as input
  Int_t LoadEvent(Int_t ifile, Int_t ievent);

  // Reads data from global event number given as input
  Int_t LoadEvent(Long64_t ilongevent);

  // Read configuration from file
  void LoadConfig();

  // Set calibration...
  void SetCalibration(Int_t ndetectors); // from default file
  void SetCalibration(Int_t ndetectors, TString filename); // from a given file
  void SetCalibration(TArrayD cal); // from a given array

  // Set delay time from a given array
  void SetDelay(TArrayI delay);

  // Set pedestal level from a given array
  void SetPedestal(TArrayD pedestal);

  // Print configuration from file
  void PrintConfig(FILE *fp = stdout);

  // Print raw data without any modification
  void PrintRawData(FILE *fp = stdout);

  // Print data in double precision format. They could be modified by analysis program
  void PrintData(FILE *fp = stdout);

  // Print raw data (if level=0) or data (if level=1) to a file
  void PrintData(Int_t level);

  // Print calibrations
  void PrintCalibration(FILE *fp = stdout);

  // Print delay
  void PrintDelay(FILE *fp = stdout);



  //test (khoi nguyen)
  //retrieve unixTime of event
  UInt_t unixTime(){return eUnixTime;}
  void   setUnixTime(UInt_t time) { eUnixTime = time;}

  //end test



  */


  //ClassDef(ArRun,1);
  ClassDef(ArRun,1)
};

#endif /* Ar_ArRun */
