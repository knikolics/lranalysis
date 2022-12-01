#define MAXPMTS    16
#define FitBins  8050
#define Sigma 5.
#define Tau1 17.
#define Tau2 3000.
#define Tau3 100.

TH1F *Trace[MAXPMTS+1];
TF1  *f3[MAXPMTS+1];

void ReadTrace(const char *filename, Int_t log_scale = 0, Int_t display_error = 0)
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
  Int_t sqrtn = (int)(sqrt(NPMTs+1)+0.8);
  c->Divide(sqrtn,sqrtn);
  Data->SetMarkerStyle(1);

  char varexp[100],selection[100];

  char name[100];
  for (Int_t i=0;i<NPMTs;i++)
    {
      sprintf(name,"TracePMT%d",i+FirstPMT);
      Trace[i] = new TH1F(name, name, TotalTime, FirstTime-0.5, LastTime+0.5);
      Trace[i]->GetXaxis()->SetTitle("Time [ns]");
      Trace[i]->GetYaxis()->SetTitle("Trace [V]");
    }
  sprintf(name,"TraceVPMT");
  Trace[NPMTs] = new TH1F(name, name, TotalTime, FirstTime-0.5, LastTime+0.5);
  Trace[NPMTs]->GetXaxis()->SetTitle("Time [ns]");
  Trace[NPMTs]->GetYaxis()->SetTitle("Trace [V]");


  for (Int_t i=0;i<MeanTrace->GetEntries();i++) {
    MeanTrace->GetEntry(i);
    Trace[pmt-FirstPMT]->SetBinContent(time-FirstTime+1,data);
    if (display_error) Trace[pmt-FirstPMT]->SetBinError(time-FirstTime+1,dataerror/sqrt(20000.));
  }

  for (Int_t i=0;i<NPMTs+1;i++)
    {
      c->cd(i+1);
      //      if (log_scale) gPad->SetLogy();

      Trace[i]->Draw();
    }

}
