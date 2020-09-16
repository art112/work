#include <TGraph.h>
#include <TGraphErrors.h>
// TTree Reader
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TSelector.h"
#include <TH2.h>
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

#include <iomanip>
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

float ToFloat( const std::string& s )
{
    std::istringstream in(s) ;
    float returnValue ;
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

string FloatToString (float var )
{
    string str = std::to_string(var); 
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    //   if(strncmp(&str.back(),".",1)==0)cout <<"str before " <<str << endl;
    if(strncmp(&str.back(),".",1)==0){str.pop_back(); 
        /*cout <<"str after " <<str << endl;*/}
        return str;
}


double NIST_EnergyToRange (double energy ){
    
    std::map <double, double > map_Energy_Range_NIST = {{5, 0.3613},{10, 1.23}, {15, 2.54}, {20, 4.26}, {25, 6.37},{30, 8.85}, {35, 11.70}, {40, 14.89}, {45, 18.41},{50, 22.27}, {55, 26.44}, {60, 30.93}, {65, 35.72},{70, 40.80}, {75, 46.18}, {80, 51.84} };
    
    return map_Energy_Range_NIST[energy];
}


int MarkerStyle_Auto (int iMakerStyle ){
    
//     iMakerStyle = 20;c // ref
    
    if(iMakerStyle>49) iMakerStyle = 0;
    iMakerStyle = iMakerStyle + 20; 
    
    
//     	  iMakerStyle++;
	  if(iMakerStyle == 24)iMakerStyle = 29;
	  if(iMakerStyle == 30)iMakerStyle = 33;
	    if(iMakerStyle == 35)iMakerStyle = 39;
	     if(iMakerStyle == 40)iMakerStyle = 43;
	    	     if(iMakerStyle == 44)iMakerStyle = 45;
		         if(iMakerStyle == 46)iMakerStyle = 47;
			 if(iMakerStyle == 49)iMakerStyle = 20;
    
    
    return iMakerStyle;
}

void format_h(TH1F* h, int linecolor){
h->SetLineWidth(3);
h->SetLineColor(linecolor);
}

// Macro include5

using namespace std;

//********************** Main Function *************************************************************************************************************************



void Etude_Potentiel_Ionisation_Pos_Pic_Devellopement_Os(){
    
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
    std::vector<string> Run_Number;
//     Equivalent_Density_Water DoseToWater_G4_Water DoseToWater_G4_Water_YZ DoseToWater_G4_Water_Z_All Essai_ENERGY
    TString Study_Material = "Essai_ENERGY";
        // EDepthIoni  Dose_DoseToWater
    TString TypeOfStudy = "Dose_DoseToWater" ;
    
    // if DoseToWater Z YZ XYZ
       TString SelectionOfTypeHisto = "Z";

    bool Print = false;
    // RibBone
    
    if(Study_Material == "RibBone"){
        PathFile = "../runs_Potentiel_Ioni_Material/";
        //      std::vector<string> Run_Number {"4","5","6","7","8"};  
        Run_Number.push_back("1"); 
        Run_Number.push_back("2"); 
        Run_Number.push_back("3");
        Run_Number.push_back("4"); 
        Run_Number.push_back("5");  
        Run_Number.push_back("6");
        //         Run_Number.push_back("8");  
    }
    else if(Study_Material == "Water"){
        // Water
        PathFile = "../Potentiel_Ioni_Water/";
        //     std::vector<string> Run_Number {"1","2","3","4","5","6"};
        Run_Number.push_back("4"); 
        Run_Number.push_back("5");  
        Run_Number.push_back("6");
        Run_Number.push_back("7"); 
        Run_Number.push_back("8"); 
        
    }
    else if(Study_Material == "Lung"){
        // Lung
        
        
        PathFile = "../runs_Potentiel_Ioni/";
        //      std::vector<string> Run_Number {"13","14","15","17"};  
        Run_Number.push_back("28"); 
        Run_Number.push_back("29"); 
        Run_Number.push_back("30");
        Run_Number.push_back("31");  
        Run_Number.push_back("32"); 
        
    }
    else if(Study_Material == "Muscle"){
        // Muscle
        
        PathFile = "../runs_Potentiel_Ioni_Material/";
        Run_Number.push_back("8"); 
        Run_Number.push_back("9"); 
        //         Run_Number.push_back("10");
        Run_Number.push_back("11");  
        Run_Number.push_back("12");  
        Run_Number.push_back("13");  
        Run_Number.push_back("20");  
    }
    else if(Study_Material == "Cartilage"){
        // Cartilage 
        PathFile = "../runs_Potentiel_Ioni_Material/";
        Run_Number.push_back("14"); 
        Run_Number.push_back("15"); 
        Run_Number.push_back("16");
        Run_Number.push_back("17");  
        Run_Number.push_back("18");  
        
        //          Run_Number.push_back("13");  
    }
    else if(Study_Material == "Density_Water"){
        // Cartilage 
        PathFile = "../runs_Potentiel_Ioni_Water_Density/";
        Run_Number.push_back("1"); 
        Run_Number.push_back("2");
        Run_Number.push_back("21"); 
        Run_Number.push_back("3");
        Run_Number.push_back("4");  
        Run_Number.push_back("22"); 
        //         Run_Number.push_back("20");  
        //          Run_Number.push_back("13");  
    }
    else if(Study_Material == "Equivalent_Density_Water"){
        // Cartilage 
        PathFile = "../runs_Potentiel_Ioni_Water_Density/";
        /*   Run_Number.push_back("1"); 
         *        Run_Number.push_back("2"); 
         *       
         *        Run_Number.push_back("4"); */ 
        
        
         Run_Number.push_back("3"); // Water
//         Run_Number.push_back("22"); // G4_WATER
//       Run_Number.push_back("66");  // Water_G4
        
      
//         Run_Number.push_back("43");  // G4_BONE_COMPACT_ICRU
//         Run_Number.push_back("25");  // RibBone_G4_d_1_00
//               Run_Number.push_back("40");  // RibBone_d_1_00 PB Ionisation mean different !!! not 75 eV
      
                      Run_Number.push_back("71");  // RibBoned_1_00
//               Run_Number.push_back("72");  // RibBone_G4_d_1_00
              
        
        Run_Number.push_back("5");  // PMMA_d_1_00
//         Run_Number.push_back("64");  // PMMA_G4_d_1_00
        
        
        
        Run_Number.push_back("27");  // Cartilage_d_1_00
//         Run_Number.push_back("62");  // Cartilage_G4_d_1_00
        
//         Run_Number.push_back("34"); // 
        Run_Number.push_back("29");  // Testis_d_1_00
        Run_Number.push_back("38");  // Lung_d_1_00
//   
//         
//       
//         
        Run_Number.push_back("44");  // SpineBone_d_1_00
//       Run_Number.push_back("67");  // SpineBone_G4_d_1_00
//         
//         
        Run_Number.push_back("46"); // Skeleton_spongiosa_d_1_00
//         Run_Number.push_back("60");  // Skeleton_spongiosa_d_1_00
//         
//         
//         
        Run_Number.push_back("49");  // Skeleton_yellow_marrow_d_1_00
        Run_Number.push_back("55");  // Muscle_d_1_00
//        
        Run_Number.push_back("59");  // Skeleton_red_marrow_d_1_00
       
     
       
//         Run_Number.push_back("69");
//         Run_Number.push_back("70");  
        //                                             Run_Number.push_back("41");  // Muscle_d_1_00
        //                                               Run_Number.push_back("42");   // SpineBone
        //                                                 Run_Number.push_back("43");  // G4_BONE_COMPACT_ICRU
        
    }
    else if(Study_Material == "DoseToWater_G4_Water_Z"){
        // Cartilage 
        PathFile = "../runs_Dose_DoseToWater/";
        /*   Run_Number.push_back("1"); 
         *        Run_Number.push_back("2"); 
         *        Run_Number.push_back("3");
         *        Run_Number.push_back("4"); */ 
        Run_Number.push_back("2"); 
//         Run_Number.push_back("3"); 
        
    }
    else if(Study_Material == "DoseToWater_G4_Water_Z_All"){
        // Cartilage 
        PathFile = "../runs_Potentiel_Ioni_Material/";
        /*   Run_Number.push_back("1"); 
         *        Run_Number.push_back("2"); 
         *        Run_Number.push_back("3");
         *        Run_Number.push_back("4"); */ 
        Run_Number.push_back("1");
        Run_Number.push_back("7");
        Run_Number.push_back("13");
        Run_Number.push_back("17");
        
        
//         Run_Number.push_back("3"); 
        
    }
  else if(Study_Material == "DoseToWater_G4_Water_YZ"){
        // Cartilage 
        PathFile = "../runs_Dose_DoseToWater/";
        /*   Run_Number.push_back("1"); 
         *        Run_Number.push_back("2"); 
         *        Run_Number.push_back("3");
         *        Run_Number.push_back("4"); */ 
//         Run_Number.push_back("2"); 
        Run_Number.push_back("6"); 
        
    }
     else if(Study_Material == "Essai_ENERGY"){
        // Cartilage 
        PathFile = "../runs_Test_Energy/";
        /*   Run_Number.push_back("1"); 
         *        Run_Number.push_back("2"); 
         *        Run_Number.push_back("3");
         *        Run_Number.push_back("4"); */ 
//         Run_Number.push_back("2"); 
        Run_Number.push_back("65"); 
        
    }
    else cout << "Unknow Material " << endl;
    
    
    string NameFile = "dose-proton-";
    
    vector <Double_t> Cut_Time; // ns
    
//     std::vector<string> Type {"Dose", "Dose-Uncertainty"};  
    
         std::vector<string> Type {"Dose", "Dose-Uncertainty","DoseToWater", "DoseToWater-Uncertainty"};   
    //      std::vector<string> Type {"Dose","Dose-Uncertainty","DoseToWater","DoseToWater-Uncertainty","Edep","Edep-Uncertainty","NbOfHits"};   
    
    //      string Type =  "Dose";
    
    // lecture des setups indiquer le numéro des runs !
    //      std::vector<string> Run_Number {"24","25","26"}; 
    
    
    
    //     std::vector<string> Run_Number {"7","8","9","10","11","12"};  
    //      std::vector<string> Run_Number {"1","2"};  
    
    map <int,map <TString, TString>> map_LoadedMultiSetup;
    int ID_Number_Init = 1;
    int Test = 0;
    
    
    
    const_ICO->LoadSetup_with_RunSystem_v2(map_LoadedMultiSetup, PathFile,  NameFile ,Run_Number,ID_Number_Init, Test);
    cout <<"\n SetupLoad : ID_Number_Init " << ID_Number_Init-1 << endl; 
    cout << endl;
    
    Test = 0;
//     const_ICO->ReadRunDose_v2(  map_LoadedMultiSetup,   Type ,  Category_comment,  map_Run, Test );
    
    cout << "*** Timer phase LoadData -> ";timer.Stop(); timer.Print();     timer.Continue();
    
    /*********************************************End lecture data ******************************************************************************************************/
    
    
    /**************** Analyse **************************************************************************************************************************************/
    
    // ******************************************************************************************************************************/
    // *********** NOTE Etude E vs Depth ******************** ************************** ********************** *******************
    // ******************************************************************************************************************************/
    

    
    cout << "TypeOfStudy selected : " << TypeOfStudy << endl;
    
    // WARNING NOTE Study EDepthIoni Impact Potiential Ionisation Density Atomique Composition  CompareDose To Wate
    if(TypeOfStudy.Contains("Dose_DoseToWater")){
        
        
        cout << "\n*** Analyse Dose_DoseToWater START ***" << endl;
        
        std::map <TString , TString >  map_Init_For_Compare = {
//             { "Actor_1_attachTo", "frite"},
//             { "Actor_1_enableDose", "true"},
//             { "Actor_1_enableDoseToWater", "true"},
//             { "Actor_1_enableEdep", "true"},
//             { "Actor_1_enableNumberOfHits", "true"},
//             { "Actor_1_enableUncertaintyDose", "true"},
//             { "Actor_1_enableUncertaintyDoseToWater", "true"},
//             { "Actor_1_enableUncertaintyEdep", "true"},
//             { "Actor_1_normaliseDoseToIntegral", "false"},
//             //         { "Actor_1_setResolution_X", "1"},
//             //         { "Actor_1_setResolution_Y", "1"},
//             //         { "Actor_1_setResolution_Z", "500"},
//             { "Actor_1_stepHitType", "random"},
//             { "Beam_Particle_Source", "proton"},
//             { "Beam_Type", "Beam"},
//             { "Beam_centre_Unit", "cm"},
//             { "Beam_centre_X", "0"},
//             { "Beam_centre_Y", "0"},
//             { "Beam_centre_Z", "0"},
//             { "Beam_direction_X", "0"},
//             { "Beam_direction_Y", "0"},
//             { "Beam_direction_Z", "1"},
// //                     { "Beam_ene_mono", "68"},
//             //         { "Beam_ene_mono_Unit", "MeV"},
//             //         { "Beam_ene_sigma", "0"},
//             //         { "Beam_ene_sigma_Unit", "MeV"},
//             { "Beam_ene_type", "Gauss"},
//             { "Beam_shape", "Circle"},
//             { "Beam_sigma_Unit", "mm"},
//             { "Beam_sigma_x", "0."},
//             { "Beam_sigma_y", "0."},
// //             { "EVENT_NB", "100000"},
//             { "Frite_Lenght_Unit", "cm"},
//             // { "Frite_Material", "G4_WATER"},
//             { "Frite_Placement_Unit", "cm"},
//             //         { "Frite_Placement_X", "0"},
//             //         { "Frite_Placement_Y", "0"},
//             //         { "Frite_Placement_Z", "-2.5"},
//             //         { "Frite_XLength", "10."},
//             //         { "Frite_YLength", "10."},
//             //         { "Frite_ZLength", "5."},
//             // { "IonisationPotential", "78"},
//             { "Physics", "GateV8_BIC_EMZ.mac"},
//             //         { "Pos_Cuve_Real_Z", "50"},
//             { "SetCutCuve", "0.1"},
//             { "System", "Naked.mac"},
//             //         { "Target_Lenght_Unit", "cm"},
//             // { "Target_Material", "G4_WATER"},
//             //         { "Target_Placement_Ref_Unit", "mm"},
//             //         { "Target_Placement_Ref_X", "0."},
//             //         { "Target_Placement_Ref_Y", "0."},
//             //         { "Target_Placement_Ref_Z", "50"},
//             //         { "Target_XLength", "10."},
//             //         { "Target_YLength", "10."},
//             //         { "Target_ZLength", "20."},
//             { "ENERGY", "60"},
//             { "VAR_PARAMETER", "BeamEnergy"}
        };
        
         std::vector< int > vec_RunConfig_ok;
        const_ICO->CompareRunWithMultiSetup(map_LoadedMultiSetup,map_Init_For_Compare, vec_RunConfig_ok,  "PotentielIonisation",  "Default");
        
        const_ICO->ReadRunDose_v2_Light(  map_LoadedMultiSetup,   Type ,  Category_comment,  map_Run,vec_RunConfig_ok,  2 );
     
        
        if(SelectionOfTypeHisto == "Z"){
            
            
            
              std::vector<TString > Category_Type ;
        for (auto& type_vec: Type){
        TString Type_TS = type_vec ; 
        if(Type_TS.Contains("-Uncertainty")==true){continue;}
        else{ Category_Type.push_back(Type_TS);
            cout << "Categorie Read " << Type_TS << endl;
        }
        }
            
            
            std::map<int,map< TString, TH1F* >> map_Histo_var_Energy_Z; 
            std::vector< TLine* > vec_TLine; 
            
            
            float Compteur_Energy = 0;
            // 
            // File hISTO
            Test =0;
            TString MiseEnFormeData = "nGy_Per_Particle";
            
            const_ICO->Histo_var_Energy_Z_v3( map_LoadedMultiSetup, map_Histo_var_Energy_Z, Category_comment, map_Run,vec_RunConfig_ok, MiseEnFormeData,Type ,Test );
            
            
            // Graphique Part
            
            std::map<TString , std::map< float,int >> map_MATERIAL_IONI_E ; // Material Energie Type
            
            // { "IonisationPotential", "78"},
            // { "Target_Material", "G4_WATER"},
            // { "ENERGY", "70.000000"},
            
            // Mise en forme pour exploiter les données.
            for (auto& id: vec_RunConfig_ok){
                int ID_Setup = id;
                TString MATERIAL = map_LoadedMultiSetup[ID_Setup]["Target_Material"];
                Float_t IonisationPotential = ToFloat(map_LoadedMultiSetup[ID_Setup]["IonisationPotential"].Data());
                float ENERGY_ft =  ToDouble(map_LoadedMultiSetup[ID_Setup]["ENERGY"].Data());
                map_MATERIAL_IONI_E[MATERIAL][ENERGY_ft]=ID_Setup;
                cout << "id " << id << " MATERIAL " << MATERIAL << " IonisationPotential " << IonisationPotential << " ENERGY_ft " << ENERGY_ft << endl;
            }
            
            for (auto& Material_i: map_MATERIAL_IONI_E){
            cout << "\n\n   Material " <<   Material_i.first  << endl;
            TString Material_TS =  Material_i.first;
//             
            for (auto& Energy_i: Material_i.second){
            
                cout << "\n           Energy_i " <<   Energy_i.first << endl;
                
                
                int Setup = Energy_i.second;
                
                TString NameHisto_temps = "";
                NameHisto_temps = "hDose_"+ Material_i.first + FloatToString(Energy_i.first);
                
                           NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
                C_temp= new TCanvas(NameCanvas, NameCanvas);
                
                bool First = true;
                
                TH1F * dividend; 
                
                for (auto& Category_i: Category_Type){
                    cout << "Category_i " << Category_i << endl;
                    
                    if(First == true){
                        First = false;
                         map_Histo_var_Energy_Z[Setup][Category_i]->Draw();
                         dividend =new TH1F(*map_Histo_var_Energy_Z[Setup][Category_i]);
                    }
                    else{ 
                     
                        map_Histo_var_Energy_Z[Setup][Category_i]->Draw("same");
                        dividend->Divide( map_Histo_var_Energy_Z[Setup][Category_i]);
                    }
                }
                
TLegend *leg;
                   leg = new TLegend(0.1888058,0.6379379,0.3999438,0.8562011,NULL,"brNDC");
                
                   Float_t IonisationPotential = ToFloat(map_LoadedMultiSetup[Setup]["IonisationPotential"].Data());
                   
                    
                           std::ostringstream streamObj3;
                    // Set Fixed -Point Notation
                    streamObj3 << std::fixed;
                    // Set precision to 2 digits
                    streamObj3 << std::setprecision(2);
                    //Add double to stream
                    streamObj3 << IonisationPotential;
                    
                    // Get string from output string stream
                    std::string strObj3 = streamObj3.str();
                    
                     TString HeaderLeg = Material_TS + " I=" + strObj3 + " eV";
                    leg->SetHeader(HeaderLeg);
                
                // Graphical Maquillage
              dividend->SetTitle(";Z;Dose / DoseToWater");
             format_h(dividend,kOrange);
             
             map_Histo_var_Energy_Z[Setup]["Dose"]->SetTitle(";;nGy / Particle");
             gStyle->SetOptStat(0);
            
             NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
             C_temp= new TCanvas(NameCanvas, NameCanvas);
                
C_temp->Divide(1,2,0,0);
C_temp->cd(1);
C_temp->GetPad(1)->SetRightMargin(.01);

map_Histo_var_Energy_Z[Setup]["Dose"]->SetLineColor(2);
map_Histo_var_Energy_Z[Setup]["DoseToWater"]->SetLineColor(4);

map_Histo_var_Energy_Z[Setup]["Dose"]->Draw("Hist");
map_Histo_var_Energy_Z[Setup]["DoseToWater"]->Draw("HistSame");

leg->AddEntry(map_Histo_var_Energy_Z[Setup]["Dose"], "Dose", "alp");
leg->AddEntry(map_Histo_var_Energy_Z[Setup]["DoseToWater"], "DoseToWater", "alp");
leg->Draw();
              

C_temp->cd(2);
dividend->GetYaxis()->SetRangeUser(0,2.49);
C_temp->GetPad(2)->SetGridy();
C_temp->GetPad(2)->SetRightMargin(.01);
dividend->Draw();
C_temp->Draw();
            

  
            } // END Energy_i
            } // END map_MATERIAL_IONI_E
            
            
        } // Z
        
        if(SelectionOfTypeHisto == "YZ"){
            
            
            
              std::vector<TString > Category_Type ;
        for (auto& type_vec: Type){
        TString Type_TS = type_vec ; 
        if(Type_TS.Contains("-Uncertainty")==true){continue;}
        else{ Category_Type.push_back(Type_TS);
            cout << "Categorie Read " << Type_TS << endl;
        }
        }
            
            
            std::map<int,map< TString, TH2F* >> map_Histo_var_Energy_Z; 
            std::vector< TLine* > vec_TLine; 
            
            
            float Compteur_Energy = 0;
            // 
            // File hISTO
            Test =0;
            TString MiseEnFormeData = "nGy_Per_Particle";
            
            const_ICO->Histo_var_Energy_YZ_v3( map_LoadedMultiSetup, map_Histo_var_Energy_Z, Category_comment, map_Run,vec_RunConfig_ok, MiseEnFormeData,Type ,2 );
            
            
            // Graphique Part
            
            std::map<TString , std::map< float,int >> map_MATERIAL_IONI_E ; // Material Energie Type
            
            // { "IonisationPotential", "78"},
            // { "Target_Material", "G4_WATER"},
            // { "ENERGY", "70.000000"},
            
            // Mise en forme pour exploiter les données.
            for (auto& id: vec_RunConfig_ok){
                int ID_Setup = id;
                TString MATERIAL = map_LoadedMultiSetup[ID_Setup]["Target_Material"];
                Float_t IonisationPotential = ToFloat(map_LoadedMultiSetup[ID_Setup]["IonisationPotential"].Data());
                float ENERGY_ft =  ToDouble(map_LoadedMultiSetup[ID_Setup]["ENERGY"].Data());
                map_MATERIAL_IONI_E[MATERIAL][ENERGY_ft]=ID_Setup;
                cout << "id " << id << " MATERIAL " << MATERIAL << " IonisationPotential " << IonisationPotential << " ENERGY_ft " << ENERGY_ft << endl;
            }
            
            for (auto& Material_i: map_MATERIAL_IONI_E){
            cout << "\n\n   Material " <<   Material_i.first  << endl;
            TString Material_TS =  Material_i.first;
// //             
            for (auto& Energy_i: Material_i.second){
//             
                cout << "\n           Energy_i " <<   Energy_i.first << endl;
//                 
//                 
                int Setup = Energy_i.second;
//                 
                TString NameHisto_temps = "";
                NameHisto_temps = "hDose_"+ Material_i.first + FloatToString(Energy_i.first);
//                 
                  
//                 
                bool First = true;
//                 
//                 TH2F * dividend; 
//                 
                for (auto& Category_i: Category_Type){
                    cout << "Category_i " << Category_i << endl;
//                     

                    
                NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
                C_temp= new TCanvas(NameCanvas, NameCanvas);
                map_Histo_var_Energy_Z[Setup][Category_i]->Draw("colz");
                
                    
                    
                } // END Category
//                 
// 
//   
            } // END Energy_i
            } // END map_MATERIAL_IONI_E
            
            
        } // YZ
        
//         TH1F * hTemps;
//         
//         
//         TH2D *Cadrehisto = new TH2D ("Cadrehisto", "Diff ; Beam energy [MeV]; #Delta [mm] ", 70, 0, 70,100,-10,10);
//         Cadrehisto->GetYaxis()->SetRangeUser(-1,1);
//                 
//         
//         
//         for (auto& Material_i: map_MATERIAL_IONI_E){
//             cout << "\n\n   Material " <<   Material_i.first  << endl;
//             TString Material_TS =  Material_i.first;
//             
//             for (auto& Energy_i: Material_i.second){
//                 
//                  cout << "\n           Energy_i " <<   Energy_i.first /*<< " Size Energy_i " << map_MATERIAL_IONI_E[Ionisation_i.first][Energy_i.first].size()*/ << " Setup " << Energy_i.second << endl;
//                 
//                  
//                 TString NameHisto_temps = "";
//                 NameHisto_temps = "hDiff_E_"+ Material_i.first + FloatToString(Ionisation_i.first);
//                 
//                 hDiff2D= new TH2D (NameHisto_temps.Data(), "Diff Depth par rapport à l iono de ref; Beam energy [MeV]; #Delta [mm] ", 70, 0, 70,100,-10,10);
//                 
//                 
//                 
//                 NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
//                 C_temp= new TCanvas(NameCanvas, NameCanvas);
//                 
//                  
//                  
//                  
//                  
//             }
//             
//             
//              }
//             
//        
//                 
//                 
//     
//         
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        cout << "\n*** Analyse Dose_DoseToWater END ***" << endl;
    } // END TypeOfStudy 
    
    
    
    // WARNING NOTE Study EDepthIoni Impact Potiential Ionisation Density Atomique Composition 
    if(TypeOfStudy.Contains("EDepthIoni")){
        
        cout << "\n*** Analyse EDepthIoni START ***" << endl;
        
        std::map <TString , TString >  map_Init_For_Compare = {
//             { "Actor_1_attachTo", "frite"},
//             { "Actor_1_enableDose", "true"},
//             { "Actor_1_enableDoseToWater", "true"},
//             { "Actor_1_enableEdep", "true"},
//             { "Actor_1_enableNumberOfHits", "true"},
//             { "Actor_1_enableUncertaintyDose", "true"},
//             { "Actor_1_enableUncertaintyDoseToWater", "true"},
//             { "Actor_1_enableUncertaintyEdep", "true"},
//             { "Actor_1_normaliseDoseToIntegral", "false"},
//             //         { "Actor_1_setResolution_X", "1"},
//             //         { "Actor_1_setResolution_Y", "1"},
//             //         { "Actor_1_setResolution_Z", "500"},
//             { "Actor_1_stepHitType", "random"},
//             { "Beam_Particle_Source", "proton"},
//             { "Beam_Type", "Beam"},
//             { "Beam_centre_Unit", "cm"},
//             { "Beam_centre_X", "0"},
//             { "Beam_centre_Y", "0"},
//             { "Beam_centre_Z", "0"},
//             { "Beam_direction_X", "0"},
//             { "Beam_direction_Y", "0"},
//             { "Beam_direction_Z", "1"},
//             //         { "Beam_ene_mono", "68"},
//             //         { "Beam_ene_mono_Unit", "MeV"},
//             //         { "Beam_ene_sigma", "0"},
//             //         { "Beam_ene_sigma_Unit", "MeV"},
//             { "Beam_ene_type", "Gauss"},
//             { "Beam_shape", "Circle"},
//             { "Beam_sigma_Unit", "mm"},
//             { "Beam_sigma_x", "0."},
//             { "Beam_sigma_y", "0."},
//             { "EVENT_NB", "100000"},
//             { "Frite_Lenght_Unit", "cm"},
//             // { "Frite_Material", "G4_WATER"},
//             { "Frite_Placement_Unit", "cm"},
//             //         { "Frite_Placement_X", "0"},
//             //         { "Frite_Placement_Y", "0"},
//             //         { "Frite_Placement_Z", "-2.5"},
//             //         { "Frite_XLength", "10."},
//             //         { "Frite_YLength", "10."},
//             //         { "Frite_ZLength", "5."},
//             // { "IonisationPotential", "78"},
//             { "Physics", "GateV8_BIC_EMZ.mac"},
//             //         { "Pos_Cuve_Real_Z", "50"},
//             { "SetCutCuve", "0.1"},
//             { "System", "Naked.mac"},
//             //         { "Target_Lenght_Unit", "cm"},
//             // { "Target_Material", "G4_WATER"},
//             //         { "Target_Placement_Ref_Unit", "mm"},
//             //         { "Target_Placement_Ref_X", "0."},
//             //         { "Target_Placement_Ref_Y", "0."},
//             //         { "Target_Placement_Ref_Z", "50"},
//             //         { "Target_XLength", "10."},
//             //         { "Target_YLength", "10."},
//             //         { "Target_ZLength", "20."},
//             { "VAR_PARAMETER", "BeamEnergy"}
        };
        
        
        //  void CompareRunWithMultiSetup(map <int,map <TString, TString>> &map_LoadedMultiSetup,std::map <TString , TString >  map_Init_For_Compare, std::vector< int > &vec_RunConfig_ok, TString NameConfig = "Default", string Mode = "Default");
        
        // ******************************************
        // NOTE Recherche des Setups adaptées à l'étude 
        // *****
        
        
        
        std::vector< int > vec_RunConfig_ok;
        const_ICO->CompareRunWithMultiSetup(map_LoadedMultiSetup,map_Init_For_Compare, vec_RunConfig_ok,  "PotentielIonisation",  "Default");
        const_ICO->ReadRunDose_v2_Light(  map_LoadedMultiSetup,   Type ,  Category_comment,  map_Run,vec_RunConfig_ok,  Test );
        // **** 
        
        // Var Energy
        std::map<int, TH1F* > map_Histo_var_Energy_Z;
        std::map<int,map < TString, TH3F* >> map_Histo_var_Energy_XYZ; 
        
        std::vector< TLine* > vec_TLine; 
        
        std::map<int, std::map< TString, TString > > map_Histo_var_Energy_Z_HalfMaxCoords; // Extraction information position half maximum 
        
        float Compteur_Energy = 0;
        // 
        // File hISTO
        Test =0;
        TString MiseEnFormeData = "nGy_Per_Particle";
        const_ICO->Histo_var_Energy_Z_v2( map_LoadedMultiSetup, map_Histo_var_Energy_Z, Category_comment, map_Run,vec_RunConfig_ok, MiseEnFormeData ,Test );
        
        // File hISTO
        // Bool Integral 
        const_ICO->FindMaxHalfCoordinateHisto(map_Histo_var_Energy_Z,map_Histo_var_Energy_Z_HalfMaxCoords,true);
        
        // Graphique Part
        
        std::map<TString, std::map< Float_t , std::map< float ,int > >> map_MATERIAL_IONI_E ; // Material Ionisation Energie
        
        // { "IonisationPotential", "78"},
        // { "Target_Material", "G4_WATER"},
        // { "ENERGY", "70.000000"},
        
        // Mise en forme pour exploiter les données.
        
        map < float, int > SetupRef_map;
        
        int Setup_Ref = -1;
        
        TString SetupRef_TS ;
        
        for (auto& id: vec_RunConfig_ok){
            
            int ID_Setup = id;
            
            TString MATERIAL = map_LoadedMultiSetup[ID_Setup]["Target_Material"];
            Float_t IonisationPotential = ToFloat(map_LoadedMultiSetup[ID_Setup]["IonisationPotential"].Data());
            float ENERGY_ft =  ToDouble(map_LoadedMultiSetup[ID_Setup]["ENERGY"].Data());
            
            map_MATERIAL_IONI_E[MATERIAL][IonisationPotential][ENERGY_ft]=ID_Setup;
            cout << "id " << id << " MATERIAL " << MATERIAL << " IonisationPotential " << IonisationPotential << " ENERGY_ft " << ENERGY_ft << endl;
            
            
            if( MATERIAL.Contains("RibBone") ){ 
                if(map_LoadedMultiSetup[ID_Setup]["IonisationPotential"] == "102.82"){
                    Setup_Ref = id; 
                    SetupRef_map[ENERGY_ft]=id;
                    
                } 
            }
            else if( MATERIAL.Contains("Lung") ){ 
                if(map_LoadedMultiSetup[ID_Setup]["IonisationPotential"] == "69.7265"){
                    Setup_Ref = id; 
                    SetupRef_map[ENERGY_ft]=id;
                    
                }
            }
            else if( MATERIAL.Contains("Muscle") ){ 
                if(map_LoadedMultiSetup[ID_Setup]["IonisationPotential"].Contains( "69.48")){
                    Setup_Ref = id; 
                    SetupRef_map[ENERGY_ft]=id;
                    
                } 
            }
            else if( MATERIAL.Contains("Cartilage") ){ 
                if(map_LoadedMultiSetup[ID_Setup]["IonisationPotential"] == "72.1596"){
                    Setup_Ref = id; 
                    SetupRef_map[ENERGY_ft]=id;
                    
                } 
            }
            else if( MATERIAL=="Water" ){ 
                //             if(map_LoadedMultiSetup[ID_Setup]["IonisationPotential"] == "75"){
                //                 Setup_Ref = id; 
                //                 SetupRef_map[ENERGY_ft]=id;
                //                 
                //             } 
            }
            else {;}
            //          NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
            //         C_temp= new TCanvas(NameCanvas, NameCanvas);
            
            //         map_Histo_var_Energy_Z[id]->Draw();
            
        }
        
        
        if( Setup_Ref > -1) {
            
            
            SetupRef_TS +=  map_LoadedMultiSetup[Setup_Ref]["Target_Material"] + " " + map_LoadedMultiSetup[Setup_Ref]["IonisationPotential"] + " eV";
            cout << "We have a Setup ref !!! -> "<< Setup_Ref  << endl;
        }
        
        //    std::map<TString, std::map< int 
        
        //      vector <TGraph*> StockageGraphForMulti;
        
        TGraphErrors* Nist_Water ;
        
        bool NIST = true;
        
        if(NIST == true){
            std::vector<Float_t> x_TGraph;
            std::vector<Float_t> y_TGraph;
            std::vector<Float_t> ex_TGraph;
            std::vector<Float_t> ey_TGraph;
            
            for(int i = 5; i< 75; i+=5){
                x_TGraph.push_back(i);
                y_TGraph.push_back(NIST_EnergyToRange(i));
                ex_TGraph.push_back(0);
                ey_TGraph.push_back(0.01);
            }
            Int_t n = x_TGraph.size();
            Nist_Water = new TGraphErrors(n,&x_TGraph[0],&y_TGraph[0],&ex_TGraph[0],&ey_TGraph[0]);
            
            
        }
        
        
        std::map<TString, std::map< Float_t ,TGraphErrors* >> map_StockageGraphForMulti;
        
        std::map<TString, std::map< Float_t ,TGraphErrors* >> map_StockageGraphForMulti_Diff;
        
        std::map<TString,std::map<float,TH2D*>> map_StockageTH1DForMulti;
        
        TGraphErrors * TempsGraph;
        
        TGraphErrors * TempsGraph_Diff;
        
        TGraphErrors * TGraphG4_Water;
        
        TH2D* hDiff2D;
        
        
        // Stat
        for (auto& Material_i: map_MATERIAL_IONI_E){
            cout << "\n\n   Material " <<   Material_i.first << " Size Material " << map_MATERIAL_IONI_E.size() << endl;
            
            TString Material_TS =  Material_i.first;
            
            for (auto& Ionisation_i: Material_i.second){
                cout << "\n     Ionisation " <<   Ionisation_i.first  << endl;
                
                
                std::vector<Float_t> x_TGraph;
                std::vector<Float_t> y_TGraph;
                std::vector<Float_t> ex_TGraph;
                std::vector<Float_t> ey_TGraph;
                
                
                std::vector<Float_t> x_TGraph_Diff;
                std::vector<Float_t> y_TGraph_Diff;
                std::vector<Float_t> ex_TGraph_Diff;
                std::vector<Float_t> ey_TGraph_Diff;
                
                
                //              TH2D* hDiff2D; hDiff2D = 
                
                TString NameHisto_temps = "";
                NameHisto_temps = "hDiff_E_"+ Material_i.first + FloatToString(Ionisation_i.first);
                
                hDiff2D= new TH2D (NameHisto_temps.Data(), "Diff Depth par rapport à l iono de ref; Beam energy [MeV]; #Delta [mm] ", 70, 0, 70,100,-10,10);
                
                
                
                NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
                C_temp= new TCanvas(NameCanvas, NameCanvas);
                
                
                
                for (auto& Energy_i: Ionisation_i.second){
                    
                    
                    cout << "\n           Energy_i " <<   Energy_i.first /*<< " Size Energy_i " << map_MATERIAL_IONI_E[Ionisation_i.first][Energy_i.first].size()*/ << " Setup " << Energy_i.second << endl;
                    
                    
                    float m_Yhigh_80 = 0;
                    TString Temps = map_Histo_var_Energy_Z_HalfMaxCoords[Energy_i.second]["m_Xhigh_80"];
                    cout << "Temps " << Temps << endl;
                    
                    m_Yhigh_80 = ToDouble(Temps.Data());
                    
                    if(m_Yhigh_80 > 0){
                        
                        x_TGraph.push_back(Energy_i.first);
                        
                        y_TGraph.push_back(m_Yhigh_80);
                        
                        ex_TGraph.push_back(0);
                        ey_TGraph.push_back(0.1);
                        
                        //                 cout << "map_Histo_var_Energy_Z_HalfMaxCoords[Energy_i.second][m_Yhigh_80] " << map_Histo_var_Energy_Z_HalfMaxCoords[Energy_i.second]["m_Xhigh_80"] << " " << m_Yhigh_80 << endl;
                        
                        
                        x_TGraph_Diff.push_back(Energy_i.first);
                        
                        
                        if(Material_TS.Contains("Water")|| Material_TS.Contains("WATER") || Material_TS.Contains("d_1_00") ){ y_TGraph_Diff.push_back(m_Yhigh_80-NIST_EnergyToRange(Energy_i.first));}
                        else if (Setup_Ref >-1){y_TGraph_Diff.push_back(m_Yhigh_80- ToFloat(map_Histo_var_Energy_Z_HalfMaxCoords[SetupRef_map[Energy_i.first]]["m_Xhigh_80"].Data()) );
                            
                            cout << "Diff Setup ref " << SetupRef_map[Energy_i.first] << " " << ToFloat(map_Histo_var_Energy_Z_HalfMaxCoords[SetupRef_map[Energy_i.first]]["m_Xhigh_80"].Data()) << " " << m_Yhigh_80 << endl;
                            
                        }
                        else{cout << "*** WARNING Unknow reference please check " << Material_TS << " WARNING ***" << endl; y_TGraph_Diff.push_back(m_Yhigh_80-0); }
                        
                        ex_TGraph_Diff.push_back(0);
                        ey_TGraph_Diff.push_back(0.1);
                        
                        
                        hDiff2D->Fill(Energy_i.first,m_Yhigh_80-NIST_EnergyToRange(Energy_i.first));
                        
                        cout << m_Yhigh_80-NIST_EnergyToRange(Energy_i.first) << endl;
                    }
                    
                    
                    std::ostringstream streamObj3;
                    // Set Fixed -Point Notation
                    streamObj3 << std::fixed;
                    // Set precision to 2 digits
                    streamObj3 << std::setprecision(2);
                    //Add double to stream
                    streamObj3 << Ionisation_i.first;
                    
                    // Get string from output string stream
                    std::string strObj3 = streamObj3.str();
                    
                    TString Title = "Bragg Peak energy for " + Material_TS + " I=" + strObj3 + " eV";
                    
                    map_Histo_var_Energy_Z[Energy_i.second]->SetTitle(Title);
                    map_Histo_var_Energy_Z[Energy_i.second]->GetYaxis()->SetTitle("Dose [nGy/particle]");
                    map_Histo_var_Energy_Z[Energy_i.second]->GetXaxis()->SetTitle("Depth (cm)");
                    
                    map_Histo_var_Energy_Z[Energy_i.second]->Draw("same hist PLC");
                    
                }
                Int_t n = x_TGraph.size();
                
                TempsGraph = new TGraphErrors(n,&x_TGraph[0],&y_TGraph[0],&ex_TGraph[0],&ey_TGraph[0]);
                
                map_StockageTH1DForMulti[Material_i.first][Ionisation_i.first]= hDiff2D;
                
                //              hDiff1D->Draw("hist") ;
                
                map_StockageGraphForMulti[Material_i.first][Ionisation_i.first]= TempsGraph;
                cout << map_StockageGraphForMulti[Material_i.first][Ionisation_i.first] << endl;
                
                n = x_TGraph.size();
                TempsGraph_Diff = new TGraphErrors(n,&x_TGraph_Diff[0],&y_TGraph_Diff[0],&ex_TGraph_Diff[0],&ey_TGraph_Diff[0]);
                map_StockageGraphForMulti_Diff[Material_i.first][Ionisation_i.first]= TempsGraph_Diff;
                
                //                 NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
                //         C_temp= new TCanvas(NameCanvas, NameCanvas);
                //         map_StockageTH1DForMulti[Material_i.first][Ionisation_i.first]->Draw();
                
            }
            
        }
        
        
        // ATTENTION  
        
        TMultiGraph* multi; TLegend *leg;
        
        TMultiGraph* multi_general; TLegend *leg_general;
        
        bool General = true;
        
        
        
        
        if(General == true){
            //   Proton range in function of beam energy for
            
            multi_general= new TMultiGraph();
            leg_general = new TLegend(0.1888058,0.6379379,0.3999438,0.8562011,NULL,"brNDC");
            
            
            
            for (auto& Material_i: map_StockageGraphForMulti){
                
                cout << "\n\n   Material " <<   Material_i.first << " Size Material " << map_MATERIAL_IONI_E.size() << endl;
                
                //             cout << map_StockageGraphForMulti["G4_WATER"][78] << endl;
                multi = new TMultiGraph(); 
                
                leg = new TLegend(0.1888058,0.6379379,0.3999438,0.8562011,NULL,"brNDC");
                
                
                TString Material_ST = Material_i.first;
                
                for (auto& Ionisation_i: Material_i.second){
                    cout << "\n     Ionisation " <<   Ionisation_i.first  << endl;
                    
                    float Ionisation_ST = Ionisation_i.first;
                    
                    //       cout << " map_StockageGraphForMulti[Material_i.first][Ionisation_i.second()] " <<  map_StockageGraphForMulti[Material_ST][Ionisation_ST] << endl;
                    
                    TGraphErrors* Temps_graph = ( map_StockageGraphForMulti[Material_ST][Ionisation_ST]);
                    TGraphErrors* Temps_graph_general = (TGraphErrors*)Temps_graph->Clone();
                    
                    TString NameHisto_temps = "";
                    
                    
                    std::ostringstream streamObj3;
                    // Set Fixed -Point Notation
                    streamObj3 << std::fixed;
                    // Set precision to 2 digits
                    streamObj3 << std::setprecision(2);
                    //Add double to stream
                    streamObj3 << Ionisation_ST;
                    
                    // Get string from output string stream
                    std::string strObj3 = streamObj3.str();
                    
                    
                    
                    NameHisto_temps =  Material_i.first +" "+ strObj3 + " eV";
                    
                           if(Study_Material == "Equivalent_Density_Water"){
                        
                           TString Material_TS = Material_i.first;
                        
                        if(Material_TS.Contains("_d_1_00") == true){  
                            
                            leg_general->SetHeader("Density=1.00 & I=75eV ");
                             Material_TS.ReplaceAll("_d_1_00","");
                             NameHisto_temps =  Material_TS;
                            
                        }
                        else if (Material_TS=="Water" ){ NameHisto_temps =  Material_TS;}
                        else {
                            cout << "WARNING NO density 1 !!! " << Material_TS  << endl;
                        }
                    
                           }
                    leg_general->AddEntry(Temps_graph_general, NameHisto_temps, "alp");
                    multi_general->Add(Temps_graph_general,"PL");
                    
                    
                    leg->AddEntry(Temps_graph, NameHisto_temps, "alp");
                    
                    if(Material_ST == "G4_WATER")TGraphG4_Water =  (TGraphErrors*)Temps_graph->Clone();
                    
                    multi->Add(Temps_graph,"PL");
                    
                }
                
                //          multi->Add(TGraphG4_Water,"PLC PMC");
                //          leg->AddEntry(TGraphG4_Water, "G4_WATER 78 eV", "alp");
                
                
                //           multi->Add(Nist_Water,"PLC PMC");
                //          leg->AddEntry(Nist_Water, "NIST 75 eV", "alp");
                
                
                
                TString Title = "Proton range in function of beam energy for " + Material_ST;
                
                multi->SetTitle(Title);
                multi->GetXaxis()->SetTitle("Beam energy [MeV]");
                multi->GetYaxis()->SetTitle("Depth [mm]");
                
                // cosmetic
                double size_graph = 0.04;
                
                multi->GetXaxis()->SetTitleSize(size_graph);
                multi->GetYaxis()->SetTitleSize(size_graph);
                multi->GetXaxis()->SetLabelSize(size_graph);
                multi->GetYaxis()->SetLabelSize(size_graph);
                
                multi->GetXaxis()->SetTitleOffset(1.30);
                multi->GetYaxis()->SetTitleOffset(1.35);
                
                multi->GetXaxis()->SetNdivisions(20, 10, 0, kTRUE);
                multi->GetYaxis()->SetNdivisions(20, 10, 0, kTRUE);
                
                multi->GetXaxis()->SetMoreLogLabels(kTRUE);
                multi->GetXaxis()->SetNoExponent(kFALSE);
                
                
                
                if(Print == true){
                
                NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
                C_temp= new TCanvas(NameCanvas, NameCanvas);
                
                multi->Draw("a PLC PMC");
                
                gPad->SetGrid();
                gPad->Modified();
                gPad->Update();
                
                leg->SetBorderSize(1);
                leg->SetTextAlign(22);
                leg->SetTextFont(42);
                leg->SetTextSize(0.025);
                leg->SetLineColor(1);
                leg->SetLineStyle(1);
                leg->SetLineWidth(2);
                leg->SetFillColor(10);
                leg->SetFillStyle(1001);
                leg->Draw();
                }
                //      gPad->SetGrid();
                //     gPad->Modified();
                //     gPad->Update();
                //     
                //         
                //         NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
                //         C_temp= new TCanvas(NameCanvas, NameCanvas);
                
                
                //         hDiff2D->Draw();
                
                
            }
            
            
            
            TString Title = "Proton range in function of beam energy for different material";
            
            multi_general->SetTitle(Title);
            multi_general->GetXaxis()->SetTitle("Beam energy [MeV]");
            multi_general->GetYaxis()->SetTitle("Depth [mm]");
            
            // cosmetic
            double size_graph = 0.04;
            
            multi_general->GetXaxis()->SetTitleSize(size_graph);
            multi_general->GetYaxis()->SetTitleSize(size_graph);
            multi_general->GetXaxis()->SetLabelSize(size_graph);
            multi_general->GetYaxis()->SetLabelSize(size_graph);
            
            multi_general->GetXaxis()->SetTitleOffset(1.30);
            multi_general->GetYaxis()->SetTitleOffset(1.35);
            
            multi_general->GetXaxis()->SetNdivisions(20, 10, 0, kTRUE);
            multi_general->GetYaxis()->SetNdivisions(20, 10, 0, kTRUE);
            
            multi_general->GetXaxis()->SetMoreLogLabels(kTRUE);
            multi_general->GetXaxis()->SetNoExponent(kFALSE);
            
            
            NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
            C_temp= new TCanvas(NameCanvas, NameCanvas);
            
            multi_general->Draw("a PLC PMC");
            
            
            
            gPad->SetGrid();
            gPad->Modified();
            gPad->Update();
            
            
            
            leg_general->SetBorderSize(1);
            leg_general->SetTextAlign(22);
            leg_general->SetTextFont(42);
            leg_general->SetTextSize(0.025);
            leg_general->SetLineColor(1);
            leg_general->SetLineStyle(1);
            leg_general->SetLineWidth(2);
            leg_general->SetFillColor(10);
            leg_general->SetFillStyle(1001);
            leg_general->Draw();
            
            
            
            
            
        }
        
        // NOTE ATTENTION    //***************************************************
        
        
        //         TMultiGraph* multi_general; TLegend *leg_general;
        int CountForMarkerSytle = -1;
        
        bool General2 = true;
        if(General2 == true){
            
            multi_general= new TMultiGraph();
            leg_general = new TLegend(0.1888058,0.6379379,0.3999438,0.8562011,NULL,"brNDC");
            
            for (auto& Material_i: map_StockageGraphForMulti_Diff){
                
                cout << "\n\n   Material " <<   Material_i.first << " Size Material " << map_MATERIAL_IONI_E.size() << endl;
                
                //             cout << map_StockageGraphForMulti["G4_WATER"][78] << endl;
                multi = new TMultiGraph(); 
                
                leg = new TLegend(0.1888058,0.6379379,0.3999438,0.8562011,NULL,"brNDC");
                
                
                TString Material_ST = Material_i.first;
                
                for (auto& Ionisation_i: Material_i.second){
                    cout << "\n     Ionisation " <<   Ionisation_i.first  << endl;
                    
                    float Ionisation_ST = Ionisation_i.first;
                    
                    //       cout << " map_StockageGraphForMulti[Material_i.first][Ionisation_i.second()] " <<  map_StockageGraphForMulti[Material_ST][Ionisation_ST] << endl;
                    
                    TGraphErrors* Temps_graph = ( map_StockageGraphForMulti_Diff[Material_ST][Ionisation_ST]);
                    
                    Temps_graph->SetMarkerSize(2);
                    
                    CountForMarkerSytle++;
                    Temps_graph->SetMarkerStyle(MarkerStyle_Auto(CountForMarkerSytle));
                    Temps_graph->SetLineStyle(CountForMarkerSytle%10);
                    
                    TGraphErrors* Temps_graph_general = (TGraphErrors*)Temps_graph->Clone();
                    
                    TString NameHisto_temps = "";
                    
                    std::ostringstream streamObj3;
                    // Set Fixed -Point Notation
                    streamObj3 << std::fixed;
                    // Set precision to 2 digits
                    streamObj3 << std::setprecision(2);
                    //Add double to stream
                    streamObj3 << Ionisation_ST;
                    
                    // Get string from output string stream
                    std::string strObj3 = streamObj3.str();
                    
                    
                    
                    
                    NameHisto_temps =  Material_i.first +" "+ strObj3 + " eV";
                    
                    if(Study_Material == "Equivalent_Density_Water"){
                        
                           TString Material_TS = Material_i.first;
                        
                        if(Material_TS.Contains("_d_1_00") == true){  
                            
                            leg_general->SetHeader("Density=1.00 & I=75eV ");
                             Material_TS.ReplaceAll("_d_1_00","");
                             NameHisto_temps =  Material_TS;
                            
                        }
                        else if (Material_TS=="Water" ){ NameHisto_temps =  Material_TS;}
                        else {
                            cout << "WARNING NO density 1 !!! " << Material_TS  << endl;
                        }

                        
                    leg_general->AddEntry(Temps_graph_general, NameHisto_temps, "alp");
                    }
                    else leg_general->AddEntry(Temps_graph_general, NameHisto_temps, "alp");
                    
                    
                    multi_general->Add(Temps_graph_general,"PL");
                    
                    
                    leg->AddEntry(Temps_graph, NameHisto_temps, "alp");
                    
                    //             if(Material_ST == "G4_WATER")TGraphG4_Water =  (TGraphErrors*)Temps_graph->Clone();
                    
                    multi->Add(Temps_graph,"PL");
                    
                }
                
                //          multi->Add(TGraphG4_Water,"PLC PMC");
                //          leg->AddEntry(TGraphG4_Water, "G4_WATER 78 eV", "alp");
                
                
                //           multi->Add(Nist_Water,"PLC PMC");
                //          leg->AddEntry(Nist_Water, "NIST 75 eV", "alp");
                
                
                
                TString Title = "#Delta Gate - NIST Proton range in function of beam energy for " + Material_ST;
                if(SetupRef_map.size()>0) Title = "#Delta Gate - RefGate " + SetupRef_TS +  " Proton range in function of beam energy for different material";
                
                multi->SetTitle(Title);
                multi->GetXaxis()->SetTitle("Beam energy [MeV]");
                multi->GetYaxis()->SetTitle("#Delta [mm]");
                
                // cosmetic
                double size_graph = 0.04;
                
                multi->GetXaxis()->SetTitleSize(size_graph);
                multi->GetYaxis()->SetTitleSize(size_graph);
                multi->GetXaxis()->SetLabelSize(size_graph);
                multi->GetYaxis()->SetLabelSize(size_graph);
                
                multi->GetXaxis()->SetTitleOffset(1.30);
                multi->GetYaxis()->SetTitleOffset(1.35);
                
                multi->GetXaxis()->SetNdivisions(20, 10, 0, kTRUE);
                multi->GetYaxis()->SetNdivisions(20, 10, 0, kTRUE);
                
                multi->GetXaxis()->SetMoreLogLabels(kTRUE);
                multi->GetXaxis()->SetNoExponent(kFALSE);
                
                 if(Print == true){
                NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
                C_temp= new TCanvas(NameCanvas, NameCanvas);
                
                multi->Draw("A PLC PMC");
                
                
                gPad->SetGrid();
                
                gPad->Update();
                
                
                TLine * line2 = new TLine(gPad->GetUxmin(),  0.1 ,gPad->GetUxmax(), 0.1 );
                //       line2 = new TLine(Max,  gPad->GetUymin(),Max,  x_Max_Line );
                
                line2 -> SetLineStyle(9);
                //       line2->IsHorizontal(kTRUE);
                line2->SetLineColor(kBlack);
                line2->SetLineWidth(4);
                line2->Draw();
                
                TLine * line1 = new TLine(gPad->GetUxmin(),  -0.1 ,gPad->GetUxmax(), -0.1 );
                //       line2 = new TLine(Max,  gPad->GetUymin(),Max,  x_Max_Line );
                
                line1 -> SetLineStyle(9);
                //       line2->IsHorizontal(kTRUE);
                line1->SetLineColor(kBlack);
                line1->SetLineWidth(4);
                line1->Draw();
                
                
                leg->SetBorderSize(1);
                leg->SetTextAlign(22);
                leg->SetTextFont(42);
                leg->SetTextSize(0.025);
                leg->SetLineColor(1);
                leg->SetLineStyle(1);
                leg->SetLineWidth(2);
                leg->SetFillColor(10);
                leg->SetFillStyle(1001);
                leg->Draw();
                 }
                
                //      gPad->SetGrid();
                //     gPad->Modified();
                //     gPad->Update();
                //     
                
                //         NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
                //         C_temp= new TCanvas(NameCanvas, NameCanvas);
                
                
                //         hDiff2D->Draw();
                
                
            }
            
            
            
            
            TString Title = "#Delta Gate - NIST Proton range in function of beam energy for different material";
            
            if(SetupRef_map.size()>0) Title = "#Delta Gate - RefGate " + SetupRef_TS +  " Proton range in function of beam energy for different material";
            
            multi_general->SetTitle(Title);
            multi_general->GetXaxis()->SetTitle("Beam energy [MeV]");
            multi_general->GetYaxis()->SetTitle("#Delta [mm]");
            
            // cosmetic
            double size_graph = 0.04;
            
            multi_general->GetXaxis()->SetTitleSize(size_graph);
            multi_general->GetYaxis()->SetTitleSize(size_graph);
            multi_general->GetXaxis()->SetLabelSize(size_graph);
            multi_general->GetYaxis()->SetLabelSize(size_graph);
            
            multi_general->GetXaxis()->SetTitleOffset(1.30);
            multi_general->GetYaxis()->SetTitleOffset(1.35);
            
            multi_general->GetXaxis()->SetNdivisions(20, 10, 0, kTRUE);
            multi_general->GetYaxis()->SetNdivisions(20, 10, 0, kTRUE);
            
            multi_general->GetXaxis()->SetMoreLogLabels(kTRUE);
            multi_general->GetXaxis()->SetNoExponent(kFALSE);
            
            
            NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
            C_temp= new TCanvas(NameCanvas, NameCanvas);
            
            multi_general->Draw("a PLC PMC");
            
            
            
            gPad->SetGrid();
            gPad->Modified();
            gPad->Update();
            
            
            TLine * line2 = new TLine(gPad->GetUxmin(),  0.1 ,gPad->GetUxmax(), 0.1 );
            //       line2 = new TLine(Max,  gPad->GetUymin(),Max,  x_Max_Line );
            
            line2 -> SetLineStyle(9);
            //       line2->IsHorizontal(kTRUE);
            line2->SetLineColor(kBlack);
            line2->SetLineWidth(4);
            line2->Draw();
            
            TLine * line1 = new TLine(gPad->GetUxmin(),  -0.1 ,gPad->GetUxmax(), -0.1 );
            //       line2 = new TLine(Max,  gPad->GetUymin(),Max,  x_Max_Line );
            
            line1 -> SetLineStyle(9);
            //       line2->IsHorizontal(kTRUE);
            line1->SetLineColor(kBlack);
            line1->SetLineWidth(4);
            line1->Draw();
            
            
            leg_general->SetBorderSize(1);
            leg_general->SetTextAlign(22);
            leg_general->SetTextFont(42);
            leg_general->SetTextSize(0.025);
            leg_general->SetLineColor(1);
            leg_general->SetLineStyle(1);
            leg_general->SetLineWidth(2);
            leg_general->SetFillColor(10);
            leg_general->SetFillStyle(1001);
            leg_general->Draw();
            
            //      gPad->SetGrid();
            //     gPad->Modified();
            //     gPad->Update();
            //     
            
            
        }
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        // DIff mais avec TH2D
        
        TH2D* Temps_histo1;
        TH2D* G4_WaterSave;
        TH2D *Cadrehisto ;
        
        bool Print = false;
        
        if(Print == true){
            
            /* = new TH1D ("Cadrehisto", "Diff ; Beam energy [MeV]; #Delta [mm] ", 70, 0, 70);
             *         	 Cadrehisto->GetYaxis()->SetRangeUser(-1,1);
             *		    
             *		    
             *		    
             *             NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
             *        C_temp= new TCanvas(NameCanvas, NameCanvas);
             */
            //         Cadrehisto->Draw("hist axis");
            
            NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
            C_temp= new TCanvas(NameCanvas, NameCanvas);
            //         map_StockageTH1DForMulti["G4_WATER"][80]->Draw();
            
            for (auto& Material_i: map_StockageTH1DForMulti){
                
                TH2D *Cadrehisto = new TH2D ("Cadrehisto", "Diff ; Beam energy [MeV]; #Delta [mm] ", 70, 0, 70,100,-10,10);
                Cadrehisto->GetYaxis()->SetRangeUser(-1,1);
                
                NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
                C_temp= new TCanvas(NameCanvas, NameCanvas);
                
                Cadrehisto->Draw("hist axis");
                
                
                cout << "\n\n   Material " <<   Material_i.first << " Size Material " << map_MATERIAL_IONI_E.size() << endl;
                
                //             cout << map_StockageGraphForMulti["G4_WATER"][78] << endl;
                multi = new TMultiGraph(); 
                
                leg = new TLegend(0.1888058,0.6379379,0.3999438,0.8562011,NULL,"brNDC");
                
                
                TString Material_ST = Material_i.first;
                
                for (auto& Ionisation_i: Material_i.second){
                    cout << "\n     Ionisation " <<   Ionisation_i.first  << endl;
                    
                    float Ionisation_ST = Ionisation_i.first;
                    
                    cout << " map_StockageGraphForMulti[Material_i.first][Ionisation_i.second()] "<< Material_ST << " " << Ionisation_ST <<  map_StockageGraphForMulti[Material_ST][Ionisation_ST] << endl;
                    
                    Temps_histo1 = map_StockageTH1DForMulti[Material_ST][Ionisation_ST];
                    
                    //               if(Material_ST == "G4_WATER")G4_WaterSave =  (TH1D*)Temps_histo1->Clone();
                    TString NameHisto_temps = "";
                    
                    //               float PotentielIonisation_FT = FloatToString(Ionisation_i.first);
                    
                    //               if(PotentielIonisation_FT == 75.0)
                    NameHisto_temps =  Material_i.first +" "+ Ionisation_ST + " eV";
                    
                    leg->AddEntry(Temps_histo1, NameHisto_temps, "alp");
                    
                    Temps_histo1->Draw("same hist PMC PLC");
                    
                    
                    
                    //             multi->Add(Temps_graph,"PLC PMC");
                    
                }
                
                
                //           if(Material_ST != "G4_WATER")  leg->AddEntry( map_StockageTH1DForMulti["G4_WATER"][80], "G4_WATER 78 eV", "alp");
                //               if(Material_ST != "G4_WATER")   map_StockageTH1DForMulti["G4_WATER"][80]->Draw("same hist  PMC PLC");
                
                //         multi->Draw("a PLC PMC");
                gStyle->SetOptStat(0);
                
                gPad->SetGrid();
                gPad->Modified();
                gPad->Update();
                
                leg->SetBorderSize(1);
                leg->SetTextAlign(22);
                leg->SetTextFont(42);
                leg->SetTextSize(0.025);
                leg->SetLineColor(1);
                leg->SetLineStyle(1);
                leg->SetLineWidth(2);
                leg->SetFillColor(10);
                leg->SetFillStyle(1001);
                leg->Draw();
                
                gPad->SetGrid();
                gPad->Modified();
                gPad->Update();
                
                
                //         NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
                //         C_temp= new TCanvas(NameCanvas, NameCanvas);
                
                
                //         hDiff2D->Draw();
                
                
            }
            
        }
        
        cout << "\n*** Analyse EDepthIoni END ***" << endl;
    } 
    //     map_StockageTH1DForMulti
    
    
    
    //          NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
    //       C_temp= new TCanvas(NameCanvas, NameCanvas);
    //       bool First = true;
    //       TLine * line2 ;
    /*        
     *       for (auto& Histo_i: map_Histo_var_Energy_Z){
     *       
     *               C_temp->Update();
     *           TH1F* h = Histo_i.second;
     *           
     *           float PourcentageMax = 80;
     *           HalfMaxCoords =  constants->FindHalfMaxCoords(h,PourcentageMax);
     *           FWHM = constants->m_FWHM;
     *           Max = constants->fMax;
     *  
     *        cout << "Max BraggPeak " << Max << " " << PourcentageMax << " % Max " << constants->m_Xhigh << " Integrale " << h->Integral()<< endl;
     *       binmax = h->GetMaximumBin();  x_Max_Line = h->GetBinContent(binmax);
     *       
     *       cout << "x_Max_Line " << x_Max_Line << endl;
     *   
     *      
     * //          Histo_i.second->Draw("e same");
     *           if(First == true){Histo_i.second->Draw("e same"); First = false; C_temp->Update();}
     *           else{Histo_i.second->Draw("e same"); C_temp->Update(); }
     *      
     *      C_temp->Update();
     *        line2 = new TLine(Max,  gPad->GetUymin(),Max,  x_Max_Line );
     *      
     *      line2 -> SetLineStyle(9);
     *      line2 -> SetVertical(kTRUE);
     *      line2->SetLineColor(kBlack);
     *      
     *      vec_TLine.push_back(line2);
     *           
     *              C_temp->Modified();
     *             C_temp->Update();
     *             
     * //               line2->Draw("same");
}

for (auto& Line_i: vec_TLine){
    C_temp->Update();
    Line_i->Draw();
    C_temp->Update();
    
}
*/
    
    
    
    
    
    
} // end main


