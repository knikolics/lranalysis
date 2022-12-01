#define MAXPMTS    14
#define MAXRUN     100

TTree *CalibTree;



void PlotCalibrationVSRun(const char *filename)
{
  gStyle->SetOptFit(11111);

  FILE *fp;
  if ( (fp=fopen(filename,"r")) == NULL ) {
    printf("Error! File %s not found\n",filename);
    return;
  }

  Float_t run[MAXRUN],time[MAXRUN],position[MAXRUN];
  Float_t cal[MAXPMTS][MAXRUN],ecal[MAXPMTS][MAXRUN];
  Float_t runmin=99999,runmax=0;
  Float_t timemin=99999,timemax=0;
  Int_t color[MAXPMTS] = {1,2,3,4,5,6,7,1,2,3,4,5,6,7};
  Int_t style[MAXPMTS] = {1,1,1,1,1,1,1,2,2,2,2,2,2,2};

  Int_t NPMTs = MAXPMTS;

  Int_t nruns=0;
  while (!feof(fp)) {
    Float_t drun,dtime,dposition;
    if (fscanf(fp,"%f %f %f",&drun,&dtime,&dposition)!=3) break;
    run[nruns]=drun;
    time[nruns]=dtime;
    position[nruns]=dposition;
    if (drun<runmin) runmin=drun;
    if (drun>runmax) runmax=drun;
    if (dtime<timemin) timemin=dtime;
    if (dtime>timemax) timemax=dtime;
    for (int ipmt=0;ipmt<NPMTs;ipmt++) fscanf(fp,"%f %f",&cal[ipmt][nruns],&ecal[ipmt][nruns]);
    nruns++;
  }

  printf("Number of runs = %d, number of PMTs = %d\n",nruns,NPMTs);

  TCanvas *c = new TCanvas("Calibration behaviour VS run","Calibration behavviour VS run");

  char varexp[100],selection[100];
  //  Float_t *ical = cal[0];
  
  TH2F *frame = new TH2F("frame","frame",100,runmin-2,runmax+2,100,0,0.4);
  frame->Draw();

  for (Int_t ipmt=0;ipmt<NPMTs;ipmt++)
    {
      TGraphErrors *ge = new TGraphErrors(nruns,run,cal[ipmt],0,ecal[ipmt]);
      ge->SetMarkerStyle(6);
      ge->SetLineWidth(2);
      ge->SetLineColor(color[ipmt]);
      ge->SetLineStyle(style[ipmt]);
      ge->Draw("PL");
    }


  Float_t meancal[MAXPMTS],RMScal[MAXPMTS];

  for (Int_t ipmt=0;ipmt<NPMTs;ipmt++)
    {
      meancal[ipmt]=0;
      RMScal[ipmt]=0;
      for (Int_t irun=0;irun<nruns;irun++)
	{
	  meancal[ipmt] += cal[ipmt][irun];
	  RMScal[ipmt]  += cal[ipmt][irun]*cal[ipmt][irun];
	}
      meancal[ipmt] /= nruns;
      RMScal[ipmt]  /= nruns;
      RMScal[ipmt]  -= meancal[ipmt]*meancal[ipmt];
      RMScal[ipmt]   = sqrt(RMScal[ipmt]);
    }

  //  Float_t CLS[MAXPMTS] = {48,48,52,55,51,50,53,54,51,51,56,52,47,44}; // Cathode Luminous Sensitivity
  Float_t PMTname[MAXPMTS] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};

  TCanvas *cpmt = new TCanvas("Calibration VS PMT","Calibration VS PMT");

  TH2F *framepmt = new TH2F("framepmt","framepmt",16,0,15,100,0,0.4);
  framepmt->Draw();

  //  TGraphErrors *ge = new TGraphErrors(nruns,CLS,meancal,0,RMScal);
  TGraphErrors *ge = new TGraphErrors(NPMTs,PMTname,meancal);

  ge->SetMarkerStyle(6);
  ge->SetLineWidth(2);
  ge->Draw("B");

  Float_t mean_calibration = 0;
  Float_t rms_calibration = 0;
  for (Int_t ipmt=0;ipmt<NPMTs;ipmt++)
    {
      mean_calibration += meancal[ipmt];
      rms_calibration  += meancal[ipmt]*meancal[ipmt];
    }
  mean_calibration /= NPMTs;
  rms_calibration  /= NPMTs;
  rms_calibration  -= mean_calibration*mean_calibration;
  rms_calibration   = sqrt(rms_calibration);

  printf("Mean conversion factor over %d PMTs and %d runs: %.3f +- %.3f nVs/pe\n",
	 NPMTs,nruns,mean_calibration,rms_calibration);


//   TCanvas *ct = new TCanvas("Calibration behaviour VS time","Calibration behavviour VS time");

//   TH2F *framet = new TH2F("framet","framet",10000,timemin-2,timemax+2,100,0,0.8);
//   framet->Draw();

//   for (Int_t ipmt=0;ipmt<NPMTs;ipmt++)
//     {
//       TGraphErrors *ge = new TGraphErrors(nruns,time,cal[ipmt],0,ecal[ipmt]);
//       ge->SetMarkerStyle(6);
//       ge->SetLineWidth(2);
//       ge->SetLineColor(color[ipmt]);
//       ge->SetLineStyle(style[ipmt]);
//       ge->Draw("PL");
//     }

}
