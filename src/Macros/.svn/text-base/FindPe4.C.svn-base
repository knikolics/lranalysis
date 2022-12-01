TF1 *fSkellam;
TF1 *fPoisson;
TF1 *FunPe;
TH1F *hh;

Double_t SkellamPe(Double_t *x, Double_t *par)
{
  Int_t n;
  if (x[0]!=0) n = (Int_t)(x[0]-0.5+0.5*x[0]/TMath::Abs(x[0]));
  else n = 0;
  return (Double_t) ( par[0]*exp(-2*par[1])*TMath::BesselI(abs(n),2*par[1]) );
}

Double_t Skellam2Pe(Double_t *x, Double_t *par)
{
  Int_t n;
  if (x[0]!=0) n = (Int_t)(x[0]-0.5+0.5*x[0]/TMath::Abs(x[0]));
  else n = 0;
  return (Double_t) ( par[0]*exp(-(par[1]+par[5]))*TMath::Power(par[1]/par[5],n/2.)*TMath::BesselI(abs(n),2*sqrt(par[1]*par[5])) );
}

Double_t PoissonPe(Double_t *x, Double_t *par)
{
  if (x[0]>=0.) return (Double_t) ( par[2]*TMath::Poisson(x[0],par[4]) );
  else return (Double_t)(0.);
}

Double_t Function2Pe(Double_t *x, Double_t *par)
{
  return (Double_t) ( Skellam2Pe(x,par) + PoissonPe(x,par) );
}

Double_t FunctionPe(Double_t *x, Double_t *par)
{
  return (Double_t) ( SkellamPe(x,par) + PoissonPe(x,par) );
}

void FindPe(const char *filename)
{
  gStyle->SetOptFit(11111);

  TFile *f = new TFile();
  f->Open(filename);

  Int_t NPMTs;
  Data->SetBranchAddress("NPMTs",&NPMTs);
  Data->GetEntry(0);


  TCanvas *ctb = new TCanvas("Number of peaks top-bottom","Number of peaks top-bottom");
  TCanvas *c = new TCanvas("Number of peaks","Number of peaks");
  Int_t sqrtn = (int)(sqrt(NPMTs)+0.5);
  ctb->Divide(sqrtn,sqrtn);
  c->Divide(sqrtn,sqrtn);
  Data->SetMarkerStyle(1);

  char regexp[100],selection[100];

  for (Int_t i=0;i<NPMTs;i++)
    //  for (Int_t i=1;i<2;i++)
    {
      ctb->cd(i+1);
      gPad->SetLogy();

      gStyle->SetOptStat(1111);

      char funpe_name[10];
      sprintf(funpe_name,"FunPe%d",i);
      FunPe = new TF1(funpe_name,FunctionPe,-6,10,6);

      sprintf(regexp,"PMT[%d].eNpe",i);
      sprintf(selection,"PMT[%d].eNpe==0",i);      
      Int_t M = Data->Draw(regexp,selection);
      Double_t mu = 1.0E-4;
      Double_t A  = M*exp(2*mu)/TMath::BesselI(0,2*sqrt(mu));

      sprintf(regexp,"PMT[%d].eNpe",i);
      sprintf(selection,"PMT[%d].eNpe==2",i);
      Double_t N = (Double_t)(Data->Draw(regexp,selection));
      Double_t mean = 1;
      Double_t sigma = 1.;

      FunPe->SetParameters(A,mu,N,0,mean,mu);

      sprintf(regexp,"PMT[%d].eNpe",i);
      sprintf(selection,"PMT[%d].eNpe>=-6&&PMT[%d].eNpe<=10&&PMT[%d].ePeakTime>950&&PMT[%d].ePeakTime<1100",i,i,i,i);
      Data->Fit(funpe_name,regexp,selection);

      char skellam_name[10];
      sprintf(skellam_name,"Skellam%d",i);
      fSkellam = new TF1(skellam_name,SkellamPe,-6,10,6);
      fSkellam->SetParameters(FunPe->GetParameters());

      char poisson_name[10];
      sprintf(poisson_name,"Poisson%d",i);
      fPoisson = new TF1(poisson_name,PoissonPe,-6,10,6);
      fPoisson->SetParameters(FunPe->GetParameters());
      
      fSkellam->Draw("same");
      fSkellam->SetLineColor(2);
      fPoisson->Draw("same");
      fPoisson->SetLineColor(3);
      FunPe->Draw("same");
      FunPe->SetLineColor(1);
      
      //   TH1F *po = new TH1F("po","po",10,0,10);
      //   for (int i=0;i<10;i++) {
      //     Double_t x[1]={i*1.};
      //     po->Fill(x[0],PoissonPe(x,fPoisson->GetParameters()));
      //   }
      //   po->Draw();

      printf("Photoelectrons: Mean value of the Poisson fit = %f\n",FunPe->GetParameter(4));
      
      c->cd(i+1);     

      char hh_name[10];
      sprintf(hh_name,"hh%d",i);
      hh = new TH1F(hh_name,hh_name,10,0,10);
      sprintf(regexp,"PMT[%d].eNpe>>%s",i,hh_name);
      sprintf(selection,"PMT[%d].eNpe>=-6&&PMT[%d].eNpe<=10&&PMT[%d].ePeakTime>950&&PMT[%d].ePeakTime<1100",i,i,i,i);      
      Data->Draw(regexp,selection);
      
      Double_t x[1]={0.5};
      //      printf("%f %f\n",hh->GetBinContent(1),Skellam2Pe(x,fSkellam->GetParameters()));
      //  for (Int_t i=0;i<6;i++) printf("%f\n",fSkellam->GetParameter(i));
      
      char pe_name[10];
      sprintf(pe_name,"pe%d",i);
      TH1F *pe = new TH1F(pe_name,pe_name,10,0,10);
      for (int j=0;j<10;j++) {
	Double_t x[1]={j*1.+0.5};
	pe->Fill(x[0],hh->GetBinContent(j+1)-SkellamPe(x,fSkellam->GetParameters()));
	//    pe->Fill(x[0],Skellam2Pe(x,fSkellam->GetParameters()));
	//    pe->Fill(x[0],hh->GetBinContent(j+1));
      }

      gStyle->SetOptStat(1111101);
      pe->Draw("E");
      fPoisson->Draw("same");
      
      printf("Photoelectrons: Mean value of the distribution = %f\n",pe->GetMean());
    }


  //  new TCanvas;


//   // total distribution
//   TH1F *totf = new TH1F("totf","totf",20,-10,10);
//   Data->Draw("eNpe>>totf");
//   totf->Draw();
//   //  printf("%d\n",totf->GetNbinsX());

//   // taking the noise
//   TH1F *noise = new TH1F("noise","noise",20,-10,10);
//   for (int i=0;i<21;i++) {
//     Double_t x[1]={-10.+i*1.+0.5};
//     Double_t *y = fSkellam->GetParameters();
//     noise->Fill(x[0],Skellam2Pe(x,y));
//   }


//   // total distribution
//   TH1F *totf = new TH1F("totf","totf",6,0,6);
//   Data->Draw("eNpe>>totf");
//   totf->Draw();
//   //  printf("%d\n",totf->GetNbinsX());

//   // taking the noise
//   TH1F *noise = new TH1F("noise","noise",6,0,6);
//   for (int i=0;i<6;i++) {
//     Double_t x[1]={i*1.+0.5};
//     Double_t *y = fSkellam->GetParameters();
//     noise->Fill(x[0],Skellam2Pe(x,y));
//   }
//   noise->Draw("");

//   // make difference
//   totf->Add(noise,-1);
//   totf->Draw();
//   fPoisson->Draw("same");

//   printf("Average number of photoelectrons = %f\n",totf->GetMean());
}
