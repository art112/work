
 
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
 
 
 
 void Read_Dose(){
     
     // Maquillage graphique
     DPGAConstants *constants = new DPGAConstants();
     TString NameCanvas = "";
     Int_t Number_temp = 0;
     TCanvas * C_temp = NULL;
     TString NameForGet="";
     
       double Profil_entries ; TLorentzVector HalfMaxCoords ; double FWHM ; double Max ;int binmax; double x_Max_Line;
     
     // **************************************************************
     // Unité de base le mm ! MeV ! 
     // ***************************************************************
     
     int Run_ID = 0;
     std::map <int, map <string, float> >Category_comment;
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
     
     for (auto& id: Run_Number){
         
         
         for (int i = 0; i < NbrSetup; i++){
             map_Run_ID[Compteur_Id][id]=to_string(i+1);
             
             Compteur_Id++;
             //          .push_back(to_string(i));
         }
     }
     
     cout << "********************************************" << endl;
     cout << endl;
     cout << "Compteur ID " << Compteur_Id << endl;
     cout << endl;
     cout << "********************************************" << endl;
     
     for (auto& Run_id: map_Run_ID){
         for (auto& Run_i: Run_id.second){
             
             cout << "#id " << Run_id.first << " #Run " << Run_i.first << " #Setup " << Run_i.second << endl;
             
             Run_ID = Run_id.first;
             
             string Run_ST = Run_i.first;
             string Setup_ST = Run_i.second;
             
             for (auto& Type_vec: Type){
                 
                 
                 //      
                 // Type de fichier for() Dose Dose Uncertiyt ...
                 //      /media/sf_PartageVbox8/work/runs_PosCuve/r
                 
                 string Name_Folder_Intput= PathFile + "run_" + Run_ST  + "/output/Setup_" + Setup_ST + "/" + NameFile + Type_vec + ".txt";
                 cout <<"Name Input " <<  Name_Folder_Intput << endl;
                 
                 ifstream fichier (Name_Folder_Intput,ios::in);// ouvre un nouveau fichier en lecture. On lui associe l'objet: fichier
                 
                 if(!fichier){cout << " L'erreur est humaine, admettre la sienne est surhumain. [Doug Larson]" << endl; continue;}
                 
                 
                 int nbLignes = 0;
                 int Category_comment_Counter=0;
                 
                 
                 int x_bin = 1; int y_bin = 1; int z_bin =1;
                 
                 int Compteur_0_Test = 0;
                 
                 int ComptLegend = 0;
                 bool Mode_Only_Z = false;
                 
                 while (fichier)
                 {
                     nbLignes++;
                     
                     string s;
                     
                     if (!getline( fichier, s )) break;
                     
                     istringstream ss( s );
                     
                     string str = "#";
                     
                     //                            cout << s << endl ;
                     Int_t Comment=0;
                     
                     if(s.size()>0){
                         
                         if(s.at(0) == str){/*cout << s << endl;*/ Comment=1; } // si # -> Mode comment
                         if(s.size()>1){ if(s.at(1) == str && Comment==1) Comment = 2; } // si ## 
                         
                     }
                     else{ 
                         Comment = -1; 
                         z_bin +=1; // cas de ligne vide
                         y_bin = 0;
                     }
                     
                     vector <string> v_record;
                     
                     //       	cout << "s  "<< s << endl;
                     while (ss)
                     {
                         string s;
                         if ((!getline( ss, s, ' ' ))) break;
                         v_record.push_back( s );
                         
                     }
                     
                     
                     if(Comment == 2){ComptLegend ++;}
                     
                     if(Comment == 1){
                         
                         int i_pos = 0;
                         
                         string Category =v_record[1];
                         if(Category =="Matrix") Category += "_Size";
                         string Value = v_record[v_record.size()-1];
                         
                         TString Value_TS = Value;
                         
                         
                         vector <string> v_record_value;
                         
                         if(Value_TS.Contains("(")){
                             Value.erase(Value.begin(),Value.begin()+1);
                             Value.pop_back();
                             
                             istringstream ss_value( Value );
                             while (ss_value)
                             {
                                 string svalue;
                                 if ((!getline( ss_value, svalue, ',' ))) break;
                                 v_record_value.push_back( svalue );
                             }
                             
                             Category_comment[Run_ID][Category+"_X"]=ToDouble(v_record_value[0]);
                             Category_comment[Run_ID][Category+"_Y"]=ToDouble(v_record_value[1]);
                             Category_comment[Run_ID][Category+"_Z"]=ToDouble(v_record_value[2]);
                         }
                         else{
                             
                             Category_comment[Run_ID][Category]=ToDouble(Value);
                         }
                         
                     }
                     
                     if(ComptLegend == 2)
                     {
                         if( (int)Category_comment[Run_ID]["Resol_X"] == 1 && (int)Category_comment[Run_ID]["Resol_Y"] == 1 )  Mode_Only_Z = true;
                         ComptLegend++;
                         cout << "Mode_Only_Z True " << endl;
                     }
                     
                     
                     if(Comment ==0){
                         
                         x_bin = 0;
                         
                         for (auto& x: v_record){
                             map_Run[Run_ID][z_bin][x_bin][y_bin][Type_vec]= ToDouble(x);
                             x_bin+=1;
                             
                             cout << "X " << x_bin*Category_comment[Run_ID]["VoxelSize_X"] << endl;
                             cout << "Y " << y_bin*Category_comment[Run_ID]["VoxelSize_Y"] << endl;
                             cout << "Z " << z_bin * Category_comment[Run_ID]["VoxelSize_Z"] << endl;
                             
                             cout << "Value " << x << endl;
                             
                         }
                         
                         //                       *Category_comment[Run_ID]["VoxelSize_Z"]
                         //                          *Category_comment[Run_ID]["VoxelSize_X"]
                         //                          *Category_comment[Run_ID]["VoxelSize_Y"]
                         
                         if( Mode_Only_Z == false) y_bin++;
                         if( Mode_Only_Z == true){ z_bin+=1;; cout <<"zbin +1 "<< z_bin <<  endl; }
                         
                         // 	int i_v_record_1 = ToInteger(v_record[0]);
                         // 	cout << "Run_Number "<< i_v_record_1 << endl;
                         // 	map_Run[i_v_record_1]= v_record;
                     }
                     
                 } // end while
                 fichier.close();  // on ferme le fichier 
                 
                 
                 
                 cout << "End Read FIle " << endl;
                 
                 
                 for (auto& x: Category_comment[Run_ID]){
                     
                     cout << x.first << " " << x.second << endl;
                 }
                 
                 
                 
                 for (auto& z: map_Run[Run_ID]){
                     
                     for (auto& x: z.second){
                         
                         for (auto& y: x.second){
                             
                             for (auto& cat: y.second){
                                 cout << x.first << " " << y.first << " " << z.first << " " << cat.first << " " << cat.second << endl;
                                 if(cat.second == 0) Compteur_0_Test +=1;
                             }
                         }
                     }
                 }
                 
                 cout << "Compteur_0_Test " << Compteur_0_Test << endl;
                 
             } // ENd for choose Type
             
         }
     }
     
     /*********************************************End lecture******************************************************************************************************/
     
     
     /**************** Analyse **************************************************************************************************************************************/
     
     
     
     /********/
     // *** Verif 
     /********/
     Run_ID = 1;
     
     string TypeOfAnalyse;
     
     TH1F* hDose; TH1F* hTemps;
     
     TypeOfAnalyse = "Dose";
     
     // Dose 
     
     NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
     C_temp= new TCanvas(NameCanvas, NameCanvas);
     
     for (auto& Run_ID_i: Category_comment){
         
         Run_ID = Run_ID_i.first;
         
         TString NameHisto = "hDose_Z_" + DoubleToString(Run_ID);  ;
         
         cout << "Category_comment[Run_ID][Resol_Z] " << Category_comment[Run_ID]["Resol_Z"] << " (int)Category_comment[Run_ID][Matrix_Size_Z] " << (int)Category_comment[Run_ID]["Matrix_Size_Z"] << endl;
         
         
         if(Run_ID ==1){
             TH1F* h = new TH1F(NameHisto.Data(),"h1" , (int)Category_comment[Run_ID]["Resol_Z"], 0- (float)Category_comment[Run_ID]["VoxelSize_Z"]/2, (int)Category_comment[Run_ID]["Matrix_Size_Z"]-(float)Category_comment[Run_ID]["VoxelSize_Z"]/2 );
             
             hDose = h;
         }
         
         
         for (auto& z: map_Run[Run_ID]){
             for (auto& x: z.second){
                 for (auto& y: x.second){
                     
                     //                  hDose->Fill(z.first,y.second["Dose"]);
                     
                     double BinContent =  hDose->GetBinContent(z.first);
                     hDose->SetBinContent(z.first,BinContent + y.second["Dose"]);
                     
                     //                  hDose->Fill(z.first* (Int_t)Category_comment[Run_ID]["Resol_Z"],y.second["Dose"] );
                     
                     cout << z.first << " " << BinContent + y.second["Dose"]  << endl;
                     
                     if(hDose->GetBinError(z.first)>0)  hDose->SetBinError(z.first,0);
                     else hDose->SetBinError(z.first,y.second["Dose-Uncertainty"]);
                     
                     //              for (auto& cat: y.second){
                     //      cout << x.first << " " << y.first << " " << z.first << " " << cat.first << " " << cat.second << endl;
                     //  }
                 }
             }
         }
         
     }
     
     hDose->Draw("hist");
     
     //      hDose->FindHalfMaxCoords(hDose);
     
     // Var Energy
     
     std::map<int, TH1F* > map_Histo_var_Energy; 
     std::vector< TLine* > vec_TLine; 
     
     float Compteur_Energy = 0;
     
     // 
     
     for (auto& Run_ID_i: Category_comment){
         
         Run_ID = Run_ID_i.first;
         cout << "\n\n RUN_ID " << Run_ID << endl;
             TString NameHisto = "hDose_Z_" + DoubleToString(Run_ID);  ;
                     
                     cout << "Category_comment[Run_ID][Resol_Z] " << Category_comment[Run_ID]["Resol_Z"] << " (int)Category_comment[Run_ID][Matrix_Size_Z] " << (int)Category_comment[Run_ID]["Matrix_Size_Z"] << endl;
                     
                     map_Histo_var_Energy[Run_ID] = new TH1F(NameHisto.Data(),NameHisto.Data() , (int)Category_comment[Run_ID]["Resol_Z"], 0, (int)Category_comment[Run_ID]["Matrix_Size_Z"]);
         
         for (auto& z: map_Run[Run_ID]){
             for (auto& x: z.second){
                 for (auto& y: x.second){
                     
                     //                  hDose->Fill(z.first,y.second["Dose"]);
                     hTemps = map_Histo_var_Energy[Run_ID];
                     
                     double BinContent =  hTemps->GetBinContent(z.first);
                     hTemps->SetBinContent(z.first,BinContent + y.second["Dose"]);

                     
                     double error = (y.second["Dose-Uncertainty"]);
                     
                     cout << z.first << " " << BinContent + y.second["Dose"] << " +- " << y.second["Dose-Uncertainty"]<< " % " << error*100 << endl;
                     
                      if(hTemps->GetBinError(z.first)>0)  hTemps->SetBinError(z.first,0);
                      else if (error >100);
                      else hTemps->SetBinError(z.first,y.second["Dose-Uncertainty"]*(BinContent + y.second["Dose"]));
                     
                     //              for (auto& cat: y.second){
                     //      cout << x.first << " " << y.first << " " << z.firdst << " " << cat.first << " " << cat.second << endl;
                     //  }
                 }
             }
         }
         
     }
     
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
           if(First == true){Histo_i.second->Draw("hist same"); First = false; C_temp->Update();}
           else{Histo_i.second->Draw("hist same"); C_temp->Update(); }
      
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
 
 
