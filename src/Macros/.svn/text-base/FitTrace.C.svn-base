#define MAXPMTS    16
Double_t Sigma=2.7;
//#define Tau1 14.4
Double_t Tau1=8.8;
//#define Tau2 2500.
Double_t Tau2=366.;
Double_t Tau3=100.;

Double_t A=201.;
Double_t B=842.;
Double_t C=200.;

#define MinRej 0
#define MaxRej 0
// #define MinRej 290
// #define MaxRej 340

TH1F *Trace[MAXPMTS+1];
TF1  *f3[MAXPMTS+1];

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

 if (x[0]>MinRej  && x[0]<MaxRej){
   TF1::RejectPoint();
   return 0;
 }


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


void FitTrace(const char *filename, Int_t nexp = 2, Int_t iset = -1)
{
  gStyle->SetOptFit(11111);

  TFile *f = new TFile();
  f->Open(filename);

  Int_t NPMTs;
  Data->SetBranchAddress("NPMTs",&NPMTs);
  Data->GetEntry(0);

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

  TCanvas *c = new TCanvas("Trace","Trace");
  Int_t sqrtn = (int)(sqrt(NPMTs+1)+0.5);
  c->Divide(sqrtn,sqrtn);
  Data->SetMarkerStyle(1);

  char varexp[100],selection[100];

  char name[100];
  for (Int_t i=0;i<NPMTs;i++)
    {
      sprintf(name,"TracePMT%d",i+FirstPMT);
      Trace[i] = new TH1F(name, name, TotalTime, FirstTime-0.5, LastTime+0.5);
      Trace[i]->GetXaxis()->SetTitle("Time [ns]");
      Trace[i]->GetYaxis()->SetTitle("Mean trace [V]");
    }
  sprintf(name,"TraceVPMT");
  Trace[NPMTs] = new TH1F(name, name, TotalTime, FirstTime-0.5, LastTime+0.5);
  Trace[NPMTs]->GetXaxis()->SetTitle("Time [4 ns]");
  Trace[NPMTs]->GetYaxis()->SetTitle("Trace [V]");



  for (Int_t i=0;i<MeanTrace->GetEntries();i++) {
    MeanTrace->GetEntry(i);
    Trace[pmt-FirstPMT]->SetBinContent(time-FirstTime+1,data);
    Trace[pmt-FirstPMT]->SetBinError(time-FirstTime+1,dataerror/sqrt(20000.));
  }

  //  for (Int_t i=0;i<NPMTs+1;i++)
  for (Int_t i=NPMTs;i<NPMTs+1;i++)
    {
      c->cd(i+1);
      gPad->SetLogy();
      
      //      for (Int_t itime=9000;itime<MeanTrace->GetEntries();itime++) Trace[i]->SetBinError(itime, Trace[i]->GetBinError(itime)/100.);
      
      Trace[i]->Draw();
      
      float PeakValue = Trace[i]->GetMaximum();
      float PeakTime = Trace[i]->GetMaximumBin();
      float JunctionValue = Trace[i]->GetBinContent(PeakTime+10*Tau1);

      char funname[10];
      sprintf(funname,"f%d",i+FirstPMT);
      if (nexp==2) {
	f3[i] = new TF1(funname,SignalKeto2,PeakTime-3*Sigma,PeakTime+3*Tau2,6);
	f3[i]->SetParNames("A", "B", "#sigma" ,"#tau_{1}" ,"#tau_{2}" ,"t_{0}");
	f3[i]->SetParameters( (PeakValue-JunctionValue)*2*Tau1,  JunctionValue*Tau2, Sigma, Tau1, Tau2, PeakTime);
      } else {
	f3[i] = new TF1(funname,SignalKeto3,PeakTime-3*Sigma,PeakTime+3*Tau2,8);
	f3[i]->SetParNames("A", "B", "#sigma" ,"#tau_{1}" ,"#tau_{2}" ,"t_{0}", "C", "#tau_{3}");
// 	f3[i]->SetParameters( (PeakValue-JunctionValue)*2*Tau1,  JunctionValue*2*Tau2, Sigma, Tau1, Tau2, PeakTime, 
// 			      (PeakValue-0.5*JunctionValue)*2*Tau3, Tau3 );
	f3[i]->SetParameters( A,  B, Sigma, Tau1, Tau2, PeakTime, C, Tau3 );
      }

      f3[i]->Print();
      f3[i]->SetNpx(1000);
      f3[i]->Draw("same");
      f3[i]->SetLineColor(2);
      //Trace[i]->Fit(funname,"","",900,7000);
      //Trace[i]->Fit(funname,"","",240,2000);
      Trace[i]->Fit(funname,"R");
      f3[i]->Print();

      f3[i]->Draw("same");
      
      for (Int_t itime=0;itime<MeanTrace->GetEntries();itime++) Trace[i]->SetBinError(itime,0);
      gPad->Update();
      c->Update();

      if (iset>=0) {
	char dumpcommand[100];
	sprintf(dumpcommand,"echo %d %f >> tau2vsTime.dat",iset,f3[i]->GetParameter(4)*4);
	gSystem->Exec(dumpcommand);
      }

//       Int_t nbins = 40;
//       Double_t low_margin = -0.1;
//       Double_t high_margin = 1.0;
//       Double_t stepsize = (high_margin-low_margin)/nbins;
//       TH1F *p = new TH1F(p_name,p_name,nbins,low_margin,high_margin);

//       sprintf(varexp,"PMT[%d].eIntegralPe>>%s",i,p_name);
//       //      sprintf(selection,"PMT[%d].eIntegralPedestal<1",i);
//       //      sprintf(selection,"PMT[%d].eNpe>0&&PMT[%d].eNpe<3",i,i);
//       //      sprintf(selection,"PMT[%d].eNpe==1&&PMT[%d].eIntegralPe>0.1",i,i);
//       sprintf(selection,"PMT[%d].eNpe==1",i);
//       Data->Draw(varexp,selection);
//       //      Data->Draw(varexp);

//       char fun_name[100];
//       sprintf(fun_name,"f%d",i);
//       TF1 *fun = new TF1(fun_name,MultiGaus,0.05,0.5,4);
//       //      fun->SetParameters(7.,0.2,0.1,1.);
//       fun->SetParameters(1.,0.2,0.1,0.4);
//       p->Fit(fun_name);
//       Double_t mean,sigma;
//       mean = fun->GetParameter(1);
//       sigma = fun->GetParameter(2);
//       p->Fit(fun_name,"","",mean-sigma,mean+3*sigma);
//       mean = fun->GetParameter(1);
//       sigma = fun->GetParameter(2);
//       p->Fit(fun_name,"","",mean-sigma,mean+3*sigma);

//       Double_t events = fun->GetParameter(0)/stepsize;
//       printf("events = %f\n",events);

    }

}
