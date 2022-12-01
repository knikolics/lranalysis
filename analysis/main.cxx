#include "TROOT.h"
#include "TStyle.h"
#include "TChain.h"
#include <TFile.h>
#include "Riostream.h"
#include "TSystem.h"
#include "analysis.h"
#include <string>
#include "/eth/ethraid5/home/knikolic/LRAnalysis/include/ArRecoPMT.hh"

using namespace std;
//-----------------------------
int main(int argc,char **argv) {
  if (argc != 2) {
    cout << "Give a ROOT file as input !";
    return 0;
  gSystem->Load("/home/knikolic/LRAnalysis/lib/ArDM.so");
  string output = "histosRun"+ (string)argv[1] + "_April2013.root";
  cout << "Create output file " << output << endl;
  TFile* myfile = new TFile(output.c_str(),"RECREATE");
  
  TChain* chain = new TChain("Data");
  //TString infile = "/eth/ethraid11/ArDM/april2013/analysis/Run"+ (TString)argv[1];
  //infile +=  + "/Run" + (TString)argv[1] + "-File.output.root";
  TString infile;
  infile +=  "Run" + (TString)argv[1] + "-File.output.root";
  cout << "Adding file " << infile << " to TChain" << endl;
  chain->Add(infile.Data());

  cout << "Entries in analysis chain: " << chain->GetEntries() << endl;
  analysis* ardm = new analysis(chain);
  ardm->Loop();

  myfile->Write();
  myfile->Close();

 
  return 0;
}

