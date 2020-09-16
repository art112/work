
 
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
     
     //      map <float,float> map_Data_XP_Serie_A;
     //      
     //      map_Data_XP_Serie_A[0.139]=1.63;
     //      map_Data_XP_Serie_A[25.417]=2.59;
     //      map_Data_XP_Serie_A[30.695]=5.05;
     //      map_Data_XP_Serie_A[31.473]=6.88;
     //      map_Data_XP_Serie_A[32.251]=3.35;
     //      
     //      map_Data_XP_Serie_A[32.529]=1.26;
     //      map_Data_XP_Serie_A[32.807]=0.32;
     //      map_Data_XP_Serie_A[33.085]=0.05;
     //      
     //      map <float,float> map_Data_XP_Serie_A_ey;
     //      
     //      map_Data_XP_Serie_A_ey[0.139]=0.03;
     //      map_Data_XP_Serie_A_ey[25.417]=0.05;
     //      map_Data_XP_Serie_A_ey[30.695]=0.09;
     //      map_Data_XP_Serie_A_ey[31.473]=0.1;
     //      map_Data_XP_Serie_A_ey[32.251]=0.08;
     //      
     //      map_Data_XP_Serie_A_ey[32.529]=0.04;
     //      map_Data_XP_Serie_A_ey[32.807]=0.10;
     //      map_Data_XP_Serie_A_ey[33.085]=0.10;
     //           map <float,float> map_Data_XP_Serie_A;
     //      
     //      map_Data_XP_Serie_A[0.139]=1.8807;
     //      map_Data_XP_Serie_A[25.417]=2.9756;
     //      map_Data_XP_Serie_A[30.695]=5.5161;
     //      map_Data_XP_Serie_A[31.473]=7.3771;
     //      map_Data_XP_Serie_A[32.251]=3.6026;
     //      
     //      map_Data_XP_Serie_A[32.529]=1.3446;
     //      map_Data_XP_Serie_A[32.807]=0.361;
     //      map_Data_XP_Serie_A[33.085]=0.0778;
     //      
     //      map <float,float> map_Data_XP_Serie_A_ey;
     //      
     //      map_Data_XP_Serie_A_ey[0.139]=0.03;
     //      map_Data_XP_Serie_A_ey[25.417]=0.05;
     //      map_Data_XP_Serie_A_ey[30.695]=0.09;
     //      map_Data_XP_Serie_A_ey[31.473]=0.1;
     //      map_Data_XP_Serie_A_ey[32.251]=0.08;
     //      
     //      map_Data_XP_Serie_A_ey[32.529]=0.04;
     //      map_Data_XP_Serie_A_ey[32.807]=0.10;
     //      map_Data_XP_Serie_A_ey[33.085]=0.10;
     
     
     
     
     
     
     map <float,float> map_Data_XP_Serie_A;
     
     
     
     map_Data_XP_Serie_A[4.3]=1;
     map_Data_XP_Serie_A[7.3]=1.04430534264175;
     map_Data_XP_Serie_A[10.3]=1.09545535965087;
     map_Data_XP_Serie_A[13.3]=1.15682540781059;
     map_Data_XP_Serie_A[16.3]=1.22896285856409;
     map_Data_XP_Serie_A[18.3]=1.28365257744401;
     map_Data_XP_Serie_A[20.3]=1.35037816406502;
     map_Data_XP_Serie_A[22.3]=1.43274156441718;
     map_Data_XP_Serie_A[24.3]=1.53267346784306;
     map_Data_XP_Serie_A[26.3]=1.66277590440032;
     map_Data_XP_Serie_A[27.3]=1.74285565999241;
     map_Data_XP_Serie_A[28.3]=1.84398622599115;
     map_Data_XP_Serie_A[29.3]=1.96651273867443;
     map_Data_XP_Serie_A[30.3]=2.12261141624466;
     map_Data_XP_Serie_A[31.3]=2.33477307949783;
     map_Data_XP_Serie_A[32.3]=2.62312556824232;
     map_Data_XP_Serie_A[32.8]=2.82868996170859;
     map_Data_XP_Serie_A[33.3]=3.10991370760274;
     map_Data_XP_Serie_A[33.8]=3.5184885387479;
     map_Data_XP_Serie_A[34.05]=3.81605470732045;
     map_Data_XP_Serie_A[34.3]=4.18368327045773;
     map_Data_XP_Serie_A[34.55]=4.59258144965536;
     map_Data_XP_Serie_A[34.675]=4.80142125622325;
     map_Data_XP_Serie_A[34.8]=4.93864390120916;
     map_Data_XP_Serie_A[34.8]=4.98458256404512;
     map_Data_XP_Serie_A[34.925]=5.00877540811293;
     map_Data_XP_Serie_A[35]=5.00853080873104;
     map_Data_XP_Serie_A[35.05]=5.02825483387618;
     map_Data_XP_Serie_A[35.05]=5.05905234098022;
     map_Data_XP_Serie_A[35.175]=5.02512430734217;
     map_Data_XP_Serie_A[35.25]=4.90214910676149;
     map_Data_XP_Serie_A[35.3]=4.61344684696103;
     map_Data_XP_Serie_A[35.3]=4.85548080516335;
     map_Data_XP_Serie_A[35.35]=4.72526194251051;
     map_Data_XP_Serie_A[35.4]=4.56565275920101;
     map_Data_XP_Serie_A[35.425]=4.40268696672017;
     map_Data_XP_Serie_A[35.425]=4.46262199898227;
     map_Data_XP_Serie_A[35.475]=4.21591392376319;
     map_Data_XP_Serie_A[35.5]=4.11002709571417;
     map_Data_XP_Serie_A[35.55]=3.76512805612266;
     map_Data_XP_Serie_A[35.6]=3.63089009519647;
     map_Data_XP_Serie_A[35.7]=3.12571268549004;
     map_Data_XP_Serie_A[35.8]=2.60513727753143;
     map_Data_XP_Serie_A[36.05]=1.49803174530094;
     map_Data_XP_Serie_A[36.3]=0.65658404355552;
     map_Data_XP_Serie_A[36.8]=0.0691258767312517;
     map_Data_XP_Serie_A[37.3]=0.00196208126647758;
     map_Data_XP_Serie_A[38.3]=0;
     map_Data_XP_Serie_A[39.3]=0;
     
     
     
     
     
     
     
     
     map <float,float> map_Data_XP_Serie_A_ey;
     
     //      map_Data_XP_Serie_A_ey[0.139]=0.03;
     //      map_Data_XP_Serie_A_ey[25.417]=0.05;
     //      map_Data_XP_Serie_A_ey[30.695]=0.09;
     //      map_Data_XP_Serie_A_ey[31.473]=0.1;
     //      map_Data_XP_Serie_A_ey[32.251]=0.08;
     //      
     //      map_Data_XP_Serie_A_ey[32.529]=0.04;
     //      map_Data_XP_Serie_A_ey[32.807]=0.10;
     //      map_Data_XP_Serie_A_ey[33.085]=0.10;
     //      
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
         ex_TGraph.push_back(0.1);
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
 
 void CompileHomogeneite_TGraph(TH1D* Projection, map < TString , TGraphErrors* >& map_Results_TGraph, float Pas_Commande=-1, float MaxHomogeneity_Commande=10){ 
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
     cout << "bin_Mean " << bin_Mean << endl;
     // Nbr de binma
     // Valur max histo valeur min 
     // Pas 
     
     map < TString , vector <float> > map_Results_Temps; 
     
     for(int i = 0; i < NbrDeBin+1; i+=Pas_Bin){
         
         int i_min = bin_Mean - i ;
         int i_max = bin_Mean  + i ;
         
         vector<float> vec_Projection;
         
         if(i_min < 1 || i_max > NbrDeBin+1) break;
         
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
             
             double DeltaZ = (i_max - i_min) * Pas ;
             
             map_Results_Temps["DeltaZ"].push_back(DeltaZ);
             //              map_Results_Temps["DeltaZ_Methode_3"].push_back(DeltaZ);
             //              map_Results_Temps["DeltaZ_Methode_3"].push_back(DeltaZ);
             
             double Methode_1; double Methode_2; double Methode_3_a; double Methode_3_b;
             
             Methode_1 = (vec_Projection.back() - vec_Projection.front())/(vec_Projection.back() + vec_Projection.front())*100;
             
             if(mean>0) Methode_2 = stdev/mean*100;
             else Methode_2 = 100;
             
             Methode_3_a = TMath::Abs((vec_Projection.front()-mean)) / mean *100; 
             Methode_3_b = TMath::Abs((vec_Projection.back()-mean)) / mean *100;
             
             map_Results_Temps["Methode_1"].push_back(Methode_1);
             map_Results_Temps["Methode_2"].push_back(Methode_2);
             map_Results_Temps["Methode_3_a"].push_back(Methode_3_a);
             map_Results_Temps["Methode_3_b"].push_back(Methode_3_b);
             
             cout << "DeltaZ " << DeltaZ << " " << Methode_1 << endl;
             
             if(Methode_1 > MaxHomogeneity_Commande) break;
         }     
         
         
     }
     
     Int_t n;
     TGraphErrors * TempsGraph;
     
     std::vector<Float_t> ex_TGraph;
     std::vector<Float_t> ey_TGraph;
     
     for(auto& x: map_Results_Temps["DeltaZ"]){
         ex_TGraph.push_back(0);
         ey_TGraph.push_back(0);
     }
     
     
     n = map_Results_Temps["DeltaZ"].size();
     
     cout << "n " << n << endl;
     
     TempsGraph = new TGraphErrors(n,&map_Results_Temps["DeltaZ"][0],&map_Results_Temps["Methode_1"][0],&ex_TGraph[0],&ey_TGraph[0]);
     map_Results_TGraph["Methode_1"]=TempsGraph;
     
     TempsGraph = new TGraphErrors(n,&map_Results_Temps["DeltaZ"][0],&map_Results_Temps["Methode_2"][0],&ex_TGraph[0],&ey_TGraph[0]);
     map_Results_TGraph["Methode_2"]=TempsGraph;
     
     TempsGraph = new TGraphErrors(n,&map_Results_Temps["DeltaZ"][0],&map_Results_Temps["Methode_3_a"][0],&ex_TGraph[0],&ey_TGraph[0]);
     map_Results_TGraph["Methode_3_a"]=TempsGraph;
     
     TempsGraph = new TGraphErrors(n,&map_Results_Temps["DeltaZ"][0],&map_Results_Temps["Methode_3_b"][0],&ex_TGraph[0],&ey_TGraph[0]);
     map_Results_TGraph["Methode_3_b"]=TempsGraph;
     
     return;
 } 
 
 // Macro include
 
 using namespace std;
 
 //********************** Main Function *************************************************************************************************************************
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaractérisationDuSetup.C
 
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C
 
 //  /media/sf_PartageVbox8/work/AnalyseDose/Multi_Run_Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C
 
 void Analyse_Dose_ManipZebrafish_Stack_Projet_ROOS_config1(){
     
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
     
     TLegend *leg2 = new TLegend(0.2008032,0.5793173,0.4761905,0.8453815,NULL,"brNDC");
     leg2->SetBorderSize(1);
     leg2->SetTextAlign(22);
     leg2->SetTextSize(0.0403071);
     leg2->SetLineColor(1);
     leg2->SetLineStyle(1);
     leg2->SetLineWidth(2);
     leg2->SetFillColor(10);
     leg2->SetFillStyle(1001);
     
//      TLegend *leg = new TLegend(0.2036718,0.6203156,0.4784854,0.8392505,NULL,"brNDC");
//      leg->SetBorderSize(1);
//      leg->SetTextAlign(22);
     
     //      /home/gate/nantes_sps/abongran/All_Results/z_Projet_ROOS/9.0/Test_Distribute_Ref_Stack_vROOS_v2_Ligne_Seule_CI_Config2_v1_Stack_Manip_13_08_20_config2_68MeV_0/Decalage_35
     
     ///****** *********** ************//
     string Projet = "z_Projet_ROOS";
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
     
     string Coef_Event = "1";
     string Unit_Event = "k" ;
     
     std::vector<string> vec_Option {"v1_68MeV_0_","v1_68MeV_0.68_","v1_68MeV_1_"};
     
     if(Farm == true){
         
         vec_Option.clear();
         NameSetup="vROOS_v2_Ligne_Seule_CI_Config2_v1_Stack_Manip_13_08_20_config2";
         CompleteNameSetup= NameSetup +"_68MeV_0" ; vec_Option.push_back(CompleteNameSetup);
         //          CompleteNameSetup= NameSetup +"_68MeV_0.68_" ; vec_Option.push_back(CompleteNameSetup);
         //          CompleteNameSetup= NameSetup +"_68MeV_1_" ; vec_Option.push_back(CompleteNameSetup)
         
         vec_Option.clear();
         NameSetup="vROOS_v4_Ligne_Seule_CI_Config2_v1_Stack_Manip_13_08_20_config2";
         CompleteNameSetup= NameSetup +"_68MeV_0" ; vec_Option.push_back(CompleteNameSetup);
         //          CompleteNameSetup= NameSetup +"_68MeV_0.68_" ; vec_Option.push_back(CompleteNameSetup);
         //          CompleteNameSetup= NameSetup +"_68MeV_1_" ; vec_Option.push_back(CompleteNameSetup)
         
         vec_Option.clear();
         NameSetup="vROOS_v7_Ligne_Seule_CI_Config1_v1_Stack_Manip_13_08_20_config1";
//          CompleteNameSetup= NameSetup +"_68MeV_0" ; vec_Option.push_back(CompleteNameSetup);
         CompleteNameSetup= NameSetup +"_68MeV_0.68" ; vec_Option.push_back(CompleteNameSetup);
//          CompleteNameSetup= NameSetup +"_68MeV_1" ; vec_Option.push_back(CompleteNameSetup);
//          CompleteNameSetup= NameSetup +"_68MeV_0.30" ; vec_Option.push_back(CompleteNameSetup);
      
         vec_Option.clear();
         NameSetup="vROOS_v8_Ligne_Seule_CI_Config1_v1_Stack_Manip_13_08_20_config1";
                  CompleteNameSetup= NameSetup +"_68MeV_0" ; vec_Option.push_back(CompleteNameSetup);
                  CompleteNameSetup= NameSetup +"_68MeV_0.35" ; vec_Option.push_back(CompleteNameSetup);
                  CompleteNameSetup= NameSetup +"_68MeV_0.68" ; vec_Option.push_back(CompleteNameSetup);
                 
         //          CompleteNameSetup= NameSetup +"_68MeV_0.30" ; vec_Option.push_back(CompleteNameSetup);
         
     }
     
     bool Etude_Epaisseur_Position = false;
     
     bool Etude_Film = false;
     bool Y_Homogeneity = true;
     bool X_Homogeneity = true;
     
     // Path Output
     if(Farm==true){Projet+="_FARM";}
     
     // Local
     TString PathOutput = "/media/sf_PartageVbox8/Analyse_Results/";
     TString NameFile = NameSetup /*+ "_" +Coef_Event + Unit_Event*/;     
     TString TypeAnalyse = "Analyse_Dose";
     
     TString Total_Path = PathOutput +Projet;
     TString CheckFileAnalysis = "mkdir "+ PathOutput  ; system(CheckFileAnalysis.Data());
     CheckFileAnalysis = "mkdir "+ Total_Path  ; system(CheckFileAnalysis.Data());
     Total_Path += "/" + NameFile ; CheckFileAnalysis = "mkdir "+ Total_Path  ; system(CheckFileAnalysis.Data());
     Total_Path += "/" + TypeAnalyse ; CheckFileAnalysis = "mkdir "+ Total_Path  ; system(CheckFileAnalysis.Data());
     //      TString CheckFileAnalysis_rm = "rm "+ Total_Path + "/*" ; system(CheckFileAnalysis_rm.Data());
     
     //      std::vector<string> Category_Of_Analyse {"Epaisseur", "Pos"};
     //      std::vector<string> type_Of_Analyse_2 {"Min", "Max"};
     
     std::vector<string> vec_Ref_Case ;
     
     for(auto& x: vec_Option){
         string Title = "Distribute_Ref_Stack_" + x;
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
     
     TMultiGraph * multi_general= new TMultiGraph();
     
     leg2->AddEntry((TObject*)0, "Simulation G4 10.4", "");
     
     for(int i = 0; i < vec_Ref_Case.size();i++){
         
         cout << "\n *** ***" << endl;
         
         i_Setup = i;
         CompteurCaseForResult = 1;
         
         TString NameInput;
         TString NameCase = vec_Ref_Case[i_Setup] /*+ Coef_Event + Unit_Event*/;
         TString PathInput = PathFile + NameCase;
         
        
         
        TString totalPathReadInformation =    PathInput;
        
        TGraphErrors* TGraph_1 = const_ICO->LoadSetup_with_RunSystem_ROOS( totalPathReadInformation.Data(),0);
        
        TGraph_1->SetLineColor(i+2);
        TGraph_1->SetLineStyle(10);
        TGraph_1->SetLineWidth(3);
        
        TGraph_1->SetMarkerStyle(i+20);
        
        TGraph_1->SetLineColor(i+2);
        TGraph_1->SetMarkerColor(i+2);
        
        
        TGraph_1->Draw("apc");
        
        TString WichCase =  NameCase;
        
        if(WichCase.Contains("MeV_0.68"))  leg2->AddEntry(TGraph_1, "68 #pm 0.68 MeV", "lpe");
        else if(WichCase.Contains("MeV_0.30"))  leg2->AddEntry(TGraph_1, "68 #pm 0.30 MeV", "lpe");
        else if(WichCase.Contains("MeV_0.35"))  leg2->AddEntry(TGraph_1, "68 #pm 0.35 MeV", "lpe");
        else if(WichCase.Contains("MeV_0.40"))  leg2->AddEntry(TGraph_1, "68 #pm 0.40 MeV", "lpe");
        else if(WichCase.Contains("MeV_0.2"))  leg2->AddEntry(TGraph_1, "68 #pm 0.20 MeV", "lpe");
        else if(WichCase.Contains("MeV_1"))  leg2->AddEntry(TGraph_1, "68 #pm 1.00 MeV", "lpe");
        else if(WichCase.Contains("MeV_0"))  leg2->AddEntry(TGraph_1, "68 #pm 0.00 MeV", "lpe");
        else cout << "Unknow" << endl;
        
        
        cout << "NameCase " << NameCase << endl;
        multi_general->Add(TGraph_1,"ePL");
         
         
         //ReadInformation :
//          TString totalPathReadInformation =    PathInput+"/SaveHistory.txt";
//          const_ICO->LoadSetup_with_RunSystem_SaveHistoryGate(map_Comment_Ref, totalPathReadInformation.Data(), i_Setup ,0);
//          
//          TString File_To_Read = "log";
//          totalPathReadInformation =    PathInput+"/" +File_To_Read + ".txt";
//          const_ICO->LoadSetup_with_RunSystem_GateLog(map_Comment_Ref, totalPathReadInformation.Data(), i_Setup ,0);
         
         
         //**********************************************************************************************************************************************
         //**********************************************************************************************************************************************     
         
     }
     
     TGraphErrors* TGraph_Data = TGraphConstruct_SerieA();
     
     NameCanvas = "Config1";
     C_temp= new TCanvas(NameCanvas, NameCanvas,0,25,1920,1041);
     
     multi_general->Add(TGraph_Data,"ePc");
     leg2->AddEntry((TObject*)0, "Data", "");
     leg2->AddEntry(TGraph_Data, "13/08/20 Config1", "lpe");
     
     TString Title = "Proton range in function of beam energy for different material";
     
     multi_general->SetTitle(Title);
     multi_general->GetXaxis()->SetTitle("Depth [mm]");
     multi_general->GetYaxis()->SetTitle("Dose [a.u.]");
     
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
     
     leg2->Draw();
     
     
     timer.Stop();
     cout << "**** Multi_Run_Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C , timer" <<endl;
     timer.Print();
     
     
     
 } // end main
 
 
 
 
 
 
 
 
 
 
 
