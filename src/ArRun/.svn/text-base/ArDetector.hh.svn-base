#ifndef Ar_ArDetector
#define Ar_ArDetector

#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <TROOT.h>
#include <TClonesArray.h>
#include <TArrayI.h>

#include "ArRun.hh"
#include "ArAnalysis.hh"

class ArDetector : public TObject {
  
protected:
  
  // Channel related variables
  Int_t    eID;
  TString  eName;
  TString  eType;

public:
  ArDetector();
  ~ArDetector();
  
  TArrayS RawData;
  TArrayD Data;

  Int_t    ID()          { return eID;     }
  TString  Name()        { return eName;   }
  TString  Type()        { return eType;   }

  void SetID(Int_t id)       { eID=id;     }
  void SetName(TString name) { eName=name; }
  void SetType(TString type) { eType=type; }

  ClassDef(ArDetector,1);
};

#endif /* Ar_ArDetector */
