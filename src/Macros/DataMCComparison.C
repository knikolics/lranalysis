#define  RUNS       4
//#define  BinNIPH  80

#define  BinNIPH  600
Float_t  MinIPH  = -100.5;
Float_t  MaxIPH  =  MinIPH+BinNIPH;

TH1F  *_hData[RUNS];
TH1F  *_hData1[RUNS];
TH1F  *_hDataCut2[RUNS];
TH1F  *_hDataMod[RUNS];
TH1F  *_hSimu[RUNS];
TH1F  *_hSimuIn[RUNS];
TH1F  *_hSimuOut[RUNS];
TH1I  *_hSimuConv[RUNS];
TH2F  *_Chi[RUNS]; 
Int_t *_hSimuVals[RUNS][BinNIPH];

Int_t   _RebinLevel = 10;

Int_t   _maxBin     = 0;
Int_t   _maxBinVal  = 0;
Int_t   _nDatEvents[RUNS];
Int_t   _nSimEvents[RUNS];
Int_t   _BStart[RUNS];
Int_t   _BStop[RUNS];
Int_t   _run;
Int_t    oldrun  = 0;
Float_t  step    = 1.;
//Float_t  MinIPH  = -20.5;
Double_t oldpar[5]={-1000,-1000,-1000,-1000,-1000};
Int_t    npfits;

TH1F *hSimuConv[RUNS];



//Double_t 
void myFcn(Int_t & /*nPar*/, Double_t * /*grad*/ , Double_t &fval, Double_t *p, Int_t /*iflag */  )
{

  double chi2 = 0; 
  double x[1],ey; 
  double tmp;
  npfits = 0;
  Int_t run  = (Int_t) p[4];
  for (int i=_BStart[run]; i< _BStop[run];i++){
    x[0]= _hData[run]->GetBinCenter(i);
    ey=  TMath::Sqrt(_hData[run]->GetBinContent(i));
    if (ey<1) ey= 1;
    tmp = (_hData[run]->GetBinContent(i)- Sim2Data(x,p))/ ey;
    chi2 += tmp*tmp; 
    npfits++;
    
  }
    
  fval = chi2; 
}



Double_t Sim2Data(Double_t *x, Double_t *par)
{
  Double_t sig      = 0;
 
  Double_t tmpF,simA,simB,simC,hscale; 
  Int_t tmpI;

  Int_t run  = (Int_t) par[4];
  
  
  if (par[0]== oldpar[0] &&
      par[1]== oldpar[1] &&
      par[3]== oldpar[3]
      ){
  }
  else{ // Depends on par[0],par[1],par[3]
    hSimuConv[run]->Reset("");
    //    cout << "  0: " << par[0] << "  1: " << par[1]
    // 	 << "  2: " << par[2] << "  3: " << par[3] 
    // 	 << "  4: " << par[4] << endl;
    _hSimu[run]->SetBinContent(_maxBin,par[3]); // max bin + 1?
      //_nDatEvents[run]/_hSimu[run]->Integral(-100,400);// /_hSimu[run]->Integral(-100,400));
    
    for (Int_t i=0;i<BinNIPH;i++){
      tmpF = 0;
      //      Double_t sigma = i/66.;
      Double_t sigma = par[0];
      for (Int_t j=i-5*sigma; j<(i+5*sigma);j++){
	if (j>0 && j<BinNIPH) {
	  //	  tmpF += (TMath::Gaus(j,i,par[0],1))*_hSimu[run]->GetBinContent(j+1-((int) par[1]));

	  tmpF += (TMath::Gaus(j,i,sigma,1))*_hSimu[run]->GetBinContent(j+1-((int) par[1]));
	}
      }
      tmpI = (Int_t )tmpF;  
      hSimuConv[run] -> Fill(i-100,tmpI);
    }
    hscale = _hData[run]->Integral(0,300)/hSimuConv[run]->Integral(0,300);
    //    hSimuConv[run] ->Scale(hscale);
    hSimuConv[run] ->Scale(0.1);
#ifdef DEBUG
    cout << _nSimEvents[run] << " " 
	 << _nDatEvents[run] << " "
	 << _hData[run]->Integral(-100,400)     << " " 
	 << _hSimu[run]->Integral(-100,400)     << " " 
	 <<  hSimuConv[run]->Integral(-100,400) << endl;
#endif
    oldpar[0]=par[0];
    oldpar[1]=par[1];
    oldpar[3]=par[3];
  }


  tmpF = (x[0]/par[2]+0.5+ par[1] + 100 +1);// - par[1] + 100 + 0.5);
  tmpI = (Int_t) tmpF;
  
  //sig = hSimuConv[run] -> GetBinContent( tmpI)/par[2];
  //  linear interpolation
  if ((Double_t)tmpI == (tmpF-0.5)){
    //     cout << "  0: " << par[0] << "  1: " << par[1]
    // 	 << "  2: " << par[2] << "  3: " << par[3] 
    // 	 << "  4: " << par[4] << endl;
    sig = hSimuConv[run] -> GetBinContent( tmpI)/par[2];
  }
  else if (((Double_t)tmpI )>(tmpF-0.5)){
    simA =  hSimuConv[run] -> GetBinContent( tmpI -1);
    simB =  hSimuConv[run] -> GetBinContent( tmpI);
    sig  = (simB +((simB-simA)/(1.))*(tmpF-0.5 - ((Double_t) tmpI)) )/par[2];
  }
  else if (((Double_t)tmpI)<(tmpF-0.5)){
    simB = hSimuConv[run] -> GetBinContent( tmpI);
    simC = hSimuConv[run] -> GetBinContent( tmpI +1);
    sig  = (simB +((simC - simB)/(1.))*(tmpF-0.5 - ((Double_t) tmpI)) )/par[2];
  }

  return _RebinLevel*sig;
}



Double_t SimConv(Double_t *x, Double_t *par)
{
  Double_t sig = 0.;
 
  //  Int_t ix = (int)(x[0]+0.5);

  Double_t time,sigma;

  //  Double_t integration_error = 10*0.1E-3/0.2; // sigma of noise per each integrated ns, 0.2 is to convert to pe
  Double_t integration_error = 0.1E-3/0.2; // sigma of noise per each integrated ns, 0.2 is to convert to pe

  Int_t run  = (Int_t) par[4];

  for (Int_t i=0;i<BinNIPH;i++) {
    //    sigma = par[0]*i;
    sigma = par[0];
//     time = 210*sqrt(TMath::Abs((Double_t)(i+MinIPH)));
//     if (i+MinIPH>0) sigma = time*integration_error;
//     else sigma = 1;
    sig += _hSimu[run]->GetBinContent(i)*TMath::Gaus(x[0]-i+0.5-MinIPH,0,sigma,1);
  }
  
  return sig;
}

string i2s(int run){
  char trun[6];
  sprintf(trun,"%d",run);
  string srun;
  srun = trun;
  return srun;
}

string f2s(float run){
  char trun[8];
  sprintf(trun,"%.5f",run);
  string srun;
  srun = trun;
  return srun;
}

struct DMatter_t {
  Int_t Nevent;
  Float_t ErecoilMeV;
  Int_t Initial_electrons;
  Int_t Initial_photons;
  Int_t abs_photons;
  Int_t phe;
  Float_t VertexPositionXcm;
  Float_t VertexPositionYcm;
  Float_t VertexPositionZcm;
};

struct PMT{
  int phqel;
  // G4float* n_pmt_sig;
};

struct EnInFV{
  float ene;
  float x;
  float y;
  float z;
};


void DataMCComparison()
{
  gStyle->SetOptStat(0);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);

  TFile *FileBG = new TFile ("BG.root","read");
  TH1F *HBG = (TH1F*)FileBG->Get("HBG");
  Int_t BGentries = HBG->GetEntries();
  HBG->Scale(1./BGentries);


  // Run  Position
  // 1873 B
  // 1874 D
  // 1880 F
  // 1877 G
  string DataNames[4] = {
    //    "Run1873/Run1873-File.output.200000.nocut.new2.root",
    "Run1873/Run1873-File.output.internal20V.root",
    "Run1874/Run1874-File.output.200000.nocut.new2.root",
    "Run1880/Run1880-File.output.200000.nocut.new2.root",
    //    "Run1877/Run1877-File.output.200000.nocut.new2.root"
    "Run1877/Run1877-File.output.internal20V.root"
  };

  string SimuNames[4] = {
    "MC/Gamma_Na_ShpheB_Ref88_small.root",
    "MC/Gamma_Na_ShpheD_Ref88_small.root",
    "MC/Gamma_Na_ShpheF_Ref88_small.root",
    "MC/Gamma_Na_ShpheG_Ref88_small.root"
    // "MC/Gamma_Na_LqeB.root",
    // "MC/Gamma_Na_LqeD.root",
    // "MC/Gamma_Na_LqeF.root",
    // // // "MC/Gamma_Na_LqeG.root"
    // // // "MC/Gamma_Na_ShpheG_Ref92.root"
    // // // "MC/Gamma_Na_ShpheG_Ref90.root"
    // "MC/Gamma_Na_LqeG.root"
  };

  string Label[4] = { "Pos. B",
		      "Pos. D",
		      "Pos. F",
		      "Pos. G"};

  //  Double_t hZeroScale[4] = { 0.01, 0.01, 0.01, 0.01};
  Double_t hZeroScale[4] = { 0.08, 0.05, 0.05, 0.05};
  //  Double_t hZeroScale[4] = { 1.,  1., 1., 1.};
  Int_t    ColorIndex[4] = {    1,    2,    8,    9};
  //  Double_t hConvSigma[4] = {    3,    3,    3,    3};
  Double_t hConvSigma[4] = {    2,    1,    1,    1};
  Int_t     hConvOffs[4] = {   20,   20,   20,   20};
  Double_t hConvExp[4]   = {  1.1,  1.1,  1.1,  1.1};

  //  Double_t pStep[5]      = { 1, 1, 0.01, 0.05, 1};

  if (RUNS >= 5){ cout << "Warning only 4 runs defined" << endl; return; }

  TFile *DataFile[RUNS];
  TFile *SimuFile[RUNS];

  TTree *Data[RUNS];
  TTree *SimuTree[RUNS];
  TBranch *SimuBranch[RUNS];
 
  string label,name,cmd,cut,opt;

  TCanvas *cn = new TCanvas("DataMCComparison","DataMCComparison", 700, 700);
  cn->Divide(2,2);
  //cn->Divide(1,1);

  TCanvas *cmom = new TCanvas("mom","mom", 700, 700);
  cmom->Divide(2,2);
  

  Int_t canvas = 0;

  TH1F *hPedMean[RUNS];
  TH1F *hPedRMS[RUNS];
  TF1  *fGaus[RUNS];
  TF1  *fGausp[RUNS];
  TF1  *fConv[RUNS];

  Double_t fPar[5];
  Double_t GPar[3];
  Double_t MeanPed[RUNS];
  Double_t hMax[RUNS];


  Double_t tmpF;
  Int_t tmpI;

  TF1 *fg  = new TF1("fg","gaus",-100,33);
  TLegend *leg[RUNS];
  
  for (int irun=0;irun<RUNS;irun++){
  //    for (int irun=RUNS-1;irun<RUNS;irun++){
  //  for (int irun=0;irun<1;irun++){
    leg[irun] = new TLegend(0.4,0.7,0.89,0.89);

    DataFile[irun]  = new TFile (DataNames[irun].c_str(),"read");
    Data[irun]  = (TTree*)DataFile[irun]->Get("Data");

    cmom->cd(1);

    name = "_hData_f";     name += i2s(irun);

    _hData[irun] = new TH1F(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);
    _hData[irun] ->SetLineColor(ColorIndex[irun]);
    //    cmd  = "VPMT.eIntegralPe/0.2/(1-0.04)>>";
    //cmd  = "VPMT.eIntegral/0.2>>";
    // cmd  = "PMT[0].eIntegralPe/0.2+PMT[1].eIntegralPe/0.2+PMT[2].eIntegralPe/0.2+PMT[3].eIntegralPe/0.2+PMT[4].eIntegralPe/0.2+PMT[5].eIntegralPe/0.2+PMT[6].eIntegralPe/0.2>>";
    cmd  = "(PMT[0].eIntegralPe/0.2+PMT[1].eIntegralPe/0.2+PMT[2].eIntegralPe/0.2+PMT[3].eIntegralPe/0.2+PMT[4].eIntegralPe/0.2+PMT[5].eIntegralPe/0.2+PMT[6].eIntegralPe/0.2)/(1-0.04)>>";
    cmd += name;
    //    cut  = "FileNumber<100&&abs(VPMT.ePeakTime-1000)<200&&VPMT.eNpe>5";
    //        cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<1400&&VPMT.eNpe>3";
//     if (irun==0) cut  = "VPMT.ePeakTime>980-000&&VPMT.ePeakTime<1100+000";
//     if (irun==1) cut  = "VPMT.ePeakTime>980-100&&VPMT.ePeakTime<1100+200";
//     if (irun==2) cut  = "VPMT.ePeakTime>980-200&&VPMT.ePeakTime<1100+400";
//     if (irun==3) cut  = "VPMT.ePeakTime>980-300&&VPMT.ePeakTime<1100+600";
    cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<10000-4500";

    // if (irun==0) cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<10000-7500";
    // if (irun==1) cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<10000-6500";
    // if (irun==2) cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<10000-5500";
    // if (irun==3) cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<10000-4500";

//    cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<1400&&VPMT.ePedSigma<0.0125";
//    cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<1400";
//    cut  = "VPMT.ePeakTime>900&&VPMT.ePeakTime<1100"; // very good
//    cut  = "VPMT.ePeakTime>950&&VPMT.ePeakTime<1050";

    Data[irun]->Draw(cmd.c_str(),cut.c_str()); 
    for (int j=0;j<BinNIPH;j++) 
      _hData[irun]->SetBinError(j+1,TMath::Sqrt(_hData[irun]->GetBinContent(j+1)));

    _hData[irun]->Scale(0.5);
    // if (irun==0) _hData[irun]->Scale(0.8);
    // if (irun==1) _hData[irun]->Scale(1.0);
    // if (irun==2) _hData[irun]->Scale(1.1);
    // if (irun==3) _hData[irun]->Scale(1.1);


    //
    // Opening Simulation Files
    //

    SimuFile[irun] = new TFile (SimuNames[irun].c_str(),"read");
    SimuTree[irun] = (TTree*)SimuFile[irun]->Get("tree");
    cout << SimuTree[irun]->GetEntries() << endl;
    
    DMatter_t dmread;
    SimuBranch[irun] = SimuTree[irun]->Branch("DMatter",&dmread.Nevent,"Nevent/I:ErecoilMeV/F:Initial_electrons/I:Initial_photons:abs_photons:phe:VertexPositionXcm/F:VertexPositionYcm:VertexPositionZcm");

    name  = "hSimu_f";      name += i2s(irun);
    label = "MC: ";         label += Label[irun];
    _hSimu[irun] = new TH1F(name.c_str(),label.c_str(),BinNIPH,MinIPH,MaxIPH);
    cmd  = "((PMT3+PMT5+PMT6+PMT7+PMT8+PMT10+PMT11)*"; cmd += f2s(step); cmd += ")>>"; cmd += name;
    cn->cd(irun+1);

    _hData[irun]->Draw();
    _hData[irun]->SetTitle(Label[irun].c_str());
    _hData[irun]->GetXaxis()->SetTitle("Light [p.e.]");
    _hData[irun]->GetYaxis()->SetTitle("Events");
    _hData[irun]->GetXaxis()->SetTitleOffset(1.5);
    _hData[irun]->GetYaxis()->SetTitleOffset(1.5);
    _hData[irun]->GetXaxis()->SetTitleSize(.045);
    _hData[irun]->GetYaxis()->SetTitleSize(.045);
    _hData[irun]->GetXaxis()->SetLabelSize(.045);
    _hData[irun]->GetYaxis()->SetLabelSize(.045);


    _nDatEvents[irun] = _hData[irun]->GetEntries();
    _hData[irun]->SetLineColor(1);
    
    SimuTree[irun]->Draw(cmd.c_str(),"(PMT3+PMT5+PMT6+PMT7+PMT8+PMT10+PMT11)>-100","same");
    _nSimEvents[irun] = _hSimu[irun]->GetEntries();

    //    _hData[irun]->Draw();
    //_hDataCut1[irun]->Draw("same");
    //_hDataCut2[irun]->Draw("same");


    for (int j=0;j<BinNIPH;j++) 
      _hSimuVals[irun][j] = _hSimu[irun]->GetBinContent(j+1);
    
    // Resetting first peak
    // hMax[irun] = _hSimu[irun]->GetMaximumBin();
    // _hSimu[irun]->SetBinContent(hMax[irun],_hSimu[irun]->GetBinContent(hMax[irun])*0);


    // Changing first peak
    Double_t correction = (10000-4500-800)/10000.;
    hMax[irun] = _hSimu[irun]->GetMaximumBin();
    _hSimu[irun]->SetBinContent(hMax[irun],_hSimu[irun]->GetBinContent(hMax[irun])*correction);
    _hSimu[irun]->SetBinContent(hMax[irun]+1,_hSimu[irun]->GetBinContent(hMax[irun]+1)*correction);
    _hSimu[irun]->SetBinContent(hMax[irun]+2,_hSimu[irun]->GetBinContent(hMax[irun]+2)*correction);
    _hSimu[irun]->SetBinContent(hMax[irun]+3,_hSimu[irun]->GetBinContent(hMax[irun]+3)*correction);
    _hSimu[irun]->SetBinContent(hMax[irun]+4,_hSimu[irun]->GetBinContent(hMax[irun]+4)*correction);



    //    _hSimu[irun]->SetBinContent(hMax[irun],_hSimu[irun]->GetBinContent(hMax[irun])*hZeroScale[irun]);
    //    _hSimu[irun]->SetBinContent(hMax[irun]+1,_hSimu[irun]->GetBinContent(hMax[irun]+1)*hZeroScale[irun]);
    //_hSimu[irun]->SetBinContent(hMax[irun]+2,_hSimu[irun]->GetBinContent(hMax[irun]+2)*hZeroScale[irun]);
    //

    _hSimu[irun]->SetLineColor(kBlue); 
    _maxBin    = _hSimu[irun]->GetMaximumBin();
    _maxBinVal = _hSimu[irun]->GetBinContent(_maxBin);
    cout << _hSimu[irun]->GetMaximumBin() << "   "  << _hSimu[irun]->GetBinContent(_maxBin) <<  endl;
    cout << _maxBin << endl;
    gPad->SetLogy();
    cn->Update();
    //    _hData[irun] ->Fit("fg","R");
    fg->GetParameters(GPar);



    // convolution starts
    name  = "hSimuConv_f";      name += i2s(irun);
    label = "MC: ";         label += Label[irun];
    _hSimuConv[irun] = new TH1F(name.c_str(),label.c_str(),BinNIPH,MinIPH,MaxIPH);

    Double_t tmpF;
    for (int i=1;i<BinNIPH;i++) {
      tmpF = 0;
      Double_t k = 1.-exp(-abs(i-100)/5.);
      for (int j=-100;j<BinNIPH;j++)

	// digitization effect
	// if (i-j-100>0) {
	//   if ((int)((i-j-100.)/k+100.+0.5)>0&&(int)((i-j-100.)/k+100.+0.5)<BinNIPH&&j+100>0&&j+100<BinNIPH) tmpF += HBG->GetBinContent((int)((i-j-100.)/k+100.+0.5))*_hSimu[irun]->GetBinContent(j+100);
	// } else {
	//   if ((int)((i-j-100.)*k+100.+0.5)>0&&(int)((i-j-100.)*k+100.+0.5)<BinNIPH&&j+100>0&&j+100<BinNIPH) tmpF += HBG->GetBinContent((int)((i-j-100.)*k+100.+0.5))*_hSimu[irun]->GetBinContent(j+100);
	// }

        // the correct one
	if (i-j>0&&i-j<BinNIPH&&j+100>0&&j+100<BinNIPH)
	  tmpF += HBG->GetBinContent(i-j)*_hSimu[irun]->GetBinContent(j+100);
	// no shift
	// if (i-j>0&&i-j<BinNIPH)
	//   tmpF += HBG->GetBinContent(i-j)*_hSimu[irun]->GetBinContent(j);
      
      _hSimuConv[irun]->Fill(i-100,tmpF);
    }
    // convolution ends


    //    _hData[irun]->Draw("same");
    _hData[irun]->Draw("");
    cn->Update();


    //    _hSimu[irun]->Draw("same");

    _hSimuConv[irun]->Draw("same");
    _hSimuConv[irun]->SetLineColor(2);
  }

}


DataMCComparisonOld()
{
  gStyle->SetOptStat(0);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);

  // Run  Position
  // 1873 B
  // 1874 D
  // 1880 F
  // 1877 G
  string DataNames[4] = {
    //    "Run1873/Run1873-File.output.200000.nocut.new2.root",
    "Run1873/Run1873-File.output.internal20V.root",
    "Run1874/Run1874-File.output.200000.nocut.new2.root",
    "Run1880/Run1880-File.output.200000.nocut.new2.root",
    //    "Run1877/Run1877-File.output.200000.nocut.new2.root"
    "Run1877/Run1877-File.output.internal20V.root"
  };

  string SimuNames[4] = {
    "MC/Gamma_Na_LqeB.root",
    "MC/Gamma_Na_LqeD.root",
    "MC/Gamma_Na_LqeF.root",
    "MC/Gamma_Na_LqeG.root"
    //"MC/Gamma_Na_ShpheG_Ref80.root"
  };

  string Label[4] = { "Pos. B",
		      "Pos. D",
		      "Pos. F",
		      "Pos. G"};

  //  Double_t hZeroScale[4] = { 0.01, 0.01, 0.01, 0.01};
  Double_t hZeroScale[4] = { 0.08, 0.05, 0.05, 0.05};
  //  Double_t hZeroScale[4] = { 1.,  1., 1., 1.};
  Int_t    ColorIndex[4] = {    1,    2,    8,    9};
  //  Double_t hConvSigma[4] = {    3,    3,    3,    3};
  Double_t hConvSigma[4] = {    2,    1,    1,    1};
  Int_t     hConvOffs[4] = {   20,   20,   20,   20};
  Double_t hConvExp[4]   = {  1.1,  1.1,  1.1,  1.1};

  //  Double_t pStep[5]      = { 1, 1, 0.01, 0.05, 1};

  if (RUNS >= 5){ cout << "Warning only 4 runs defined" << endl; return; }

  TFile *DataFile[RUNS];
  TFile *SimuFile[RUNS];

  TTree *Data[RUNS];
  TTree *SimuTree[RUNS];
  TBranch *SimuBranch[RUNS];
 
  string label,name,cmd,cut,opt;

  TCanvas *cn = new TCanvas("DataMCComparison","DataMCComparison", 700, 700);
  cn->Divide(2,2);
  //cn->Divide(1,1);

  TCanvas *cmom = new TCanvas("mom","mom", 700, 700);
  cmom->Divide(2,2);
  

  Int_t canvas = 0;

  TH1F *hPedMean[RUNS];
  TH1F *hPedRMS[RUNS];
  TF1  *fGaus[RUNS];
  TF1  *fGausp[RUNS];
  TF1  *fConv[RUNS];

  Double_t fPar[5];
  Double_t GPar[3];
  Double_t MeanPed[RUNS];
  Double_t hMax[RUNS];


  Double_t tmpF;
  Int_t tmpI;

  TF1 *fg  = new TF1("fg","gaus",-100,33);
  TLegend *leg[RUNS];
  
    for (int irun=0;irun<RUNS;irun++){
  //  for (int irun=0;irun<1;irun++){
    leg[irun] = new TLegend(0.4,0.7,0.89,0.89);

    DataFile[irun]  = new TFile (DataNames[irun].c_str(),"read");
    Data[irun]  = (TTree*)DataFile[irun]->Get("Data");

    cmom->cd(1);

    name = "_hData_f";     name += i2s(irun);

    _hData[irun] = new TH1F(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);
    _hData[irun] ->SetLineColor(ColorIndex[irun]);
    //    cmd  = "VPMT.eIntegralPe/0.2/(1-0.04)>>";
    //cmd  = "VPMT.eIntegral/0.2>>";
    // cmd  = "PMT[0].eIntegralPe/0.2+PMT[1].eIntegralPe/0.2+PMT[2].eIntegralPe/0.2+PMT[3].eIntegralPe/0.2+PMT[4].eIntegralPe/0.2+PMT[5].eIntegralPe/0.2+PMT[6].eIntegralPe/0.2>>";
    cmd  = "(PMT[0].eIntegralPe/0.2+PMT[1].eIntegralPe/0.2+PMT[2].eIntegralPe/0.2+PMT[3].eIntegralPe/0.2+PMT[4].eIntegralPe/0.2+PMT[5].eIntegralPe/0.2+PMT[6].eIntegralPe/0.2)/(1-0.04)>>";
    cmd += name;
    //    cut  = "FileNumber<100&&abs(VPMT.ePeakTime-1000)<200&&VPMT.eNpe>5";
    //        cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<1400&&VPMT.eNpe>3";
//     if (irun==0) cut  = "VPMT.ePeakTime>980-000&&VPMT.ePeakTime<1100+000";
//     if (irun==1) cut  = "VPMT.ePeakTime>980-100&&VPMT.ePeakTime<1100+200";
//     if (irun==2) cut  = "VPMT.ePeakTime>980-200&&VPMT.ePeakTime<1100+400";
//     if (irun==3) cut  = "VPMT.ePeakTime>980-300&&VPMT.ePeakTime<1100+600";
    cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<10000-4500";

//    cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<1400&&VPMT.ePedSigma<0.0125";
//    cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<1400";
//    cut  = "VPMT.ePeakTime>900&&VPMT.ePeakTime<1100"; // very good
//    cut  = "VPMT.ePeakTime>950&&VPMT.ePeakTime<1050";

    Data[irun]->Draw(cmd.c_str(),cut.c_str()); 
    for (int j=0;j<BinNIPH;j++) 
      _hData[irun]->SetBinError(j+1,TMath::Sqrt(_hData[irun]->GetBinContent(j+1)));

    _hData[irun]->Scale(0.5);
    if (irun==0) _hData[irun]->Scale(0.8);
    if (irun==1) _hData[irun]->Scale(1.0);
    if (irun==2) _hData[irun]->Scale(1.1);
    if (irun==3) _hData[irun]->Scale(1.1);


    //
    // Opening Simulation Files
    //

    SimuFile[irun] = new TFile (SimuNames[irun].c_str(),"read");
    SimuTree[irun] = (TTree*)SimuFile[irun]->Get("tree");
    cout << SimuTree[irun]->GetEntries() << endl;
    
    DMatter_t dmread;
    SimuBranch[irun] = SimuTree[irun]->Branch("DMatter",&dmread.Nevent,"Nevent/I:ErecoilMeV/F:Initial_electrons/I:Initial_photons:abs_photons:phe:VertexPositionXcm/F:VertexPositionYcm:VertexPositionZcm");

    name  = "hSimu_f";      name += i2s(irun);
    label = "MC: ";         label += Label[irun];
    _hSimu[irun] = new TH1F(name.c_str(),label.c_str(),BinNIPH,MinIPH,MaxIPH);
    cmd  = "((PMT3+PMT5+PMT6+PMT7+PMT8+PMT10+PMT11)*"; cmd += f2s(step); cmd += ")>>"; cmd += name;
    cn->cd(irun+1);

    _hData[irun]->Draw();
    _hData[irun]->SetTitle(Label[irun].c_str());
    _hData[irun]->GetXaxis()->SetTitle("Light [p.e.]");
    _hData[irun]->GetYaxis()->SetTitle("Events");
    _hData[irun]->GetXaxis()->SetTitleOffset(1.5);
    _hData[irun]->GetYaxis()->SetTitleOffset(1.5);
    _hData[irun]->GetXaxis()->SetTitleSize(.045);
    _hData[irun]->GetYaxis()->SetTitleSize(.045);
    _hData[irun]->GetXaxis()->SetLabelSize(.045);
    _hData[irun]->GetYaxis()->SetLabelSize(.045);


    _nDatEvents[irun] = _hData[irun]->GetEntries();
    _hData[irun]->SetLineColor(1);
    
    SimuTree[irun]->Draw(cmd.c_str(),"(PMT3+PMT5+PMT6+PMT7+PMT8+PMT10+PMT11)>-100","same");
    _nSimEvents[irun] = _hSimu[irun]->GetEntries();

    //    _hData[irun]->Draw();
    //_hDataCut1[irun]->Draw("same");
    //_hDataCut2[irun]->Draw("same");


    for (int j=0;j<BinNIPH;j++) 
      _hSimuVals[irun][j] = _hSimu[irun]->GetBinContent(j+1);
    
    // Changing first peak
    hMax[irun] = _hSimu[irun]->GetMaximumBin();
    _hSimu[irun]->SetBinContent(hMax[irun],_hSimu[irun]->GetBinContent(hMax[irun])*0);

    //    _hSimu[irun]->SetBinContent(hMax[irun],_hSimu[irun]->GetBinContent(hMax[irun])*hZeroScale[irun]);
    //    _hSimu[irun]->SetBinContent(hMax[irun]+1,_hSimu[irun]->GetBinContent(hMax[irun]+1)*hZeroScale[irun]);
    //_hSimu[irun]->SetBinContent(hMax[irun]+2,_hSimu[irun]->GetBinContent(hMax[irun]+2)*hZeroScale[irun]);
    //

    _hSimu[irun]->SetLineColor(kBlue); 
    _maxBin    = _hSimu[irun]->GetMaximumBin();
    _maxBinVal = _hSimu[irun]->GetBinContent(_maxBin);
    cout << _hSimu[irun]->GetMaximumBin() << "   "  << _hSimu[irun]->GetBinContent(_maxBin) <<  endl;
    cout << _maxBin << endl;
    gPad->SetLogy();
    cn->Update();
    //    _hData[irun] ->Fit("fg","R");
    fg->GetParameters(GPar);


    //
    // Convolutions
    //
    name  = "hSimuConv_f";      name += i2s(irun);
    _hSimuConv[irun] =new TH1I(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);

    name  = "hSimuConvB_f";      name += i2s(irun);
    hSimuConv[irun] =new TH1F(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);
 
    name = "SimToData_f"; name += i2s(irun);
    //    fConv[irun] = new TF1 (name.c_str(),Sim2Data,-100,400,5);
    fConv[irun] = new TF1 (name.c_str(),SimConv,-100,400,5);
    fConv[irun]->SetNpx(500);

    fConv[irun]->SetParameters(hConvSigma[irun],
			       hConvOffs[irun],
			       hConvExp[irun],
			       // _hSimu[irun]->GetBinContent(_maxBin)*hZeroScale[irun],
			       1,
			       (Double_t) irun);
    fConv[irun]->SetLineColor(8);
    fConv[irun]->SetLineWidth(2);

    fConv[irun]->Draw("same");
    
    fConv[irun]->SetParLimits(0,20,30);
    fConv[irun]->FixParameter(0,hConvSigma[irun]);
    
    fConv[irun]->SetParLimits(1,5,20);
    fConv[irun]->FixParameter(1,hConvOffs[irun]);
    
    fConv[irun]->SetParLimits(2,0.9,1.3);
    
    fConv[irun]->SetParLimits(3,1e4,2e5);
    
    fConv[irun]->FixParameter(4,(Double_t) irun);


    _hData[irun]->Draw("same");
    cn->Update();


    _hSimu[irun]->Draw("same");

  }

}



ShowData()
{
  gStyle->SetOptStat(0);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);

  // Run  Position
  // 1873 B
  // 1874 D
  // 1880 F
  // 1877 G
  string DataNames[4] = {
    "Run1873/Run1873-File.output.200000.nocut.new2.root",
    "Run1874/Run1874-File.output.200000.nocut.new2.root",
    "Run1880/Run1880-File.output.200000.nocut.new2.root",
    "Run1877/Run1877-File.output.200000.nocut.new2.root",
  };

  string Label[4] = { "Pos. B",
		      "Pos. D",
		      "Pos. F",
		      "Pos. G"};

  Int_t    ColorIndex[4] = {    1,    2,    8,    9};

  if (RUNS >= 5){ cout << "Warning only 4 runs defined" << endl; return; }

  TFile *DataFile[RUNS];
  TTree *Data[RUNS];

  string label,name,cmd,cut,opt;

  TCanvas *cn = new TCanvas("ShowData","ShowData", 700, 700);
  cn->Divide(2,2);

  for (int irun=0;irun<RUNS;irun++)
    {
      cn->cd(irun+1);

      DataFile[irun]  = new TFile (DataNames[irun].c_str(),"read");
      Data[irun]  = (TTree*)DataFile[irun]->Get("Data");
      
      name = "_hData_f";     name += i2s(irun);
      
      _hData[irun] = new TH1F(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);

      cmd  = "VPMT.eIntegralPe/0.2>>";
      //      cmd  = "PMT[0].eIntegralPe/0.2+PMT[1].eIntegralPe/0.2+PMT[2].eIntegralPe/0.2+PMT[3].eIntegralPe/0.2+PMT[4].eIntegralPe/0.2+PMT[5].eIntegralPe/0.2+PMT[6].eIntegralPe/0.2>>";
      cmd += name;

      //      cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<1400&&VPMT.eNpe>3";
      cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<1400";
      //    cut  = "FileNumber<100&&VPMT.ePeakTime<10000-4500";
      
      Data[irun]->Draw(cmd.c_str(),cut.c_str()); 
//       for (int j=0;j<BinNIPH;j++) 
// 	_hData[irun]->SetBinError(j+1,TMath::Sqrt(_hData[irun]->GetBinContent(j+1)));
      
      _hData[irun]->Scale(0.5);

      _hData[irun]->Draw();
      _hData[irun]->SetTitle(Label[irun].c_str());
      _hData[irun]->GetXaxis()->SetTitle("Light [p.e.]");
      _hData[irun]->GetYaxis()->SetTitle("Events");
      _hData[irun]->GetXaxis()->SetTitleOffset(1.5);
      _hData[irun]->GetYaxis()->SetTitleOffset(1.5);
      _hData[irun]->GetXaxis()->SetTitleSize(.045);
      _hData[irun]->GetYaxis()->SetTitleSize(.045);
      _hData[irun]->GetXaxis()->SetLabelSize(.045);
      _hData[irun]->GetYaxis()->SetLabelSize(.045);

      _hData[irun]->SetLineColor(1);

      name += i2s(1);
      _hData1[irun] = new TH1F(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);
      //      cmd  = "VPMT.eIntegralPe/0.2>>";
      cmd  = "PMT[0].eIntegralPe/0.2+PMT[1].eIntegralPe/0.2+PMT[2].eIntegralPe/0.2+PMT[3].eIntegralPe/0.2+PMT[4].eIntegralPe/0.2+PMT[5].eIntegralPe/0.2+PMT[6].eIntegralPe/0.2>>";
      cmd += name;
      cut  = "VPMT.ePeakTime>800&&VPMT.ePeakTime<1400";
      Data[irun]->Draw(cmd.c_str(),cut.c_str(),"goff");
      _hData1[irun]->SetLineColor(2);
      _hData1[irun]->Draw("same");

      _hData1[irun]->Scale(0.5);

      gPad->SetLogy();

    }

}





ShowMC()
{
  gStyle->SetOptStat(0);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);

  string SimuNames[4] = {
    "MC/Gamma_Na_LqeB.root",
    "MC/Gamma_Na_LqeD.root",
    "MC/Gamma_Na_LqeF.root",
    //    "MC/Gamma_Na_LqeG.root"
    "MC/Gamma_Na_ShpheG_Ref80.root"
  };

  string Label[4] = { "Pos. B",
		      "Pos. D",
		      "Pos. F",
		      "Pos. G"};

  TF1  *fConv[RUNS];
  Double_t hZeroScale[4] = { 0.50,  0.6, 0.85, 0.21};
  //  Double_t hZeroScale[4] = { 1.,  1., 1., 1.};
  Int_t    ColorIndex[4] = {    1,    2,    8,    9};
  //  Double_t hConvSigma[4] = {    2,    2,    2,    2};
  Double_t hConvSigma[4] = {    1,    1,    1,    1};
  Int_t     hConvOffs[4] = {   20,   20,   20,   20};
  Double_t hConvExp[4]   = {  1.1,  1.1,  1.1,  1.1};


  if (RUNS >= 5){ cout << "Warning only 4 runs defined" << endl; return; }

  TFile *DataFile[RUNS];
  TFile *SimuFile[RUNS];

  TTree *Data[RUNS];
  TTree *SimuTree[RUNS];
  //  TBranch *SimuBranch[RUNS];
 
  string label,name,cmd,cut,opt;

  TCanvas *cn = new TCanvas("MC","MC", 700, 700);
  cn->Divide(2,2);

  for (int irun=0;irun<RUNS;irun++){
    //  for (int irun=0;irun<1;irun++){

    SimuFile[irun] = new TFile (SimuNames[irun].c_str(),"read");
    SimuTree[irun] = (TTree*)SimuFile[irun]->Get("tree");
    
//     DMatter_t dmread;
//     SimuBranch[irun] = SimuTree[irun]->Branch("DMatter",&dmread.Nevent,"Nevent/I:ErecoilMeV/F:Initial_electrons/I:Initial_photons:abs_photons:phe:VertexPositionXcm/F:VertexPositionYcm:VertexPositionZcm");

    cn->cd(irun+1);

    name  = "hSimu_f";      name += i2s(irun);
    label = "MC: ";         label += Label[irun];
    _hSimu[irun] = new TH1F(name.c_str(),label.c_str(),BinNIPH,MinIPH,MaxIPH);
    cmd  = "((PMT3+PMT5+PMT6+PMT7+PMT8+PMT10+PMT11)*"; cmd += f2s(step); cmd += ")>>"; cmd += name;
    SimuTree[irun]->Draw(cmd.c_str(),"");

    name  = "hSimuIn_f";      name += i2s(irun);
    label = "MCIn: ";         label += Label[irun];
    _hSimuIn[irun] = new TH1F(name.c_str(),label.c_str(),BinNIPH,MinIPH,MaxIPH);
    cmd  = "((PMT3+PMT5+PMT6+PMT7+PMT8+PMT10+PMT11)*"; cmd += f2s(step); cmd += ")>>"; cmd += name;
    //    SimuTree[irun]->Draw(cmd.c_str(),"z>-8000");
    //    SimuTree[irun]->Draw(cmd.c_str(),"EDep!=0");
    SimuTree[irun]->Draw(cmd.c_str(),"EdepFV!=0");

    name  = "hSimuOut_f";      name += i2s(irun);
    label = "MCOut: ";         label += Label[irun];
    _hSimuOut[irun] = new TH1F(name.c_str(),label.c_str(),BinNIPH,MinIPH,MaxIPH);
    cmd  = "((PMT3+PMT5+PMT6+PMT7+PMT8+PMT10+PMT11)*"; cmd += f2s(step); cmd += ")>>"; cmd += name;
    //    SimuTree[irun]->Draw(cmd.c_str(),"z<-8000");
    //    printf("Edep == 0 : %d\n",SimuTree[irun]->Draw(cmd.c_str(),"EDep==0"));
    printf("Edep == 0 : %d\n",SimuTree[irun]->Draw(cmd.c_str(),"EdepFV==0"));


    _hSimu[irun]->SetTitle(Label[irun].c_str());
    _hSimu[irun]->GetXaxis()->SetTitle("Light [p.e.]");
    _hSimu[irun]->GetYaxis()->SetTitle("Events");
    _hSimu[irun]->GetXaxis()->SetTitleOffset(1.5);
    _hSimu[irun]->GetYaxis()->SetTitleOffset(1.5);
    _hSimu[irun]->GetXaxis()->SetTitleSize(.045);
    _hSimu[irun]->GetYaxis()->SetTitleSize(.045);
    _hSimu[irun]->GetXaxis()->SetLabelSize(.045);
    _hSimu[irun]->GetYaxis()->SetLabelSize(.045);


    _hSimuOut[irun]->Draw("");
    _hSimuOut[irun]->SetLineColor(4);
    _hSimuIn[irun]->Draw("same");
    _hSimuIn[irun]->SetLineColor(2);

//     _hSimu[irun]->Draw();
//     _hSimuIn[irun]->Draw("same");
//     _hSimuIn[irun]->SetLineColor(2);
//     _hSimuOut[irun]->Draw("same");
//     _hSimuOut[irun]->SetLineColor(4);
//     _hSimu[irun]->Draw("same");

    name = "SimToData_f"; name += i2s(irun);
    fConv[irun] = new TF1 (name.c_str(),SimConv,-100,500,5);
    fConv[irun]->SetNpx(500);
    fConv[irun]->SetParameters(hConvSigma[irun],hConvOffs[irun],hConvExp[irun],1,(Double_t) irun);
    fConv[irun]->SetLineColor(8);
    fConv[irun]->SetLineWidth(2);
    //    fConv[irun]->Draw("same");

    gPad->SetLogy();
    cn->Update();

  }

}





CompareMC()
{
  gStyle->SetOptStat(0);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);

  string SimuNames[2] = {
    "MC/Gamma_Na_LqeG.root",
    "MC/Gamma_Na_ShpheG_Ref80.root"
    //    "MC/Gamma_Na_LqeG.root"
  };

  string Label[2] = { "Pos. G",
		      "Pos. G"};

  TFile *DataFile[RUNS];
  TFile *SimuFile[RUNS];

  TTree *Data[RUNS];
  TTree *SimuTree[RUNS];
 
  string label,name,cmd,cut,opt;

  TCanvas *cn = new TCanvas("MC","MC", 700, 700);

  for (int irun=0;irun<2;irun++){

    SimuFile[irun] = new TFile (SimuNames[irun].c_str(),"read");
    SimuTree[irun] = (TTree*)SimuFile[irun]->Get("tree");
    
    name  = "hSimu_f";      name += i2s(irun);
    label = "MC: ";         label += Label[irun];
    _hSimu[irun] = new TH1F(name.c_str(),label.c_str(),BinNIPH,MinIPH,MaxIPH);
    cmd  = "((PMT3+PMT5+PMT6+PMT7+PMT8+PMT10+PMT11)*"; cmd += f2s(step); cmd += ")>>"; cmd += name;
    SimuTree[irun]->Draw(cmd.c_str(),"");

    _hSimu[irun]->SetTitle(Label[irun].c_str());
    _hSimu[irun]->GetXaxis()->SetTitle("Light [p.e.]");
    _hSimu[irun]->GetYaxis()->SetTitle("Events");
    _hSimu[irun]->GetXaxis()->SetTitleOffset(1.5);
    _hSimu[irun]->GetYaxis()->SetTitleOffset(1.5);
    _hSimu[irun]->GetXaxis()->SetTitleSize(.045);
    _hSimu[irun]->GetYaxis()->SetTitleSize(.045);
    _hSimu[irun]->GetXaxis()->SetLabelSize(.045);
    _hSimu[irun]->GetYaxis()->SetLabelSize(.045);

    gPad->SetLogy();
  }

  string DataNames[1] = {
    "Run1877/Run1877-File.output.200000.nocut.new2.root",
  };
 
  string label,name,cmd,cut,opt;

  Int_t canvas = 0;

  TH1F *hPedMean[RUNS];
  TH1F *hPedRMS[RUNS];
  TF1  *fGaus[RUNS];
  TF1  *fGausp[RUNS];
  TF1  *fConv[RUNS];

  Double_t fPar[5];
  Double_t GPar[3];
  Double_t MeanPed[RUNS];
  Double_t hMax[RUNS];


  Double_t tmpF;
  Int_t tmpI;

  TF1 *fg  = new TF1("fg","gaus",-100,33);
  
  int irun=0;

  DataFile[irun]  = new TFile (DataNames[irun].c_str(),"read");
  Data[irun]  = (TTree*)DataFile[irun]->Get("Data");

  name = "_hData_f";     name += i2s(irun);

  _hData[irun] = new TH1F(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);
  cmd  = "(PMT[0].eIntegralPe/0.2+PMT[1].eIntegralPe/0.2+PMT[2].eIntegralPe/0.2+PMT[3].eIntegralPe/0.2+PMT[4].eIntegralPe/0.2+PMT[5].eIntegralPe/0.2+PMT[6].eIntegralPe/0.2)/(1-0.04)>>";
    cmd += name;
    cut  = "VPMT.ePeakTime>900&&VPMT.ePeakTime<1100"; // very good

    Data[irun]->Draw(cmd.c_str(),cut.c_str()); 
    for (int j=0;j<BinNIPH;j++) 
      _hData[irun]->SetBinError(j+1,TMath::Sqrt(_hData[irun]->GetBinContent(j+1)));

    _hData[irun]->Scale(0.5);
    _hData[irun]->Scale(1.1);




  _hSimu[0]->Draw("");
  _hSimu[0]->SetLineColor(4);
  _hSimu[1]->Draw("same");
  _hSimu[1]->SetLineColor(2);
  _hData[irun]->Draw("same");
  _hData[irun]->SetLineColor(1);

}





convolute()
{
  gStyle->SetOptStat(0);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);

  // other files in the dir.
  // 1873 pos B
  // 1874 pos D 
  // 1877 pos G
  // 1880 pos F
  string DataNames[4] = { "Run1873-File.result.root",
			  "Run1874-File.result.root",
			  "Run1880-File.result.root",
			  "Run1877-File.result.root" };

  string SimuNames[4] = { "Gamma_Na_LqeB.root",
			  "Gamma_Na_LqeD.root",
			  "Gamma_Na_LqeF.root",
 			  "Gamma_Na_LqeG.root"};

//   string Label[4] = { "^{22}Na in Pos. B",
// 		      "^{22}Na in Pos. D",
// 		      "^{22}Na in Pos. F",
// 		      "^{22}Na in Pos. G"};
  string Label[4] = { "Pos. B",
		      "Pos. D",
		      "Pos. G",
		      "Pos. F"};
  Double_t hZeroScale[4] = { 0.50,  0.6, 0.85, 0.85};//0.55, 0.55, 0.55, 0.55};
  Int_t    ColorIndex[4] = {    1,    2,    8,    9};
  Double_t hConvSigma[4] = {   28,   28,   28,   28};
  Int_t     hConvOffs[4] = {   14,   14,   14,   14};
  Double_t hConvExp[4]   = {  1.1,  1.1,  1.1,  1.1};

  Double_t pStep[5]      = { 1, 1, 0.01, 0.05, 1};

  if (RUNS >= 5){ cout << "Warning only 4 runs defined" << endl; return; }

  TFile *DataFile[RUNS];
  TFile *SimuFile[RUNS];

  TTree *DataTree[RUNS];
  TTree *SimuTree[RUNS];
  TBranch *SimuBranch[RUNS];
 

 
   

//   TCanvas *c1 = new TCanvas("c1","c1",800,600);
//   c1->Divide(2,2);

  string label,name,cmd,cut,opt;

  TCanvas *cn[RUNS+1];
  for (int i=0;i<3;i++){
    name = "cn"; name += i2s(i);
    cn[i] = new TCanvas(name.c_str(), name.c_str(), 700, 700);
    cn[i]->Divide(2,2);
  }
  
  DMatter_t dmread;
  Int_t canvas = 0;

  TH1F *hPedMean[RUNS];
  TH1F *hPedRMS[RUNS];
  TF1  *fGaus[RUNS];
  TF1  *fGausp[RUNS];
  TF1  *fConv[RUNS];

  Double_t fPar[5];
  Double_t GPar[3];
  Double_t MeanPed[RUNS];
  Double_t hMax[RUNS];


  Double_t tmpF;
  Int_t tmpI;

  TF1 *fg  = new TF1("fg","gaus",-100,33);
  TLegend *leg[RUNS];
  
  for (int i=0;i<RUNS;i++){
    leg[i] = new TLegend(0.4,0.7,0.89,0.89);
     _run=i;
    //    canvas = i;
    DataFile[i]  = new TFile (DataNames[i].c_str(),"read");
    
    // Experimental Data Run1873 no cuts
    DataTree[i]  = (TTree*)DataFile[i]->Get("DataTree");

    name = "PedestalMean_f"; name += i2s(i);
    hPedMean[i]  = new TH1F(name.c_str(), name.c_str(), 500,-0.05, -0.01);
    hPedMean[i] ->SetLineColor(ColorIndex[i]);

    name = "PedestalRMS_f";  name += i2s(i);
    hPedRMS[i]   = new TH1F(name.c_str(), name.c_str(), 500,-0.001,0.02);
    hPedRMS[i] ->SetLineColor(ColorIndex[i]);
    
    
    if ( (i >=1) && (canvas==0)) opt = "same";
    cut = "File<100 && PedRMS[7] <0.012";

    cn[canvas]->cd(1);
    cmd = "PedMean[7]>>PedestalMean_f";  cmd += i2s(i);
    DataTree[i]->Draw(cmd.c_str(),cut.c_str(),opt.c_str());  //,"(PH[7]<0.15 || PT[7] <1500) && PedRMS[7]<0.013");

    name = "fg";   name += i2s(i);
    fGaus[i] = new TF1 (name.c_str(),"gaus",-0.05,-0.037);
 
    fGaus[i]    ->SetParameters(9000,-0.04,0.001);
    hPedMean[i] ->Fit(name.c_str(),"RNO"); 
    fGaus[i]    ->GetParameters(GPar);  
    
    name = "fg_p";   name += i2s(i);
    fGausp[i] = new TF1 (name.c_str(),"gaus",-0.05,-0.037);
    fGausp[i] ->SetLineColor(ColorIndex[i]);
    fGausp[i] ->SetParameters(GPar);  
    fGausp[i] ->Draw("same");
    
    cn[canvas]->cd(2);
    cmd = "PedRMS[7]>>PedestalRMS_f"; cmd += i2s(i);
    DataTree[i]->Draw(cmd.c_str(),cut.c_str(),opt.c_str());  //,"(PH[7]<0.15 || PT[7] <1500) && PedRMS[7]<0.013");

    MeanPed[i] = GPar[1];



    cn[canvas]->cd(3);
    name = "_hData_f";     name += i2s(i);

    _hData[i] = new TH1F(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);
    _hData[i] ->SetLineColor(ColorIndex[i]);
    cmd  = "IPH[7]/0.2>>"; cmd += name;
    cut  = "File<100  && PedRMS[7]<0.013";// && PH[7]<0.8";
    // && (PH[7]<0.15 || PT[7]<1500)";
    
    //&& PedMean[7]>";
    //   cut += f2s(GPar[1]-2*GPar[2]); cut += " && PedMean[7]<";
    //cut += f2s(GPar[1]+2*GPar[2]);
    //    cut = "(PH[7]<0.15 || PT[7] <1500) && PedRMS[7]<1.2e-2";
    DataTree[i]->Draw(cmd.c_str(),cut.c_str(),opt.c_str()); 
    for (int j=0;j<BinNIPH;j++) 
      _hData[i]->SetBinError(j+1,TMath::Sqrt(_hData[i]->GetBinContent(j+1)));

 


    name = "_hDataCutA_f";     name += i2s(i);
    _hDataCut1[i] = new TH1F(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);
    _hDataCut1[i] ->SetLineColor(2);
    cmd  = "(IPH[7]/0.2 )>>"; cmd += name;
    cut  = "File<100  && PedRMS[7]<0.013 && PH[0]<0.89 && PH[1]<0.89 && PH[2]<0.89 && PH[3]<0.89 && PH[4]<0.89 && PH[5]<0.89 && PH[6]<0.89 ";
    DataTree[i]->Draw(cmd.c_str(),cut.c_str(),opt.c_str()); 
    for (int j=0;j<BinNIPH;j++) 
      _hDataCut1[i]->SetBinError(j+1,TMath::Sqrt(_hDataCut1[i]->GetBinContent(j+1)));
  
    name = "_hDataCutB_f";     name += i2s(i);
    _hDataCut2[i] = new TH1F(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);
    _hDataCut2[i] ->SetLineColor(6);
    cmd  = "(IPH[7]/0.2 )>>"; cmd += name;
    cut  = "File<100  && PedRMS[7]<0.013 && PH[7]<0.8&& (PT[7]<1200 || PH[7] <0.5)";
    DataTree[i]->Draw(cmd.c_str(),cut.c_str(),opt.c_str()); 
    for (int j=0;j<BinNIPH;j++) 
      _hDataCut2[i]->SetBinError(j+1,TMath::Sqrt(_hDataCut1[i]->GetBinContent(j+1)));
  
  


    cn[canvas]->cd(4);
    name = "_hDataMod_f";  name += i2s(i);
    _hDataMod[i] = new TH1F(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);
    _hDataMod[i] ->SetLineColor(ColorIndex[i]);
  
    cmd = "(( (IPH[7] + ( PedMean[7] *5000)) - (";
    cmd += f2s(MeanPed[i]);
    cmd += "*5000) )/0.2) >>"; cmd += name;
    cout << cmd << endl;
    //   cut = "(PH[7]<0.15 || PT[7] <1500) && PedRMS[7]<1.3e-2 ";
    // DataTree[i]->Draw(cmd.c_str(),cut.c_str(),opt.c_str());  
    //   // && PH[0] <0.89 && PH[1] <0.89 && PH[2] <0.89 && PH[3] <0.89 &&
    //      PH[4] <0.89 && PH[5] <0.89 && PH[6] <0.89   ");
    //   // PT[7] > 950 && PT[7]<1150");
    


    //
    // Opening Simulation Files
    //

    SimuFile[i] = new TFile (SimuNames[i].c_str(),"read");
    SimuTree[i] = (TTree*)SimuFile[i]->Get("tree");
    cout << SimuTree[i]->GetEntries() << endl;
    
    SimuBranch[i] = SimuTree[i]->Branch("DMatter",&dmread.Nevent,"Nevent/I:ErecoilMeV/F:Initial_electrons/I:Initial_photons:abs_photons:phe:VertexPositionXcm/F:VertexPositionYcm:VertexPositionZcm");   

    name  = "hSimu_f";      name += i2s(i);
    label = "MC: ";        label += Label[i];
    _hSimu[i] = new TH1F(name.c_str(),label.c_str(),BinNIPH,MinIPH,MaxIPH);
    cmd  = "((PMT3+PMT5+PMT6+PMT7+PMT8+PMT10+PMT11)*"; cmd += f2s(step); cmd += ")>>"; cmd += name;
    cn[1]->cd(i+1);

    _hData[i]->Draw();
    _hData[i]->SetTitle(Label[i].c_str());
    _hData[i]->GetXaxis()->SetTitle("Light [p.e.]");
    _hData[i]->GetYaxis()->SetTitle("Events");
    _hData[i]->GetXaxis()->SetTitleOffset(1.5);
    _hData[i]->GetYaxis()->SetTitleOffset(1.5);
    _hData[i]->GetXaxis()->SetTitleSize(.045);
    _hData[i]->GetYaxis()->SetTitleSize(.045);
    _hData[i]->GetXaxis()->SetLabelSize(.045);
    _hData[i]->GetYaxis()->SetLabelSize(.045);
 

    _nDatEvents[i] = _hData[i]->GetEntries();
    _hData[i]->SetLineColor(1);
    
    SimuTree[i]->Draw(cmd.c_str(),"","same");
    _nSimEvents[i] = _hSimu[i]->GetEntries();

    _hData[i]->Draw();
    //_hDataCut1[i]->Draw("same");
    _hDataCut2[i]->Draw("same");


    for (int j=0;j<BinNIPH;j++) 
      _hSimuVals[i][j] = _hSimu[i]->GetBinContent(j+1);
    
    //  hMax[i] = _hSimu[i]->GetMaximumBin();
    // _hSimu[i]->SetBinContent(hMax[i],_hSimu[i]->GetBinContent(hMax[i])*hZeroScale[i]);
    _hSimu[i]->SetLineColor(kBlue); 
    _maxBin    = _hSimu[_run]->GetMaximumBin();
    _maxBinVal = _hSimu[_run]->GetBinContent(_maxBin);
    cout << _hSimu[_run]->GetMaximumBin() << "   "  << _hSimu[_run]->GetBinContent(_maxBin) <<  endl;
    cout << _maxBin << endl;
    gPad->SetLogy();
    cn[1]->Update();
    //    _hData[i] ->Fit("fg","R");
    fg->GetParameters(GPar);


    //
    // Convolutions
    //
    name  = "hSimuConv_f";      name += i2s(i);
    _hSimuConv[i] =new TH1I(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);

    name  = "hSimuConvB_f";      name += i2s(i);
    hSimuConv[i] =new TH1F(name.c_str(),name.c_str(),BinNIPH,MinIPH,MaxIPH);
 
    name = "SimToData_f"; name += i2s(i);
    fConv[i] = new TF1 (name.c_str(),Sim2Data,-100,400,5);
    fConv[i]->SetNpx(500);
    // Double_t hZeroScale[4] = {   1,  1,  1,  1};//0.55, 0.55, 0.55, 0.55};
    // Int_t    ColorIndex[4] = {   1,  2,  8,  9};
    // Double_t hConvSigma[4] = {  25, 25, 25, 25};
    // Double_t hConvExp[4]   = {  25, 25, 25, 25};
    // Int_t    hConvOffs[4]  = {   4,  4,  4,  4};
    // Double_t tmpval= _hSimu[i]->SetBinContent(_maxBin, _hSimu[i]->GetBinContent(_maxBin)*hZeroScale[i]);
    fConv[i]->SetParameters(hConvSigma[i],
			    hConvOffs[i],
			    hConvExp[i],
			    _hSimu[i]->GetBinContent(_maxBin)*hZeroScale[i],
			    (Double_t) i);
    fConv[i]->SetLineColor(8);
    fConv[i]->SetLineWidth(2);

    fConv[i]->Draw("same");
    
    fConv[i]->SetParLimits(0,20,30);
    fConv[i]->FixParameter(0,hConvSigma[i]);
    
    fConv[i]->SetParLimits(1,5,20);
    fConv[i]->FixParameter(1,hConvOffs[i]);
    
    fConv[i]->SetParLimits(2,0.9,1.3);
    
    fConv[i]->SetParLimits(3,1e4,2e5);
    
    fConv[i]->FixParameter(4,(Double_t) i);
    // fConv[i]->SetParLimits(3,8e4,2e5);
    // double tmpFF = fg->Integral(-200,200);//GPar[0]/(GPar[2]*TMath::Sqrt(2*TMath::Pi()));
    // cout << " " << tmpFF << endl; 
    // fConv[i]->FixParameter(3,tmpFF);
    
    //     _hData[i] ->Draw();
    _hData[i] ->Rebin(_RebinLevel);
    _hDataCut1[i] ->Rebin(_RebinLevel);
    _hDataCut2[i] ->Rebin(_RebinLevel);

    leg[i]->AddEntry(_hData[i], "Data (w/o cuts)","l");
    // leg[i]->AddEntry(_hDataCut1[i], "Data (cuts)","l");
    leg[i]->AddEntry(_hDataCut2[i], "Data (with cuts)","l");
    // leg[i]->AddEntry(_hSimu[i], "MC","l");
    leg[i]->AddEntry( fConv[i], "MC - digitized","l");
   
    if (i==0) leg[i]->Draw();
    _hData[i]->Draw("same");
    cn[1]->Update();
    //_hData[i] ->Fit(name.c_str(),"R","",-100,350);//,-100,400);
    
#define SELFMINIMIZER   
#ifdef  SELFMINIMIZER   
    name = "_Chi_f"; name += i2s(i);
    _Chi[i]= new TH2F (name.c_str(),name.c_str(),21,-10.5,10.5,21,-10.5,10.5);
    
    
    _BStart[i]= 1;
    _BStop[i] = 1+(Int_t) (400./(Float_t) _RebinLevel);
    Double_t minchi2=1e6;
    
    Double_t valk,valh;
    Double_t valkm,valhm;
    Double_t Bvalk,Bvalh;
    for (int h=-10;h<10;h++){
      for (int k=-10;k<10;k++){
	
	valk = hConvExp[i]+(k*pStep[2]);
	valh =(hZeroScale[i]+ h*pStep[3]);
	
	fConv[i]->SetParameter(2,valk);
	fConv[i]->SetParameter(3,_maxBinVal*valh);
	fConv[i]->GetParameters(fPar);    
	double chi2 = 0; 
	double x[1],ey; 
	double tmp;
	npfits = 0;
	//Int_t run  = (Int_t) fPar[4];
	for (int j=_BStart[i]; j< _BStop[i];j++){
	  x[0]= _hDataCut2[i]->GetBinCenter(j);
	  ey=  TMath::Sqrt(_hDataCut2[i]->GetBinContent(j));
	  if (ey<1) ey= 1;
	  tmp = (_hDataCut2[i]->GetBinContent(j)- Sim2Data(x,fPar))/ ey;
	  chi2 += tmp*tmp; 
	  npfits++;
	  
	}
	_Chi[i]->Fill(k,h,chi2);
	if (chi2 < minchi2 ) { minchi2 = chi2; Bvalk=valk; Bvalh=valh;}
	cout << chi2 << " " << fPar[2] <<" " << fPar[3]/_maxBinVal << endl;
	
	
      }
    }
    cout << " Minima at 2:" << Bvalk << "  3:" << Bvalh << "   Chi2=" << minchi2/(_BStop[i]-_BStart[i]) << endl;  
    //getchar();
    fConv[i]->SetParameter(2,Bvalk);
    fConv[i]->SetParameter(3,_maxBinVal*Bvalh);
    _hData[i]->Draw();
    fConv[i]->Draw("same");
    if (i==0) leg[i]->Draw();
    _hDataCut2[i]->Draw("same");
    _hData[i]->Draw("same");
    _hSimu[i]->Draw("same");
#endif  
    
#ifdef MINUIT   
    TVirtualFitter::SetDefaultFitter("Minuit");
    TVirtualFitter * minuit = TVirtualFitter::Fitter(0,5);
    
    minuit->SetParameter(0, fConv[i]->GetParName(0),  hConvSigma[i], pStep[0],  25,  30);//hConvSigma[i] ,hConvSigma[i]);
    minuit->FixParameter(0);
    
    minuit->SetParameter(1, fConv[i]->GetParName(1),  hConvOffs[i],  pStep[1],   1,  20);
    minuit->FixParameter(1);
    
    minuit->SetParameter(2, fConv[i]->GetParName(2),  hConvExp[i],   pStep[2], 0.9, 1.4);
    minuit->FixParameter(2);
    
    minuit->SetParameter(3, fConv[i]->GetParName(3),  _hSimu[i]->GetBinContent(_maxBin)*hZeroScale[i], pStep[3],1e4 ,1e5);
    
    minuit->SetParameter(4, fConv[i]->GetParName(4),  (Double_t) i, 0, (Double_t) i ,(Double_t) i);
    minuit->FixParameter(4);
    minuit->SetFCN(myFcn);   
    
    double arglist[100];
    arglist[0] = 0;
    // set print level
    minuit->ExecuteCommand("SET PRINT",arglist,2);
    
    // minimize
    arglist[0] = 5000; // number of function calls
    arglist[1] = 2.; // tolerance
    minuit->ExecuteCommand("MIGRAD",arglist,2);
#endif  
  }     
//   for (int i=0;i<RUNS;i++){
//     cn[2]->cd(i+1);
//     _Chi[i]->Draw("COLZ");
//     cn[2]->Update();
//   }
}
