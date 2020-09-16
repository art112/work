
 
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
 
 
 #include "../../Include/DPGAConstants.cc" 
 #include "../../Include/ICOConstants.cc" 
 
 
string IntToString (int var , bool deleteZeroafterComma ){
    string str = std::to_string(var); 
    
    if(var ==0) return str;
    
    if(deleteZeroafterComma == 1){
        str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
        //   if(strncmp(&str.back(),".",1)==0)cout <<"str before " <<str << endl;
        if(strncmp(&str.back(),".",1)==0){str.pop_back();  /*cout <<"str after " <<str << endl;*/}
    }
    return str;
    
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
 
 
 double ToLong( const std::string& s )
 {
     std::istringstream in(s) ;
     long returnValue ;
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
     
     /////***** /// // Ne pas utiliser !!!
     
     // Nota>ons of CheVy et al., “Repor&ng and analysing sta&s&cal uncertain&es in Monte Carlo-based treatment planning”, 2006
     
     cout << "\n*** BEGIN IncertitudeSurLaDoseTotale " << Total_Dose_Squared << " " << Total_Dose << " "  << N_Primaries << endl; 
     
     //      N_Primaries=1;
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
     
     double Incertitude_Total_Dose = sqrt( NormalisationFactor * (somme1 - somme2_squared_ROOT) );
     
     
     // DoseActor
     Incertitude_Total_Dose = sqrt( (1.0/(N_Primaries-1))*( TMath::Abs(Total_Dose_Squared/N_Primaries - pow(Total_Dose/N_Primaries, 2))) )/(Total_Dose/N_Primaries);
     
     int I_Number=0;
     
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
 
 
 double IncertitudeSurLaDoseTotale_MoyennePonderee(TH1D* hTotal_Dose_Uncertainty, TH1D * hTotal_Dose,TH1D * hTotal_Hit, double N_Primaries,bool IncertitudeAbsolue = 0, int Test =0) 
 { 
     // Nota>ons of CheVy et al., “Repor&ng and analysing sta&s&cal uncertain&es in Monte Carlo-based treatment planning”, 2006
     
     cout << "\n*** BEGIN IncertitudeSurLaDoseTotale " << hTotal_Dose_Uncertainty << " " << hTotal_Dose << " "  << N_Primaries << endl; 
     
     
     double Total_Dose = hTotal_Dose->Integral();
     
     
     double Incertitude =0;
     
     for(int i=1; i<hTotal_Dose_Uncertainty->GetNbinsX()+1; i++)
     {
         Incertitude +=   hTotal_Dose_Uncertainty->GetBinContent(i) *hTotal_Hit->GetBinContent(i) ;
     }
     
     double IncertitudeDose_Abs = Incertitude / hTotal_Hit->Integral() ;
     double Incertitude_Total_Dose = IncertitudeDose_Abs * Total_Dose;
     
     
     cout << "Dose Total " << Total_Dose << " [Gy] +- " << Incertitude_Total_Dose << " ("   << IncertitudeDose_Abs*100 << " %)" << endl;
     //      
     cout << "*** END IncertitudeSurLaDoseTotale2 ***\n" << endl;
     
     if(IncertitudeAbsolue == 0) return Incertitude_Total_Dose;
     else return IncertitudeDose_Abs;
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
 
 long PGCD_Macro (long f_firstNumber,long f_secondNumber)
 {
     if (f_firstNumber == 0 and f_secondNumber == 0)//Pour le cas PGCD(0;0) et pour éviter plantage du logiciel
     {
         return 0;
     }
     
     int resultatEuclidien (0);
     
     while (f_secondNumber != 0)
     {
         resultatEuclidien = f_firstNumber % f_secondNumber;
         
         f_firstNumber = f_secondNumber;
         f_secondNumber = resultatEuclidien;
     }
     
     return f_firstNumber;
 }
 
 void MaquillageGraphique_PaveText(TPaveText * pt ,map<int, map < TString,TString >> & map_Comment_Ref,int i_Setup=0){
     
     
     TText *pt_LaTex = NULL;
     
     
     pt->SetMargin(0.02);
     pt->SetBorderSize(0);
     pt->SetFillColor(10);
     pt->SetFillStyle(1001);
     pt->SetLineWidth(1);
     pt->SetTextAlign(12);
     pt->SetTextFont(42);
     pt->SetTextSize(0.04318618); 
     
     TString SimulationTitle = "Gate " + map_Comment_Ref[i_Setup]["Gate"] + " Geant4 " + map_Comment_Ref[i_Setup]["Geant4"];
     TString Model_Hadron = map_Comment_Ref[i_Setup]["/gate/physics/addPhysicsList"];
     
     
     TString TypeParticules = map_Comment_Ref[i_Setup]["/gate/source/pbeam/gps/particle"];
     
     string TypeParticules_S = TypeParticules.Data();
     
     
     if( !TypeParticules_S.empty() )
     {
         TypeParticules_S[0] = std::toupper( TypeParticules_S[0] );
         
         for( std::size_t i = 1 ; i < TypeParticules_S.length() ; ++i )
             TypeParticules_S[i] = std::tolower( TypeParticules_S[i] );
     }
     
     TypeParticules = TypeParticules_S;
     
     long N_Primaries_Long = ToLong(map_Comment_Ref[i_Setup]["/gate/application/setTotalNumberOfPrimaries"].Data());
     //      long PGCD = PGCD_Macro(N_Primaries_Long,10);     
     //      long Reste = (double)N_Primaries_Long/(double)PGCD;
     
     //      N_Primaries_Long = 100;
     
     int NbrChiffres = (int)(1+log10(N_Primaries_Long));
     char str[NbrChiffres];
     sprintf(str, "%ld", N_Primaries_Long);
     string chiffre; 
     int nbr0 = 0;
     for(int i = 0;i < NbrChiffres ; i++){
         
         string ala;
         ala.push_back(str[i]);
         
         if(ala != "0")chiffre += str[i];
         else nbr0++;
     }
     
     TString N_Primaries;
     if(nbr0 == 0 ){
         
         cout <<"N_Primaries stay in long format" << endl;
         N_Primaries = TString::Format("N_%s %ld",TypeParticules.Data(),N_Primaries_Long);
     }
     else if(nbr0 > 0 && chiffre=="1"){
         N_Primaries = TString::Format("N_%s 10^{%d}",TypeParticules.Data(),nbr0);
     } 
     else{
         N_Primaries = TString::Format("N_%s %s.10^{%d}",TypeParticules.Data(), chiffre.c_str(),nbr0);
     } 
     
     
     //      if(Reste ==1){
     //          
     //     } 
     //     else{
     //         N_Primaries = TString::Format("N_Primaries %0.lf^{6}",ToDouble(map_Comment_Ref[i_Setup]["/gate/application/setTotalNumberOfPrimaries"].Data())/1e6);
     //     } 
     
     
     //      DoubleToString((ToDouble(map_Comment_Ref[i_Setup]["/gate/application/setTotalNumberOfPrimaries"].Data())/6))^{6};
     
     pt_LaTex = pt->AddText(SimulationTitle);
     pt_LaTex = pt->AddText(Model_Hadron);
     pt_LaTex = pt->AddText(N_Primaries);
     // 		 pt->Draw();
     
 }

 TH1D* PutOrigineToZero(TH1D* h, string SizeCube, int I_Number=0 ){
        
        I_Number++;
        
        TString nameClone;
        nameClone(h->GetName());
        nameClone += " " + IntToString(I_Number,0);
        
        //      h->SetBins (201, -100 , 100);
        TString SizeCube_TS = SizeCube;
        
        cout << "SizeCube_TS "<< SizeCube_TS << endl;
        
        int SizeExtract = extractIntegerWords(SizeCube);
        float SizeExtract_float = SizeExtract;
        if(SizeCube_TS.Contains("cm")== true){
            SizeExtract_float *=10;
        }
        
        int Nombre_deBin = h->GetNbinsX();
        
        cout << "SizeExtract " << SizeExtract << endl;
        
        
        cout << "h_dose_Clone->GetNbinsX() " << h->GetNbinsX() << endl;
        cout << "GetBinCenter 1 " <<  h->GetBinCenter(1) << endl;
        
        TH1D * NewTH1 = new TH1D( nameClone.Data(),"Profile_Z ;z [mm] ",Nombre_deBin,0,SizeExtract_float );
        
        NewTH1->SetTitle(h->GetTitle());
        
        for(int i = 1 ; i< h->GetNbinsX()+1; i++){  
            NewTH1->SetBinContent(i,h->GetBinContent(i));
        }
//         NewTH1->Draw();
        
        return NewTH1;
    }
 
 
TGraphErrors * TGraphConstruct_SerieA(){

map <float,float> map_Data_XP_Serie_A;
     
map_Data_XP_Serie_A[0.139]=1.63;
map_Data_XP_Serie_A[25.417]=2.59;
map_Data_XP_Serie_A[30.695]=5.05;
map_Data_XP_Serie_A[31.473]=6.88;
map_Data_XP_Serie_A[32.251]=3.35;

map_Data_XP_Serie_A[32.529]=1.26;
map_Data_XP_Serie_A[32.807]=0.32;
map_Data_XP_Serie_A[33.085]=0.05;

map <float,float> map_Data_XP_Serie_A_ey;
     
map_Data_XP_Serie_A_ey[0.139]=0.03;
map_Data_XP_Serie_A_ey[25.417]=0.05;
map_Data_XP_Serie_A_ey[30.695]=0.09;
map_Data_XP_Serie_A_ey[31.473]=0.1;
map_Data_XP_Serie_A_ey[32.251]=0.08;

map_Data_XP_Serie_A_ey[32.529]=0.04;
map_Data_XP_Serie_A_ey[32.807]=0.10;
map_Data_XP_Serie_A_ey[33.085]=0.10;


map <float,float> map_Data_XP_Serie_B;

// map_Data_XP_Serie_B[0.0]=43.2;
// map_Data_XP_Serie_B[27.11]=59.2;
// map_Data_XP_Serie_B[32.22]=33.3;


map <float,float> map_Data_XP_Serie_B_ey;
// map_Data_XP_Serie_B_ey[0.0]=0.69;
// map_Data_XP_Serie_B_ey[27.11]=1.15;
// map_Data_XP_Serie_B_ey[32.22]=0.55;


map <TString, TGraph*> Result_TGraph;


TGraphErrors * TempsGraph;
     
std::vector<Float_t> x_TGraph;
std::vector<Float_t> y_TGraph;
            std::vector<Float_t> ex_TGraph;
            std::vector<Float_t> ey_TGraph;
     
//             int i =-1;
            
            for(auto& x: map_Data_XP_Serie_A){
//                 i++;
                x_TGraph.push_back(x.first);
                y_TGraph.push_back(x.second);
                ex_TGraph.push_back(0.4);
                ey_TGraph.push_back(map_Data_XP_Serie_A_ey[x.first]);
            }
Int_t n = x_TGraph.size();
TempsGraph = new TGraphErrors(n,&x_TGraph[0],&y_TGraph[0],&ex_TGraph[0],&ey_TGraph[0]);
     
// float Bin = 0.01;
// 
// TH1D * h_Temps = new TH1D("h_Temps","h_Temps",4000,0,40);
//           for(auto& x: map_Data_XP_Serie_A){
//                 h_Temps->Fill(x.first,x.second);
//             }
// 
//  TempsGraph = new TGraphErrors(h_Temps);   
            
return TempsGraph;

}
 
TGraphErrors * TGraphConstruct_SerieB(double ScaleCoefPrimaries=1){

    double primaries = 1.0137e10;
    
map <float,float> map_Data_XP_Serie_B;


// map_Data_XP_Serie_B[0.000001]=41.4;
// map_Data_XP_Serie_B[25.110001]=56.1;
map_Data_XP_Serie_B[27.22]=59.2;
map_Data_XP_Serie_B[28.33]=64.9;
map_Data_XP_Serie_B[33.44]=0.0;



map_Data_XP_Serie_B[0.0]=43.2;
map_Data_XP_Serie_B[27.11]=59.2;
map_Data_XP_Serie_B[32.22]=33.3;

map <float,float> map_Data_XP_Serie_B_ey;


// map_Data_XP_Serie_B_ey[0.000001]=0.64;
// map_Data_XP_Serie_B_ey[25.110001]=0.83;
map_Data_XP_Serie_B_ey[27.22]=0.73;
map_Data_XP_Serie_B_ey[28.33]=1.52;
map_Data_XP_Serie_B_ey[33.44]=0.0;


map_Data_XP_Serie_B_ey[0.0]=0.69;
map_Data_XP_Serie_B_ey[27.11]=1.15;
map_Data_XP_Serie_B_ey[32.22]=0.55;

double Scale = ScaleCoefPrimaries / primaries;


map <TString, TGraph*> Result_TGraph;


TGraphErrors * TempsGraph;
     
std::vector<Float_t> x_TGraph;
std::vector<Float_t> y_TGraph;
            std::vector<Float_t> ex_TGraph;
            std::vector<Float_t> ey_TGraph;
     
//              int i =-1;
            
            for(auto& x: map_Data_XP_Serie_B){
//                 i++;
                x_TGraph.push_back(x.first);
                
                float Y = Scale * x.second;
                
                y_TGraph.push_back(Y);
                ex_TGraph.push_back(0.4);
        
                float eY = Scale * map_Data_XP_Serie_B_ey[x.first];
                ey_TGraph.push_back(eY);
                
              
                
            }
Int_t n = x_TGraph.size();
TempsGraph = new TGraphErrors(n,&x_TGraph[0],&y_TGraph[0],&ex_TGraph[0],&ey_TGraph[0]);
   
return TempsGraph;

} 
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
     for(int i = BinMin_Projection ; i< BinMax_Projection+1; i++){
         vec_Projection.push_back(Projection->GetBinContent(i));
     }
     
     if(vec_Projection.size()>1){
     
     std::sort (vec_Projection.begin(), vec_Projection.end(), myfunction_float);
     
     double sum = std::accumulate(vec_Projection.begin(), vec_Projection.end(), 0.0);
     double mean = sum / vec_Projection.size();
     
     double sq_sum = std::inner_product(vec_Projection.begin(), vec_Projection.end(), vec_Projection.begin(), 0.0);
     double stdev = std::sqrt(sq_sum / vec_Projection.size() - mean * mean);
     
     cout << " myvector.front() " <<  vec_Projection.front() << " myvector.back() " << vec_Projection.back() << endl;
     cout << "homogénéité " << (vec_Projection.back() - vec_Projection.front())/(vec_Projection.back() + vec_Projection.front())*100 << endl;
     
     cout << "Max_Projection " << Max_Projection << " BinMin_Projection " << BinMin_Projection << " BinMax_Projection " << BinMax_Projection << " RangeMin " << RangeMin << " RangeMax " << RangeMax<< endl;
     
     map_Results[Methode]["Range_Min"]=RangeMin;
     map_Results[Methode]["Range_Max"]=RangeMax;
     map_Results[Methode]["Amplitude_Max"]=vec_Projection.back();
     map_Results[Methode]["Amplitude_Min"]=vec_Projection.front();
     map_Results[Methode]["Mean"]=mean;
     map_Results[Methode]["STD"]=stdev;
     
     map_Results[Methode]["Homogeneite_1"]= (vec_Projection.back() - vec_Projection.front())/(vec_Projection.back() + vec_Projection.front())*100;
     
     if(mean>0) map_Results[Methode]["Homogeneite_2"]= stdev/mean*100;
     else map_Results[Methode]["Homogeneite_2"]= 0;
     
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
     
     if(mean>0) map_Results[Methode]["Homogeneite_2"]= stdev/mean*100;
     else map_Results[Methode]["Homogeneite_2"]= 0;
     
     vec_Projection.clear();
     }
     else{
        cout <<  "No heterogénéité " << endl;
    }
     
     return;
 } 
 
 void CompileHomogeneite_TGraph(TH1D* Projection, map < int , TGraph* > map_Results_TGraph, float Pas_Commande=-1){ 
     int Methode = 0;
     
     
     // On pars de 0 et ajoute un delta z pour obtenir une homogéniétié à partir du centre 
     // 
     
     double Mean_Projection = Projection->GetMean();
     if(Mean_Projection < 0.001) Mean_Projection = 0;
     int bin_Mean = Projection->FindBin(Mean_Projection);
     
     double Max_Projection = Projection->GetMaximum();
     
     int NbrDeBin = Projection->GetNbinsX();
     
     Double_t Bin_Min_Value = Projection->GetBinCenter(1)-Projection->GetBinWidth(1)/2;
     Double_t Bin_Max_Value = Projection->GetBinCenter(NbrDeBin)+Projection->GetBinWidth(NbrDeBin)/2;
     
     Double_t SizeHisto = Bin_Max_Value - Bin_Min_Value ;
     
     Double_t Pas = Projection->GetBinWidth(1);
     
     int Pas_Bin = Pas_Commande / Pas;
     if(Pas_Bin <0 ) Pas_Bin = 1;
     
     cout << "Caracteristique histo " << NbrDeBin << " " << Bin_Min_Value << " " << Bin_Max_Value << " " << SizeHisto << " Pas[mm] " << Pas << endl;
     cout << "Bin loweredge " << Projection->GetBinLowEdge(1) << endl;
     // Nbr de binma
     // Valur max histo valeur min 
     // Pas 
     
     map <int, map < TString , vector <float> >> map_Results_Temps; 
     
     for(int i = bin_Mean; i < NbrDeBin+1; i++){
         
         int i_min = bin_Mean - i ;
         int i_max = bin_Max  + i ;
         
         vector<float> vec_Projection;
         for(int i = i_min ; i< i_max+1; i++){
             vec_Projection.push_back(Projection->GetBinContent(i));
         }
         
              if(vec_Projection.size()>1){
     
    std::sort (vec_Projection.begin(), vec_Projection.end(), myfunction_float);
     
     double sum = std::accumulate(vec_Projection.begin(), vec_Projection.end(), 0.0);
     double mean = sum / vec_Projection.size();
     
     double sq_sum = std::inner_product(vec_Projection.begin(), vec_Projection.end(), vec_Projection.begin(), 0.0);
     double stdev = std::sqrt(sq_sum / vec_Projection.size() - mean * mean);
         
         // Calcul Methode Dose Max - Dose Min
             
                  
            }     
         
         
     }
     
     
     
     // Method_1 Find 80% du max
     
     
     
     
//      Methode++;
//      
//      float Max_Projection = Projection->GetMaximum();
//      
//      int BinMin_Projection = (Float_t)Projection->FindFirstBinAbove(Max_Projection*0.8) ;
//      int BinMax_Projection = (Float_t)Projection->FindLastBinAbove(Max_Projection*0.8) ;
//      
//      float RangeMin = Projection->GetBinCenter(BinMin_Projection);
//      float RangeMax = Projection->GetBinCenter(BinMax_Projection);
//      
//      vector<float> vec_Projection;
//      for(int i = BinMin_Projection ; i< BinMax_Projection+1; i++){
//          vec_Projection.push_back(Projection->GetBinContent(i));
//      }
//      
//      if(vec_Projection.size()>1){
//      
//      std::sort (vec_Projection.begin(), vec_Projection.end(), myfunction_float);
//      
//      double sum = std::accumulate(vec_Projection.begin(), vec_Projection.end(), 0.0);
//      double mean = sum / vec_Projection.size();
//      
//      double sq_sum = std::inner_product(vec_Projection.begin(), vec_Projection.end(), vec_Projection.begin(), 0.0);
//      double stdev = std::sqrt(sq_sum / vec_Projection.size() - mean * mean);
//      
//      cout << " myvector.front() " <<  vec_Projection.front() << " myvector.back() " << vec_Projection.back() << endl;
//      cout << "homogénéité " << (vec_Projection.back() - vec_Projection.front())/(vec_Projection.back() + vec_Projection.front())*100 << endl;
//      
//      cout << "Max_Projection " << Max_Projection << " BinMin_Projection " << BinMin_Projection << " BinMax_Projection " << BinMax_Projection << " RangeMin " << RangeMin << " RangeMax " << RangeMax<< endl;
//      
//      map_Results[Methode]["Range_Min"]=RangeMin;
//      map_Results[Methode]["Range_Max"]=RangeMax;
//      map_Results[Methode]["Amplitude_Max"]=vec_Projection.back();
//      map_Results[Methode]["Amplitude_Min"]=vec_Projection.front();
//      map_Results[Methode]["Mean"]=mean;
//      map_Results[Methode]["STD"]=stdev;
//      
//      map_Results[Methode]["Homogeneite_1"]= (vec_Projection.back() - vec_Projection.front())/(vec_Projection.back() + vec_Projection.front())*100;
//      
//      if(mean>0) map_Results[Methode]["Homogeneite_2"]= stdev/mean*100;
//      else map_Results[Methode]["Homogeneite_2"]= 0;
//      
//      // Method_1 Find 80% du max + 
//      Methode++;
//      
//      vec_Projection.clear();
//      BinMin_Projection = Projection->FindBin(-1*(RangeMax * 0.80));
//      BinMax_Projection = Projection->FindBin(1*(RangeMax * 0.80));
//      
//       RangeMin = Projection->GetBinCenter(BinMin_Projection);
//       RangeMax = Projection->GetBinCenter(BinMax_Projection);
//      
//      
//      for(int i = BinMin_Projection ; i< BinMax_Projection; i++){
//          vec_Projection.push_back(Projection->GetBinContent(i));
//      }
//      std::sort (vec_Projection.begin(), vec_Projection.end(), myfunction_float);
//      
//       sum = std::accumulate(vec_Projection.begin(), vec_Projection.end(), 0.0);
//       mean = sum / vec_Projection.size();
//      
//       sq_sum = std::inner_product(vec_Projection.begin(), vec_Projection.end(), vec_Projection.begin(), 0.0);
//       stdev = std::sqrt(sq_sum / vec_Projection.size() - mean * mean);
//      
//            map_Results[Methode]["Range_Min"]=RangeMin;
//      map_Results[Methode]["Range_Max"]=RangeMax;
//      map_Results[Methode]["Amplitude_Max"]=vec_Projection.back();
//      map_Results[Methode]["Amplitude_Min"]=vec_Projection.front();
//      map_Results[Methode]["Mean"]=mean;
//      map_Results[Methode]["STD"]=stdev;
//      
//      map_Results[Methode]["Homogeneite_1"]= (vec_Projection.back() - vec_Projection.front())/(vec_Projection.back() + vec_Projection.front())*100;
//      
//      if(mean>0) map_Results[Methode]["Homogeneite_2"]= stdev/mean*100;
//      else map_Results[Methode]["Homogeneite_2"]= 0;
//      
//      vec_Projection.clear();
//      }
//      else{
//         cout <<  "No heterogénéité " << endl;
//     }
     
     return;
 } 

 // Macro include
 
 using namespace std;
 
 //********************** Main Function *************************************************************************************************************************
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaractérisationDuSetup.C
 
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Multi_Run_Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C
 
 void Analyse_Dose_ManipZebrafish_Stack(){
     
     // Rajouter Ekin DOSE Cube.
     
     
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
     
     TString TitleSave;
     
     ICOConstants *const_ICO = new ICOConstants();
     
     int Save = 1 ;
     
     
   
    
     ///****** *********** ************//
     string Projet = "z_Projet_Manip_ZebraFish_11_03_20";
     string NameSetup = "";
     string CompleteNameSetup = "";
     ///****** *********** ************//
     
     bool Farm = true;
     
     string PathFile = "/media/sf_PartageVbox8/GATE/All_Simulation/"+Projet +"/";
     
      PathFile = "/media/sf_PartageVbox8/Results/"+Projet +"/";
     
     if(Farm == true){
         PathFile = "/home/gate/nantes_sps/abongran/All_Results/"+Projet +"/9.0/";
     }
     
//        PathFile = "/media/sf_PartageVbox8/Results/"+Projet +"/";
     
     string Coef_Event = "10";
     string Unit_Event = "M" ;
     
     std::vector<string> vec_Option {"v1_68MeV_0_","v1_68MeV_0.68_","v1_68MeV_1_"};
     
     if(Farm == true){

//          output_Ref_Cube_G4_PLEXIGLASS_vMZfv4_Stack_Manip_ZebraFish_110320_v1_68MeV_2_2M
         
          vec_Option.clear();
         NameSetup="Cube_G4_PLEXIGLASS_vMZfv4.2_Stack_Manip_ZebraFish_110320_v1";
//          CompleteNameSetup= NameSetup +"_68MeV_0_" ; vec_Option.push_back(CompleteNameSetup);
         CompleteNameSetup= NameSetup +"_68MeV_0.68_" ; vec_Option.push_back(CompleteNameSetup);
//          CompleteNameSetup= NameSetup +"_68MeV_1_" ; vec_Option.push_back(CompleteNameSetup);
//          CompleteNameSetup= NameSetup +"_68MeV_2_" ; vec_Option.push_back(CompleteNameSetup);
         
     }
     
     bool Etude_Epaisseur_Position = false;
     
     bool Etude_Film = true;
     
     
     // Path Output
     if(Farm==true){Projet+="_FARM";}
     
     // Local
     TString PathOutput = "/media/sf_PartageVbox8/Analyse_Results/";
     TString NameFile = NameSetup + "_" +Coef_Event + Unit_Event;     
     TString TypeAnalyse = "Analyse_Dose";
     
     TString Total_Path = PathOutput +Projet;
     TString CheckFileAnalysis = "mkdir "+ PathOutput  ; system(CheckFileAnalysis.Data());
     CheckFileAnalysis = "mkdir "+ Total_Path  ; system(CheckFileAnalysis.Data());
     Total_Path += "/" + NameFile ; CheckFileAnalysis = "mkdir "+ Total_Path  ; system(CheckFileAnalysis.Data());
     Total_Path += "/" + TypeAnalyse ; CheckFileAnalysis = "mkdir "+ Total_Path  ; system(CheckFileAnalysis.Data());
     TString CheckFileAnalysis_rm = "rm "+ Total_Path + "/*" ; system(CheckFileAnalysis_rm.Data());
    
     std::vector<string> Category_Of_Analyse {"Epaisseur", "Pos"};
     std::vector<string> type_Of_Analyse_2 {"Min", "Max"};
     
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
     
     
     // FILM
      map<int, map< TString, map< int, map < TString ,TH2F* >>>>map_Histo_Film;
     
     int NombreDeFilm=8;
     
     int i_Setup = 0;
     
     int CompteurCaseForResult = 0;
     
         double N_Real_Event = 0;
         double N_Primaries = 0;
     
     
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
         
         TString File_To_Read = "log";
         totalPathReadInformation =    PathInput+"/" +File_To_Read + ".txt";
         const_ICO->LoadSetup_with_RunSystem_GateLog(map_Comment_Ref, totalPathReadInformation.Data(), i_Setup ,0);
         
         
//          Stack_1_Actor-DoseToWater-Squared
         
         cout <<"PathInput " << PathInput << endl; 
         
         bool ToWater = false; int TypeDose = 1;
         TString TS_DoseToWater = "";   TString TS_TypeDose = "";
         if(ToWater == true)TS_DoseToWater = "ToWater";
         
         if(TypeDose == 0)TS_TypeDose = "dose_Cube_after_All";
         else TS_TypeDose = "Stack_1_Actor";
         
//          _Frite
//          Dose_Cube_After_Frite-Dose
//          dose_Cube_After_Frite-Dose.root
         
         NameInput = PathInput + "/" + TS_TypeDose+ "-Dose"+ TS_DoseToWater +".root";
         TFile *myFile_Ref_Dose = TFile::Open(NameInput.Data()); 
         TH1D * h_dose = (TH1D*)myFile_Ref_Dose->Get("histo");  
         TH1D* h_dose_Clone = (TH1D*)h_dose->Clone();
         
         NameInput = PathInput + "/" + TS_TypeDose +"-DoseToWater.root";
         TFile *myFile_Ref_DoseToWater = TFile::Open(NameInput.Data()); 
         TH1D * h_doseToWater = (TH1D*)myFile_Ref_DoseToWater->Get("histo");  
         TH1D* h_doseToWater_Clone = (TH1D*)h_doseToWater->Clone();
         
         
         NameInput = PathInput + "/" + TS_TypeDose +"-Dose" + TS_DoseToWater + "-Uncertainty.root";
         TFile *myFile_Ref_Dose_Uncertity = TFile::Open(NameInput.Data()); 
         TH1D * h_dose_Uncertainty = (TH1D*)myFile_Ref_Dose_Uncertity->Get("histo");  
         TH1D* h_dose_Uncertainty_Clone = (TH1D*)h_dose_Uncertainty->Clone();
         
         
         NameInput = PathInput + "/" + TS_TypeDose +"-NbOfHits.root";
         TFile *myFile_Ref_Dose_NbOfHits = TFile::Open(NameInput.Data()); 
         TH1D * h_dose_NbOfHits = (TH1D*)myFile_Ref_Dose_NbOfHits->Get("histo");  
         TH1D* h_dose_NbOfHits_Clone = (TH1D*)h_dose_NbOfHits->Clone();
         
         // Calcul de l'incertitude 
         
         NameInput = PathInput + "/" + TS_TypeDose +"-Dose" +TS_DoseToWater +"-Squared.root";
         TFile *myFile_Ref_Dose_Squared = TFile::Open(NameInput.Data()); 
         TH1D * h_dose_Dose_Squared = (TH1D*)myFile_Ref_Dose_Squared->Get("histo");  
         TH1D* h_dose_Dose_Squared_Clone = (TH1D*)h_dose_Dose_Squared->Clone();
         
         
         for(int i_Film=1; i_Film < NombreDeFilm+1; i_Film++ ){
             NameInput = PathInput + "/F_" + IntToString(i_Film,0) + "_ActiveLayer_Actor";
//              cout << "NameInput " << NameInput << endl;
             
             std::vector<string> vec_Category_Dose{"Dose","DoseToWater","NbOfHits"};
//              std::vector<string> vec_Category_Dose_2{"","-Uncertainty","-Squared"};
              std::vector<string> vec_Category_Dose_2{"","-Uncertainty"};
             
             for(auto& x_film: vec_Category_Dose){
                   TString x_Film_TS = x_film;
                 for(auto& y_film: vec_Category_Dose_2){
                     
                     TString y_Film_TS = y_film;
                     if(x_Film_TS .Contains("NbOfHits")== true && y_Film_TS != ""){
                         continue;
                    }
                     
                     TString NameInputFilm =  NameInput + "-" + x_Film_TS +  y_Film_TS + ".root";
                     TString NameCase_Film=x_Film_TS  + y_Film_TS;
                     
//                      cout << "NameInputFilm " << NameInputFilm << endl;
                     
                     TFile *myFile_Ref_Dose = TFile::Open(NameInputFilm.Data()); 
                     TH2F * h_film = (TH2F*)myFile_Ref_Dose->Get("histo");  
                     TH2F* h_film_Clone = (TH2F*)h_film->Clone();
                     map_Histo_Film[i_Setup][NameCase][i_Film][NameCase_Film]=h_film_Clone;
//                      h_film_Clone->Draw("colz");
                 }
                 
             }
             
         }
         
         
         
         // Nombre d'événements : 2 Méthode de calcul -> 
         // -> Nombre de "primaries" Nombre de particules lancées dans Gate/G4 ex : 10 M
         // -> Nombre de particules qui rentre dans la cible d'eau ie après le collimateur. N_Event << N_Primaries (~3.5%)
         
     
         
         NameInput = PathInput + "/PhaseSpace_Entrance_Cube_After.root";
         TFile *myFile_Ref_Caracterisation_Output = TFile::Open(NameInput.Data()); 
//          if(myFile_Ref_Caracterisation_Output){
//              TTree* TTMakePhaseSpace = (TTree*) myFile_Ref_Caracterisation_Output->Get("PhaseSpace");
//              
//              TString NameTTreeDraw = "Ekine >> h1(7000,0,70)";
//              TTMakePhaseSpace->Draw(NameTTreeDraw.Data(),"","goff");
//              TH1D* hNameHisto_Ekin =(TH1D*)gDirectory->Get("h1")->Clone("hNameHisto_Ekin");
//              
//              N_Real_Event =hNameHisto_Ekin->GetEntries();
//          }
         
         
         cout << "Total Event :: " << N_Real_Event << endl;
         
         if(N_Real_Event==0)N_Real_Event=300000;  //  
         
         
         map_Histo_Ref[i_Setup]["hdose"]=h_dose_Clone;
         map_Histo_Ref[i_Setup]["hdoseToWater"]=h_doseToWater_Clone;
         
         
         map_Histo_Ref[i_Setup]["hdose_Uncertainty"]=h_dose_Uncertainty_Clone;
         map_Histo_Ref[i_Setup]["h_dose_NbOfHits"]=h_dose_NbOfHits_Clone;
         map_Histo_Ref[i_Setup]["h_dose_Dose_Squared"]=h_dose_Dose_Squared_Clone;
//          map_Histo_Ref[i_Setup]["hdoseToWater"]=h_doseToWater;
         
//          string SizeCube =  map_Comment_Ref[i_Setup]["/gate/Cube_After/geometry/setZLength"].Data();
         
         string SizeCube =  map_Comment_Ref[i_Setup]["/gate/Stack_1/geometry/setZLength"].Data();
         
         
         
         N_Primaries = ToDouble(map_Comment_Ref[i_Setup]["/gate/application/setTotalNumberOfPrimaries"].Data());
         cout << "N_Primaries " << N_Primaries << endl;
         
         map_Histo_Ref[i_Setup]["hdose"] = const_ICO->PutOrigineToZero(map_Histo_Ref[i_Setup]["hdose"],SizeCube);
         map_Histo_Ref[i_Setup]["hdose_Uncertainty"] = const_ICO->PutOrigineToZero(map_Histo_Ref[i_Setup]["hdose_Uncertainty"],SizeCube);
         
         map_Histo_Ref[i_Setup]["hdoseToWater"] = const_ICO->PutOrigineToZero(map_Histo_Ref[i_Setup]["hdoseToWater"],SizeCube);
         
//          map_Histo_Ref[i_Setup]["h_dose_NbOfHits"] = const_ICO->PutOrigineToZero(map_Histo_Ref[i_Setup]["h_dose_NbOfHits"],SizeCube);
         
//         map_Histo_Ref[i_Setup]["hdoseToWater"] = const_ICO->PutOrigineToZero(map_Histo_Ref[i_Setup]["hdoseToWater"],SizeCube);
         
         // Representation des erreur de dose pour le Graph
         for(int i = 1 ; i< h_dose_Clone->GetNbinsX()+1; i++){  
             double Error_Calcul = map_Histo_Ref[i_Setup]["hdose_Uncertainty"]->GetBinContent(i) * map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i);
             //              cout << "Bin error " <<map_Histo_Ref[i_Setup]["hdose_Uncertainty"]->GetBinContent(i)*100 << " " <<  map_Histo_Ref[i_Setup]["hdose"]->GetBinContent(i) << " " << Error_Calcul << endl;
             map_Histo_Ref[i_Setup]["hdose"]->SetBinError(i, Error_Calcul );
             //        h_dose_Clone->SetBinError(i,0);
         }
         
         
         
   
         
         
         //            NameCanvas = constants->NameCanvas(Number_temp, "68MeV_0");
         //              C_temp = new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
         
         NameCanvas = const_ICO->NameCanvas(Number_temp, vec_Ref_Case[i_Setup]);
         C_temp= new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
         
         //          gStyle->SetOptStat(0);
         C_temp->Divide(2,2);
         
         C_temp->cd(1);
         map_Histo_Ref[i_Setup]["hdose"]->Draw("hist");
         
         gStyle->SetOptStat("i");
         
        TPaveStats *st = (TPaveStats*)map_Histo_Ref[i_Setup]["hdose"]->FindObject("stats");
//         st->SetX1NDC(0.2134327); //new x start position
//         st->SetX2NDC(0.4215481); //new x end position
//         st->SetY1NDC(0.6646904); //new x start position
//         st->SetY2NDC(0.8314394); //new x end position
         
             pt = new TPaveText(0.1830743,0.7079208,0.4340818,0.8534653,"blNDC");
            MaquillageGraphique_PaveText(pt,map_Comment_Ref);
            pt->Draw();
        
         
         C_temp->cd(2);
         map_Histo_Ref[i_Setup]["h_dose_Dose_Squared"]->Draw("hist");
         
                 gStyle->SetOptStat("eMRi");
         
        TPaveStats *st2 = (TPaveStats*)map_Histo_Ref[i_Setup]["hdose"]->FindObject("stats");
        st2->SetX1NDC(0.2134327); //new x start position
        st2->SetX2NDC(0.4215481); //new x end position
        st2->SetY1NDC(0.6646904); //new x start position
        st2->SetY2NDC(0.8314394); //new x end position
         
         
         
         C_temp->cd(3);
         map_Histo_Ref[i_Setup]["hdose_Uncertainty"]->Draw("hist");
         gPad->SetLogy();
         
                 gStyle->SetOptStat("eMRi");
         
        TPaveStats *st3 = (TPaveStats*)map_Histo_Ref[i_Setup]["hdose"]->FindObject("stats");
        st3->SetX1NDC(0.2134327); //new x start position
        st3->SetX2NDC(0.4215481); //new x end position
        st3->SetY1NDC(0.6646904); //new x start position
        st3->SetY2NDC(0.8314394); //new x end position

         
         
         C_temp->cd(4);
         map_Histo_Ref[i_Setup]["h_dose_NbOfHits"]->Draw("hist");
         
                          gStyle->SetOptStat("eMRi");
         
        TPaveStats *st4 = (TPaveStats*)map_Histo_Ref[i_Setup]["hdose"]->FindObject("stats");
        st4->SetX1NDC(0.2134327); //new x start position
        st4->SetX2NDC(0.4215481); //new x end position
        st4->SetY1NDC(0.6646904); //new x start position
        st4->SetY2NDC(0.8314394); //new x end position
         
         
         
         
         
            if(Save > 0){
                    TitleSave = C_temp->GetName();
                    C_temp->SaveAs( TString::Format("%s/%s.png",Total_Path.Data(),TitleSave.Data()));
                }
   
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************   
   
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
         
         
         
         
         //          double somme1 = totalDose_squared / N_Primaries;
         //          double somme2 = totalDose / N_Primaries;
         //          
         //          double IncertitudeAuCarre = (1/(float(N_Primaries-1)))* ( (somme1-(somme2))*(somme1-(somme2)) );
         //          float incertitude_Dose = TMath::Sqrt(IncertitudeAuCarre);
         //          
         //          cout << "somme1 " << somme1 << " somme2 " << somme2 << endl;
         //          cout << "IncertitudeAuCarre " << IncertitudeAuCarre << " incertitude_Dose " << incertitude_Dose << endl;
         //          
         //          if(totalDose>0){incertitude_Dose = incertitude_Dose / totalDose;}
         //          else incertitude_Dose = 0;
         //          incertitude_Dose = incertitude_Dose * N_Primaries;
         //          
         //          incertitude_Dose = TMath::Sqrt((1/(N_Real_Event-1))*( ((totalDose_squared/N_Real_Event)-(totalDose/N_Real_Event))*((totalDose_squared/N_Real_Event)-(totalDose/N_Real_Event))));
         //          
         //          incertitude_Dose /= totalDose;
         //          incertitude_Dose *=  N_Real_Event;
         //          
         //          cout << "incertitude-Dose Relative [0:1] " << incertitude_Dose << endl;
         
         // ************************************************
         // Fonction incertitude 
         //          incertitude_Dose = IncertitudeSurLaDoseTotale(map_Histo_Ref[i_Setup]["h_dose_Dose_Squared"]->Integral(),map_Histo_Ref[i_Setup]["hdose"]->Integral(), N_Primaries,1);
         
         
         double incertitude_Dose =IncertitudeSurLaDoseTotale_MoyennePonderee(map_Histo_Ref[i_Setup]["hdose_Uncertainty"], map_Histo_Ref[i_Setup]["hdose"],map_Histo_Ref[i_Setup]["h_dose_NbOfHits"], N_Primaries,0, 1) ;
         
         
         
         
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
                     
                     incertitude_Dose =IncertitudeSurLaDoseTotale_MoyennePonderee(map_Histo_StudyCase[i_Setup][x][y]["hdose_Uncertainty"], map_Histo_StudyCase[i_Setup][x][y]["hdose"],map_Histo_StudyCase[i_Setup][x][y]["h_dose_NbOfHits"], N_Primaries,0, 1) ;
                     
                     
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

                 
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
                 NameCanvas = const_ICO->NameCanvas(Number_temp, vec_Ref_Case[i_Setup]);
                 
                 NameCanvas += x; 
                 
                 C_temp= new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
                 

                 
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
                 
                      
                if(Save > 0){
                    TitleSave = C_temp->GetName();
                    C_temp->SaveAs( TString::Format("%s/%s.png",Total_Path.Data(),TitleSave.Data()));
                }
     
     
                 
                 
             } // end type_Of_Analyse_2
             
             
         } // end bool Etude_Epaisseur_Position
         
         
         
     } 
     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
     
     
     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
// Resultats pour excel
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************

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

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************    
    
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
     // ATTENTION
     // PLot pic de Bragg cas ref
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
     
     TLegend * leg = NULL;
     NameCanvas = "a_Comparaison Pic de Bragg Beam spread"; 
     C_temp= new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
     
//      TLegend *leg = new TLegend(0.373431,0.7094862,0.8043933,0.8517787,NULL,"brNDC");
//    leg->SetBorderSize(1);
//    leg->SetTextAlign(32);
//    leg->SetTextSize(0.0403071);
//    leg->SetLineColor(1);
//    leg->SetLineStyle(1);
//    leg->SetLineWidth(2);
//    leg->SetFillColor(10);
//    leg->SetFillStyle(1001);
     
     TLegend *leg2 = new TLegend(0.1935146,0.5405138,0.7845188,0.6828063,NULL,"brNDC");
     leg2->SetBorderSize(1);
     leg2->SetTextAlign(32);
     leg2->SetTextSize(0.0403071);
     leg2->SetLineColor(1);
     leg2->SetLineStyle(1);
     leg2->SetLineWidth(2);
     leg2->SetFillColor(10);
     leg2->SetFillStyle(1001);
     
     leg2->SetMargin(0.1);
     
     TLine * line2;
     
     TH1D * histo_de_reference = NULL;
     
     for(auto& x: map_Histo_Ref){
         
         int Setup = x.first;
         
         
         TString WichCase =  vec_Option[Setup];
         
         x.second["hdose"]->SetTitle("Dose");
         
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
         
         cout << "-> " << HalfMaxCoords.X() << " | " << HalfMaxCoords.Z()  << " | " <<  HalfMaxCoords.Y()  << " | " <<  HalfMaxCoords.T() << endl;
         
         line2 -> SetLineStyle(9);
         line2 -> SetHorizontal(kFALSE);
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
         
         double Integral_Dose = x.second["hdose"]->Integral();
         
          std::ostringstream streamObj4;
         // Set Fixed -Point Notation
         streamObj4 << std::fixed;
         // Set precision to 2 digits
         streamObj4 << std::setprecision(2);
         //Add double to stream
         streamObj4 << DoubleToString(Integral_Dose);
               // Get string from output string stream
         std::string strObj4 = streamObj4.str();
         
         
         Data += " Integral " + strObj4 ;
         
         
         
         
         if(WichCase.Contains("_0.68_")) { 
             
             leg2->AddEntry(x.second["hdose"], "68 #pm 0.68 MeV " + Data, "l");
             
         }
         if(WichCase.Contains("_1_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 1.00 MeV " + Data, "l");
         if(WichCase.Contains("_0_")) { leg2->AddEntry(x.second["hdose"], "68 #pm 0.00 MeV " + Data, "l");
             histo_de_reference = (TH1D*)x.second["hdose"]->Clone();   
         }
         
         
     }
     
        pt = new TPaveText(0.1830743,0.7079208,0.4340818,0.8534653,"blNDC");
     MaquillageGraphique_PaveText(pt,map_Comment_Ref);
     pt->Draw();
     
     leg2->Draw();
     
     //      map_Histo_Ref[i_Setup]["hdose"]
     
            if(Save > 0){
         TitleSave = C_temp->GetName();
         C_temp->SaveAs( TString::Format("%s/%s.png",Total_Path.Data(),TitleSave.Data()));
    }
     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
     
     
     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
     // ATTENTION
     // PLot pic de Bragg cas ref
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
     NameCanvas = "b_Comparaison Pic de Bragg Beam spread"; 
     C_temp= new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
     
//      TLegend *leg = new TLegend(0.373431,0.7094862,0.8043933,0.8517787,NULL,"brNDC");
//    leg->SetBorderSize(1);
//    leg->SetTextAlign(32);
//    leg->SetTextSize(0.0403071);
//    leg->SetLineColor(1);
//    leg->SetLineStyle(1);
//    leg->SetLineWidth(2);
//    leg->SetFillColor(10);
//    leg->SetFillStyle(1001);
     
      leg2 =new TLegend(0.2003454,0.5039604,0.6781808,0.6465347,NULL,"brNDC");
     leg2->SetBorderSize(1);
     leg2->SetTextAlign(32);
     leg2->SetTextSize(0.0403071);
     leg2->SetLineColor(1);
     leg2->SetLineStyle(1);
     leg2->SetLineWidth(2);
     leg2->SetFillColor(10);
     leg2->SetFillStyle(1001);
     
     leg2->SetMargin(0.1);
     
      line2;
     
    histo_de_reference = NULL;
     
     for(auto& x: map_Histo_Ref){
         
         int Setup = x.first;
         
         
         TString WichCase =  vec_Option[Setup];
         
         x.second["hdoseToWater"]->SetTitle("DoseToWater");
         
         x.second["hdoseToWater"]->GetXaxis()->SetTitle("z [mm]");
         //    h1->GetXaxis()->SetRange(1,34);
         x.second["hdoseToWater"]->GetXaxis()->SetNdivisions(507);
         x.second["hdoseToWater"]->GetXaxis()->SetLabelFont(42);
         x.second["hdoseToWater"]->GetXaxis()->SetLabelOffset(0.015);
         x.second["hdoseToWater"]->GetXaxis()->SetLabelSize(0.05);
         x.second["hdoseToWater"]->GetXaxis()->SetTitleSize(0.05);
         x.second["hdoseToWater"]->GetXaxis()->SetTitleFont(42);
         x.second["hdoseToWater"]->GetXaxis()->SetTitleOffset(1.1);
         x.second["hdoseToWater"]->GetYaxis()->SetTitle("Dose [Gy]");
         //    h1->GetYaxis()->SetRange(1,66);
         x.second["hdoseToWater"]->GetYaxis()->SetNdivisions(507);
         x.second["hdoseToWater"]->GetYaxis()->SetLabelFont(42);
         x.second["hdoseToWater"]->GetYaxis()->SetLabelOffset(0.015);
         x.second["hdoseToWater"]->GetYaxis()->SetTitleOffset(0.85);
         x.second["hdoseToWater"]->GetYaxis()->SetLabelSize(0.05);
         x.second["hdoseToWater"]->GetYaxis()->SetTitleSize(0.05);
         x.second["hdoseToWater"]->GetYaxis()->SetTitleFont(42);
         
         C_temp->SetGridx();
         C_temp->SetGridy();
         
         gStyle->SetOptStat(0);
         
         //          map_Comment_Ref[i_Setup]["/gate/Cube_After/geometry/setHeight"].Data();
         x.second["hdoseToWater"]->SetLineColor(Setup+1);
         
         
         C_temp->cd();
         x.second["hdoseToWater"]->Draw("hist same");
         
         double Profil_entries ; TLorentzVector HalfMaxCoords ; double FWHM ; double Max ;int binmax; double x_Max_Line;
         double     PourcentageMax = 80;
         HalfMaxCoords =  const_ICO->FindHalfMaxCoords(x.second["hdoseToWater"],PourcentageMax);
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
         HalfMaxCoords =  const_ICO->FindHalfMaxCoords(x.second["hdoseToWater"],PourcentageMax);
         FWHM = const_ICO->m_FWHM;
         Max = const_ICO->fMax;
         
         
         line2 = new TLine(HalfMaxCoords.X(), HalfMaxCoords.Z(),HalfMaxCoords.Y(), HalfMaxCoords.T() );
         
         cout << "-> " << HalfMaxCoords.X() << " | " << HalfMaxCoords.Z()  << " | " <<  HalfMaxCoords.Y()  << " | " <<  HalfMaxCoords.T() << endl;
         
         line2 -> SetLineStyle(9);
         line2 -> SetHorizontal(kFALSE);
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
             
             leg2->AddEntry(x.second["hdoseToWater"], "68 #pm 0.68 MeV " + Data, "l");
             
         }
         if(WichCase.Contains("_1_"))  leg2->AddEntry(x.second["hdoseToWater"], "68 #pm 1.00 MeV " + Data, "l");
         if(WichCase.Contains("_0_")) { leg2->AddEntry(x.second["hdoseToWater"], "68 #pm 0.00 MeV " + Data, "l");
             histo_de_reference = (TH1D*)x.second["hdoseToWater"]->Clone();   
         }
         
         
     }
     
        pt = new TPaveText(0.1830743,0.7079208,0.4340818,0.8534653,"blNDC");
     MaquillageGraphique_PaveText(pt,map_Comment_Ref);
     pt->Draw();
     
     leg2->Draw();
     
     //      map_Histo_Ref[i_Setup]["hdoseToWater"]
     
            if(Save > 0){
         TitleSave = C_temp->GetName();
         C_temp->SaveAs( TString::Format("%s/%s.png",Total_Path.Data(),TitleSave.Data()));
    }
     
     
  //**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
        
     
     //**********************************************************************************************************************************************
//**********************************************************************************************************************************************
     // ATTENTION
     // PLot pic de Bragg cas ref
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
     
        NameCanvas = "c_Comparaison Pic de Bragg Beam spread SimData"; 
     C_temp= new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
     
//      TLegend *leg = new TLegend(0.373431,0.7094862,0.8043933,0.8517787,NULL,"brNDC");
//    leg->SetBorderSize(1);
//    leg->SetTextAlign(32);
//    leg->SetTextSize(0.0403071);
//    leg->SetLineColor(1);
//    leg->SetLineStyle(1);
//    leg->SetLineWidth(2);
//    leg->SetFillColor(10);
//    leg->SetFillStyle(1001);
     
     leg2 = new TLegend(0.4594128,0.580198,0.6729994,0.8534653,NULL,"brNDC");
     leg2->SetBorderSize(1);
     leg2->SetTextAlign(22);
     leg2->SetTextSize(0.0403071);
     leg2->SetLineColor(1);
     leg2->SetLineStyle(1);
     leg2->SetLineWidth(2);
     leg2->SetFillColor(10);
     leg2->SetFillStyle(1001);
     
  
     
     leg2->SetMargin(0.1);

     
    histo_de_reference = NULL;
     
     float Scale_Data = 42.3;
     float Scale_DataX = 0;
     
    Scale_Data = 58.7;
    Scale_DataX = 27.0;
     
    // Scale coef 
    
     double Scale_Coef =-1;
     bool First = true;
     
     TMultiGraph * multi_general= new TMultiGraph();
     
     
     double TotalParticuleExperiment = 437500000;
     
     TGraphErrors * gr_temps;
     
     for(auto& x: map_Histo_Ref){
         
         
         
         
         
         
         
         
         int Setup = x.first;
         TString WichCase =  vec_Option[Setup];
         TH1D* h = x.second["hdose"];
         
         TH1D*h_un= x.second["hdose_Uncertainty"];
         
         h = x.second["hdoseToWater"];
         h_un= x.second["hdose_Uncertainty"];
         
         TH1D* h_Clone = (TH1D*)h->Clone();
         
         if(WichCase.Contains("_0.68_")) { 
             Scale_Coef = (4.419118914 * TotalParticuleExperiment) / (double)N_Primaries;
         }
         if(WichCase.Contains("_1_")) Scale_Coef = (4.408121523* TotalParticuleExperiment) / (double)N_Primaries;
         if(WichCase.Contains("_0_"))Scale_Coef = (4.397653412 * TotalParticuleExperiment) /(double)N_Primaries;
         
         
         
         
//          if(First == true){
// //              Scale_Coef = Scale_Data / (h_Clone->GetBinContent((h_Clone->FindBin(Scale_DataX)))); 
//              
//              Scale_Coef = (4.3976 * 1.0137e10) / 1e6;
//              cout << "Scale_Coef " << Scale_Coef << endl;
//              First = false;
//         }
             
         for(int i = 1 ; i< h_Clone->GetNbinsX()+1; i++){  
             
             h_Clone->SetBinContent(i,(h->GetBinContent(i)*Scale_Coef)); 
//              h_Clone->SetBinError(i,(h_Clone->GetBinContent(i))); 
         }
         
         //          map_Comment_Ref[i_Setup]["/gate/Cube_After/geometry/setHeight"].Data();
         
            h_Clone->SetLineWidth(3);
   h_Clone->SetMarkerStyle(3);
   h_Clone->SetMarkerSize(0.2);
         
         h_Clone->SetLineColor(Setup+1);
         
         
                for(int i = 1 ; i< h_Clone->GetNbinsX()+1; i++){  
                         double Error_Calcul = h_Clone->GetBinContent(i) * h_un->GetBinContent(i);
                         //                          cout << "Bin error " <<map_Histo_StudyCase[i_Setup][x][y]["hdose_Uncertainty"]->GetBinContent(i)*100 << " " <<  map_Histo_StudyCase[i_Setup][x][y]["hdose"]->GetBinContent(i) << " " << Error_Calcul << endl;
                         h_Clone->SetBinError(i, Error_Calcul );
                         //        h_Clone->SetBinError(i,0);
                     }
         
         
         C_temp->cd();
         
         gr_temps = new TGraphErrors(h_Clone);
         
          multi_general->Add(gr_temps,"ePL");
         
//          h_Clone->Draw("hist same");
         
         
         if(WichCase.Contains("_0.68_")) { 
             leg2->AddEntry(h_Clone, "68 #pm 0.68 MeV ", "elp");
         }
         if(WichCase.Contains("_1_"))  leg2->AddEntry(h_Clone, "68 #pm 1.00 MeV ", "l");
         if(WichCase.Contains("_0_")) { leg2->AddEntry(h_Clone, "68 #pm 0.00 MeV ", "l");
         }
         
         
     }
     
     TGraphErrors * gr_Serie1 = TGraphConstruct_SerieA() ;
     
      leg2->AddEntry((TObject*)0, "Data", "");
     
     gr_Serie1->SetMarkerColor(6);
     gr_Serie1->SetMarkerStyle(23);
     gr_Serie1->SetMarkerSize(1.4);
     
     leg2->AddEntry(gr_Serie1, "EBT3 05/20", "ep");
     
     multi_general->Add(gr_Serie1,"eP");
     
     TGraphErrors * gr_Serie2 = TGraphConstruct_SerieB(TotalParticuleExperiment) ;
     
     
     gr_Serie2->SetMarkerColor(4);
     gr_Serie2->SetMarkerStyle(22);
     gr_Serie2->SetMarkerSize(1.4);
     
     leg2->AddEntry(gr_Serie2, "HDV2 03/20", "ep");
     
     multi_general->Add(gr_Serie2,"eP");
     
     
            TString Title = "Proton range in function of beam energy for different material";
            
            multi_general->SetTitle(Title);
            multi_general->GetXaxis()->SetTitle("Depth [mm]");
            multi_general->GetYaxis()->SetTitle("Dose [Gy]");
            
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
     
     
            multi_general->Draw("ae1");
     
     
        pt = new TPaveText(0.1830743,0.7079208,0.4340818,0.8534653,"blNDC");
     MaquillageGraphique_PaveText(pt,map_Comment_Ref);
     pt->Draw();
     
     leg2->Draw();
     
     //      map_Histo_Ref[i_Setup]["hdose"]
     
            if(Save > 0){
         TitleSave = C_temp->GetName();
         C_temp->SaveAs( TString::Format("%s/%s.png",Total_Path.Data(),TitleSave.Data()));
   
    }
     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
     
     
     
     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
               
     
     // ATTENTION
     // PLot pic de Bragg cas ref
     
     // Etude de la différence du plateau en fonction de z
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     


//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
     //           TLegend * leg = NULL;
     NameCanvas = "1_Comparaison Pic de Bragg Beam spread"; 
     
     C_temp= new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
     
     NameCanvas = "2_Comparaison Pic de Bragg Beam spread"; 
     C_temp_2 = new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
     

     
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
             
             h_Clone->SetBinContent(i,(h->GetBinContent(i)/h->GetBinContent(RefBin)*100)-100); 
         }
         
         
         if(WichCase.Contains("_0.68_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 0.68 MeV", "l");
         if(WichCase.Contains("_1_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 1.00 MeV", "l");
         if(WichCase.Contains("_0_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 0.00 MeV", "l");
         
         
         MaquillageGraphique(h_Clone);
         
         h_Clone->GetXaxis()->SetTitle("z [mm]");
         
         
         h_Clone->SetTitle("Dose[z]/Dose[z=0.1]");
         
         h_Clone->GetYaxis()->SetTitle("[%]");
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
     
     
     pt = new TPaveText(0.1830743,0.7079208,0.4340818,0.8534653,"blNDC");
     MaquillageGraphique_PaveText(pt,map_Comment_Ref);
    
     C_temp->cd();
     pt->Draw();
     leg2->Draw();
     
     C_temp_2->cd();
     pt->Draw();
     leg2->Draw();
     
         if(Save > 0){
         TitleSave = C_temp->GetName();
         C_temp->SaveAs( TString::Format("%s/%s.png",Total_Path.Data(),TitleSave.Data()));
    }
          if(Save > 0){
         TitleSave = C_temp_2->GetName();
         C_temp_2->SaveAs( TString::Format("%s/%s.png",Total_Path.Data(),TitleSave.Data()));
    }

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     



//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
     
     // ATTENTION
     // PLot pic de Bragg cas ref
     
     // Etude de la différence du plateau en fonction de z

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     
               
     
     //           TLegend * leg = NULL;
     NameCanvas = "1_Comparaison Pic de Bragg Beam bin à bin"; 
     
     C_temp= new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
     
     NameCanvas = "2_Comparaison Pic de Bragg Beam bin à bin"; 
     C_temp_2 = new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
     
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
     RefBin = 10;
     
     for(auto& x: map_Histo_Ref){
         
         int Setup = x.first;
         TString WichCase =  vec_Option[Setup];
         TH1D* h = x.second["hdose"];
         
         TH1D* h_Clone = (TH1D*)h->Clone();
         
         
         //         // Remise à 0 pour verif 
         //         for(int i = 1 ; i< h_Clone->GetNbinsX()+1; i++){  
         //             h_Clone->SetBinContent(i,-1000);
         //          }
         
         for(int i = RefBin ; i< h_Clone->GetNbinsX()+1; i++){  
             
             h_Clone->SetBinContent(i,(((  TMath::Abs(h->GetBinContent(i)-h->GetBinContent(i-1)) /h->GetBinContent(i-1))*100 ))); 
         }
         
         
         if(WichCase.Contains("_0.68_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 0.68 MeV", "l");
         if(WichCase.Contains("_1_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 1.00 MeV", "l");
         if(WichCase.Contains("_0_"))  leg2->AddEntry(x.second["hdose"], "68 #pm 0.00 MeV", "l");
         
         
         MaquillageGraphique(h_Clone);
         
         h_Clone->GetXaxis()->SetTitle("z [mm]");
         
         
         h_Clone->SetTitle("Ecart relatif Dose[z]-Dose[z-0.01]/Dose[z-0.01]");
         
         h_Clone->GetYaxis()->SetTitle("[%]");
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
     
    
     pt = new TPaveText(0.1830743,0.7079208,0.4340818,0.8534653,"blNDC");
     MaquillageGraphique_PaveText(pt,map_Comment_Ref);
     
     C_temp->cd();
     pt->Draw();
     leg2->Draw();
     
     C_temp_2->cd();
     pt->Draw();
     leg2->Draw();
     
     
         if(Save > 0){
         TitleSave = C_temp->GetName();
         C_temp->SaveAs( TString::Format("%s/%s.png",Total_Path.Data(),TitleSave.Data()));
    }
    
        if(Save > 0){
         TitleSave  = C_temp_2->GetName();
         C_temp_2->SaveAs( TString::Format("%s/%s.png",Total_Path.Data(),TitleSave.Data()));
    }
     
     
     /*     
      *       	          TitleSave = "Fluence_PerCent"+ constants->DoubleToString(Energy_Max) + "MeV_Superposition";
      *  C_temp->SaveAs( TString::Format("%s/%s.png",Path_Save2.Data(),TitleSave.Data()));
      *  C_temp->SaveAs( TString::Format("%s/%s.pdf",Path_Save2.Data(),TitleSave.Data()));
      */
     
     
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************     

// Film EBT3
        
//         map_Histo_Film[i_Setup][NameCase][i_Film][NameCase_Film]=h_film_Clone;
        if(Etude_Film == true){
        
        for(auto& SetupNumber: map_Histo_Film){
            
            cout << "\nSetup #" << SetupNumber.first << endl;
            
            
            TString CaseType_TS;
            for(auto& CaseType: SetupNumber.second){
                //             if(CaseType.first.Contains("Ref") || CaseType.first.Contains("Pos")){ 
                CaseType_TS = CaseType.first;
                cout << "   CaseType " << CaseType_TS << endl;
                
                //             }
                for(auto& i_Film: CaseType.second){
                    
                    cout << "       i_Film " << i_Film.first << endl; 
                    
                    TH1F * h_graph; TH2F * h_graph2; TH2F * h_temps;
                    TPaveStats *st;
                    
                    NameCanvas = constants->NameCanvas(Number_temp, CaseType_TS);
                    C_temp = new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
                    
                    C_temp->Divide(2,2);
                    
                    C_temp->cd(3);
                    
                    
                    C_temp->cd(1);
                    
                    //              h_graph2 =  map_Result_TH2F[i_Config][Case]["Histo_XY"];
                    h_graph2 =  i_Film.second["DoseToWater"];
                    h_temps =  i_Film.second["DoseToWater-Uncertainty"];
                    for(Int_t i = 1; i < h_graph2->GetNbinsX()+1; i++) {
                          for(Int_t j = 1; j < h_graph2->GetNbinsY()+1; j++) {
                              h_graph2->SetBinError(i,j,h_graph2->GetBinError(i,j)*h_temps->GetBinError(i,j));
                        }
                    }
                    
                    
                    float DeltaBin = (TMath::Abs((Float_t)h_graph2->FindLastBinAbove()) - TMath::Abs((Float_t)h_graph2->FindFirstBinAbove()))*1.844380403/2;
                    
                    int BinMin_Y = (Float_t)h_graph2->FindFirstBinAbove(0,2) - DeltaBin  ;
                    int BinMax_Y = (Float_t)h_graph2->FindLastBinAbove(0,2) + DeltaBin;
                    cout << "Bin " << BinMin_Y <<  " " << BinMax_Y << endl;
                    
                    //           h_graph2->GetXaxis()->SetRange(BinMin_Y,BinMax_Y);
                    //           h_graph2->GetYaxis()->SetRange(h_graph2->FindFirstBinAbove(0,2),h_graph2->FindLastBinAbove(0,2));
                    
                    
                    double DeltaUser = h_graph2->GetStdDev(2)*4*1.844380403/2;
                    
                    double BinMinUser_Y = h_graph2->GetStdDev(2)*-4 - DeltaUser  ;
                    double BinMaxUser_Y = h_graph2->GetStdDev(2)*4 + DeltaUser ;
                    
//                     h_graph2->GetYaxis()->SetRangeUser(h_graph2->GetStdDev(2)*-4,h_graph2->GetStdDev(2)*4);
//                     h_graph2->GetXaxis()->SetRangeUser(BinMinUser_Y,BinMaxUser_Y);
                    
                    h_graph2->SetTitle( CaseType_TS + "_Film_" + IntToString(i_Film.first,0));
                    
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
                    
                    C_temp->cd(2);
                    
                    int NbrBin =  h_graph2->GetNbinsX();
                    
                    float NbrBin_Center=NbrBin/2;
                    
                    TH1D* Projection_X = (TH1D*)h_graph2->ProjectionX("ProjectionX of biny=[1996,2006] [y=-0.55..0.55]",(int)NbrBin_Center-3,(int)NbrBin_Center+3)->Clone();
                    //              h_graph =  Projection_X;
                    
                    Projection_X->GetXaxis()->SetNdivisions(712);
                    Projection_X->GetXaxis()->SetLabelOffset(0.015);
                    Projection_X->SetFillColor(38);
                    Projection_X->GetYaxis()->SetTitle("Number of Entries");
//                     Projection_X->GetXaxis()->SetRangeUser(BinMinUser_Y/1.844380403,BinMaxUser_Y/1.844380403);
                    Projection_X->SetTitle("ProjectionX of biny=[1996,2006] [y=-0.55..0.55]");
                    Projection_X->Draw();
                    
                    map  <int, map < TString, Float_t>> map_Results;
                    CompileHomogeneite(Projection_X, map_Results);
                    
                    map < int , TGraph* > map_Results_TGraph;
                    CompileHomogeneite_TGraph(Projection_X,map_Results_TGraph);
                    
//                     TMultiGraph* multi ;
//                     TLegend * leg_Multi ;
//                     MakeTMulti_CompileHomogeneite(map_Results_TGraph,leg_Multi);

                    
                
     
//                     int Method = 0;
//                     Method++;
//                     fichier_Save2 << NameCase_TS 
//                     << " " << x_TCut 
//                     <<" X " 
//                     << Method << " "
//                     << map_Results[Method]["Range_Min"] << " " 
//                     << map_Results[Method]["Range_Max"] << " " 
//                     << map_Results[Method]["Amplitude_Min"] <<  " " 
//                     << map_Results[Method]["Amplitude_Max"] << " "
//                     << map_Results[Method]["Mean"] << " "
//                     << map_Results[Method]["STD"] << " "
//                     << map_Results[Method]["Homogeneite_1"] << " "
//                     << map_Results[Method]["Homogeneite_2"] << endl;
//                     
//                     Method++;
//                     fichier_Save2 << NameCase_TS
//                     << " " << x_TCut 
//                     <<" X " 
//                     << Method << " "
//                     << map_Results[Method]["Range_Min"] << " " 
//                     << map_Results[Method]["Range_Max"] << " " 
//                     << map_Results[Method]["Amplitude_Min"] <<  " " 
//                     << map_Results[Method]["Amplitude_Max"] << " "
//                     << map_Results[Method]["Mean"] << " "
//                     << map_Results[Method]["STD"] << " "
//                     << map_Results[Method]["Homogeneite_1"] << " "
//                     << map_Results[Method]["Homogeneite_2"] << endl;
                    
                    
                    cout << "map_Results[Methode][Homogeneite_1] " << map_Results[1]["Homogeneite_1"] <<  endl;
                    cout << "map_Results[Methode][Homogeneite_2] " << map_Results[1]["Homogeneite_2"] <<  endl;
                    
                    cout << "map_Results[Methode][Homogeneite_1] " << map_Results[2]["Homogeneite_1"] <<  endl;
                    cout << "map_Results[Methode][Homogeneite_2] " << map_Results[2]["Homogeneite_2"] <<  endl;
                    
                    gStyle->SetOptStat("eMR");
//                     st = (TPaveStats*)h_graph->FindObject("stats");
                    
                    
                    C_temp->cd(3);
                    pt = new TPaveText(0.1186367,0.09444993,0.3692469,0.9961298,"blNDC");
                    pt->SetMargin(0.02);
                    pt->SetBorderSize(0);
                    pt->SetFillColor(10);
                    pt->SetFillStyle(1001);
                    pt->SetLineWidth(1);
                    pt->SetTextAlign(12);
                    pt->SetTextFont(42);
                    pt->SetTextSize(0.04318618); 
                    
                    pt_LaTex = pt->AddText("Projection X");
                    for(auto& x: map_Results){
                        TString Method_TS = "Methode " + IntToString(x.first,0); 
                        pt_LaTex = pt->AddText(Method_TS);
                        for(auto& y: x.second){
                        TString A1 = y.first;
                        TString A2 = TString::Format("%.4g",y.second);
                        
                        int A1_Size = A1.Sizeof();
                        int A2_Size = A2.Sizeof();   
                        
                        int A_Size = 50 - (A1_Size + A2_Size);
                        
                        cout << "A1_Size " << A1_Size << endl;
                        for(int i=A1_Size; i < A_Size ; i++) A1+= " ";
                        
                        A1 += A2 ; 
                        
                        pt_LaTex = pt->AddText(A1);
                        }
                    
                    }
           
                    pt->Draw();
                    
                    
                    C_temp->cd(4);
                    
                    
                    TH1D* Projection_Y = (TH1D*)h_graph2->ProjectionY("ProjectionY of binx=[1996,2006] [x=-0.55..0.55]",(int)NbrBin_Center-3,(int)NbrBin_Center+3)->Clone();
                    //              h_graph =  Projection_X;
                    
                    Projection_Y->GetXaxis()->SetNdivisions(712);
                    Projection_Y->GetXaxis()->SetLabelOffset(0.015);
                    Projection_Y->SetFillColor(38);
                    Projection_Y->GetYaxis()->SetTitle("Number of Entries");
//                     Projection_Y->GetXaxis()->SetRangeUser(BinMinUser_Y,BinMaxUser_Y);
                    Projection_Y->SetTitle("ProjectionY of binx=[1996,2006] [x=-0.55..0.55]");
                    Projection_Y->Draw();
//                     
                    /// Projection homogénéité
                    
                    
                    map_Results.clear();
                    CompileHomogeneite(Projection_Y, map_Results);
//                     
//                     Method = 0;
//                     Method++;
//                     
//                     
//                     
//                     fichier_Save2 << NameCase_TS 
//                     << " " << x_TCut 
//                     <<" Y " 
//                     << Method << " "
//                     << map_Results[Method]["Range_Min"] << " " 
//                     << map_Results[Method]["Range_Max"] << " " 
//                     << map_Results[Method]["Amplitude_Min"] <<  " " 
//                     << map_Results[Method]["Amplitude_Max"] << " "
//                     << map_Results[Method]["Mean"] << " "
//                     << map_Results[Method]["STD"] << " "
//                     << map_Results[Method]["Homogeneite_1"] << " "
//                     << map_Results[Method]["Homogeneite_2"] << endl;
//                     
//                     Method++;
//                     fichier_Save2 << NameCase_TS 
//                     << " " << x_TCut 
//                     <<" Y " 
//                     << Method << " "
//                     << map_Results[Method]["Range_Min"] << " " 
//                     << map_Results[Method]["Range_Max"] << " " 
//                     << map_Results[Method]["Amplitude_Min"] <<  " " 
//                     << map_Results[Method]["Amplitude_Max"] << " "
//                     << map_Results[Method]["Mean"] << " "
//                     << map_Results[Method]["STD"] << " "
//                     << map_Results[Method]["Homogeneite_1"] << " "
//                     << map_Results[Method]["Homogeneite_2"] << endl;
                    
                    
                    
                    
//                     st = (TPaveStats*)h_graph->FindObject("stats");
//                     st->SetLineColor(1);
//  
                    gStyle->SetOptStat("eMR");
// 
                                       C_temp->cd(3);
                    pt = new TPaveText(0.5348675,0.09469697,0.7854777,0.9963768,"blNDC");
                    pt->SetMargin(0.02);
                    pt->SetBorderSize(0);
                    pt->SetFillColor(10);
                    pt->SetFillStyle(1001);
                    pt->SetLineWidth(1);
                    pt->SetTextAlign(12);
                    pt->SetTextFont(42);
                    pt->SetTextSize(0.04318618); 
                    
                    pt_LaTex = pt->AddText("Projection Y");
                    for(auto& x: map_Results){
                        TString Method_TS = "Methode " + IntToString(x.first,0); 
                        pt_LaTex = pt->AddText(Method_TS);
                        for(auto& y: x.second){
                        TString A1 = y.first;
                        int A1_Size = A1.Sizeof();
//                         cout << "A1_Size " << A1_Size << endl;
                        for(int i=A1_Size; i < 25 ; i++) A1+= " ";
                        
                        A1 += TString::Format("%.4g",y.second) ; 
                        
                        pt_LaTex = pt->AddText(A1);
                        }
                    
                    }
           
                    pt->Draw();
                    if(Save > 0){
                        TitleSave = C_temp->GetName();
                        C_temp->SaveAs( TString::Format("%s/%s.png",Total_Path.Data(),TitleSave.Data()));
                    }
                    
                    
                }
                
        
                
            }
//             cout << endl;
            
         
         
         
     } // end x
     
     
     
     
        }
     
     
     
     
     
     
     
     
     timer.Stop();
     cout << "**** Multi_Run_Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C , timer" <<endl;
     timer.Print();
     
     
     
 } // end main
 
 
 
 
 
 
 
 
