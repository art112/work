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

#include "../Include/hdrloader.cpp" 

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



void LoadHDR(){
    
             ICOConstants *const_ICO = new ICOConstants();
    TStopwatch timer;
    timer.Start();
    
        string PathFile = "/media/sf_PartageVbox8/Materiaux_Caroline/";
    
    string NameFile = "Image125tissus.db";
  
   
        
        
HDRLoaderResult result;
bool ret = HDRLoader::load("../Include/souris-Dose.hdr", result); 
    
    cout << "width " << result.width << endl;

     cout << "Height " << result.height << endl;
  
     cout << "cols " << result.size() << endl;
    
    cout << "*** Timer -> ";timer.Stop(); timer.Print();   timer.Continue();   
}
