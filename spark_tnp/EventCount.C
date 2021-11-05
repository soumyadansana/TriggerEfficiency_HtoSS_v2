#include "TFile.h"
#include "TH1.h"
#include "TLegend.h"
#include "TString.h"
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TStyle.h"

const char *file_loc[7]={"/eos/user/s/sdansana/TnP_ntuples/muon/JPsi/Run2017_UL/MINIAOD/JpsiToMuMu_JpsiPt8_TuneCP5_13TeV-pythia8/crab_TnP_ntuplizer_muon_JPsi_Run2017_UL_MINIAOD_Jpsi/210523_221530/0000/output_%d.root","/eos/user/s/sdansana/TnP_ntuples/muon/JPsi/Run2017_UL/MINIAOD/Charmonium/crab_TnP_ntuplizer_muon_JPsi_Run2017_UL_MINIAOD_Run2017B/210524_192759/0000/output_%d.root","/eos/user/s/sdansana/TnP_ntuples/muon/JPsi/Run2017_UL/MINIAOD/Charmonium/crab_TnP_ntuplizer_muon_JPsi_Run2017_UL_MINIAOD_Run2017C/210524_192759/0000/output_%d.root","/eos/user/s/sdansana/TnP_ntuples/muon/JPsi/Run2017_UL/MINIAOD/Charmonium/crab_TnP_ntuplizer_muon_JPsi_Run2017_UL_MINIAOD_Run2017D/210524_192759/0000/output_%d.root","/eos/user/s/sdansana/TnP_ntuples/muon/JPsi/Run2017_UL/MINIAOD/Charmonium/crab_TnP_ntuplizer_muon_JPsi_Run2017_UL_MINIAOD_Run2017E/210531_000746/0000/output_%d.root","/eos/user/s/sdansana/TnP_ntuples/muon/JPsi/Run2017_UL/MINIAOD/Charmonium/crab_TnP_ntuplizer_muon_JPsi_Run2017_UL_MINIAOD_Run2017F/210531_040519/0000/output_%d.root","./output_%d.root"};
const char *era[7]={"MC","B","C","D","E","F","test"};
const int n1_ini[7]={1,1,1,1,1,1,1};
const int n2_fin[7]={490,258,519,271,429,586,100};

//const char *cent[3]={"HT100-200","HT200-400","HT400-600"};
const float cs[1]={874800.0};// pb unit
const float L_int[3]={139.762413829};//2017_UL HLT_Mu7p5_Track2_Jpsi_v in pb-1
//const int ptr = 1;

void EventCount(int ptr=0,int n1=1, int n2=600) {

  TH1F* h_tag_pt = new TH1F("h_tag_pt", "tag_pt", 500, 0.0, 500.0);
  TH1F* prescale = new TH1F("prescale", "prescale", 500, 0.0, 500.0);
  double h_wt;
  int flag; // to keep track of which trigger is fired
  double nevts;
  
  //do {
  nevts=0;
  if (n1<=n1_ini[ptr]) n1=n1_ini[ptr];
  if (n2>=n2_fin[ptr]) n2=n2_fin[ptr];
  for (int k=n1;k<=n2;k++) {
    
    
    if (k%1==0)
      cout<<"File number - "<<k<<endl;
    
    if (gSystem->AccessPathName(Form(file_loc[ptr],k))) {
      cout<<"Skipping file no. "<<k<<endl;
      continue;
    }

    TFile* f_in = TFile::Open(Form(file_loc[ptr],k),"Read");
    if (!f_in->IsOpen()) {
      cout<<"File not open"<<endl;
      continue;
    }
    // ---- efficiency vs pt
    
    TNtuple* data = (TNtuple*)f_in->Get("muon/Events");
    
    Int_t nentries=(Int_t)data->GetEntries();
    nevts+=nentries;
    //nevts+=nentries;
    
    f_in->Close();
    //write below
    //if (i==0) TFile *rootfile = new TFile("all_plots.root","Recreate");
    //else TFile *rootfile = new TFile("all_plots.root","update");
    
    //rootfile->Close();
  }
  ptr++;
  //}
  //while((ptr>1)&&(ptr<6));
  cout<<"Total Events-"<<nevts<<endl;
  flag=0;
  for (float i=0.5;i<500;i++) {
    //if (i>20) flag=2;
    //else if (i>17) flag=1;
    //else flag=0;
    h_wt=(cs[0]*L_int[flag])/nevts;
    //h_tag_pt->Fill(i,h_wt);
    prescale->Fill(i,h_wt);
    //cout<<i<<endl;
  }
  TFile* out_file = new TFile("./prescale/mc/Run2017_UL.root","Recreate");
  //h_tag_pt->Write();
  prescale->Write();
  out_file->Close();
  return;
}
