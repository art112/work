
 
 // TTree Reader
 #include "TTreeReader.h"
 #include "TTreeReaderValue.h"
 #include "TSelector.h"
 
 // Basic include
 #include "TFile.h"
 #include "TTree.h"
 #include "TCanvas.h"
 #include "TFrame.h"
 #include "TH1D.h"
 #include "TH3D.h"
 #include "TH1F.h"
 #include "TH2F.h"
 #include "TBenchmark.h"
 #include "TRandom.h"
 #include "TSystem.h"
 
 //c++ include
 #include <cstdlib>  /// for exit(0) 
 
 #include "TStyle.h"
 #include "TGaxis.h"
 #include "TAxis.h"
 #include "TCanvas.h"
 #include <fstream>      // std::ifstream
 #include <vector>
 #include <sstream>
 #include <string>
 #include <limits>  // pour utiliser numeric_limits<int>::max()
 #include "TNtuple.h"
 #include <stdio.h>
 #include <THStack.h>
 #include <TMath.h>
 #include "TProfile.h"
 #include "TF1.h"
 #include "TAttLine.h"
 
 
#include "../Include/DPGAConstants.cc" 
#include "../Include/ICOConstants.cc" 
 
 int ToInteger( const std::string& s )
 {
     std::istringstream in(s) ;
     int returnValue ;
     in >> returnValue ;
     return returnValue ;
 }
 double ToDouble( const std::string& s )
 {
     std::istringstream in(s) ;
     double returnValue ;
     in >> returnValue ;
     return returnValue ;
 }
 
 string DoubleToString (double var )
 {
     string str = std::to_string(var); 
     str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
     //   if(strncmp(&str.back(),".",1)==0)cout <<"str before " <<str << endl;
     if(strncmp(&str.back(),".",1)==0){str.pop_back(); 
         /*cout <<"str after " <<str << endl;*/}
         return str;
 }
 
  double NIST_EnergyToRange (double energy ){
     
       std::map <double, double > map_Energy_Range_NIST = {{10, 1.23}, {15, 2.54}, {20, 4.26}, {25, 6.37},{30, 8.85}, {35, 11.70}, {40, 14.89}, {45, 18.41},{50, 22.27}, {55, 26.44}, {60, 30.93}, {65, 35.72},{70, 40.80}, {75, 46.18}, {80, 51.84} };
      
       return map_Energy_Range_NIST[energy];
}
 
 
 // Macro include
 
 using namespace std;
 
 //********************** Main Function *************************************************************************************************************************
 
 
 
 void Macro_Ref(){
     
       TStopwatch timer;
       timer.Start();
       
     // Maquillage graphique
     DPGAConstants *constants = new DPGAConstants();
     ICOConstants *const_ICO = new ICOConstants();
      
     cout << "*** Timer -> ";timer.Stop(); timer.Print();     timer.Continue();
       
       
     TString NameCanvas = "";
     Int_t Number_temp = 0;
     TCanvas * C_temp = NULL;
     TString NameForGet="";
     
     double Profil_entries ; TLorentzVector HalfMaxCoords ; double FWHM ; double Max ;int binmax; double x_Max_Line;
     
     // **************************************************************
     // Unité de base le mm ! MeV ! 
     // ***************************************************************
     
     int Run_ID = 0;
     std::map <int, map <string, float> >Category_comment; // VoxelSize_Z 
     std::map<int, map < float, map < float, map < float , map < string , float > > > > > map_Run; // Run_ID Z X Y Category(Dose uncertiyty dose ...) Value [mm] unite de base
     
     
     std::map<int,map <string, string >> map_Run_ID; //   #ID unique Run et Setup
     
     /***********Lecture des fichiers ******************/
     
     
     string PathFile = "../runs_PosCuve/";
     PathFile = "../runs_Prod/";
     
     
     string NameFile = "dose-proton-";
     
     vector <Double_t> Cut_Time; // ns
     
     
     std::vector<string> Type {"Dose", "Dose-Uncertainty","DoseToWater", "DoseToWater-Uncertainty"};   
     //      std::vector<string> Type {"Dose","Dose-Uncertainty","DoseToWater","DoseToWater-Uncertainty","Edep","Edep-Uncertainty","NbOfHits"};   
     
     //      string Type =  "Dose";
     
     // lecture des setups indiquer le numéro des runs !
//      std::vector<string> Run_Number {"24","25","26"};   
     std::vector<string> Run_Number {"29"};   
     int NbrSetup = 8;
     
     int Compteur_Id = 1; // init à 1

   
     const_ICO->ReadRunDose( Compteur_Id,  Run_Number, map_Run_ID, NbrSetup , PathFile,  NameFile,  Type , Category_comment,  map_Run);
      
     
     map <int,map <TString, TString>> map_LoadedMultiSetup;
     int Test = 2;
     const_ICO->LoadSetup_with_RunSystem(map_LoadedMultiSetup, PathFile,  NameFile, map_Run_ID,Test);
     
     
     /*********************************************End lecture******************************************************************************************************/
     
     
     /**************** Analyse **************************************************************************************************************************************/
  
    cout << "*** Timer -> ";timer.Stop(); timer.Print();     timer.Continue();
//      cout << endl;
// Paramètre des ID_Run      

std::map <TString , TString >  map_Init_For_Compare = {
{ "Actor_1_setResolution_X", "1"},
{ "Actor_1_setResolution_Y", "1"},
{ "Actor_1_setResolution_Z", "500"},
{ "Actor_1_stepHitType", "random"},
{ "Beam_Particle_Source", "proton"},
{ "Beam_Type", "Beam"},
{ "Beam_centre_Unit", "cm"},
{ "Beam_centre_X", "0"},
{ "Beam_centre_Y", "0"},
{ "Beam_centre_Z", "0"},
{ "Beam_direction_X", "0"},
{ "Beam_direction_Y", "0"},
{ "Beam_direction_Z", "1"},
{ "Beam_ene_mono", "68"},
{ "Beam_ene_mono_Unit", "MeV"},
{ "Beam_ene_sigma", "0"},
{ "Beam_ene_sigma_Unit", "MeV"},
{ "Beam_ene_type", "Gauss"},
{ "Beam_shape", "Circle"},
{ "Beam_sigma_Unit", "mm"},
{ "Beam_sigma_x", "0."},
{ "Beam_sigma_y", "0."},
// { "ENERGY", "20.000000"},
{ "EVENT_NB", "100000"},
{ "Frite_Lenght_Unit", "cm"},
{ "Frite_Material", "Water"},
{ "Frite_Placement_Unit", "cm"},
{ "Frite_Placement_X", "0"},
{ "Frite_Placement_Y", "0"},
{ "Frite_Placement_Z", "-2.5"},
{ "Frite_XLength", "10."},
{ "Frite_YLength", "10."},
{ "Frite_ZLength", "5."},
{ "IonisationPotential_Water", "78"},
{ "Physics", "GateV8_BIC_EMZ.mac"},
{ "Pos_Cuve_Real_Z", "50"},
{ "System", "Naked.mac"},
{ "Target_Lenght_Unit", "cm"},
{ "Target_Material", "Water"},
{ "Target_Placement_Ref_Unit", "mm"},
{ "Target_Placement_Ref_X", "0."},
{ "Target_Placement_Ref_Y", "0."},
{ "Target_Placement_Ref_Z", "50"},
{ "Target_XLength", "10."},
{ "Target_YLength", "10."},
{ "Target_ZLength", "10."},
{ "VAR_PARAMETER", "BeamEnergy"},
{ "vRoot", "6.14.00"}
};
     
//      void CompareRunWithMultiSetup(map <int,map <TString, TString>> &map_LoadedMultiSetup,std::map <TString , TString >  map_Init_For_Compare, std::vector< int > &vec_RunConfig_ok, TString NameConfig = "Default", string Mode = "Default");
    
std::vector< int > vec_RunConfig_ok;

     const_ICO->CompareRunWithMultiSetup(map_LoadedMultiSetup,map_Init_For_Compare, vec_RunConfig_ok,  "PotentielIonisation",  "Default");
     
     // Var Energy
     
     std::map<int, TH1F* > map_Histo_var_Energy; 
     std::vector< TLine* > vec_TLine; 
     
     float Compteur_Energy = 0;
     
     // 
     // File hISTO
     
     const_ICO->Histo_var_Energy_Z(  map_Histo_var_Energy,  Category_comment,   map_Run);

     
     // Graphique Part
     
      NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
      C_temp= new TCanvas(NameCanvas, NameCanvas);
      bool First = true;
      TLine * line2 ;
        
        
        
       for (auto& Histo_i: map_Histo_var_Energy){
       
               C_temp->Update();
           TH1F* h = Histo_i.second;
           
           float PourcentageMax = 80;
           HalfMaxCoords =  constants->FindHalfMaxCoords(h,PourcentageMax);
           FWHM = constants->m_FWHM;
           Max = constants->fMax;
  
        cout << "Max BraggPeak " << Max << " " << PourcentageMax << " % Max " << constants->m_Xhigh << " Integrale " << h->Integral()<< endl;
       binmax = h->GetMaximumBin();  x_Max_Line = h->GetBinContent(binmax);
       
       cout << "x_Max_Line " << x_Max_Line << endl;
   
      
//          Histo_i.second->Draw("e same");
           if(First == true){Histo_i.second->Draw("e same"); First = false; C_temp->Update();}
           else{Histo_i.second->Draw("e same"); C_temp->Update(); }
      
      C_temp->Update();
        line2 = new TLine(Max,  gPad->GetUymin(),Max,  x_Max_Line );
      
      line2 -> SetLineStyle(9);
      line2 -> SetVertical(kTRUE);
      line2->SetLineColor(kBlack);
      
      vec_TLine.push_back(line2);
           
              C_temp->Modified();
             C_temp->Update();
             
//               line2->Draw("same");
       }
       
         for (auto& Line_i: vec_TLine){
               C_temp->Update();
             Line_i->Draw();
             C_temp->Update();
             
        }
       
       
        
       
       
       
       
 } // end main
 
 
