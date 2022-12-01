#define MAXRUNS    7
#define MAXPMTS    15

#define ConversionFactor 0.2

void ShowSeriePosition(const char *filename)
{
  gStyle->SetOptStat(0);


  FILE *fp;
  if ( (fp=fopen(filename,"r")) == 0 ) {
    printf("Error! File %s cannot be opened\n",filename);
    return;
  }

  string PMTName[MAXPMTS] = {"1:FA0020","2:FA0010", "3:FA0015", "4:FA0006", "5:FA0013", "6:FA0009", "7:FA0006",
			     "8:FA0018","9:FA0011","10:FA0007","11:FA0014","12:FA0016","13:FA0002","14:FA0001",
			     "15:VIRTUAL"};


  Int_t RunNumber[MAXRUNS],NPMTs[MAXRUNS];
  Double_t Timestamp[MAXRUNS],STimestamp[MAXRUNS];
  Double_t SourcePosition[MAXRUNS],SSourcePosition[MAXRUNS];

  Double_t A[MAXPMTS][MAXRUNS],SA[MAXPMTS][MAXRUNS],B[MAXPMTS][MAXRUNS],SB[MAXPMTS][MAXRUNS];
  Double_t sigma[MAXPMTS][MAXRUNS],Ssigma[MAXPMTS][MAXRUNS];
  Double_t tau1[MAXPMTS][MAXRUNS],Stau1[MAXPMTS][MAXRUNS],tau2[MAXPMTS][MAXRUNS],Stau2[MAXPMTS][MAXRUNS];
  Double_t t0[MAXPMTS][MAXRUNS],St0[MAXPMTS][MAXRUNS];
  Double_t C[MAXPMTS][MAXRUNS],SC[MAXPMTS][MAXRUNS],tau3[MAXPMTS][MAXRUNS],Stau3[MAXPMTS][MAXRUNS];

  Double_t Tot[MAXPMTS][MAXRUNS],STot[MAXPMTS][MAXRUNS];
  Double_t TotM[MAXPMTS][MAXRUNS],STotM[MAXPMTS][MAXRUNS];

  Int_t nruns,NPMTs;
  fscanf(fp,"%d",&nruns);

  for (int irun=0;irun<nruns;irun++) {

    fscanf(fp,"%d",&NPMTs[irun]);
    
    for (Int_t ipmt=0;ipmt<NPMTs[irun]+1;ipmt++) {
      
      Int_t run,pmt;

      fscanf(fp,"%d %d %d %lf %lf %lf",&run,&RunNumber[irun],&pmt,&Timestamp[irun],
	     &SourcePosition[irun],&SSourcePosition[irun]);
      fscanf(fp,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	     &A[ipmt][irun],&SA[ipmt][irun],
	     &B[ipmt][irun],&SB[ipmt][irun],
	     &sigma[ipmt][irun],&Ssigma[ipmt][irun],
	     &tau1[ipmt][irun],&Stau1[ipmt][irun],
	     &tau2[ipmt][irun],&Stau2[ipmt][irun]);
      fscanf(fp,"%lf %lf %lf %lf %lf %lf",
	     &t0[ipmt][irun],&St0[ipmt][irun],
	     &C[ipmt][irun],&SC[ipmt][irun],
	     &tau3[ipmt][irun],&Stau3[ipmt][irun]);


      printf("%d %d %d %f %lf %lf ",run,RunNumber[irun],pmt,Timestamp[irun],
	     SourcePosition[irun],SSourcePosition[irun]);
      printf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ",
	     A[ipmt][irun],SA[ipmt][irun],
	     B[ipmt][irun],SB[ipmt][irun],
	     sigma[ipmt][irun],Ssigma[ipmt][irun],
	     tau1[ipmt][irun],Stau1[ipmt][irun],
	     tau2[ipmt][irun],Stau2[ipmt][irun]);
      printf("%lf %lf %lf %lf %lf %lf\n",
	     t0[ipmt][irun],St0[ipmt][irun],
	     C[ipmt][irun],SC[ipmt][irun],
	     tau3[ipmt][irun],Stau3[ipmt][irun]);

      if (irun==0) start_time = Timestamp[0];

      SourcePosition[irun] = 20 - SourcePosition[irun];

      Timestamp[irun] -= start_time;
      Timestamp[irun] /= 60;
      STimestamp[irun] = 0;

      //      tau2[ipmt][irun] += Timestamp[irun]*0.056;

      A[ipmt][irun] /= ConversionFactor;
      SA[ipmt][irun] /= ConversionFactor;
      B[ipmt][irun] /= ConversionFactor;
      SB[ipmt][irun] /= ConversionFactor;
      C[ipmt][irun] /= ConversionFactor;
      SC[ipmt][irun] /= ConversionFactor;

      Tot[ipmt][irun] = A[ipmt][irun] + B[ipmt][irun];
      TotM[ipmt][irun] = B[ipmt][irun]*(3200./tau2[ipmt][irun]) + A[ipmt][irun];
      STot[ipmt][irun] = sqrt(SA[ipmt][irun]*SA[ipmt][irun] + SB[ipmt][irun]*SB[ipmt][irun]);
      STotM[ipmt][irun] = sqrt( SA[ipmt][irun]*SA[ipmt][irun] 
				+ (SB[ipmt][irun]*3200./tau2[ipmt][irun])**2
				+ (Stau2[ipmt][irun]*B[ipmt][irun]*3200./(tau2[ipmt][irun]*tau2[ipmt][irun]))**2 );

    }

  }

  fclose(fp);






  Double_t  Sqrt3 = TMath::Sqrt(3.);
  Double_t  Dist  = 0.1035;

  Double_t posX[14],posY[14];
  Int_t IdxMar[MAXPMTS];
  Int_t IdxCol[MAXPMTS];

  posX[0]  =-2*Sqrt3*Dist; posY[0]  =-1*Dist;    IdxMar[0]  = 20;   IdxCol[0]  = kGreen;
  posX[1]  =-2*Sqrt3*Dist; posY[1]  = 1*Dist;    IdxMar[1]  = 21;   IdxCol[1]  = kGreen;

  posX[2]  =-Sqrt3*Dist;   posY[2]  =-2*Dist;    IdxMar[2]  = 20;   IdxCol[2]  = kRed;
  posX[3]  =-Sqrt3*Dist;   posY[3]  = 0*Dist;    IdxMar[3]  = 20;   IdxCol[3]  = kBlue;
  posX[4]  =-Sqrt3*Dist;   posY[4]  = 2*Dist;    IdxMar[4]  = 21;   IdxCol[4]  = kRed;

  posX[5]  = 0;            posY[5]  =-3*Dist;    IdxMar[5]  = 22;   IdxCol[5]  = kGreen;
  posX[6]  = 0;            posY[6]  =-1*Dist;    IdxMar[6]  = 21;   IdxCol[6]  = kBlue;
  posX[7]  = 0;            posY[7]  = 1*Dist;    IdxMar[7]  = 22;   IdxCol[7]  = kBlue;
  posX[8]  = 0;            posY[8]  = 3*Dist;    IdxMar[8]  = 23;   IdxCol[8]  = kGreen;

  posX[9]  = Sqrt3*Dist;   posY[9]  =-2*Dist;    IdxMar[9]  = 22;   IdxCol[9]  = kRed;
  posX[10] = Sqrt3*Dist;   posY[10] = 0*Dist;    IdxMar[10] = 23;   IdxCol[10] = kBlue;
  posX[11] = Sqrt3*Dist;   posY[11] = 2*Dist;    IdxMar[11] = 23;   IdxCol[11] = kRed;

  posX[12] = 2*Sqrt3*Dist; posY[12] =-1*Dist;    IdxMar[12] = 24;   IdxCol[12] = kGreen;
  posX[13] = 2*Sqrt3*Dist; posY[13] = 1*Dist;    IdxMar[13] = 25;   IdxCol[13] = kGreen;

  
  IdxMar[14] = 20;   IdxCol[14] = kBlack;


  // Position distribution of light

  gStyle->SetPalette(200);
  gStyle->SetPaintTextFormat(".1f");
 

  TCanvas *ct= new TCanvas("ct","ct",1600,800);
  ct->Divide(4,2);
  
  for (Int_t irun=0;irun<nruns;irun++) {

    ct->cd(irun+1);

    //  TH2F *h2 = new TH2F("pos","pos",39,-5*Dist,+5*Dist,39,-5*Dist,+5*Dist);
    char name[100],namen[100];
    sprintf(name,"Position %d",SourcePosition[irun]);
    sprintf(namen,"PositionNumber %d",SourcePosition[irun]);
    TH2F *h2 = new TH2F(name,name,11,-5*Dist,+5*Dist,11,-5*Dist,+5*Dist);
    TH2F *h2n = new TH2F(namen,namen,11,-5*Dist,+5*Dist,11,-5*Dist,+5*Dist);

    Double_t Light[MAXPMTS];
    for (Int_t ipmt=0;ipmt<NPMTs[0];ipmt++) {
      h2->Fill(posX[ipmt],posY[ipmt],Tot[ipmt][irun]);
      h2n->Fill(posX[ipmt]+Dist,posY[ipmt],Tot[ipmt][irun]);
    }    

    h2->Fill(-5*Dist,-5*Dist,100.);

    Double_t mx=0,my=0,smx=0,smy=0,den=0;
    for (Int_t ipmt=0;ipmt<NPMTs[0];ipmt++)
      {
	mx += posX[ipmt]*Tot[ipmt][irun];
	my += posY[ipmt]*Tot[ipmt][irun];
	den += Tot[ipmt][irun];
      }

    mx /= den;
    my /= den;

    for (Int_t ipmt=0;ipmt<NPMTs[0];ipmt++)
      {
	smx += (posX[ipmt]-mx)*(posX[ipmt]-mx)*Tot[ipmt][irun];
	smy += (posY[ipmt]-my)*(posY[ipmt]-my)*Tot[ipmt][irun];
      }

    smx = sqrt(smx/den);
    smy = sqrt(smy/den);
    
    h2->Draw("colz");
    h2n->Draw("text,same");
    h2n->SetMarkerColor(kBlack);
    h2n->SetMarkerSize(2);
    TArrow *a = new TArrow(0,0,mx,my,0.005,"|>");
    a->Draw();
    TEllipse *sa = new TEllipse(mx,my,smx,smy);
    sa->SetFillStyle(0);
    sa->Draw();

    //    h2->SetBinContent(1,1,0);
    ct->Update();
  }

  return;

  //  TLegend *legt = new TLegend(0.12,0.58,0.22,0.88);

  TH2F *framet = new TH2F("framet","framet",100,Timestamp[0]-60,Timestamp[nruns-1]+60,100,2400,3200);
  framet->SetTitle("Slow Component Decay Time vs. Time");
  framet->GetXaxis()->SetTitle("Time [min]");
  framet->GetYaxis()->SetTitle("Decay time of the slow component #tau_{2} [ns]");
  framet->Draw();

  for (Int_t ch=0;ch<NPMTs[0]+1;ch++){
    ct->cd(1);

    gt[ch]->Draw("PL");
    gt[ch]->SetMarkerColor(IdxCol[ch]);
    gt[ch]->SetMarkerStyle(IdxMar[ch]);
    if (ch==NPMTs[0]) gt[ch]->SetLineWidth(4);
    legt->AddEntry(gt[ch], PMTName[ch].c_str(),"P");
    ct->Update(); 
  }
  legt->Draw();

  Float_t dt = Timestamp[nruns-1] - Timestamp[0] + 60*2;
  Float_t dy = 3200-2400;

  Float_t x0 = Timestamp[0] - 60 + 1.*dt/4.;
  Float_t y0 = 2400 + 1.*dy/4.;
  Float_t scale = 0.2;
  Float_t radius = 0.1;

  TEllipse *el = new TEllipse(x0,y0,radius*dt,radius*dy);
  el->Draw();
  for (Int_t ch=0;ch<NPMTs[0];ch++){
    TMarker *m = new TMarker(x0+scale*posX[ch]*dt,y0+scale*posY[ch]*dy,IdxMar[ch]);
    m->SetMarkerColor(IdxCol[ch]);
    m->Draw();
  }

  ct->Update();



  for (int irun=0;irun<nruns;irun++)
    for (Int_t ipmt=0;ipmt<NPMTs[irun]+1;ipmt++)
      tau2[ipmt][irun] += (Timestamp[irun]+0.)*3.31;



  //Tau2 dependence on time: Slope correction
  TCanvas *ctv= new TCanvas("ctv","ctv",800,800);
  TGraphErrors *gtv[MAXPMTS];
  TLegend *legtv = new TLegend(0.12,0.58,0.22,0.88);

  TH2F *frametv = new TH2F("frametv","frametv",100,Timestamp[0]-60,Timestamp[nruns-1]+60,100,2800,3200);
  frametv->SetTitle("Slow Component Decay Time vs. Time");
  frametv->GetXaxis()->SetTitle("Time [min]");
  frametv->GetYaxis()->SetTitle("Decay time of the slow component #tau_{2} [ns]");
  frametv->Draw();

  for (Int_t ch=0;ch<NPMTs[0]+1;ch++){
    ctv->cd(1);
    gtv[ch]=new TGraphErrors(nruns,Timestamp,&tau2[ch][0],STimestamp,&Stau2[ch][0]);
    gtv[ch]->Draw("PL");
    gtv[ch]->SetMarkerColor(IdxCol[ch]);
    gtv[ch]->SetMarkerStyle(IdxMar[ch]);
    if (ch==NPMTs[0]) gtv[ch]->SetLineWidth(4);
    legtv->AddEntry(gtv[ch], PMTName[ch].c_str(),"P");
    ctv->Update(); 
  }
  legtv->Draw();

  Float_t dt = Timestamp[nruns-1] - Timestamp[0] + 60*2;
  Float_t dy = 3200-2800;

  Float_t x0 = Timestamp[0] - 60 + 1.*dt/4.;
  Float_t y0 = 2800 + 1.*dy/4.;
  Float_t scale = 0.2;
  Float_t radius = 0.1;

  el = new TEllipse(x0,y0,radius*dt,radius*dy);
  el->Draw();
  for (Int_t ch=0;ch<NPMTs[0];ch++){
    TMarker *m = new TMarker(x0+scale*posX[ch]*dt,y0+scale*posY[ch]*dy,IdxMar[ch]);
    m->SetMarkerColor(IdxCol[ch]);
    m->Draw();
  }

  ctv->Update();






  // Total Light Yield vs Position
  TCanvas *cl= new TCanvas("cl","cl",800,800);
  TGraphErrors *gl[MAXPMTS];
  TLegend  *legl = new TLegend(0.12,0.58,0.22,0.88);

  //  TH2F *framel = new TH2F("framel","framel",100,SourcePosition[0]-10,SourcePosition[nruns-1]+10,100,0,200);
  TH2F *framel = new TH2F("framel","framel",100,SourcePosition[nruns-1]-10,SourcePosition[0]+10,100,0,100);
  framel->SetTitle("Total Light Yield vs Position");
  framel->GetXaxis()->SetTitle("Actuator position [cm]");
  framel->GetYaxis()->SetTitle("Total light (extr) [pe]");
  framel->Draw();

  for (Int_t ch=0;ch<NPMTs[0];ch++){
    cl->cd(1);
    gl[ch]=new TGraphErrors(nruns,SourcePosition,&TotM[ch][0],SSourcePosition,&STotM[ch][0]);
    gl[ch]->Draw("P");
    gl[ch]->SetMarkerColor(IdxCol[ch]);
    gl[ch]->SetMarkerStyle(IdxMar[ch]);
    legl->AddEntry(gl[ch], PMTName[ch].c_str(),"P");
    cl->Update();
  }
  legl->Draw();

  x0 = 30.;
  y0 = 15.;
  scale = 0.2;
  radius = 0.1;

  el = new TEllipse(x0,y0,radius*80,radius*100);
  el->Draw();
  for (Int_t ch=0;ch<NPMTs[0];ch++){
    //    TEllipse *el = new TEllipse(x0+scale*posX[ch]*80.,y0+scale*posY[ch]*2000.,radius*80,radius*2000);
    TMarker *m = new TMarker(x0+scale*posX[ch]*80.,y0+scale*posY[ch]*100.,IdxMar[ch]);
    m->SetMarkerColor(IdxCol[ch]);
    m->Draw();
//     el->SetFillStyle(1001);
//     el->SetFillColor(IdxCol[ch]);
//     el->SetLineColor(IdxCol[ch]);
//     el->Draw();
  }

  cl->Update();






  // Total Light Yield vs Position
  TCanvas *clv= new TCanvas("clv","clv",800,800);
  TGraphErrors *glv[MAXPMTS];
  TLegend  *leglv = new TLegend(0.12,0.58,0.22,0.88);

  //  TH2F *framelv = new TH2F("framelv","framelv",100,SourcePosition[0]-10,SourcePosition[nruns-1]+10,100,0,200);
  TH2F *framelv = new TH2F("framelv","framelv",100,SourcePosition[nruns-1]-10,SourcePosition[0]+10,100,0,1600);
  framelv->SetTitle("Total Light Yield vs Position");
  framelv->GetXaxis()->SetTitle("Actuator position [cm]");
  framelv->GetYaxis()->SetTitle("Total light (extr) [pe]");

  Float_t rightmax = 1.1*framelv->GetYaxis()->GetXmax();
  Float_t scale = gPad->GetUymax()/rightmax;
  framelv->SetLineColor(kRed);
  framelv->Scale(scale);
  framelv->Draw("");

  Int_t ch=NPMTs[0];

  clv->cd(1);
  glv[ch]=new TGraphErrors(nruns,SourcePosition,&TotM[ch][0],SSourcePosition,&STotM[ch][0]);
  glv[ch]->Draw("P");
  glv[ch]->SetMarkerColor(IdxCol[ch]);
  glv[ch]->SetMarkerStyle(IdxMar[ch]);
  leglv->AddEntry(glv[ch], PMTName[ch].c_str(),"P");
  clv->Update();

  leglv->Draw();

  clv->Update();

}
