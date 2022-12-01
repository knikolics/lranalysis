#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <TROOT.h>
#include <TClonesArray.h>
#include <TSystem.h>

#include "ArRun.hh"
#include "ArDetector.hh"
#include "ArConfiguration.hh"
#include "ArAnalysis.hh"
#include "ArIO.hh"

ClassImp(ArConfiguration)


ArConfiguration :: ArConfiguration()
{
  Reset();
}



ArConfiguration :: ArConfiguration(const char *path, Int_t runnumber)
{
  Reset();
  ePath=path;
  eRunNumber=runnumber;
}



ArConfiguration :: ~ArConfiguration()
{

}


void ArConfiguration :: Reset()
{
  BitResolution = 10;
  SampleSize = 1.;
  FirstEvent = 0;
  LastEvent = -1;
  EventsInTheMask = 0;
  EventsMaskFile = "";

  CalibrationFile="";
  Calibration.Set(0);
  Delay.Set(0);
  ApplyDelayCorrectionFromDAQInfo = 1;
  EvaluateChannelsFromVirtualChannel = 1;
  FakeSignalTime = -1;
  SingletIntegrationTime = 50.;
  PedPeakFindingTimeMargin = 10.;
  PeakFindingTimeMargin = 20.;
  PedPeakFindingDisentangleTime = 40.;
  PeakFindingDisentangleTime = 40.;
  PedPeakFindingSigmaThreshold = 4.;
  PeakFindingSigmaThreshold = 4.;
  Pedestal.Set(0);
  VirtualChannelMask=0;
  VirtualChannelMaskList.Set(0);

  ReferenceConversionFactor = 1.0;
  MaximumIntegrationTime = 10000.;
}



void ArConfiguration :: Print(FILE *fp)
{
  ArIO::PrintColor(fp,"** Analysis Configuration\n");
  fprintf(fp,"** Current file name used: %s\n",eCurrentFileName.Data());
  fprintf(fp,"** Bit resolution = %d bits\n",BitResolution);
  fprintf(fp,"** Sample size = %f ns\n",SampleSize);
  if (EventsMaskFile!="") {
    fprintf(fp,"** Event mask from file used\n");
    fprintf(fp,"** Events in the mask file %s = %d\n",EventsMaskFile.Data(),EventsInTheMask);
  } else if (EventsInTheMask) {
    fprintf(fp,"** Event mask used\n");
    fprintf(fp,"** Events in the mask = %d\n",EventsInTheMask);
  } else {
    fprintf(fp,"** Event range used\n");
    fprintf(fp,"** Events in the range [%lld,%lld]\n",FirstEvent,LastEvent);
  }
  if (ApplyDelayCorrectionFromDAQInfo)
    fprintf(fp,"** Delay correction from DAQ info is applied\n");
  else fprintf(fp,"** Delay correction from DAQ info is not applied\n");
  if (EvaluateChannelsFromVirtualChannel)
    fprintf(fp,"** Channels are re-eveluated after virtual channel\n");
  else fprintf(fp,"** Channels are not re-eveluated after virtual channel\n");

  if (FakeSignalTime>=0)
    fprintf(fp,"** Fake signal time = %d\n",FakeSignalTime);

  fprintf(fp,"** Singlet integration time = %f ns\n",SingletIntegrationTime);
  fprintf(fp,"** Pedestal peak finding time margin = %f ns\n",PedPeakFindingTimeMargin);
  fprintf(fp,"** Peak finding time margin = %f ns\n",PeakFindingTimeMargin);

  fprintf(fp,"** Pedestal peak finding disentangle time = %f ns\n",PedPeakFindingDisentangleTime);
  fprintf(fp,"** Peak finding disentangle time = %f ns\n",PeakFindingDisentangleTime);
  fprintf(fp,"** Pedestal peak finding sigma threshold = %f\n",PedPeakFindingSigmaThreshold);
  fprintf(fp,"** Peak finding sigma threshold = %f\n",PeakFindingSigmaThreshold);

  fprintf(fp,"** Pedestal time range = [%d,%d]\n",StartPedestal,StopPedestal);
  fprintf(fp,"** Peak signal time range = [%d,%d]\n",StartPeakSignal,StopPeakSignal);
  fprintf(fp,"** Reference conversion factor = %f\n",ReferenceConversionFactor);
  if (VirtualChannelMask) {
    fprintf(fp,"** Virtual channel mask set to: ");
    for (int i=0;i<VirtualChannelMaskList.GetSize();i++) fprintf(fp,"%d ",VirtualChannelMaskList[i]);
    fprintf(fp,"\n");
  }
  fprintf(fp,"** Maximum time for the evaluation of the integral = %f ns\n",MaximumIntegrationTime);
}



Int_t ArConfiguration :: Load()
{
  TString completefilename;
  ArIO::MakeFileName(completefilename,ePath,eRunNumber,-1,"cfg");

  TString localfilename = completefilename;
  localfilename.Remove(0, localfilename.Last('/') + 1);

  if (!gSystem->AccessPathName(localfilename.Data(),kFileExists)) eCurrentFileName = localfilename;
  else if (!gSystem->AccessPathName(completefilename.Data(),kFileExists)) eCurrentFileName = completefilename;
  else {
    if (ArDBLEVEL>=2) printf("Warning! No configuration file %s found\n",localfilename.Data());
    eCurrentFileName = "";
    return(0);
  }

  char phrase[1000];
  sprintf(phrase,"** Loading analysis configuration file: %s\n",eCurrentFileName.Data());
  ArIO::PrintColor(stdout,phrase);

  ifstream s;
  s.open(eCurrentFileName.Data());

  if (!s.is_open()) {
    if (ArDBLEVEL>=2) printf("Warning! Configuration file %s cannot be opened\n",eCurrentFileName.Data());
    return(0);
  }
  
  TString card;

  while (s.good() && (!s.eof())) {

    s >> card;

    if (card=="DEBUGLEVEL") {
      s >> ArDBLEVEL;
      if (ArDBLEVEL>=4) printf("Setting debug level to %d\n",ArDBLEVEL);
    }
    
    else if (card=="BIT") {
      s >> BitResolution;
      if (ArDBLEVEL>=4) printf("Setting DAQ resolution to %d bits\n",BitResolution);
    }

    else if (card=="SAMPLE_SIZE") {
      s >> SampleSize;
      if (ArDBLEVEL>=4) printf("Setting sample size to %f ns\n",SampleSize);
    }

    else if (card=="EVENTS") {
      //      EventsInTheMask = 0;
      s >> FirstEvent;
      s >> LastEvent;
      if (ArDBLEVEL>=4) printf("Setting events range from %lld to %lld\n",FirstEvent,LastEvent);
    }

    else if (card=="EVENTS_MASK") {
      s >> EventsInTheMask;
      if (EventsInTheMask) ReadMask(s);
      if (ArDBLEVEL>=4) printf("Setting %d events from the mask\n",EventsInTheMask);
    }

    else if (card=="EVENTS_MASK_FILE") {
      s >> EventsMaskFile;
      if (ArDBLEVEL>=4) printf("Setting the events mask file to %s\n",EventsMaskFile.Data());
      LoadEventsMaskFile();
      if (ArDBLEVEL>=4) printf("Setting %d events from the mask file\n",EventsInTheMask);
    }

    else if (card=="DELAY") {
      ReadDelay(s);
      if (ArDBLEVEL>=4) printf("Setting delay time to the given values\n");
    }

    else if (card=="APPLY_DELAY_CORRECTION_FROM_DAQ_INFO") {
      s >> ApplyDelayCorrectionFromDAQInfo;
      if (ArDBLEVEL>=4) {
	if (ApplyDelayCorrectionFromDAQInfo) printf("Delay correction from DAQ info is applied\n");
	else printf("Delay correction from DAQ info is not applied\n");
      }
    }

    else if (card=="EVALUATE_CHANNELS_FROM_VIRTUAL_CHANNEL") {
      s >> EvaluateChannelsFromVirtualChannel;
      if (ArDBLEVEL>=4) {
	if (EvaluateChannelsFromVirtualChannel) printf("Channels are re-eveluated after virtual channel\n");
	else printf("Channels are not re-eveluated after virtual channel\n");
      }
    }

    else if (card=="SET_FAKE_SIGNAL_TIME") {
      s >> FakeSignalTime;
      if (ArDBLEVEL>=4) printf("Setting fake signal time to %d ns\n",FakeSignalTime);
    }
    
    else if (card=="SINGLET_INTEGRATION_TIME") {
      s >> SingletIntegrationTime;
      if (ArDBLEVEL>=4) printf("Setting singlet integration time to %f ns\n",SingletIntegrationTime);
    }

    else if (card=="PEDESTAL_PEAK_FINDING_TIME_MARGIN") {
      s >> PedPeakFindingTimeMargin;
      if (ArDBLEVEL>=4) printf("Setting pedestal peak finding time margin to %f ns\n",PedPeakFindingTimeMargin);
    }

    else if (card=="PEAK_FINDING_TIME_MARGIN") {
      s >> PeakFindingTimeMargin;
      if (ArDBLEVEL>=4) printf("Setting peak finding time margin to %f ns\n",PeakFindingTimeMargin);
    }

    else if (card=="PEDESTAL_PEAK_FINDING_DISENTANGLE_TIME") {
      s >> PedPeakFindingDisentangleTime;
      if (ArDBLEVEL>=4) printf("Setting pedestal peak finding disentangle time to %f ns\n",PedPeakFindingDisentangleTime);
    }

    else if (card=="PEAK_FINDING_DISENTANGLE_TIME") {
      s >> PeakFindingDisentangleTime;
      if (ArDBLEVEL>=4) printf("Setting peak finding disentangle time to %f ns\n",PeakFindingDisentangleTime);
    }

    else if (card=="PEDESTAL_PEAK_FINDING_SIGMA_THRESHOLD") {
      s >> PedPeakFindingSigmaThreshold;
      if (ArDBLEVEL>=4) printf("Setting pedestal peak finding sigma threshold to %f\n",PedPeakFindingSigmaThreshold);
    }

    else if (card=="PEAK_FINDING_SIGMA_THRESHOLD") {
      s >> PeakFindingSigmaThreshold;
      if (ArDBLEVEL>=4) printf("Setting peak finding sigma threshold to %f\n",PeakFindingSigmaThreshold);
    }

    else if (card=="CALIBRATION_FILE") {
      s >> CalibrationFile;
      if (ArDBLEVEL>=4) printf("Setting calibration file to %s\n",CalibrationFile.Data());
    }
    
    else if (card=="CALIBRATION") {
      ReadCalibration(s);
      if (ArDBLEVEL>=4) printf("Setting calibration to the given values\n");
    }

    else if (card=="PEDESTAL") {
      ReadPedestal(s);
      if (ArDBLEVEL>=4) printf("Setting pedestal level to the given values\n");
    }

    else if (card=="PEDESTAL_TIME_RANGE") {
      s >> StartPedestal;
      s >> StopPedestal;
      if (ArDBLEVEL>=4) printf("Setting pedestal time range from %d to %d\n",StartPedestal,StopPedestal);
    }

    else if (card=="PEAK_SIGNAL_TIME_RANGE") {
      s >> StartPeakSignal;
      s >> StopPeakSignal;
      if (ArDBLEVEL>=4) printf("Setting peak signal time range from %d to %d\n",StartPeakSignal,StopPeakSignal);
    }

    else if (card=="REFERENCE_CONVERSION_FACTOR") {
      s >> ReferenceConversionFactor;
      if (ArDBLEVEL>=4) printf("Setting reference conversion factor to %f\n",ReferenceConversionFactor);
    }

    else if (card=="VIRTUAL_CHANNEL_MASK") {
      ReadVirtualChannelMask(s);
      if (ArDBLEVEL>=4) printf("Setting the mask of PMT for building the virtual channel to the given values\n");
    }

    else if (card=="MAXIMUM_INTEGRATION_TIME") {
      s >> MaximumIntegrationTime;
      if (ArDBLEVEL>=4) printf("Setting the maximum time for the evaluation of the integral to %f ns\n",MaximumIntegrationTime);
    }


  }

  s.close();

  return(1);
}



Int_t ArConfiguration :: ReadMask(ifstream &s)
{
  FileList.Set(EventsInTheMask);
  EventList.Set(EventsInTheMask);

  Int_t ifile, ievent;

  for (Int_t i=0;i<EventsInTheMask;i++)
    {
      s >> ifile;
      s >> ievent;
      FileList.AddAt(ifile,i);
      EventList.AddAt(ievent,i);
      if (!s.good() || s.eof()) {
	printf("Warning! Events mask stream unexpectedly ended\n");
	return(-1);
      }
    }

  return(EventsInTheMask);
}



Int_t ArConfiguration :: LoadEventsMaskFile()
{
  ifstream s;
  s.open(EventsMaskFile.Data());

  if (!s.is_open()) {
    if (ArDBLEVEL>=2) printf("Warning! Events mask file %s cannot be opened\n",EventsMaskFile.Data());
    return(0);
  }

  printf("*** Loading event mask file: %s\n",EventsMaskFile.Data());
 
  s >> EventsInTheMask;

//   FirstEvent = 0;
//   LastEvent = EventsInTheMask - 1;

  Int_t status = ReadMask(s);

  if (status==0) {
    printf("*** No mask entry is loaded\n");
    return(0);
  } else if (status<0) return(-1);

  s.close();

  return(EventsInTheMask);

}


Int_t ArConfiguration :: PrintMask()
{
  printf("*** Event mask, %d elements\n",EventsInTheMask);
  printf("*** File number, Event number\n");
  for (Int_t i=0;i<EventsInTheMask;i++)
    printf("%8d %8d\n",FileList[i],EventList[i]);
}



Int_t ArConfiguration :: ReadCalibration(ifstream &s)
{
  Int_t NPMTsCal;

  s >> NPMTsCal;

  Calibration.Set(NPMTsCal);

  for (Int_t i=0;i<NPMTsCal;i++)
    {
      Double_t cal;
      s >> cal;

      Calibration.AddAt(cal,i);
       if (!s.good() || s.eof()) {
	printf("Warning! Stream unexpectedly ended\n");
	return(-1);
      }
    }

  return(NPMTsCal);
}


Int_t ArConfiguration :: ReadDelay(ifstream &s)
{
  Int_t NPMTsCal;

  s >> NPMTsCal;

  Delay.Set(NPMTsCal);

  for (Int_t i=0;i<NPMTsCal;i++)
    {
      Int_t delay;
      s >> delay;

      Delay.AddAt(delay,i);
       if (!s.good() || s.eof()) {
	printf("Warning! Stream unexpectedly ended\n");
	return(-1);
      }
    }

  return(NPMTsCal);
}


Int_t ArConfiguration :: ReadPedestal(ifstream &s)
{
  Int_t NPMTsCal;

  s >> NPMTsCal;

  Pedestal.Set(NPMTsCal);

  for (Int_t i=0;i<NPMTsCal;i++)
    {
      Double_t pedestal;
      s >> pedestal;

      Pedestal.AddAt(pedestal,i);
      
      if (!s.good() || s.eof()) {
	printf("Warning! Stream unexpectedly ended\n");
	return(-1);
      }
    }

  return(NPMTsCal);
}


Int_t ArConfiguration :: ReadVirtualChannelMask(ifstream &s)
{
  Int_t NPMTsMask;

  s >> NPMTsMask;

  VirtualChannelMaskList.Set(NPMTsMask);

  for (Int_t i=0;i<NPMTsMask;i++)
    {
      Int_t flag;
      s >> flag;

      VirtualChannelMaskList.AddAt(flag,i);

      if (!s.good() || s.eof()) {
	printf("Warning! Virtual channel mask stream unexpectedly ended\n");
	return(-1);
      }
    }

  VirtualChannelMask=1;

  return(NPMTsMask);
}
