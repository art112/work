
 
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
     //   if(strncmp(&str.back(),".",1)==0)cout <<"str before " <<str << endl;
     if(strncmp(&str.back(),".",1)==0){str.pop_back(); 
     }
     return str;
 }
 
 
 // Macro include
 
 using namespace std;
 
 //********************** Main Function *************************************************************************************************************************
 
 //**** ATTENTION
 
 // bUT DE LA macro : COnvertir en graphiques les phases space produits avec la simulation de la ligne + Cube after
 
 // A associer avec /media/sf_PartageVbox8/work/AnalyseDose/Macro_ok/Multi_Run_Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.sh
 
 //**** ATTENTION
 
 
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaractérisationDuSetup.C
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C
 //  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C
 //  /media/sf_PartageVbox8/work/AnalyseDose/Macro_ok/Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C
 //  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C
 
 void Analyse_CaracterisationDuSetup_Collimator(const char* PathFile_CS = "/home/gate/nantes_sps/abongran/All_Results/z_Projet_Arronax_Compensator_design/9.0/output_Ref_vACDv4.3_Ligne_ICO_v2T_68MeV_0.68_10M/" ){
     
     //      /media/sf_PartageVbox8/work/AnalyseDose/Macro_ok/Analyse_CaracterisationDuSetup_Collimator.C
     
     ICOConstants *const_ICO = new ICOConstants();
     TStopwatch timer;
     timer.Start();
     
     bool bool_Save_OutputFile = true; // Si true ecriture du fichier root -> PLus d'histo !
     
     bool Analyse_principal = true;
     bool OnlyKaptonColli = true; 
     
     bool bool_Draw = false;
     bool NTUPLE_BOOL = false;
     
     // Maquillage graphique
     DPGAConstants *constants = new DPGAConstants();
     TString NameCanvas = "";
     Int_t Number_temp = 0;
     TCanvas * C_temp = NULL;
     TString NameForGet="";
     
     string PathFile = PathFile_CS; 
     string PathInput = PathFile;
     
     
     cout << "PathFile "<< PathFile << endl;
     
     //      string PathFile = "/media/sf_PartageVbox8/GATE/souris/output/";
     string extension = ".root";
     string extension_txt = ".txt";
     
     std::vector<string> vec_Name_PhaseSpace { "frite_colli_before_phasespaceactor","frite_colli_after_phasespaceactor","frite_colli_after_phasespaceactor2"}; 
     
     //      vec_Name_PhaseSpace.clear();
     //      
     //           vec_Name_PhaseSpace.push_back("PhaseSpace_Entrance_Cube_After");
     
     
     
     //     std::vector<string> vec_Name_CutOrder{"ParentID==0","ParentID!=0"};
     
     
     
     
     std::vector<string> vec_Name_CutOrder{"ParentID==0","ParentID!=0","ParentID>=0"};
     
     // Fichier save
     TString NameFile = "Analyse_Caracterisation_Output";
     
     TString SaveTxt =  PathFile + NameFile + extension_txt;
     ofstream Fichier_Save(SaveTxt, ios::out | ios::trunc);
     
     TString SaveROOT = PathFile + NameFile + extension;
     TFile *Output_File = new TFile(SaveROOT.Data(), "RECREATE");
     
     
     
     
     map<int, map < TString,TString >> map_Comment_Ref;
     
     int i_Setup = 0;
     
     //ReadInformation :
     TString totalPathReadInformation =    PathInput+"SaveHistory.txt";
     const_ICO->LoadSetup_with_RunSystem_SaveHistoryGate(map_Comment_Ref, totalPathReadInformation.Data(), i_Setup ,0);
     
     TString File_To_Read = "log";
     totalPathReadInformation =    PathInput +File_To_Read + ".txt";
     const_ICO->LoadSetup_with_RunSystem_GateLog(map_Comment_Ref, totalPathReadInformation.Data(), i_Setup ,0);
     
     
     
     TH1F * hNameHisto_Ekin;
     TH2F * hNameHisto_XY;
     TH1F * hZ;
     TH1F * hNameHisto_X;
     TH1F * hNameHisto_Y;
     
       TH2F * hNameHisto_XY_Ekine;
     
     TH2F * hNameHisto_ZEkin;
     
     TH3F * hNameHisto_XYEkin;
     TH3F * hNameHisto_ZXEkin;
     
     //      TH3F * hNameHisto_XYZEkin;
     
//      hZ =new TH1F("h","Histogram title;;Y Axis",100,0,1);
//      hZ->Fill(0);
     
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
     
     
     
     Fichier_Save << "## Numero Name_PhaseSpace Cut_Order Histo_Ekin Histo_XY hXYEkine_Rel Histo_ZEkin hNameHisto_XYEkin hNameHisto_ZXEkin Histo_X Histo_Y Ekin_Mean Ekine_STD Ekin_Max Ekin_Max_BinAboveZero X Y Z Transparence[%] DeltaEkin[MeV] PathRoot NameROOTFile" << endl;
     
     float Z = -1;
     
     
     
     string SizeCube =  map_Comment_Ref[i_Setup]["/gate/Cube_After/geometry/setHeight"].Data();
     
     // Adapte le bin 
     
     // TODO if necessary
     
     // 
     float Import_X_Min = -200; float Import_X_Max = 200; 
     float Import_Y_Min = -200; float Import_Y_Max = 200; 
     float Import_Z_Min = -200; float Import_Z_Max = 200; 
     
     float Import_Ekine_Min = -200; float Import_Ekine_Max = 200; 
     
     int Bin_X = 4001;
     int Bin_Y = 4001;
     int Bin_Z = 4001;
     
      // TODO if necessary
     
     //           TFile *myFile = TFile::Open(NameInput.Data());
     //      TTree* TTMakeMAR = (TTree*) myFile->Get("tree");
     //      
     //      Int_t Nevents = TTMakeMAR->GetEntries();
     //      cout <<"MakeMARFromCrystals TTMakeMAR->GetEntries() " << Nevents << endl;
     //      
     //      TTreeReader myReader(TTMakeMAR);
     //      
     
     //      vector <TString > vec_File ;
     
     
     
     if(Analyse_principal == true){
         
         for(auto& NamePhaseSpace: vec_Name_PhaseSpace){
             i++;   
             
             cout <<"*** NamePhaseSpace " << NamePhaseSpace << endl;
             
             TString CompletePath = PathFile + NamePhaseSpace + extension;
             
             TString RealNameSpace = NamePhaseSpace;
             
             //          vec_File.push_back(CompletePath);
             
             if(NamePhaseSpace == "frite_colli_after_phasespaceactor2" ){
                 string Replace_NamePhaseSpace = "frite_colli_after_phasespaceactor";
                 CompletePath = PathFile + Replace_NamePhaseSpace + extension;
                 RealNameSpace = Replace_NamePhaseSpace;
             }
             TFile *myFile = TFile::Open(CompletePath.Data());
             TTree* TTMakePhaseSpace = (TTree*) myFile->Get("PhaseSpace");
             
                        bool Relecture = true;
             map<string,TH2F*> Results_h_XY_Ekine;
             map<string,string> Results_h_XY_Ekine_Name;
             
             if(Relecture==true){
                 
                 TTreeReader myReader(TTMakePhaseSpace);
                 TTreeReaderValue<float> Ekine_TTR(myReader, "Ekine");
                 TTreeReaderValue<float> X_TTR(myReader, "X");
                 TTreeReaderValue<float> Y_TTR(myReader, "Y");
                 TTreeReaderValue<int> ParentID_TTR(myReader, "ParentID");
                 long Nevents = TTMakePhaseSpace->GetEntries();
                 cout <<"TTMakePhaseSpace->GetEntries() " << Nevents << endl;
                 
                 
                 Results_h_XY_Ekine.clear();
                 
                 for(auto& Name_CutOrder: vec_Name_CutOrder){
                     TString Cut_Order = Name_CutOrder;
                     if(NamePhaseSpace == "frite_colli_after_phasespaceactor2" )Cut_Order += "&&X<50&&X>-50&&Y<50&&Y>-50";
                     
                     hNameHisto_XY_Ekine = new TH2F("hXYEkine_Rel", "(X:Y) * Ekine" , 4001,-200,200,4001,-200,200);
                     hNameHisto_XY_Ekine->SetTitle(Cut_Order + " " + NamePhaseSpace);
                     hNameHisto_XY_Ekine->SetName("hNameHisto_XY_Ekine_Rel_"+Cut_Order + "_" + NamePhaseSpace);
                     hNameHisto_XY_Ekine->GetXaxis()->SetTitle("X [mm]");
                     hNameHisto_XY_Ekine->GetYaxis()->SetTitle("Y [mm]");
                     
                     Results_h_XY_Ekine[Name_CutOrder]=hNameHisto_XY_Ekine;
                     Results_h_XY_Ekine_Name[Name_CutOrder]=hNameHisto_XY_Ekine->GetName();
                     //                    hNameHisto_ZEkin->SetTitle(Cut_Order + " " + NamePhaseSpace);
                 }
                 
                 
                 while ( myReader.Next()) {
                     float X = *X_TTR;
                     float Y = *Y_TTR;
                     float Ekine = *Ekine_TTR;
                     int ParentID = *ParentID_TTR;

                     //                      cout << "X " << X << " Y " << Y << " Ekine " << Ekine << " ParentID " << ParentID << endl;
                     
                     TString Condition_Supp = "&&X<50&&X>-50&&Y<50&&Y>-50";
                     
                     bool Ok_For_Sace = true;
                     
                     if(NamePhaseSpace == "frite_colli_after_phasespaceactor2" ){
                         if(X<50&&X>-50&&Y<50&&Y>-50 );
                         else  Ok_For_Sace = false;
                     } 
                     
                     
                     
                     for(auto& Name_CutOrder: vec_Name_CutOrder){
                         hNameHisto_XY_Ekine= Results_h_XY_Ekine[Name_CutOrder];
                         
                         TString Name_CutOrder_TS = Name_CutOrder;
                         if(Name_CutOrder_TS.Contains("==0") && ParentID == 0){
                             if(Ok_For_Sace==true){hNameHisto_XY_Ekine->Fill(X,Y,Ekine); 
                                 //                              cout << "Fill Ok !" << endl;
                             }
                         }
                         else if (Name_CutOrder_TS.Contains("!=0")&& ParentID != 0){
                             if(Ok_For_Sace==true)hNameHisto_XY_Ekine->Fill(X,Y,Ekine);
                         }    
                         else if (Name_CutOrder_TS.Contains(">=0")&& ParentID >= 0){
                             if(Ok_For_Sace==true)hNameHisto_XY_Ekine->Fill(X,Y,Ekine);    
                         }  
                         else ;
                         
                         Results_h_XY_Ekine[Name_CutOrder]=hNameHisto_XY_Ekine;
                     }
                     
                 } // end while
                 
                 for(auto& Name_CutOrder: vec_Name_CutOrder){
                     
                     if (gDirectory->FindObject(Results_h_XY_Ekine[Name_CutOrder])) {
                         Output_File->Add( Results_h_XY_Ekine[Name_CutOrder]);
                         Output_File->Write();
                         delete Results_h_XY_Ekine[Name_CutOrder];
                     } 
                     // output
                     
                 }
                 
             } // end relecture data for make operation
             
             
             
             for(auto& Name_CutOrder: vec_Name_CutOrder){
                 
                 TString Cut_Order = Name_CutOrder;
                 
                 map <TString, TString > map_Result_Histo;
                 
                 
                 cout <<"   *** Name_CutOrder " << Name_CutOrder << endl;
                 
                 if(NamePhaseSpace == "frite_colli_after_phasespaceactor2" )Cut_Order += "&&X<50&&X>-50&&Y<50&&Y>-50";
                 
                 TCut  cut(Cut_Order.Data());
                 
                 TString NameTTreeDraw = "Z >> hz";
                 //              TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
                 //              hZ =(TH1F*)gDirectory->Get("hz")->Clone("hZ");
                 
                 
                 
                 TString Open_Condition1 = "/gate/actor/" + RealNameSpace + "/enableZPosition" ;
                 TString Open_Condition2 = "/gate/actor/" + RealNameSpace + "/storeAllStep" ;
                 
                 
                 cout << "      " << Open_Condition1 << " " << map_Comment_Ref[i_Setup][Open_Condition1] << endl;
                 cout << "      " << Open_Condition2 << " " << map_Comment_Ref[i_Setup][Open_Condition2] << endl;
                 
                 map_Result_Histo["hNameHisto_ZEkin"]="false";
                 map_Result_Histo["hNameHisto_XYEkin"]="false";
                 map_Result_Histo["hNameHisto_ZXEkin"]="false";
                 
                 if(map_Comment_Ref[i_Setup][Open_Condition1]== "true" && map_Comment_Ref[i_Setup][Open_Condition2]== "true" ){
                     
                     
                     cout << "map_Comment_Ref[i_Setup][Open_Condition1]== true && map_Comment_Ref[i_Setup][Open_Condition2]== true" << endl;
                     // Z:
                     NameTTreeDraw = "Z >> hz";
                     TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
                     hZ =(TH1F*)gDirectory->Get("hz")->Clone("hZ");
                     Z = hZ->GetMean();
                     
                     
                     // Z:Ekine
                     NameTTreeDraw = "Ekine:Z >> hZEkin(500,0,50,7000,0,70)";
                     TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
                     hNameHisto_ZEkin =(TH2F*)gDirectory->Get("hZEkin")->Clone("hNameHisto_ZEkin");
                     
                     hNameHisto_ZEkin->SetTitle(Cut_Order + " " + NamePhaseSpace);
                     hNameHisto_ZEkin->SetName("hNameHisto_ZEkin_"+Cut_Order + "_" + NamePhaseSpace);
                     hNameHisto_ZEkin->GetXaxis()->SetTitle("Z [mm]");
                     hNameHisto_ZEkin->GetYaxis()->SetTitle("Ekine [MeV]");
                     map_Result_Histo["hNameHisto_ZEkin"]=hNameHisto_ZEkin->GetName();
                     
                     cout << "Pb1" << endl;
                     
                     // X:Y:Ekine
                     NameTTreeDraw = "Ekine:Y:X >> hXYEkine";
                     TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
                     hNameHisto_XYEkin =(TH3F*)gDirectory->Get("hXYEkine")->Clone("hNameHisto_XYEkin");
                     
                     hNameHisto_XYEkin->SetTitle(Cut_Order + " " + NamePhaseSpace);
                     hNameHisto_XYEkin->SetName("hNameHisto_XYEkin_"+Cut_Order + "_" + NamePhaseSpace);
                     hNameHisto_XYEkin->GetXaxis()->SetTitle("X [mm]");
                     hNameHisto_XYEkin->GetYaxis()->SetTitle("Y [mm]");
                     hNameHisto_XYEkin->GetZaxis()->SetTitle("Ekine [MeV]");
                     map_Result_Histo["hNameHisto_XYEkin"]=hNameHisto_XYEkin->GetName();
                     
                     cout << "Pb2" << endl;
                     
                     // Z:X:Ekine
                     NameTTreeDraw = "Ekine:Z:Y >> hZXEkine";
                     TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
                     hNameHisto_ZXEkin =(TH3F*)gDirectory->Get("hZXEkine")->Clone("hNameHisto_ZXEkin");
                     
                     hNameHisto_ZXEkin->SetTitle(Cut_Order + " " + NamePhaseSpace);
                     hNameHisto_ZXEkin->SetName("hNameHisto_ZYEkin_"+Cut_Order + "_" + NamePhaseSpace);
                     hNameHisto_ZXEkin->GetXaxis()->SetTitle("Z [mm]");
                     hNameHisto_ZXEkin->GetYaxis()->SetTitle("Y [mm]");
                     hNameHisto_ZXEkin->GetZaxis()->SetTitle("Ekine [MeV]");
                     map_Result_Histo["hNameHisto_ZXEkin"]=hNameHisto_ZXEkin->GetName();
                     
                     
                     cout << "End" << endl;
                 }
                 else if(map_Comment_Ref[i_Setup][Open_Condition1]== "true"){
                     
                     NameTTreeDraw = "Z >> hz";
                     TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
                     hZ =(TH1F*)gDirectory->Get("hz")->Clone("hZ");
                     Z = hZ->GetMean();
                     
                     
                 }
                 else{
                     Z +=1;
                 } 
                 
                 //          (" + constants->IntToString(TotalBin_MAR_Z,0) +","+ constants->IntToString(Bin_Min_MAR_Z,0) +"," +constants->IntToString(Bin_Max_MAR_Z,0) +")"
                 
                 
                 NameTTreeDraw = "Ekine >> h1(25000,0,250)";
                 
                 
                 
                 TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
                 hNameHisto_Ekin =(TH1F*)gDirectory->Get("h1")->Clone("hNameHisto_Ekin");
                 
                 hNameHisto_Ekin->SetTitle(Cut_Order + " " + NamePhaseSpace);
                 hNameHisto_Ekin->SetName("hNameHisto_Ekin_"+Cut_Order + "_" + NamePhaseSpace);
                 hNameHisto_Ekin->GetXaxis()->SetTitle("Ekine [MeV]");
                 hNameHisto_Ekin->GetYaxis()->SetTitle("Entries");
                 //          hNameHisto_Ekin->SetLineColor(kRed);
                 
                 map_Result_Histo["hNameHisto_Ekin"]=hNameHisto_Ekin->GetName();
                 
                 
                 //          TCut cut2(Cut_Order.Data());
                 NameTTreeDraw = "Y:X >> h2(4001,-200,200,4001,-200,200)";
                 TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
                 hNameHisto_XY =(TH2F*)gDirectory->Get("h2")->Clone("hNameHisto_XY");
                 
                 hNameHisto_XY->SetTitle(Cut_Order + " " + NamePhaseSpace);
                 hNameHisto_XY->SetName("hNameHisto_XY_"+ Cut_Order + "_" + NamePhaseSpace);
                 hNameHisto_XY->GetXaxis()->SetTitle("X [mm]");
                 hNameHisto_XY->GetYaxis()->SetTitle("Y [mm]");
                 map_Result_Histo["hNameHisto_XY"]=hNameHisto_XY->GetName();
                 // X
                 
                 NameTTreeDraw = "X >> hx(4001,-200,200)";
                 TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
                 hNameHisto_X =(TH1F*)gDirectory->Get("hx")->Clone("hNameHisto_X");
                 
                 hNameHisto_X->SetTitle(Cut_Order + " " + NamePhaseSpace);
                 hNameHisto_X->SetName("hNameHisto_X_"+ Cut_Order + "_" + NamePhaseSpace);
                 hNameHisto_X->GetXaxis()->SetTitle("X [mm]");
                 hNameHisto_X->GetYaxis()->SetTitle("Entries");
                 map_Result_Histo["hNameHisto_X"]=hNameHisto_X->GetName();
                 // Y
                 
                 NameTTreeDraw = "Y >> hy(4001,-200,200)";
                 TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),cut,"goff");
                 hNameHisto_Y =(TH1F*)gDirectory->Get("hy")->Clone("hNameHisto_Y");
                 
                 hNameHisto_Y->SetTitle(Cut_Order + " " + NamePhaseSpace);
                 hNameHisto_Y->SetName("hNameHisto_Y_"+ Cut_Order + "_" + NamePhaseSpace);
                 hNameHisto_Y->GetXaxis()->SetTitle("Y [mm]");
                 hNameHisto_Y->GetYaxis()->SetTitle("Entries");
                 map_Result_Histo["hNameHisto_Y"]=hNameHisto_Y->GetName();
                 
                 
                 
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
                 
                 
                 cout << "Fichier Save begin " << endl;
                 
                 
                 // Result Pointeur 
                 
                 for(auto& x: map_Result_Histo){
                     
                     TString name = x.second;
                     if (gDirectory->FindObject(name)) printf ("%s exist \n", name.Data());
                     else  printf ("%s doesn't exist \n", name.Data());
                     
                 }
                 
                 
                 //              if (gDirectory->FindObject(map_Result_Histo["hNameHisto_ZEkin"])) ;
                 //              else << " " << "false"
                 //              if (gDirectory->FindObject(map_Result_Histo["hNameHisto_XYEkin"])) << " " << hNameHisto_XYEkin->GetName()
                 //              else << " " << "false"
                 //              if (gDirectory->FindObject(map_Result_Histo["hNameHisto_ZXEkin"])) << " " << hNameHisto_ZXEkin->GetName()
                 //              else << " " << "false"
                 //              
                 
                 Fichier_Save 
                 << "#"
                 << " " << i 
                 << " " << map_Result[Cut_Order][Z]["Name"] 
                 << " " << Cut_Order
                 << " " << hNameHisto_Ekin->GetName()
                 << " " << hNameHisto_XY->GetName()
                  << " " << Results_h_XY_Ekine_Name[Name_CutOrder]
                 << " " << map_Result_Histo["hNameHisto_ZEkin"]
                 << " " << map_Result_Histo["hNameHisto_XYEkin"]
                 << " " << map_Result_Histo["hNameHisto_ZXEkin"]
                 << " " << hNameHisto_X->GetName()
                 << " " << hNameHisto_Y->GetName()
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
                 
                 cout << "Fichier Save end " << endl;
                 
                 
                 
                 map_EntriesForFluence[Cut_Order] = hNameHisto_Ekin->GetEntries();
                 
                 cout << "map_Result["<<Cut_Order<<"] " << map_EntriesForFluence[Cut_Order] << endl;
                 Z_Memory = Z;
                 
                 map_Z_Memory[Cut_Order] = Z_Memory;
                 
                 
                 if(bool_Draw == true){
                     NameCanvas = const_ICO->NameCanvas(Number_temp, "PDGkernel");
                     C_temp= new TCanvas(NameCanvas, NameCanvas);
                     //        
                     hNameHisto_Ekin->DrawNormalized("e");
                 }
                 
                 if(bool_Draw == true){
                     
                     NameCanvas = const_ICO->NameCanvas(Number_temp, "PDGkernel");
                     C_temp= new TCanvas(NameCanvas, NameCanvas);
                     //          
                     hNameHisto_XY->Draw("colz");
                 } 
                 
                 //          else delete C_temp;
                 
                 
                 
                 
                 
                 // SaveData 
                 Output_File->Add(hNameHisto_Ekin);
                 Output_File->Add(hNameHisto_XY);
                 Output_File->Add(hNameHisto_X);
                 Output_File->Add(hNameHisto_Y);
                 
                 
                 
                 //              Output_File->Add(hZ);
                 if (gDirectory->FindObject(map_Result_Histo["hNameHisto_ZEkin"])){ Output_File->Add(hNameHisto_ZEkin); }
                 if (gDirectory->FindObject(map_Result_Histo["hNameHisto_XYEkin"])){ Output_File->Add(hNameHisto_XYEkin);}
                 if (gDirectory->FindObject(map_Result_Histo["hNameHisto_ZXEkin"])){ Output_File->Add(hNameHisto_ZXEkin);}
                 
                 
//                if (gDirectory->FindObject(hZ)){if(hZ->GetEntries()<2) delete hZ;}
                 
                 Output_File->Write();
                 
                 delete hNameHisto_Ekin;
                 delete hNameHisto_XY;
                 delete hNameHisto_X;
                 delete hNameHisto_Y;
                 
                 if (gDirectory->FindObject(map_Result_Histo["hNameHisto_ZEkin"])){  delete hNameHisto_ZEkin;}
                 if (gDirectory->FindObject(map_Result_Histo["hNameHisto_XYEkin"])){delete hNameHisto_XYEkin;}
                 if (gDirectory->FindObject(map_Result_Histo["hNameHisto_ZXEkin"])){ delete hNameHisto_ZXEkin;}
                 
             }
             
                       myFile->Close();
             
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
             
             if(bool_Draw == true){
                 NameCanvas = const_ICO->NameCanvas(Number_temp, "PDGkernel");
                 C_temp= new TCanvas(NameCanvas, NameCanvas);
                 TGraph_EKine->Draw();
             }
             
             
         } // TCut
         
         
     } // end if
     
     
     
     if(NTUPLE_BOOL == true){
         // Open File for NTUPLE
         i=0;
         // Enregistrement de l'ouverture des TFiles 
         
         vector <TFile*> vec_TFile;
         vector <TTree*> vec_TTree;
         
         TFile *myFile_1;
         TTree * t ;
         
         vec_Name_PhaseSpace.clear();
         
         vec_Name_PhaseSpace.push_back("frite_kapton_phasespaceactor"); 
         vec_Name_PhaseSpace.push_back("frite_Kapton_after_phasespaceactor");
         
         vec_Name_PhaseSpace.push_back("frite_Tungsten_before_phasespaceactor"); 
         vec_Name_PhaseSpace.push_back("frite_Tungsten_after_phasespaceactor");
         
         vec_Name_PhaseSpace.push_back("frite_DetecteurCharbel_before_phasespaceactor"); 
         vec_Name_PhaseSpace.push_back("frite_DetecteurCharbel_after_phasespaceactor");
         
         vec_Name_PhaseSpace.push_back("frite_cuivre_before_phasespaceactor"); 
         vec_Name_PhaseSpace.push_back("frite_cuivre_after_phasespaceactor");
         
         
         vec_Name_PhaseSpace.push_back("frite_colli_before_phasespaceactor"); 
         vec_Name_PhaseSpace.push_back("frite_colli_after_phasespaceactor" );   
         
         
         for(auto& NamePhaseSpace: vec_Name_PhaseSpace){
             i++;   
             
             cout <<"*** NamePhaseSpace " << NamePhaseSpace << endl;
             
             TString CompletePath = PathFile + NamePhaseSpace + extension;
             
             TString RealNameSpace = NamePhaseSpace;
             
             
             if(NamePhaseSpace == "frite_colli_after_phasespaceactor2" ){
                 string Replace_NamePhaseSpace = "frite_colli_after_phasespaceactor";
                 CompletePath = PathFile + Replace_NamePhaseSpace + extension;
                 RealNameSpace = Replace_NamePhaseSpace;
             }
             
             myFile_1 = new TFile(CompletePath.Data());
             vec_TFile.push_back(myFile_1);
             
             TTree *T = new TTree("t","t");
             T->SetDirectory(0);
             T=(TTree*)myFile_1->Get("PhaseSpace");
             vec_TTree.push_back(T);
         }
         
         
         if(vec_TFile.size()==2){
             
             
             int i_TT = 0;
             
             //          TTree* TTMakeMAR_0 = (TTree*) vec_TFile[i_TT]->Get("PhaseSpace");
             
             TTree* TTMakeMAR_0 = vec_TTree[i_TT];
             TTreeReader myReader_0(TTMakeMAR_0);
             
             TTreeReaderValue<float> Ekine_0(myReader_0, "Ekine");
             TTreeReaderValue<int> ParentID_0(myReader_0, "ParentID");
             
             
             long Nevents = TTMakeMAR_0->GetEntries();
             cout <<"MakeMARFromCrystals TTMakeMAR->GetEntries() " << Nevents << endl;
             
             i_TT++;
             
             //          TTree* TTMakeMAR_1 = (TTree*) vec_TFile[i_TT]->Get("PhaseSpace");
             
             TTree* TTMakeMAR_1 = vec_TTree[i_TT];
             TTreeReader myReader_1(TTMakeMAR_1);
             
             TTreeReaderValue<float> Ekine_1(myReader_1, "Ekine");
             TTreeReaderValue<int> ParentID_1(myReader_1, "ParentID");
             
             TNtuple *nt = new TNtuple("nt","Demo ntuple","x:y");
             
             
             while (myReader_0.Next() && myReader_1.Next()) {
                 
                 
                 if(*ParentID_0==0 && *ParentID_1==0) nt->Fill(*Ekine_0, *Ekine_1);
                 
             }
             
             NameCanvas = const_ICO->NameCanvas(Number_temp, "PDGkernel");
             C_temp= new TCanvas(NameCanvas, NameCanvas);
             
             nt->Draw("x:y","","PARA");
             
             //          void parallel_example() {
             //    TNtuple *nt = new TNtuple("nt","Demo ntuple","x:y:z:u:v:w:a:b:c");
             //    
             
             
             //    for (Int_t i=0; i<3000; i++) {
             //       nt->Fill(   rnd,   rnd,   rnd,    rnd,    rnd,    rnd, rnd, rnd, rnd );
             //    
             //    }
             
             
         }
         
         
         
         
         
         if(vec_TFile.size()==10){
             
             
             int i_TT = 0; cout << vec_Name_PhaseSpace[i_TT] << endl;
             TTree* TTMakeMAR_0 = vec_TTree[i_TT];
             TTreeReader myReader_0(TTMakeMAR_0);
             TTreeReaderValue<float> Ekine_0(myReader_0, "Ekine");
             TTreeReaderValue<int> ParentID_0(myReader_0, "ParentID");
             TTreeReaderValue<int> EventID_0(myReader_0, "EventID");
             
             long Nevents = TTMakeMAR_0->GetEntries();
             cout <<"MakeMARFromCrystals TTMakeMAR->GetEntries() " << Nevents << endl;
             
             i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
             TTree* TTMakeMAR_1 = vec_TTree[i_TT];
             TTreeReader myReader_1(TTMakeMAR_1);
             TTreeReaderValue<float> Ekine_1(myReader_1, "Ekine");
             TTreeReaderValue<int> ParentID_1(myReader_1, "ParentID");
             TTreeReaderValue<int> EventID_1(myReader_1, "EventID");
             
             
             i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
             TTree* TTMakeMAR_2 = vec_TTree[i_TT];
             TTreeReader myReader_2(TTMakeMAR_2);
             TTreeReaderValue<float> Ekine_2(myReader_2, "Ekine");
             TTreeReaderValue<int> ParentID_2(myReader_2, "ParentID");
             TTreeReaderValue<int> EventID_2(myReader_2, "EventID");
             
             i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
             TTree* TTMakeMAR_3 = vec_TTree[i_TT];
             TTreeReader myReader_3(TTMakeMAR_3);
             TTreeReaderValue<float> Ekine_3(myReader_3, "Ekine");
             TTreeReaderValue<int> ParentID_3(myReader_3, "ParentID");
             TTreeReaderValue<int> EventID_3(myReader_3, "EventID");
             
             i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
             TTree* TTMakeMAR_4 = vec_TTree[i_TT];
             TTreeReader myReader_4(TTMakeMAR_4);
             TTreeReaderValue<float> Ekine_4(myReader_4, "Ekine");
             TTreeReaderValue<int> ParentID_4(myReader_4, "ParentID");
             TTreeReaderValue<int> EventID_4(myReader_4, "EventID");
             
             i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
             TTree* TTMakeMAR_5 = vec_TTree[i_TT];
             TTreeReader myReader_5(TTMakeMAR_5);
             TTreeReaderValue<float> Ekine_5(myReader_5, "Ekine");
             TTreeReaderValue<int> ParentID_5(myReader_5, "ParentID");
             TTreeReaderValue<int> EventID_5(myReader_5, "EventID");
             
             i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
             TTree* TTMakeMAR_6 = vec_TTree[i_TT];
             TTreeReader myReader_6(TTMakeMAR_6);
             TTreeReaderValue<float> Ekine_6(myReader_6, "Ekine");
             TTreeReaderValue<int> ParentID_6(myReader_6, "ParentID");
             TTreeReaderValue<int> EventID_6(myReader_6, "EventID");
             
             i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
             TTree* TTMakeMAR_7 = vec_TTree[i_TT];
             TTreeReader myReader_7(TTMakeMAR_7);
             TTreeReaderValue<float> Ekine_7(myReader_7, "Ekine");
             TTreeReaderValue<int> ParentID_7(myReader_7, "ParentID");
             TTreeReaderValue<int> EventID_7(myReader_7, "EventID");
             
             i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
             TTree* TTMakeMAR_8 = vec_TTree[i_TT];
             TTreeReader myReader_8(TTMakeMAR_8);
             TTreeReaderValue<float> Ekine_8(myReader_8, "Ekine");
             TTreeReaderValue<int> ParentID_8(myReader_8, "ParentID");
             TTreeReaderValue<int> EventID_8(myReader_8, "EventID");
             
             i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
             TTree* TTMakeMAR_9 = vec_TTree[i_TT];
             TTreeReader myReader_9(TTMakeMAR_9);
             TTreeReaderValue<float> Ekine_9(myReader_9, "Ekine");
             TTreeReaderValue<int> ParentID_9(myReader_9, "ParentID");
             TTreeReaderValue<float> X(myReader_9, "X");
             TTreeReaderValue<float> Y(myReader_9, "Y");
             TTreeReaderValue<int> EventID_9(myReader_9, "EventID");
             
             TString Legend = "Kapton_in:Kapton_out:Tungsten_in:Tungsten_out:Ionizationchamber_in:Ionizationchamber_out:Lightdetector_in:Lightdetector_out:Colli_in:Colli_out";
             
             Legend = "Kapton_in:Kapton_out:Tungsten_in:T_out:LD_in:LD_out:IC_in:IC_out:Colli_in:Colli_out";
             
             
             TNtuple *nt = new TNtuple("nt_Ekine","Demo ntuple",Legend);
             
             long Compteur_Event_Tot = 0; long Compteur_Event_NotFOund = 0;
             
             myReader_0.Next() ; myReader_1.Next() ; myReader_2.Next() ; myReader_3.Next() ; myReader_4.Next() ; myReader_5.Next() ; myReader_6.Next() ; myReader_7.Next(); myReader_8.Next() ;
             
             
             while ( myReader_9.Next()) {
                 
                 //              myReader_0.Next() && myReader_1.Next() && myReader_2.Next() && myReader_3.Next() && myReader_4.Next() && myReader_5.Next() && myReader_6.Next() && myReader_7.Next()&& myReader_8.Next() &&
                 
                 //              if(*ParentID_0==0 && *ParentID_1==0 && *ParentID_2==0 && *ParentID_3==0); 
                 
                 while(*EventID_0 < *EventID_9 )myReader_0.Next(); 
                 while(*EventID_1 < *EventID_9 )myReader_1.Next();
                 while(*EventID_2 < *EventID_9 )myReader_2.Next();
                 while(*EventID_3 < *EventID_9 )myReader_3.Next();
                 while(*EventID_4 < *EventID_9 )myReader_4.Next();
                 while(*EventID_5 < *EventID_9 )myReader_5.Next();
                 while(*EventID_6 < *EventID_9 )myReader_6.Next();
                 while(*EventID_7 < *EventID_9 )myReader_7.Next();
                 while(*EventID_8 < *EventID_9 )myReader_8.Next();
                 
                 bool True_Event = true;
                 
                 float Correction_Ekine_6 = *Ekine_6;
                 float Correction_Ekine_7 = *Ekine_7;
                 
                 //              if(*EventID_0 == *EventID_9 || *EventID_1 == *EventID_9 || *EventID_2 == *EventID_9 || *EventID_3 == *EventID_9 || *EventID_4 == *EventID_9 || *EventID_5 == *EventID_9 || *EventID_6 > *EventID_9 || *EventID_7 > *EventID_9 || *EventID_8 == *EventID_9){
                 //                  Correction_Ekine_6=*Ekine_5;
                 //                  Correction_Ekine_7=*Ekine_8;
                 //             }
                 
                 if(*EventID_0 > *EventID_9 || *EventID_1 > *EventID_9 || *EventID_2 > *EventID_9 || *EventID_3 > *EventID_9 || *EventID_4 > *EventID_9 || *EventID_5 > *EventID_9 || *EventID_6 > *EventID_9 || *EventID_7 > *EventID_9 || *EventID_8 > *EventID_9){
                     
                     //                  cout << "EventID_9 " << *EventID_9 << endl;
                     //                  
                     //                  cout << "EventID_0 " << *EventID_0 << endl;
                     //                  cout << "EventID_1 " << *EventID_1 << endl;
                     //                  cout << "EventID_2 " << *EventID_2 << endl;
                     //                  cout << "EventID_3 " << *EventID_3 << endl;
                     //                  cout << "EventID_4 " << *EventID_4 << endl;
                     //                  cout << "EventID_5 " << *EventID_5 << endl;
                     //                  cout << "EventID_6 " << *EventID_6 << endl;
                     //                  cout << "EventID_7 " << *EventID_7 << endl;
                     //                  cout << "EventID_8 " << *EventID_8 << endl;
                     //                  
                     //                  cout <<endl;
                     True_Event=false;
                     Compteur_Event_NotFOund++;
                     
                     continue;
                     
                 }
                 
                 //              cout << "EventID " << *EventID_1 << endl;
                 
                 
                 if(*ParentID_0==0 && *ParentID_1==0 && *ParentID_2==0 && *ParentID_3==0,*ParentID_4==0 && *ParentID_5==0 && *ParentID_6==0 && *ParentID_7==0 && *ParentID_8==0 && *ParentID_9==0 &&*X<50&&*X>-50&&*Y<50&&*Y>-50 && True_Event==true ) {  
                     
                     
                     nt->Fill(*Ekine_0, *Ekine_1,*Ekine_2,*Ekine_3,*Ekine_4, *Ekine_5,Correction_Ekine_6,Correction_Ekine_7,*Ekine_8, *Ekine_9);
                     
                     Compteur_Event_Tot++;
                     
                 }
                 
             }
             
             NameCanvas = const_ICO->NameCanvas(Number_temp, "PDGkernel");
             C_temp= new TCanvas(NameCanvas, NameCanvas);
             
             nt->Draw(Legend,"","PARA");
             Output_File->Add(nt);
             
             cout << "Compteur_Event_NotFOund " << Compteur_Event_NotFOund << " Compteur_Event_Tot " << Compteur_Event_Tot << endl;
         }
         
         
         
         
         if(OnlyKaptonColli == true){
             
             
             
             
             
             for(auto& NamePhaseSpace: vec_TTree){
                 delete NamePhaseSpace;
             }
             
             for(auto& NamePhaseSpace: vec_TFile){
                 delete NamePhaseSpace;
             }
             
             
             vec_TFile.clear();
             vec_TTree.clear();
             vec_Name_PhaseSpace.clear();
             
             vec_Name_PhaseSpace.push_back("frite_kapton_phasespaceactor"); 
             vec_Name_PhaseSpace.push_back("frite_Kapton_after_phasespaceactor");
             vec_Name_PhaseSpace.push_back("frite_colli_before_phasespaceactor"); 
             vec_Name_PhaseSpace.push_back("frite_colli_after_phasespaceactor" );   
             
             
             
             for(auto& NamePhaseSpace: vec_Name_PhaseSpace){
                 i++;   
                 
                 cout <<"*** NamePhaseSpace " << NamePhaseSpace << endl;
                 
                 TString CompletePath = PathFile + NamePhaseSpace + extension;
                 
                 TString RealNameSpace = NamePhaseSpace;
                 
                 
                 if(NamePhaseSpace == "frite_colli_after_phasespaceactor2" ){
                     string Replace_NamePhaseSpace = "frite_colli_after_phasespaceactor";
                     CompletePath = PathFile + Replace_NamePhaseSpace + extension;
                     RealNameSpace = Replace_NamePhaseSpace;
                 }
                 
                 myFile_1 = new TFile(CompletePath.Data());
                 vec_TFile.push_back(myFile_1);
                 
                 TTree *T = new TTree("t","t");
                 T->SetDirectory(0);
                 T=(TTree*)myFile_1->Get("PhaseSpace");
                 vec_TTree.push_back(T);
             }
             
             
             
             
             
             
             if(vec_TFile.size()==4){
                 
                 
                 int i_TT = 0; cout << vec_Name_PhaseSpace[i_TT] << endl;
                 TTree* TTMakeMAR_0 = vec_TTree[i_TT];
                 TTreeReader myReader_0(TTMakeMAR_0);
                 TTreeReaderValue<float> Ekine_0(myReader_0, "Ekine");
                 TTreeReaderValue<int> ParentID_0(myReader_0, "ParentID");
                 TTreeReaderValue<int> EventID_0(myReader_0, "EventID");
                 
                 long Nevents = TTMakeMAR_0->GetEntries();
                 cout <<"MakeMARFromCrystals TTMakeMAR->GetEntries() " << Nevents << endl;
                 
                 i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
                 TTree* TTMakeMAR_1 = vec_TTree[i_TT];
                 TTreeReader myReader_1(TTMakeMAR_1);
                 TTreeReaderValue<float> Ekine_1(myReader_1, "Ekine");
                 TTreeReaderValue<int> ParentID_1(myReader_1, "ParentID");
                 TTreeReaderValue<int> EventID_1(myReader_1, "EventID");
                 
                 
                 
                 i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
                 TTree* TTMakeMAR_8 = vec_TTree[i_TT];
                 TTreeReader myReader_8(TTMakeMAR_8);
                 TTreeReaderValue<float> Ekine_8(myReader_8, "Ekine");
                 TTreeReaderValue<int> ParentID_8(myReader_8, "ParentID");
                 TTreeReaderValue<int> EventID_8(myReader_8, "EventID");
                 
                 i_TT++; cout << vec_Name_PhaseSpace[i_TT] << endl;
                 TTree* TTMakeMAR_9 = vec_TTree[i_TT];
                 TTreeReader myReader_9(TTMakeMAR_9);
                 TTreeReaderValue<float> Ekine_9(myReader_9, "Ekine");
                 TTreeReaderValue<int> ParentID_9(myReader_9, "ParentID");
                 TTreeReaderValue<float> X(myReader_9, "X");
                 TTreeReaderValue<float> Y(myReader_9, "Y");
                 TTreeReaderValue<int> EventID_9(myReader_9, "EventID");
                 
                 TString Legend = "Kapton_in:Kapton_out:Colli_in:Colli_out";
                 
                 TNtuple *nt = new TNtuple("nt_Ekine_Only","Demo ntuple",Legend);
                 
                 long Compteur_Event_Tot = 0; long Compteur_Event_NotFOund = 0;
                 
                 myReader_0.Next() ; myReader_1.Next() ;  myReader_8.Next() ;
                 
                 
                 while ( myReader_9.Next()) {
                     
                     //              myReader_0.Next() && myReader_1.Next() && myReader_2.Next() && myReader_3.Next() && myReader_4.Next() && myReader_5.Next() && myReader_6.Next() && myReader_7.Next()&& myReader_8.Next() &&
                     
                     //              if(*ParentID_0==0 && *ParentID_1==0 && *ParentID_2==0 && *ParentID_3==0); 
                     
                     while(*EventID_0 < *EventID_9 )myReader_0.Next(); 
                     while(*EventID_1 < *EventID_9 )myReader_1.Next();
                     while(*EventID_8 < *EventID_9 )myReader_8.Next();
                     
                     bool True_Event = true;
                     
                     if(*EventID_0 > *EventID_9 || *EventID_1 > *EventID_9  || *EventID_8 > *EventID_9){
                         
                         //                  cout << "EventID_9 " << *EventID_9 << endl;
                         //                  
                         //                  cout << "EventID_0 " << *EventID_0 << endl;
                         //                  cout << "EventID_1 " << *EventID_1 << endl;
                         //                  cout << "EventID_8 " << *EventID_8 << endl;
                         
                         //                  cout <<endl;
                         True_Event=false;
                         Compteur_Event_NotFOund++;
                         
                         continue;
                         
                     }
                     
                     //              cout << "EventID " << *EventID_1 << endl;
                     
                     
                     if(*ParentID_0==0 && *ParentID_1==0   && *ParentID_8==0 && *ParentID_9==0 &&*X<50&&*X>-50&&*Y<50&&*Y>-50 && True_Event==true ) {  
                         
                         
                         nt->Fill(*Ekine_0, *Ekine_1,*Ekine_8, *Ekine_9);
                         
                         Compteur_Event_Tot++;
                         
                     }
                     
                 }
                 
                 NameCanvas = const_ICO->NameCanvas(Number_temp, "PDGkernel");
                 C_temp= new TCanvas(NameCanvas, NameCanvas);
                 
                 nt->Draw(Legend,"","PARA");
                 
                 
                 Output_File->Add(nt);
                 
                 
                 cout << "Compteur_Event_NotFOund " << Compteur_Event_NotFOund << " Compteur_Event_Tot " << Compteur_Event_Tot << endl;
             }  
             
         }
         
         
     } // end if NTUPLE
     
     cout << "***Total Timer -> ";timer.Stop(); timer.Print();   timer.Continue();   
     
     if(bool_Save_OutputFile == true){
         Output_File->Write();
         Output_File->Close();
     }
     
     Fichier_Save.close();
     
     
     
     
     
     
     
     
     
     
     
     
     delete const_ICO;
     delete constants;
     
     
 } // end main
 
 
 
 
 
