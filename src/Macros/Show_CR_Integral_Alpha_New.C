#define MAXPMTS    16

Float_t CR_min = 0.1;
Float_t CR_max = 0.25;
Float_t Integral_min = 300.*0.2.; // nVs
Float_t Integral_max = 700.*0.2; // nVs

void Show_CR_Integral(const char *filename, int pe = 0, float conv = 0.2)
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
  Int_t sqrtn = (int)(sqrt(NPMTs)+0.5);
  c->Divide(sqrtn,sqrtn);
  Data->SetMarkerStyle(1);

  char varexp[100][1000],selection[100][1000];

  Float_t  Sqrt3 = TMath::Sqrt(3.);
  Float_t  Dist  = 0.1035;

  float posX[14],posY[14];

  posX[0]  =-2*Sqrt3*Dist; posY[0]  =-1*Dist;
  posX[1]  =-2*Sqrt3*Dist; posY[1]  = 1*Dist;

  posX[2]  =-Sqrt3*Dist;   posY[2]  =-2*Dist;
  posX[3]  =-Sqrt3*Dist;   posY[3]  = 0*Dist;
  posX[4]  =-Sqrt3*Dist;   posY[4]  = 2*Dist;

  posX[5]  = 0;            posY[5]  =-3*Dist;
  posX[6]  = 0;            posY[6]  =-1*Dist;
  posX[7]  = 0;            posY[7]  = 1*Dist;
  posX[8]  = 0;            posY[8]  = 3*Dist;

  posX[9]  = Sqrt3*Dist;   posY[9]  =-2*Dist;
  posX[10] = Sqrt3*Dist;   posY[10] = 0*Dist;
  posX[11] = Sqrt3*Dist;   posY[11] = 2*Dist;

  posX[12] = 2*Sqrt3*Dist; posY[12] =-1*Dist;
  posX[13] = 2*Sqrt3*Dist; posY[13] = 1*Dist;
  
  Float_t SizePad = Dist*1.7;

  TH2F *frame[MAXPMTS+1];
  char framename[100];

  for (Int_t i=0;i<NPMTs;i++)
    {
      c->cd(i+1);

      c->cd(i+1)->SetPad(0.5+posX[i]-SizePad/2,0.5+posY[i]-SizePad/2,0.5+posX[i]+SizePad/2,0.5+posY[i]+SizePad/2);

      sprintf(framename,"PMT_%02d",i+1);
      frame[i] = new TH2F(framename, framename, 100, 0/conv, 3/conv, 100, 0, 2);
      if (!pe) frame[i]->GetXaxis()->SetTitle("Integral [nVs]");
      else frame[i]->GetXaxis()->SetTitle("Integral [pe]");
      frame[i]->GetYaxis()->SetTitle("L_{50}/L_{tot}");

      sprintf(varexp[0],"(PMT[%d].eIntegralSinglet0+PMT[%d].eIntegralSinglet1)/PMT[%d].eIntegralPe:PMT[%d].eIntegralPe/%f",i,i,i,i,conv);

      TString total_selection;
      total_selection="";

      for (int isel=0;isel<100;isel++) sprintf(selection[isel],"");
      sprintf(selection[0],"1");
      sprintf(selection[1],"&&PMT[%d].ePeakIsSaturated==0",i);
//       sprintf(selection[2],"&&PMT[%d].eCR>0&&PMT[%d].eCR<1",i,i);
//       sprintf(selection[2],"&&PMT[%d].eNpe>2",i);
//       sprintf(selection[3],"&&PMT[%d].eIntegral>0&&PMT[%d].eIntegral<30",i,i);
//      sprintf(selection[4],"&&VPMT.eCR>%f&&VPMT.eCR<%f",CR_min,CR_max);
//      sprintf(selection[4],"&&(PMT[%d].eIntegralSinglet0+PMT[%d].eIntegralSinglet1)/PMT[%d].eIntegralPe>%f&&(PMT[%d].eIntegralSinglet0+PMT[%d].eIntegralSinglet1)/PMT[%d].eIntegralPe<%f",i,i,i,CR_min,i,i,i,CR_max);
//      sprintf(selection[4],"&&(PMT[0].eIntegralSinglet0+PMT[0].eIntegralSinglet1)/PMT[0].eIntegralPe>%f&&(PMT[0].eIntegralSinglet0+PMT[0].eIntegralSinglet1)/PMT[0].eIntegralPe<%f&&PMT[0].eIntegralPe<2",0.9,1.1);
//      sprintf(selection[5],"&&VPMT.eIntegral>%f&&VPMT.eIntegral<%f",Integral_min,Integral_max);
      //      sprintf(selection[4],"PMT[0].eIntegral>1");
      //      for (int ic=0;ic<14;ic++) sprintf(selection[5+ic],"&&PMT[%d].ePeakValue>0.02",ic);
      //      sprintf(selection[5],"&&1");

      for (int isel=0;isel<100;isel++) if (selection[isel]!="") total_selection.Append(selection[isel]);

      printf("%s\n",total_selection.Data());

      frame[i]->Draw();
      Int_t entries = Data->Draw(varexp[0],total_selection.Data(),"same");
      printf("Selected %d events\n",entries);
      c->Update();

    }

  c->cd(NPMTs+1);

  c->cd(NPMTs+1)->SetPad(1-SizePad,0,1,SizePad);
  //  c->cd(NPMTs+1)->SetFillColor(30);

  sprintf(framename,"VPMT",i);

  frame[NPMTs] = new TH2F(framename, framename, 100, 0/conv, 200/conv, 100, 0, 1);
  if (!pe) frame[NPMTs]->GetXaxis()->SetTitle("Integral [nVs]");
  else frame[NPMTs]->GetXaxis()->SetTitle("Integral [pe]");
  frame[NPMTs]->GetYaxis()->SetTitle("L_{50}/L_{tot}");

  sprintf(varexp[0],"(VPMT.eIntegralSinglet0+VPMT.eIntegralSinglet1)/(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe+PMT[7].eIntegralPe+PMT[8].eIntegralPe+PMT[9].eIntegralPe+PMT[10].eIntegralPe+PMT[11].eIntegralPe+PMT[12].eIntegralPe+PMT[13].eIntegralPe):(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe+PMT[7].eIntegralPe+PMT[8].eIntegralPe+PMT[9].eIntegralPe+PMT[10].eIntegralPe+PMT[11].eIntegralPe+PMT[12].eIntegralPe+PMT[13].eIntegralPe)/%f",conv);

  
  TString total_selection;
  total_selection="";
  
  for (int isel=0;isel<100;isel++) sprintf(selection[isel],"");
  
  sprintf(selection[0],"1");
  sprintf(selection[1],"&&1");
  sprintf(selection[2],"&&1");
  sprintf(selection[3],"&&1");
  sprintf(selection[4],"&&1");
  //       sprintf(selection[1],"&&VPMT.ePeakIsSaturated==0",i);
  //  sprintf(selection[2],"&&(VPMT.eIntegralSinglet0+VPMT.eIntegralSinglet1)/(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)>0&&(VPMT.eIntegralSinglet0+VPMT.eIntegralSinglet1)/(PMT[0].eIntegralPe+PMT[1].eIntegralPe+PMT[2].eIntegralPe+PMT[3].eIntegralPe+PMT[4].eIntegralPe+PMT[5].eIntegralPe+PMT[6].eIntegralPe)<1");
  //  sprintf(selection[3],"&&VPMT.eIntegral>%f&&VPMT.eIntegral<%f",0,300);

  //       //      sprintf(selection[4],"PMT[0].eIntegral>1");
  //      for (int ic=0;ic<14;ic++) sprintf(selection[5+ic],"&&PMT[%d].ePeakValue>0.02",ic);
  //      for (int ic=0;ic<14;ic++) sprintf(selection[5+ic],"&&1");
  //      sprintf(selection[5],"&&1");
  
  for (int isel=0;isel<100;isel++) if (selection[isel]!="") total_selection.Append(selection[isel]);
  
  printf("%s\n",total_selection.Data());
  
  frame[NPMTs]->Draw();
  Data->Draw(varexp[0],total_selection.Data(),"same");
  TBox *CRIcut = new TBox(Integral_min/conv,CR_min,Integral_max/conv,CR_max);
  CRIcut->SetFillStyle(0);
  CRIcut->SetLineColor(4);
  CRIcut->Draw();
  c->Update();
  
  c->cd(NPMTs+2);
  
  c->cd(NPMTs+2)->SetPad(0,1-SizePad/10,SizePad/10,1);
}
