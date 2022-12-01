#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <TROOT.h>
#include <TClonesArray.h>
#include <TSystem.h>

#include "ArRun.hh"
#include "ArPMT.hh"
#include "ArConfiguration.hh"
#include "ArAnalysis.hh"
#include "ArIO.hh"

#include "sstream"
#include "bitset"

#include "TStopwatch.h"


using namespace std;

ClassImp(ArRun)

const int ArRun_verbose = 0;


ArRun :: ArRun()
{
  ePMTs = new TClonesArray("ArPMT");
  eVPMT = new ArPMT();
  Reset();
}







ArRun :: ArRun(const char *path, Int_t runnumber)
{
  ePMTs = new TClonesArray("ArPMT");
  eVPMT = new ArPMT();
  Reset();
  ePath=path;
  eRunNumber=runnumber;


  ostringstream pathStr,runnumberStr;
  pathStr<<path;
  runnumberStr<<runnumber;

  eData = new ArDMData(pathStr.str(),runnumberStr.str());

}


ArRun :: ~ArRun()
{
  if (ePMTs) {
    ePMTs->Delete();
    delete ePMTs;
    ePMTs=0;
  }

  if(eVPMT) delete eVPMT;
  eVPMT = 0;
}







void ArRun :: Reset()
{
  for(int i=0;i<sizeof(eTTT)/sizeof(Long64_t);i++) eTTT[i] = 0LL;
  for(int i=0;i<sizeof(eBoardEventNumber)/sizeof(Int_t);i++) eBoardEventNumber[i] = -1;
  for (Int_t i=0;i<sizeof(eDelay)/sizeof(Int_t);i++) eDelay[i] = 0;
  eNumberOfSamples = 0;

  ePath="";
  eRunNumber=0;
  eEventNumber=-1;
  if(ePMTs) ePMTs->Clear();
  eVPMT->Reset();

  eUnixTime = -111111;

  if(eData) delete eData;

  return;
}



void ArRun::BeginningOfEvent(){

  eRunNumber       = -1;
  eEventNumber     = -1;
  eUnixTime        = -1;
  for(int i=0;i<sizeof(eTTT)/sizeof(uint32_t);i++) eTTT[i] = -1;
  for(int i=0;i<sizeof(eBoardEventNumber)/sizeof(Int_t);i++) eBoardEventNumber[i] = -1;
  eSumPeakIntegralTop = -1;
  eSumPeakIntegralBottom = -1;
  //eAcquisitionTime = -1;

  return;
}




ArPMT *ArRun :: AddPMT()
{
  return new((*ePMTs)[NPMTs()]) ArPMT();
}




void ArRun::LoadConfig(){
  
  ostringstream runNumberStr;
  runNumberStr<<eRunNumber;
//   for(int i=0;i<NBOARD;i++) new(eData[i]) ArDMData(ePath,runNumberstr.str());
  

  ArDMData* eData = new ArDMData((string)ePath,runNumberStr.str());
  //eData->readEvent(0,0);
  eData->readSubEvent(0,0);
  //eNumberOfSamples = eData->getNbData()/NPMTonBoard;
  //eNumberOfSamples = eData->getNbData()/6;

  //for more information, see explanation in ArRun.hh !
  eNumberOfSamples = eData->getNbData()/NCHANNEL_ON_BOARD;

  eTotalNumberOfFiles = 1;
  eNumberOfEventsInFile = eData->getNbEvent();
  //<-- this is not the number of events in the run !
  //it's the number of data blocks, each of which corresponds to the data recorded by 1 board in 1 event !!
  //i.e., if in each event, **all** 4 boards record some data and those are stored in the rawDataFile
  //the actual number of events in the run is getNbEvent()/4 !!
  //it is possible that only 2 or 3 boards record data in some events, and there are therefore only 2/3 data blocks


  //eNPMTs = NPMT;
  //eNPMTs = 24;

  //for more information, see ArRun.hh !
  eNPMTs = NBOARD*NCHANNEL_IN_ONE_BOARD_STORED_IN_OUTPUT_TREE;

  if(ArRun_verbose) cout<<"in ArRun::LoadConfig() ... eNumberOfEventsInFile "<<eNumberOfEventsInFile<<endl;
  
  //for(int i=0;i<NPMT;i++){
  for(int i=0;i<eNPMTs;i++){

    if(ArRun_verbose) cout<<"pmtID "<<i<<endl;

    ArPMT* pmt = new((*ePMTs)[i]) ArPMT();
    //ArPMT* pmt = AddPMT();

    if(ArRun_verbose) cout<<"pmtID "<<i<<"\t pmt "<<pmt<<endl;

    pmt->SetID(i);

    ostringstream chname;
    chname<<i;
    //some what arbitray number for the time being
    pmt->SetFullScale(1.0);
    pmt->SetOffset(4001.477);
    pmt->SetCoupling(3);
    pmt->SetBandWidth(250);
    pmt->SetDiscThrs(3680);
    pmt->SetName(chname.str());
    pmt->RawData.Set(eNumberOfSamples);
    pmt->Data.Set(eNumberOfSamples);
    pmt->DataError.Set(eNumberOfSamples);    
  }



  if(ArRun_verbose) cout<<"in ArRun::LoadConfig() ... create configuration for virtual channel ...ePMTs "<<ePMTs<<endl;

  // create configuration for the virtual channel copying from the first PMT
  ArPMT *pmt0 = GetPMT(0); 

  if(ArRun_verbose) cout<<"in ArRun::LoadConfig() ... create configuration for virtual channel ... pmt0 "<<pmt0<<endl;

  eVPMT->SetID(NPMTs()+1);
  eVPMT->SetFullScale(pmt0->FullScale());
  eVPMT->SetOffset(pmt0->Offset());
  eVPMT->SetCoupling(pmt0->Coupling());
  eVPMT->SetBandWidth(pmt0->BandWidth());
  eVPMT->SetDiscThrs(pmt0->DiscThrs());
  eVPMT->SetName("Virtual");
  eVPMT->SetType(pmt0->Type());
  eVPMT->Data.Set(pmt0->Data.GetSize());
  eVPMT->DataError.Set(pmt0->DataError.GetSize());

  if(ArRun_verbose) cout<<"in ArRun::LoadConfig() ... finish loading configuration ..."<<endl;
  return;
}



Int_t ArRun::LoadEvent(Int_t ievent){

  if (ievent<0) {
    printf("Error! Event %d cannot be read because no file was specified\n",ievent);
    return(-1);
  }


  TStopwatch timer;
  timer.Start();

  if(ArRun_verbose) cout<<"in ArRun::LoadEvent(..) ... event "<<ievent<<"\t calling run->BeginningOfEvent(..) ... "<<endl;

  BeginningOfEvent();


  if(ArRun_verbose){
    timer.Stop();
    cout<<"in ArRun::LoadEvent(..) ... event "<<ievent<<"\t finished run->BeginningOfEvent(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
    timer.Start();
  }



  if(ArRun_verbose) cout<<"in ArRun::LoadEvent(..) ... event "<<ievent<<"\t calling ArDMData::getEvent(..) ... "<<endl;


  tIndexItem evtIndex[NBOARD];
  int boardmask = eData->getEvent(ievent,evtIndex);


  if(ArRun_verbose){
    timer.Stop();
    cout<<"in ArRun::LoadEvent(..) ... event "<<ievent<<"\t finished ArDMData::getEvent(..)  ... \t\t\t\t"<<timer.RealTime()<<endl;
    timer.Start();
  }


  if(ArRun_verbose) cout<<"in ArRun::LoadEvent(..) ... event "<<ievent<<"\t loading data from rawDataFile ... "<<endl;

  if(ArRun_verbose){
    std::bitset<4> boardmaskBinary(boardmask);
    cout<<"event "<<ievent<<"\t boardmaskBinary "<<boardmaskBinary<<endl;;
  }

  for(int i=0;i<NBOARD;i++){
    if(ArRun_verbose) cout<<"event "<<ievent<<"\t position 1 ... i "<<i<<endl;
    if((boardmask>>i)&0x1){
      if(ArRun_verbose) cout<<"event "<<ievent<<"\t position 2 ... i "<<i<<endl;

      if(i==0 && ArRun_verbose) cout<<"in ArRun::LoadEvent(..) ... event "<<ievent<<"\t calling ArDMData::readSubEvent(..) ... "<<endl;      

      eData->readSubEvent(&evtIndex[i]);

      //eTTT[evtIndex[i].board] = eData->getTTT();
      eTTT[i] = eData->getTTT();
      eBoardEventNumber[i] = eData->getEventID();
      //cout<<"event "<<ievent<<"\t i "<<i<<"\t evtIndex["<<i<<"].board "<<evtIndex[i].board<<endl;

      if(i==0 && ArRun_verbose){
	timer.Stop();
	cout<<"in ArRun::LoadEvent(..) ... event "<<ievent<<"\t finished calling ArDMData::readSubEvent(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }

      for(int ii=0;ii<NCHANNEL_IN_ONE_BOARD_STORED_IN_OUTPUT_TREE;ii++){
	ArPMT* pmt = GetPMT(i*NCHANNEL_IN_ONE_BOARD_STORED_IN_OUTPUT_TREE+ii);
	for(int samplei=0;samplei<eNumberOfSamples;samplei++){
	  pmt->RawData[samplei] = eData->getData(ii*eNumberOfSamples+samplei);
	}
      }
    }


  }


  if(ArRun_verbose){
    timer.Stop();
    cout<<"in ArRun::LoadEvent(..) ... event "<<ievent<<"\t finished loading data from rawDataFile ... \t\t\t\t"<<timer.RealTime()<<endl;
    timer.Start();
  }



  if(ArRun_verbose) cout<<"in ArRun::LoadEvent(..) ... event "<<ievent<<"\t loading event infos ... "<<endl;

  for(int i=0;i<NBOARD;i++){
    if((boardmask>>i)&0x1){
      eUnixTime = eData->getTimeStamp();
      //eTTT      = eData->getTTT();
      //eAcquisitionTime = eData->getRunTimeStamp();
      eRunNumber   = eData->getIdRun();
      eEventNumber = ievent;//eData->getEventID();
      //eFileNumber = 1; //default value; //<-- do we need that any longer !?
      eNPMTs = NBOARD*NCHANNEL_IN_ONE_BOARD_STORED_IN_OUTPUT_TREE;
      for(int i=0;i<sizeof(eDelay)/sizeof(Int_t);i++) eDelay[i] = 0;
      
      break;
    }
  }


  if(ArRun_verbose){
    timer.Stop();
    cout<<"in ArRun::LoadEvent(..) ... event "<<ievent<<"\t finished loading event infos ... \t\t\t\t"<<timer.RealTime()<<endl;
    timer.Start();
  }

  timer.Stop();

  return(eEventNumber);
  //return(ievent);
}





Int_t ArRun :: LoadEvent(Long64_t ilongevent)
{
//   Int_t ifile  = (Int_t) (ilongevent / eNumberOfEventsInFile);
//   Int_t ievent = (Int_t) (ilongevent % eNumberOfEventsInFile);

//   Int_t outputvalue;
//   if((outputvalue = OpenFile(ifile))<0) return(outputvalue);

  return( LoadEvent((int)ilongevent) );
}














////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*


ArRun :: ArRun()
{
  ePMTs = new TClonesArray("ArPMT");
  eVPMT = new ArPMT();
  Reset();
}


ArRun :: ArRun(const char *path, Int_t runnumber)
{
  ePMTs = new TClonesArray("ArPMT");
  eVPMT = new ArPMT();
  Reset();
  ePath=path;
  eRunNumber=runnumber;
}


ArRun :: ~ArRun()
{
  if (ePMTs) {
    ePMTs->Delete();
    delete ePMTs;
    ePMTs=0;
  }
  delete eVPMT;
  eVPMT = 0;
}


void ArRun :: Reset()
{
  eTTT = 0LL;
  for (Int_t i=0;i<16;i++) eDelay[i] = 0;
  eNSamples = 0;

  ePath="";
  eRunNumber=0;
  eCurrentFileNumber=-1;
  eCurrentEventNumber=-1;
  eCurrentFileName="";
  eCurrentFile=0;
  if(ePMTs) ePMTs->Clear();
  eVPMT->Reset();

  //test (khoi nguyen)
  //retrieve unixTime of event
  eUnixTime = -111111;
  //end test


}


void ArRun :: Print(FILE *fp)
{
  ArIO::PrintColor(fp,"** Run\n");
  fprintf(fp,"Run number %d, stored in path %s\n",eRunNumber,ePath.Data());
  fprintf(fp,"Total number of files: %d\n",eTotalNumberOfFiles);
  fprintf(fp,"Number of events in file: %d\n",eNumberOfEventsInFile);
  fprintf(fp,"Current file number: %d\n",eCurrentFileNumber);
  fprintf(fp,"Current event number: %d\n",eCurrentEventNumber);
  fprintf(fp,"Number of samples: %d\n",eNumberOfSamples);
  fprintf(fp,"DisMajThresholds: %d\n",eDisMajThresholds);
  fprintf(fp,"Trigger delay: %d\n",eTriggerDelay);
  fprintf(fp,"Trigger type: %d\n",eTriggerType);
  fprintf(fp,"Acquisition time: %ld\n",eAcquisitionTime);
  fprintf(fp,"TTT: %lld\n",eTTT);
  fprintf(fp,"Delay: ");
  for (int id=0;id<16;id++) fprintf(fp,"%d ",eDelay[id]);
  fprintf(fp,"\n");
  fprintf(fp,"NSamples: %d\n",eNSamples);

  fprintf(fp,"Number of channels (detectors): %d\n",NPMTs());

  for (int i=0;i<NPMTs();i++) {
    ArPMT *d = GetPMT(i);
    d->Print(fp);
  } 
  eVPMT->Print(fp);
}


ArPMT *ArRun :: AddPMT()
{
  return new((*ePMTs)[NPMTs()]) ArPMT();
}


Int_t ArRun :: OpenNextFile()
{
  return( OpenFile(eCurrentFileNumber+1) );
}


Int_t ArRun :: OpenFile(Int_t ifile)
{
  // Open file number ifile and, if successful, set eCurrentFileNumber to ifile
  // Output:
  // n>=  0 current opened file number (eCurrentFileNumber)
  //   = -1 skipping a file     (TODO!)
  //   = -2 filecount exceeded  (TODO!)
  //   = -3 file not found

  if (eCurrentFileNumber==ifile) return(eCurrentFileNumber);
  else CloseFile();

  TString filename;

  ArIO::MakeFileName(filename,ePath,eRunNumber,ifile,"root");

  if (gSystem->AccessPathName(filename.Data(),kFileExists)) return(-3);

  eCurrentFile = new TFile(filename.Data(),"READ");

  if (!eCurrentFile->IsOpen()) {
    printf("Error! File %s cannot be opened\n",filename.Data());
    return(-3);
  }

  eCurrentFileName = filename;
  eCurrentFileNumber = ifile;

  if (ArDBLEVEL>=2) printf("Opening file %s\n",eCurrentFileName.Data());

  TString type;
  if (eCurrentFile->FindKey("AcqirisData")) {
    if (ArDBLEVEL>=2) printf("Data come from Acqiris\n");
    type="Acqiris";
    //  } else if (eCurrentFile->FindKey("tree")) {
  } else if (eCurrentFile->FindKey("CAENData")) {
    if (ArDBLEVEL>=2) printf("Data come from CAEN\n");
    type="CAEN";
  } else {
    if (ArDBLEVEL>=2) printf("Data come from an oscilloscope\n");
    type="Oscilloscope";
  }

  if (type=="CAEN") {
    eTree = (TTree*)eCurrentFile->Get("CAENData");
    eBranchTTT = eTree->GetBranch("TTT");
    eBranchTTT->SetAddress(&eTTT);
    eBranchDelay = eTree->GetBranch("delay");
    eBranchDelay->SetAddress(eDelay);
    eBranchNSamples = eTree->GetBranch("nSamples");
    eBranchNSamples->SetAddress(&eNSamples);

    //test (khoi nguyen)
    //retrieve unixTime of event
    eBranchUnixTime = eTree->GetBranch("unixTime"); 
    if(eBranchUnixTime) eBranchUnixTime->SetAddress(&eUnixTime);
    //end test

  }

  for (Int_t i=0;i<NPMTs();i++) {
    GetPMT(i)->SetType(type);
    GetPMT(i)->SetTree(eCurrentFile,i);
  }

  return(eCurrentFileNumber);
}


Int_t ArRun :: CloseFile()
{
  if (eCurrentFile) {
    eCurrentFile->Close();
    delete eCurrentFile;
    eCurrentFile=0;
  }
}


Int_t ArRun :: LoadNextEvent()
{
  eCurrentEventNumber++;

  for (Int_t i=0;i<NPMTs();i++) {
    if (!GetPMT(i)->LoadEvent(eCurrentEventNumber)) {
      if (ArDBLEVEL>=3) printf("File is finished\n");
      return(-1);
    }
  }

  if (ArDBLEVEL>=3) printf("Getting Event %6d\n",eCurrentEventNumber);

  return(eCurrentEventNumber);
}


Int_t ArRun :: LoadEvent(Int_t ievent)
{
  if (eCurrentFileNumber<0) {
    printf("Error! Event %d cannot be read because no file was specified\n",ievent);
    return(-1);
  }

  eCurrentEventNumber = ievent;

  if (ArDBLEVEL>=3) printf("Getting Event %6d\n",eCurrentEventNumber);

  if (GetPMT(0)->Type()=="CAEN") {
    eBranchTTT->GetEntry(ievent);
    eBranchDelay->GetEntry(ievent);
    eBranchNSamples->GetEntry(ievent);
    
  //test (khoi nguyen)
  //retrieve unixTime of event
    if(eBranchUnixTime) eBranchUnixTime->GetEntry(ievent);
  //end test

  }

  for (Int_t i=0;i<NPMTs();i++) {
    if (!GetPMT(i)->LoadEvent(ievent)) {
      if (ArDBLEVEL>=3) printf("Event %d does not exist: PMT = %d\n",eCurrentEventNumber,i);
      return(-1);
    }
  }

  return(eCurrentEventNumber);
}


Int_t ArRun :: LoadEvent(Int_t ifile, Int_t ievent)
{
  Int_t outputvalue;
  if((outputvalue = OpenFile(ifile))<0) return(outputvalue);
  return( LoadEvent(ievent) );
}


Int_t ArRun :: LoadEvent(Long64_t ilongevent)
{
  Int_t ifile  = (Int_t) (ilongevent / eNumberOfEventsInFile);
  Int_t ievent = (Int_t) (ilongevent % eNumberOfEventsInFile);

  Int_t outputvalue;
  if((outputvalue = OpenFile(ifile))<0) return(outputvalue);
  return( LoadEvent(ievent) );
}


void ArRun :: LoadConfig()
{
  TString filename;
  ArIO::MakeFileName(filename,ePath,eRunNumber,-1,"chcfg");

  if (ArDBLEVEL>=2) printf("Opening configuration file %s\n",filename.Data());

  ifstream s;
  s.open(filename.Data());

  TString tmpS;
  s >> tmpS >> eNumberOfSamples
    >> tmpS >> eNumberOfEventsInFile
    >> tmpS >> eTotalNumberOfFiles
    >> tmpS >> eDisMajThresholds
    >> tmpS >> eTriggerDelay
    >> tmpS >> eTriggerType;

  TString MonthNames[12] = {"Jan","Feb","Mar","Apr","May","Jun",
			    "Jul","Aug","Sep","Oct","Nov","Dec"};
  TString MonthName, WeekDay;
  Int_t Month, Day, Hour, Minute, Second, Year;
  Char_t ch;


  //informations in the .chcfg file
  //decide how many PMTs are to be generated

  while (!s.eof()) {
    s >> tmpS;
    if (strstr(tmpS,"channel")>0) {
      ArPMT *d = AddPMT();
      Int_t id,discthrs;
      Double_t fullscale,offset;
      Long_t coupling,bandwidth;
      TString chname;

      sscanf(tmpS,"channel%d",&id);
      s >> fullscale >> offset >> coupling >> bandwidth >> discthrs >> chname;
      d->SetID(id);
      d->SetFullScale(fullscale);
      d->SetOffset(offset);
      d->SetCoupling(coupling);
      d->SetBandWidth(bandwidth);
      d->SetDiscThrs(discthrs);
      d->SetName(chname);
      d->RawData.Set(eNumberOfSamples);
      d->Data.Set(eNumberOfSamples);
      d->DataError.Set(eNumberOfSamples);

    } else {
      WeekDay = tmpS;
      s >> MonthName >> Day >> Hour >> ch >> Minute >> ch >> Second >> Year;
      for (int k=0;k<12;k++) if (MonthName == MonthNames[k]) Month = k;
      time_t rawtime;
      time(&rawtime);
      struct tm *timeinfo;
      timeinfo = localtime(&rawtime);
      timeinfo->tm_sec   = Second;
      timeinfo->tm_min   = Minute;
      timeinfo->tm_hour  = Hour;
      timeinfo->tm_mday  = Day;
      timeinfo->tm_mon   = Month;
      timeinfo->tm_year  = Year-1900;
      //eAcquisitionTime = mktime(timeinfo);
      break;
    }
  }

  // create configuration for the virtual channel copying from the first PMT
  ArPMT *pmt0 = GetPMT(0);

  eVPMT->SetID(NPMTs()+1);
  eVPMT->SetFullScale(pmt0->FullScale());
  eVPMT->SetOffset(pmt0->Offset());
  eVPMT->SetCoupling(pmt0->Coupling());
  eVPMT->SetBandWidth(pmt0->BandWidth());
  eVPMT->SetDiscThrs(pmt0->DiscThrs());
  eVPMT->SetName("Virtual");
  eVPMT->SetType(pmt0->Type());
  eVPMT->Data.Set(pmt0->Data.GetSize());
  eVPMT->DataError.Set(pmt0->DataError.GetSize());
}



void ArRun :: SetCalibration(Int_t ndetectors)
{
  TString filename;
  if (NPMTs()==14) ArIO::MakeFileName(filename,ePath,-1,-1,"calibrations14.new.dat");
  else ArIO::MakeFileName(filename,ePath,-1,-1,"../calibrations.dat");
  SetCalibration(ndetectors,filename);
}



void ArRun :: SetCalibration(Int_t ndetectors, TString filename)
{
  if (ArDBLEVEL>=2) printf("Opening calibration file %s\n",filename.Data());

  ifstream s;
  s.open(filename.Data());

  TString tmpS;
  Int_t run,runp;
  Int_t runm=-1;
  Float_t time,timep,timem;

  TArrayD cal,calp,calm;
  cal.Set(ndetectors);
  calp.Set(ndetectors);
  calm.Set(ndetectors);

  while (!s.eof()) {
    s >> tmpS;
    if (strstr(tmpS,"Run")>0) {

      s >> run;
      s >> time;

      for (Int_t i=0;i<cal.GetSize();i++) s >> cal[i];

      if (run<=eRunNumber) {
	runm = run;
	timem = time;
	for (Int_t i=0;i<cal.GetSize();i++) calm[i] = cal[i];
      }

      if (run>=eRunNumber) {
	runp = run;
	timep = time;
	for (Int_t i=0;i<cal.GetSize();i++) calp[i] = cal[i];
	break;
      }
    }
  }

  if (run==eRunNumber||runm<0) {
    if (ArDBLEVEL>=2) printf("Taking calibration for run %d\n",run);
    eTime = time;
    for (int i=0;i<NPMTs();i++) {
      ArPMT *d = GetPMT(i);
      if (i<ndetectors) d->SetCalibration(cal[i]);
      else d->SetCalibration(1.);
    }
  } else {
    if (ArDBLEVEL>=2) {
      printf("Calibration from run %d is missing!\n",eRunNumber);
      printf("Taking calibration averaging between run %d and %d\n",runm,runp);
    }
    eTime = (timem+timep)/2.;
    for (int i=0;i<NPMTs();i++) {
      ArPMT *d = GetPMT(i);
      if (i<ndetectors) d->SetCalibration((calm[i]+calp[i])/2.);
      else d->SetCalibration(1.);
    }
  }
  
  s.close();
}


void ArRun :: SetCalibration(TArrayD cal)
{
  if (ArDBLEVEL>=2) printf("Setting calibration from a given array\n");

  Int_t ndetectors = cal.GetSize();
  if (ndetectors>NPMTs()) {
    if (ArDBLEVEL>=0) printf("SetCalibration Warning! The array size (%d) is bigger than number of PMTs (%d)\n",ndetectors,NPMTs());
    ndetectors = NPMTs();
  } else if (ndetectors<NPMTs())
    if (ArDBLEVEL>=0) printf("SetCalibration Warning! The array size (%d) is smaller than number of PMTs (%d)\n",ndetectors,NPMTs());

  for (int i=0;i<NPMTs();i++) {
    ArPMT *d = GetPMT(i);
    if (i<ndetectors) d->SetCalibration(cal[i]);
    else d->SetCalibration(1.);
  }
}



void ArRun :: SetDelay(TArrayI delay)
{
  if (ArDBLEVEL>=2) printf("Setting delay from a given array\n");

  Int_t ndetectors = delay.GetSize();
  if (ndetectors>NPMTs()) {
    if (ArDBLEVEL>=0) printf("SetDelay Warning! The array size (%d) is bigger than number of PMTs (%d)\n",ndetectors,NPMTs());
    ndetectors = NPMTs();
  } else if (ndetectors<NPMTs())
    if (ArDBLEVEL>=0) printf("SetDelay Warning! The array size (%d) is smaller than number of PMTs (%d)\n",ndetectors,NPMTs());

  for (int i=0;i<NPMTs();i++) {
    ArPMT *d = GetPMT(i);
    if (i<ndetectors) d->SetDelay(delay[i]);
    else d->SetDelay(0);
  }
}

void ArRun :: SetPedestal(TArrayD pedestal)
{
  if (ArDBLEVEL>=2) printf("Setting pedestal level from a given array\n");

  Int_t ndetectors = pedestal.GetSize();
  if (ndetectors>NPMTs()) {
    if (ArDBLEVEL>=0) printf("SetDelay Warning! The array size (%d) is bigger than number of PMTs (%d)\n",ndetectors,NPMTs());
    ndetectors = NPMTs();
  } else if (ndetectors<NPMTs())
    if (ArDBLEVEL>=0) printf("SetDelay Warning! The array size (%d) is smaller than number of PMTs (%d)\n",ndetectors,NPMTs());

  for (int i=0;i<NPMTs();i++) {
    ArPMT *d = GetPMT(i);
    if (i<ndetectors) d->SetPedestal(pedestal[i]);
    else d->SetPedestal(0);
  }
}

void ArRun :: PrintConfig(FILE *fp)
{
  ArIO::PrintColor(fp,"** DAQ Configuration:\n");

  fprintf(fp,"nsamples   %d\n", eNumberOfSamples);
  fprintf(fp,"nsegments  %d\n", eNumberOfEventsInFile);
  fprintf(fp,"nfiles     %d\n", eTotalNumberOfFiles);
  fprintf(fp,"majthr     %d\n", eDisMajThresholds);
  fprintf(fp,"pretrigger %d\n", eTriggerDelay);
  fprintf(fp,"tritype    %d\n", eTriggerType);

  for (int i=0;i<NPMTs();i++) {
    ArPMT *d = GetPMT(i);
    fprintf(fp,"channel%d\t%4.3lf\t\t%4.3lf\t\t%ld\t%ld\t%d\t%s\n",
	    d->ID(), d->FullScale(), d->Offset(), d->Coupling(), d->BandWidth(), d->DiscThrs(),(d->Name()).Data());
  }
}



void ArRun :: PrintRawData(FILE *fp)
{
  fprintf(fp,"# Event number %d\n",eCurrentEventNumber);
  fprintf(fp,"# Showing raw data as they come from DAQ:\n");
  for (int id=0;id<NPMTs();id++) {
    ArPMT *d = GetPMT(id);
    d->PrintRawData(fp);
  }
}



void ArRun :: PrintData(FILE *fp)
{
  fprintf(fp,"# Event number %d\n",eCurrentEventNumber);
  fprintf(fp,"# Showing data:\n");
  for (int id=0;id<NPMTs();id++) {
    ArPMT *d = GetPMT(id);
    d->PrintData(fp);
  }
  fprintf(fp,"# Virtual channel:\n");
  eVPMT->PrintData(fp);
}


void ArRun :: PrintData(Int_t level)
{
  char outfilename[100];
  sprintf(outfilename,"Run%d/run%d_file%d_event%d.dat",
	  eRunNumber,eRunNumber,eCurrentFileNumber,eCurrentEventNumber);
  FILE *fp;
  if ( (fp = fopen(outfilename,"w")) == 0 ) {
    printf("Error! File %s cannot be created\n",outfilename);
    return;
  }
  if (level==0) PrintRawData(fp);
  else PrintData(fp);
  fclose(fp);
}

void ArRun :: PrintCalibration(FILE *fp)
{
  fprintf(fp,"## Calibration of run %d for %d PMTs\n",eRunNumber,NPMTs());
  fprintf(fp,"ID) Name Calibration\n");
  for (int id=0;id<NPMTs();id++) {
    ArPMT *d = GetPMT(id);
    fprintf(fp,"%d) %s %f [nVs/pe]\n", d->ID(), d->Name().Data(), d->Calibration());
  }
}



void ArRun :: PrintDelay(FILE *fp)
{
  fprintf(fp,"## Delay of %d PMTs\n",NPMTs());
  fprintf(fp,"ID) Name Delay\n");
  for (int id=0;id<NPMTs();id++) {
    ArPMT *d = GetPMT(id);
    fprintf(fp,"%d) %s %d\n", d->ID(), d->Name().Data(), d->Delay());
  }
}


*/
