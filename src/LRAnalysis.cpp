/*****************************************************
 ** LRAnalysis                                      **
 ** Analysis program of light readout data,         **
 ** developed for ArDM by Luca Scotto Lavina,       **
 ** starting from the work done by Vittorio Boccone **
 *****************************************************/

#define Version "LRAnalysis - 1.1 - 30/10/10"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>

#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TGraph.h>
#include <TEllipse.h>
#include <TMath.h>
#include <TSpectrum.h>
#include <TVirtualFFT.h>

#include "ArAnalysis.hh"
#include "ArRun.hh"
#include "ArIO.hh"
#include "ArOutput.hh"

#include "TStopwatch.h"

TString   Path;
Int_t     RunNumber;  

const int LRAnalysis_verbose=0;
//const int RUNTIME_MONITOR = 1;
//#define RUNTIME_MONITOR 1

void Analysis(void)
{

  //cout<<"RUNTIME_MONITOR "<<RUNTIME_MONITOR<<endl; getchar();

  TStopwatch masterTimer;
  masterTimer.Start();

  TStopwatch timer;
  timer.Start();


  ArDBLEVEL = 4;

  ArAnalysis *analysis = new ArAnalysis();


  /*********************
   ** Opening the run **
   *********************/

  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 1 ... "<<endl;

  ArRun *run = new ArRun(Path.Data(),RunNumber);
  run->LoadConfig();     // Loading chcfg file
  //run->PrintConfig();

  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 1a ... "<<endl;

  analysis->SetRun(run);

  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 2 ... "<<endl;

  /************************************************
   ** Opening the configuration for the analysis **
   ************************************************/

  ArConf = new ArConfiguration(Path.Data(),RunNumber);

  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 2a ... "<<endl;

  ArConf->Load();  // Loading the cfg file

  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 2b ... "<<endl;

  analysis->ApplyConfiguration();

  //run->Print();

  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 3 ... "<<endl;

  ArConf->Print();
  //run->PrintCalibration();
  //run->PrintDelay();

  /************************
   ** Opening the output **
   ************************/

  TString filename;
  ArIO::MakeFileName(filename,".",RunNumber,-1,"output.root");
  ArOutput *output = new ArOutput(filename);
  output->Open();
  //  output->FillHeader(run);

  /******************************
   ** Creating the mean traces **
   ******************************/

  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 4 ... "<<endl;

  ArRun *meanrun = new ArRun(Path.Data(),RunNumber);
  meanrun->LoadConfig();
  analysis->SetMeanTrace(meanrun);
  analysis->ResetMeanTrace();

  /*************************
   ** Looping over events **
   *************************/

  Int_t processedevents = 0;


  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 5 ... ArConf->EventsInTheMask "
			     <<ArConf->EventsInTheMask
			     <<endl;


  if(RUNTIME_MONITOR) cout<<"start processing ... "<<endl;


  if (ArConf->EventsInTheMask) {
    //    ArConf->PrintMask();

    if (ArConf->LastEvent<0) ArConf->LastEvent = ArConf->EventsInTheMask - 1;

    for (Long64_t i=ArConf->FirstEvent;i<=ArConf->LastEvent;i++) {

      //cout<<"in LRAnalysis.cpp, Analysis() .... event "<<i<<"\t blah 1 ..."<<endl;

      if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 6a ... "<<endl;

      //if (run->LoadEvent(ArConf->FileList[i],ArConf->EventList[i])<0) break;


      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling run->LoadEvent(..) ... "<<endl;


      //if (run->LoadEvent(ArConf->EventList[i])) break;
      if (run->LoadEvent(ArConf->EventList[i])<0) continue;

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished run->LoadEvent(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }

      processedevents++;

      if(!(i%1000)) fprintf(stderr," > %lld/%lld\r",i-ArConf->FirstEvent,ArConf->LastEvent - ArConf->FirstEvent + 1);


      if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 6b ... "<<endl;

      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->ConvertData(..) ... "<<endl;

      analysis->ConvertData();

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->ConvertData(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }


      if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 6c ... "<<endl;


      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->FindPedestal(..) ... "<<endl;

      analysis->FindPedestal();

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->FindPedestal(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }

      if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 6d... "<<endl;



      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->CorrectData(..) ... "<<endl;

      analysis->CorrectData();

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->CorrectData(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }


      if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 6e ... "<<endl;


      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->FindSignal(..) ... "<<endl;

      if (ArConf->FakeSignalTime<0) 
	analysis->FindSignal(); // also Find Peaks luillo
      else analysis->SetFakeSignal(ArConf->FakeSignalTime);

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->FindSignal(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }

      // analysis->FindSignalWithOfflineTrigger(0.020,2);
      // if (!analysis->Triggered()) continue;

      // analysis->FilterBackground();

      if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 6f ... "<<endl;



      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->EvaluateIntegral(..) ... "<<endl;

      analysis->EvaluateIntegral();

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->EvaluateIntegral(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }



      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->FillVirtualChannel(..) ... "<<endl;

      analysis->FillVirtualChannel();


      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->FillVirtualChannel(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }

/*
      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->AnalyseVirtualChannel(..) ... "<<endl;

      analysis->AnalyseVirtualChannel();


      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->AnalyseVirtualChannel(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }
*/

      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->AnalyseVirtualChannel(..) ... "<<endl;

      if (ArConf->EvaluateChannelsFromVirtualChannel) {
	analysis->EvaluateChannelsFromVirtualChannel();
	analysis->AnalyseVirtualChannel();
      }


      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->AnalyseVirtualChannel(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }

      // analysis->ShiftVirtualChannel(1000);


      if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 6g ... "<<endl;


      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->FillMeanTrace(..) ... "<<endl;

      analysis->FillMeanTrace();

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->FillMeanTrace(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }

      output->Cd();
      output->FillData(run);

      // run->PrintData(1);
      // run->PrintRawData();
      // run->PrintData();
      // run->Print();
      // analysis->Print();
      // analysis->ClearHistograms();

      if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 6h ... "<<endl;

    }

  } else {

    if (ArConf->LastEvent<0) ArConf->LastEvent = run->TotalNumberOfFiles() * run->NumberOfEventsInFile() - 1;

    
    if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7 ...  "
			       <<"FirstEvent "<<ArConf->FirstEvent
			       <<"\t LastEvent "<<ArConf->LastEvent
			       <<endl;

    for (Long64_t i=ArConf->FirstEvent;i<=ArConf->LastEvent-1;i++)
      {

	//cout<<"in LRAnalysis.cpp, Analysis() .... event "<<i<<"\t blah 2 ..."<<endl;

	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7.0 ... event "<<i<<endl;

      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling run->LoadEvent(..) ... "<<endl;


      //if (run->LoadEvent(i)<0) break;
      if (run->LoadEvent(i)<0) continue;;

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished run->LoadEvent(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }


	processedevents++;
	
	if(!(i%1000)) fprintf(stderr," > %lld/%lld\r",i-ArConf->FirstEvent,ArConf->LastEvent - ArConf->FirstEvent + 1);


	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7a ... "<<endl;

	//	run->PrintRawData();

      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->ConvertData(..) ... "<<endl;

	analysis->ConvertData();

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->ConvertData(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }


	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7b ... "<<endl;



      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->FindPedestal(..) ... "<<endl;

	analysis->FindPedestal();

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->FindPedestal(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }


	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7c ... "<<endl;


      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->CorrectData(..) ... "<<endl;

	analysis->CorrectData();

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->CorrectData(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }


	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7d ... "<<endl;


      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->FindSignal(..) ... "<<endl;

	if (ArConf->FakeSignalTime<0) 
	  analysis->FindSignal();
	else analysis->SetFakeSignal(ArConf->FakeSignalTime);

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->FindSignal(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }

	// analysis->FindSignalWithOfflineTrigger(0.020,2);
	// if (!analysis->Triggered()) continue;

	// analysis->FilterBackground();

	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7e ... "<<endl;


      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->EvaluateIntegral(..) ... "<<endl;

	analysis->EvaluateIntegral();


      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->EvaluateIntegral(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }


	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7f ... "<<endl;


      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->FillVirtualChannel(..) ... "<<endl;

	analysis->FillVirtualChannel();

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->FillVirtualChannel(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }


	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7g ... "<<endl;


      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->AnalyseVirtualChannel(..) ... "<<endl;

	analysis->AnalyseVirtualChannel();


      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->AnalyseVirtualChannel(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }



	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7h ... "<<endl;


      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->AnalyseVirtualChannel(..) ... "<<endl;

	if (ArConf->EvaluateChannelsFromVirtualChannel) {
	  analysis->EvaluateChannelsFromVirtualChannel();
	  analysis->AnalyseVirtualChannel();
	}

      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->AnalyseVirtualChannel(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }


	// analysis->ShiftVirtualChannel(1000);


	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7i ... "<<endl;


      if(RUNTIME_MONITOR) cout<<"event "<<i<<"\t calling analysis->FillMeanTrace(..) ... "<<endl;

 	analysis->FillMeanTrace();


      if(RUNTIME_MONITOR){
	timer.Stop();
	cout<<"event "<<i<<"\t finished analysis->FillMeanTrace(..) ... \t\t\t\t"<<timer.RealTime()<<endl;
	timer.Start();
      }


      if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ..... output = "<<output<<endl;
      output->Cd();

	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7j ... "<<endl;

 	output->Cd();


	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7k ... "<<endl;

 	output->FillData(run);

	// run->PrintData(1);
	// run->PrintRawData();
 	// run->PrintData();
	// run->Print();
	// analysis->Print();
	// analysis->ClearHistograms();	

	if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 7l ... "<<endl;

      }

  }

  
  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 8 ... "<<endl;

  analysis->ScaleMeanTrace( (Double_t)(processedevents) );
  
  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 9 ... "<<endl;

  output->FillMeanTrace(analysis);
  
//   FILE *fp=fopen("example.dat","w");
//   analysis->PrintMeanTrace(fp);
//   fclose(fp);
  

  
  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 10 ... "<<endl;


  /***************
   ** Game over **
   ***************/

  //run->CloseFile();

  output->Cd();
  output->Close();
  
  if(LRAnalysis_verbose) cout<<"in LRAnalysis.cpp, Analysis() ... position 11 ... "<<endl;

  //  analysis->ClearHistograms();
  timer.Stop();
  if(RUNTIME_MONITOR) cout<<"real time "<<timer.RealTime()<<endl;

  masterTimer.Stop();
  cout<<"CPU time "<<masterTimer.CpuTime()<<"\t real time "<<masterTimer.RealTime()<<endl;
}


int main(int argc, char **argv)
{
  char *tmp;

  if (argc==3) {
    Path         = argv[1];
    RunNumber    = strtol(argv[2], &tmp,0);
  } else if (argc==2) {
    Path         = argv[1];
    RunNumber    = -1;
  } else {
    fprintf(stderr,"%s\n",Version);
    fprintf(stderr,"SYNOPSYS\n");
    fprintf(stderr,"  %s path runnumber\n",argv[0]);
    fprintf(stderr,"  %s fileprefix\n\n",argv[0]);
    fprintf(stderr,"DESCRIPTION\n");
    fprintf(stderr,"  path         path where run directories are stored\n");
    fprintf(stderr,"  runnumber    run number to be analyzed\n");
    fprintf(stderr,"  fileprefix   file prefix to build the sequence <fileprefix><eventnumber>.root\n");
    fprintf(stderr,"\nWrong synopsis!\n");
    return(-1);
  }

  Analysis();

  return(0);
}
