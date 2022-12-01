#define analysis_cxx
#include "analysis.h"
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TTree.h>
#include <TCanvas.h>
#include <string>
#include <iostream>
#include "TSystem.h"
#include "/eth/ethraid5/home/knikolic/LRAnalysis/include/ArRecoPMT.hh"

using namespace std;

void analysis::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L analysis.C
//      Root > analysis t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

   float top = 0;
   float bottom = 0;
   float total = 0;
   float ttr = 0;
   int npeaks = 0;
   int count =0;
   vector<double> peakIntegral;
   vector<int> peakTime;
   
   const double sampleTime = 4; //ns, 250 MHz sampling
   const int    nSamples   = 2000; //8 mus per event
   const double FULLSCALE_VOLTAGE  = 2.; //volt
   const int    NDIVISION_FULLSCALE = 4096; //2^12 , 12 bits
   const double VOLTAGE_PER_DIVISION = FULLSCALE_VOLTAGE/NDIVISION_FULLSCALE*1000;//in mV


   Long64_t nentries = fChain->GetEntries();
   TH1F* cr = new TH1F("cr","component ratio",100,0,1);
   TH1F* totyield = new TH1F("totyield","",1000,0,5000);
   TH1F* TTR = new TH1F("TTR","top to bottom ratio",100,0,1);
   TH2F* yieldTTR = new TH2F("yieldTTR","",1000,0,5000,500,0,1);
   TH1F* avpulse[kMaxPMT];// = new TH1F("avpulse","avpulse",nSamples,0,nSamples*sampleTime);

   if (fChain == 0) return;
   

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      fChain->GetEntry(ientry);
      count++;
      if(count%10000==0) cout << "Processed " << count << " events" << endl;
      cr->Fill(eCR);
      top = 0;
      bottom = 0;
      total = 0;
      ttr = 0;

      for (int k=0; k< NPMTs; k++) {
      /* do analysis on single PMT */
          cout << "peakIntegral size " << sizeof(PMT_ePeakIntegral) << endl;
	  avpulse[k] = new TH1F(Form("avpulse%i",k),"",nSamples,0,nSamples*sampleTime);
          for(int ii=0;ii<sizeof(PMT_ePeakIntegral);ii++){
          avpulse[k]->Fill(PMT_ePeakTimes->at(ii)*sampleTime,PMT_ePeakIntegral->at(ii)*VOLTAGE_PER_DIVISION);
           }
       }
      ttr = eSumPeakIntegralTop/eSumPeakIntegralTotal;
      TTR->Fill(ttr);
      totyield->Fill(eSumPeakIntegralTotal/100);
      yieldTTR->Fill(eSumPeakIntegralTotal/100,ttr);
     }
   
}
