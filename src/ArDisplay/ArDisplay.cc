#include <TApplication.h>
#include <TGClient.h>
#include <TCanvas.h>	       
#include <TF1.h>		       
#include <TRandom.h>	       
#include <TGButton.h>	       
#include <TRootEmbeddedCanvas.h>
#include "ArDisplay.hh"

#include "TGDockableFrame.h"
#include "TGMenu.h"
#include "TGMdiDecorFrame.h"
#include "TG3DLine.h"
#include "TGMdiFrame.h"
#include "TGMdiMainFrame.h"
#include "TGMdiMenu.h"
#include "TGColorDialog.h"
#include "TGListBox.h"
#include "TGNumberEntry.h"
#include "TGScrollBar.h"
#include "TGuiBldHintsEditor.h"
#include "TRootBrowser.h"
#include "TGuiBldNameFrame.h"
#include "TGFrame.h"
#include "TGFileDialog.h"
#include "TGShutter.h"
#include "TGButtonGroup.h"
#include "TGCommandPlugin.h"
#include "TGCanvas.h"
#include "TGFSContainer.h"
#include "TGuiBldEditor.h"
#include "TGColorSelect.h"
#include "TGTextEdit.h"
#include "TGButton.h"
#include "TGFSComboBox.h"
#include "TGLabel.h"
#include "TGView.h"
#include "TGMsgBox.h"
#include "TRootGuiBuilder.h"
#include "TGFileBrowser.h"
#include "TGTab.h"
#include "TGListView.h"
#include "TGSplitter.h"
#include "TGTextEditor.h"
#include "TRootCanvas.h"
#include "TGStatusBar.h"
#include "TGListTree.h"
#include "TGuiBldGeometryFrame.h"
#include "TGToolTip.h"
#include "TGToolBar.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TGuiBldDragManager.h"
#include "TGHtmlBrowser.h"
#include "TMath.h"
#include "TArrow.h"
#include "TMarker.h"

ArDisplay::ArDisplay(const TGWindow *p,UInt_t w,UInt_t h, ArAnalysis *analysis, ArRun *run)
: TGMainFrame(p,w,h)
{
  gStyle->SetPalette(1);

  fAnalysis = analysis;
  fRun = run;
  
  // status bar
  fStatusBar = new TGStatusBar(this,939,20);
  AddFrame(fStatusBar, new TGLayoutHints(kLHintsBottom | kLHintsExpandX));
  Int_t parts[] = {15, 45, 10, 30};
  fStatusBar->SetParts(parts,4);
  
  // horizontal frame
  TGHorizontalFrame *fHorizontalFrame2409 = new TGHorizontalFrame(this,935,543,kHorizontalFrame);
  
  // vertical frame
  TGVerticalFrame *fVerticalFrame2678 = new TGVerticalFrame(fHorizontalFrame2409,192,472,kVerticalFrame | kRaisedFrame);
  fVerticalFrame2678->SetLayoutBroken(kTRUE);
  
  // horizontal frame
  TGHorizontalFrame *fHorizontalFrame3435 = new TGHorizontalFrame(fVerticalFrame2678,188,56,kHorizontalFrame);
  fHorizontalFrame3435->SetLayoutBroken(kTRUE);
  TGIcon *fIcon3471 = new TGIcon(fHorizontalFrame3435,"ArDMlogo.jpg");
  fHorizontalFrame3435->AddFrame(fIcon3471, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fIcon3471->MoveResize(2,2,62,52);
  TGLabel *fLabel3840 = new TGLabel(fHorizontalFrame3435,"LookAt, version 1.0");
  fLabel3840->SetTextJustify(36);
  fLabel3840->SetMargins(0,0,0,0);
  fLabel3840->SetWrapLength(-1);
  fHorizontalFrame3435->AddFrame(fLabel3840, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fLabel3840->MoveResize(64,12,122,32);
  
  fVerticalFrame2678->AddFrame(fHorizontalFrame3435, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX,2,2,2,2));
  fHorizontalFrame3435->MoveResize(2,2,188,56);
  
  // horizontal frame
  TGHorizontalFrame *fHorizontalFrame4033 = new TGHorizontalFrame(fVerticalFrame2678,188,24,kHorizontalFrame);
  fHorizontalFrame4033->SetLayoutBroken(kTRUE);
  TGTextButton *fTextButtonLoadEvent = new TGTextButton(fHorizontalFrame4033,"Load event");
  fTextButtonLoadEvent->Connect("Clicked()","ArDisplay",this,"LoadEvent()");
  
  fTextButtonLoadEvent->SetTextJustify(36);
  fTextButtonLoadEvent->SetMargins(0,0,0,0);
  fTextButtonLoadEvent->SetWrapLength(-1);
  fTextButtonLoadEvent->Resize(99,24);
  fHorizontalFrame4033->AddFrame(fTextButtonLoadEvent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fTextButtonLoadEvent->MoveResize(0,0,99,24);
  fEventNumber = new TGNumberEntry(fHorizontalFrame4033, (Double_t) 0,9,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 1);
  fHorizontalFrame4033->AddFrame(fEventNumber, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fEventNumber->MoveResize(104,1,83,22);
  
  fVerticalFrame2678->AddFrame(fHorizontalFrame4033, new TGLayoutHints(kLHintsNormal));
  fHorizontalFrame4033->MoveResize(0,64,188,24);
  
  // horizontal frame
  TGHorizontalFrame *fHorizontalFrame4374 = new TGHorizontalFrame(fVerticalFrame2678,190,24,kHorizontalFrame);
  fHorizontalFrame4374->SetLayoutBroken(kTRUE);
  TGTextButton *fTextButtonPreviousEvent = new TGTextButton(fHorizontalFrame4374,"Previous event");
  fTextButtonPreviousEvent->Connect("Clicked()","ArDisplay",this,"PreviousEvent()");
  fTextButtonPreviousEvent->SetTextJustify(36);
  fTextButtonPreviousEvent->SetMargins(0,0,0,0);
  fTextButtonPreviousEvent->SetWrapLength(-1);
  fTextButtonPreviousEvent->Resize(99,24);
  fHorizontalFrame4374->AddFrame(fTextButtonPreviousEvent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fTextButtonPreviousEvent->MoveResize(0,0,99,24);
  TGTextButton *fTextButtonNextEvent = new TGTextButton(fHorizontalFrame4374,"Next event");
  fTextButtonNextEvent->Connect("Clicked()","ArDisplay",this,"NextEvent()");
  fTextButtonNextEvent->SetTextJustify(36);
  fTextButtonNextEvent->SetMargins(0,0,0,0);
  fTextButtonNextEvent->SetWrapLength(-1);
  fTextButtonNextEvent->Resize(83,24);
  fHorizontalFrame4374->AddFrame(fTextButtonNextEvent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fTextButtonNextEvent->MoveResize(104,0,83,24);
  
  fVerticalFrame2678->AddFrame(fHorizontalFrame4374, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fHorizontalFrame4374->MoveResize(0,94,190,24);
  
  // horizontal frame
  TGHorizontalFrame *fHorizontalFrame4619 = new TGHorizontalFrame(fVerticalFrame2678,188,24,kHorizontalFrame);
  fHorizontalFrame4619->SetLayoutBroken(kTRUE);
  TGTextButton *fTextButton4632 = new TGTextButton(fHorizontalFrame4619,"Load first n events");
  fTextButton4632->SetTextJustify(36);
  fTextButton4632->SetMargins(0,0,0,0);
  fTextButton4632->SetWrapLength(-1);
  fTextButton4632->Resize(119,24);
  fHorizontalFrame4619->AddFrame(fTextButton4632, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fTextButton4632->MoveResize(0,0,119,24);
  TGNumberEntry *fNumberEntry4673 = new TGNumberEntry(fHorizontalFrame4619, (Double_t) 2,6,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 2,(TGNumberFormat::ELimit) 1,2);
  fHorizontalFrame4619->AddFrame(fNumberEntry4673, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fNumberEntry4673->MoveResize(126,1,59,22);
  
  fVerticalFrame2678->AddFrame(fHorizontalFrame4619, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fHorizontalFrame4619->MoveResize(0,124,188,24);
  
  // "Options" group frame
  TGGroupFrame *fGroupFrameOptions = new TGGroupFrame(fVerticalFrame2678,"Options");
  fGroupFrameOptions->SetLayoutBroken(kTRUE);
  fTextButtonAutomaticView = new TGCheckButton(fGroupFrameOptions,"Automatic view");
  fTextButtonAutomaticView->SetTextJustify(36);
  fTextButtonAutomaticView->SetMargins(0,0,0,0);
  fTextButtonAutomaticView->SetWrapLength(-1);
  fTextButtonAutomaticView->Resize(119,24);
  fTextButtonAutomaticView->SetState(kButtonDown);
  fTextButtonAutomaticView->Connect("Clicked()", "ArDisplay", this, "RedisplayEvent()");
  
  fGroupFrameOptions->AddFrame(fTextButtonAutomaticView, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fTextButtonAutomaticView->MoveResize(5,24,116,19);
  
  fGroupFrameOptions->SetLayoutManager(new TGVerticalLayout(fGroupFrameOptions));
  fGroupFrameOptions->Resize(128,64);
  fVerticalFrame2678->AddFrame(fGroupFrameOptions, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fGroupFrameOptions->MoveResize(4,176,184,64);
  
  // Quit button
  
  TGTextButton *fTextButtonQuit = new TGTextButton(fVerticalFrame2678,"&Quit","gApplication->Terminate(0)");
  fTextButtonQuit->SetTextJustify(36);
  fTextButtonQuit->SetMargins(0,0,0,0);
  fTextButtonQuit->SetWrapLength(-1);
  fTextButtonQuit->Resize(99,24);
  fVerticalFrame2678->AddFrame(fTextButtonQuit, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fTextButtonQuit->MoveResize(48,432,99,24);
  
  fHorizontalFrame2409->AddFrame(fVerticalFrame2678, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  
  // tab widget
  TGTab *fTabMain = new TGTab(fHorizontalFrame2409,735,539);
  
  // container of "Trace"
  TGCompositeFrame *fCompositeFrame2692;
  fCompositeFrame2692 = fTabMain->AddTab("Trace");
  fCompositeFrame2692->SetLayoutManager(new TGVerticalLayout(fCompositeFrame2692));
  
  // tab widget
  TGTab *fTabTrace = new TGTab(fCompositeFrame2692,727,510);
  
  // container of "Trace -> All"
  TGCompositeFrame *fCompositeFrame3017;
  fCompositeFrame3017 = fTabTrace->AddTab("All");
  fCompositeFrame3017->SetLayoutManager(new TGVerticalLayout(fCompositeFrame3017));
  
  // embedded canvas
  TRootEmbeddedCanvas *fRootEmbeddedCanvasTraceAll = new TRootEmbeddedCanvas(0,fCompositeFrame3017,719,481);
  Int_t wfRootEmbeddedCanvasTraceAll = fRootEmbeddedCanvasTraceAll->GetCanvasWindowId();
  cTraceAll = new TCanvas("cTraceAll", 10, 10, wfRootEmbeddedCanvasTraceAll);
  cTraceAll->Divide(4,4);
  fRootEmbeddedCanvasTraceAll->AdoptCanvas(cTraceAll);
  fCompositeFrame3017->AddFrame(fRootEmbeddedCanvasTraceAll, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));


  // container of "Trace -> Sum"
  TGCompositeFrame *fCompositeFrame3019;
  fCompositeFrame3019 = fTabTrace->AddTab("Sum");
  fCompositeFrame3019->SetLayoutManager(new TGVerticalLayout(fCompositeFrame3019));

  // embedded canvas
  TRootEmbeddedCanvas *fRootEmbeddedCanvasTraceSum = new TRootEmbeddedCanvas(0,fCompositeFrame3019,719,481);
  Int_t wfRootEmbeddedCanvasTraceSum = fRootEmbeddedCanvasTraceSum->GetCanvasWindowId();
  cTraceSum = new TCanvas("cTraceSum", 10, 10, wfRootEmbeddedCanvasTraceSum);
  cTraceSum->Divide(1,2);
  fRootEmbeddedCanvasTraceSum->AdoptCanvas(cTraceSum);
  fCompositeFrame3019->AddFrame(fRootEmbeddedCanvasTraceSum, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));


  // container of "Trace -> One"
  TGCompositeFrame *fCompositeFrame3096;
  fCompositeFrame3096 = fTabTrace->AddTab("One");
  fCompositeFrame3096->SetLayoutManager(new TGVerticalLayout(fCompositeFrame3096));

  // embedded canvas
  TRootEmbeddedCanvas *fRootEmbeddedCanvasTraceOne = new TRootEmbeddedCanvas(0,fCompositeFrame3096,719,481);
  Int_t wfRootEmbeddedCanvasTraceOne = fRootEmbeddedCanvasTraceOne->GetCanvasWindowId();
  TCanvas *c125 = new TCanvas("c125", 10, 10, wfRootEmbeddedCanvasTraceOne);
  fRootEmbeddedCanvasTraceOne->AdoptCanvas(c125);
  fCompositeFrame3096->AddFrame(fRootEmbeddedCanvasTraceOne, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));


  fTabTrace->SetTab(0);

  fTabTrace->Resize(fTabTrace->GetDefaultSize());
  fCompositeFrame2692->AddFrame(fTabTrace, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));


  // container of "Plots"
  TGCompositeFrame *fCompositeFrame2694;
  fCompositeFrame2694 = fTabMain->AddTab("Plots");
  fCompositeFrame2694->SetLayoutManager(new TGVerticalLayout(fCompositeFrame2694));

  // tab widget
  TGTab *fTabPlots = new TGTab(fCompositeFrame2694,727,510);

  // container of "PeakTime"
  TGCompositeFrame *fCompositeFrame2942;
  fCompositeFrame2942 = fTabPlots->AddTab("PeakTime");
  fCompositeFrame2942->SetLayoutManager(new TGVerticalLayout(fCompositeFrame2942));

  // embedded canvas
  TRootEmbeddedCanvas *fRootEmbeddedCanvasPlotsPeakTime = new TRootEmbeddedCanvas(0,fCompositeFrame2942,719,481);
  Int_t wfRootEmbeddedCanvasPlotsPeakTime = fRootEmbeddedCanvasPlotsPeakTime->GetCanvasWindowId();
  cPlotsPeakTime = new TCanvas("cPlotsPeakTime", 10, 10, wfRootEmbeddedCanvasPlotsPeakTime);
  cPlotsPeakTime->Divide(4,4);
  fRootEmbeddedCanvasPlotsPeakTime->AdoptCanvas(cPlotsPeakTime);
  fCompositeFrame2942->AddFrame(fRootEmbeddedCanvasPlotsPeakTime, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));


  // container of "CRvsIntegral"
  TGCompositeFrame *fCompositeFrame2944;
  fCompositeFrame2944 = fTabPlots->AddTab("CRvsIntegral");
  fCompositeFrame2944->SetLayoutManager(new TGVerticalLayout(fCompositeFrame2944));

  // embedded canvas
  TRootEmbeddedCanvas *fRootEmbeddedCanvasPlotsCRvsIntegral = new TRootEmbeddedCanvas(0,fCompositeFrame2944,719,481);
  Int_t wfRootEmbeddedCanvasPlotsCRvsIntegral = fRootEmbeddedCanvasPlotsCRvsIntegral->GetCanvasWindowId();
  cPlotsCRvsIntegral = new TCanvas("cPlotsCRvsIntegral", 10, 10, wfRootEmbeddedCanvasPlotsCRvsIntegral);
  cPlotsCRvsIntegral->Divide(4,4);
  fRootEmbeddedCanvasPlotsCRvsIntegral->AdoptCanvas(cPlotsCRvsIntegral);
  fCompositeFrame2944->AddFrame(fRootEmbeddedCanvasPlotsCRvsIntegral, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

  // container of "Pedestal"
  TGCompositeFrame *fCompositeFrame2945;
  fCompositeFrame2945 = fTabPlots->AddTab("Pedestal");
  fCompositeFrame2945->SetLayoutManager(new TGVerticalLayout(fCompositeFrame2945));

  // embedded canvas
  TRootEmbeddedCanvas *fRootEmbeddedCanvasPlotsPedestal = new TRootEmbeddedCanvas(0,fCompositeFrame2945,719,481);
  Int_t wfRootEmbeddedCanvasPlotsPedestal = fRootEmbeddedCanvasPlotsPedestal->GetCanvasWindowId();
  cPlotsPedestal = new TCanvas("cPlotsPedestal", 10, 10, wfRootEmbeddedCanvasPlotsPedestal);
  cPlotsPedestal->Divide(4,4);
  fRootEmbeddedCanvasPlotsPedestal->AdoptCanvas(cPlotsPedestal);
  fCompositeFrame2945->AddFrame(fRootEmbeddedCanvasPlotsPedestal, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));


  fTabPlots->SetTab(0);

  fTabPlots->Resize(fTabPlots->GetDefaultSize());
  fCompositeFrame2694->AddFrame(fTabPlots, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));


  fTabMain->SetTab(0);

  fTabMain->Resize(fTabMain->GetDefaultSize());
  fHorizontalFrame2409->AddFrame(fTabMain, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

  AddFrame(fHorizontalFrame2409, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

  SetMWMHints(kMWMDecorAll,
	      kMWMFuncAll,
	      kMWMInputModeless);
  MapSubwindows();

  Resize(this->GetDefaultSize());
  MapWindow();
  Resize(941,569);

  Initialize();
  LoadEvent();
}  


void ArDisplay::Initialize()
{
  graph = new TClonesArray("TGraph",20);
  frame = new TClonesArray("TH2F",20);
  hPeakTime = new TClonesArray("TH1F",20);
  hCRvsIntegral = new TClonesArray("TH2F",20);
  hPedestal = new TClonesArray("TH1F",20);
  Time = new TArrayD(fRun->NumberOfSamples());
  for (int i=0;i<Time->GetSize();i++) Time->SetAt((Double_t)i,i);
  margin = 1.2;
  ped_line = new TClonesArray("TLine",20);
  pedtime_line = new TClonesArray("TLine",20);
  peaktime_line = new TClonesArray("TLine",20);
  peakstarttime_line = new TClonesArray("TArrow",20);
  analysis_text = new TClonesArray("TText",20);
}


ArDisplay::~ArDisplay()
{
  Cleanup();
}

void ArDisplay::AnalyseEvent()
{
  fAnalysis->ConvertData();
  fAnalysis->FindPedestal();
  fAnalysis->CorrectData();
  if (ArConf->FakeSignalTime<0) 
    fAnalysis->FindSignal();
  else fAnalysis->SetFakeSignal(ArConf->FakeSignalTime);
  fAnalysis->EvaluateIntegral();
  
  fAnalysis->FillVirtualChannel();
  fAnalysis->AnalyseVirtualChannel();
  if (ArConf->EvaluateChannelsFromVirtualChannel) {
    fAnalysis->EvaluateChannelsFromVirtualChannel();
    fAnalysis->AnalyseVirtualChannel();
  }

  fAnalysis->Print();
}

void ArDisplay::CleanGraphs()
{
  graph->Delete();
  frame->Delete();
  hPeakTime->Delete();
  hCRvsIntegral->Delete();
  hPedestal->Delete();
  ped_line->Delete();
  pedtime_line->Delete();
  peaktime_line->Delete();
  peakstarttime_line->Delete();
  analysis_text->Delete();
}

void ArDisplay::Process()
{
  AnalyseEvent();
  DisplayEvent();
}

void ArDisplay::LoadEvent()
{
  CleanGraphs();
  EventNumber = (Long64_t)fEventNumber->GetNumberEntry()->GetIntNumber();
  if (fRun->LoadEvent(EventNumber)<0) return;
  Process();
}

void ArDisplay::NextEvent()
{
  CleanGraphs();
  EventNumber++;
  if (fRun->LoadEvent(EventNumber)<0) return;
  Process();
}

void ArDisplay::PreviousEvent()
{
  CleanGraphs();
  if (EventNumber>0) EventNumber--;
  if (fRun->LoadEvent(EventNumber)<0) return;
  Process();
}

void ArDisplay::RedisplayEvent()
{
  CleanGraphs();
  DisplayEvent();
}

void ArDisplay::DisplayEvent()
{
  char text[20];
  sprintf(text,"Event %lld",EventNumber);
  fStatusBar->SetText(text,0);

  DisplayTraceAll();
  DisplayTraceSum();
  DisplayPlotsPeakTime();
  //  DisplayPlotsCRvsIntegral();
  DisplayPlotsPedestal();
}


void ArDisplay::DisplayTraceAll()
{
  if (!fRun) return;

  for (Int_t id=0;id<fRun->NPMTs();id++) {
    ArPMT *d = fRun->GetPMT(id);
    cTraceAll->cd(id+1);
    char name[10];
    sprintf(name,"PMT%d",d->ID());
    TH2F *f;
    if (fTextButtonAutomaticView->GetState() != kButtonUp) 
      f = new((*frame)[id]) TH2F(name,name,d->Data.GetSize(),0,d->Data.GetSize(),1000,(d->PedMean()-d->PedSigma()*3)*margin,d->PeakValue()*margin);
    else
      f = new((*frame)[id]) TH2F(name,name,d->Data.GetSize(),0,d->Data.GetSize(),1000,-0.01,2);
    f->SetStats(0);
    f->Draw();
    TGraph *g = new((*graph)[id]) TGraph(d->Data.GetSize(),Time->fArray,d->Data.fArray);
    g->Draw("PL");
    if (id==14) {
      g->SetLineColor(2);
      g->SetMarkerColor(2);
    }

    TLine *l;
    l = new((*ped_line)[id]) TLine(0,0,Time->GetSize(),0);
    l->SetLineStyle(2);
    l->SetLineColor(3);
    l->Draw();
    l = new((*peaktime_line)[id]) TLine(d->PeakTime(),0,d->PeakTime(),d->PeakValue()*1.1);
    l->SetLineStyle(3);
    l->SetLineColor(4);
    l->Draw();
    l = new((*peakstarttime_line)[id]) TArrow(d->PeakStartTime(),d->PeakValue()*1.1,d->PeakTime(),d->PeakValue()*1.1,0.005,"|>");
    l->SetLineStyle(1);
    l->SetLineColor(4);
    l->Draw();
    l = new((*pedtime_line)[id]) TLine(d->PedTimeMin(),0,d->PedTimeMax(),0);
    l->SetLineStyle(1);
    l->SetLineColor(6);
    l->SetLineWidth(2);
    l->Draw();
    TText *t;
    char text[100];
    sprintf(text,"IPe = %.2f Vs,CR = %.2f,  Npe = %d",d->IntegralPe(),d->CR(),d->Npe());
    t = new((*analysis_text)[id]) TText(0.15,0.91,text);
    t->SetNDC(kTRUE);
    t->Draw();

    if(d->NPeaks()) {
      for(int i=0; i<d->NPeaks(); i++) {
    	int time = d->Peaks.At(i);
    	TMarker *m = new TMarker((double)time,d->Data.At(time),29);
    	m->SetMarkerColor(kRed);
    	m->SetMarkerSize(1.0);
    	m->Draw("same");
      }
    }

  }

  cTraceAll->cd(0);
  cTraceAll->Update();

  printf("Trace->All drawn\n");
}


void ArDisplay::DisplayTraceSum()
{
  if (!fRun) return;

  Int_t vid = fRun->NPMTs();
  ArPMT *d = fRun->GetVPMT();

  cTraceSum->cd(1);

  TH2F *f;
  if (fTextButtonAutomaticView->GetState() != kButtonUp)
    f = new((*frame)[vid]) TH2F("AllPMTs","All PMTs",d->Data.GetSize(),0,d->Data.GetSize(),1000,(d->PedMean()-d->PedSigma()*3)*margin,d->PeakValue()*margin);
  else
    f = new((*frame)[vid]) TH2F("AllPMTs","All PMTs",d->Data.GetSize(),0,d->Data.GetSize(),1000,-0.01,2);
  f->SetStats(0);
  f->Draw();

  for (Int_t id=0;id<fRun->NPMTs();id++)
    graph->At(id)->Draw("PL");

  TLine *l;
  l = new((*ped_line)[vid]) TLine(0,0,Time->GetSize(),0);
  l->SetLineStyle(2);
  l->SetLineColor(3);
  l->Draw();
  l = new((*peaktime_line)[vid]) TLine(d->PeakTime(),0,d->PeakTime(),d->PeakValue()*1.1);
  l->SetLineStyle(3);
  l->SetLineColor(4);
  l->Draw();
  l = new((*peakstarttime_line)[vid]) TArrow(d->PeakStartTime(),d->PeakValue()*1.1,d->PeakTime(),d->PeakValue()*1.1,0.005,"|>");
  l->SetLineStyle(1);
  l->SetLineColor(4);
  l->Draw();
  l = new((*pedtime_line)[vid]) TLine(d->PedTimeMin(),0,d->PedTimeMax(),0);
  l->SetLineStyle(1);
  l->SetLineColor(6);
  l->SetLineWidth(2);
  l->Draw();

  cTraceSum->cd(2);

  vid++;

  if (fTextButtonAutomaticView->GetState() != kButtonUp)
    f = new((*frame)[vid]) TH2F("VPMT","VPMT",d->Data.GetSize(),0,d->Data.GetSize(),1000,(d->PedMean()-d->PedSigma()*3)*margin,d->PeakValue()*margin);
  else
    f = new((*frame)[vid]) TH2F("VPMT","VPMT",d->Data.GetSize(),0,d->Data.GetSize(),1000,-0.01,2);
  f->SetStats(0);
  f->Draw();
  TGraph *g = new((*graph)[vid]) TGraph(d->Data.GetSize(),Time->fArray,d->Data.fArray);
  g->Draw("PL");

  l = new((*ped_line)[vid]) TLine(0,0,Time->GetSize(),0);
  l->SetLineStyle(2);
  l->SetLineColor(3);
  l->Draw();
  l = new((*peaktime_line)[vid]) TLine(d->PeakTime(),0,d->PeakTime(),d->PeakValue()*1.1);
  l->SetLineStyle(3);
  l->SetLineColor(4);
  l->Draw();
  l = new((*peakstarttime_line)[vid]) TArrow(d->PeakStartTime(),d->PeakValue()*1.1,d->PeakTime(),d->PeakValue()*1.1,0.005,"|>");
  l->SetLineStyle(1);
  l->SetLineColor(4);
  l->Draw();
  l = new((*pedtime_line)[vid]) TLine(d->PedTimeMin(),0,d->PedTimeMax(),0);
  l->SetLineStyle(1);
  l->SetLineColor(6);
  l->SetLineWidth(2);
  l->Draw();
  TText *t;
  char text[100];
  sprintf(text,"IntegralPe = %.2f Vs, CR = %.2f,  Npe = %d",d->IntegralPe(),d->CR(),d->Npe());
  t = new((*analysis_text)[vid]) TText(0.15,0.91,text);
  t->SetNDC(kTRUE);
  t->Draw();
  if(d->NPeaks()) {
    for(int i=0; i<d->NPeaks(); i++) {
      int time = d->Peaks.At(i);
      TMarker *m = new TMarker((double)time,d->Data.At(time),29);
      m->SetMarkerColor(kRed);
      m->SetMarkerSize(1.0);
      m->Draw("same");
    }
  }

  cTraceSum->Update();

  printf("Trace->Sum drawn\n");
}


void ArDisplay::DisplayPlotsPeakTime()
{
  if (!fRun) return;

  for (Int_t id=0;id<fRun->NPMTs();id++) {
    ArPMT *d = fRun->GetPMT(id);
    cPlotsPeakTime->cd(id+1);
    char name[20];
    sprintf(name,"PeakTime_PMT%d",d->ID());
    TH1F *h = new((*hPeakTime)[id]) TH1F(name,name,Time->GetSize(),0,Time->GetSize());
    h->Fill(d->PeakTime());
    h->Draw();
  }

  cPlotsPeakTime->cd(0);
  cPlotsPeakTime->Update();

  printf("Plots->PeakTime drawn\n");
}


void ArDisplay::DisplayPlotsCRvsIntegral()
{
  if (!fRun) return;

  for (Int_t id=0;id<fRun->NPMTs();id++) {
    ArPMT *d = fRun->GetPMT(id);
    cPlotsCRvsIntegral->cd(id+1);
    char name[30];
    sprintf(name,"CRvsIntegral_PMT%d",d->ID());
    TH2F *h = new((*hCRvsIntegral)[id]) TH2F(name,name,100,0,500,100,0,2);
    h->SetStats(0);
    h->Fill(d->IntegralPe(),d->CR());
    h->Draw("colz");
  }

  cPlotsCRvsIntegral->cd(0);
  cPlotsCRvsIntegral->Update();

  printf("Plots->CRvsIntegral drawn\n");
}

void ArDisplay::DisplayPlotsPedestal()
{
  if (!fRun) return;

  for (Int_t id=0;id<fRun->NPMTs();id++) {
    ArPMT *d = fRun->GetPMT(id);
    cPlotsPedestal->cd(id+1);
    char name[30];
    sprintf(name,"Pedestal_PMT%d",d->ID());

    double range = TMath::MaxElement(d->Data.GetSize(), d->Data.GetArray());
    TH1F *h = new((*hPedestal)[id]) TH1F(name,name,100,-range,range);
    
    double pedTimeMin = d->PedTimeMin();
    double pedTimeMax = d->PedTimeMax();
    for(int i=0; i<d->Data.GetSize(); i++) {
      if(i <  pedTimeMin) continue;
      if(i >= pedTimeMax) continue;
      h->Fill(d->Data[i]);
    }

    h->Draw("");
    h->SetMinimum(1);
    h->SetMaximum(1000);
    gPad->Update();

    TPaveStats *st = (TPaveStats*)h->FindObject("stats");
    st->SetOptStat(1111111);
    st->SetX1NDC(0.6);
    st->SetY1NDC(0.2);

    gPad->SetLogy(1);
    gPad->Update();
  }

  cPlotsPedestal->cd(0);
  cPlotsPedestal->Update();

  printf("Plots->Pedestal drawn\n");
}
