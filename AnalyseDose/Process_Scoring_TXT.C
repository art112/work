
#include "TROOT.h"
#include "TF1.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH2.h"
#include "TProfile.h"
#include "TStopwatch.h"
#include <iostream>
#include <fstream>
#include "TDirectoryFile.h" // void Delete()
#include "TAxis.h"
#include <THStack.h>
#include "TPaveStats.h"

#include <TMath.h>

// A simple TTreeReader use: read data from hsimple.root (written by hsimple.C)
#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"


#include <sstream>
#include <string>



#include "TSystem.h"

using namespace std;

void Process_Scoring_TXT()
{
  
  string Name_Folder_Intput="NAMEDOSEEDEP";
  
  cout << Name_Folder_Intput << endl;
  // gSystem->Exec("awk \'{s=$0; gsub(\",\", \" \", s); print s}\' dose_Pen.txt > dose_Pen_withSpaces.txt");
  // gSystem->Exec("awk \'FS=\",\" {if($4!=0){print $0} }\' dose_Pen.txt");
  
  // read file $ROOTSYS/tutorials/tree/basic.dat
  // this file has 3 columns of float data
  
  
  //   ${WORK_DIR}/Dose_Results/Dose_MATERIAL_ENERGY_EVENT_NB.txt
  
//   TString Init_Setup = "### MATERIAL ENERGY EVENT_NB BOXSIZE BINSIZE V_GEANT4";
//   TString Init_Setup_Name = "## Material Energy EVENT_nb bOXSIZE_x bOXSIZE_y bOXSIZE_z bOXSIZE_unit bINSIZE_x bINSIZE_y bINSIZE_z V_geant4";
//   cout << "Setup : " << Init_Setup << endl;
  
  ifstream fichier (Name_Folder_Intput,ios::in);// ouvre un nouveau fichier en lecture. On lui associe l'objet: fichier
  
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
    
    Int_t flag_Dose =-1;
    int nbLignes = 0;
    int nbLignes_after = 0;
    
    TString Name_Folder_temp =   Name_Folder_Intput + "_Process";
    ofstream fichier_temp(Name_Folder_temp, ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
    
    while (fichier)
    {

/*      if(nbLignes==0){
      nbLignes++;  if(fichier_temp){
	fichier_temp << Init_Setup_Name << endl;
	fichier_temp << Init_Setup << endl;
      }
      
      }
      */
      nbLignes++;
      
      string s;
      
      if (!getline( fichier, s )) break;
      
      istringstream ss( s );
      
      string str = "#";
      string strDose = "# iX, iY, iZ, value [Gy]";
      string strEdep = "# iX, iY, iZ, value [MeV]";
      
      if(s == strEdep){flag_Dose=0;} /*cout << s << endl*/
	if(s == strDose){ cout << "Start Read Dose !" << endl; flag_Dose=1;} 
	/*cout << s << endl*/  
	Bool_t Comment=0;
	if(s.at(0) == str){cout << s << endl; Comment=1; }
	vector <string> record;
	
	//       cout << "s  "<< s << endl;
	while (ss)
	{
	  string s;
	  if (!getline( ss, s, ',' )) break;
	  record.push_back( s );
	}
// 	      cout << record.size() << endl;
	
	bool Cut=true;
	if(Comment==0 && record[3]=="0" ) Cut= false;
	
	if(record.size()>0 && Cut==true /* && flag_Dose==0*/ )
	{
	  
	  if(fichier_temp){
	    Int_t i_position = -1;
	    nbLignes_after++;
	    for (auto& x: record) {
	      i_position++;
	      if(i_position<(record.size()-1))fichier_temp << x << " ";
	      else fichier_temp << x;
	    }
	    fichier_temp << endl;
	  }
	  else{cout << "Impossible d'ouvrir le fichier !" << endl;}
	  
	}
	
    }  
    cout << Name_Folder_Intput <<" contenait " << nbLignes << " lignes." << endl;
    cout << Name_Folder_temp   << " contient " << nbLignes_after << " lignes." << endl;
    cout << "Reduction " << 100.00-(((Double_t)nbLignes_after/(Double_t)nbLignes)*100.00) << " %" << endl; 
    fichier.close();  // on ferme le fichier
    fichier_temp.close();
  }
  
  
  system("pwd");
  
}