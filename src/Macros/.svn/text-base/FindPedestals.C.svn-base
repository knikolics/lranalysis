void FindPedestals(const char *filename)
{
  gStyle->SetOptFit(11111);

  TFile *f = new TFile();
  f->Open(filename);

  Int_t NPMTs;
  Data->SetBranchAddress("NPMTs",&NPMTs);
  Data->GetEntry(0);

  // Sigma pedestal determination

  TArrayF Sigma_Mean(NPMTs);
  TArrayF Sigma_RMS(NPMTs);

  TCanvas *c = new TCanvas("Sigma Pedestal","Sigma Pedestal");
  Int_t sqrtn = (int)(sqrt(NPMTs)+0.5);
  c->Divide(sqrtn,sqrtn);
  Data->SetMarkerStyle(1);

  TClonesArray *hsigma1 = new TClonesArray("TH1F",16);
  TClonesArray *hsigma2 = new TClonesArray("TH1F",16);
  TClonesArray *hsigma3 = new TClonesArray("TH1F",16);

  TH1F *h;
  char hname[10];

// This when you want to observe the tail in the RMS distribution
//   for (Int_t i=0;i<NPMTs;i++)
//     {
//       c->cd(i+1);

//       char varexp[100];
//       sprintf(varexp,"PMT[%d].ePedSigma",i);
//       Data->Draw(varexp);

//       float low  = Data->GetHistogram()->GetMean() - 3*Data->GetHistogram()->GetRMS();
//       float high = Data->GetHistogram()->GetMean() + 3*Data->GetHistogram()->GetRMS();

//       sprintf(hname,"h1_%d",i);
//       h = new((*hsigma1)[i]) TH1F(hname,hname,1000,-3,-1);
//       sprintf(varexp,"log10(PMT[%d].ePedSigma)>>%s",i,hname);
//       Data->Draw(varexp);
//       low  = Data->GetHistogram()->GetMean() - 3*Data->GetHistogram()->GetRMS();
//       high = Data->GetHistogram()->GetMean() + 3*Data->GetHistogram()->GetRMS();
//     }
// return;

  for (Int_t i=0;i<NPMTs;i++)
    {
      c->cd(i+1);

      char varexp[100];
      sprintf(varexp,"PMT[%d].ePedSigma",i);
      Data->Draw(varexp);

      float low  = Data->GetHistogram()->GetMean() - 3*Data->GetHistogram()->GetRMS();
      float high = Data->GetHistogram()->GetMean() + 3*Data->GetHistogram()->GetRMS();

      sprintf(hname,"h1_%d",i);
      h = new((*hsigma1)[i]) TH1F(hname,hname,15,low,high);
      sprintf(varexp,"PMT[%d].ePedSigma>>%s",i,hname);
      Data->Draw(varexp);
      low  = Data->GetHistogram()->GetMean() - 3*Data->GetHistogram()->GetRMS();
      high = Data->GetHistogram()->GetMean() + 3*Data->GetHistogram()->GetRMS();

      sprintf(hname,"h2_%d",i);
      h = new((*hsigma2)[i]) TH1F(hname,hname,15,low,high);
      sprintf(varexp,"PMT[%d].ePedSigma>>%s",i,hname);
      Data->Draw(varexp);
      low  = Data->GetHistogram()->GetMean() - 3*Data->GetHistogram()->GetRMS();
      high = Data->GetHistogram()->GetMean() + 3*Data->GetHistogram()->GetRMS();

      sprintf(hname,"h3_%d",i);
      h = new((*hsigma3)[i]) TH1F(hname,hname,15,low,high);
      sprintf(varexp,"PMT[%d].ePedSigma>>%s",i,hname);
      Data->Draw(varexp);
      Sigma_Mean[i] = h->GetMean();
      Sigma_RMS[i] = h->GetRMS();
    }

  TString selection="";

  for (Int_t i=0;i<NPMTs;i++)
    {
      if (i) selection +="&&";
      selection += "PMT[";
      selection += i;
      selection += "].ePedSigma<";
      selection += Sigma_Mean[i]+3*Sigma_RMS[i];
    }

  printf("Preliminary cut used: %s\n",selection.Data());

  TClonesArray *hmean = new TClonesArray("TH1F",16);

  for (Int_t i=0;i<NPMTs;i++)
    //  for (Int_t i=0;i<1;i++)
    {
      c->cd(i+1);

      char varexp[100];
      sprintf(varexp,"PMT[%d].ePedSigma",i);
      Data->Draw(varexp,selection);

      float low  = Data->GetHistogram()->GetMean() - 3*Data->GetHistogram()->GetRMS();
      float high = Data->GetHistogram()->GetMean() + 3*Data->GetHistogram()->GetRMS();

      sprintf(hname,"hmean_%d",i);
      h = new((*hmean)[i]) TH1F(hname,hname,15,low,high);
      sprintf(varexp,"PMT[%d].ePedSigma>>%s",i,hname);
      Data->Draw(varexp,selection);

      Sigma_Mean[i] = h->GetMean();
      Sigma_RMS[i] = h->GetRMS();

      // TF1 *f1 = new TF1("poi","[0]*TMath::Poisson(x*[1],[2]*[1])",h->GetMean()-3*h->GetRMS(),h->GetMean()+3*h->GetRMS());
      // f1->SetParameters(h->Integral(),h->GetMean()/(h->GetRMS()**2),h->GetMean());
      // TF1 *f1 = new TF1("poi","[1]*TMath::Poisson(x/[2],[0])",0,1);
      // h->Fit("poi");
      h->Fit("gaus");
      
      TF1 *fit = h->GetFunction("gaus");
      printf("PMT %2d) Pedestal: sigma = %f\n",i+1,fit->GetParameter(1));

      Sigma_Mean[i] = fit->GetParameter(1);
      Sigma_RMS[i] = fit->GetParameter(2);
    }

//   if (hmean) {
//     hmean->Delete();
//     delete hmean;
//     hmean=0;
//   }

  selection="";

  for (Int_t i=0;i<NPMTs;i++)
    {
      if (i) selection +="&&";
      selection += "PMT[";
      selection += i;
      selection += "].ePedSigma<";
      selection += Sigma_Mean[i]+3*Sigma_RMS[i];
    }

  //  selection += "&&FileNumber>10";

  printf("Cut used: %s\n",selection.Data());

  // Mean pedestal determination

  TArrayF Mean_Mean(NPMTs);
  TArrayF Mean_RMS(NPMTs);

  TCanvas *c = new TCanvas("Mean Pedestal","Mean Pedestal");
  Int_t sqrtn = (int)(sqrt(NPMTs)+0.5);
  c->Divide(sqrtn,sqrtn);
  Data->SetMarkerStyle(1);

  for (Int_t i=0;i<NPMTs;i++)
    //for (Int_t i=0;i<1;i++)
    {
      c->cd(i+1);

      char varexp[100];
      sprintf(varexp,"PMT[%d].ePedMean",i);
      Data->Draw(varexp,selection);

      float low  = Data->GetHistogram()->GetMean() - 3*Data->GetHistogram()->GetRMS();
      float high = Data->GetHistogram()->GetMean() + 3*Data->GetHistogram()->GetRMS();

      sprintf(hname,"hm1_%d",i);
      h = new TH1F(hname,hname,40,low,high);
      sprintf(varexp,"PMT[%d].ePedMean>>%s",i,hname);
      Data->Draw(varexp,selection);

      h->Fit("gaus");
      TF1 *fit = h->GetFunction("gaus");
      printf("PMT %2d) Pedestal: mean = %f\n",i+1,fit->GetParameter(1));

      Mean_Mean[i] = fit->GetParameter(1);
      Mean_RMS[i] = fit->GetParameter(2);
    }




  TCanvas *c = new TCanvas("Time dependence","Time dependence");
  c->Divide(sqrtn,sqrtn);
  //  c->Divide(1,NPMTs);
  Data->SetMarkerStyle(1);
  
  // for (Int_t i=0;i<1;i++)
  for (Int_t i=0;i<NPMTs;i++)
    {
      char varexp[100];

      c->cd(i+1);

      sprintf(varexp,"PMT[%d].ePedMean",i);
      Data->Draw(varexp,selection);

      printf("PMT %2d) Pedestal: mean = %f %f\n",i+1,Mean_Mean[i],Mean_RMS[i]);

      //      TH2F *h2 = new TH2F("h2","h2",200,0,199999,100,Mean_Mean[i]-3*Mean_RMS[i],Mean_Mean[i]+3*Mean_RMS[i]);
      sprintf(varexp,"PMT[%d].ePedMean:(FileNumber*1000+EventNumber)>>hh%d",i,i);
      Data->Draw(varexp,selection,"prof");

      // delete h;
    }

  printf("PEDESTAL %d ",NPMTs);
  for (Int_t i=0;i<NPMTs;i++)
    {
      printf("%f ",Mean_Mean[i]);
    }
  printf("\n");
}
