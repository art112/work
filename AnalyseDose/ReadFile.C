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


    



void ReadFile(){
    
    TStopwatch timer;
    timer.Start();
    
    string PathFile = "/media/sf_PartageVbox8/";
    string NameFile = "Beta_Spectrum.root";
    TString Complete_PathFile = PathFile+NameFile;
    
    TFile *fileData = new TFile(Complete_PathFile);
    TH1F * h1 = (TH1F*)fileData->Get("energySpectrumNbPart"); 
    
    
//     string PathFile = "/media/sf_PartageVbox8/";
    string NameFile2 = "Beta_Spectrum2.root";
    TString Complete_PathFile2 = PathFile+NameFile2;
    
    TFile *fileData2 = new TFile(Complete_PathFile2);
    TH1F * h2 = (TH1F*)fileData2->Get("energySpectrumNbPart"); 
    
       
   TString NameCanvas = "Name_Fenetre";
   TCanvas * C_temp = NULL;

    C_temp= new TCanvas(NameCanvas, NameCanvas,67,24,1853,1071);
	       
// 	         gStyle->SetOptStat(0);
	       
// 	       C_temp->SetGrid(); 
	       
//   h1->GetXaxis()->SetRangeUser(hSimu->GetMean()-40,hSimu->GetMean()+35);
	       
// 	       hSimu->GetYaxis()->SetRangeUser(0,1.2);
	       
// 	       hSimu->SetLineWidth(3);
// 	       hSimu->SetTitle(Title);
	      
    h1->SetLineColor(2);
       h1->DrawNormalized("");
	    h2->DrawNormalized("same");   
        
        string i = "0";
        int int_i = ToInteger(i);
  /*
   * 
   * http://web.mit.edu/root_v6.12/ROOT-Primer.pdf
   * 
   #====================================
#Physics GATE V8
#====================================

/gate/physics/addPhysicsList QGSP_BIC_EMZ

/gate/physics/addProcess Decay
/gate/physics/addProcess RadioactiveDecay

/gate/physics/processes/PhotoElectric/setAugerElectron false
/process/em/fluo false
/process/em/pixe false
/process/em/auger false
/process/em/augerCascade false

#/gate/physics/processes/PhotoElectric/setDeltaRayCut [value] [unit]
#/gate/physics/processes/PhotoElectric/setXRayCut 0. MeV*/
        
        
    cout << "*** Timer -> ";timer.Stop(); timer.Print();   timer.Continue();   
}
