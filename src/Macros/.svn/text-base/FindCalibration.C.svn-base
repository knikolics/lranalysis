#define MAXCHANNELS 16
#define MAXPMTS  14
#define SAMPLING 4.   // 1 sample == 4 ns

TF1  *funsingle[MAXPMTS];
TF1  *fun[MAXPMTS];

#define MIN 0.04
#define MAX 0.4

Double_t min[MAXPMTS]     = {  MIN,  MIN,  MIN,  MIN,  MIN,  MIN,  MIN,  MIN,  MIN,  MIN,  MIN,  MIN,  MIN,  MIN};
Double_t max[MAXPMTS]     = {  MAX,  MAX,  MAX,  MAX,  MAX,  MAX,  MAX,  MAX,  MAX,  MAX,  MAX,  MAX,  MAX,  MAX};

Double_t MultiGaus(Double_t *x, Double_t *par)
{
  Double_t sig      = 0;
  
  sig =  par[0]*TMath::Gaus(x[0],par[1],par[2]);
  sig += par[3]*TMath::Gaus(x[0],2*par[1],par[2]*TMath::Sqrt(2));
  //  sig += par[4]*TMath::Gaus(x[0],3*par[1],par[2]*TMath::Sqrt(3));
  //  sig += par[5]*TMath::Gaus(x[0],4*par[1],par[2]*TMath::Sqrt(4));

  return sig;
}

Double_t SingleGaus(Double_t *x, Double_t *par)
{
  Double_t sig      = 0;
  
  sig =  par[0]*TMath::Gaus(x[0],par[1],par[2]);

  return sig;
}




void FindCalibration(const char *filename, Double_t HV = 0.)
{
  gStyle->SetOptFit(11111);

  TFile *f = new TFile();
  f->Open(filename);

  Int_t NPMTs,RunNumber,Timestamp;
  Data->SetBranchAddress("NPMTs",&NPMTs);
  Data->SetBranchAddress("RunNumber",&RunNumber);
  Data->SetBranchAddress("Timestamp",&Timestamp);
  Data->GetEntry(0);

  TCanvas *c = new TCanvas("Calibration","Calibration");
  Int_t sqrtn = (int)(sqrt(NPMTs)+0.5);
  c->Divide(sqrtn,sqrtn);
  Data->SetMarkerStyle(1);

  Int_t nbins = 40;
  Double_t low_margin = -0.05;
  Double_t high_margin = 0.6;
  Double_t stepsize = (high_margin-low_margin)/nbins;

  char varexp[100],selection[100];

  for (Int_t i=0;i<MAXPMTS;i++)
    {
      c->cd(i+1);
      //      gPad->SetLogy();

      char p_name[100];
      sprintf(p_name,"Ped%d",i+1);
      printf("%s\n",p_name);
      TH1F *p = new TH1F(p_name,p_name,nbins,low_margin,high_margin);

      sprintf(varexp,"PMT[%d].eIntegralPe*%f>>%s",i,SAMPLING,p_name);
      //      sprintf(selection,"PMT[%d].eIntegralPedestal<1",i);
      //      sprintf(selection,"PMT[%d].eNpe>0&&PMT[%d].eNpe<3",i,i);
      //      sprintf(selection,"PMT[%d].eNpe==1&&PMT[%d].eIntegralPe>0.1",i,i);
      //      sprintf(selection,"PMT[%d].eNpe==1&&!PMT[%d].ePeakIsSaturated",i,i);
      // sprintf(selection,"PMT[%d].eNpe==1&&PMT[%d].ePeakTime>280&&PMT[%d].ePeakTime<330&&PMT[%d].eIntegralPe>0.01",i,i,i,i);
      // Double_t nentries = Data->Draw(varexp,selection);
      // nentries /= stepsize;

      sprintf(selection,"PMT[%d].eNpe==1&&PMT[%d].ePeakTime>280&&PMT[%d].ePeakTime<330",i,i,i);
      Data->Draw(varexp,selection);

      Double_t valuemax=0;
      Int_t ibinmax;

      for (Int_t ibin=1;ibin<=p->GetNbinsX();ibin++)
	{
	  if (p->GetBinLowEdge(ibin)>min[i]&&p->GetBinLowEdge(ibin)<max[i]&&p->GetBinContent(ibin)>valuemax)
	    { valuemax=p->GetBinContent(ibin); ibinmax=ibin; }
	}
      Double_t xmax = p->GetBinLowEdge(ibinmax);

      char fun_name[100];
      sprintf(fun_name,"f%d",i);
      funsingle[i] = new TF1(fun_name,SingleGaus,min[i],max[i],3);
      funsingle[i]->SetParameters(valuemax,xmax,0.1);
      p->Fit(fun_name,"","",min[i],max[i]);
      Double_t height,mean,sigma,pe2;
      height = funsingle[i]->GetParameter(0);
      mean = funsingle[i]->GetParameter(1);
      sigma = TMath::Abs(funsingle[i]->GetParameter(2));

      //      fun[i] = new TF1(fun_name,MultiGaus,min[i],mean+2*sigma,4);
      fun[i] = new TF1(fun_name,MultiGaus,mean-1*sigma,mean+2*sigma,4);
      fun[i]->SetParameters(height,mean,sigma,height/10.);
      fun[i]->SetParLimits(3,0,height);
      p->Fit(fun_name,"","",mean-1*sigma,mean+2*sigma);
      height = fun[i]->GetParameter(0);
      mean = fun[i]->GetParameter(1);
      sigma = TMath::Abs(fun[i]->GetParameter(2));
      pe2 = fun[i]->GetParameter(3);
      fun[i]->SetParameters(height,mean,sigma,pe2);
      p->Fit(fun_name,"","",mean-1*sigma,mean+3*sigma);

      Double_t nsigma=1.;
      while ((fun[i]->GetChisquare()/fun[i]->GetNDF()>2.)&&(nsigma>0.5)) {
      	height = fun[i]->GetParameter(0);
      	mean = fun[i]->GetParameter(1);
      	sigma = TMath::Abs(fun[i]->GetParameter(2));
      	pe2 = fun[i]->GetParameter(3);
      	fun[i]->SetParameters(height,mean,sigma,pe2);
      	p->Fit(fun_name,"","",mean-nsigma*sigma,mean+4*sigma);
      	nsigma -= 0.1;
      }

      // todo: calcolo esatto dell'area ora che ho cambiato la normalizzazione della gaussiana

      Double_t events = fun[i]->GetParameter(0)/TMath::Gaus(0,0,fun[i]->GetParameter(2),kTRUE)/stepsize;
      printf("events under gaussian = %f\n",events);

    }

  Int_t tstamp=0;
  printf("Run%04d %d %d ",RunNumber,RunNumber,Timestamp);
  for (int i=0; i<MAXPMTS; i++){
    printf("%f ",fun[i]->GetParameter(1));
  }
  printf("\n");

  printf("Run%04d %d %d %.0f ",RunNumber,RunNumber,Timestamp,HV);
  for (int i=0; i<MAXPMTS; i++){
    Double_t mean = fun[i]->GetParameter(1);
    Double_t events = fun[i]->GetParameter(0)/TMath::Gaus(0,0,fun[i]->GetParameter(2),kTRUE)/stepsize;
    Double_t sigma = TMath::Abs(fun[i]->GetParameter(2))/sqrt(events);
    printf("%f %f ",mean,sigma);
  }
  printf("\n");


}
