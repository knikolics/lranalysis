void ShowBackground(const char *filename)
{
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111);
  gStyle->SetPalette(1);

  TFile *f = new TFile();
  f->Open(filename);

  Int_t NPMTs;
  Data->SetBranchAddress("NPMTs",&NPMTs);
  Data->GetEntry(0);

  TCanvas *c = new TCanvas("Background","Background");
  Data->SetMarkerStyle(1);

  char varexp[100][1000],selection[100][1000];

  TH1F *frame;
  char framename[100];

  sprintf(framename,"VPMT");

  frame = new TH1F(framename, framename, 81, -20, 60);
  frame->GetXaxis()->SetTitle("Integral [pe]");

  sprintf(varexp[0],"(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)/0.2*4.5/2.0>>%s",framename);

  TString total_selection;
  total_selection="";
  
  for (int isel=0;isel<100;isel++) sprintf(selection[isel],"");
  
  sprintf(selection[0],"1");
  //  sprintf(selection[1],"&&(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)/0.2>-0.5");
  sprintf(selection[2],"&&VPMT.ePeakTime<7000");
  sprintf(selection[3],"&&VPMT.ePedSigma<0.008");
  sprintf(selection[4],"&&VPMT.eCR<1");

  for (int isel=0;isel<100;isel++) if (selection[isel]!="") total_selection.Append(selection[isel]);
  
  printf("%s\n",total_selection.Data());
  
  //frame->Draw();
  //  int entries = Data->Draw(varexp[0],total_selection.Data(),"same");
  int entries = Data->Draw(varexp[0],total_selection.Data(),"");
  printf("entries = %d\n",entries);

}
