#include "TFile.h"
#include "TH1.h"
#include "TLegend.h"
#include "TString.h"
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TStyle.h"

const char *file_loc[7]={"./MC/0000/output_%d.root","/eos/user/s/sdansana/TnP_ntuples/muon/JPsi/Run2017_UL/MINIAOD/SingleMuon/crab_TnP_ntuplizer_muon_JPsi_Run2017_UL_MINIAOD_Run2017B/210524_192759/0000/output_%d.root","/eos/user/s/sdansana/TnP_ntuples/muon/JPsi/Run2017_UL/MINIAOD/SingleMuon/crab_TnP_ntuplizer_muon_JPsi_Run2017_UL_MINIAOD_Run2017C/210918_213334/0000/output_%d.root","/eos/user/s/sdansana/TnP_ntuples/muon/JPsi/Run2017_UL/MINIAOD/SingleMuon/crab_TnP_ntuplizer_muon_JPsi_Run2017_UL_MINIAOD_Run2017D/210918_213657/0000/output_%d.root","/eos/user/s/sdansana/TnP_ntuples/muon/JPsi/Run2017_UL/MINIAOD/SingleMuon/crab_TnP_ntuplizer_muon_JPsi_Run2017_UL_MINIAOD_Run2017E/210918_213744/0000/output_%d.root","/eos/user/s/sdansana/TnP_ntuples/muon/JPsi/Run2017_UL/MINIAOD/SingleMuon/crab_TnP_ntuplizer_muon_JPsi_Run2017_UL_MINIAOD_Run2017F/210918_213831/0000/output_%d.root","./output_%d.root"};
const char *era[7]={"MC","B","C","D","E","F","test"};
const int n1_ini[7]={1,1,1,1,1,1,1};
const int n2_fin[7]={490,244,519,271,439,585,100};
//const int ptr = 1;

void all_in_one_v4_singlemuon(int ptr=1,int n1=1, int n2=600) {

  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  //gStyle->SetOptStat(0);
  TEfficiency *h_pt_eff[100];
  TEfficiency *h_eta_eff[100];
  TEfficiency *h_phi_eff[100];
  char grfName_pt[100];
  char grfName_eta[100];
  char grfName_phi[100];
  //TFile *f_in;
  //TH1::SetDefaultSumw2();
  
  float pair_pt;
  float pair_mass;
  float pair_eta;
  float pair_phi;
  float pair_dR;
  float pair_dz;
  
  float tag_pt;
  float tag_eta;
  float tag_phi;
  int tag_charge;
  float probe_pt;
  float probe_eta;
  float probe_phi;
  int probe_charge;
  float sum_tag, sum_probe;
  float tag_pfIso;
  float probe_pfIso;  
  float tag_pfIso04_charged;
  float tag_pfIso04_neutral;
  float tag_pfIso04_photon;
  float tag_pfIso04_sumPU;
  float probe_pfIso04_charged;
  float probe_pfIso04_neutral;
  float probe_pfIso04_photon;
  float probe_pfIso04_sumPU;
  float pv_x;
  float pv_y;
  float pv_z;
  float genmu1_pt;
  float genmu2_pt;
  int pair_rank;
  float tag_relIso04;
  float probe_relIso04;
  float tag_relIso04_new;//defined by subtracting pT of other leg
  float probe_relIso04_new;
  float tag_dz;
  float probe_dz;
  
  bool HLT_IsoMu27_v;
  bool HLT_Mu8_v;
  bool HLT_Mu17_v;
  bool HLT_Mu20_v;
  bool tag_HLT_Mu8_v;
  bool tag_HLT_Mu17_v;
  bool tag_HLT_Mu20_v;
  bool probe_HLT_Mu8_v;
  bool probe_HLT_Mu17_v;
  bool probe_HLT_Mu20_v;
  bool tag_HLT_IsoMu27_v;
  bool probe_HLT_IsoMu27_v;
  bool tag_isTight;
  bool probe_isTight;
  bool probe_isMedium;
  bool probe_isLoose;
  bool probe_isMuMatched;
  
  
  TH1F* h_HLT_singlemu_trig = new TH1F("h_HLT_singlemu_trig", "HLT_singlemu_trig", 2, -0.5, 1.5);
  TH1F* h_HLT_IsoMu27_v = new TH1F("h_HLT_IsoMu27_v", "HLT_IsoMu27_v",2, -0.5, 1.5);
  TH1F* h_probe_isMuMatched = new TH1F("h_probe_isMuMatched", "probe_isMuMatched", 2, -0.5, 1.5);
  TH1F* h_probe_isMuMatched_Pass = new TH1F("h_probe_isMuMatched_Pass", "probe_isMuMatched_Pass", 2, -0.5, 1.5);
  TH1F* h_pv_z = new TH1F("h_pv_z", "pv_z", 300, -15, 15);
  TH1F* h_pv_z_Pass = new TH1F("h_pv_z_Pass", "pv_z_Pass", 300, -15, 15);
  TH1F* h_pv_x = new TH1F("h_pv_x", "pv_x", 200, -0.1, 0.1);
  TH1F* h_pv_y = new TH1F("h_pv_y", "pv_y", 200, -0.1, 0.1);
  
    
  //TH1F* h_tag_HLT_singlemu_trig = new TH1F("h_tag_HLT_singlemu_trig", "tag_HLT_singlemu_trig", 2, -0.5, 1.5);
  TH1F* h_tag_HLT_IsoMu27_v = new TH1F("h_tag_HLT_IsoMu27_v", "tag_HLT_IsoMu27_v", 2, -0.5, 1.5);
  TH1F* h_probe_HLT_singlemu_trig = new TH1F("h_probe_HLT_singlemu_trig", "probe_HLT_singlemu_trig", 2, -0.5, 1.5);
  TH1F* h_probe_HLT_IsoMu27_v = new TH1F("h_probe_HLT_IsoMu27_v", "probe_HLT_IsoMu27_v", 2, -0.5, 1.5);
  TH1F* h_tag_isTight = new TH1F("h_tag_isTight", "tag_isTight", 2, -0.5, 1.5);
  TH1F* h_probe_isTight = new TH1F("h_probe_isTight", "probe_isTight", 2, -0.5, 1.5);
  TH1F* h_probe_isMedium = new TH1F("h_probe_isMedium", "probe_isMedium", 2, -0.5, 1.5);
  TH1F* h_probe_isLoose = new TH1F("h_probe_isLoose", "probe_isLoose", 2, -0.5, 1.5);
  TH1F* h_tag_charge = new TH1F("h_tag_charge", "tag_charge", 3, -1.5, 1.5);
  TH1F* h_probe_charge = new TH1F("h_probe_charge", "probe_charge", 3, -1.5, 1.5);
  
  TH1F* h_tag_pt = new TH1F("h_tag_pt", "tag_pt", 500, 0.0, 500.0);
  TH1F* h_tag_eta = new TH1F("h_tag_eta", "tag_eta", 100, -3.0, 3.0);
  TH1F* h_tag_pt_Pass = new TH1F("h_tag_pt_Pass", "tag_pt_Pass", 500, 0.0, 500.0);
  TH1F* h_tag_eta_Pass = new TH1F("h_tag_eta_Pass", "tag_eta_Pass", 100, -3.0, 3.0);
  TH1F* h_tag_phi = new TH1F("h_tag_phi", "tag_phi", 100, -3.14, 3.14);
  TH1F* h_probe_pt = new TH1F("h_probe_pt", "probe_pt", 500, 0.0, 500.0);
  TH1F* h_probe_eta = new TH1F("h_probe_eta", "probe_eta", 100, -3.0, 3.0);
  TH1F* h_probe_pt_Pass = new TH1F("h_probe_pt_Pass", "probe_pt_Pass", 500, 0.0, 500.0);
  TH1F* h_probe_eta_Pass = new TH1F("h_probe_eta_Pass", "probe_eta_Pass", 100, -3.0, 3.0);
  TH1F* h_probe_phi = new TH1F("h_probe_phi", "probe_phi", 100, -3.14, 3.14);
  
  TH1F* h_genmu1_pt = new TH1F("h_genmu1_pt", "genmu1_pt", 500, 0.0, 500.0);
  TH1F* h_genmu2_pt = new TH1F("h_genmu2_pt", "genmu2_pt", 500, 0.0, 500.0);
  
  TH1F* h_pair_pt = new TH1F("h_pair_pt", "pair_pt", 500, 0.0, 500.0);
  TH1F* h_pair_eta = new TH1F("h_pair_eta", "pair_eta", 100, -3.0, 3.0);
  TH1F* h_pair_phi = new TH1F("h_pair_phi", "pair_phi", 100, -3.14, 3.14);
  TH1F* h_pair_mass = new TH1F("h_pair_mass", "pair_mass", 120, 2., 4.);
  TH1F* h_pair_mass_Pass = new TH1F("h_pair_mass_Pass", "pair_mass_Pass", 120, 2., 4.);
  TH1F* h_pair_dR = new TH1F("h_pair_dR", "pair_dR", 100, 0., 1.);
  TH1F* h_pair_dR_Pass = new TH1F("h_pair_dR_Pass", "pair_dR_Pass", 100, 0., 1.);
  TH1F* h_pair_absdz = new TH1F("h_pair_absdz", "pair_absdz", 400, 0., 2.0);
  TH1F* h_pair_absdz_Pass = new TH1F("h_pair_absdz_Pass", "pair_absdz_Pass", 400, 0., 2.0);
  
  //double edges[8] = {0., 0.05, 0.10, 0.15, 0.20, 0.25, 0.40, 1.0};
  TH1F* h_tag_pfIso = new TH1F("h_tag_pfIso", "tag_pfIso", 100, 0., 1.);
  TH1F* h_probe_pfIso = new TH1F("h_probe_pfIso", "probe_pfIso", 100, 0.,1.);
  
  TH1F* h_tag_relIso04 = new TH1F("h_tag_relIso04", "tag_relIso04", 100, 0., 10.);
  TH1F* h_probe_relIso04 = new TH1F("h_probe_relIso04", "probe_relIso04", 100, 0., 10.);
  
  TH1F* h_tag_relIso04_Pass = new TH1F("h_tag_relIso04_Pass", "tag_relIso04_Pass", 100, 0., 10.);
  TH1F* h_probe_relIso04_Pass = new TH1F("h_probe_relIso04_Pass", "probe_relIso04_Pass", 100, 0., 10.);
  
  TH2F* h_tag_pt_pair_dR = new TH2F("h_tag_pt_pair_dR", "tag_pt_pair_dR", 500, 0.0, 500.0, 100, 0., 1.);
  TH2F* h_probe_pt_pair_dR = new TH2F("h_probe_pt_pair_dR", "probe_pt_pair_dR", 500, 0.0, 500.0, 100, 0., 1.);
  TH2F* h_tag_pt_probe_pt = new TH2F("h_tag_pt_probe_pt", "tag_pt_probe_pt", 500, 0.0, 500.0, 500, 0., 500.);
  TH2F* h_tag_pt_pair_dR_Pass = new TH2F("h_tag_pt_pair_dR_Pass", "tag_pt_pair_dR_Pass", 500, 0.0, 500.0, 100, 0., 1.);
  TH2F* h_probe_pt_pair_dR_Pass = new TH2F("h_probe_pt_pair_dR_Pass", "probe_pt_pair_dR_Pass", 500, 0.0, 500.0, 100, 0., 1.);
  TH2F* h_tag_pt_probe_pt_Pass = new TH2F("h_tag_pt_probe_pt_Pass", "tag_pt_probe_pt_Pass", 500, 0.0, 500.0, 500, 0., 500.);
  
  TH2F* h_tag_pt_tag_relIso04 = new TH2F("h_tag_pt_tag_relIso04", "tag_pt_tag_relIso04", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_tag_pt_probe_relIso04 = new TH2F("h_tag_pt_probe_relIso04", "tag_pt_probe_relIso04", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_probe_pt_tag_relIso04 = new TH2F("h_probe_pt_tag_relIso04", "probe_pt_tag_relIso04", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_probe_pt_probe_relIso04 = new TH2F("h_probe_pt_probe_relIso04", "probe_pt_probe_relIso04", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_tag_pt_tag_relIso04_Pass = new TH2F("h_tag_pt_tag_relIso04_Pass", "tag_pt_tag_relIso04_Pass", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_tag_pt_probe_relIso04_Pass = new TH2F("h_tag_pt_probe_relIso04_Pass", "tag_pt_probe_relIso04_Pass", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_probe_pt_tag_relIso04_Pass = new TH2F("h_probe_pt_tag_relIso04_Pass", "probe_pt_tag_relIso04_Pass", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_probe_pt_probe_relIso04_Pass = new TH2F("h_probe_pt_probe_relIso04_Pass", "probe_pt_probe_relIso04_Pass", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_dR_tag_relIso04 = new TH2F("h_dR_tag_relIso04", "dR_tag_relIso04", 100, 0.0, 1.0, 100, 0., 10.);
  TH2F* h_dR_probe_relIso04 = new TH2F("h_dR_probe_relIso04", "dR_probe_relIso04", 100, 0.0, 1.0, 100, 0., 10.);
  TH2F* h_dR_tag_relIso04_Pass = new TH2F("h_dR_tag_relIso04_Pass", "dR_tag_relIso04_Pass", 100, 0.0, 1.0, 100, 0., 10.);
  TH2F* h_dR_probe_relIso04_Pass = new TH2F("h_dR_probe_relIso04_Pass", "dR_probe_relIso04_Pass", 100, 0.0, 1.0, 100, 0., 10.);
  TH2F* h_probe_relIso04_tag_relIso04_Pass = new TH2F("h_probe_relIso04_tag_relIso04_Pass", "probe_relIso04_tag_relIso04_Pass", 100, 0.0, 10.0, 100, 0., 10.);
  TH2F* h_probe_relIso04_tag_relIso04 = new TH2F("h_probe_relIso04_tag_relIso04", "probe_relIso04_tag_relIso04", 100, 0.0, 10.0, 100, 0., 10.);
  
  TH1F* h_tag_relIso04_new = new TH1F("h_tag_relIso04_new", "tag_relIso04_new", 100, 0., 10.);
  TH1F* h_probe_relIso04_new = new TH1F("h_probe_relIso04_new", "probe_relIso04_new", 100, 0., 10.);
  
  TH1F* h_tag_relIso04_new_Pass = new TH1F("h_tag_relIso04_new_Pass", "tag_relIso04_new_Pass", 100, 0., 10.);
  TH1F* h_probe_relIso04_new_Pass = new TH1F("h_probe_relIso04_new_Pass", "probe_relIso04_new_Pass", 100, 0., 10.);
  
  TH2F* h_tag_pt_tag_relIso04_new = new TH2F("h_tag_pt_tag_relIso04_new", "tag_pt_tag_relIso04_new", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_tag_pt_probe_relIso04_new = new TH2F("h_tag_pt_probe_relIso04_new", "tag_pt_probe_relIso04_new", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_probe_pt_tag_relIso04_new = new TH2F("h_probe_pt_tag_relIso04_new", "probe_pt_tag_relIso04_new", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_probe_pt_probe_relIso04_new = new TH2F("h_probe_pt_probe_relIso04_new", "probe_pt_probe_relIso04_new", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_tag_pt_tag_relIso04_new_Pass = new TH2F("h_tag_pt_tag_relIso04_new_Pass", "tag_pt_tag_relIso04_new_Pass", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_tag_pt_probe_relIso04_new_Pass = new TH2F("h_tag_pt_probe_relIso04_new_Pass", "tag_pt_probe_relIso04_new_Pass", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_probe_pt_tag_relIso04_new_Pass = new TH2F("h_probe_pt_tag_relIso04_new_Pass", "probe_pt_tag_relIso04_new_Pass", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_probe_pt_probe_relIso04_new_Pass = new TH2F("h_probe_pt_probe_relIso04_new_Pass", "probe_pt_probe_relIso04_new_Pass", 500, 0.0, 500.0, 100, 0., 10.);
  TH2F* h_dR_tag_relIso04_new = new TH2F("h_dR_tag_relIso04_new", "dR_tag_relIso04_new", 100, 0.0, 1.0, 100, 0., 10.);
  TH2F* h_dR_probe_relIso04_new = new TH2F("h_dR_probe_relIso04_new", "dR_probe_relIso04_new", 100, 0.0, 1.0, 100, 0., 10.);
  TH2F* h_dR_tag_relIso04_new_Pass = new TH2F("h_dR_tag_relIso04_new_Pass", "dR_tag_relIso04_new_Pass", 100, 0.0, 1.0, 100, 0., 10.);
  TH2F* h_dR_probe_relIso04_new_Pass = new TH2F("h_dR_probe_relIso04_new_Pass", "dR_probe_relIso04_new_Pass", 100, 0.0, 1.0, 100, 0., 10.);
  TH2F* h_probe_relIso04_tag_relIso04_new_Pass = new TH2F("h_probe_relIso04_tag_relIso04_new_Pass", "probe_relIso04_tag_relIso04_new_Pass", 100, 0.0, 10.0, 100, 0., 10.);
  TH2F* h_probe_relIso04_tag_relIso04_new = new TH2F("h_probe_relIso04_tag_relIso04_new", "probe_relIso04_tag_relIso04_new", 100, 0.0, 10.0, 100, 0., 10.);
  
  //do {
  if (n1<=n1_ini[ptr]) n1=n1_ini[ptr];
  if (n2>=n2_fin[ptr]) n2=n2_fin[ptr];
  for (int k=n1;k<=n2;k++) {
  
    if (gSystem->AccessPathName(Form(file_loc[ptr],k))) {
      cout<<"Skipping file no. "<<k<<endl;
      continue;
    }

    TFile* f_in = TFile::Open(Form(file_loc[ptr],k),"Read");
    if (!f_in->IsOpen()) {
      cout<<"File not open"<<endl;
      continue;
    }
    if (k%1==0)
      cout<<"File number - "<<k<<endl;
    // ---- efficiency vs pt
    
    TNtuple* data = (TNtuple*)f_in->Get("muon/Events");
    
    data->SetBranchAddress("HLT_Mu8_v",&HLT_Mu8_v);
    data->SetBranchAddress("HLT_Mu17_v",&HLT_Mu17_v);
    data->SetBranchAddress("HLT_Mu20_v",&HLT_Mu20_v);
    data->SetBranchAddress("HLT_IsoMu27_v",&HLT_IsoMu27_v);
    
    data->SetBranchAddress("probe_isMuMatched",&probe_isMuMatched);
    
    
    data->SetBranchAddress("pv_x",&pv_x);
    data->SetBranchAddress("pv_y",&pv_y);
    data->SetBranchAddress("pv_z",&pv_z);
    data->SetBranchAddress("genmu1_pt",&genmu1_pt);
    data->SetBranchAddress("genmu2_pt",&genmu2_pt);
    
    data->SetBranchAddress("tag_pt",&tag_pt);
    data->SetBranchAddress("tag_eta",&tag_eta);
    data->SetBranchAddress("tag_phi",&tag_phi);
    data->SetBranchAddress("tag_charge",&tag_charge);    
    data->SetBranchAddress("tag_isTight",&tag_isTight);
    data->SetBranchAddress("probe_pt",&probe_pt);
    data->SetBranchAddress("probe_eta",&probe_eta);
    data->SetBranchAddress("probe_phi",&probe_phi);
    data->SetBranchAddress("probe_charge",&probe_charge);  
    data->SetBranchAddress("probe_isTight",&probe_isTight);
    data->SetBranchAddress("probe_isMedium",&probe_isMedium);
    data->SetBranchAddress("probe_isLoose",&probe_isLoose);
    data->SetBranchAddress("tag_HLT_Mu8_v",&tag_HLT_Mu8_v);
    data->SetBranchAddress("tag_HLT_Mu17_v",&tag_HLT_Mu17_v);
    data->SetBranchAddress("tag_HLT_Mu20_v",&tag_HLT_Mu20_v);
    data->SetBranchAddress("tag_HLT_IsoMu27_v",&tag_HLT_IsoMu27_v);
    data->SetBranchAddress("probe_HLT_IsoMu27_v",&probe_HLT_IsoMu27_v);
    data->SetBranchAddress("probe_HLT_Mu8_v",&probe_HLT_Mu8_v);
    data->SetBranchAddress("probe_HLT_Mu17_v",&probe_HLT_Mu17_v);
    data->SetBranchAddress("probe_HLT_Mu20_v",&probe_HLT_Mu20_v);
    
    data->SetBranchAddress("tag_pfIso04_charged",&tag_pfIso04_charged);
    data->SetBranchAddress("tag_pfIso04_neutral",&tag_pfIso04_neutral);
    data->SetBranchAddress("tag_pfIso04_photon",&tag_pfIso04_photon);
    data->SetBranchAddress("tag_pfIso04_sumPU",&tag_pfIso04_photon);
    data->SetBranchAddress("probe_pfIso04_charged",&probe_pfIso04_charged);
    data->SetBranchAddress("probe_pfIso04_neutral",&probe_pfIso04_neutral);
    data->SetBranchAddress("probe_pfIso04_photon",&probe_pfIso04_photon);
    data->SetBranchAddress("probe_pfIso04_sumPU",&probe_pfIso04_sumPU);
    
    data->SetBranchAddress("pair_mass",&pair_mass);
    data->SetBranchAddress("pair_pt",&pair_pt);
    data->SetBranchAddress("pair_eta",&pair_eta);
    data->SetBranchAddress("pair_phi",&pair_phi);
    data->SetBranchAddress("pair_dR",&pair_dR);
    data->SetBranchAddress("pair_dz",&pair_dz);
    data->SetBranchAddress("pair_rank",&pair_rank);
    
    data->SetBranchAddress("tag_relIso04",&tag_relIso04);
    data->SetBranchAddress("probe_relIso04",&probe_relIso04);
    
    data->SetBranchAddress("tag_dz",&tag_dz);    
    data->SetBranchAddress("probe_dz",&probe_dz);
    
    Int_t nentries=(Int_t)data->GetEntries();
    for (Int_t i=0;i<nentries;i++) {
		  data->GetEntry(i);
		  //h_HLT_Mu8_v->Fill(HLT_Mu8_v);
		  h_HLT_singlemu_trig->Fill((HLT_Mu8_v == 1) || (HLT_Mu17_v == 1) || (HLT_Mu20_v == 1));
		  // tag_charge > 0, probe_charge < 0
      if (tag_pt > 8 && abs(tag_eta) < 2.4 && tag_isTight == 1 && (tag_HLT_Mu8_v == 1 || tag_HLT_Mu17_v == 1 || tag_HLT_Mu20_v == 1) && abs(probe_eta) < 2.4 && probe_pt>2 && pair_dR >= 0.1 && abs(tag_dz)<0.5 && abs(probe_dz)<0.5)  {
      //if (((tag_HLT_Mu7p5_Track2_Jpsi_v == 1) && (probe_HLT_Mu7p5_Track2_Jpsi_v == 1)) && ((tag_charge>0)&&(probe_charge<0))) {
        //sum_tag = tag_pfIso04_neutral + tag_pfIso04_photon - 0.5*tag_pfIso04_sumPU + abs(tag_pfIso04_neutral + tag_pfIso04_photon - 0.5*tag_pfIso04_sumPU);
        //sum_probe = probe_pfIso04_neutral + probe_pfIso04_photon - 0.5*probe_pfIso04_sumPU + abs(probe_pfIso04_neutral + probe_pfIso04_photon - 0.5*probe_pfIso04_sumPU);
        tag_pfIso = (tag_pfIso04_charged + std::max(0., tag_pfIso04_neutral + tag_pfIso04_photon - 0.5*tag_pfIso04_sumPU))/tag_pt;
        probe_pfIso = (probe_pfIso04_charged + std::max(0., probe_pfIso04_neutral + probe_pfIso04_photon - 0.5*probe_pfIso04_sumPU))/probe_pt;
        
        tag_relIso04_new = tag_relIso04 - (probe_pt/tag_pt);
        if (tag_relIso04_new < 0) tag_relIso04=0;
        probe_relIso04_new = probe_relIso04 - (tag_pt/probe_pt);
        if (probe_relIso04_new < 0) probe_relIso04=0;
	h_HLT_IsoMu27_v->Fill(HLT_IsoMu27_v);
        h_tag_charge->Fill(tag_charge);
        h_probe_charge->Fill(probe_charge);
        //cout<<"probe_charge-"<<probe_charge<<endl;
        
        h_tag_HLT_IsoMu27_v->Fill(tag_HLT_IsoMu27_v);
        h_tag_isTight->Fill(tag_isTight);
        h_tag_pt->Fill(tag_pt);
        h_tag_eta->Fill(tag_eta);
        h_tag_phi->Fill(tag_phi);
        h_tag_pfIso->Fill(tag_pfIso);
        h_probe_HLT_IsoMu27_v->Fill(probe_HLT_IsoMu27_v);
        h_probe_isTight->Fill(probe_isTight);
        h_probe_isMedium->Fill(probe_isMedium);
        h_probe_isLoose->Fill(probe_isLoose);
        h_probe_pt->Fill(probe_pt);
        h_probe_eta->Fill(probe_eta);
        h_probe_phi->Fill(probe_phi);
        h_probe_pfIso->Fill(probe_pfIso);
        
        h_pair_mass->Fill(pair_mass);
        h_pair_pt->Fill(pair_pt);
        h_pair_eta->Fill(pair_eta);
        h_pair_phi->Fill(pair_phi);
        h_pair_dR->Fill(pair_dR);
        
        h_genmu1_pt->Fill(genmu1_pt);
        h_genmu2_pt->Fill(genmu2_pt);

        h_probe_isMuMatched->Fill(probe_isMuMatched);
      
        h_tag_pt_pair_dR->Fill(tag_pt, pair_dR);
        h_probe_pt_pair_dR->Fill(probe_pt, pair_dR);  
        h_tag_pt_probe_pt->Fill(tag_pt,probe_pt);    
        
        h_pv_z->Fill(pv_z);
        h_pv_x->Fill(pv_x);
        h_pv_y->Fill(pv_y);
        
        h_tag_relIso04->Fill(tag_relIso04);
        h_probe_relIso04->Fill(probe_relIso04);
        
        h_probe_HLT_singlemu_trig->Fill((probe_HLT_Mu8_v == 1) || (probe_HLT_Mu17_v == 1) || (probe_HLT_Mu20_v == 1));
        
        h_tag_pt_tag_relIso04->Fill(tag_pt, tag_relIso04);
        h_tag_pt_probe_relIso04->Fill(tag_pt, probe_relIso04);
        h_probe_pt_tag_relIso04->Fill(probe_pt, tag_relIso04);
        h_probe_pt_probe_relIso04->Fill(probe_pt, probe_relIso04);
        h_dR_tag_relIso04->Fill(pair_dR, tag_relIso04);
        h_dR_probe_relIso04->Fill(pair_dR, probe_relIso04);
        h_probe_relIso04_tag_relIso04->Fill(probe_relIso04,tag_relIso04);
        
        
        h_tag_relIso04_new->Fill(tag_relIso04_new);
        h_probe_relIso04_new->Fill(probe_relIso04_new);
        
        h_tag_pt_tag_relIso04_new->Fill(tag_pt, tag_relIso04_new);
        h_tag_pt_probe_relIso04_new->Fill(tag_pt, probe_relIso04_new);
        h_probe_pt_tag_relIso04_new->Fill(probe_pt, tag_relIso04_new);
        h_probe_pt_probe_relIso04_new->Fill(probe_pt, probe_relIso04_new);
        h_dR_tag_relIso04_new->Fill(pair_dR, tag_relIso04_new);
        h_dR_probe_relIso04_new->Fill(pair_dR, probe_relIso04_new);
        h_probe_relIso04_tag_relIso04_new->Fill(probe_relIso04_new,tag_relIso04_new);
        
        if (probe_HLT_IsoMu27_v == 1) {
          h_pair_dR_Pass->Fill(pair_dR);
          h_pv_z_Pass->Fill(pv_z);
          h_tag_pt_Pass->Fill(tag_pt);
          h_tag_eta_Pass->Fill(tag_eta);
          h_probe_pt_Pass->Fill(probe_pt);
          h_probe_eta_Pass->Fill(probe_eta);
          h_pair_mass_Pass->Fill(pair_mass);
          h_probe_isMuMatched_Pass->Fill(probe_isMuMatched);
          h_tag_pt_pair_dR_Pass->Fill(tag_pt, pair_dR);
          h_probe_pt_pair_dR_Pass->Fill(probe_pt, pair_dR);
          h_tag_pt_probe_pt_Pass->Fill(tag_pt,probe_pt);    
          
          h_probe_relIso04_Pass->Fill(probe_relIso04);
          h_tag_relIso04_Pass->Fill(tag_relIso04);
          
          h_tag_pt_tag_relIso04_Pass->Fill(tag_pt, tag_relIso04);
          h_tag_pt_probe_relIso04_Pass->Fill(tag_pt, probe_relIso04);
          h_probe_pt_tag_relIso04_Pass->Fill(probe_pt, tag_relIso04);
          h_probe_pt_probe_relIso04_Pass->Fill(probe_pt, probe_relIso04);
          h_dR_tag_relIso04_Pass->Fill(pair_dR, tag_relIso04);
          h_dR_probe_relIso04_Pass->Fill(pair_dR, probe_relIso04);
          h_probe_relIso04_tag_relIso04_Pass->Fill(probe_relIso04_new,tag_relIso04);
          
          h_probe_relIso04_new_Pass->Fill(probe_relIso04_new);
          h_tag_relIso04_new_Pass->Fill(tag_relIso04_new);
          
          h_tag_pt_tag_relIso04_new_Pass->Fill(tag_pt, tag_relIso04_new);
          h_tag_pt_probe_relIso04_new_Pass->Fill(tag_pt, probe_relIso04_new);
          h_probe_pt_tag_relIso04_new_Pass->Fill(probe_pt, tag_relIso04_new);
          h_probe_pt_probe_relIso04_new_Pass->Fill(probe_pt, probe_relIso04_new);
          h_dR_tag_relIso04_new_Pass->Fill(pair_dR, tag_relIso04_new);
          h_dR_probe_relIso04_new_Pass->Fill(pair_dR, probe_relIso04_new);
          h_probe_relIso04_tag_relIso04_new_Pass->Fill(probe_relIso04_new,tag_relIso04_new);
        
          
        }
        h_pair_absdz->Fill(abs(pair_dz));
      }
    }
    f_in->Close();
    //write below
    //if (i==0) TFile *rootfile = new TFile("all_plots.root","Recreate");
    //else TFile *rootfile = new TFile("all_plots.root","update");
    
    //rootfile->Close();
  }
  ptr++;
  //}
  //while((ptr>1)&&(ptr<6));
  
  
  //h_pt_denom1->Draw();
  TFile* out_file = new TFile(Form("hists_secondset_2017%s_UL_[%d-%d]_filter_matching_singlemuon.root",era[ptr-1],n1,n2),"Recreate");
	
  h_HLT_IsoMu27_v->Write();
  h_tag_HLT_IsoMu27_v->Write();
  h_probe_HLT_IsoMu27_v->Write();
  h_tag_isTight->Write();
  h_probe_isTight->Write();
  h_probe_isMedium->Write();
  h_probe_isLoose->Write();
  
  h_tag_pt->Write();
  h_tag_eta->Write();
  h_tag_phi->Write();
  h_probe_pt->Write();
  h_probe_eta->Write();
  h_probe_phi->Write();
  h_tag_pfIso->Write();
  h_probe_pfIso->Write();
  h_tag_charge->Write();
  h_probe_charge->Write();
       
  h_pair_mass->Write();
  h_pair_pt->Write();
  h_pair_eta->Write();
  h_pair_phi->Write();
  h_pair_dR->Write();
  h_pair_absdz->Write();
  
  h_pv_z->Write();
  h_pv_x->Write();
  h_pv_y->Write();
  h_genmu1_pt->Write();
  h_genmu2_pt->Write();
  
  h_pair_dR_Pass->Write();
  h_pv_z_Pass->Write();
  h_tag_pt_Pass->Write();
  h_tag_eta_Pass->Write();
  h_probe_pt_Pass->Write();
  h_probe_eta_Pass->Write();
  h_pair_mass_Pass->Write();
  h_probe_isMuMatched_Pass->Write();
  h_probe_isMuMatched->Write();
  
  h_tag_pt_pair_dR->Write();
  h_probe_pt_pair_dR->Write();    
  h_tag_pt_pair_dR_Pass->Write();
  h_probe_pt_pair_dR_Pass->Write();   
  h_tag_pt_probe_pt_Pass->Write(); 
  h_tag_pt_probe_pt->Write();
  
  h_tag_relIso04->Write();
  h_probe_relIso04->Write(); 
  
  h_tag_relIso04_Pass->Write();
  h_probe_relIso04_Pass->Write();
  
  h_HLT_singlemu_trig->Write();
  h_probe_HLT_singlemu_trig->Write();
  
  
  h_tag_pt_tag_relIso04_Pass->Write();
  h_tag_pt_probe_relIso04_Pass->Write();
  h_probe_pt_tag_relIso04_Pass->Write();
  h_probe_pt_probe_relIso04_Pass->Write();
  h_tag_pt_tag_relIso04->Write();
  h_tag_pt_probe_relIso04->Write();
  h_probe_pt_tag_relIso04->Write();
  h_probe_pt_probe_relIso04->Write();
  h_dR_probe_relIso04->Write();
  h_dR_tag_relIso04->Write();
  h_dR_probe_relIso04_Pass->Write();
  h_dR_tag_relIso04_Pass->Write();
  h_probe_relIso04_tag_relIso04->Write();
  h_probe_relIso04_tag_relIso04_Pass->Write();
  
  
  h_tag_relIso04_new->Write();
  h_probe_relIso04_new->Write(); 
  
  h_tag_relIso04_new_Pass->Write();
  h_probe_relIso04_new_Pass->Write();
  
  h_tag_pt_tag_relIso04_new_Pass->Write();
  h_tag_pt_probe_relIso04_new_Pass->Write();
  h_probe_pt_tag_relIso04_new_Pass->Write();
  h_probe_pt_probe_relIso04_new_Pass->Write();
  h_tag_pt_tag_relIso04_new->Write();
  h_tag_pt_probe_relIso04_new->Write();
  h_probe_pt_tag_relIso04_new->Write();
  h_probe_pt_probe_relIso04_new->Write();
  h_dR_probe_relIso04_new->Write();
  h_dR_tag_relIso04_new->Write();
  h_dR_probe_relIso04_new_Pass->Write();
  h_dR_tag_relIso04_new_Pass->Write();
  h_probe_relIso04_tag_relIso04_new->Write();
  h_probe_relIso04_tag_relIso04_new_Pass->Write();
        
  
  out_file->Close();
          
  return;
}
