#define MAXPMTS    14
#define FitBins  8050
//#define shift     950
#define Sigma 5.
#define Tau1 17.
#define Tau2 3000.


TH1F *Trace[MAXPMTS];
TF1  *f3[MAXPMTS];

TCanvas *c,*cerror;

void ReadEvents(const char *prefix, Int_t start, Int_t stop)
{
  char name[200];

  for (Int_t i=start;i<=stop;i++)
    {
      sprintf(name,"%s%d.dat",prefix,i);
      printf("%s\n",name);
      ReadEvent(name);
      char input;
      scanf("%c",&input);
    }
}

void ReadEvent(const char *filename, Int_t log = 0)
{
  TTree *tt = new TTree();
  tt->ReadFile(filename,"d/I:t/I:data/F:dataerror/F");

  Int_t d,t;
  Float_t data,dataerror;
  tt->SetBranchAddress("data",&data);
  tt->SetBranchAddress("dataerror",&dataerror);
  tt->SetBranchAddress("d",&d);
  tt->SetBranchAddress("t",&t);

  Int_t firstPMT = tt->GetMinimum("d");
  Int_t lastPMT = tt->GetMaximum("d");
  //  printf("%d %d\n",firstPMT,lastPMT);
  Int_t nPMTs = lastPMT - firstPMT + 1;
  Int_t sqrtn = (int)(sqrt(nPMTs)+0.5);

  //  printf("%d\n",sqrtn);

  if (c) delete c;
  c = new TCanvas("Trace","Trace");
  c->Divide(sqrtn,sqrtn);

  tt->SetMarkerStyle(1);

  TH1F *Trace[14];
  for (Int_t i=firstPMT;i<=lastPMT;i++) {
    char name[10];
    sprintf(name,"TracePMT%d",i);
    Trace[i] = new TH1F(name,name,10000,-0.5, -0.5+10000);
    Trace[i]->GetXaxis()->SetTitle("Time [ns]");
    Trace[i]->GetYaxis()->SetTitle("Mean Signal [V]");
  }
  
  for (Int_t idata=0;idata<tt->GetEntries();idata++) {
    tt->GetEntry(idata);
    Trace[d]->Fill((float)t,data);
  }

  for (Int_t i=firstPMT;i<=lastPMT;i++)
    {
      c->cd(i-firstPMT+1);
      //      if (log) c->SetLogy();
      Trace[i]->Draw();
      //      if (log) gPad->SetLogy();
      if (log) c->GetPad(i)->SetLogy();
    }

  c->cd(lastPMT+1);
  c->Update();

  return;

  if (cerror) delete cerror;
  cerror = new TCanvas("TraceError","TraceError");
  cerror->Divide(sqrtn,sqrtn);

  tt->SetMarkerStyle(1);

  TH1F *TraceError[14];
  for (Int_t i=firstPMT;i<=lastPMT;i++) {
    char name[10];
    sprintf(name,"TraceErrorPMT%d",i);
    TraceError[i] = new TH1F(name,name,10000,-0.5, -0.5+10000);
    TraceError[i]->GetXaxis()->SetTitle("Time [ns]");
    TraceError[i]->GetYaxis()->SetTitle("Mean Signal Error [V]");
  }
  
  for (Int_t idata=0;idata<tt->GetEntries();idata++) {
    tt->GetEntry(idata);
    TraceError[d]->Fill((float)t,dataerror);
  }

  for (Int_t i=firstPMT;i<=lastPMT;i++)
    {
      cerror->cd(i-firstPMT+1);
      //      if (log) c->SetLogy();
      TraceError[i]->Draw();
      //      if (log) gPad->SetLogy();
      if (log) cerror->GetPad(i)->SetLogy();
    }

  cerror->cd(lastPMT+1);
  cerror->Update();
}






Double_t SignalKetoA(Double_t *x, Double_t *par)
{
  Double_t arg      = 0;
  Double_t t        = x[0] -par[5];
  Double_t sqrt2    = TMath::Sqrt(2);
  Double_t norm1    = (par[0]/par[3])/2;
  
  Double_t argExp1=(par[2]*par[2]/(2*par[3]*par[3]) -t/par[3]);
  Double_t argErf1=((par[2]*par[2] -par[3]*t)/(sqrt2*par[2]*par[3]));
  
  Double_t sig;
  sig =  norm1 * TMath::Exp(argExp1) * (1- TMath::Erf(argErf1));
  return sig;
}

Double_t SignalKetoB(Double_t *x, Double_t *par)
{
  Double_t arg      = 0;
  Double_t t        = x[0] -par[5];
  Double_t sqrt2    = TMath::Sqrt(2);
  Double_t norm1    = (par[1]/par[4])/2;
  
  Double_t argExp1=(par[2]*par[2]/(2*par[4]*par[4]) -t/par[4]);
  Double_t argErf1=((par[2]*par[2] -par[4]*t)/(sqrt2*par[2]*par[4]));
  
  Double_t sig;
  sig =  norm1 * TMath::Exp(argExp1) * (1- TMath::Erf(argErf1));
  return sig;
}

Double_t SignalKetoC(Double_t *x, Double_t *par)
{
  Double_t arg      = 0;
  Double_t t        = x[0] -par[5];
  Double_t sqrt2    = TMath::Sqrt(2);
  Double_t norm1    = (par[6]/par[7])/2;
  
  Double_t argExp1=(par[2]*par[2]/(2*par[7]*par[7]) -t/par[7]);
  Double_t argErf1=((par[2]*par[2] -par[7]*t)/(sqrt2*par[2]*par[7]));
  
  Double_t sig;
  sig =  norm1 * TMath::Exp(argExp1) * (1- TMath::Erf(argErf1));
  return sig;
}


Double_t SignalKeto(Double_t *x, Double_t *par)
{
 Double_t arg      = 0;
 Double_t t        = x[0] - par[5];
 Double_t sqrt2    = TMath::Sqrt(2);
 Double_t norm1    = (par[0]/par[3])/2;
 Double_t norm2    = (par[1]/par[4])/2;
 // Double_t norm3    = (par[6]/par[7])/2;

 Double_t argExp1=(par[2]*par[2]/(2*par[3]*par[3]) -t/par[3]);
 Double_t argExp2=(par[2]*par[2]/(2*par[4]*par[4]) -t/par[4]);
 // Double_t argExp3=(par[2]*par[2]/(2*par[7]*par[7]) -t/par[7]);

 Double_t argErf1=((par[2]*par[2] -par[3]*t)/(sqrt2*par[2]*par[3]));
 Double_t argErf2=((par[2]*par[2] -par[4]*t)/(sqrt2*par[2]*par[4]));
 // Double_t argErf3=((par[2]*par[2] -par[7]*t)/(sqrt2*par[2]*par[7]));

 Double_t sig;

 sig =  norm1 * TMath::Exp(argExp1) * (1- TMath::Erf(argErf1));
 sig += norm2 * TMath::Exp(argExp2) * (1- TMath::Erf(argErf2));
 // sig += norm3 * TMath::Exp(argExp3) * (1- TMath::Erf(argErf3));

 return sig;
}






void FitEvent(const char *filename, Int_t log = 0)
{
  TTree *tt = new TTree();
  tt->ReadFile(filename,"d/I:t/I:data/F:dataerror/F");

  Int_t d,t;
  Float_t data,dataerror;
  tt->SetBranchAddress("data",&data);
  tt->SetBranchAddress("dataerror",&dataerror);
  tt->SetBranchAddress("d",&d);
  tt->SetBranchAddress("t",&t);

  Int_t firstPMT = tt->GetMinimum("d");
  Int_t lastPMT = tt->GetMaximum("d");
  //  printf("%d %d\n",firstPMT,lastPMT);
  Int_t nPMTs = lastPMT - firstPMT + 1;
  Int_t sqrtn = (int)(sqrt(nPMTs)+0.5);

  //  printf("%d\n",sqrtn);

  if (c) delete c;
  c = new TCanvas("Trace","Trace");
  c->Divide(sqrtn,sqrtn);

  tt->SetMarkerStyle(1);

  //  TH1F *Trace[14];
  for (Int_t i=0;i<nPMTs;i++) {
    char name[10];
    sprintf(name,"TracePMT%d",i+firstPMT);
    Trace[i] = new TH1F(name,name,10000,-0.5, -0.5+10000);
    Trace[i]->GetXaxis()->SetTitle("Time [ns]");
    Trace[i]->GetYaxis()->SetTitle("Mean Signal [V]");
  }

  for (Int_t idata=0;idata<tt->GetEntries();idata++) {
    tt->GetEntry(idata);
    Trace[d-firstPMT]->Fill((float)t,data);
  }

  for (Int_t i=0;i<nPMTs;i++)
    {
      c->cd(i+1);
      //      if (log) c->SetLogy();
      Trace[i]->Draw();
      //      if (log) gPad->SetLogy();
      if (log) c->GetPad(i+1)->SetLogy();
    }

  c->cd(lastPMT+1);
  c->Update();


//   TF1  *f3A       = new TF1 ("f3A",SignalKetoA,shift,FitBins+shift,8);
//   TF1  *f3B       = new TF1 ("f3B",SignalKetoB,shift,FitBins+shift,8);
//   TF1  *f3C       = new TF1 ("f3C",SignalKetoC,shift,FitBins+shift,8);

  for (Int_t i=0;i<nPMTs;i++)
  //  for (Int_t i=0;i<1;i++)
    {
      c->cd(i+1);
      Trace[i]->Draw();
      if (log) c->GetPad(i+1)->SetLogy();

      // finding the signal peak
      float PeakValue = Trace[i]->GetMaximum();
      float PeakTime = Trace[i]->GetMaximumBin();
      float JunctionValue = Trace[i]->GetBinContent(PeakTime+10*Tau1);

      char funname[10];
      sprintf(funname,"f%d",i+firstPMT);
      f3[i]  = new TF1(funname,SignalKeto,PeakTime-3*Sigma,PeakTime+3*Tau2,6);

      

      f3[i]->SetParNames("A", "B", "#sigma" ,"#tau_{1}" ,"#tau_{2}" ,"t_{0}");
      f3[i]->SetParameters( (PeakValue-JunctionValue)*2*Tau1,  JunctionValue*2*Tau2, Sigma, Tau1, Tau2, PeakTime);
      f3[i]->Draw("same");
      f3[i]->SetLineColor(2);
      //      Trace[i]->Fit(funname,"RLL");
      Trace[i]->Fit(funname);
    }




//   if (cerror) delete cerror;
//   cerror = new TCanvas("TraceError","TraceError");
//   cerror->Divide(sqrtn,sqrtn);

//   tt->SetMarkerStyle(1);

//   TH1F *TraceError[14];
//   for (Int_t i=firstPMT;i<=lastPMT;i++) {
//     char name[10];
//     sprintf(name,"TraceErrorPMT%d",i);
//     TraceError[i] = new TH1F(name,name,10000,-0.5, -0.5+10000);
//     TraceError[i]->GetXaxis()->SetTitle("Time [ns]");
//     TraceError[i]->GetYaxis()->SetTitle("Mean Signal Error [V]");
//   }
  
//   for (Int_t idata=0;idata<tt->GetEntries();idata++) {
//     tt->GetEntry(idata);
//     TraceError[d]->Fill((float)t,dataerror);
//   }

//   for (Int_t i=firstPMT;i<=lastPMT;i++)
//     {
//       cerror->cd(i-firstPMT+1);
//       if (log) c->SetLogy();
//       TraceError[i]->Draw();
//       //      if (log) gPad->SetLogy();
//       if (log) cerror->GetPad(i)->SetLogy();
//     }

//   cerror->cd(lastPMT+1);
//   cerror->Update();
}
