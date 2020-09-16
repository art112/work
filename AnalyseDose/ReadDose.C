 
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
 
 
// Macro include

using namespace std;

//********************** Main Function *************************************************************************************************************************
void ReadDose(){

  vector<Double_t> PointeurTTree;
  Bool_t flag_Dose = 0;
  
  Int_t verbose = 0;
  
  TH1D *hEdep_Z = new TH1D("hEdep_Z","Edep Z; Z(mm);Edep",500,0,500);
  TH1D *hDose_Z = new TH1D("hDose_Z","Dose (Gy) Z(mm)); Z;Dose (Gy)",500,0,500);
  
  TH2D *hEdep_XY = new TH2D("hEdep_XY","Edep Z; X;Y",100,0,100,100,0,100);
  TH2D *hDose_XY = new TH2D("hDose_XY","Dose (Gy) ;X;Y",100,0,100,100,0,100);
  
  Double_t TotalDose = 0;
  Double_t TotalEdep =0 ;

  Double_t x,y,z,i; // def variable temps Tim Pulse Filtre integral  
  // ouvre fichier en lecture uniquement. On lui associe l'objet: fichier
ifstream fichier ("Dose_1.txt");// ouvre un nouveau fichier en lecture. On lui associe l'objet: fichier
Int_t Line_Counter = -1;
  
        if(!fichier)  // si l'ouverture a réussi "!" existe pas attention pour commande g systeme c est l inverse fichier.fail()) // checks to see if file opended 
        {      
	
	      cout << " L'erreur est humaine, admettre la sienne est surhumain. [Doug Larson]" << endl;       
	}
	
	  else 
	  {
	  
	    cout << "Le fichier existe bien et est correctement ouvert !" << endl;
	    cout << " ! ne garantie pas l'exactitude des données du fichier ! " << endl;
	    cout << "A l'ouverture du fichier, le curseur de trouve a l'octet " << fichier.tellg() << "." << endl << endl;
	    cout << "Le fichier contient : " << endl << endl;
	    
	    
    string ligne; //Une variable pour stocker les lignes lues
    
      while(getline(fichier, ligne)) //Tant qu'on n'est pas à la fin, on lit
      {
        Line_Counter += 1; 
	
// 	if(Line_Counter<10){cout << ligne << endl;  }
         //Et on l'affiche dans la console
         //Ou alors on fait quelque chose avec cette ligne
         //À vous de voir
	 
// 	 cout << "Line " << ligne.at(0) << endl;
	 string str = "#";
	 string strDose = "# iX, iY, iZ, value [Gy]";
	 
	 if(ligne.at(0) == str){if(ligne == strDose)flag_Dose=1; cout << ligne << endl;  }
	 else{
// 	  cout << "Original " << ligne << endl;
	 string Lecture = "";
	 string str2 =","; // choice of separator
	 Int_t XYZD = 0;
	 vector<Double_t> XYZD_vec;
	 XYZD_vec.clear();
	 string Copy = "";
	 
	 for (int i=0; i<ligne.length(); ++i)
	  {
	    if(ligne.at(i) != str2 ){
	    Lecture += ligne.at(i);
	    }
	    else if(ligne.at(i) == str2){ XYZD +=1;double LectureConversion =std::stod(Lecture); XYZD_vec.push_back(LectureConversion); Lecture="";}
	    else {cout << "Pb ???" << endl;}
	    
	    if(i == ligne.length()-1){double LectureConversion =std::stod(Lecture); XYZD_vec.push_back(LectureConversion);}
// 	    XYZD_vec.push_back(Lecture);
// 	    cout << ligne.at(i) ;
	  }
	    Copy = "";
	    for(Int_t i=0; i< XYZD_vec.size(); i++){
// 	    cout << XYZD_vec[i] ;
// 	    if(i < XYZD_vec.size()-1)cout << ",";
	    
// 	      Copy += XYZD_vec[i].str();
// 	      if(i < XYZD_vec.size()-1)Copy += ",";
	    }
	    
	    if(flag_Dose == 0){hEdep_Z->Fill(XYZD_vec[2],XYZD_vec[3]);
	      hEdep_XY->Fill(XYZD_vec[0],XYZD_vec[1],XYZD_vec[3]);
	      TotalEdep += XYZD_vec[3];
	    }
	      if(flag_Dose == 1){
		hDose_XY->Fill(XYZD_vec[0],XYZD_vec[1],XYZD_vec[3]);
		hDose_Z->Fill(XYZD_vec[2],XYZD_vec[3]);
	      
	     TotalDose +=  XYZD_vec[3];
	    }
// 	 cout << endl;
	 
// 	if(ligne.at(0) != str){ fichier >> x >> y >> z >> i ;
// 	if (Line_Counter < 40) printf("x=%lf, y=%15lf, z=%15lf, i=%15lf\n",x,y,z,i);
// 	}
// 	 if(Copy != ligne) cout << "Pb lecture !!! " << endl;
	 }
//       if(Line_Counter >10) break;
	
      }
// 		 

	    
	    cout << "Total Edep (MeV) " << TotalEdep << endl;
            cout << "Total Dose (Gy) " << TotalDose << endl;
// 	    hEdep_Z->Draw();
// 	    hDose_Z->Draw();
	    cout << "Le fichier contient " << Line_Counter << " lignes." << endl;
	    fichier.close();  // on ferme le fichier
	    
	    
	    
TCanvas* c1= new TCanvas("c1", "BraggPeak");

  c1->Divide(2,2);
  c1->cd(1);  
//     hEfficiency_X->GetHistogram()->GetXaxis()->SetTitle("Z (mm)"); 
//     hEfficiency_X->GetHistogram()->GetYaxis()->SetTitle("X (mm)"); 
    
    
   hEdep_Z->Draw("hist");
   
  c1->cd(2);  
  
  hDose_Z->Draw("hist");
  
  c1->cd(3);
  hEdep_XY->Draw("colz");
  
  c1->cd(4);
  hDose_XY->Draw("colz");
  
	    
	    
	  }

 
  
  
  
  
  
}