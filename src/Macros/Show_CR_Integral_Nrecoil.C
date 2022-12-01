Float_t CR_min = 0.1;
Float_t CR_max = 0.25;
Float_t Integral_min = 300.*0.2.; // nVs
Float_t Integral_max = 700.*0.2; // nVs

void Show_CR_Integral_Nrecoil(const char *filename, int pe = 0, float conv = 0.2)
{
  if (!pe) conv = 1.;

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111);
  gStyle->SetPalette(1);

  TFile *f = new TFile();
  f->Open(filename);

  Int_t NPMTs;
  Data->SetBranchAddress("NPMTs",&NPMTs);
  Data->GetEntry(0);

  TCanvas *c = new TCanvas("CRVSIntegral","CR VS Integral");
  Data->SetMarkerStyle(1);

  char varexp[100][1000],selection[100][1000];

  TH2F *frame;
  char framename[100];

  sprintf(framename,"VPMT");

  frame = new TH2F(framename, framename, 100, 0/conv, 200/conv, 100, 0, 1);
  if (!pe) frame->GetXaxis()->SetTitle("Integral [nVs]");
  else frame->GetXaxis()->SetTitle("Integral [pe]");
  frame->GetYaxis()->SetTitle("L_{50}/L_{tot}");

  //  sprintf(varexp[0],"(VPMT.eIntegralSinglet0+VPMT.eIntegralSinglet1)/(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe):(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)/%f",conv);
  //  sprintf(varexp[0],"(VPMT.eIntegralSinglet0+VPMT.eIntegralSinglet1)/(PMT[0].eIntegral+PMT[1].eIntegral+PMT[2].eIntegral+PMT[3].eIntegral+PMT[4].eIntegral+PMT[5].eIntegral+PMT[6].eIntegral):(PMT[0].eIntegral+PMT[1].eIntegral+PMT[2].eIntegral+PMT[3].eIntegral+PMT[4].eIntegral+PMT[5].eIntegral+PMT[6].eIntegral)/%f",conv);
  //  sprintf(varexp[0],"VPMT.ePeakTime");
  sprintf(varexp[0],"(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)/%f",conv);
  
  TString total_selection;
  total_selection="";
  
  for (int isel=0;isel<100;isel++) sprintf(selection[isel],"");
  
  sprintf(selection[0],"1");
  //  sprintf(selection[1],"&&abs((PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)/0.2-300)<300");
  //  sprintf(selection[2],"&&abs((VPMT.eIntegralSinglet0+VPMT.eIntegralSinglet1)/(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)-1)<1");
  sprintf(selection[2],"&&abs((VPMT.eIntegralSinglet0+VPMT.eIntegralSinglet1)/(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)-0.775)<0.125");
  sprintf(selection[3],"&&(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)/0.2>10");
  sprintf(selection[4],"&&VPMT.ePeakTime<10000-4500");
  sprintf(selection[5],"&&(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)/0.2<200");
  //  sprintf(selection[4],"&&VPMT.ePeakTime<9000");
  //       sprintf(selection[1],"&&VPMT.ePeakIsSaturated==0",i);
  //  sprintf(selection[2],"&&(VPMT.eIntegralSinglet0+VPMT.eIntegralSinglet1)/(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)>0&&(VPMT.eIntegralSinglet0+VPMT.eIntegralSinglet1)/(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)<1");
  //  sprintf(selection[3],"&&VPMT.eIntegral>%f&&VPMT.eIntegral<%f",0,300);

  //       //      sprintf(selection[4],"PMT[0].eIntegral>1");
  //      for (int ic=0;ic<14;ic++) sprintf(selection[5+ic],"&&PMT[%d].ePeakValue>0.02",ic);
  //      for (int ic=0;ic<14;ic++) sprintf(selection[5+ic],"&&1");
  //      sprintf(selection[5],"&&1");
  
  for (int isel=0;isel<100;isel++) if (selection[isel]!="") total_selection.Append(selection[isel]);
  
  printf("%s\n",total_selection.Data());
  
  frame->Draw();
  //int entries = Data->Draw(varexp[0],total_selection.Data(),"same");
  int entries = Data->Draw(varexp[0],total_selection.Data(),"");
  printf("entries = %d\n",entries);
  TBox *CRIcut = new TBox(Integral_min/conv,CR_min,Integral_max/conv,CR_max);
  CRIcut->SetFillStyle(0);
  CRIcut->SetLineColor(4);
  //  CRIcut->Draw();
  c->Update();
  
}
