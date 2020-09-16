
 
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
 
 
 // Macro include
 
 using namespace std;
 
 //********************** Main Function *************************************************************************************************************************
 
 
 
 void Only_Read_Dose(){
     
     // Maquillage graphique
     DPGAConstants *constants = new DPGAConstants();
     TString NameCanvas = "";
     Int_t Number_temp = 0;
     TCanvas * C_temp = NULL;
     TString NameForGet="";
     
     // Unite de base le mm !
     
     
     
     
     
     int Run_ID = 0;
     std::map <int, map <string, float> >Category_comment;
     std::map<int, map < float, map < float, map < float , map < string , float > > > > > map_Run; // Run_ID Z X Y Category(Dose uncertiyty dose ...) Value [mm] unite de base
     
     
     /***********Lecture des fichiers ******************/
     
     
     string PathFile = "/media/sf_PartageVbox8/work/runs_PosCuve/";
     string NameFile = "dose-proton-";
     
      vector <Double_t> Cut_Time; // ns
     
     
     std::vector<string> Type {"Dose"};   
//      std::vector<string> Type {"Dose","Dose-Uncertainty","DoseToWater","DoseToWater-Uncertainty","Edep","Edep-Uncertainty","NbOfHits"};   
     
//      string Type =  "Dose";
    

     
    for (auto& Type_vec: Type){
 
     
//      
     // Type de fichier for() Dose Dose Uncertiyt ...
//      /media/sf_PartageVbox8/work/runs_PosCuve/r
     
     string Name_Folder_Intput= PathFile + "run_" + to_string(rock) + "/" + NameFile + Type_vec + ".txt";
     cout <<"Name Input " <<  Name_Folder_Intput << endl;
     
     ifstream fichier (Name_Folder_Intput,ios::in);// ouvre un nouveau fichier en lecture. On lui associe l'objet: fichier
     
     int nbLignes = 0;
     int Category_comment_Counter=0;
     
     
     int x_bin = 0; int y_bin = 0; int z_bin =0;
     
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
         
         //       cout << s << endl ;
         Int_t Comment=0;
         
         if(s.size()>0){
             
             if(s.at(0) == str){/*cout << s << endl;*/ Comment=1; } // si # -> Mode comment
             if(s.at(1) == str && Comment==1) Comment = 2; // si ## 
             
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
         }
     
     
     if(Comment ==0){
         
         x_bin = 0;
         
          for (auto& x: v_record){
              map_Run[Run_ID][z_bin*Category_comment[Run_ID]["VoxelSize_Z"]][x_bin*Category_comment[Run_ID]["VoxelSize_X"]][y_bin*Category_comment[Run_ID]["VoxelSize_Y"]][Type_vec]= ToDouble(x);
               x_bin+=1;
        }
         
         if( Mode_Only_Z == false) y_bin++;
         if( Mode_Only_Z == true) z_bin ++;
         
         // 	int i_v_record_1 = ToInteger(v_record[0]);
         // 	cout << "Run_Number "<< i_v_record_1 << endl;
         // 	map_Run[i_v_record_1]= v_record;
     }
     
 } // end while
 fichier.close();  // on ferme le fichier 
 
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
 
     
     
     
 /*********************************************End lecture******************************************************************************************************/
 
 
 /**************** Analyse **************************************************************************************************************************************/
 
 Run_ID = 0;
 
 string TypeOfAnalyse;

 TH1F* hDose;
 
 TypeOfAnalyse = "Dose";
 
 // Dose 
   
  NameCanvas = constants->NameCanvas(Number_temp, "PDGkernel");
		 C_temp= new TCanvas(NameCanvas, NameCanvas);
 
   TString NameHisto = "hDose_Z_" + DoubleToString(Run_ID);  ;
   
   TH1F* h = new TH1F(NameHisto.Data(),"h1" , (int)Category_comment[Run_ID]["Resol_Z"], 0, (int)Category_comment[Run_ID]["Matrix_Size_Z"] );
   
   hDose = h;
   
     for (auto& z: map_Run[Run_ID]){
      for (auto& x: z.second){
          for (auto& y: x.second){
              
              hDose->Fill(z.first,y.second["Dose"]);
              
              
//              for (auto& cat: y.second){
//      cout << x.first << " " << y.first << " " << z.first << " " << cat.first << " " << cat.second << endl;
//  }
          }
      }
  }
   
   hDose->Draw("e");
   
 
 
 
 
 
 
 } // end main
