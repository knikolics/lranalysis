#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <TROOT.h>

#include "ArIO.hh"

ClassImp(ArIO)

Int_t   ArDBLEVEL = 2;

ArConfiguration *ArConf = 0;

ArIO :: ArIO()
{

}


ArIO :: ~ArIO()
{

}


TString ArIO::i2s(int run)
{
  char trun[6];
  sprintf(trun,"%d",run);
  TString srun;
  srun = trun;
  return srun;
}


TString ArIO::r2s(int run)
{
  char trun[6];
  sprintf(trun,"%.4d",run);
  TString srun;
  srun = trun;
  return srun;
}


void ArIO :: MakeFileName(TString &s, const char *path, Int_t runnumber, Int_t filenumber, const char *suffix)
{
  // make file name qith a given path, run number, file number and suffix
  // - if both runnumber and filenumber are positive, file name will be like:
  //     <path>/Run<runnumber>/Run<runnumber>-File<filenumber>.<suffix>
  // - if filenumber is negative, file name will be like:
  //     <path>/Run<runnumber>/Run<runnumber>-File.<suffix>
  // - if runnumber and filenumber are negative, file name will be like:
  //     <path>.<suffix>
  // - if runnumber is negative, file name will be like:
  //     <path><filenumber>.<suffix>
  // - if path is NULL, file name is local

  char str[256];
  TString completepath(path,256);

//   if (path) {
//     completepath[strlen(path)]='/';
//     completepath[strlen(path)+1]='\0';
//   }
  
  if (runnumber>=0) {
    if (filenumber>=0)
      sprintf(str,"%s/Run%04d/Run%04d-File%d.%s",
	      completepath.Data(),runnumber,runnumber,filenumber,suffix);
    else
      sprintf(str,"%s/Run%04d/Run%04d-File.%s",
	      completepath.Data(),runnumber,runnumber,suffix);
  } else {
    if (filenumber>=0)
      sprintf(str,"%s%04d.%s",
	      path,filenumber,suffix);      
    else
      sprintf(str,"%s/%s",
	      completepath.Data(),suffix);
  }
  
  s=str;
}


void ArIO :: PrintColor(FILE *fp, const char *phrase)
{
  fprintf(fp,"%c[%d;%dm%s%c[%dm", 0x1B, 1, 31, phrase, 0x1B, 0);
}
