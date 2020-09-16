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


// Macro include5

using namespace std;

//********************** Main Function *************************************************************************************************************************



void Etude_Potentiel_Ionisation_Pos_Pic_Lung(){
    
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
    PathFile = "../runs_Potentiel_Ioni/";
    
    
    string NameFile = "dose-proton-";
    
    vector <Double_t> Cut_Time; // ns
    
    std::vector<string> Type {"Dose", "Dose-Uncertainty"};  
    
    //      std::vector<string> Type {"Dose", "Dose-Uncertainty","DoseToWater", "DoseToWater-Uncertainty"};   
    //      std::vector<string> Type {"Dose","Dose-Uncertainty","DoseToWater","DoseToWater-Uncertainty","Edep","Edep-Uncertainty","NbOfHits"};   
    
    //      string Type =  "Dose";
    
    // lecture des setups indiquer le numéro des runs !
    //      std::vector<string> Run_Number {"24","25","26"};   
    std::vector<string> Run_Number {"13","14","15","17"};  
    
    //      std::vector<string> Run_Number {"1","2"};  
    
    int NbrSetup = 9;
    
    int Compteur_Id = 1; // init à 1
    
    
    const_ICO->ReadRunDose( Compteur_Id,  Run_Number, map_Run_ID, NbrSetup , PathFile,  NameFile,  Type , Category_comment,  map_Run);
    
    
    map <int,map <TString, TString>> map_LoadedMultiSetup;
    int Test = 2;
    //      const_ICO->LoadSetup_with_RunSystem(map_LoadedMultiSetup, PathFile,  NameFile, map_Run_ID,Test);
    
    const_ICO->LoadSetup_with_RunSystem_Run(map_LoadedMultiSetup, PathFile,  NameFile, map_Run_ID,Run_Number,Test);
    
    /*********************************************End lecture******************************************************************************************************/
    
    
    /**************** Analyse **************************************************************************************************************************************/
    
    cout << "*** Timer -> ";timer.Stop(); timer.Print();     timer.Continue();
    //      cout << endl;
    // Paramètre des ID_Run      
    
    std::map <TString , TString >  map_Init_For_Compare = {
        { "Actor_1_attachTo", "frite"},
        { "Actor_1_enableDose", "true"},
        { "Actor_1_enableDoseToWater", "true"},
        { "Actor_1_enableEdep", "true"},
        { "Actor_1_enableNumberOfHits", "true"},
        { "Actor_1_enableUncertaintyDose", "true"},
        { "Actor_1_enableUncertaintyDoseToWater", "true"},
        { "Actor_1_enableUncertaintyEdep", "true"},
        { "Actor_1_normaliseDoseToIntegral", "false"},
//         { "Actor_1_setResolution_X", "1"},
//         { "Actor_1_setResolution_Y", "1"},
//         { "Actor_1_setResolution_Z", "500"},
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
//         { "Beam_ene_mono", "68"},
//         { "Beam_ene_mono_Unit", "MeV"},
//         { "Beam_ene_sigma", "0"},
//         { "Beam_ene_sigma_Unit", "MeV"},
        { "Beam_ene_type", "Gauss"},
        { "Beam_shape", "Circle"},
        { "Beam_sigma_Unit", "mm"},
        { "Beam_sigma_x", "0."},
        { "Beam_sigma_y", "0."},
        { "EVENT_NB", "100000"},
        { "Frite_Lenght_Unit", "cm"},
        // { "Frite_Material", "G4_WATER"},
        { "Frite_Placement_Unit", "cm"},
//         { "Frite_Placement_X", "0"},
//         { "Frite_Placement_Y", "0"},
//         { "Frite_Placement_Z", "-2.5"},
//         { "Frite_XLength", "10."},
//         { "Frite_YLength", "10."},
//         { "Frite_ZLength", "5."},
        // { "IonisationPotential", "78"},
        { "Physics", "GateV8_BIC_EMZ.mac"},
//         { "Pos_Cuve_Real_Z", "50"},
        { "SetCutCuve", "0.1"},
        { "System", "Naked.mac"},
//         { "Target_Lenght_Unit", "cm"},
        // { "Target_Material", "G4_WATER"},
//         { "Target_Placement_Ref_Unit", "mm"},
//         { "Target_Placement_Ref_X", "0."},
//         { "Target_Placement_Ref_Y", "0."},
//         { "Target_Placement_Ref_Z", "50"},
//         { "Target_XLength", "10."},
//         { "Target_YLength", "10."},
//         { "Target_ZLength", "20."},
        { "VAR_PARAMETER", "BeamEnergy"}
    };
    
    
    
    //      void CompareRunWithMultiSetup(map <int,map <TString, TString>> &map_LoadedMultiSetup,std::map <TString , TString >  map_Init_For_Compare, std::vector< int > &vec_RunConfig_ok, TString NameConfig = "Default", string Mode = "Default");
    
    std::vector< int > vec_RunConfig_ok;
    
    const_ICO->CompareRunWithMultiSetup(map_LoadedMultiSetup,map_Init_For_Compare, vec_RunConfig_ok,  "PotentielIonisation",  "Default");
    
    // Var Energy
    
    std::map<int, TH1F* > map_Histo_var_Energy_Z; 
    std::vector< TLine* > vec_TLine; 
    
    std::map<int, std::map< TString, TString > > map_Histo_var_Energy_Z_HalfMaxCoords; // Extraction information position half maximum 
    
    
    
    float Compteur_Energy = 0;
    
    // 
    // File hISTO
    
    const_ICO->Histo_var_Energy_Z(  map_Histo_var_Energy_Z,  Category_comment,   map_Run);
    const_ICO->FindMaxHalfCoordinateHisto(map_Histo_var_Energy_Z,map_Histo_var_Energy_Z_HalfMaxCoords,true);
    
    // Graphique Part
    
    std::map<TString, std::map< Float_t , std::map< float ,int > >> map_MATERIAL_IONI_E ; // Material Ionisation Energie
    
    // { "IonisationPotential", "78"},
    // { "Target_Material", "G4_WATER"},
    // { "ENERGY", "70.000000"},
    
    
    for (auto& id: vec_RunConfig_ok){
        
        int ID_Setup = id;
        
        TString MATERIAL = map_LoadedMultiSetup[ID_Setup]["Target_Material"];
        Float_t IonisationPotential = ToFloat(map_LoadedMultiSetup[ID_Setup]["IonisationPotential"].Data());
        float ENERGY_ft =  ToDouble(map_LoadedMultiSetup[ID_Setup]["ENERGY"].Data());
        
        map_MATERIAL_IONI_E[MATERIAL][IonisationPotential][ENERGY_ft]=ID_Setup;
        cout << "id " << id << " MATERIAL " << MATERIAL << " IonisationPotential " << IonisationPotential << " ENERGY_ft " << ENERGY_ft << endl;
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
     std::map<TString,std::map<float,TH1D*>> map_StockageTH1DForMulti;
    
    TGraphErrors * TempsGraph;
    
     TGraphErrors * TGraphG4_Water;
    
       TH1D* hDiff1D;
    
    // Stat
    for (auto& Material_i: map_MATERIAL_IONI_E){
        cout << "\n\n   Material " <<   Material_i.first << " Size Material " << map_MATERIAL_IONI_E.size() << endl;
        for (auto& Ionisation_i: Material_i.second){
            cout << "\n     Ionisation " <<   Ionisation_i.first  << endl;
            
            
            std::vector<Float_t> x_TGraph;
            std::vector<Float_t> y_TGraph;
            std::vector<Float_t> ex_TGraph;
            std::vector<Float_t> ey_TGraph;
            
//              TH2D* hDiff2D; hDiff2D = 
            
             TString NameHisto_temps = "";
            NameHisto_temps = "hDiff_E_"+ Material_i.first + FloatToString(Ionisation_i.first);
            
          hDiff1D= new TH1D (NameHisto_temps.Data(), "Diff Depth par rapport à l iono de ref; Beam energy [MeV]; #Delta [mm] ", 70, 0, 70);
             
            for (auto& Energy_i: Ionisation_i.second){
                cout << "\n           Energy_i " <<   Energy_i.first /*<< " Size Energy_i " << map_MATERIAL_IONI_E[Ionisation_i.first][Energy_i.first].size()*/ << " Setup " << Energy_i.second << endl;
                
                x_TGraph.push_back(Energy_i.first);
                
                float m_Yhigh_80 = 0;
                TString Temps = map_Histo_var_Energy_Z_HalfMaxCoords[Energy_i.second]["m_Xhigh_80"];
                cout << "Temps " << Temps << endl;
                m_Yhigh_80 = ToDouble(Temps.Data());
                
                y_TGraph.push_back(m_Yhigh_80);
                
                ex_TGraph.push_back(0);
                ey_TGraph.push_back(0.1);
                
                cout << "map_Histo_var_Energy_Z_HalfMaxCoords[Energy_i.second][m_Yhigh_80] " << map_Histo_var_Energy_Z_HalfMaxCoords[Energy_i.second]["m_Xhigh_80"] << " " << m_Yhigh_80 << endl;
                
               
                
                hDiff1D->Fill(Energy_i.first,m_Yhigh_80-NIST_EnergyToRange(Energy_i.first));
                
                cout << m_Yhigh_80-NIST_EnergyToRange(Energy_i.first) << endl;
                
                
            }
            Int_t n = x_TGraph.size();
            
            TempsGraph = new TGraphErrors(n,&x_TGraph[0],&y_TGraph[0],&ex_TGraph[0],&ey_TGraph[0]);
            
             map_StockageTH1DForMulti[Material_i.first][Ionisation_i.first]= hDiff1D;
            
//              hDiff1D->Draw("hist") ;
             
            map_StockageGraphForMulti[Material_i.first][Ionisation_i.first]= TempsGraph;
            cout << map_StockageGraphForMulti[Material_i.first][Ionisation_i.first] << endl;
            
                NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
        C_temp= new TCanvas(NameCanvas, NameCanvas);
        map_StockageTH1DForMulti[Material_i.first][Ionisation_i.first]->Draw();
            
        }
        
    }
    
    
    //      cout << map_StockageGraphForMulti["G4_WATER"][78] << endl;
    
    TMultiGraph* multi; TLegend *leg;
    
//     TH2D* hDiff2D; 
    
 
    
    
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
            
            
              TString NameHisto_temps = "";
              
              
            NameHisto_temps =  Material_i.first +" "+ FloatToString(Ionisation_ST) + " eV";
            
            leg->AddEntry(Temps_graph, NameHisto_temps, "alp");
            
            if(Material_ST == "G4_WATER")TGraphG4_Water =  (TGraphErrors*)Temps_graph->Clone();
            
            multi->Add(Temps_graph,"PLC PMC");
            
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
        
     gPad->SetGrid();
    gPad->Modified();
    gPad->Update();
    
        
//         NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
//         C_temp= new TCanvas(NameCanvas, NameCanvas);
        
        
//         hDiff2D->Draw();
        
        
    }
     TH1D* Temps_histo1;
    TH1D* G4_WaterSave;
     TH1D *Cadrehisto ;/* = new TH1D ("Cadrehisto", "Diff ; Beam energy [MeV]; #Delta [mm] ", 70, 0, 70);
         	 Cadrehisto->GetYaxis()->SetRangeUser(-1,1);
		    
		
		    
		    
             NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
        C_temp= new TCanvas(NameCanvas, NameCanvas);
    */
//         Cadrehisto->Draw("hist axis");
    
           NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
        C_temp= new TCanvas(NameCanvas, NameCanvas);
//         map_StockageTH1DForMulti["G4_WATER"][80]->Draw();
    
      for (auto& Material_i: map_StockageTH1DForMulti){
        
            TH1D *Cadrehisto = new TH1D ("Cadrehisto", "Diff ; Beam energy [MeV]; #Delta [mm] ", 70, 0, 70);
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


