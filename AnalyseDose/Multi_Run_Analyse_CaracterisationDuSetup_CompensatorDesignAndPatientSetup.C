
 
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
     
     double Incertitude_Total_Dose = TMath::Sqrt( NormalisationFactor * (somme1 - somme2_squared) );
     
     
     // DoseActor
     Incertitude_Total_Dose = sqrt( (1.0/(N_Primaries-1))*(Total_Dose_Squared/N_Primaries - pow(Total_Dose/N_Primaries, 2)) )/(Total_Dose/N_Primaries);
     
     
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
 
 
 // Macro include
 
 using namespace std;
 
 //********************** Main Function *************************************************************************************************************************
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaractérisationDuSetup.C
 
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Multi_Run_Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C
 
 void Multi_Run_Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup(){
     
     TStopwatch timer;
     timer.Start();
     
     // Maquillage graphique
     DPGAConstants *constants = new DPGAConstants();
     TString NameCanvas = "";
     Int_t Number_temp = 0;
     TCanvas * C_temp = NULL;
     
     TCanvas * C_temp_2 = NULL;
     
     TString NameForGet="";
     
     TPaveText * pt = NULL;
     TText *pt_LaTex = NULL;
     
     ICOConstants *const_ICO = new ICOConstants();
     
     bool Farm = false;
     
     
     string PathFile = "/media/sf_PartageVbox8/GATE/All_Simulation/z_Projet_Arronax_Compensator_design/";
     
     if(Farm == true) PathFile = "/home/gate/nantes/abongran/All_Simulation/z_Projet_Arronax_Compensator_design/";
     
     
     string Coef_Event = "10";
     string Unit_Event = "M" ;
     
     //      std::vector<string> vec_Option {"68MeV_0_" , "68MeV_1_","68MeV_0.68_", "v1_68MeV_0.68_"};
     
     
     //      std::vector<string> vec_Option {"v1_68MeV_0_","v1_68MeV_1_","v1_68MeV_1_8.1_","v1_68MeV_0.68_"};
     
     std::vector<string> vec_Option {"v1_68MeV_0_","v1_68MeV_0.68_","v1_68MeV_1_"};
     
     
     if(Farm == true){
         
         vec_Option.clear();
         vec_Option.push_back("v1_OnlyOneBin_68MeV_0_");
     }
     
     bool Etude_Epaisseur_Position = true;
     
     std::vector<string> Category_Of_Analyse {"Epaisseur", "Pos"};
     std::vector<string> type_Of_Analyse_2 {"Min", "Max"};
     
     
     
     
     TString  CheckFile =  "mkdir " + PathFile + "output_Analyse_Design_Setup_" + Coef_Event + Unit_Event ;
     system(CheckFile.Data());
     
     
     // Partie 1 Cas de référence :
     
     
     //   Analyse_Caracterisation_Output
     
     //   dose_Cube_after_All-Dose
     
     std::vector<string> vec_Ref_Case ;
     
     for(auto& x: vec_Option){
         string Title = "output_Ref_" + x;
         vec_Ref_Case.push_back(Title);
     }
     
     //****************************************************** 
     cout << "Partie 1 Cas de reference " << endl;
     //****************************************************** 
     
     map<int, map < TString,TString >> map_Comment_Ref;
     map<int, map < TString,TH1D* >> map_Histo_Ref;
     map<int, map < TString,TString >> map_Result_Ref;
     
     
     map<int, map< TString, map < TString ,float >>> map_Histo_Results;
     map<int, map< TString, map < TString ,float >>> map_Histo_Results_Incertitude;
     
     map<int, map< TString, map< TString, map < TString ,TH1D* >>>>map_Histo_StudyCase;
     
     
     
     //      map<int, map < TString,TString >> map_Comment_Ref;
     //      map<int, map < TString,TH1D* >> map_Histo_Ref;
     //      map<int, map < TString,TString >> map_Result_Ref;
     
     
     int i_Setup = 0;
     
     int CompteurCaseForResult = 0;
     
     for(int i = 0; i < vec_Ref_Case.size();i++){
         
         cout << "\n *** ***" << endl;
         
         i_Setup = i;
         CompteurCaseForResult = 1;
         
         TString NameInput;
         
         TString NameCase = vec_Ref_Case[i_Setup] + Coef_Event + Unit_Event;
         
         TString PathInput = PathFile + NameCase;
         
         
         //ReadInformation :
         TString totalPathReadInformation =    PathInput+"/SaveHistory.txt";
         const_ICO->LoadSetup_with_RunSystem_SaveHistoryGate(map_Comment_Ref, totalPathReadInformation.Data(), i_Setup ,0);
         
         
         cout <<"PathInput " << PathInput << endl; 
         
         
         
         NameInput = PathInput + "/dose_Cube_after_All-Dose.root";
         TFile *myFile_Ref_Dose = TFile::Open(NameInput.Data()); 
         TH1D * h_dose = (TH1D*)myFile_Ref_Dose->Get("histo");  
         TH1D* h_dose_Clone = (TH1D*)h_dose->Clone();
         
         
         NameInput = PathInput + "/dose_Cube_after_All-Dose-Uncertainty.root";
         TFile *myFile_Ref_Dose_Uncertity = TFile::Open(NameInput.Data()); 
         TH1D * h_dose_Uncertainty = (TH1D*)myFile_Ref_Dose_Uncertity->Get("histo");  
         TH1D* h_dose_Uncertainty_Clone = (TH1D*)h_dose_Uncertainty->Clone();
         
         
         NameInput = PathInput + "/dose_Cube_after_All-NbOfHits.root";
         TFile *myFile_Ref_Dose_NbOfHits = TFile::Open(NameInput.Data()); 
         TH1D * h_dose_NbOfHits = (TH1D*)myFile_Ref_Dose_NbOfHits->Get("histo");  
         TH1D* h_dose_NbOfHits_Clone = (TH1D*)h_dose_NbOfHits->Clone();
         
         // Calcul de l'incertitude 
         
         NameInput = PathInput + "/dose_Cube_after_All-Dose-Squared.root";
         TFile *myFile_Ref_Dose_Squared = TFile::Open(NameInput.Data()); 
         TH1D * h_dose_Dose_Squared = (TH1D*)myFile_Ref_Dose_Squared->Get("histo");  
         TH1D* h_dose_Dose_Squared_Clone = (TH1D*)h_dose_Dose_Squared->Clone();
         
         
         // Nombre d'événements : 2 Méthode de calcul -> 
         // -> Nombre de "primaries" Nombre de particules lancées dans Gate/G4 ex : 10 M
         // -> Nombre de particules qui rentre dans la cible d'eau ie après le collimateur. N_Event << N_Primaries (~3.5%)
         
         double N_Real_Event = 0;
         double N_Primaries = 0;
         
         NameInput = PathInput + "/PhaseSpace_Entrance_Cube_After.root";
         TFile *myFile_Ref_Caracterisation_Output = TFile::Open(NameInput.Data()); 
         if(myFile_Ref_Caracterisation_Output){
             TTree* TTMakePhaseSpace = (TTree*) myFile_Ref_Caracterisation_Output->Get("PhaseSpace");
             
             TString NameTTreeDraw = "Ekine >> h1(7000,0,70)";
             TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),"","goff");
             TH1D* hNameHisto_Ekin =(TH1D*)gDirectory->Get("h1")->Clone("hNameHisto_Ekin");
             
             N_Real_Event =hNameHisto_Ekin->GetEntries();
         }
         
         
         cout << "Total Event :: " << N_Real_Event << endl;
         
         if(N_Real_Event==0)N_Real_Event=300000;  //  
         
         
         map_Histo_Ref[i_Setup]["hdose"]=h_dose_Clone;
         map_Histo_Ref[i_Setup]["hdose_Uncertainty"]=h_dose_Uncertainty_Clone;
         map_Histo_Ref[i_Setup]["h_dose_NbOfHits"]=h_dose_NbOfHits_Clone;
         map_Histo_Ref[i_Setup]["h_dose_Dose_Squared"]=h_dose_Dose_Squared_Clone;
         
         
         string SizeCube =  map_Comment_Ref[i_Setup]["/gate/Cube_After/geometry/setHeight"].Data();
         
         N_Primaries = ToDouble(map_Comment_Ref[i_Setup]["/gate/application/setTotalNumberOfPrimaries"].Data());
         cout << "N_Primaries " << N_Primaries << endl;
         
         map_Histo_Ref[i_Setup]["hdose"] = const_ICO->PutOrigineToZero(map_Histo_Ref[i_Setup]["hdose"],SizeCube);
         map_Histo_Ref[i_Setup]["hdose_Uncertainty"] = const_ICO->PutOrigineToZero(map_Histo_Ref[i_Setup]["hdose_Uncertainty"],SizeCube);
         map_Histo_Ref[i_Setup]["h_dose_NbOfHits"] = const_ICO->PutOrigineToZero(map_Histo_Ref[i_Setup]["h_dose_NbOfHits"],SizeCube);
         
         // Representation des erreur de dose pour le Graph
         for(int i = 1 ; i< h_dose_Clone->GetNbinsX()+1; i++){  
             double Error_Calcul = map_Histo_Ref[i_Setup]["hdose_Uncertainty"]->GetBinContent(i) * map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i);
             //              cout << "Bin error " <<map_Histo_Ref[i_Setup]["hdose_Uncertainty"]->GetBinContent(i)*100 << " " <<  map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i) << " " << Error_Calcul << endl;
             map_Histo_Ref[i_Setup]["hdose"]->SetBinError(i, Error_Calcul );
             //        h_dose_Clone->SetBinError(i,0);
         }
         
         
         NameCanvas = const_ICO->NameCanvas(Number_temp, vec_Ref_Case[i_Setup]);
         C_temp= new TCanvas(NameCanvas, NameCanvas);
         
         //          gStyle->SetOptStat(0);
         C_temp->Divide(2,2);
         
         C_temp->cd(1);
         map_Histo_Ref[i_Setup]["hdose"]->Draw("hist");
         
         C_temp->cd(2);
         map_Histo_Ref[i_Setup]["h_dose_Dose_Squared"]->Draw("hist");
         
         C_temp->cd(3);
         map_Histo_Ref[i_Setup]["hdose_Uncertainty"]->Draw("hist");
         
         gPad->SetLogy();
         C_temp->Update();
         C_temp->cd(4);
         map_Histo_Ref[i_Setup]["h_dose_NbOfHits"]->Draw("hist");
         
         
         //**** Etude du pic de Bragg ****/ 
         double Profil_entries ; TLorentzVector HalfMaxCoords ; double FWHM ; double Max ;int binmax; double x_Max_Line;
         
         
         float PourcentageMax = 80;
         HalfMaxCoords =  const_ICO->FindHalfMaxCoords(map_Histo_Ref[i_Setup]["hdose"],PourcentageMax);
         FWHM = const_ICO->m_FWHM;
         Max = const_ICO->fMax;
         
         // ATTENTION 
         NameCase = const_ICO->IntToString(CompteurCaseForResult,0) + NameCase;         
         map_Histo_Results[i_Setup][NameCase]["Integral-Dose[Gy]"]= map_Histo_Ref[i_Setup]["hdose"]->Integral();
         
         
         double Total_Dose_Squared_Analythique =0;
         
         double totalDose = 0; 
         for(int i = 1 ; i< h_dose_Clone->GetNbinsX()+1; i++){  
             
             totalDose += map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i);
             
             Total_Dose_Squared_Analythique += map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i)*map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i);
             
             
             //              IncertitudeSurLaDoseTotale(h_dose_Dose_Squared_Clone->GetBinContent(i),map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i), N_Primaries,0);
             
             //        h_dose_Clone->SetBinError(i,0);
         }
         
         double totalDose_squared = 0; 
         for(int i = 1 ; i< h_dose_Dose_Squared_Clone->GetNbinsX()+1; i++){  
             
             totalDose_squared += h_dose_Dose_Squared_Clone->GetBinContent(i);
             
             //             cout << map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i)*map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i) << "  " << h_dose_Dose_Squared_Clone->GetBinContent(i) << "  "<< map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i)*map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i) /h_dose_Dose_Squared_Clone->GetBinContent(i) << endl;
             //              
             //        h_dose_Clone->SetBinError(i,0);
         }
         
         cout << "totalDose " << totalDose << " Integral " << map_Histo_Ref[i_Setup]["hdose"]->Integral() << " Diff  " << totalDose/map_Histo_Ref[i_Setup]["hdose"]->Integral() << endl;
         
         
         cout << "totalDose_squared " << totalDose_squared << " Integral " << h_dose_Dose_Squared_Clone->Integral() << " Diff  " << totalDose_squared/h_dose_Dose_Squared_Clone->Integral() << endl;
         
         cout << "Total_Dose_Squared_Analythique " << Total_Dose_Squared_Analythique << " Integral " << h_dose_Dose_Squared_Clone->Integral() << " Diff  " << Total_Dose_Squared_Analythique/h_dose_Dose_Squared_Clone->Integral() << endl;
         
         
         double somme1 = totalDose_squared / N_Primaries;
         double somme2 = totalDose / N_Primaries;
         
         double IncertitudeAuCarre = (1/(float(N_Primaries-1)))* ( (somme1-(somme2))*(somme1-(somme2)) );
         float incertitude_Dose = TMath::Sqrt(IncertitudeAuCarre);
         
         cout << "somme1 " << somme1 << " somme2 " << somme2 << endl;
         cout << "IncertitudeAuCarre " << IncertitudeAuCarre << " incertitude_Dose " << incertitude_Dose << endl;
         
         if(totalDose>0){incertitude_Dose = incertitude_Dose / totalDose;}
         else incertitude_Dose = 0;
         incertitude_Dose = incertitude_Dose * N_Primaries;
         
         incertitude_Dose = TMath::Sqrt((1/(N_Real_Event-1))*( ((totalDose_squared/N_Real_Event)-(totalDose/N_Real_Event))*((totalDose_squared/N_Real_Event)-(totalDose/N_Real_Event))));
         
         incertitude_Dose /= totalDose;
         incertitude_Dose *=  N_Real_Event;
         
         cout << "incertitude-Dose Relative [0:1] " << incertitude_Dose << endl;
         
         // ************************************************
         // Fonction incertitude 
         incertitude_Dose = IncertitudeSurLaDoseTotale(Total_Dose_Squared_Analythique,map_Histo_Ref[i_Setup]["hdose"]->Integral(), N_Primaries,1);
         incertitude_Dose = IncertitudeSurLaDoseTotale(Total_Dose_Squared_Analythique,totalDose, N_Primaries,1);
         //          incertitude_Dose = IncertitudeSurLaDoseTotale(map_Histo_Ref[i_Setup]["h_dose_Dose_Squared"]->Integral(),map_Histo_Ref[i_Setup]["hdose"]->Integral(), N_Primaries,1);
         
         // ******************************************************
         // ******************************************************
         
         /*         
          *            for(auto p:mMapIdToSingleRegion) {
          *     auto region = p.second;
          *     double edep = region->sum_edep;
          *     double sq_edep = region->sum_squared_edep;
          *     double std_edep = sqrt( (1.0/(N-1))*(sq_edep/N - pow(edep/N, 2)) )/(edep/N);
          *     if( edep == 0.0 || N == 1 || sq_edep == 0 )
          *       std_edep = 1.0; // relative uncertainty of 100%
          *     double dose = region->sum_dose;
          *     double sq_dose = region->sum_squared_dose;
          *     double std_dose = sqrt( (1.0/(N-1))*(sq_dose/N - pow(dose/N, 2)) )/(dose/N);
          *     if( dose == 0.0 || N == 1 || sq_dose == 0 )
          *       std_dose = 1.0; // relative uncertainty of 100%
          *     os.precision(15);
          *     os << region->id << "\t"
          *        << region->volume << "\t"
          *        << edep << "\t"
          *        << std_edep << "\t"
          *        << sq_edep << "\t"
          *        << dose << "\t"
          *        << std_dose << "\t"
          *        << sq_dose << "\t"
          *        << region->nb_hits-1 << "\t"
          *        << region->nb_event_hits-1 << std::endl;
          */
         
         
         
         
         map_Histo_Results_Incertitude[i_Setup][NameCase]["Integral-Dose[Gy]"]= incertitude_Dose;
         
         cout << "Incertitude Dose map " << NameCase <<" " <<  map_Histo_Results_Incertitude[i_Setup][NameCase]["Integral-Dose[Gy]"] << endl;
         // A calculer map_Histo_Results_Incertitude[i_Setup][NameCase]["Integral-Dose[Gy]"]= 0.01;
         
         map_Histo_Results[i_Setup][NameCase]["Z_PositionDuPic-Max[mm]"]= Max;
         map_Histo_Results_Incertitude[i_Setup][NameCase]["Z_PositionDuPic-Max[mm]"]= 0.01;
         
         map_Histo_Results[i_Setup][NameCase]["Z_PositionDuPic-80%[mm]"]= HalfMaxCoords.Y();
         map_Histo_Results_Incertitude[i_Setup][NameCase]["Z_PositionDuPic-80%[mm]"]= 0.01;
         
         //          map_Histo_Results[i_Setup][NameCase]["Y_PositionDuPic-80%[mm]"]= map_Histo_Ref[i_Setup]["hdose"]->HalfMaxCoords.T();
         const_ICO->FindHalfMaxCoords(map_Histo_Ref[i_Setup]["hdose"],50);
         map_Histo_Results[i_Setup][NameCase]["FWHM[mm]"]= const_ICO->m_FWHM;      
         
         map_Histo_Results_Incertitude[i_Setup][NameCase]["FWHM[mm]"]= const_ICO->m_FWHM_Incertitude; 
         
         cout << "FWHM[mm] " << map_Histo_Results[i_Setup][NameCase]["FWHM[mm]"] << " +- " << map_Histo_Results_Incertitude[i_Setup][NameCase]["FWHM[mm]"] << endl;
         
         cout <<  HalfMaxCoords.Y() << " " << HalfMaxCoords.T() << " FWHM (80%) " << FWHM<< endl;
         cout << "Max BraggPeak " << Max << " " << PourcentageMax << " % Max Integrale " << h_dose_Clone->Integral()<< endl;
         
         
         
         if(Etude_Epaisseur_Position==true){
             
             //****************************************************** 
             cout << "Partie 2 Etude de l'épaisseur et de la Position " << endl;
             //****************************************************** 
             
             
             //              TString Name
             
             for(auto& x: Category_Of_Analyse){
                 
                 cout << endl;
                 
                 for(auto& y: type_Of_Analyse_2){
                     
                     
                     CompteurCaseForResult++;
                     
                     
                     string PathFile_Total = PathFile + "output_"+ x + "_" + y + "_" + vec_Option[i] + "_" + Coef_Event + Unit_Event + "/";
                     
                     
                     
                     NameCase = "output_"+ x + "_" + y + "_" + vec_Option[i] + Coef_Event + Unit_Event;
                     PathInput =  PathFile + NameCase;
                     
                     cout << "Path Input " << PathInput << endl;
                     
                     NameInput = PathInput + "/dose_Cube_after_All-Dose.root";
                     myFile_Ref_Dose = TFile::Open(NameInput.Data()); 
                     h_dose = (TH1D*)myFile_Ref_Dose->Get("histo");  
                     h_dose_Clone = (TH1D*)h_dose->Clone();
                     
                     
                     NameInput = PathInput + "/dose_Cube_after_All-Dose-Uncertainty.root";
                     myFile_Ref_Dose_Uncertity = TFile::Open(NameInput.Data()); 
                     h_dose_Uncertainty = (TH1D*)myFile_Ref_Dose_Uncertity->Get("histo");  
                     h_dose_Uncertainty_Clone = (TH1D*)h_dose_Uncertainty->Clone();
                     
                     
                     NameInput = PathInput + "/dose_Cube_after_All-NbOfHits.root";
                     myFile_Ref_Dose_NbOfHits = TFile::Open(NameInput.Data()); 
                     h_dose_NbOfHits = (TH1D*)myFile_Ref_Dose_NbOfHits->Get("histo");  
                     h_dose_NbOfHits_Clone = (TH1D*)h_dose_NbOfHits->Clone();
                     
                     NameInput = PathInput + "/dose_Cube_after_All-Dose-Squared.root";
                     myFile_Ref_Dose_Squared = TFile::Open(NameInput.Data()); 
                     h_dose_Dose_Squared = (TH1D*)myFile_Ref_Dose_Squared->Get("histo");  
                     h_dose_Dose_Squared_Clone = (TH1D*)h_dose_Dose_Squared->Clone();
                     
                     // Nombre d'événements : 2 Méthode de calcul -> 
                     // -> Nombre de "primaries" Nombre de particules lancées dans Gate/G4 ex : 10 M
                     // -> Nombre de particules qui rentre dans la cible d'eau ie après le collimateur. N_Event << N_Primaries (~3.5%)
                     
                     N_Real_Event = 0;
                     //           N_Primaries = 0;
                     
                     NameInput = PathInput + "/PhaseSpace_Entrance_Cube_After.root";
                     myFile_Ref_Caracterisation_Output = TFile::Open(NameInput.Data()); 
                     if(myFile_Ref_Caracterisation_Output){
                         TTree* TTMakePhaseSpace = (TTree*) myFile_Ref_Caracterisation_Output->Get("PhaseSpace");
                         
                         TString NameTTreeDraw = "Ekine >> h1(7000,0,70)";
                         TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),"","goff");
                         TH1D* hNameHisto_Ekin =(TH1D*)gDirectory->Get("h1")->Clone("hNameHisto_Ekin");
                         
                         //          N_Real_Event =hNameHisto_Ekin->GetEntries();
                     }
                     
                     
                     cout << "N_Real_Event :: " << N_Real_Event << endl;
                     
                     //          if(N_Real_Event==0)N_Real_Event=300000;
                     
                     
                     
                     
                     
                     map_Histo_StudyCase[i_Setup][x][y]["hdose"] = const_ICO->PutOrigineToZero(h_dose_Clone,SizeCube);
                     map_Histo_StudyCase[i_Setup][x][y]["hdose_Uncertainty"] = const_ICO->PutOrigineToZero(h_dose_Uncertainty_Clone,SizeCube);
                     map_Histo_StudyCase[i_Setup][x][y]["h_dose_NbOfHits"] = const_ICO->PutOrigineToZero(h_dose_NbOfHits_Clone,SizeCube);
                     map_Histo_StudyCase[i_Setup][x][y]["h_dose_Dose_Squared"] = h_dose_Dose_Squared_Clone;
                     
                     
                     // Rentre l'incertitude dans l'histo de dose
                     for(int i = 1 ; i< h_dose_Clone->GetNbinsX()+1; i++){  
                         double Error_Calcul = map_Histo_StudyCase[i_Setup][x][y]["hdose_Uncertainty"]->GetBinContent(i) * map_Histo_StudyCase[i_Setup][x][y]["hdose"]->GetBinContent(i);
                         //                          cout << "Bin error " <<map_Histo_StudyCase[i_Setup][x][y]["hdose_Uncertainty"]->GetBinContent(i)*100 << " " <<  map_Histo_StudyCase[i_Setup][x][y]["hdose"]->GetBinContent(i) << " " << Error_Calcul << endl;
                         map_Histo_StudyCase[i_Setup][x][y]["hdose"]->SetBinError(i, Error_Calcul );
                         //        h_dose_Clone->SetBinError(i,0);
                     }
                     
                     
                     PourcentageMax = 80;
                     HalfMaxCoords =  const_ICO->FindHalfMaxCoords(map_Histo_StudyCase[i_Setup][x][y]["hdose"],PourcentageMax);
                     FWHM = const_ICO->m_FWHM;
                     Max = const_ICO->fMax;
                     
                     
                     // ATTENTION Résultats
                     
                     NameCase = const_ICO->IntToString(CompteurCaseForResult,0) + NameCase;
                     map_Histo_Results[i_Setup][NameCase]["Integral-Dose[Gy]"]= map_Histo_StudyCase[i_Setup][x][y]["hdose"]->Integral();
                     
                     
                     
                     Total_Dose_Squared_Analythique =0;
                     
                     totalDose = 0;
                     for(int i = 1 ; i< h_dose_Clone->GetNbinsX()+1; i++){  
                         totalDose +=  map_Histo_StudyCase[i_Setup][x][y]["hdose"]->GetBinContent(i);
                         
                         Total_Dose_Squared_Analythique += map_Histo_StudyCase[i_Setup][x][y]["hdose"]->GetBinContent(i)*map_Histo_StudyCase[i_Setup][x][y]["hdose"]->GetBinContent(i);
                         
                         
                         //              IncertitudeSurLaDoseTotale(h_dose_Dose_Squared_Clone->GetBinContent(i),map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i), N_Primaries,1);
                         
                         
                     }
                     
                     totalDose_squared = 0; 
                     for(int i = 1 ; i< h_dose_Clone->GetNbinsX()+1; i++){  
                         totalDose_squared += h_dose_Dose_Squared_Clone->GetBinContent(i);
                         //        h_dose_Clone->SetBinError(i,0);
                     }
                     
                     // ************************************************
                     // Fonction incertitude 
                     incertitude_Dose =   IncertitudeSurLaDoseTotale(Total_Dose_Squared_Analythique,map_Histo_StudyCase[i_Setup][x][y]["hdose"]->Integral(), N_Primaries,1);
                     
                     //                      incertitude_Dose =   IncertitudeSurLaDoseTotale(map_Histo_StudyCase[i_Setup][x][y]["h_dose_Dose_Squared"]->Integral(),totalDose, N_Primaries,1);
                     
                     //                      
                     
                     // ******************************************************
                     // ******************************************************
                     
                     
                     
                     //                      cout << "incertitude-Dose Relative [0:1] " << incertitude_Dose << endl;
                     
                     map_Histo_Results_Incertitude[i_Setup][NameCase]["Integral-Dose[Gy]"]= incertitude_Dose;
                     cout << "Incertitude Dose map " << NameCase <<" " <<  map_Histo_Results_Incertitude[i_Setup][NameCase]["Integral-Dose[Gy]"] << endl;
                     
                     
                     map_Histo_Results[i_Setup][NameCase]["Z_PositionDuPic-Max[mm]"]= Max;
                     map_Histo_Results[i_Setup][NameCase]["Z_PositionDuPic-80%[mm]"]= HalfMaxCoords.Y();
                     //          map_Histo_Results[i_Setup][NameCase]["Y_PositionDuPic-80%[mm]"]= map_Histo_Ref[i_Setup]["hdose"]->HalfMaxCoords.T();
                     const_ICO->FindHalfMaxCoords(map_Histo_StudyCase[i_Setup][x][y]["hdose"],50);
                     map_Histo_Results[i_Setup][NameCase]["FWHM[mm]"]= const_ICO->m_FWHM;            
                     
                     map_Histo_Results_Incertitude[i_Setup][NameCase]["Z_PositionDuPic-Max[mm]"]= 0.01;
                     map_Histo_Results_Incertitude[i_Setup][NameCase]["Z_PositionDuPic-80%[mm]"]= 0.01;
                     map_Histo_Results_Incertitude[i_Setup][NameCase]["FWHM[mm]"]= const_ICO->m_FWHM_Incertitude; 
                     
                     cout << "FWHM[mm] " << map_Histo_Results[i_Setup][NameCase]["FWHM[mm]"] << " +- " << map_Histo_Results_Incertitude[i_Setup][NameCase]["FWHM[mm]"] << endl;
                     
                     cout <<  HalfMaxCoords.Y() << " " << HalfMaxCoords.T() << " FWHM " << FWHM<< endl;
                     cout << "Max BraggPeak " << Max << " " << PourcentageMax << " % Max " << const_ICO->m_Xhigh << " Integrale " << h_dose_Clone->Integral()<< endl;
                     
                     
                     
                 } // end Category_Of_Analyse
                 
                 NameCanvas = const_ICO->NameCanvas(Number_temp, vec_Ref_Case[i_Setup]);
                 
                 NameCanvas += x; 
                 
                 C_temp= new TCanvas(NameCanvas, NameCanvas);
                 
                 //                  gStyle->SetOptStat(0);
                 C_temp->Divide(2,2);
                 
                 C_temp->cd(1);
                 
                 //WARNING// Distribution de dose non rebin avec error
                 
                 //          map_Histo_Ref[i_Setup]["hdose"]->Draw("hist");
                 
                 for(auto& y: type_Of_Analyse_2){
                     
                     //               map_Histo_StudyCase[i_Setup][x][y]["hdose"]->Rebin(10);
                     map_Histo_StudyCase[i_Setup][x][y]["hdose"]->Draw("same hist");
                     cout  << " Integrale " << map_Histo_StudyCase[i_Setup][x][y]["hdose"]->Integral()<< endl;
                 }
                 
                 
                 C_temp->cd(2);
                 
                 // WARNING Distribution de dose rebin 10 (Clone) 
                 
                 for(auto& y: type_Of_Analyse_2){
                     
                     
                     
                     //               map_Histo_StudyCase0[i_Setup][x][y]["hdose"]->Rebin(10);
                     map_Histo_StudyCase[i_Setup][x][y]["h_dose_Dose_Squared"]->Draw("hist");
                     //            cout  << " Integrale " << map_Histo_StudyCase[i_Setup][x][y]["hdose"]->Integral()<< endl;
                     
                     //                      sP22__2->SetLineWidth(2);
                     //                      sP22__2->SetMarkerColor(2);
                     //                      sP22__2->SetMarkerStyle(8);
                     //                      sP22__2->SetMarkerSize(0.5);
                     
                     
                 }
                 
                 
                 
                 C_temp->cd(3);
                 
                 for(auto& y: type_Of_Analyse_2){
                     
                     //                      map_Histo_StudyCase[i_Setup][x][y]["hdose_Uncertainty"]->Rebin(10);
                     map_Histo_StudyCase[i_Setup][x][y]["hdose_Uncertainty"]->Draw("same hist");
                     //            cout  << " Integrale " << map_Histo_StudyCase[i_Setup][x][y]["hdose_Uncertainty"]->Integral()<< endl;
                 }
                 
                 gPad->SetLogy();
                 C_temp->Update();
                 C_temp->cd(4);
                 map_Histo_Ref[i_Setup]["h_dose_NbOfHits"]->Draw("hist");
                 
                 for(auto& y: type_Of_Analyse_2){
                     map_Histo_StudyCase[i_Setup][x][y]["h_dose_NbOfHits"]->Draw("hist same");
                     
                 }
                 
                 
             } // end type_Of_Analyse_2
             
             
         } // end bool Etude_Epaisseur_Position
         
         
         
     } 
     
     
     
     
     
     
     
     //ATTENTION Print Result All Case 
     
     
     for(auto& SetupNumber: map_Histo_Results){
         
         cout << "\nSetup #" << SetupNumber.first << endl;
         
         cout << "Category" ;
         
         TString CaseType_TS;
         
         for(auto& CaseType: SetupNumber.second){
             //             if(CaseType.first.Contains("Ref") || CaseType.first.Contains("Pos")){ 
             cout << " " << CaseType.first << " Incertitude" ;
             CaseType_TS = CaseType.first;
             //             }
         }
         cout << endl;
         
         for(auto& Value: SetupNumber.second[CaseType_TS]){
             cout << Value.first ;
             
             for(auto& CaseType: SetupNumber.second){
                 
                 cout << " " << CaseType.second[Value.first] << " " << map_Histo_Results_Incertitude[SetupNumber.first][CaseType.first][Value.first];
             }
             cout << endl;
             
         }
         
         
         
     } // end x
     
     
     // ATTENTION
     // PLot pic de Bragg cas ref
     
     
     TLegend * leg = NULL;
     NameCanvas = "a_Comparaison Pic de Bragg Beam spread"; 
     C_temp= new TCanvas(NameCanvas, NameCanvas);
    
     
     
     TLegend *leg2 =new TLegend(0.2099828,0.6457529,0.7934596,0.8194981,NULL,"brNDC");
     leg2->SetBorderSize(1);
     leg2->SetTextAlign(32);
     leg2->SetTextSize(0.0403071);
     leg2->SetLineColor(1);
     leg2->SetLineStyle(1);
     leg2->SetLineWidth(2);
     leg2->SetFillColor(10);
     leg2->SetFillStyle(1001);
     
     TLine * line2;
     
     TH1D * histo_de_reference = NULL;
     
     for(auto& x: map_Histo_Ref){
         
         int Setup = x.first;
         
         
         TString WichCase =  vec_Option[Setup];
         
         x.second["hdose"]->SetTitle("Proton 10M");
         
         x.second["hdose"]->GetXaxis()->SetTitle("z [mm]");
         //    h1->GetXaxis()->SetRange(1,34);
         x.second["hdose"]->GetXaxis()->SetNdivisions(507);
         x.second["hdose"]->GetXaxis()->SetLabelFont(42);
         x.second["hdose"]->GetXaxis()->SetLabelOffset(0.015);
         x.second["hdose"]->GetXaxis()->SetLabelSize(0.05);
         x.second["hdose"]->GetXaxis()->SetTitleSize(0.05);
         x.second["hdose"]->GetXaxis()->SetTitleFont(42);
         x.second["hdose"]->GetXaxis()->SetTitleOffset(1.1);
         x.second["hdose"]->GetYaxis()->SetTitle("Dose [Gy]");
         //    h1->GetYaxis()->SetRange(1,66);
         x.second["hdose"]->GetYaxis()->SetNdivisions(507);
         x.second["hdose"]->GetYaxis()->SetLabelFont(42);
         x.second["hdose"]->GetYaxis()->SetLabelOffset(0.015);
         x.second["hdose"]->GetYaxis()->SetTitleOffset(0.85);
         x.second["hdose"]->GetYaxis()->SetLabelSize(0.05);
         x.second["hdose"]->GetYaxis()->SetTitleSize(0.05);
         x.second["hdose"]->GetYaxis()->SetTitleFont(42);
         
         C_temp->SetGridx();
         C_temp->SetGridy();
         
         gStyle->SetOptStat(0);
         
         //          map_Comment_Ref[i_Setup]["/gate/Cube_After/geometry/setHeight"].Data();
         x.second["hdose"]->SetLineColor(Setup+1);
         
         
         C_temp->cd();
         x.second["hdose"]->Draw("hist same");
         
         double Profil_entries ; TLorentzVector HalfMaxCoords ; double FWHM ; double Max ;int binmax; double x_Max_Line;
         double     PourcentageMax = 80;
         HalfMaxCoords =  const_ICO->FindHalfMaxCoords(x.second["hdose"],PourcentageMax);
         FWHM = const_ICO->m_FWHM;
         Max = const_ICO->fMax;
         
         std::ostringstream streamObj3;
         // Set Fixed -Point Notation
         streamObj3 << std::fixed;
         // Set precision to 2 digits
         streamObj3 << std::setprecision(2);
         //Add double to stream
         streamObj3 << DoubleToString(HalfMaxCoords.Y()+0.005);
         
         // Get string from output string stream
         std::string strObj3 = streamObj3.str();
         
         
         TString Data = "BP[80%] " + strObj3;
         
         
         PourcentageMax = 50;
         HalfMaxCoords =  const_ICO->FindHalfMaxCoords(x.second["hdose"],PourcentageMax);
         FWHM = const_ICO->m_FWHM;
         Max = const_ICO->fMax;
         
         
         line2 = new TLine(HalfMaxCoords.X(), HalfMaxCoords.Z(),HalfMaxCoords.Y(), HalfMaxCoords.T() );
         line2 -> SetLineStyle(9);
         
         line2 -> SetHorizontal(kTRUE);
         line2->SetLineColor(kBlack);
         line2->Draw();
         
         
         std::ostringstream streamObj2;
         // Set Fixed -Point Notation
         streamObj2 << std::fixed;
         // Set precision to 2 digits
         streamObj2 << std::setprecision(2);
         //Add double to stream
         streamObj2 << DoubleToString(FWHM);
         
         // Get string from output string stream
         std::string strObj2 = streamObj2.str();
         
         
         Data += " FWHM " + strObj2;
         
         
         if(WichCase.Contains("_0.68_")) { 
             
             leg2->AddEntry(x.second["hdose"], "68 #pm 0.68 MeV " + Data, "l");
            
        }
         if(WichCase.Contains("_1_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 1.00 MeV " + Data, "l");
         if(WichCase.Contains("_0_")) { leg2->AddEntry(x.second["hdose"], "68 #pm 0.00 MeV " + Data, "l");
           histo_de_reference = (TH1D*)x.second["hdose"]->Clone();   
         }
         
         
     }
     
     leg2->Draw();
     
     //      map_Histo_Ref[i_Setup]["hdose"]
     
          // ATTENTION
     // PLot pic de Bragg cas ref
     
     NameCanvas = "b_Comparaison Pic de Bragg Beam spread"; 
     C_temp = new TCanvas(NameCanvas, NameCanvas);
     
     
      for(auto& x: map_Histo_Ref){
         
         int Setup = x.first;
         
         
         TString WichCase =  vec_Option[Setup];
         
         
         
         TH1D* h = (TH1D*)x.second["hdose"]->Clone();
         
              for(int i = 1 ; i< h->GetNbinsX()+1; i++){  
             
             h->SetBinContent(i,h->GetBinContent(i)/histo_de_reference->GetBinContent(i)*histo_de_reference->GetBinContent(i)); 
         }
         
         
         
         C_temp->SetGridx();
         C_temp->SetGridy();
         
         gStyle->SetOptStat(0);
         
         //          map_Comment_Ref[i_Setup]["/gate/Cube_After/geometry/setHeight"].Data();
         
         h->SetLineColor(Setup+1);
         
         
         C_temp->cd();
         h->Draw("hist same");
         
         double Profil_entries ; TLorentzVector HalfMaxCoords ; double FWHM ; double Max ;int binmax; double x_Max_Line;
         double     PourcentageMax = 80;
         HalfMaxCoords =  const_ICO->FindHalfMaxCoords(h,PourcentageMax);
         FWHM = const_ICO->m_FWHM;
         Max = const_ICO->fMax;
         
    
         
         TString Data = " FWHM ";
         
         
//          if(WichCase.Contains("_0.68_")) { 
//              
//              leg2->AddEntry(x.second["hdose"], "68 #pm 0.68 MeV " + Data, "l");
//              
//         }
//          if(WichCase.Contains("_1_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 1.00 MeV " + Data, "l");
//          if(WichCase.Contains("_0_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 0.00 MeV " + Data, "l");
//          
         
     }
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
          // ATTENTION
     // PLot pic de Bragg cas ref
     
     // Etude de la différence du plateau en fonction de z
     
     //           TLegend * leg = NULL;
     NameCanvas = "1_Comparaison Pic de Bragg Beam spread"; 
     
     C_temp= new TCanvas(NameCanvas, NameCanvas);
     
     NameCanvas = "2_Comparaison Pic de Bragg Beam spread"; 
     C_temp_2 = new TCanvas(NameCanvas, NameCanvas);
     
     //      C_temp->Divide(2,1);
     
     leg2 = new TLegend(0.4864442,0.6771654,0.6741397,0.8523622,NULL,"brNDC");
     leg2->SetBorderSize(1);
     leg2->SetTextAlign(32);
     leg2->SetTextSize(0.0403071);
     leg2->SetLineColor(1);
     leg2->SetLineStyle(1);
     leg2->SetLineWidth(2);
     leg2->SetFillColor(10);
     leg2->SetFillStyle(1001);
     
     //      TLine * line2;
     int RefBin = 10;
     
     for(auto& x: map_Histo_Ref){
         
         int Setup = x.first;
         TString WichCase =  vec_Option[Setup];
         TH1D* h = x.second["hdose"];
         
         TH1D* h_Clone = (TH1D*)h->Clone();
         
         
         //         // Remise à 0 pour verif 
         //         for(int i = 1 ; i< h_Clone->GetNbinsX()+1; i++){  
         //             h_Clone->SetBinContent(i,-1000);
         //          }
         
         for(int i = 1 ; i< h_Clone->GetNbinsX()+1; i++){  
             
             h_Clone->SetBinContent(i,h->GetBinContent(i)/h->GetBinContent(RefBin)*1); 
         }
         
         
         if(WichCase.Contains("_0.68_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 0.68 MeV", "l");
         if(WichCase.Contains("_1_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 1.00 MeV", "l");
         if(WichCase.Contains("_0_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 0.00 MeV", "l");
         
         
         MaquillageGraphique(h_Clone);
         
         h_Clone->GetXaxis()->SetTitle("z [mm]");
         h_Clone->GetYaxis()->SetTitle("Rapport Dose[z]/Dose[z=0]");
         C_temp->SetGridx();
         C_temp->SetGridy();
         
         C_temp_2->SetGridx();
         C_temp_2->SetGridy();
         
         
         C_temp->cd();
         
         h_Clone->GetXaxis()->SetNdivisions(508);
         h_Clone->GetXaxis()->SetTitleOffset(1.1);
         h_Clone->GetYaxis()->SetTitleOffset(1.15);
         h_Clone->GetYaxis()->SetNdivisions(717);
         
         h_Clone->Draw("hist same");
         
         C_temp_2->cd();
         TH1D* h_Clone_Zoom = (TH1D*)h_Clone->Clone();
         
         
         h_Clone_Zoom->GetXaxis()->SetRangeUser(0,30);
         h_Clone->GetXaxis()->SetNdivisions(514);
         h_Clone->GetXaxis()->SetTitleOffset(1.1);
         h_Clone->GetYaxis()->SetNdivisions(513);
         h_Clone->GetYaxis()->SetTitleOffset(1.15);
         h_Clone_Zoom->Draw("hist same");
         
     }
     
     
     pt = new TPaveText(0.1836845,0.7049808,0.3554835,0.8505747,"blNDC");
     MaquillageGraphique_PaveText(pt);
     
     C_temp->cd();
     pt->Draw();
     leg2->Draw();
     
     C_temp_2->cd();
     pt->Draw();
     leg2->Draw();
     
     timer.Stop();
     cout << "**** Multi_Run_Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C , timer" <<endl;
     timer.Print();
     
     
     
 } // end main
 
 
 
 
 
 
 
