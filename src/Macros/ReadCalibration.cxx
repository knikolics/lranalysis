#define NPMTS 14
#define NDATARUNS 40
#define NLEDRUNS 23

const int DATANumber[NDATARUNS] = { 3565,3628,3688,3704,3734,3773,3831,3851,3908,3938, 4000,4033,4058,4069,4084,4108,4130,4147,4178,4234, 4250,4265,4313,4329,4400,4826,4897,4965,5035,5086, 5254,5319,5383,5447,5512,5576,5640,5738,5758,5880};
const int LEDNumber[NLEDRUNS] = { 3545,3992,3997,4055,4208,4211,4214,4221,4224,4227, 4243,4247,4262,4453,4761,4821,5032,5173,5251,5731, 5875,6062,6066};

Int_t GetCalibration(Int_t runNumber,Float_t *mean, Float_t *sigma, Float_t* meanError, Float_t *sigmaError,  UInt_t &time)
{
  char fileName[256];
  sprintf(fileName,"/home/dlussi/ArDMrepos/LRAnalysis/devel/AnalysisData/Run%04d/calibration.txt",runNumber);
  //printf("%s\n", fileName);
  char buffer[512];
  Int_t tmp;
  FILE * file = fopen(fileName, "r");
  if (file == NULL) 
    {
      perror("ERROR");
      return 0;
    }
  fgets(buffer,512,file);
  fscanf(file,"%lu %lu",&tmp, &time );
  for (Int_t ii=0; ii<NPMTS; ii++)
    fscanf(file,"%f %f %f %f",mean+ii,meanError+ii,sigma+ii,sigmaError+ii);
  fclose(file);
}

void Print(Int_t runNumber)
{
  UInt_t time;
  Float_t mean[NPMTS], sigma[NPMTS], meanError[NPMTS], sigmaError[NPMTS];
  GetCalibration(runNumber,mean,sigma,meanError,sigmaError,time);
  for (Int_t ii=0; ii<NPMTS; ii++)
    cout << "pmt_" << ii << "  spe(mean)=" << mean[ii] << "+/-" << meanError[ii] << ", spe(sigma)=" << sigma[ii] << "+/-" << sigmaError[ii] << " time="  << time << endl;
}

void PlotCalibration(void)
{
  gROOT->SetStyle("Plain");
  
  Float_t timeLED[NLEDRUNS], timeErrorLED[NLEDRUNS];
  Float_t timeDATA[NDATARUNS], timeErrorDATA[NDATARUNS];
  Float_t mean[NPMTS], sigma[NPMTS], meanError[NPMTS], sigmaError[NPMTS];
  
  Float_t lMean[NPMTS][NDATARUNS], lSigma[NPMTS][NDATARUNS], lMeanError[NPMTS][NDATARUNS], lSigmaError[NPMTS][NDATARUNS], lRes[NPMTS][NDATARUNS], lResError[NPMTS][NDATARUNS];
  Float_t dMean[NPMTS][NDATARUNS], dSigma[NPMTS][NDATARUNS], dMeanError[NPMTS][NDATARUNS], dSigmaError[NPMTS][NDATARUNS], dRes[NPMTS][NDATARUNS], dResError[NPMTS][NDATARUNS];

  TH1F* resLED = new TH1F("resLED", "Resolution histogram (LED)",100,0,1.);
  TH1F* resDATA = new TH1F("resDATA", "Resolution histogram (DATA)",100,0,1.);
  TH1F* resLEDDATA = new TH1F("resLEDDATA", "Resolution histogram (LED/DATA)",100,0,1.);
  TGraphErrors *grMeanLED[NPMTS];
  TGraphErrors *grMeanDATA[NPMTS];
  TGraphErrors *grSigmaLED[NPMTS];
  TGraphErrors *grSigmaDATA[NPMTS];
  TGraphErrors *grResLED[NPMTS];
  TGraphErrors *grResDATA[NPMTS];

  
  for (int kk=0; kk<NLEDRUNS; kk++)
    {
      UInt_t time;
      //cout << kk << " " <<LEDNumber[kk]<< endl;
      timeErrorLED[kk]=0;
      GetCalibration(LEDNumber[kk],mean,sigma,meanError,sigmaError,time);
      timeLED[kk] = (Float_t)time;
      for (int ii=0; ii<NPMTS; ii++)
	{
	  lMean[ii][kk]=mean[ii];
	  lSigma[ii][kk]=sigma[ii];
	  lSigmaError[ii][kk]=sigmaError[ii];
	  lMeanError[ii][kk]=meanError[ii];
	  lRes[ii][kk]=sigma[ii]/mean[ii];
	  lResError[ii][kk]=sqrt((sigmaError[ii]/mean[ii])**2+(sigma[ii]*meanError[ii]/mean[ii]**2)**2);
	  resLED->Fill(lRes[ii][kk]);
	  resLEDDATA->Fill(lRes[ii][kk]);	
	}
    }
 for (int kk=0; kk<NDATARUNS; kk++)
    {
      UInt_t time;
      //cout << kk << " " <<DATANumber[kk]<< endl;
      timeErrorDATA[kk]=0;
      GetCalibration(DATANumber[kk],mean,sigma,meanError,sigmaError,time);
      timeDATA[kk] = (Float_t)time;
      for (int ii=0; ii<NPMTS; ii++)
	{
	  dMean[ii][kk]=mean[ii];
	  dSigma[ii][kk]=sigma[ii];
	  dSigmaError[ii][kk]=sigmaError[ii];
	  dMeanError[ii][kk]=meanError[ii];
	  dRes[ii][kk]=sigma[ii]/mean[ii];
	  dResError[ii][kk]=sqrt((sigmaError[ii]/mean[ii])**2+(sigma[ii]*meanError[ii]/mean[ii]**2)**2);
	  resDATA->Fill(dRes[ii][kk]);
	  resLEDDATA->Fill(dRes[ii][kk]);
	}
    }
 
   

  for (Int_t ii=0; ii<NPMTS; ii++)
    {
      grMeanLED[ii] = new TGraphErrors(NLEDRUNS,timeLED,lMean[ii],timeErrorLED,lMeanError[ii]);   
      grMeanDATA[ii] = new TGraphErrors(NDATARUNS,timeDATA,dMean[ii],timeErrorDATA,dMeanError[ii]);   
      grSigmaLED[ii] = new TGraphErrors(NLEDRUNS,timeLED,lSigma[ii],timeErrorLED,lSigmaError[ii]);   
      grSigmaDATA[ii] = new TGraphErrors(NDATARUNS,timeDATA,dSigma[ii],timeErrorDATA,dSigmaError[ii]);   
      grResLED[ii] = new TGraphErrors(NLEDRUNS,timeLED,lRes[ii],timeErrorLED,lResError[ii]);   
      grResDATA[ii] = new TGraphErrors(NDATARUNS,timeDATA,dRes[ii],timeErrorDATA,dResError[ii]);   
    }
  
  TCanvas *c1 = new TCanvas("c1","Calibration Evolution (data)",200,10,1400,1000);
  c1->Divide(2,2);
  c1->cd(1);
  for (Int_t ii=0;ii<NPMTS; ii++)
    {
      grMeanDATA[ii]->SetLineColor(2+ii);
      if (ii==0) {
	grMeanDATA[ii]->Draw("AP");
	grMeanDATA[ii]->GetYaxis()->SetRangeUser(0,0.3);
	grMeanDATA[ii]->SetTitle("Data calibration (mean value)");
	grMeanDATA[ii]->GetXaxis()->SetTimeDisplay(1);
	grMeanDATA[ii]->GetXaxis()->SetTitle("time");
	grMeanDATA[ii]->GetYaxis()->SetTitle("integral (nVs)");
	grMeanDATA[ii]->GetYaxis()->SetTitleOffset(1.3);
      }
      else grMeanDATA[ii]->Draw("PSAME");
    }
  c1->cd(2);
  for (Int_t ii=0;ii<NPMTS; ii++)
    {
      grSigmaDATA[ii]->SetLineColor(2+ii);
      if (ii==0) {
	grSigmaDATA[ii]->Draw("AP");
	grSigmaDATA[ii]->GetYaxis()->SetRangeUser(0,0.13);
	grSigmaDATA[ii]->SetTitle("Data calibration (sigma)");
	grSigmaDATA[ii]->GetXaxis()->SetTimeDisplay(1);
	grSigmaDATA[ii]->GetXaxis()->SetTitle("time");
	grSigmaDATA[ii]->GetYaxis()->SetTitle("sigma (nVs)");
	grSigmaDATA[ii]->GetYaxis()->SetTitleOffset(1.3);
	
      }
      else grSigmaDATA[ii]->Draw("PSAME");
    }
  c1->cd(3);
  for (Int_t ii=0;ii<NPMTS; ii++)
    {
      grResDATA[ii]->SetLineColor(2+ii);
      if (ii==0) {
	grResDATA[ii]->Draw("AP");
	grResDATA[ii]->GetYaxis()->SetRangeUser(0,0.5);
	grResDATA[ii]->SetTitle("Data calibration (resolution)");
	grResDATA[ii]->GetXaxis()->SetTimeDisplay(1);
	grResDATA[ii]->GetXaxis()->SetTitle("time");
	grResDATA[ii]->GetYaxis()->SetTitle("sigma/mean (nVs)");
	grResDATA[ii]->GetYaxis()->SetTitleOffset(1.3);
      }
      else grResDATA[ii]->Draw("PSAME");
    }
  c1->cd(4);
  resDATA->Draw();
  
  TCanvas *c2 = new TCanvas("c2","Calibration Evolution (LED)",200,10,1400,1000);
  c2->Divide(2,2);
  c2->cd(1);
  for (Int_t ii=0;ii<NPMTS; ii++)
    {
      grMeanLED[ii]->SetLineColor(2+ii);
      if (ii==0) {
	grMeanLED[ii]->Draw("AP");
	grMeanLED[ii]->GetYaxis()->SetRangeUser(0,0.3);
	grMeanLED[ii]->SetTitle("LED calibration (mean value)");
	grMeanLED[ii]->GetXaxis()->SetTimeDisplay(1);
	grMeanLED[ii]->GetXaxis()->SetTitle("time");
	grMeanLED[ii]->GetYaxis()->SetTitle("integral (nVs)");
	grMeanLED[ii]->GetYaxis()->SetTitleOffset(1.3);
      }
      else grMeanLED[ii]->Draw("PSAME");
    }
  c2->cd(2);
  for (Int_t ii=0;ii<NPMTS; ii++)
    {
      grSigmaLED[ii]->SetLineColor(2+ii);
      if (ii==0) {
	grSigmaLED[ii]->Draw("AP");
	grSigmaLED[ii]->GetYaxis()->SetRangeUser(0,0.13);
	grSigmaLED[ii]->SetTitle("LED calibration (sigma)");
	grSigmaLED[ii]->GetXaxis()->SetTimeDisplay(1);
	grSigmaLED[ii]->GetXaxis()->SetTitle("time");
	grSigmaLED[ii]->GetYaxis()->SetTitle("sigma (nVs)");
	grSigmaLED[ii]->GetYaxis()->SetTitleOffset(1.3);
	
      }
      else grSigmaLED[ii]->Draw("PSAME");
    }
  c2->cd(3);
  for (Int_t ii=0;ii<NPMTS; ii++)
    {
      grResLED[ii]->SetLineColor(2+ii);
      if (ii==0) {
	grResLED[ii]->Draw("AP");
	grResLED[ii]->GetYaxis()->SetRangeUser(0,0.5);
	grResLED[ii]->SetTitle("LED calibration (resolution)");
	grResLED[ii]->GetXaxis()->SetTimeDisplay(1);
	grResLED[ii]->GetXaxis()->SetTitle("time");
	grResLED[ii]->GetYaxis()->SetTitle("sigma/mean (nVs)");
	grResLED[ii]->GetYaxis()->SetTitleOffset(1.3);
      }
      else grResLED[ii]->Draw("PSAME");
    }
  c2->cd(4);
  resLED->Draw();

  TCanvas *c3 = new TCanvas("c3","Calibration Evolution (LED+data)",200,10,1400,1000);
  c3->Divide(2,2);
  c3->cd(1);
  for (Int_t ii=0;ii<NPMTS; ii++)
    {
      grMeanLED[ii]->SetLineColor(2+ii);
      grMeanDATA[ii]->SetLineColor(2+ii);
      if (ii==0) {
	grMeanLED[ii]->Draw("AP");
	grMeanLED[ii]->GetYaxis()->SetRangeUser(0,0.3);
	grMeanLED[ii]->SetTitle("LED/data calibration (mean value)");
	grMeanLED[ii]->GetXaxis()->SetTimeDisplay(1);
	grMeanLED[ii]->GetXaxis()->SetTitle("time");
	grMeanLED[ii]->GetYaxis()->SetTitle("integral (nVs)");
	grMeanLED[ii]->GetYaxis()->SetTitleOffset(1.3);
	grMeanDATA[ii]->Draw("PSAME");
      }
      else 
	{
	  grMeanLED[ii]->Draw("PSAME");
	  grMeanDATA[ii]->Draw("PSAME");
	}
    }
  c3->cd(2);
  for (Int_t ii=0;ii<NPMTS; ii++)
    {
      grSigmaLED[ii]->SetLineColor(2+ii);
      grSigmaDATA[ii]->SetLineColor(2+ii);
      if (ii==0) {
	grSigmaLED[ii]->Draw("AP");
	grSigmaLED[ii]->GetYaxis()->SetRangeUser(0,0.13);
	grSigmaLED[ii]->SetTitle("LED/data calibration (sigma)");
	grSigmaLED[ii]->GetXaxis()->SetTimeDisplay(1);
	grSigmaLED[ii]->GetXaxis()->SetTitle("time");
	grSigmaLED[ii]->GetYaxis()->SetTitle("sigma (nVs)");
	grSigmaLED[ii]->GetYaxis()->SetTitleOffset(1.3);
	grSigmaDATA[ii]->Draw("PSAME");
      }
      else 
	{grSigmaLED[ii]->Draw("PSAME");
	  grSigmaDATA[ii]->Draw("PSAME");
	}
    }
  c3->cd(3);
  for (Int_t ii=0;ii<NPMTS; ii++)
    {
      grResLED[ii]->SetLineColor(2+ii);
      grResDATA[ii]->SetLineColor(2+ii);
      if (ii==0) {
	grResLED[ii]->Draw("AP");
	grResLED[ii]->GetYaxis()->SetRangeUser(0,0.5);
	grResLED[ii]->SetTitle("LED/data calibration (resolution)");
	grResLED[ii]->GetXaxis()->SetTimeDisplay(1);
	grResLED[ii]->GetXaxis()->SetTitle("time");
	grResLED[ii]->GetYaxis()->SetTitle("sigma/mean (nVs)");
	grResLED[ii]->GetYaxis()->SetTitleOffset(1.3);
	grResDATA[ii]->Draw("PSAME");
      }
      else 
	{
	  grResLED[ii]->Draw("PSAME");
	  grResDATA[ii]->Draw("PSAME");
	}
    }  
  c3->cd(4);
  resLEDDATA->Draw();
}
