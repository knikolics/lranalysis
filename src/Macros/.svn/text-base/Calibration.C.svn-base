// main calibration script for LED and data runs
#define MAXCHANNELS 16
#define MAXPMTS  14
#define SAMPLING 4.   // 1 sample == 4 ns

TF1  *fun[MAXPMTS];

#define MIN 0.03
#define MAX 0.7

//Double_t min[MAXPMTS]     = {  0.13,  0.15,  0.15,  0.13,  0.15,  0.15,  0.15,  0.10,  0.12,  0.15,  0.13,  0.15,  0.16,  0.15};
Double_t min[MAXPMTS]     = {  0.12,  0.12,  0.12,  0.12,  0.12,  0.12,  0.12,  0.12,  0.12,  0.12,  0.12,  0.12,  0.12,  0.12};
Double_t max[MAXPMTS]     = {  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5};
Double_t singlePE[MAXPMTS]     = {  0.2,  0.2,  0.2,  0.2,  0.2,  0.2,  0.2,  0.2,  0.2,  0.2,  0.2,  0.2,  0.2,  0.2};
Double_t singlePEsigma[MAXPMTS]     = {  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1};

Int_t nbins = 100;
Double_t low_margin = -0.1;
Double_t high_margin = 0.9;
//Double_t high_margin = 1.1;

Double_t MultiGaus(Double_t *x, Double_t *par)
{
  Double_t sig = 0;
  sig =  par[0]*TMath::Gaus(x[0],par[1],par[2]); //single pe
  sig += par[3]*TMath::Gaus(x[0],2*par[1],par[2]*TMath::Sqrt(2)); //double pe
  //sig += par[4]*TMath::Gaus(x[0],3*par[1],par[2]*TMath::Sqrt(3)); //double pe
  //sig += par[5]*TMath::Gaus(x[0],par[6],par[7]); //pedestal
  // sig += par[5]*TMath::Exp(-(x[0]-par[6])/par[7]); //pedestal

  return sig;
}

void FindCalibration(const char *filename, Int_t pulser=0)
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptFit(11111);
  
  TFile *f = new TFile();
  f->Open(filename);

  Int_t NPMTs,RunNumber,Timestamp;
  Data->SetBranchAddress("NPMTs",&NPMTs);
  Data->SetBranchAddress("RunNumber",&RunNumber);
  Data->SetBranchAddress("AcquisitionTime",&Timestamp);
  Data->GetEntry(0);
  
  TCanvas *c = new TCanvas("Calibration","Calibration");
  Int_t sqrtn = (int)(sqrt(NPMTs)+0.5);
  c->Divide(sqrtn,sqrtn);
  Data->SetMarkerStyle(1);

  
  Double_t stepsize = (high_margin-low_margin)/nbins;
  char varexp[100],selection[100];
  Double_t integral[MAXPMTS];

  for (Int_t i=0;i<MAXPMTS;i++)
    {
      c->cd(i+1);
      
      char p_name[100];
      sprintf(p_name,"Ped%d",i+1);
      printf("%s\n",p_name);
      TH1F *p = new TH1F(p_name,p_name,nbins,low_margin,high_margin);
      
      sprintf(varexp,"PMT[%d].eIntegralPe*%f>>%s",i,SAMPLING,p_name);
      if (pulser==1) sprintf(selection,"PMT[%d].eNpe==1 && PMT[%d].ePeakTime>328&&PMT[%d].ePeakTime<500",i,i,i);
      else sprintf(selection,"PMT[%d].eNpe==1 && PMT[%d].ePeakTime>1910&&PMT[%d].ePeakTime<1990",i,i,i);
      
      Data->Draw(varexp,selection);
      integral[i]=p->Integral("width");
      //p->Scale(1./integral[i]);
    
      char fun_name[100];
      sprintf(fun_name,"f%d",i);
      Double_t height,mean,sigma,pe2;
      
      fun[i] = new TF1(fun_name,MultiGaus,min[i],max[i],4);
      fun[i]->SetParameters(2.745*integral[i],singlePE[i],singlePEsigma[i],0.08*integral[i]);
      p->Fit(fun_name,"R");
      p->GetXaxis()->SetTitle("integral (nVs)");
      p->GetYaxis()->SetTitle("cnts / 0.01 nVs");
    }

  //char tmpName[256];
  //sprintf(tmpName,"Run%04d/Ped.pdf",RunNumber);

  char tmpName[256];
  sprintf(tmpName,"Run%04d/calibration.txt",RunNumber);
  
  ofstream calibFile;
  calibFile.open(tmpName);
  
  printf("%d %d ",RunNumber,Timestamp);
  calibFile << RunNumber << " " << Timestamp << " ";
  for (int i=0; i<MAXPMTS; i++){
    printf("%f ",fun[i]->GetParameter(1));
    calibFile << (float) (fun[i]->GetParameter(1)) << " ";
  }
  printf("\n");
  calibFile << endl;
  
  printf("%d %d ",RunNumber,Timestamp);
  calibFile << RunNumber << " " << Timestamp << " ";
  for (int i=0; i<MAXPMTS; i++){
    Double_t mean = fun[i]->GetParameter(1);
    Double_t sigma = fun[i]->GetParameter(2);
    Double_t sigmaError = fun[i]->GetParError(2);
    Double_t meanError = fun[i]->GetParError(1);
    //Double_t events = fun[i]->GetParameter(0)/TMath::Gaus(0,0,fun[i]->GetParameter(2),kTRUE)/stepsize;
    //Double_t sigma = TMath::Abs(fun[i]->GetParameter(2))/sqrt(events);
    printf("%f %f %f %f ",mean,meanError,sigma, sigmaError);
    calibFile << mean << " " << meanError << " " << sigma << " " << sigmaError << " ";
  }
  printf("\n");
  calibFile << endl;
  calibFile.close();

  sprintf(tmpName,"Run%04d/calibration.pdf",RunNumber);
  c->SaveAs(tmpName);
  sprintf(tmpName,"Run%04d/calibration.png",RunNumber);
  c->SaveAs(tmpName);
}
