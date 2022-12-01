#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <TROOT.h>
#include <TSystem.h>
#include <TTree.h>
#include <TFile.h>
#include <TClonesArray.h>

#include "ArOutput.hh"

#include "ArRun.hh"
#include "ArRecoPMT.hh"


ClassImp(ArOutput)


ArOutput :: ArOutput(const char *filename)
{
  eFileName = filename;
  eFile = 0;
  eTreeData = 0;
  //  eTreeHeader = 0;

  eRecoVPMT=0;
}


ArOutput :: ~ArOutput()
{

}


Int_t ArOutput :: Open()
{
  eFile = new TFile(eFileName,"RECREATE");

  eTreeData = new TTree("Data","Uncut Data");
  eTreeMeanTrace = new TTree("MeanTrace","Mean trace");
  //  eTreeHeader = new TTree("Header","Header");
  
  eRecoPMTs = new TClonesArray("ArRecoPMT");

  // Data tree branches definition

  //test (khoi nguyen)
  //retrieve unixTime of event
  eTreeData->Branch("unixTime",&eUnixTime,"unixTime/I");
  eTreeData->Branch("eSumPeakIntegralTop",&eSumPeakIntegralTop,"eSumPeakIntegralTop/D");
  eTreeData->Branch("eSumPeakIntegralBottom",&eSumPeakIntegralBottom,"eSumPeakIntegralBottom/D");
  eTreeData->Branch("eSumPeakIntegralTotal",&eSumPeakIntegralTotal,"eSumPeakIntegralTotal/D");
  //end test



  eTreeData->Branch("RunNumber",&eRunNumber,"RunNumber/I");
  //eTreeData->Branch("FileNumber",&eFileNumber,"FileNumber/I");
  eTreeData->Branch("EventNumber",&eEventNumber,"EventNumber/I");
  eTreeData->Branch("BoardEventNumber",&eBoardEventNumber,"BoardEventNumber[4]/I"); //4 = NBOARD
  eTreeData->Branch("NPMTs",&eNPMTs,"NPMTs/I");
  //eTreeData->Branch("AcquisitionTime",&eAcquisitionTime,"AcquisitionTime/I");
  eTreeData->Branch("PMT", &eRecoPMTs);
  eTreeData->Branch("VPMT", &eRecoVPMT);
  //eTreeData->Branch("TTT", &eTTT,"TTT[4]/I");//4=NBOARD //I = signed integer, 32 bits
  eTreeData->Branch("TTT", &eTTT,"TTT[4]/i");//4=NBOARD   //i = unsigned integer, 32 bits
  //eTreeData->Branch("delay", eDelay,"delay[16]/I");
  //eTreeData->Branch("delay", eDelay,"delay[24]/I");
  //eTreeData->Branch("delay", eDelay,"delay[NPMTs]/I"); //NPMTs= NBOARD*NCHANNEL_IN_ONE_BOARD_STORED_IN_RAW_DATA_FILE
  eTreeData->Branch("nSamples", &eNSamples,"nSamples/I");

  // Mean trace tree branches definition
  eTreeMeanTrace->Branch("PMT",&ePMT,"PMT/I");
  eTreeMeanTrace->Branch("Time",&eTime,"Time/I");
  eTreeMeanTrace->Branch("Data",&eData,"Data/D");
  eTreeMeanTrace->Branch("DataError",&eDataError,"DataError/D");

  // Header tree branches definition
  // TODO
}


Int_t ArOutput :: Cd()
{
  if (eFile) eFile->cd();
  return(0);
}


Int_t ArOutput :: FillData(ArRun *run)
{


  //test (khoi nguyen)
  //retrieve unixTime of event
  eUnixTime = run->unixTime(); 
  for(int i=0;i<NBOARD;i++) eBoardEventNumber[i] = run->eBoardEventNumber[i];
  eSumPeakIntegralTop = run->eSumPeakIntegralTop;
  eSumPeakIntegralBottom = run->eSumPeakIntegralBottom;
  eSumPeakIntegralTotal = eSumPeakIntegralTop+eSumPeakIntegralBottom;
  //end test


  eRunNumber = run->RunNumber();
  //eFileNumber = run->CurrentFileNumber();
  eEventNumber = run->CurrentEventNumber();
  eNPMTs = run->NPMTs();
  //eAcquisitionTime = run->AcquisitionTime();
  //eTTT = run->TTT();
  for(int i=0;i<NBOARD;i++) eTTT[i]=run->TTT()[i];

  //change from 16 to ???
  //for (Int_t i=0;i<16;i++) eDelay[i] = run->Delay(i);
  //for (Int_t i=0;i<sizeof(eDelay)/sizeof(Int_t);i++) eDelay[i] = run->Delay(i);
  eNSamples = run->NSamples();

  eRecoPMTs->Clear("C");
  for (Int_t i=0;i<run->NPMTs();i++)
    new ((*eRecoPMTs)[i]) ArRecoPMT(run->GetPMT(i));
  
  if (eRecoVPMT) { delete eRecoVPMT; eRecoVPMT=0; }
  eRecoVPMT = new ArRecoPMT(run->GetVPMT());

  if (eTreeData) eTreeData->Fill();
  
  return(0);
}

Int_t ArOutput :: FillMeanTrace(ArAnalysis *analysis)
{
  ArRun *meanrun = analysis->MeanRun();

  for (Int_t id=0;id<meanrun->NPMTs();id++) {
    ArPMT *meand = meanrun->GetPMT(id);
    ePMT = meand->ID();
    for (eTime=0;eTime<meand->Data.GetSize();eTime++) {
      eData = meand->Data[eTime];
      eDataError = meand->DataError[eTime];
      eTreeMeanTrace->Fill();      
    }
  }

  ArPMT *meand = meanrun->GetVPMT();
  ePMT = meand->ID();
  for (eTime=0;eTime<meand->Data.GetSize();eTime++) {
    eData = meand->Data[eTime];
    eDataError = meand->DataError[eTime];
    eTreeMeanTrace->Fill();
  }

  return(0);
}

// Int_t ArOutput :: FillHeader(ArRun *run)
// {
//   eRunNumber = run->RunNumber();
//   eFileNumber = run->CurrentFileNumber();
//   eEventNumber = run->CurrentEventNumber();
//   eNPMTs = run->NPMTs();

//   if (eTreeHeader) eTreeHeader->Fill();

//   return(0);
// }


Int_t ArOutput :: Close()
{
  eTreeData->Write();
  eTreeMeanTrace->Write();
  //  eTreeHeader->Write();
  if (eFile) eFile->Close();

  return(0);
}
