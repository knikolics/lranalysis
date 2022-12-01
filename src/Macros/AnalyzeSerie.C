#include "TH1.h"
#include "TF1.h"
#include "TROOT.h"
#include "TStyle.h"
#include <fstream>
#include <iostream>

#define MAXRUNS    10

#define MAXPMTS    14
#define FitBins  8050
#define Sigma 5.
#define Tau1 17.
#define Tau2 3000.
#define Tau3 100.

#define PositionStart -20.
#define PositionStep   10.
//#define PositionStart  40.
//#define PositionStep  -10.

//#define DEBUG 

// Int_t    MinRej=1025;
// Int_t    MaxRej=1300;
// Double_t resultPars[PMTS],resultErrs[PMTS];
// string   PMTName[PMTS] ={ "1:FA0020","2:FA0010", "3:FA0015", "4:FA0006", "5:FA0013", "6:FA0009", "7:FA0006",
// 			  "8:FA0018","9:FA0011","10:FA0007","11:FA0014","12:FA0016","13:FA0002","14:FA0001"};

Double_t SignalKeto2(Double_t *x, Double_t *par)
{
 Double_t arg      = 0;
 Double_t t        = x[0] - par[5];
 Double_t sqrt2    = TMath::Sqrt(2);
 Double_t norm1    = (par[0]/par[3])/2;
 Double_t norm2    = (par[1]/par[4])/2;

 Double_t argExp1=(par[2]*par[2]/(2*par[3]*par[3]) -t/par[3]);
 Double_t argExp2=(par[2]*par[2]/(2*par[4]*par[4]) -t/par[4]);

 Double_t argErf1=((par[2]*par[2] -par[3]*t)/(sqrt2*par[2]*par[3]));
 Double_t argErf2=((par[2]*par[2] -par[4]*t)/(sqrt2*par[2]*par[4]));

 Double_t sig;

 sig =  norm1 * TMath::Exp(argExp1) * (1- TMath::Erf(argErf1));
 sig += norm2 * TMath::Exp(argExp2) * (1- TMath::Erf(argErf2));

 return sig;
}


Double_t SignalKeto3(Double_t *x, Double_t *par)
{
 Double_t arg      = 0;
 Double_t t        = x[0] - par[5];
 Double_t sqrt2    = TMath::Sqrt(2);
 Double_t norm1    = (par[0]/par[3])/2;
 Double_t norm2    = (par[1]/par[4])/2;
 Double_t norm3    = (par[6]/par[7])/2;

 Double_t argExp1=(par[2]*par[2]/(2*par[3]*par[3]) -t/par[3]);
 Double_t argExp2=(par[2]*par[2]/(2*par[4]*par[4]) -t/par[4]);
 Double_t argExp3=(par[2]*par[2]/(2*par[7]*par[7]) -t/par[7]);

 Double_t argErf1=((par[2]*par[2] -par[3]*t)/(sqrt2*par[2]*par[3]));
 Double_t argErf2=((par[2]*par[2] -par[4]*t)/(sqrt2*par[2]*par[4]));
 Double_t argErf3=((par[2]*par[2] -par[7]*t)/(sqrt2*par[2]*par[7]));

 Double_t sig;

 sig =  norm1 * TMath::Exp(argExp1) * (1- TMath::Erf(argErf1));
 sig += norm2 * TMath::Exp(argExp2) * (1- TMath::Erf(argErf2));
 sig += norm3 * TMath::Exp(argExp3) * (1- TMath::Erf(argErf3));

 return sig;
}




string i2s(Int_t run){
  char trun[6];
  sprintf(trun,"%d",run);
  string srun;
  srun = trun;
  return srun;  
}

string f2s(Float_t run){
  char trun[6];
  sprintf(trun,"%.1f",run);
  string srun;
  srun = trun;
  return srun;

}

string f2s(Float_t run,int digits){
  char trun[6];
  string code="%.";
  code += i2s(digits);
  code += "f";
  sprintf(trun,code.c_str(),run);
  string srun;
  srun = trun;
  return srun;
}


void AnalyseSerie(const char *outputfilename, Int_t nexp = 2)
{
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptFit(1);
  gStyle->SetOptStat(1110);
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetStatX(0.99);
  gStyle->SetStatY(0.99);
  gStyle->SetStatColor(0);
  gStyle->SetStatW(0.2);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetOptLogy(0);


  /***********************
     Creating file names
  ************************/

  // First Serie with GAr and Different source position  
  Int_t RunNumber[] = {
    0025
  //  1958,
    //1961,
   // 1964,
   // 1967,
   // 1970,
   // 1973,
   // 1976
  };

  Int_t nruns = sizeof(RunNumber)/sizeof(Int_t);
  printf("Runs to be analysed: %d\n",nruns);

  char FileName[MAXRUNS][1000];
  for (int irun=0;irun<nruns;irun++)
    //sprintf(FileName[irun],"Run%d/Run%d-File.output.Alpha.root",RunNumber[irun],RunNumber[irun]);
    sprintf(FileName[irun],"Run%d/Run%d-File.output.root",RunNumber[irun],RunNumber[irun]);

  /**********************
     Reading parameters
  ***********************/

  Double_t ReflectorNumber[MAXRUNS];
  Double_t SourcePosition[MAXRUNS];
  Double_t SourcePositionE[MAXRUNS];
  Double_t RunTime[MAXRUNS];
  Double_t RunTimeE[MAXRUNS];
  Int_t    Run[MAXRUNS];

  for (Int_t irun=0;irun<nruns;irun++)
    {
      ReflectorNumber[irun] = 20.;
      SourcePosition[irun]  = PositionStart + irun*PositionStep;
      SourcePositionE[irun] = 1.;
      RunTime[irun]         = 0.;
      RunTimeE[irun]        = 5.;
      Run[irun]             = 0;
    }

//   Int_t colors[] = {kBlack,kOrange,kRed,kMagenta,kBlue,kGreen,kCyan};  
//   Int_t    IdxMar[MAXPMTS];
//   Int_t    IdxCol[MAXPMTS];
//   Char_t   IdxFun[MAXPMTS];
//   for (Int_t ipmt=0;ipmt<MAXPMTS;ipmt++) {
//     IdxMar[ipmt]          = (ipmt%2)? 20 : 21;
//     IdxCol[ipmt]          = colors[(ipmt/2)];
//     IdxFun[ipmt]          = 'a'+ipmt;
//   }

  /*************************
     Opening output files
  **************************/

  FILE *fpout;
  if ( (fpout=fopen(outputfilename,"w")) == 0 ) {
    printf("Error! File %s cannot be created\n",outputfilename);
    return;
  }

  fprintf(fpout,"%d\n",nruns);

  TFile *outfile = new TFile("prova.root","RECREATE");

  /*********************
     Looping over runs
  **********************/

  for (int irun=0;irun<nruns;irun++) {
    
    printf("\n\nOpening file %s\n",FileName[irun]);
    TFile *f = new TFile(FileName[irun],"READ");

    Int_t NPMTs,Timestamp;
    Data->SetBranchAddress("NPMTs",&NPMTs);
    Data->SetBranchAddress("Timestamp",&Timestamp);
    Data->GetEntry(0);
    
    fprintf(fpout,"%d\n",NPMTs);

    Int_t pmt;
    Int_t time;
    Double_t data;
    Double_t dataerror;
    MeanTrace->SetBranchAddress("PMT",&pmt);
    MeanTrace->SetBranchAddress("Time",&time);
    MeanTrace->SetBranchAddress("Data",&data);
    MeanTrace->SetBranchAddress("DataError",&dataerror);
    
    Int_t FirstPMT = MeanTrace->GetMinimum("PMT");
    Int_t LastPMT  = MeanTrace->GetMaximum("PMT");
    Int_t FirstTime = MeanTrace->GetMinimum("Time");
    Int_t LastTime  = MeanTrace->GetMaximum("Time");
    Int_t TotalTime = LastTime-FirstTime+1;
    
    char canvasname[100];
    sprintf(canvasname,"Run%d",irun);
    TCanvas *c = new TCanvas(canvasname,canvasname);
    Int_t sqrtn = (int)(sqrt(NPMTs+1)+0.5);
    c->Divide(sqrtn,sqrtn);
    Data->SetMarkerStyle(1);
    
    char varexp[100],selection[100];

    outfile->cd();
    
    TH1F *Trace[MAXPMTS+1];
    TF1  *fun[MAXPMTS+1];

    char name[100];
    for (Int_t ipmt=0;ipmt<NPMTs;ipmt++)
      {
	sprintf(name,"Trace_PMT%d_RUN%d",ipmt+FirstPMT,irun);
	Trace[ipmt] = new TH1F(name, name, TotalTime, FirstTime-0.5, LastTime+0.5);
	Trace[ipmt]->GetXaxis()->SetTitle("Time [ns]");
	Trace[ipmt]->GetYaxis()->SetTitle("Mean trace [V]");
      }
    sprintf(name,"TraceVPMT_RUN%d",irun);
    Trace[NPMTs] = new TH1F(name, name, TotalTime, FirstTime-0.5, LastTime+0.5);
    Trace[NPMTs]->GetXaxis()->SetTitle("Time [ns]");
    Trace[NPMTs]->GetYaxis()->SetTitle("Trace [V]");

    f->cd();
        
    for (Int_t i=0;i<MeanTrace->GetEntries();i++) {
      MeanTrace->GetEntry(i);
      Trace[pmt-FirstPMT]->SetBinContent(time-FirstTime+1,data);
      Trace[pmt-FirstPMT]->SetBinError(time-FirstTime+1,dataerror/sqrt(20000.));
    }
    
    for (Int_t ipmt=0;ipmt<NPMTs+1;ipmt++)
      //    for (Int_t ipmt=NPMTs;ipmt<NPMTs+1;ipmt++)
      {
	c->cd(ipmt+1);
	gPad->SetLogy();
	
	//  for (Int_t itime=9000;itime<MeanTrace->GetEntries();itime++) 
	//    Trace[i]->SetBinError(itime, Trace[i]->GetBinError(itime)/100.);
	
	Trace[ipmt]->Draw();
	
	float PeakValue = Trace[ipmt]->GetMaximum();
	float PeakTime = Trace[ipmt]->GetMaximumBin();
	float JunctionValue = Trace[ipmt]->GetBinContent(PeakTime+10*Tau1);
	
	char funname[10];
	sprintf(funname,"f%d",ipmt+FirstPMT);
	if (nexp==2) {
	  fun[ipmt] = new TF1(funname,SignalKeto2,PeakTime-3*Sigma,PeakTime+3*Tau2,6);
	  fun[ipmt]->SetParNames("A", "B", "#sigma" ,"#tau_{1}" ,"#tau_{2}" ,"t_{0}");
	  fun[ipmt]->SetParameters( (PeakValue-JunctionValue)*2*Tau1,  JunctionValue*Tau2, Sigma, Tau1, Tau2, PeakTime);
	} else {
	  fun[ipmt] = new TF1(funname,SignalKeto3,PeakTime-3*Sigma,PeakTime+3*Tau2,8);
	  fun[ipmt]->SetParNames("A", "B", "#sigma" ,"#tau_{1}" ,"#tau_{2}" ,"t_{0}", "C", "#tau_{3}");
	  fun[ipmt]->SetParameters( (PeakValue-JunctionValue)*2*Tau1,  JunctionValue*2*Tau2, Sigma, Tau1, Tau2, PeakTime, 
				    (PeakValue-0.5*JunctionValue)*2*Tau3, Tau3 );
	}
	
	fun[ipmt]->Print();
	fun[ipmt]->SetNpx(1000);
	fun[ipmt]->Draw("same");
	fun[ipmt]->SetLineColor(2);
	Trace[ipmt]->Fit(funname,"","",900,7000);
	fun[ipmt]->Print();
	
	fun[ipmt]->Draw("same");
	
	for (Int_t itime=0;itime<MeanTrace->GetEntries();itime++) Trace[ipmt]->SetBinError(itime,0);
	gPad->Update();
	c->Update();

	Double_t A,B,sigma,tau1,tau2,t0,C,tau3;
	Double_t SA,SB,Ssigma,Stau1,Stau2,St0,SC,Stau3;
	A     = fun[ipmt]->GetParameter(0);     SA     = fun[ipmt]->GetParError(0);
	B     = fun[ipmt]->GetParameter(1);	SB     = fun[ipmt]->GetParError(1);
	sigma = fun[ipmt]->GetParameter(2);	Ssigma = fun[ipmt]->GetParError(2);
	tau1  = fun[ipmt]->GetParameter(3);	Stau1  = fun[ipmt]->GetParError(3);
	tau2  = fun[ipmt]->GetParameter(4);	Stau2  = fun[ipmt]->GetParError(4);
	t0    = fun[ipmt]->GetParameter(5);	St0    = fun[ipmt]->GetParError(5);
	if (nexp==3) {
	  C     = fun[ipmt]->GetParameter(6);   SC     = fun[ipmt]->GetParError(6);
	  tau3  = fun[ipmt]->GetParameter(7);   Stau3  = fun[ipmt]->GetParError(7);
	} else {
	  C = 0;                                SC = 0;
	  tau3 = 0;                             Stau3 = 0;
	}

	Double_t TotMaxPurity  = B*(3200./tau2) + A;

	fprintf(fpout,"%d %d %d %d %f %f ",irun,RunNumber[irun],ipmt,Timestamp,SourcePosition[irun],SourcePositionE[irun]);
	fprintf(fpout,"%f %f %f %f %f %f %f %f %f %f ",A,SA,B,SB,sigma,Ssigma,tau1,Stau1,tau2,Stau2);
	fprintf(fpout,"%f %f %f %f %f %f\n",t0,St0,C,SC,tau3,Stau3);


      } // end ipmt


  } // end irun


  outfile->Write();
  outfile->Close();
  fclose(fpout);

}
