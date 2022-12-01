/*****************************************************
 ** LookAt                                          **
 ** Display program of light readout data,          **
 ** developed for ArDM by Luca Scotto Lavina.       **
 ** It also make a bit of basic analysis            **
 *****************************************************/

#define Version "LookAt - 1.1 - 30/10/10"

#ifndef __CINT__
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
#include "ArDisplay.hh"
#endif

#ifdef __CINT__
Int_t a = gSystem->Load("libTree.so");
Int_t b = gSystem->Load("../lib/ArDM.so");
#endif

TClonesArray *graph = new TClonesArray("TGraph",20);
TClonesArray *frame = new TClonesArray("TH2F",20);

int autozoom = 1;
Double_t margin = 1.2;


Int_t LookAt(TString Path, Int_t RunNumber)
{

  ArDBLEVEL = 4;

  ArAnalysis *analysis = new ArAnalysis();


  /*********************
   ** Opening the run **
   *********************/

  ArRun *run = new ArRun(Path.Data(),RunNumber);
  run->LoadConfig();     // Loading chcfg file
  //run->PrintConfig();
  //analysis->SetRun(run);


  /************************************************
   ** Opening the configuration for the analysis **
   ************************************************/

  ArConf = new ArConfiguration(Path.Data(),RunNumber);
  ArConf->Load();  // Loading the cfg file
  analysis->ApplyConfiguration();

  run->Print();

  ArConf->Print();
  //run->PrintCalibration();
  //run->PrintDelay();

  /******************************
   ** Creating the mean traces **
   ******************************/

  ArRun *meanrun = new ArRun(Path.Data(),RunNumber);
  meanrun->LoadConfig();
  analysis->SetMeanTrace(meanrun);
  analysis->ResetMeanTrace();

  /*********************
   ** Opening display **
   *********************/

  int argc;
  char **argv;
  //  TApplication theApp("App",&argc,argv);
  TApplication theApp("App",NULL,0);
  ArDisplay *display = new ArDisplay(gClient->GetRoot(),200,200,analysis,run);
  theApp.Run();



  /***************
   ** Game over **
   ***************/

  //run->CloseFile();

  return(0);
}

#ifndef __CINT__
int main(int argc, char **argv)
{
  char *tmp;

  TString   Path;
  Int_t     RunNumber;

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
    fprintf(stderr,"  runnumber    run number to be opened\n");
    fprintf(stderr,"  fileprefix   file prefix to build the sequence <fileprefix><eventnumber>.root\n");
    fprintf(stderr,"\nWrong synopsis!\n");
    return(-1);
  }

  return(LookAt(Path,RunNumber));
}
#endif
