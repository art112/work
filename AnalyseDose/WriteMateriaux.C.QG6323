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

float Materiaux_I (string var )
{

    std::map <string, float > map_Energy_Range_NIST = {{"Air",85.7 },{"Lung", 77.59}, {"Skeleton_yellow_marrow", 65.66}, {"Breast", 71.0}, {"Muscle", 76.40},{"Blood", 77.62}, {"Skeleton_spongiosa", -1}, {"SpineBone", 90.34}, {"Skull", 104.65},{"RibBone", 117.81} };
    
    return map_Energy_Range_NIST[var];
    
    
}



void WriteMateriaux(){
    
             ICOConstants *const_ICO = new ICOConstants();
    TStopwatch timer;
    timer.Start();
    
        string PathFile = "/media/sf_PartageVbox8/Materiaux_Caroline/";
    
    string NameFile = "Image125tissus.db";
  
    
    bool ChangeOriginalFile= true;
    
     map < string , map < string , string >> map_Materiaux_Comment;
     map < string , map < string , float >> map_Materiaux_Composition_value;
    
     
      map < string , map < string , string >> map_Materiaux_Comment_GatedB3;
     map < string , map < string , float >> map_Materiaux_Composition_value_GatedB3;
    
     
    TString NameCanvas = "";
    Int_t Number_temp = 0;
    TCanvas * C_temp = NULL;
    TString NameForGet="";
     
     
     int Test = 0;

    
    
    NameFile = "Image125tissus.db";
     const_ICO->ReadMaterial_Gate( PathFile, NameFile ,  ChangeOriginalFile, map_Materiaux_Comment, map_Materiaux_Composition_value, Test);
    
     NameFile ="GateMaterials3.db";
     const_ICO->ReadMaterial_Gate( PathFile, NameFile ,  ChangeOriginalFile, map_Materiaux_Comment_GatedB3, map_Materiaux_Composition_value_GatedB3, 1);
     
    string PathFile_Out = PathFile;
    string NameFile_Out = "Image125tissus_Corrected.db";
    
    const_ICO->WriteMaterial_Gate( PathFile_Out, NameFile_Out, map_Materiaux_Comment, map_Materiaux_Composition_value, 0);
    
    NameFile_Out = "GateMaterialAfter.db";
    
     const_ICO->WriteMaterial_Gate( PathFile_Out, NameFile_Out, map_Materiaux_Comment_GatedB3, map_Materiaux_Composition_value_GatedB3, Test);
    
 
    
    
     
    cout << "\n Map Materiaux Calcul " << endl;
    
    
    
     map <int, string  > map_Result_Name;
    
     for(auto& x: map_Materiaux_Comment){
         
     
        
         TString Mat = x.first;
        int NumberExtraction_Tissu = extractIntegerWords(x.first);
        if(Mat.Contains("Air"))NumberExtraction_Tissu = -1;
        
           map_Result_Name[NumberExtraction_Tissu]=x.first;
        
//         for(auto& y: x.second){
//             //             cout << "  " << y.first << " " << y.second <<  endl;
//             
//         }
         
                float Zeff = 0;
            float RhoZeff = 0;
         
          for(auto& y_value: map_Materiaux_Composition_value[x.first]){
               Zeff += TMath::Power(const_ICO->Element_Z(y_value.first),3.5) * y_value.second ;
              
              
        }
            
            Zeff =  TMath::Power(Zeff,1/3.5);
            RhoZeff = Zeff *  ToFloat(x.second["density"]);
            
            
            float Calcul_Ionisation_Value; 
            
            if(Mat.Contains("Air")) ;
            else Calcul_Ionisation_Value = const_ICO->Calcul_I_Value_V3(Zeff,1);
            
          
            
             if(Mat.Contains("Air"))Calcul_Ionisation_Value = 85.7; // eV
//              if(Mat== "Tissu0")Calcul_Ionisation_Value = 75.2; //eV
             
             
                         std::ostringstream streamObj3;
                    // Set Fixed -Point Notation
                    streamObj3 << std::fixed;
                    // Set precision to 2 digits
                    streamObj3 << std::setprecision(2);
                    //Add double to stream
                    streamObj3 << Calcul_Ionisation_Value;
                    // Get string from output string stream
                    std::string strObj3 = streamObj3.str();
             
             
             
             x.second["Zeff"]=FloatToString(Zeff);
             x.second["RhoZeff"]=FloatToString(RhoZeff);
             x.second["IonisationPotential"]= strObj3;
             
                cout << Mat << " " <<  x.second["density"] <<  "" <<  x.second["density_Unit"] << " Z " << Zeff << " Rhozeff " << RhoZeff << " I_Value " << Calcul_Ionisation_Value << endl;
             
//               fichier << "/gate/geometry/setIonisationPotential " <<  Mat << " " <<  strObj3 << " eV" << endl;
             
     }
    
    
    
    
    cout << endl;
    
     TH1F *Cadrehisto = new TH1F ("Cadrehisto", "Rho Zeff en fonction de ; #rhoZeff ; I [eV] ", 2700, 0, 27);
      TH1F *Cadrehisto2 = new TH1F ("Cadrehisto2", " Zeff en fonction de ; Zeff ; I [eV] ", 2700, 0, 27);
     TGraphErrors* Nist_Water;
     
           std::vector<float> x_TGraph;
            std::vector<float> y_TGraph;
            std::vector<float> ex_TGraph;
            std::vector<float> ey_TGraph;
      
      
    string Name_Folder_Output= PathFile_Out + "Correction_I_Value.mac" ;
    ofstream fichier (Name_Folder_Output,ios::trunc);// ouvre un nouveau fichier en lecture. On lui associe l'objet: fichier
    
    for(auto& x: map_Result_Name){
        
        string Mat = x.second;
        TString MAT = x.second;
//         cout << Mat << endl;
        
//         cout << map_Materiaux_Comment[Mat]["density"] << " " << map_Materiaux_Comment[Mat]["IonisationPotential"] << endl;
        
//           fichier << "/gate/geometry/setIonisationPotential " <<  Mat << " " <<  map_Materiaux_Comment[Mat]["Zeff"] << " " <<  map_Materiaux_Comment[Mat]["IonisationPotential"] << " eV" << endl;
        
                  fichier << "/gate/geometry/setIonisationPotential " <<  Mat << " " <<  map_Materiaux_Comment[Mat]["IonisationPotential"] << " eV" << endl;
        
          
          
          if(MAT.Contains("Air"));
         else {

//              Cadrehisto->Fill( ToFloat(map_Materiaux_Comment[Mat]["RhoZeff"]), ToFloat(map_Materiaux_Comment[Mat]["IonisationPotential"]) );
          
             x_TGraph.push_back(ToFloat(map_Materiaux_Comment[Mat]["Zeff"]));
             y_TGraph.push_back(ToFloat(map_Materiaux_Comment[Mat]["IonisationPotential"]));
             ex_TGraph.push_back(0);
             ey_TGraph.push_back(ToFloat(map_Materiaux_Comment[Mat]["IonisationPotential"])*0.0);
             Cadrehisto2->Fill( ToFloat(map_Materiaux_Comment[Mat]["Zeff"]), ToFloat(map_Materiaux_Comment[Mat]["IonisationPotential"]) );
         }
    }
    fichier.close();  // on ferme le fichier 
    
    
    string Incertitude_Pourcentage_I_value = "4" ; // %
    
    float Incertitude_Pourcentage_I_value_F = ToFloat(Incertitude_Pourcentage_I_value);
    Incertitude_Pourcentage_I_value_F /= 100;
    
    Name_Folder_Output= PathFile_Out + "Correction_I_Value_I_Min_" + Incertitude_Pourcentage_I_value +   ".mac" ;
    ofstream fichier2 (Name_Folder_Output,ios::trunc);// ouvre un nouveau fichier en lecture. On lui associe l'objet: fichier
    
    for(auto& x: map_Result_Name){
        
        string Mat = x.second;
        TString MAT = x.second;
//         cout << Mat << endl;
        
//         cout << map_Materiaux_Comment[Mat]["density"] << " " << map_Materiaux_Comment[Mat]["IonisationPotential"] << endl;
        
//           fichier << "/gate/geometry/setIonisationPotential " <<  Mat << " " <<  map_Materiaux_Comment[Mat]["Zeff"] << " " <<  map_Materiaux_Comment[Mat]["IonisationPotential"] << " eV" << endl;
        
        fichier2 << "/gate/geometry/setIonisationPotential " <<  Mat << " " <<  ToFloat(map_Materiaux_Comment[Mat]["IonisationPotential"])*(1-Incertitude_Pourcentage_I_value_F) << " eV" << endl;
        
    }
    fichier2.close();  // on ferme le fichier 
    
    
    Name_Folder_Output= PathFile_Out + "Correction_I_Value_I_Max_" + Incertitude_Pourcentage_I_value +   ".mac" ;
    ofstream fichier3 (Name_Folder_Output,ios::trunc);// ouvre un nouveau fichier en lecture. On lui associe l'objet: fichier
    
    for(auto& x: map_Result_Name){
        
        string Mat = x.second;
        TString MAT = x.second;
//         cout << Mat << endl;
        
//         cout << map_Materiaux_Comment[Mat]["density"] << " " << map_Materiaux_Comment[Mat]["IonisationPotential"] << endl;
        
//           fichier << "/gate/geometry/setIonisationPotential " <<  Mat << " " <<  map_Materiaux_Comment[Mat]["Zeff"] << " " <<  map_Materiaux_Comment[Mat]["IonisationPotential"] << " eV" << endl;
        
        fichier3 << "/gate/geometry/setIonisationPotential " <<  Mat << " " <<  ToFloat(map_Materiaux_Comment[Mat]["IonisationPotential"])*(1+Incertitude_Pourcentage_I_value_F) << " eV" << endl;
        
    }
    fichier3.close();  // on ferme le fichier     
    
    
    
    
    
    
    
     Int_t n = x_TGraph.size();
    
     Nist_Water = new TGraphErrors(n,&x_TGraph[0],&y_TGraph[0],&ex_TGraph[0],&ey_TGraph[0]);
    
     cout << "n " << n << " Tgraph " << Nist_Water << endl;
     
    NameCanvas = const_ICO->NameCanvas(Number_temp, "PDGkernel");
    C_temp= new TCanvas(NameCanvas, NameCanvas);
    
    TString Title = "I-value en fonction de Zeffective";
                    
    Nist_Water->SetTitle(Title);
       Nist_Water->GetYaxis()->SetTitle("I-Value [eV]");
    Nist_Water->GetXaxis()->SetTitle("Zeff");
     Nist_Water->SetLineColor(kRed);
    
    
    Nist_Water->Draw("ALP");
   
    const_ICO->Graph_ZeffIvalue->Draw("same CP");
    
    
//     Cadrehisto2->Draw();
       cout << "Search Compare " << endl;
    
    map < int , map < string , map < string , map < string ,string > > > > map_Result_Materiaux;
    
    
    
    
    
    for(auto& x: map_Materiaux_Comment){
        //         cout << x.first << " " << extractIntegerWords(x.first) << endl; 
        
        TString Mat = x.first;
        int NumberExtraction_Tissu = extractIntegerWords(x.first);
        if(Mat.Contains("Air"))NumberExtraction_Tissu = -1;
        
        for(auto& y: x.second){
            //             cout << "  " << y.first << " " << y.second <<  endl;
            
        }
        
        
           float Zeff = 0;
            float RhoZeff = 0;
            
        
        for(auto& x_Gate: map_Materiaux_Comment_GatedB3){
            
            int Diference = 0;
            int DIfferenceWithTolerance = 0;
             
            float NoteForNotSame=0;
            
                Zeff = 0;
             RhoZeff = 0;
            
         
            for(auto& y_value: map_Materiaux_Composition_value[x.first]){
                
                
                //             cout << "  " << y_value.first << " " << y_value.second <<  endl;
                
                //           cout << map_Materiaux_Composition_value_GatedB3[x_Gate.first][y_value.first] << " "<< map_Materiaux_Composition_value[x.first][y_value.first] << endl;
                
//                 cout << "Element " << y_value.first << " Z " << const_ICO->Element_Z(y_value.first) << endl;
                
                Zeff += TMath::Power(const_ICO->Element_Z(y_value.first),3.5) * y_value.second ;
                
                map_Result_Materiaux[NumberExtraction_Tissu][x.first][x_Gate.first][y_value.first]=to_string(TMath::Abs(map_Materiaux_Composition_value_GatedB3[x_Gate.first][y_value.first]- map_Materiaux_Composition_value[x.first][y_value.first]));
                
                
                if(map_Materiaux_Composition_value_GatedB3[x_Gate.first][y_value.first]!= map_Materiaux_Composition_value[x.first][y_value.first]){
                    
                    Diference++;
                    NoteForNotSame += (TMath::Abs(map_Materiaux_Composition_value_GatedB3[x_Gate.first][y_value.first]- map_Materiaux_Composition_value[x.first][y_value.first])/map_Materiaux_Composition_value[x.first][y_value.first]);
                    
                    if( (TMath::Abs(map_Materiaux_Composition_value_GatedB3[x_Gate.first][y_value.first]- map_Materiaux_Composition_value[x.first][y_value.first])/map_Materiaux_Composition_value[x.first][y_value.first]*100)>50
                      ){
                        DIfferenceWithTolerance++;
                        
//                          map_Result_Materiaux[extractIntegerWords(x.first)][x.first][x_Gate.first][map_Materiaux_Composition_value_GatedB3[x_Gate.first]]=;
                        
                    }
                }
//                 || map_Materiaux_Composition_value_GatedB3[x_Gate.first][y_value.first] < 0.0000000000001
                
                
            }
            
//             Zeff = Zeff(1/3.5);
            
            Zeff =  TMath::Power(Zeff,1/3.5);
            
            RhoZeff = Zeff *  ToFloat(x.second["density"]);
            
           
            
            
            if(Diference == 0){
//                 cout << "\nSame ! " << x.first  << " " << x_Gate.first <<  endl;
                
                map_Result_Materiaux[NumberExtraction_Tissu][x.first][x_Gate.first]["SAME"]="TRUE";
//                 map_Result_Materiaux[NumberExtraction_Tissu][x.first][x_Gate.first]["DIFFERENCE"]=to_string(Diference);
            }
            
            if(DIfferenceWithTolerance == 0 && Diference>0 && Diference < ToFloat(map_Materiaux_Comment[x.first]["n"]) ){
//                 cout << "\nNotSame ! " << x.first  << " " << x_Gate.first  << " diff " << Diference << " NoteForNotSame " << NoteForNotSame << endl;
                
                 map_Result_Materiaux[NumberExtraction_Tissu][x.first][x_Gate.first]["NOTEXACTLYSAME"]="TRUE";
                 map_Result_Materiaux[NumberExtraction_Tissu][x.first][x_Gate.first]["DIFFERENCE"]=to_string(Diference);
                 map_Result_Materiaux[NumberExtraction_Tissu][x.first][x_Gate.first]["DIFFERENCEWITHTOLERANCE"]="TRUE";
                 
                 
                    map_Result_Materiaux[NumberExtraction_Tissu][x.first][x_Gate.first]["NoteForNotSame"]=to_string(NoteForNotSame);
                   cout << "    "<<  map_Result_Materiaux[NumberExtraction_Tissu][x.first][x_Gate.first]["NoteForNotSame"] << endl;
            }
            
            
        }
        
         cout << Mat << " " <<  x.second["density"] <<  " " <<  x.second["density_Unit"] << " Z " << Zeff << " Rhozeff " << RhoZeff << " Calcul_I_Value " << const_ICO->Calcul_I_Value(RhoZeff) << endl;
        
         
         
    }
    
    
//         map < int , map < string , map < string , map < string ,string > > > > map_Result_Materiaux;
    
    cout << "\n " << endl;
    
    
      map<float , string > Classement_Note;
    
      map < string , float > map_Change_Ivalue;
      
    for(auto& x: map_Result_Materiaux){
        int Mat_Numero = x.first;
        for(auto& y: x.second){
            string Materiaux_Caroline = y.first;
            
            //             cout << x.first << " " << y.first << endl;
            
            map<float , string > Classement_Note;
            
            
            
            for(auto& z: y.second){
                
                string Materiaux_Gate = z.first;
                
                if(z.second["SAME"]== "TRUE"){ 
                    
                    Classement_Note[0.0]= Materiaux_Gate;
                }
                
                if(z.second["NOTEXACTLYSAME"]== "TRUE" && z.second["DIFFERENCEWITHTOLERANCE"]=="TRUE"){
                    
//                      cout << "    "<<  ToFloat(z.second["NoteForNotSame"]) << " " << z.second["NoteForNotSame"] << endl;
                  if(z.second["NoteForNotSame"] != "")  Classement_Note[ToFloat(z.second["NoteForNotSame"])]= Materiaux_Gate;
                    
                }
                
            } // Mat Gate
            
             for(auto& Result_x: Classement_Note){
    
            cout << "*** Result " <<  Materiaux_Caroline   << "    " << Result_x.second << " " << Result_x.first<< endl;
                 
            
            break;
                 
            }
            
            
            
        } // Mat Caroline
        
        
        
    } // Numero du matériaux
    
    
    
//             if(z.second["SAME"]== "TRUE"){
//                      for(auto& aa: z.second){   
//                      cout << "   *** " << z.first  << "       " << aa.first << " " << aa.second << endl;
//                      }
//                     }
//                     else{
//                      if(z.second["NOTEXACTLYSAME"]== "TRUE" && z.second["DIFFERENCEWITHTOLERANCE"]=="TRUE" ) {
//                            for(auto& aa: z.second){   
//                          cout << "   *** " << z.first  << "       " << aa.first << " " << aa.second << endl;
//                            }
//                      }
//                     }    
    
    
//                for(auto& aa: z.second){   
//                      cout << "   *** " << z.first  << "       " << aa.first << " " << aa.second << endl;
//                      }
    
    
    // Attribution des I potentiel aux tissus
    
//     /gate/geometry/setIonisationPotential ${Target_Material} ${IonisationPotential} eV
    
    
    
    
//         map < int , map < string , map < string , map < string ,string > > > > map_Result_Materiaux;
    
    
//     std::map <int, map <string, float> > & Category_comment
    
    
    // Lectire fichier .db
    

    
    cout << "*** Timer -> ";timer.Stop(); timer.Print();   timer.Continue();   
}
