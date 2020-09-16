
 
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
 

 // Macro include
 
 using namespace std;
 
 //********************** Main Function *************************************************************************************************************************
 
//  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaractérisationDuSetup.C
 
 void Analyse_CaracterisationDuSetup(){
     
                  ICOConstants *const_ICO = new ICOConstants();
    TStopwatch timer;
    timer.Start();
     
     
    bool bool_Save_OutputFile = true; // Si true ecriture du fichier root -> PLus d'histo !
    
     // Maquillage graphique
     DPGAConstants *constants = new DPGAConstants();
     TString NameCanvas = "";
     Int_t Number_temp = 0;
     TCanvas * C_temp = NULL;
     TString NameForGet="";
     
     
     string PathFile = "/media/sf_PartageVbox8/GATE/souris/output/";
     string extension = ".root";
     string extension_txt = ".txt";
     
     std::vector<string> vec_Name_PhaseSpace {"frite_kapton_phasespaceactor", "frite_Kapton_after_phasespaceactor", "frite_Tungsten_before_phasespaceactor", "frite_Tungsten_after_phasespaceactor", "frite_DetecteurCharbel_before_phasespaceactor", "frite_DetecteurCharbel_after_phasespaceactor", "frite_cuivre_before_phasespaceactor", "frite_cuivre_after_phasespaceactor", "frite_colli_before_phasespaceactor" ,"frite_colli_after_phasespaceactor"}; 
     
//     std::vector<string> vec_Name_CutOrder{"ParentID==0","ParentID!=0"};
      std::vector<string> vec_Name_CutOrder{"ParentID==0","ParentID!=0"};
    
      // Fichier save
      TString NameFile = "Analyse_Caracterisation_Output";
      
      TString SaveTxt =  PathFile + NameFile + extension_txt;
      ofstream Fichier_Save(SaveTxt, ios::out | ios::trunc);
    
      TString SaveROOT = PathFile + NameFile + extension;
    TFile *Output_File = new TFile(SaveROOT.Data(), "RECREATE");
    
     TH1F * hNameHisto_Ekin;
     TH2F * hNameHisto_XY;
     TH1F * hZ;
     
     map<TString, map < float , map < TString ,TString >>> map_Result; // TCUt PositionZ Category Value_TS 
     
     int i = 0;
     
     double EntriesForFluence;
     map<TString,double>map_EntriesForFluence;
     float Z_Memory;
     map<TString,float>map_Z_Memory;
     
     float Z_First;  float Z_Last;
     
     cout << "Element Ekin_Mean #pm STD[MeV] EkinMax[MeV] TailleDuChamp X _x_ Y[mm²] Transparence[%] #DeltaEkin" << endl;
     
//     Fichier_Save2 <<  "## Delta_Z TimeCut KernelParameter TriggerName PDGCode PDGKernel_Y PDGKernel_X ProfilVrai ProfilMAR ProfilPDGKernel" << endl ;
//      Fichier_Save2 <<  "## Delta_Z TimeCut KernelParameter TriggerName PDGCode PDGKernel_Y PDGKernel_X ProfilVrai ProfilMAR ProfilPDGKernel" << endl
     
     
     
       Fichier_Save << "## Numero Name_PhaseSpace Cut_Order Histo_Ekin Histo_XY Ekin_Mean Ekine_STD Ekin_Max Ekin_Max_BinAboveZero X Y Z Transparence[%] DeltaEkin[MeV] PathRoot NameROOTFile" << endl;
     
     
     for(auto& NamePhaseSpace: vec_Name_PhaseSpace){
      i++;   
         
     TString CompletePath = PathFile + NamePhaseSpace + extension;
        
         TFile *myFile = TFile::Open(CompletePath.Data());
         TTree* TTMakePhaseSpace = (TTree*) myFile->Get("PhaseSpace");
         
         for(auto& Name_CutOrder: vec_Name_CutOrder){
         
         TString Cut_Order = Name_CutOrder;
         TCut  cut(Cut_Order.Data());
         
         TString NameTTreeDraw = "Z >> hz";
         TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
         hZ =(TH1F*)gDirectory->Get("hz")->Clone("hZ");
         
         float Z = hZ->GetMean();
         if(i == 1) Z = 0;
         
//          (" + constants->IntToString(TotalBin_MAR_Z,0) +","+ constants->IntToString(Bin_Min_MAR_Z,0) +"," +constants->IntToString(Bin_Max_MAR_Z,0) +")"
//             TTree* TTMakePhaseSpace = (TTree*) myFile->Get("PhaseSpace");
         
          NameTTreeDraw = "Ekine >> h1(7000,0,70)";
         TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
         hNameHisto_Ekin =(TH1F*)gDirectory->Get("h1")->Clone("hNameHisto_Ekin");
         
         hNameHisto_Ekin->SetTitle(Cut_Order + " " + NamePhaseSpace);
         hNameHisto_Ekin->SetName("hNameHisto_Ekin_"+Cut_Order + "_" + NamePhaseSpace);
         hNameHisto_Ekin->GetXaxis()->SetTitle("Ekine [MeV]");
         hNameHisto_Ekin->GetYaxis()->SetTitle("Entries");
//          hNameHisto_Ekin->SetLineColor(kRed);
         
         
         
         
//          TCut cut2(Cut_Order.Data());
          NameTTreeDraw = "X:Y >> h2(401,-200,200,401,-200,200)";
         TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
         hNameHisto_XY =(TH2F*)gDirectory->Get("h2")->Clone("hNameHisto_XY");
         
           hNameHisto_XY->SetTitle(Cut_Order + " " + NamePhaseSpace);
           hNameHisto_XY->SetName("hNameHisto_Ekin_"+ Cut_Order + "_" + NamePhaseSpace);
         hNameHisto_XY->GetXaxis()->SetTitle("X [mm]");
         hNameHisto_XY->GetYaxis()->SetTitle("Y [mm]");
         
         
         map_Result[Cut_Order][Z]["Name"]= NamePhaseSpace;
         map_Result[Cut_Order][Z]["Ekine_Mean"]= DoubleToString(hNameHisto_Ekin->GetMean());
         map_Result[Cut_Order][Z]["Ekine_STD"]= DoubleToString(hNameHisto_Ekin->GetStdDev());
         map_Result[Cut_Order][Z]["Ekine_MAX"]= DoubleToString( hNameHisto_Ekin->GetBinCenter((hNameHisto_Ekin->GetMaximumBin())) );
         map_Result[Cut_Order][Z]["Ekine_MAX_Bin"]= DoubleToString( hNameHisto_Ekin->GetBinCenter((hNameHisto_Ekin->FindLastBinAbove())) );
         map_Result[Cut_Order][Z]["Fluence"]= DoubleToString(hNameHisto_Ekin->GetEntries());
         
         
           cout <<  NamePhaseSpace << " " << Cut_Order << " " << map_Result[Cut_Order][Z]["Ekine_Mean"] << " +- "  << map_Result[Cut_Order][Z]["Ekine_STD"] << " "  << map_Result[Cut_Order][Z]["Ekine_MAX"]<< " "  << map_Result[Cut_Order][Z]["Ekine_MAX_Bin"] << " "  << hNameHisto_XY->GetStdDev() << " _x_ " << hNameHisto_XY->GetStdDev(2) << " SizeBin " << hNameHisto_Ekin->GetBinWidth((hNameHisto_Ekin->GetMaximumBin()));
         
         
         if(i == 1) Z_First = Z;
         if(i == vec_Name_PhaseSpace.size()) Z_Last = Z;
           
         if(i%2==0){  
             
//              cout << "map_Result[Cut_Order] " << map_EntriesForFluence[Cut_Order] << endl;
             
             map_Result[Cut_Order][Z]["Transparence"]= DoubleToString((double)hNameHisto_Ekin->GetEntries()/(double)map_EntriesForFluence[Cut_Order]*100);  
             map_Result[Cut_Order][Z]["#DeltaEkin"]= DoubleToString( (( (double)hNameHisto_Ekin->GetMean() - ( ToDouble(map_Result[Cut_Order][map_Z_Memory[Cut_Order]]["Ekine_Mean"].Data())) ) / (double)ToDouble(map_Result[Cut_Order][map_Z_Memory[Cut_Order]]["Ekine_Mean"].Data()) )*100 );
          
             cout << " TestEkin " << hNameHisto_Ekin->GetMean() << "_" << map_Result[Cut_Order][Z_Memory]["Ekine_Mean"] << "_TestEkin " ;
             
          cout << " " << map_Result[Cut_Order][Z]["Transparence"]  << " " << map_Result[Cut_Order][Z]["#DeltaEkin"]   << endl; 
           
  
             
             
        }
        else{ 
            cout << endl;
         map_Result[Cut_Order][Z]["Transparence"]="-1";
         map_Result[Cut_Order][Z]["#DeltaEkin"] = "-1";
        }
        
    Fichier_Save 
     << "#"
     << " " << i 
     << " " << map_Result[Cut_Order][Z]["Name"] 
     << " " << Cut_Order
     << " " << hNameHisto_Ekin->GetName()
     << " " << hNameHisto_XY->GetName()
     << " " << map_Result[Cut_Order][Z]["Ekine_Mean"] 
     << " " << map_Result[Cut_Order][Z]["Ekine_STD"] 
     << " " << map_Result[Cut_Order][Z]["Ekine_MAX"]
     << " " << map_Result[Cut_Order][Z]["Ekine_MAX_Bin"] 
     << " " << hNameHisto_XY->GetStdDev()
     << " " << hNameHisto_XY->GetStdDev(2)
     << " " << Z
     << " " << map_Result[Cut_Order][Z]["Transparence"]
     << " " << map_Result[Cut_Order][Z]["#DeltaEkin"]
     << " " << PathFile
     << " " << NameFile
     << endl ;
        
  
     
     
        
        
         
         map_EntriesForFluence[Cut_Order] = hNameHisto_Ekin->GetEntries();
         
        cout << "map_Result["<<Cut_Order<<"] " << map_EntriesForFluence[Cut_Order] << endl;
        Z_Memory = Z;
         
        map_Z_Memory[Cut_Order] = Z_Memory;
        
                  NameCanvas = const_ICO->NameCanvas(Number_temp, "PDGkernel");
         C_temp= new TCanvas(NameCanvas, NameCanvas);
//          
         hNameHisto_Ekin->DrawNormalized("e");
         
           NameCanvas = const_ICO->NameCanvas(Number_temp, "PDGkernel");
         C_temp= new TCanvas(NameCanvas, NameCanvas);
//          
         hNameHisto_XY->Draw("colz");
         
         
             // SaveData 
          Output_File->Add(hNameHisto_Ekin);
         Output_File->Add(hNameHisto_XY);
         
         
         }
         
//           myFile->Close();
         
//           break;

    
         
     }
     
      Fichier_Save << "#@ ZFirst " << Z_First << " ZLast " <<  Z_Last << endl;
      
      float Total_Transparence = ( ToDouble(map_Result["ParentID==0"][Z_Last]["Fluence"].Data())) / ( ToDouble(map_Result["ParentID==0"][Z_First]["Fluence"].Data()))*100;  
    
      float Total_DeltaEkin =  (( ( ToDouble(map_Result["ParentID==0"][Z_Last]["Ekine_Mean"].Data())) - ( ToDouble(map_Result["ParentID==0"][Z_First]["Ekine_Mean"].Data())) ) / ( ToDouble(map_Result["ParentID==0"][Z_First]["Ekine_Mean"].Data())) )*100 ;
             
          cout<<Z_First << " " <<  Z_Last << " " << Total_Transparence  << " Ekin diff relative " << Total_DeltaEkin  << endl; 
     
//      map<TString, map < float , map < TString ,TString >>> map_Result; // TCUt PositionZ Category Value_TS 
     
      TGraphErrors* TGraph_EKine;
     
     for(auto& TCut: map_Result){
     
         if(TCut.first != "ParentID==0") continue;
         
         cout << "Cut " << TCut.first << endl;
            std::vector<float> x_TGraph;
            std::vector<float> y_TGraph;
            std::vector<float> ex_TGraph;
            std::vector<float> ey_TGraph;
         
         
         for(auto& PositionZ: TCut.second){
             
             x_TGraph.push_back( PositionZ.first/10 );
             y_TGraph.push_back( ToDouble(PositionZ.second["Ekine_Mean"].Data()) );
             ex_TGraph.push_back(0);
             float E_Kin_Std = ToDouble(PositionZ.second["Ekine_STD"].Data());
             ey_TGraph.push_back(E_Kin_Std);
        }
         
         Int_t n = x_TGraph.size();
    
     TGraph_EKine = new TGraphErrors(n,&x_TGraph[0],&y_TGraph[0],&ex_TGraph[0],&ey_TGraph[0]);
             NameCanvas = const_ICO->NameCanvas(Number_temp, "PDGkernel");
         C_temp= new TCanvas(NameCanvas, NameCanvas);
         TGraph_EKine->Draw();
     
     } // TCut
     
      cout << "***Total Timer -> ";timer.Stop(); timer.Print();   timer.Continue();   

    if(bool_Save_OutputFile == true){
     Output_File->Write();
     Output_File->Close();
    }
    
     Fichier_Save.close();
      
      
 } // end main
 
 
