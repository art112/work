
 
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
 #include "TH1D.h"
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
 
 #include <cassert>
 #include <cmath>
 
 #include "../../Include/DPGAConstants.cc" 
 #include "../../Include/ICOConstants.cc" 
 
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
     if(strncmp(&str.back(),".",1)==0)cout <<"str before " <<str << endl;
     if(strncmp(&str.back(),".",1)==0){str.pop_back(); 
     }
     return str;
 }
 
 int extractIntegerWords(string str) 
 { 
     
     // https://stackoverflow.com/questions/32586048/how-to-extract-a-number-from-string-in-c/32586306
     size_t i = 0;
     for ( ; i < str.length(); i++ ){ if ( isdigit(str[i]) ) break; }
     
     // remove the first chars, which aren't digits
     str = str.substr(i, str.length() - i );
     
     // convert the remaining text to an integer
     int id = atoi(str.c_str());
     
     //     std::cout << id << std::endl;
     return id;
 } 
 
 double IncertitudeSurLaDoseTotale(double Total_Dose_Squared, double Total_Dose, double N_Primaries, int Test =0) 
 { 
     // Nota>ons of CheVy et al., “Repor&ng and analysing sta&s&cal uncertain&es in Monte Carlo-based treatment planning”, 2006
     
     cout << "\n*** BEGIN IncertitudeSurLaDoseTotale " << Total_Dose_Squared << " " << Total_Dose << " "  << N_Primaries << endl; 
     
     
     // Dose au carre
     double somme1 = Total_Dose_Squared / N_Primaries;
     cout << "somme1 " << somme1 << endl;
     // Dose²    
     double somme2 = Total_Dose / N_Primaries;
     double somme2_squared =  somme2 * somme2;
     Double_t somme2_squared_ROOT = TMath::Power(somme2, 2);
     
     // Test Method
     cout << "somme2_squared " << somme2_squared << " somme2_squared_ROOT " << somme2_squared_ROOT << " somme2_squared_ROOT / somme2 " << somme2_squared_ROOT / somme2_squared << " Diff " << somme1 - somme2_squared_ROOT << endl;
     
     // 1/(N-1)
     double NormalisationFactor = (1.0 / (N_Primaries - 1.0));
     
     double Incertitude_Total_Dose = TMath::Sqrt( NormalisationFactor * (somme1 - somme2_squared_ROOT) );
     
     
     // DoseActor
     Incertitude_Total_Dose = sqrt( (1.0/(N_Primaries-1))*( TMath::Abs(Total_Dose_Squared/N_Primaries - pow(Total_Dose/N_Primaries, 2))) )/(Total_Dose/N_Primaries);
     
     
     //      Incertitude_Total_Dose /= Total_Dose;
     //      
     //      Incertitude_Total_Dose *=  N_Primaries;
     
     double IncertitudeDose_Abs =  Total_Dose*Incertitude_Total_Dose;
     
     
     if(Test >0){
         
         cout <<"NormalisationFactor " << NormalisationFactor << endl;
         cout << "Incertitude Relative [0-1] " << Incertitude_Total_Dose << endl;
     }
     
     cout << "Dose Total " << Total_Dose << " [Gy] +- " << IncertitudeDose_Abs << " ("   << Incertitude_Total_Dose*100 << " %)" << endl;
     
     cout << "*** END IncertitudeSurLaDoseTotale ***\n" << endl;
     
     return IncertitudeDose_Abs;
 } 
 
 
 void MaquillageGraphique(TH1D*h){
     
     
     //               x.second["hdose"]->GetXaxis()->SetTitle("z [mm]");
     //    h1->GetXaxis()->SetRange(1,34);
     h->GetXaxis()->SetNdivisions(507);
     h->GetXaxis()->SetLabelFont(42);
     h->GetXaxis()->SetLabelOffset(0.015);
     h->GetXaxis()->SetLabelSize(0.05);
     h->GetXaxis()->SetTitleSize(0.05);
     h->GetXaxis()->SetTitleFont(42);
     h->GetXaxis()->SetTitleOffset(1.1);
     //          h->GetYaxis()->SetTitle("Dose [Gy]");
     //    h1->GetYaxis()->SetRange(1,66);
     h->GetYaxis()->SetNdivisions(507);
     h->GetYaxis()->SetLabelFont(42);
     h->GetYaxis()->SetLabelOffset(0.015);
     h->GetYaxis()->SetTitleOffset(0.85);
     h->GetYaxis()->SetLabelSize(0.05);
     h->GetYaxis()->SetTitleSize(0.05);
     h->GetYaxis()->SetTitleFont(42);
     
     
     
 }
 
 
 void MaquillageGraphique_PaveText(TPaveText * pt){
     
     
     TText *pt_LaTex = NULL;
     
     
     pt->SetBorderSize(0);
     pt->SetFillColor(10);
     pt->SetFillStyle(1001);
     pt->SetLineWidth(1);
     pt->SetTextAlign(11);
     pt->SetTextFont(42);
     pt->SetTextSize(0.04318618); 
     pt_LaTex = pt->AddText("Gate 8.2 Geant4 10.5.0");
     pt_LaTex = pt->AddText("BIC_AllHP");
     pt_LaTex = pt->AddText("N_Primaries 10M");
     // 		 pt->Draw();
     
 }
 
 bool myfunction_int (int i,int j) { return (i<j); }
   bool myfunction_float (float i,float j) { return (i<j); }
 

 void CompileHomogeneite(TH1D* Projection, map < int, map < TString, float >> & map_Results){ 
     int Methode = 0;
     
     // Method_1 Find 80% du max
     Methode++;
     
     float Max_Projection = Projection->GetMaximum();
     
     int BinMin_Projection = (Float_t)Projection->FindFirstBinAbove(Max_Projection*0.8) ;
     int BinMax_Projection = (Float_t)Projection->FindLastBinAbove(Max_Projection*0.8) ;
     
     float RangeMin = Projection->GetBinCenter(BinMin_Projection);
     float RangeMax = Projection->GetBinCenter(BinMax_Projection);
     
     vector<float> vec_Projection;
     for(int i = BinMin_Projection ; i< BinMax_Projection; i++){
         vec_Projection.push_back(Projection->GetBinContent(i));
     }
     std::sort (vec_Projection.begin(), vec_Projection.end(), myfunction_float);
     
     double sum = std::accumulate(vec_Projection.begin(), vec_Projection.end(), 0.0);
     double mean = sum / vec_Projection.size();
     
     double sq_sum = std::inner_product(vec_Projection.begin(), vec_Projection.end(), vec_Projection.begin(), 0.0);
     double stdev = std::sqrt(sq_sum / vec_Projection.size() - mean * mean);
     
//      cout << " myvector.front() " <<  vec_Projection.front() << " myvector.back() " << vec_Projection.back() << endl;
//      cout << "homogénéité " << (vec_Projection.back() - vec_Projection.front())/(vec_Projection.back() + vec_Projection.front())*100 << endl;
     
//      cout << "Max_Projection " << Max_Projection << " BinMin_Projection " << BinMin_Projection << " BinMax_Projection " << BinMax_Projection << " RangeMin " << RangeMin << " RangeMax " << RangeMax<< endl;
     
     map_Results[Methode]["Range_Min"]=RangeMin;
     map_Results[Methode]["Range_Max"]=RangeMax;
     map_Results[Methode]["Amplitude_Max"]=vec_Projection.back();
     map_Results[Methode]["Amplitude_Min"]=vec_Projection.front();
     map_Results[Methode]["Mean"]=mean;
     map_Results[Methode]["STD"]=stdev;
     
     map_Results[Methode]["Homogeneite_1"]= (vec_Projection.back() - vec_Projection.front())/(vec_Projection.back() + vec_Projection.front())*100;
     map_Results[Methode]["Homogeneite_2"]= stdev/mean*100;
     
     // Method_1 Find 80% du max + 
     Methode++;
     
     vec_Projection.clear();
     BinMin_Projection = Projection->FindBin(-1*(RangeMax * 0.80));
     BinMax_Projection = Projection->FindBin(1*(RangeMax * 0.80));
     
      RangeMin = Projection->GetBinCenter(BinMin_Projection);
      RangeMax = Projection->GetBinCenter(BinMax_Projection);
     
     
     for(int i = BinMin_Projection ; i< BinMax_Projection; i++){
         vec_Projection.push_back(Projection->GetBinContent(i));
     }
     std::sort (vec_Projection.begin(), vec_Projection.end(), myfunction_float);
     
      sum = std::accumulate(vec_Projection.begin(), vec_Projection.end(), 0.0);
      mean = sum / vec_Projection.size();
     
      sq_sum = std::inner_product(vec_Projection.begin(), vec_Projection.end(), vec_Projection.begin(), 0.0);
      stdev = std::sqrt(sq_sum / vec_Projection.size() - mean * mean);
     
           map_Results[Methode]["Range_Min"]=RangeMin;
     map_Results[Methode]["Range_Max"]=RangeMax;
     map_Results[Methode]["Amplitude_Max"]=vec_Projection.back();
     map_Results[Methode]["Amplitude_Min"]=vec_Projection.front();
     map_Results[Methode]["Mean"]=mean;
     map_Results[Methode]["STD"]=stdev;
     
     map_Results[Methode]["Homogeneite_1"]= (vec_Projection.back() - vec_Projection.front())/(vec_Projection.back() + vec_Projection.front())*100;
     map_Results[Methode]["Homogeneite_2"]= stdev/mean*100;
     
     vec_Projection.clear();
     
     return;
 } 
 
 
 // Macro include
 
 using namespace std;
 
 //********************** Main Function *************************************************************************************************************************
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaractérisationDuSetup.C
 
//  /media/sf_PartageVbox8/work/AnalyseDose/Macro_ok/Analyse_Multi_Ekin_XY_z_Projet_Zebrafish.C
 // /media/sf_PartageVbox8/work/AnalyseDose/Macro_ok/Analyse_Multi_Ekin_XY_SetupArronax.C
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Multi_Run_Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C
 
 void Analyse_Multi_Ekin_XY_z_Projet_Zebrafish(){
     
     TStopwatch timer;
     timer.Start();
     
     // Maquillage graphique
     DPGAConstants *constants = new DPGAConstants();
     TString NameCanvas = "";
     Int_t Number_temp = 0;
     TCanvas * C_temp =   NULL;
     TCanvas * C_temp_2 = NULL;
     
     TString NameForGet="";
     
     TPaveText * pt = NULL;
     TText *pt_LaTex = NULL;
     
     ICOConstants *const_ICO = new ICOConstants();
     
     bool Farm = true;
     
     TH1F * h_TH1F  ; TH1F* h_TH1F_Clone ;
     TH2F * h_TH2F  ; TH2F* h_TH2F_Clone ;
     TNtuple * nt;
     
     int Save = 1 ;
     TString TitleSave;
     
     ///****** *********** ************//
     string Projet = "z_Projet_Manip_ZebraFish_11_03_20";
     string NameSetup = "";
     string CompleteNameSetup = "";
     ///****** *********** ************//
     bool LoadNtuple =false;
     bool PlotCaseByCase = true;
     
     
     
     //      string PathFile = "/media/sf_PartageVbox8/GATE/All_Simulation/z_Projet_Arronax_Compensator_design/";
     
     string PathFile = "/media/sf_PartageVbox8/GATE/All_Simulation/"+Projet +"/";
     
     if(Farm == true){
         PathFile = "/home/gate/nantes_sps/abongran/All_Results/"+Projet +"/9.0/";
     }
     
     
     string Coef_Event = "1";
     string Unit_Event = "M" ;
     
     //      std::vector<string> vec_Option {"68MeV_0_" , "68MeV_1_","68MeV_0.68_", "v1_68MeV_0.68_"};
     
     
     //      std::vector<string> vec_Option {"v1_68MeV_0_","v1_68MeV_1_","v1_68MeV_1_8.1_","v1_68MeV_0.68_"};
     
     //      std::vector<string> vec_Option {"v1_68MeV_0_","v1_68MeV_0.68_","v1_68MeV_1_"};
     
     std::vector<string> vec_Option {"v1_68MeV_0_"};
     
     if(Farm == true){
         
         vec_Option.clear();
         NameSetup="Cube_G4_PLEXIGLASS_vMZfv2_0.1_Manip_ZebraFish_110320_v1";
         CompleteNameSetup= NameSetup +"_68MeV_0_" ; vec_Option.push_back(CompleteNameSetup);
//          CompleteNameSetup= NameSetup +"_68MeV_0.68_" ; vec_Option.push_back(CompleteNameSetup);
//          CompleteNameSetup= NameSetup +"_68MeV_1_" ; vec_Option.push_back(CompleteNameSetup);
         
                  vec_Option.clear();
         NameSetup="Cube_G4_PLEXIGLASS_vMZfv3_0.1_Manip_ZebraFish_110320_v1";
         CompleteNameSetup= NameSetup +"_68MeV_0_" ; vec_Option.push_back(CompleteNameSetup);
         
     }
     
     bool Etude_Epaisseur_Position = true;
     
     // Eventuellement une extension
     //      std::vector<string> Category_Of_Analyse {"Epaisseur", "Pos"};
     //      std::vector<string> type_Of_Analyse_2 {"Min", "Max"};
     
     
     //      TString  CheckFile =  "mkdir " + PathFile + "output_Analyse_Design_Setup_" + Coef_Event + Unit_Event ;
     //      system(CheckFile.Data());
     
     // Path Output
     if(Farm==true){Projet+="_FARM";}
     
     // Local
     TString PathOutput = "/media/sf_PartageVbox8/Analyse_Results/";
     TString NameFile = NameSetup + "_" +Coef_Event + Unit_Event;     
     TString TypeAnalyse = "Analyse_Ekine";
     
     TString Total_Path = PathOutput +Projet;
     TString CheckFileAnalysis = "mkdir "+ PathOutput  ; system(CheckFileAnalysis.Data());
     CheckFileAnalysis = "mkdir "+ Total_Path  ; system(CheckFileAnalysis.Data());
     Total_Path += "/" + NameFile ; CheckFileAnalysis = "mkdir "+ Total_Path  ; system(CheckFileAnalysis.Data());
     Total_Path += "/" + TypeAnalyse ; CheckFileAnalysis = "mkdir "+ Total_Path  ; system(CheckFileAnalysis.Data());
     
     CheckFileAnalysis = "mkdir "+ Total_Path + "/FitEkine" ; system(CheckFileAnalysis.Data());
     
     //      TString CheckFileAnalysis_rm = "rm "+ Total_Path + "/*" ; system(CheckFileAnalysis_rm.Data());
     
     
     
     std::vector<string> vec_Ref_Case ;
          for(auto& x: vec_Option){
              string Title = "output_Ref_" + x;
              vec_Ref_Case.push_back(Title);
          }
//      //      
//      std::vector<string> vec_Other {"Colli2","Colli4", "Colli4Ext"};
//      for(auto& y: vec_Other){
//          for(auto& x: vec_Option){
//              string Title = "output_"+ y+ "_" + x;
//              vec_Ref_Case.push_back(Title);
//          }
//      }
//      
     //****************************************************** 
     cout << "Partie 1 lecture et Chargement des graphs " << endl;
     //****************************************************** 
     
     map<int, map < TString,TString >> map_Comment_Ref;
     
     map<int ,map< int, map < TString,TString >>> map_Comment_ToObtainResults;
     
     // COnfig Case Cut Type de Graph Graph_Adress
     map<int, map < int, map < TString,TH1F* >>> map_Result_TH1F;
     map<int, map < int, map < TString,TH2F* >>> map_Result_TH2F;
     
     map<int, map < int, map < TString,TNtuple* >>> map_Result_TNtuple;
     
     // NTUPLE
     // map<int, map < int, map < TString,TH2F* >>> map_Result_TH2F;
     
     map<int, map < int, vector <float> > > map_Result_Para_Ekine;
     
     int i_Config = 0; // 68_MeV +- 0 MeV 68MeV +- 0.68 MEv ...
     int CompteurCaseForResult = 0; // each Case has an absolut number
     
     for(int i = 0; i < vec_Ref_Case.size();i++){
         
         i_Config = i;
         
         TString NameInput;
         TString NameCase = vec_Ref_Case[i_Config] + Coef_Event + Unit_Event;
         TString PathInput = PathFile + NameCase;
         
         cout <<"\n*** Read PathInput " << PathInput << endl; 
         //ReadInformation :
         TString totalPathReadInformation =    PathInput+"/SaveHistory.txt";
         const_ICO->LoadSetup_with_RunSystem_SaveHistoryGate(map_Comment_Ref, totalPathReadInformation.Data(), i_Config ,0);
         
         TString File_To_Read = "log";
         totalPathReadInformation =    PathInput+"/" +File_To_Read + ".txt";
         const_ICO->LoadSetup_with_RunSystem_GateLog(map_Comment_Ref, totalPathReadInformation.Data(), i_Config ,0);
         
         
         File_To_Read = "Analyse_Caracterisation_Output";
         totalPathReadInformation =    PathInput+"/" +File_To_Read + ".txt";
         const_ICO->LoadSetup_with_RunSystem_ToObtainResults(map_Comment_ToObtainResults, totalPathReadInformation.Data(), i_Config, CompteurCaseForResult ,1);
         
         cout << "CompteurCaseForResult " << CompteurCaseForResult << endl;
         
         // OPen ROOT file Normaly Only One 
         
         //          /media/sf_PartageVbox8/GATE/All_Simulation/z_Projet_Arronax_Compensator_design/output_Ref_v1_68MeV_0.68_10M/media/sf_PartageVbox8/GATE/All_Simulation/z_Projet_Arronax_Compensator_design/output_Ref_v1_68MeV_0.68_10M/
         
         
         NameInput =   PathInput + "/" + File_To_Read + ".root";
         TFile * MyFile = TFile::Open(NameInput.Data()); 
         if ( MyFile->IsOpen() ) printf("File opened successfully\n");
         
         cout << "Load Data COnfig "<< NameCase << endl;
         
         
         
         if(LoadNtuple ==true){
             
             nt =  (TNtuple*)MyFile->Get("nt_Ekine") ;
             map_Result_TNtuple[i_Config][-1]["nt_Ekine"]=nt;
             nt =  (TNtuple*)MyFile->Get("nt_Ekine") ;
             map_Result_TNtuple[i_Config][-1]["nt_Ekine_Only"]=nt;
             
         }
         
         
         for(auto& x: map_Comment_ToObtainResults[i_Config]){
             
             //TODO Read Root File
             
             int Case = x.first;
             TString RealNameSpace = map_Comment_ToObtainResults[i_Config][Case]["Name_PhaseSpace"];
             cout << RealNameSpace << endl;
             
             TString Open_Condition1 = "/gate/actor/" + RealNameSpace + "/enableZPosition" ;
             TString Open_Condition2 = "/gate/actor/" + RealNameSpace + "/storeAllStep" ;
             
             
             //             cout << "      " << Open_Condition1 << " " << map_Comment_Ref[i_Config][Open_Condition1] << endl;
             //             cout << "      " << Open_Condition2 << " " << map_Comment_Ref[i_Config][Open_Condition2] << endl;
             
             
             
             
             
             //              Histo_Ekin Histo_X Histo_XY Histo_Y
             
             h_TH1F =  (TH1F*)MyFile->Get(x.second["Histo_Ekin"]) ;
             /* h_TH1F_Clone =  (TH1F*)h_TH1F->Clone();*/
             map_Result_TH1F[i_Config][Case]["Histo_Ekin"]=h_TH1F;  
             
             h_TH1F =  (TH1F*)MyFile->Get(x.second["Histo_X"]) ; 
             //              h_TH1F_Clone =  (TH1F*)h_TH1F->Clone();
             map_Result_TH1F[i_Config][Case]["Histo_X"]=h_TH1F;  
             
             h_TH1F =  (TH1F*)MyFile->Get(x.second["Histo_Y"]) ; 
             //              h_TH1F_Clone =  (TH1F*)h_TH1F->Clone();
             map_Result_TH1F[i_Config][Case]["Histo_Y"]=h_TH1F;  
             
             h_TH2F =  (TH2F*)MyFile->Get(x.second["Histo_XY"]) ; 
             //              h_TH2F_Clone =  (TH2F*)h_TH2F->Clone();
             map_Result_TH2F[i_Config][Case]["Histo_XY"]=h_TH2F;  
             
                h_TH2F =  (TH2F*)MyFile->Get(x.second["hXYEkine_Rel"]) ;
              map_Result_TH2F[i_Config][Case]["hXYEkine_Rel"]=h_TH2F;  
             
             if(map_Comment_Ref[i_Config][Open_Condition1]== "true" && map_Comment_Ref[i_Config][Open_Condition2]== "true" ){
                 
                 h_TH2F =  (TH2F*)MyFile->Get(x.second["Histo_ZEkin"]) ; 
                 //              h_TH2F_Clone =  (TH2F*)h_TH2F->Clone();
                 map_Result_TH2F[i_Config][Case]["Histo_ZEkin"]=h_TH2F;  
                 
             }
             
             
             //         map_Result_TH1F
             //         map_Result_TH2F
         }
         
         //         MyFile->Close();
         // fichier.close
         
         cout << "Load Data " << NameCase<< endl;
         
     } // end for vec_Ref_Case.size()
     
     
     
     //****************************************************** 
     cout << "Partie 2 Plot " << endl;
     //****************************************************** 
     
     // PLit Case by Case
     
     vector <TString> vec_TCut;
     vec_TCut.push_back("ParentID==0");
     vec_TCut.push_back("ParentID>=0");
     vec_TCut.push_back("ParentID!=0");
     
      ofstream fichier_Save2(Form("%s%s",Total_Path.Data(),"Results_Homogeneite.txt"), ios::out | ios::trunc);
     fichier_Save2 << "NameCase_TS x_TCut CaseType Projection Range_Min Range_Max Amplitude_Min Amplitude_Max Mean STD Homogeneite_1 Homogeneite_2" << endl;
     
     
     
     if(PlotCaseByCase==true){
         
         for(auto& x_TCut: vec_TCut){
             
             for(int i = 0; i < vec_Ref_Case.size();i++){
                 
                 i_Config = i;
                 
                 vector <int > vec_Search_CaseByCase ;
                 map <string , string > map_Search_Conditions ;
                 
                 
                 TString CutOrder_TS = x_TCut;
                 map_Search_Conditions["Cut_Order"] = CutOrder_TS;
                 
                 
                 
                 //           map_Search_Conditions["Cut_Order"] = "ParentID==0";
                 
                 for(auto& x: map_Comment_ToObtainResults[i_Config]){
                     bool SearchOk = true;
                     for(auto& s: map_Search_Conditions){
                         if(x.second[s.first] == s.second) SearchOk = true;
                         else SearchOk = false;
                     }
                     if(SearchOk == true) vec_Search_CaseByCase.push_back(x.first);  
                 }
                 
                 
                 
                 map_Search_Conditions["Cut_Order"] = CutOrder_TS+"&&X<50&&X>-50&&Y<50&&Y>-50";
                 for(auto& x: map_Comment_ToObtainResults[i_Config]){
                     bool SearchOk = true;
                     for(auto& s: map_Search_Conditions){
                         if(x.second[s.first] == s.second) SearchOk = true;
                         else SearchOk = false;
                     }
                     if(SearchOk == true) vec_Search_CaseByCase.push_back(x.first);  
                 }
                 
                 std::sort (vec_Search_CaseByCase.begin(), vec_Search_CaseByCase.end(), myfunction_int);
                 
                 cout <<"\n*** i_Config " << i_Config << endl; 
                 
                 // LOad Case PLot Case
                 TH1F * h_graph; TH2F * h_graph2;
                 TPaveStats *st;
                 
                 for(auto& Case: vec_Search_CaseByCase){
                     cout << Case << endl;
                     
                     NameCanvas = constants->NameCanvas(Number_temp, vec_Ref_Case[i]);
                     C_temp = new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
                     
                     C_temp->Divide(2,2);
                     
                     C_temp->cd(1);
                     
                     if(gDirectory->FindObject(map_Result_TH2F[i_Config][Case]["Histo_ZEkin"])){
                         
                         h_graph2 =  map_Result_TH2F[i_Config][Case]["Histo_ZEkin"];
                         
                         //              TH1* hc = h_graph2->GetCumulative();
                         // check that c has the "right" contents
                         //                      Double_t* integral = h_graph->GetIntegral();
                         //                              for (Int_t i = 1; i <= hc->GetNbinsX(); ++i) {
                         //                    assert(std::abs(integral[i] * h_graph->GetEntries() - hc->GetBinContent(i)) < 1e-7);
                         //                          }
                         
                         //              h_graph2->GetYaxis()->SetNdivisions(712); 
                         //              h_graph2->GetXaxis()->SetNdivisions(709);
                         h_graph2->GetXaxis()->SetTitleOffset(1.1);
                         h_graph2->GetXaxis()->SetRange(h_graph2->FindFirstBinAbove(),h_graph2->FindLastBinAbove());
                         h_graph2->GetYaxis()->SetRange(h_graph2->FindFirstBinAbove(0,2),h_graph2->FindLastBinAbove(0,2));
                         
                         h_graph2->Draw("CANDLEX(2311)");
                         
                         
                         TString Entries = TString::Format("Entries %.0lf", h_graph2->GetEntries());
                         TString StatHisto = TString::Format("Mean %f #pm %f", h_graph2->GetMean(),h_graph->GetMeanError());
                         TString StatHisto2 = TString::Format("Std Dev %f #pm %f", h_graph2->GetStdDev(),h_graph->GetStdDevError());
                         
                         gStyle->SetOptStat("eMR");
                         
                         //              TPaveStats *ptstats = new TPaveStats(0.2058054,0.6873353,0.4139209,0.8129117,"brNDC");
                         //       TPaveStats *ptstats = new TPaveStats(0.2134327,0.6646904,0.4215481,0.8314394,"brNDC");        
                         
                         st = (TPaveStats*)h_graph->FindObject("stats");
                         st->SetX1NDC(0.2134327); //new x start position
                         st->SetX2NDC(0.4215481); //new x end position
                         st->SetY1NDC(0.6646904); //new x start position
                         st->SetY2NDC(0.8314394); //new x end position
                         
                         //              pt = new TPaveText(0.7060726,0.6975206,0.8782346,0.8602475,"blNDC");
                         //              pt->SetBorderSize(0);
                         //              pt->SetTextAlign(22);
                         //              pt->SetTextFont(62);
                         //              pt->SetTextSize(0.02569373);
                         //              pt->SetLineColor(1);
                         //              pt->SetLineStyle(1);
                         //              pt->SetLineWidth(2);
                         //              pt->SetFillColor(0);
                         //              pt->SetFillStyle(0);
                         //              //       pt_LaTex = pt->AddText(TitleSimulation_1.Data());
                         //              //       pt_LaTex = pt->AddText(TitleSimulation_2.Data());
                         //              //       pt_LaTex = pt->AddText(TitleSimulation_3.Data());
                         //              pt_LaTex = pt->AddText(Entries.Data());
                         //              pt_LaTex = pt->AddText(StatHisto.Data());
                         //              pt_LaTex = pt->AddText(StatHisto2.Data());
                         //              pt->Draw();
                         //              
                         C_temp->Update();
                         
                         //              gPad->SetTicky(0);
                         
                         //              Float_t rightmax = 1.1*hc->GetMaximum();
                         //              Float_t scale = gPad->GetUymax()/rightmax;
                         //              hc->SetLineColor(kRed);
                         //              hc->Scale(scale);
                         //              constants->ScaleMaxAmplitude(hc,gPad->GetUymax());
                         //              hc->Draw("same hist");
                         //              
                         //              //draw an axis on the right side
                         //              TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(), gPad->GetUxmax(), gPad->GetUymax(),0,100,510,"+L");
                         //              axis->SetLineColor(kRed);
                         //              axis->SetLabelColor(kRed);
                         //              axis->Draw();
                         //              
                         gPad->SetGridx();
                         gPad->SetGridy();
                         gPad->Modified();
                         gPad->Update();
                         
                         
                         
                     }
                     else{
                         h_graph =  map_Result_TH1F[i_Config][Case]["Histo_Ekin"];
                         
                         TH1* hc = h_graph->GetCumulative();
                         // check that c has the "right" contents
                         //                      Double_t* integral = h_graph->GetIntegral();
                         //                              for (Int_t i = 1; i <= hc->GetNbinsX(); ++i) {
                         //                    assert(std::abs(integral[i] * h_graph->GetEntries() - hc->GetBinContent(i)) < 1e-7);
                         //                          }
                         
                         TString NameCase_TS = NameCase_PhaseSpace(map_Comment_ToObtainResults[i_Config][Case]["Name_PhaseSpace"]);
                         h_graph->SetTitle(CutOrder_TS+ " " + NameCase_TS);
                         
                         h_graph->GetYaxis()->SetNdivisions(712); 
                         h_graph->GetXaxis()->SetNdivisions(709);
                         h_graph->GetXaxis()->SetTitleOffset(1.1);
                         h_graph->GetXaxis()->SetRange(h_graph->FindFirstBinAbove(),h_graph->FindLastBinAbove());
                         
                         h_graph->Draw("hist");
                         
                         
                         TString Entries = TString::Format("Entries %.0lf", h_graph->GetEntries());
                         TString StatHisto = TString::Format("Mean %f #pm %f", h_graph->GetMean(),h_graph->GetMeanError());
                         TString StatHisto2 = TString::Format("Std Dev %f #pm %f", h_graph->GetStdDev(),h_graph->GetStdDevError());
                         
                         gStyle->SetOptStat("eMR");
                         
                         //              TPaveStats *ptstats = new TPaveStats(0.2058054,0.6873353,0.4139209,0.8129117,"brNDC");
                         //       TPaveStats *ptstats = new TPaveStats(0.2134327,0.6646904,0.4215481,0.8314394,"brNDC");        
                         
                         st = (TPaveStats*)h_graph->FindObject("stats");
                         st->SetX1NDC(0.2134327); //new x start position
                         st->SetX2NDC(0.4215481); //new x end position
                         st->SetY1NDC(0.6646904); //new x start position
                         st->SetY2NDC(0.8314394); //new x end position
                         
                         //              pt = new TPaveText(0.7060726,0.6975206,0.8782346,0.8602475,"blNDC");
                         //              pt->SetBorderSize(0);
                         //              pt->SetTextAlign(22);
                         //              pt->SetTextFont(62);
                         //              pt->SetTextSize(0.02569373);
                         //              pt->SetLineColor(1);
                         //              pt->SetLineStyle(1);
                         //              pt->SetLineWidth(2);
                         //              pt->SetFillColor(0);
                         //              pt->SetFillStyle(0);
                         //              //       pt_LaTex = pt->AddText(TitleSimulation_1.Data());
                         //              //       pt_LaTex = pt->AddText(TitleSimulation_2.Data());
                         //              //       pt_LaTex = pt->AddText(TitleSimulation_3.Data());
                         //              pt_LaTex = pt->AddText(Entries.Data());
                         //              pt_LaTex = pt->AddText(StatHisto.Data());
                         //              pt_LaTex = pt->AddText(StatHisto2.Data());
                         //              pt->Draw();
                         //              
                         C_temp->Update();
                         
                         gPad->SetTicky(0);
                         
                         Float_t rightmax = 1.1*hc->GetMaximum();
                         Float_t scale = gPad->GetUymax()/rightmax;
                         hc->SetLineColor(kRed);
                         hc->Scale(scale);
                         constants->ScaleMaxAmplitude(hc,gPad->GetUymax());
                         hc->Draw("same hist");
                         
                         //draw an axis on the right side
                         TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(), gPad->GetUxmax(), gPad->GetUymax(),0,100,510,"+L");
                         axis->SetLineColor(kRed);
                         axis->SetLabelColor(kRed);
                         axis->Draw();
                         
                         gPad->SetGridx();
                         gPad->SetGridy();
                         gPad->Modified();
                         gPad->Update();
                         
                     }
                     
                     C_temp->cd(2);
                     
//                      h_graph2 =  map_Result_TH2F[i_Config][Case]["Histo_XY"];
                     h_graph2 =  map_Result_TH2F[i_Config][Case]["hXYEkine_Rel"];
                     
                     
                     float DeltaBin = (TMath::Abs((Float_t)h_graph2->FindLastBinAbove()) - TMath::Abs((Float_t)h_graph2->FindFirstBinAbove()))*1.844380403/2;
                     
                     int BinMin_Y = (Float_t)h_graph2->FindFirstBinAbove(0,2) - DeltaBin  ;
                     int BinMax_Y = (Float_t)h_graph2->FindLastBinAbove(0,2) + DeltaBin;
                     cout << "Bin " << BinMin_Y <<  " " << BinMax_Y << endl;
                     
                     //           h_graph2->GetXaxis()->SetRange(BinMin_Y,BinMax_Y);
                     //           h_graph2->GetYaxis()->SetRange(h_graph2->FindFirstBinAbove(0,2),h_graph2->FindLastBinAbove(0,2));
                     
                     
                     double DeltaUser = h_graph2->GetStdDev(2)*4*1.844380403/2;
                     
                     double BinMinUser_Y = h_graph2->GetStdDev(2)*-4 - DeltaUser  ;
                     double BinMaxUser_Y = h_graph2->GetStdDev(2)*4 + DeltaUser ;
                     
                     h_graph2->GetYaxis()->SetRangeUser(h_graph2->GetStdDev(2)*-4,h_graph2->GetStdDev(2)*4);
                     h_graph2->GetXaxis()->SetRangeUser(BinMinUser_Y,BinMaxUser_Y);
                     
                     TString NameCase_TS = NameCase_PhaseSpace(map_Comment_ToObtainResults[i_Config][Case]["Name_PhaseSpace"]);
                     h_graph2->SetTitle(CutOrder_TS+ " " + NameCase_TS);
                     
                     //           GetStdDev
                     //           SetRangeUser
                     
                     
                     h_graph2->Draw("colz");
                     
                     gPad->SetRightMargin(0.1);
                     gStyle->SetOptStat("eMR");
                     
                     //              TPaveStats(0.1840132,0.6873353,0.3812326,0.8602602,"brNDC");
                     
                     TPaveStats * st1 = (TPaveStats*)h_graph2->FindObject("stats");
                     st1->SetX1NDC(0.1840132); //new x start position
                     st1->SetX2NDC(0.3812326); //new x end position
                     st1->SetY1NDC(0.6873353); //new x start position
                     st1->SetY2NDC(0.8602602); //new x end position
                     gPad->Modified();
                     gPad->Update();
                     
                     //              ptstats = new TPaveStats(0.1840132,0.7140975,0.3943079,0.8602602,"brNDC");
                     //             C_temp->Update();
                     //             C_temp->Modified();
                     
                     C_temp->cd(3);
                     
                     TH1D* Projection_X = (TH1D*)h_graph2->ProjectionX("ProjectionX of biny=[1996,2006] [y=-0.55..0.55]",1996,2006)->Clone();
                     //              h_graph =  Projection_X;
                     
                     Projection_X->GetXaxis()->SetNdivisions(712);
                     Projection_X->GetXaxis()->SetLabelOffset(0.015);
                     Projection_X->SetFillColor(38);
                     Projection_X->GetYaxis()->SetTitle("Number of Entries");
                     Projection_X->GetXaxis()->SetRangeUser(BinMinUser_Y/1.844380403,BinMaxUser_Y/1.844380403);
                     Projection_X->SetTitle("ProjectionX of biny=[1996,2006] [y=-0.55..0.55]");
                     Projection_X->Draw();
                     
                                  map  <int, map < TString, float>> map_Results;
             CompileHomogeneite(Projection_X, map_Results);
             
             int Method = 0;
             Method++;
             fichier_Save2 << NameCase_TS 
             << " " << x_TCut 
             << " " << vec_Ref_Case[i]
             <<" X " 
             << Method << " "
             << map_Results[Method]["Range_Min"] << " " 
             << map_Results[Method]["Range_Max"] << " " 
             << map_Results[Method]["Amplitude_Min"] <<  " " 
             << map_Results[Method]["Amplitude_Max"] << " "
             << map_Results[Method]["Mean"] << " "
             << map_Results[Method]["STD"] << " "
             << map_Results[Method]["Homogeneite_1"] << " "
             << map_Results[Method]["Homogeneite_2"] << endl;
             
             Method++;
             fichier_Save2 << NameCase_TS
             << " " << x_TCut
             << " " << vec_Ref_Case[i]
             <<" X " 
             << Method << " "
             << map_Results[Method]["Range_Min"] << " " 
             << map_Results[Method]["Range_Max"] << " " 
             << map_Results[Method]["Amplitude_Min"] <<  " " 
             << map_Results[Method]["Amplitude_Max"] << " "
             << map_Results[Method]["Mean"] << " "
             << map_Results[Method]["STD"] << " "
             << map_Results[Method]["Homogeneite_1"] << " "
             << map_Results[Method]["Homogeneite_2"] << endl;
             
             
                         cout << "map_Results[Methode][Homogeneite_1] " << map_Results[1]["Homogeneite_1"] <<  endl;
             cout << "map_Results[Methode][Homogeneite_2] " << map_Results[1]["Homogeneite_2"] <<  endl;
             
              cout << "map_Results[Methode][Homogeneite_1] " << map_Results[2]["Homogeneite_1"] <<  endl;
             cout << "map_Results[Methode][Homogeneite_2] " << map_Results[2]["Homogeneite_2"] <<  endl;
                     
                     //            map_Result_TH1F[i_Config][Case]["Histo_Y"]->Draw("same hist");
                     //              TPaveText *pt = new TPaveText(0.01,0.9394211,0.6875931,0.995,"blNDC");
                     //              pt->SetName("title");
                     //              pt->SetBorderSize(0);
                     //              pt->SetFillColor(0);
                     //              pt->SetLineWidth(2);
                     //              TText *pt_LaTex = pt->AddText("ProjectionX of biny=[1996,2006] [y=-0.55..0.55]");
                     //              pt->Draw();
                     gStyle->SetOptStat("eMR");
                     st = (TPaveStats*)h_graph->FindObject("stats");
                     //              st->SetX1NDC(0.2134327); //new x start position
                     //              st->SetX2NDC(0.4215481); //new x end position
                     //              st->SetY1NDC(0.6646904); //new x start position
                     //              st->SetY2NDC(0.8314394); //new x end position
                     //                          gPad->Modified();
                     //              gPad->Update();
                     
                     C_temp->cd(4);
                     
                     
                     TH1D* Projection_Y = (TH1D*)h_graph2->ProjectionY("ProjectionY of binx=[1996,2006] [x=-0.55..0.55]",1996,2006)->Clone();
                     //              h_graph =  Projection_X;
                     
                     Projection_Y->GetXaxis()->SetNdivisions(712);
                     Projection_Y->GetXaxis()->SetLabelOffset(0.015);
                     Projection_Y->SetFillColor(38);
                     Projection_Y->GetYaxis()->SetTitle("Number of Entries");
                     Projection_Y->GetXaxis()->SetRangeUser(BinMinUser_Y,BinMaxUser_Y);
                     Projection_Y->SetTitle("ProjectionY of binx=[1996,2006] [x=-0.55..0.55]");
                     Projection_Y->Draw();
                     
                                  map_Results.clear();
             CompileHomogeneite(Projection_Y, map_Results);
             
              Method = 0;
             Method++;
             
             
             
             fichier_Save2 << NameCase_TS 
             << " " << x_TCut
             << " " << vec_Ref_Case[i]
             <<" Y " 
             << Method << " "
             << map_Results[Method]["Range_Min"] << " " 
             << map_Results[Method]["Range_Max"] << " " 
             << map_Results[Method]["Amplitude_Min"] <<  " " 
             << map_Results[Method]["Amplitude_Max"] << " "
             << map_Results[Method]["Mean"] << " "
             << map_Results[Method]["STD"] << " "
             << map_Results[Method]["Homogeneite_1"] << " "
             << map_Results[Method]["Homogeneite_2"] << endl;
             
             Method++;
             fichier_Save2 << NameCase_TS 
             << " " << x_TCut
             << " " << vec_Ref_Case[i]
             <<" Y " 
             << Method << " "
             << map_Results[Method]["Range_Min"] << " " 
             << map_Results[Method]["Range_Max"] << " " 
             << map_Results[Method]["Amplitude_Min"] <<  " " 
             << map_Results[Method]["Amplitude_Max"] << " "
             << map_Results[Method]["Mean"] << " "
             << map_Results[Method]["STD"] << " "
             << map_Results[Method]["Homogeneite_1"] << " "
             << map_Results[Method]["Homogeneite_2"] << endl;
             
                     
                     
                     
                     st = (TPaveStats*)h_graph->FindObject("stats");
                     st->SetLineColor(1);
                     //              st->SetX1NDC(0.2134327); //new x start position
                     //              st->SetX2NDC(0.4215481); //new x end position
                     //              st->SetY1NDC(0.6646904); //new x start position
                     //              st->SetY2NDC(0.8314394); //new x end position
                     gStyle->SetOptStat("eMR");
                     //                          gPad->Modified();
                     //              gPad->Update();
                     
                     //              h_graph->GetXaxis()->SetNdivisions(709);
                     //              h_graph->GetXaxis()->SetLabelOffset(0.015);
                     //              h_graph->GetXaxis()->SetNdivisions(712);
                     //              
                     //              h_graph =  map_Result_TH1F[i_Config][Case]["Histo_Y"];
                     //              h_graph->GetXaxis()->SetRangeUser(BinMinUser_Y,BinMaxUser_Y);
                     //              h_graph->Draw();
                     
                     //                         break;
                     
                     if(Save > 0){
                         TitleSave = C_temp->GetName();
                         C_temp->SaveAs( TString::Format("%s/%s.png",Total_Path.Data(),TitleSave.Data()));
                     }
                     
                 }
                 
             }
             
             
         } // end for TCut
     } // end if PlotCaseByCase
     
     // NTUple
     
     ofstream fichier_Save(Form("%s%s",Total_Path.Data(),"Results.txt"), ios::out | ios::trunc);
     
     
     for(int i = 0; i < vec_Ref_Case.size();i++){
         
         i_Config = i;
         
         vector <int > vec_Search_CaseByCase ;
         vector <int > vec_Search_CaseByCase2 ;
         vector <int > vec_Search_CaseByCase3 ;
         
         map <string , string > map_Search_Conditions ;
         
         
         TString CutOrder_TS = "ParentID==0";
         map_Search_Conditions["Cut_Order"] = CutOrder_TS;
         for(auto& x: map_Comment_ToObtainResults[i_Config]){
             bool SearchOk = true;
             for(auto& s: map_Search_Conditions){
                 if(x.second[s.first] == s.second) SearchOk = true;
                 else SearchOk = false;
             }
             if(SearchOk == true) vec_Search_CaseByCase.push_back(x.first);  
         }
         
         map_Search_Conditions["Cut_Order"] = CutOrder_TS+"&&X<50&&X>-50&&Y<50&&Y>-50";
         for(auto& x: map_Comment_ToObtainResults[i_Config]){
             bool SearchOk = true;
             for(auto& s: map_Search_Conditions){
                 if(x.second[s.first] == s.second) SearchOk = true;
                 else SearchOk = false;
             }
             if(SearchOk == true) vec_Search_CaseByCase.push_back(x.first);  
         }
         
         CutOrder_TS = "ParentID>=0";
         map_Search_Conditions["Cut_Order"] = CutOrder_TS;
         for(auto& x: map_Comment_ToObtainResults[i_Config]){
             bool SearchOk = true;
             for(auto& s: map_Search_Conditions){
                 if(x.second[s.first] == s.second) SearchOk = true;
                 else SearchOk = false;
             }
             if(SearchOk == true) vec_Search_CaseByCase2.push_back(x.first);  
         }
         
         map_Search_Conditions["Cut_Order"] = CutOrder_TS+"&&X<50&&X>-50&&Y<50&&Y>-50";
         for(auto& x: map_Comment_ToObtainResults[i_Config]){
             bool SearchOk = true;
             for(auto& s: map_Search_Conditions){
                 if(x.second[s.first] == s.second) SearchOk = true;
                 else SearchOk = false;
             }
             if(SearchOk == true) vec_Search_CaseByCase2.push_back(x.first);  
         }
         
         CutOrder_TS = "ParentID!=0";
         map_Search_Conditions["Cut_Order"] = CutOrder_TS;
         for(auto& x: map_Comment_ToObtainResults[i_Config]){
             bool SearchOk = true;
             for(auto& s: map_Search_Conditions){
                 if(x.second[s.first] == s.second) SearchOk = true;
                 else SearchOk = false;
             }
             if(SearchOk == true) vec_Search_CaseByCase3.push_back(x.first);  
         }
         
         map_Search_Conditions["Cut_Order"] = CutOrder_TS+"&&X<50&&X>-50&&Y<50&&Y>-50";
         for(auto& x: map_Comment_ToObtainResults[i_Config]){
             bool SearchOk = true;
             for(auto& s: map_Search_Conditions){
                 if(x.second[s.first] == s.second) SearchOk = true;
                 else SearchOk = false;
             }
             if(SearchOk == true) vec_Search_CaseByCase3.push_back(x.first);  
         }
         
         std::sort (vec_Search_CaseByCase.begin(), vec_Search_CaseByCase.end(), myfunction_int);
         std::sort (vec_Search_CaseByCase2.begin(), vec_Search_CaseByCase2.end(), myfunction_int);
         std::sort (vec_Search_CaseByCase3.begin(), vec_Search_CaseByCase3.end(), myfunction_int);
         
         
         cout << vec_Search_CaseByCase.size() << endl;
         cout << vec_Search_CaseByCase2.size() << endl;
         cout << vec_Search_CaseByCase3.size() << endl;
         
         fichier_Save <<"\n*** i_Config " << i_Config << " " << vec_Ref_Case[i_Config] << endl; 
         
         cout <<"\n*** i_Config " << i_Config << " " << vec_Ref_Case[i_Config] << endl;
         
         // LOad Case PLot Case
         TH1F * h_graph; /*TH2F * h_graph2;*/
         TPaveStats *st;
         
         TH1F * h_graph1;  TH1F * h_graph2;  TH1F * h_graph3;
         
         cout << "NameSpace ParentID==0   ParentID>=0    ParentID!=0   " << endl;
         cout << "NameSpace Emax Emean STD Entries Emax Emean STD Entries Emax Emean STD Entries" <<endl;
         
         fichier_Save << "NameSpace ParentID==0   ParentID>=0    ParentID!=0   " << endl;
         fichier_Save << "NameSpace EmeanGaus SigmaGaus Emean STD Entries EmeanGaus SigmaGaus Emean STD Entries EmeanGaus SigmaGaus Emean STD Entries" <<endl;
         
         int i_Case = -1;
         for(auto& Case: vec_Search_CaseByCase){
             
             i_Case+=1;
             
             //              cout << "i_Case " << i_Case << endl;
             
             h_graph = map_Result_TH1F[i_Config][Case]["Histo_Ekin"];
             h_graph2= map_Result_TH1F[i_Config][vec_Search_CaseByCase2[i_Case]]["Histo_Ekin"];
             h_graph3= map_Result_TH1F[i_Config][vec_Search_CaseByCase3[i_Case]]["Histo_Ekin"];
             
             //             cout << "i_Case " << i_Case << endl;
             vector <TH1F*> vec_hrgraph;
             vec_hrgraph.push_back(h_graph);
             vec_hrgraph.push_back(h_graph2);
             vec_hrgraph.push_back(h_graph3);
             
             map<int, map <TString, double> > map_Result_Fit;
             
             //          cout << "i_Case " << i_Case << endl;
             TString NameCase_TS = NameCase_PhaseSpace(map_Comment_ToObtainResults[i_Config][Case]["Name_PhaseSpace"]);
             
             cout << "NameCase_TS " << NameCase_TS << endl;
             
             int i_hgr = -1;
             for(auto& hgr: vec_hrgraph){
                 
                 i_hgr++;
                 
                 double Profil_entries ; TLorentzVector HalfMaxCoords ; double FWHM ; double Max ;int binmax; double x_Max_Line;
                 double     PourcentageMax = 50;
                 
                 HalfMaxCoords =  const_ICO->FindHalfMaxCoords(h_graph,PourcentageMax);
                 FWHM = const_ICO->m_FWHM;
                 Max = const_ICO->fMax;  
                 
                 TString NameClone = "h_d_Clone" + DoubleToString(i_Config);
                 
                 TH1F *h_d_Clone = (TH1F*)hgr->Clone(NameClone);
                 
                 Int_t fitStatus =  h_d_Clone->Fit("gaus","","",HalfMaxCoords.X(),HalfMaxCoords.Y() );
                 
                 cout << "fitStatus " << fitStatus << endl;
                 
                 
                 if(i_hgr==0){
                     
                     cout << "i_hgr " << i_hgr << endl;
                     
                     NameCanvas = constants->NameCanvas(Number_temp, vec_Ref_Case[i_Config]+ NameCase_TS + h_d_Clone->GetTitle() );
                     C_temp = new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
                     
                              h_d_Clone->GetXaxis()->SetRangeUser(HalfMaxCoords.X(),HalfMaxCoords.Y());
                        
                      cout << "i_hgr ENd " << i_hgr << endl;        
                 }
                 
                 
                 
                 
                 if(fitStatus==0){
                     
                     cout << "map_Result_Fit " << map_Result_Fit[i_hgr]["Constant"] << endl;  
                     map_Result_Fit[i_hgr]["Constant"] = h_d_Clone->GetFunction("gaus")->GetParameter(0); 
                     map_Result_Fit[i_hgr]["Mean"] =  h_d_Clone->GetFunction("gaus")->GetParameter(1);
                     map_Result_Fit[i_hgr]["Sigma"] =  h_d_Clone->GetFunction("gaus")->GetParameter(2);
                     
                     if(i_hgr==0){ h_d_Clone->GetXaxis()->SetRangeUser(map_Result_Fit[i_hgr]["Mean"]-6*map_Result_Fit[i_hgr]["Sigma"],map_Result_Fit[i_hgr]["Mean"]+3*map_Result_Fit[i_hgr]["Sigma"]);}
                     
                     //          	TF1 *fb = new TF1("fb","exp([0]+[1]*x)",h_d->GetBinCenter(h_d->FindFirstBinAbove()),h_d->GetBinCenter(h_d->FindLastBinAbove()));
                     //        	fb->SetParameter(0,Constant);
                     // 	     fb->SetParameter(1,Slope);
                     // 	
                     // 	fb->Draw("same E1 P PMC"); 
                     
                 }
                 else{
                     map_Result_Fit[i_hgr]["Constant"] = -1; 
                     map_Result_Fit[i_hgr]["Mean"] =  -1;
                     map_Result_Fit[i_hgr]["Sigma"] =  -1;
                 }
                 
                 if(i_hgr==0){
                     
                     h_d_Clone->Draw("");
                     
                     if(Save > 0){
                         TitleSave = C_temp->GetName();
                         C_temp->SaveAs( TString::Format("%s/FitEkine/%s.png",Total_Path.Data(),TitleSave.Data()));
                     }
                     delete C_temp;
                 }
                 
                 
             }
             
             
             
             
             float SizeBin= 0.005;
             
             
             
             //          cout << NameCase_TS <<" " << h_graph->GetBinCenter(h_graph->GetMaximumBin())-SizeBin  << " " << h_graph->GetMean() << " " << h_graph->GetStdDev() <<  " " << h_graph->GetEntries() << " " << h_graph2->GetBinCenter(h_graph->GetMaximumBin())-SizeBin << " " << h_graph2->GetMean() << " " << h_graph2->GetStdDev() <<  " " << h_graph2->GetEntries() << " " << h_graph3->GetBinCenter(h_graph->GetMaximumBin())-SizeBin << " " << h_graph3->GetMean() << " " << h_graph3->GetStdDev() <<  " " << h_graph3->GetEntries()<< endl;
             //         
             //       
             cout << NameCase_TS <<" " << map_Result_Fit[0]["Mean"] << " " << map_Result_Fit[0]["Sigma"] << " " << h_graph->GetMean() << " " << h_graph->GetStdDev() <<  " " << h_graph->GetEntries() << " " << map_Result_Fit[1]["Mean"] << " " << map_Result_Fit[1]["Sigma"] << " " << " " << h_graph2->GetMean() << " " << h_graph2->GetStdDev() <<  " " << h_graph2->GetEntries() << " " << map_Result_Fit[2]["Mean"] << " " << map_Result_Fit[2]["Sigma"] << " " << " " << h_graph3->GetMean() << " " << h_graph3->GetStdDev() <<  " " << h_graph3->GetEntries()<< endl;
             
             fichier_Save << NameCase_TS <<" " << map_Result_Fit[0]["Mean"] << " " << map_Result_Fit[0]["Sigma"] << " " << h_graph->GetMean() << " " << h_graph->GetStdDev() <<  " " << h_graph->GetEntries() << " " << map_Result_Fit[1]["Mean"] << " " << map_Result_Fit[1]["Sigma"] << " " << " " << h_graph2->GetMean() << " " << h_graph2->GetStdDev() <<  " " << h_graph2->GetEntries() << " " << map_Result_Fit[2]["Mean"] << " " << map_Result_Fit[2]["Sigma"] << " " << " " << h_graph3->GetMean() << " " << h_graph3->GetStdDev() <<  " " << h_graph3->GetEntries()<< endl;
             
         }
         
         
     }
     fichier_Save.close();
     
     //               h_graph->Fit("gauss","","",h_d->GetBinCenter(h_d->FindFirstBinAbove()),h_d->GetBinCenter(h_d->FindLastBinAbove()));
     //                h_graph->Fit("gauss","","",h_d->GetBinCenter(h_d->FindFirstBinAbove()),h_d->GetBinCenter(h_d->FindLastBinAbove()));
     
     
     //                TH1D *h_d_Clone = (TH1D*)h_d->Clone("h_d_Clone");
     //       h_d_Clone->Fit("expo","","",h_d->GetBinCenter(h_d->FindFirstBinAbove()),h_d->GetBinCenter(h_d->FindLastBinAbove()));
     //       //    h_d_Clone->SetLineColor(2);
     //       
     //       if(h_d_Clone->GetEntries() > 10 && xPDGCode != 22 ){
     // 	Double_t Constant = 0; Double_t Slope = 0;
     // 	Double_t Chisquare = 0;
     // 	
     // 	Constant = h_d_Clone->GetFunction("expo")->GetParameter(0); Slope =  h_d_Clone->GetFunction("expo")->GetParameter(1);
     // 	Chisquare =  h_d_Clone->GetFunction("expo")->GetChisquare() /  (double)h_d_Clone->GetFunction("expo")->GetNDF() ;
     // 	
     // 	cout <<  "Chisquare "<< Chisquare << endl;
     // 	
     // 	TF1 *fb = new TF1("fb","exp([0]+[1]*x)",h_d->GetBinCenter(h_d->FindFirstBinAbove()),h_d->GetBinCenter(h_d->FindLastBinAbove()));
     // 	fb->SetParameter(0,Constant);
     // 	fb->SetParameter(1,Slope);
     // 	
     // 	fb->Draw("same E1 P PMC"); 
     
     
     
     
     
     
     
     
     
     // Problème for Loading
     if(LoadNtuple ==true){
         
         TString  Legend;
         
         NameCanvas = constants->NameCanvas(Number_temp, "NTUPLE_ParentID==0");
         C_temp = new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
         
         Legend = "Kapton_in:Kapton_out:Colli_in:Colli_out";
         map_Result_TNtuple[i_Config][-1]["nt_Ekine_Only"]->Draw(Legend,"","PARA");
         
         
         //           NameCanvas = constants->NameCanvas(Number_temp, "NTUPLE ParentID==0");
         //           C_temp = new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
         //          
         //             Legend = "Kapton_in:Kapton_out:Tungsten_in:T_out:IC_in:IC_out:LD_in:LD_out:Colli_in:Colli_out";
         //              map_Result_TNtuple[i_Config][-1]["nt_Ekine_Only"]->Draw(Legend,"","PARA");
         //           
         
         
         
     }
     
     
     // PLit Case by Case
     
     //      for(int i = 0; i < vec_Ref_Case.size();i++){
     //          
     //          i_Config = i;
     //          
     //          vector <int > vec_Search_CaseByCase ;
     //          map <string , string > map_Search_Conditions ;
     //          
     //          map_Search_Conditions["Cut_Order"] = "ParentID==0";
     //          
     //          
     //          
     //          //           map_Search_Conditions["Cut_Order"] = "ParentID==0";
     //          
     //          for(auto& x: map_Comment_ToObtainResults[i_Config]){
     //              bool SearchOk = true;
     //              for(auto& s: map_Search_Conditions){
     //                  if(x.second[s.first] == s.second) SearchOk = true;
     //                  else SearchOk = false;
     //              }
     //              if(SearchOk == true) vec_Search_CaseByCase.push_back(x.first);  
     //          }
     //          
     //          map_Search_Conditions["Cut_Order"] = "ParentID==0&&X<50&&X>-50&&Y<50&&Y>-50";
     //          for(auto& x: map_Comment_ToObtainResults[i_Config]){
     //              bool SearchOk = true;
     //              for(auto& s: map_Search_Conditions){
     //                  if(x.second[s.first] == s.second) SearchOk = true;
     //                  else SearchOk = false;
     //              }
     //              if(SearchOk == true) vec_Search_CaseByCase.push_back(x.first);  
     //          }
     //          
     //          std::sort (vec_Search_CaseByCase.begin(), vec_Search_CaseByCase.end(), myfunction_int);
     //          
     //          cout <<"\n*** i_Config " << i_Config << endl; 
     //          
     //          // LOad Case PLot Case
     //          TH1F * h_graph; TH2F * h_graph2;
     //          
     //          TString TS_Case_For_Para = ""; 
     //          
     //                for(auto& Case: vec_Search_CaseByCase){
     //              cout << Case << endl;
     //              if(TS_Case_For_Para != "") TS_Case_For_Para += ":";
     //              TS_Case_For_Para += Case
     //          }
     //          
     //           TNtuple *nt = new TNtuple("nt","Demo ntuple",TString::Format("%s",TS_Case_For_Para.Data()));
     //          
     //           ;
     //          /*
     //     void parallel_example() {
     //    TNtuple *nt = new TNtuple("nt","Demo ntuple","x:y:z:u:v:w:a:b:c");
     //    for (Int_t i=0; i<3000; i++) {
     //       nt->Fill(   rnd,   rnd,   rnd,    rnd,    rnd,    rnd, rnd, rnd, rnd );
     //       nt->Fill(   s1x,   s1y,   s1z,    s2x,    s2y,    s2z, rnd, rnd, rnd );
     //       nt->Fill(   rnd,   rnd,   rnd,    rnd,    rnd,    rnd, rnd, s3y, rnd );
     //       nt->Fill( s2x-1, s2y-1,   s2z, s1x+.5, s1y+.5, s1z+.5, rnd, rnd, rnd );
     //       nt->Fill(   rnd,   rnd,   rnd,    rnd,    rnd,    rnd, rnd, rnd, rnd );
     //       nt->Fill( s1x+1, s1y+1, s1z+1,  s3x-2,  s3y-2,  s3z-2, rnd, rnd, rnd );
     //       nt->Fill(   rnd,   rnd,   rnd,    rnd,    rnd,    rnd, s3x, rnd, s3z );
     //       nt->Fill(   rnd,   rnd,   rnd,    rnd,    rnd,    rnd, rnd, rnd, rnd );
     //    }
     //          */
     //          
     //      
     
     
     
     
     //****************************************************** 
     cout << "Serie 2 PLot Case by Case Objet by Objet" << endl;
     //****************************************************** 
     
     
     
     //****************************************************** 
     cout << "Serie 3 PLot Case by Case and AllCase compare Ekin " << endl;
     //****************************************************** 
     
     
     
     timer.Stop();
     cout << "**** Multi_Run_Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C , timer" <<endl;
     timer.Print();
     
     
     
 } // end main
 
 
 
 
 
 
 
 
 
 
 
