// Int_t a = gSystem->Load("libTree.so");
// Int_t b = gSystem->Load("libGui.so");
Int_t c = gSystem->Load("../lib/ArDM.so");

Int_t NPMTs,FileNumber,EventNumber;
Double_t ePeakValue;
TClonesArray *pmt = new TClonesArray("ArRecoPMT",14);
ArRecoPMT *vpmt;

Int_t Selected1()
{
  Int_t selected=1;
  for (Int_t ipmt=0;ipmt<NPMTs;ipmt++) {  
    ArRecoPMT *p = (ArRecoPMT *)pmt->At(ipmt);
    
    if (!(1
	  && p->CR()<2 && p->CR()>0
	  && TMath::Abs(p->PeakTime()-295)<25
	  //	  && TMath::Abs(p->PeakTime()-1040)<60
	  //	    && p->PeakIsSaturated()==0
	  //	    && p->Npe()==1
	  // && p->PeakValue()<0.6
	  )) selected=0;
    
  }
  return(selected);
}

Int_t Selected2() // Selection only for PMT number 1
{
  Int_t selected=1;
  ArRecoPMT *p = (ArRecoPMT *)pmt->At(0);
  
  if (!(1
	&& p->CR()<2 && p->CR()>0
 	&& TMath::Abs(p->PeakTime()-295)<25
 	&& p->PeakIsSaturated()==0
 	&& p->Npe()==1
 	)) selected=0;
  
  return(selected);
}


Int_t SelectedAlpha()
{
  Int_t selected=1;

//   for (Int_t ipmt=0;ipmt<NPMTs;ipmt++) {  
//     ArRecoPMT *p = (ArRecoPMT *)pmt->At(ipmt);
    
//     if (!(1
// 	  && TMath::Abs(p->PeakTime()-295)<25 // ???
// 	  && p->PeakIsSaturated()==0
// 	  )) selected=0;
//   }

  if (!(1
	&& vpmt->CR()>0.25 && vpmt->CR()<0.6
	&& vpmt->Integral()>70. && vpmt->Integral()<160.
	)) selected=0;
  
  return(selected);
}


Int_t SelectedFewPe()
{
  Int_t selected=1;

  if (!(1
	&& vpmt->PedSigma()<0.006648
	&& vpmt->PedTimePe()>500
	&& vpmt->PeakTime()<3000
	&& vpmt->PeakValue()<0.060
	)) selected=0;

  // if (!(1
  // 	&& vpmt->PeakTime()>900 && vpmt->PeakTime()<1100
  // 	&& vpmt->IntegralPe()/0.2>0. && vpmt->IntegralPe()/0.2<10.
  // 	&& vpmt->PedSigma()<0.013
  // 	)) selected=0;
  
  return(selected);
}


Int_t SelectedGamma()
{
  Int_t selected=1;

  if (!(1
	&& vpmt->CR()>0. && vpmt->CR()<1.2
	//	&& vpmt->Integral()>70. && vpmt->Integral()<170.
	&& TMath::Abs(vpmt->PeakTime()-1015)<60
	)) selected=0;
  
  return(selected);
}
Int_t SelectedMuon()
{
  Int_t selected=1;
  //  cout <<  vpmt->CR() << endl;
  if (!(1
	&& vpmt->CR()>0.2 && vpmt->CR()<0.5
	&& vpmt->Integral()>800. && vpmt->Integral()<1500.
	&& TMath::Abs(vpmt->PeakTime()-275)<25
	)) selected=0;
  
  return(selected);
}

void SelectEvents(const char *filename, const char *outputfile=0)
{
  FILE *fp;

  if (outputfile!=0) {
    if ( (fp = fopen(outputfile,"w")) == NULL ) {
      printf("Error: file %s cannot be created. Disk full or check your permissions\n",outputfile);
      return;
    }
  } else fp = stdout;

  TFile *f = new TFile();
  f->Open(filename);

  Data->SetBranchAddress("NPMTs",&NPMTs);
  Data->SetBranchAddress("FileNumber",&FileNumber);
  Data->SetBranchAddress("EventNumber",&EventNumber);
  //  Data->SetBranchAddress("PMT.ePeakValue",&ePeakValue);
  //  Data->SetBranchAddress("PMT.eID",&eID);
  Data->SetBranchAddress("PMT",&pmt);
  Data->SetBranchAddress("VPMT",&vpmt);

  Int_t selected_events = 0;

  for (Int_t ievent=0;ievent<Data->GetEntries();ievent++) {

    if (fp!=stdout) fprintf(stderr," > %d/%d\r",ievent,Data->GetEntries());

    Data->GetEntry(ievent);
    //    if (Selected1())
    //    if (Selected2())
    //    if (SelectedAlpha())
    //    if (SelectedGamma())
    if (SelectedFewPe())
    //    if (SelectedMuon())
      { selected_events++; fprintf(fp,"%d %d\n",FileNumber,EventNumber); }
  }

  if (fp!=stdout) {
    fclose(fp);
    
    char tmp[10]="tmp1234";
    
    char command[200];
    sprintf(command,"echo %d | cat - %s > %s",selected_events,outputfile,tmp);
    gSystem->Exec(command);
    sprintf(command,"mv %s %s",tmp,outputfile);
    gSystem->Exec(command);

    printf("File %s created\n",outputfile);
  }
  else return;

  return;
}
