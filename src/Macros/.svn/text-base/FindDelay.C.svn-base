Double_t gausCONVexpo(Double_t *x, Double_t *par)
{
  Double_t arg      = 0;
  Double_t t        = x[0] - par[3];
  Double_t sqrt2    = TMath::Sqrt(2);
  //  Double_t norm1    = par[0]/(TMath::Sqrt(2*TMath::Pi())* par[1]); //* par[3]* par[2]);
  Double_t norm1    = (par[0]/par[2])/2;
  
  Double_t argExp1  = (par[1]*par[1]/(2*par[2]*par[2]) -t/par[2]);
  Double_t argErf1  = ((par[1]*par[1] -par[2]*t)/(sqrt2*par[1]*par[2]));
  
  Double_t sig;
  sig =  norm1 * TMath::Exp(argExp1) * (1- TMath::Erf(argErf1)) + par[4];
  return sig;
}


void FindDelay(const char *filename, float start = 900, float stop = 1100, Int_t respectto = -1)
{
  gStyle->SetOptFit(11111);

  TFile *f = new TFile();
  f->Open(filename);

  Int_t NPMTs;
  Data->SetBranchAddress("NPMTs",&NPMTs);
  Data->GetEntry(0);

  NPMTs=14;

  // Sigma pedestal determination

  TArrayI TimeDelay(NPMTs);
  TArrayD PeakTime(NPMTs);

  TClonesArray *har = new TClonesArray("TH1F",20);

  TCanvas *c = new TCanvas("Time Delay","Time Delay");
  Int_t sqrtn = (int)(sqrt(NPMTs)+0.5);
  c->Divide(sqrtn,sqrtn);
  Data->SetMarkerStyle(1);

  for (Int_t i=0;i<NPMTs;i++)
  //  for (Int_t i=0;i<2;i++)
    {
      char varexp[100],selection[1000],name[10];

      sprintf(name,"PMT%d",i+1);

      c->cd(i+1);

      int nbins = 30;

      TH1F *h = new((*har)[i]) TH1F(name,name,nbins,start,stop);

      sprintf(varexp,"PMT[%d].ePeakTime>>%s",i,name);
      //      sprintf(selection,"PMT[%d].ePeakIsSaturated==0&&(PMT[%d].eIntegralSinglet0+PMT[%d].eIntegralSinglet1)/PMT[%d].eIntegral>0.15&&PMT[%d].eIntegral>0.2&&PMT[%d].ePeakValue>0.02",i,i,i,i,i,i);
      sprintf(selection,"PMT[%d].ePeakIsSaturated==0&&PMT[%d].ePeakValue>0.005",i,i);
      Data->Draw(varexp,selection);

      float peakvalue = h->GetMaximum();
      float peaktime = start + h->GetMaximumBin()*(stop-start)/nbins;
      float peaksigma = 3;

      char fun_name[10];
      sprintf(fun_name,"fun%d",i);

      TF1* fun = new TF1(fun_name,"gaus(0)");
      fun->SetParNames("A","t_{0}","#sigma");
      fun->SetParameters(peakvalue,peaktime,peaksigma);

//       float P0 = 60;
//       float tau = 20;
//       TF1* fun = new TF1(fun_name,gausCONVexpo,start,stop,5);
//       fun->SetParNames("A","#sigma","#tau","Time","C");
//       fun->SetParameters(peakvalue*tau,peaksigma,tau,peaktime,P0);
//       fun->FixParameter(2,10);
//       fun->Print();
//       h->Fit(fun_name,"","",start,stop);
//       h->Fit(fun_name,"","",980,1020);
//       fun->Print();

      h->Fit(fun_name,"","",peaktime-2*peaksigma,peaktime+2*peaksigma);
      peaktime = fun->GetParameter(1);
      peaksigma = fun->GetParameter(2);
      h->Fit(fun_name,"","",peaktime-3*peaksigma,peaktime+3*peaksigma);


      PeakTime[i] = fun->GetParameter(1);
    }
  
  double MeanPeakTime = 0.;
  for (Int_t i=0;i<NPMTs;i++) {
    MeanPeakTime += PeakTime[i]/NPMTs;
  }

  printf("Mean peak time = %.2f\n",MeanPeakTime);

  double reference;
  if (respectto<0) {
    printf("Relative delays are done with respect to the mean\n");
    reference = MeanPeakTime;
  } else {
    printf("Relative delays are done with respect to PMT %d\n",respectto);
    reference = PeakTime[respectto];
  }

  printf("Reference = %.2f\n",reference);

  for (Int_t i=0;i<NPMTs;i++) {
    TimeDelay[i] = (int) (PeakTime[i] - reference + 0.5*(PeakTime[i]-reference)/TMath::Abs(PeakTime[i]-reference) );
  }

  printf("DELAY %d ",NPMTs);
  for (Int_t i=0;i<NPMTs;i++)
    {
      printf("%d ",TimeDelay[i]);
    }
  printf("\n");
}
