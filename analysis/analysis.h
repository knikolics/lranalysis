//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jun 25 17:11:30 2013 by ROOT version 5.34/03
// from TTree Data/Uncut Data
// found on file: Run0050-File.output.root
//////////////////////////////////////////////////////////

#ifndef analysis_h
#define analysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TObject.h>

using namespace std;

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxPMT = 24;

class analysis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           unixTime;
   Double_t        eSumPeakIntegralTop;
   Double_t        eSumPeakIntegralBottom;
   Double_t        eSumPeakIntegralTotal;
   Int_t           RunNumber;
   Int_t           EventNumber;
   Int_t           BoardEventNumber[4];
   Int_t           NPMTs;
   Int_t           PMT_;
   UInt_t          PMT_fUniqueID[kMaxPMT];   //[PMT_]
   UInt_t          PMT_fBits[kMaxPMT];   //[PMT_]
   Int_t           PMT_eID[kMaxPMT];   //[PMT_]
   TString         PMT_eName[kMaxPMT];
   TString         PMT_eType[kMaxPMT];
   Double_t        PMT_eCalibration[kMaxPMT];   //[PMT_]
   Int_t           PMT_eDelay[kMaxPMT];   //[PMT_]
   Double_t        PMT_ePedestal[kMaxPMT];   //[PMT_]
   Int_t           PMT_ePedTimeMin[kMaxPMT];   //[PMT_]
   Int_t           PMT_ePedTimeMax[kMaxPMT];   //[PMT_]
   Double_t        PMT_ePedMin[kMaxPMT];   //[PMT_]
   Double_t        PMT_ePedMax[kMaxPMT];   //[PMT_]
   Double_t        PMT_ePedMean[kMaxPMT];   //[PMT_]
   Double_t        PMT_ePedSigma[kMaxPMT];   //[PMT_]
   Double_t        PMT_ePedChi2[kMaxPMT];   //[PMT_]
   Int_t           PMT_ePedNpe[kMaxPMT];   //[PMT_]
   Double_t        PMT_ePedIntegral[kMaxPMT];   //[PMT_]
   Double_t        PMT_ePedIntegralPe[kMaxPMT];   //[PMT_]
   Int_t           PMT_ePedTimePe[kMaxPMT];   //[PMT_]
   Int_t           PMT_ePeakStartTime[kMaxPMT];   //[PMT_]
   Int_t           PMT_ePeakTime[kMaxPMT];   //[PMT_]
   Double_t        PMT_ePeakValue[kMaxPMT];   //[PMT_]
   Int_t           PMT_ePeakIsSaturated[kMaxPMT];   //[PMT_]
   Int_t           PMT_eNPeaks[kMaxPMT];   //[PMT_]
   Int_t           PMT_eNpe[kMaxPMT];   //[PMT_]
   Double_t        PMT_eIntegral[kMaxPMT];   //[PMT_]
   Double_t        PMT_eIntegralPe[kMaxPMT];   //[PMT_]
   Double_t        PMT_eIntegralPeS[kMaxPMT];   //[PMT_]
   Double_t        PMT_eTimePe[kMaxPMT];   //[PMT_]
   Double_t        PMT_eIntegralSinglet0[kMaxPMT];   //[PMT_]
   Double_t        PMT_eIntegralSinglet1[kMaxPMT];   //[PMT_]
   Double_t        PMT_eCR[kMaxPMT];   //[PMT_]
   Double_t        PMT_eCRPe[kMaxPMT];   //[PMT_]
   Double_t        PMT_eCRPeS[kMaxPMT];   //[PMT_]
   vector<int>     PMT_ePeakTimes[kMaxPMT];
   vector<double>  PMT_ePeakIntegral[kMaxPMT];
   vector<double>  PMT_ePeakAmplitude[kMaxPMT];
   Double_t        PMT_eSumPeakIntegralPMT[kMaxPMT];   //[PMT_]
 //ArRecoPMT       *VPMT;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   Int_t           eID;
   TString         eName;
   TString         eType;
   Double_t        eCalibration;
   Int_t           eDelay;
   Double_t        ePedestal;
   Int_t           ePedTimeMin;
   Int_t           ePedTimeMax;
   Double_t        ePedMin;
   Double_t        ePedMax;
   Double_t        ePedMean;
   Double_t        ePedSigma;
   Double_t        ePedChi2;
   Int_t           ePedNpe;
   Double_t        ePedIntegral;
   Double_t        ePedIntegralPe;
   Int_t           ePedTimePe;
   Int_t           ePeakStartTime;
   Int_t           ePeakTime;
   Double_t        ePeakValue;
   Int_t           ePeakIsSaturated;
   Int_t           eNPeaks;
   Int_t           eNpe;
   Double_t        eIntegral;
   Double_t        eIntegralPe;
   Double_t        eIntegralPeS;
   Double_t        eTimePe;
   Double_t        eIntegralSinglet0;
   Double_t        eIntegralSinglet1;
   Double_t        eCR;
   Double_t        eCRPe;
   Double_t        eCRPeS;
   vector<int>     ePeakTimes;
   vector<double>  ePeakIntegral;
   vector<double>  ePeakAmplitude;
   Double_t        eSumPeakIntegralPMT;
   UInt_t          TTT[4];
   Int_t           nSamples;

   // List of branches
   TBranch        *b_unixTime;   //!
   TBranch        *b_eSumPeakIntegralTop;   //!
   TBranch        *b_eSumPeakIntegralBottom;   //!
   TBranch        *b_eSumPeakIntegralTotal;   //!
   TBranch        *b_RunNumber;   //!
   TBranch        *b_EventNumber;   //!
   TBranch        *b_BoardEventNumber;   //!
   TBranch        *b_NPMTs;   //!
   TBranch        *b_PMT_;   //!
   TBranch        *b_PMT_fUniqueID;   //!
   TBranch        *b_PMT_fBits;   //!
   TBranch        *b_PMT_eID;   //!
   TBranch        *b_PMT_eName;   //!
   TBranch        *b_PMT_eType;   //!
   TBranch        *b_PMT_eCalibration;   //!
   TBranch        *b_PMT_eDelay;   //!
   TBranch        *b_PMT_ePedestal;   //!
   TBranch        *b_PMT_ePedTimeMin;   //!
   TBranch        *b_PMT_ePedTimeMax;   //!
   TBranch        *b_PMT_ePedMin;   //!
   TBranch        *b_PMT_ePedMax;   //!
   TBranch        *b_PMT_ePedMean;   //!
   TBranch        *b_PMT_ePedSigma;   //!
   TBranch        *b_PMT_ePedChi2;   //!
   TBranch        *b_PMT_ePedNpe;   //!
   TBranch        *b_PMT_ePedIntegral;   //!
   TBranch        *b_PMT_ePedIntegralPe;   //!
   TBranch        *b_PMT_ePedTimePe;   //!
   TBranch        *b_PMT_ePeakStartTime;   //!
   TBranch        *b_PMT_ePeakTime;   //!
   TBranch        *b_PMT_ePeakValue;   //!
   TBranch        *b_PMT_ePeakIsSaturated;   //!
   TBranch        *b_PMT_eNPeaks;   //!
   TBranch        *b_PMT_eNpe;   //!
   TBranch        *b_PMT_eIntegral;   //!
   TBranch        *b_PMT_eIntegralPe;   //!
   TBranch        *b_PMT_eIntegralPeS;   //!
   TBranch        *b_PMT_eTimePe;   //!
   TBranch        *b_PMT_eIntegralSinglet0;   //!
   TBranch        *b_PMT_eIntegralSinglet1;   //!
   TBranch        *b_PMT_eCR;   //!
   TBranch        *b_PMT_eCRPe;   //!
   TBranch        *b_PMT_eCRPeS;   //!
   TBranch        *b_PMT_ePeakTimes;   //!
   TBranch        *b_PMT_ePeakIntegral;   //!
   TBranch        *b_PMT_ePeakAmplitude;   //!
   TBranch        *b_PMT_eSumPeakIntegralPMT;   //!
   TBranch        *b_VPMT_fUniqueID;   //!
   TBranch        *b_VPMT_fBits;   //!
   TBranch        *b_VPMT_eID;   //!
   TBranch        *b_VPMT_eName;   //!
   TBranch        *b_VPMT_eType;   //!
   TBranch        *b_VPMT_eCalibration;   //!
   TBranch        *b_VPMT_eDelay;   //!
   TBranch        *b_VPMT_ePedestal;   //!
   TBranch        *b_VPMT_ePedTimeMin;   //!
   TBranch        *b_VPMT_ePedTimeMax;   //!
   TBranch        *b_VPMT_ePedMin;   //!
   TBranch        *b_VPMT_ePedMax;   //!
   TBranch        *b_VPMT_ePedMean;   //!
   TBranch        *b_VPMT_ePedSigma;   //!
   TBranch        *b_VPMT_ePedChi2;   //!
   TBranch        *b_VPMT_ePedNpe;   //!
   TBranch        *b_VPMT_ePedIntegral;   //!
   TBranch        *b_VPMT_ePedIntegralPe;   //!
   TBranch        *b_VPMT_ePedTimePe;   //!
   TBranch        *b_VPMT_ePeakStartTime;   //!
   TBranch        *b_VPMT_ePeakTime;   //!
   TBranch        *b_VPMT_ePeakValue;   //!
   TBranch        *b_VPMT_ePeakIsSaturated;   //!
   TBranch        *b_VPMT_eNPeaks;   //!
   TBranch        *b_VPMT_eNpe;   //!
   TBranch        *b_VPMT_eIntegral;   //!
   TBranch        *b_VPMT_eIntegralPe;   //!
   TBranch        *b_VPMT_eIntegralPeS;   //!
   TBranch        *b_VPMT_eTimePe;   //!
   TBranch        *b_VPMT_eIntegralSinglet0;   //!
   TBranch        *b_VPMT_eIntegralSinglet1;   //!
   TBranch        *b_VPMT_eCR;   //!
   TBranch        *b_VPMT_eCRPe;   //!
   TBranch        *b_VPMT_eCRPeS;   //!
   TBranch        *b_VPMT_ePeakTimes;   //!
   TBranch        *b_VPMT_ePeakIntegral;   //!
   TBranch        *b_VPMT_ePeakAmplitude;   //!
   TBranch        *b_VPMT_eSumPeakIntegralPMT;   //!
   TBranch        *b_TTT;   //!
   TBranch        *b_nSamples;   //!

   analysis(TTree *tree=0);
   virtual ~analysis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef analysis_cxx
analysis::analysis(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Run0050-File.output.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("Run0050-File.output.root");
      }
      f->GetObject("Data",tree);

   }
   Init(tree);
}

analysis::~analysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t analysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t analysis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void analysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("unixTime", &unixTime, &b_unixTime);
   fChain->SetBranchAddress("eSumPeakIntegralTop", &eSumPeakIntegralTop, &b_eSumPeakIntegralTop);
   fChain->SetBranchAddress("eSumPeakIntegralBottom", &eSumPeakIntegralBottom, &b_eSumPeakIntegralBottom);
   fChain->SetBranchAddress("eSumPeakIntegralTotal", &eSumPeakIntegralTotal, &b_eSumPeakIntegralTotal);
   fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
   fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
   fChain->SetBranchAddress("BoardEventNumber", BoardEventNumber, &b_BoardEventNumber);
   fChain->SetBranchAddress("NPMTs", &NPMTs, &b_NPMTs);
   fChain->SetBranchAddress("PMT", &PMT_, &b_PMT_);
   fChain->SetBranchAddress("PMT.fUniqueID", PMT_fUniqueID, &b_PMT_fUniqueID);
   fChain->SetBranchAddress("PMT.fBits", PMT_fBits, &b_PMT_fBits);
   fChain->SetBranchAddress("PMT.eID", PMT_eID, &b_PMT_eID);
   fChain->SetBranchAddress("PMT.eName", PMT_eName, &b_PMT_eName);
   fChain->SetBranchAddress("PMT.eType", PMT_eType, &b_PMT_eType);
   fChain->SetBranchAddress("PMT.eCalibration", PMT_eCalibration, &b_PMT_eCalibration);
   fChain->SetBranchAddress("PMT.eDelay", PMT_eDelay, &b_PMT_eDelay);
   fChain->SetBranchAddress("PMT.ePedestal", PMT_ePedestal, &b_PMT_ePedestal);
   fChain->SetBranchAddress("PMT.ePedTimeMin", PMT_ePedTimeMin, &b_PMT_ePedTimeMin);
   fChain->SetBranchAddress("PMT.ePedTimeMax", PMT_ePedTimeMax, &b_PMT_ePedTimeMax);
   fChain->SetBranchAddress("PMT.ePedMin", PMT_ePedMin, &b_PMT_ePedMin);
   fChain->SetBranchAddress("PMT.ePedMax", PMT_ePedMax, &b_PMT_ePedMax);
   fChain->SetBranchAddress("PMT.ePedMean", PMT_ePedMean, &b_PMT_ePedMean);
   fChain->SetBranchAddress("PMT.ePedSigma", PMT_ePedSigma, &b_PMT_ePedSigma);
   fChain->SetBranchAddress("PMT.ePedChi2", PMT_ePedChi2, &b_PMT_ePedChi2);
   fChain->SetBranchAddress("PMT.ePedNpe", PMT_ePedNpe, &b_PMT_ePedNpe);
   fChain->SetBranchAddress("PMT.ePedIntegral", PMT_ePedIntegral, &b_PMT_ePedIntegral);
   fChain->SetBranchAddress("PMT.ePedIntegralPe", PMT_ePedIntegralPe, &b_PMT_ePedIntegralPe);
   fChain->SetBranchAddress("PMT.ePedTimePe", PMT_ePedTimePe, &b_PMT_ePedTimePe);
   fChain->SetBranchAddress("PMT.ePeakStartTime", PMT_ePeakStartTime, &b_PMT_ePeakStartTime);
   fChain->SetBranchAddress("PMT.ePeakTime", PMT_ePeakTime, &b_PMT_ePeakTime);
   fChain->SetBranchAddress("PMT.ePeakValue", PMT_ePeakValue, &b_PMT_ePeakValue);
   fChain->SetBranchAddress("PMT.ePeakIsSaturated", PMT_ePeakIsSaturated, &b_PMT_ePeakIsSaturated);
   fChain->SetBranchAddress("PMT.eNPeaks", PMT_eNPeaks, &b_PMT_eNPeaks);
   fChain->SetBranchAddress("PMT.eNpe", PMT_eNpe, &b_PMT_eNpe);
   fChain->SetBranchAddress("PMT.eIntegral", PMT_eIntegral, &b_PMT_eIntegral);
   fChain->SetBranchAddress("PMT.eIntegralPe", PMT_eIntegralPe, &b_PMT_eIntegralPe);
   fChain->SetBranchAddress("PMT.eIntegralPeS", PMT_eIntegralPeS, &b_PMT_eIntegralPeS);
   fChain->SetBranchAddress("PMT.eTimePe", PMT_eTimePe, &b_PMT_eTimePe);
   fChain->SetBranchAddress("PMT.eIntegralSinglet0", PMT_eIntegralSinglet0, &b_PMT_eIntegralSinglet0);
   fChain->SetBranchAddress("PMT.eIntegralSinglet1", PMT_eIntegralSinglet1, &b_PMT_eIntegralSinglet1);
   fChain->SetBranchAddress("PMT.eCR", PMT_eCR, &b_PMT_eCR);
   fChain->SetBranchAddress("PMT.eCRPe", PMT_eCRPe, &b_PMT_eCRPe);
   fChain->SetBranchAddress("PMT.eCRPeS", PMT_eCRPeS, &b_PMT_eCRPeS);
   fChain->SetBranchAddress("PMT.ePeakTimes", PMT_ePeakTimes, &b_PMT_ePeakTimes);
   fChain->SetBranchAddress("PMT.ePeakIntegral", PMT_ePeakIntegral, &b_PMT_ePeakIntegral);
   fChain->SetBranchAddress("PMT.ePeakAmplitude", PMT_ePeakAmplitude, &b_PMT_ePeakAmplitude);
   fChain->SetBranchAddress("PMT.eSumPeakIntegralPMT", PMT_eSumPeakIntegralPMT, &b_PMT_eSumPeakIntegralPMT);
   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_VPMT_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_VPMT_fBits);
   fChain->SetBranchAddress("eID", &eID, &b_VPMT_eID);
   fChain->SetBranchAddress("eName", &eName, &b_VPMT_eName);
   fChain->SetBranchAddress("eType", &eType, &b_VPMT_eType);
   fChain->SetBranchAddress("eCalibration", &eCalibration, &b_VPMT_eCalibration);
   fChain->SetBranchAddress("eDelay", &eDelay, &b_VPMT_eDelay);
   fChain->SetBranchAddress("ePedestal", &ePedestal, &b_VPMT_ePedestal);
   fChain->SetBranchAddress("ePedTimeMin", &ePedTimeMin, &b_VPMT_ePedTimeMin);
   fChain->SetBranchAddress("ePedTimeMax", &ePedTimeMax, &b_VPMT_ePedTimeMax);
   fChain->SetBranchAddress("ePedMin", &ePedMin, &b_VPMT_ePedMin);
   fChain->SetBranchAddress("ePedMax", &ePedMax, &b_VPMT_ePedMax);
   fChain->SetBranchAddress("ePedMean", &ePedMean, &b_VPMT_ePedMean);
   fChain->SetBranchAddress("ePedSigma", &ePedSigma, &b_VPMT_ePedSigma);
   fChain->SetBranchAddress("ePedChi2", &ePedChi2, &b_VPMT_ePedChi2);
   fChain->SetBranchAddress("ePedNpe", &ePedNpe, &b_VPMT_ePedNpe);
   fChain->SetBranchAddress("ePedIntegral", &ePedIntegral, &b_VPMT_ePedIntegral);
   fChain->SetBranchAddress("ePedIntegralPe", &ePedIntegralPe, &b_VPMT_ePedIntegralPe);
   fChain->SetBranchAddress("ePedTimePe", &ePedTimePe, &b_VPMT_ePedTimePe);
   fChain->SetBranchAddress("ePeakStartTime", &ePeakStartTime, &b_VPMT_ePeakStartTime);
   fChain->SetBranchAddress("ePeakTime", &ePeakTime, &b_VPMT_ePeakTime);
   fChain->SetBranchAddress("ePeakValue", &ePeakValue, &b_VPMT_ePeakValue);
   fChain->SetBranchAddress("ePeakIsSaturated", &ePeakIsSaturated, &b_VPMT_ePeakIsSaturated);
   fChain->SetBranchAddress("eNPeaks", &eNPeaks, &b_VPMT_eNPeaks);
   fChain->SetBranchAddress("eNpe", &eNpe, &b_VPMT_eNpe);
   fChain->SetBranchAddress("eIntegral", &eIntegral, &b_VPMT_eIntegral);
   fChain->SetBranchAddress("eIntegralPe", &eIntegralPe, &b_VPMT_eIntegralPe);
   fChain->SetBranchAddress("eIntegralPeS", &eIntegralPeS, &b_VPMT_eIntegralPeS);
   fChain->SetBranchAddress("eTimePe", &eTimePe, &b_VPMT_eTimePe);
   fChain->SetBranchAddress("eIntegralSinglet0", &eIntegralSinglet0, &b_VPMT_eIntegralSinglet0);
   fChain->SetBranchAddress("eIntegralSinglet1", &eIntegralSinglet1, &b_VPMT_eIntegralSinglet1);
   fChain->SetBranchAddress("eCR", &eCR, &b_VPMT_eCR);
   fChain->SetBranchAddress("eCRPe", &eCRPe, &b_VPMT_eCRPe);
   fChain->SetBranchAddress("eCRPeS", &eCRPeS, &b_VPMT_eCRPeS);
   fChain->SetBranchAddress("ePeakTimes", &ePeakTimes, &b_VPMT_ePeakTimes);
   fChain->SetBranchAddress("ePeakIntegral", &ePeakIntegral, &b_VPMT_ePeakIntegral);
   fChain->SetBranchAddress("ePeakAmplitude", &ePeakAmplitude, &b_VPMT_ePeakAmplitude);
   fChain->SetBranchAddress("eSumPeakIntegralPMT", &eSumPeakIntegralPMT, &b_VPMT_eSumPeakIntegralPMT);
   fChain->SetBranchAddress("TTT", TTT, &b_TTT);
   fChain->SetBranchAddress("nSamples", &nSamples, &b_nSamples);
   Notify();
}

Bool_t analysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void analysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t analysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef analysis_cxx
