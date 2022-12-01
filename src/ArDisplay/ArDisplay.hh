#include <TGFrame.h>
#include "TGStatusBar.h"

#include <TGraph.h>

#include "ArAnalysis.hh"
#include "ArRun.hh"
#include "ArConfiguration.hh"

// class TGWindow;
// class TGMainFrame;
// class TRootEmbeddedCanvas;

class ArDisplay : public TGMainFrame {

private:
  TRootEmbeddedCanvas *fEcanvas;
  TGStatusBar *fStatusBar;

  ArAnalysis *fAnalysis;
  ArRun *fRun;

  TGNumberEntry *fEventNumber;
  Long64_t EventNumber;
  TGCheckButton *fTextButtonAutomaticView;

  TCanvas *cTraceAll;
  TCanvas *cTraceSum;
  TCanvas *cPlotsPeakTime;
  TCanvas *cPlotsCRvsIntegral;
  TCanvas *cPlotsPedestal;

  TClonesArray *graph;
  TClonesArray *frame;
  TClonesArray *hPeakTime;
  TClonesArray *hCRvsIntegral;
  TClonesArray *hPedestal;
  TArrayD *Time;

  TClonesArray *ped_line;
  TClonesArray *peaktime_line;
  TClonesArray *peakstarttime_line;
  TClonesArray *pedtime_line;
  TClonesArray *analysis_text;

  Double_t margin;

public:
  ArDisplay(const TGWindow *p,UInt_t w,UInt_t h, ArAnalysis *analysis, ArRun *run);
  virtual ~ArDisplay();
  //  void DoDraw();
  void Initialize();
  void Process();
  void LoadEvent();
  void NextEvent();
  void PreviousEvent();
  void AnalyseEvent();

  void CleanGraphs();
  void DisplayEvent();
  void RedisplayEvent();
  void DisplayTraceAll();
  void DisplayTraceSum();
  void DisplayPlotsPeakTime();
  void DisplayPlotsCRvsIntegral();
  void DisplayPlotsPedestal();
  ClassDef(ArDisplay,0)
};
