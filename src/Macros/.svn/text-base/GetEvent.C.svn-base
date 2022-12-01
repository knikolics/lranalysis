#define MAXPMTS 14

Int_t load = gSystem->Load("../lib/ArDM.so");

ArRecoPMT *pmt;

void GetEvent(Int_t event, Int_t pmt_number)
{
  if (event<0) return;
  if (pmt_number<0||pmt_number>MAXPMTS) return;

  TClonesArray *pmts = new TClonesArray("ArRecoPMT",MAXPMTS);
  Data->SetBranchAddress("PMT",&pmts);
  Data->GetEntry(event);
  pmt = (ArRecoPMT *)pmts->At(pmt_number);
  printf("pmt is now filled\n");
}

