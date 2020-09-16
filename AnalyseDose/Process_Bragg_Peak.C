
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

#include "../Include/DPGAConstants.cc"

// A simple TTreeReader use: read data from hsimple.root (written by hsimple.C)
#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include <sstream>
#include <string>
#include "TSystem.h"

using namespace std;

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



void Process_Bragg_Peak()
{
   DPGAConstants *constants = new DPGAConstants();
    TString NameCanvas = "";
  Int_t Number_temp = 0;
  TCanvas * C_temp = NULL;
  TString NameForGet="";
  
  
  Int_t Begin_Run = 1; // Pour decaler l'annalyse des runs
  
  
  //To analyse only runs not processed
  vector <int> Run_Processed_List;
  string Numero_Temp;
  ifstream Run_Process ("./Base_Data_Scoring/Run_Process.txt",ios::in);
  if(Run_Process){
    while(getline (Run_Process,Numero_Temp) ){
      Run_Processed_List.push_back(ToInteger(Numero_Temp));
    }
  }
  Run_Process.close();
  
  if(Run_Processed_List.size()>0){
    reverse(Run_Processed_List.begin(),Run_Processed_List.end()); 
    Begin_Run = Run_Processed_List[0]+1;
  }
  else{ Begin_Run = 1; }
  
 Begin_Run = 28;
 
  system("mkdir Base_Data_Scoring");
  // Save File txt for base data
  TString Name_Folder_ROOT="./Base_Data_Scoring/Data_Base_histo.root";
  
  ofstream Save_Run ("./Base_Data_Scoring/Run.txt",ios::out | ios::app);
  ofstream Save_Setup ("./Base_Data_Scoring/Data_Base.txt",ios::out | ios::app);
  
  if(!Save_Setup || !Save_Run){ cout << "No txt for base data exit(0)" << endl; exit(0);}
  
  // Save histogramme in a rootfile
  TFile *Output_File = new TFile(Name_Folder_ROOT.Data(), "RECREATE"); // UPDATE
  
  //***********************************************************************************************************
  //
  // Analyse de runs/Setup.txt || contient toute l'analyse des runs
  //
  //************************************************************************************************************
  
  string Name_Folder_Intput="./runs/Setup.txt";
  
  cout <<"Name Input " <<  Name_Folder_Intput << endl;
  cout <<"Name Name_Folder_ROOT " <<  Name_Folder_ROOT << endl;
  
  
  
  // gSystem->Exec("awk \'{s=$0; gsub(\",\", \" \", s); print s}\' dose_Pen.txt > dose_Pen_withSpaces.txt");
  // gSystem->Exec("awk \'FS=\",\" {if($4!=0){print $0} }\' dose_Pen.txt");
  
  // read file $ROOTSYS/tutorials/tree/basic.dat
  // this file has 3 columns of float data
  
  
  //   ${WORK_DIR}/Dose_Results/Dose_MATERIAL_ENERGY_EVENT_NB.txt
  
  
  
  TString Init_Setup = "### MATERIAL ENERGY EVENT_NB BOXSIZE BINSIZE V_GEANT4";
  TString Init_Setup_Name = "## Material Energy EVENT_nb Boxsize Binsize V_geant4";
  cout << "Setup : " << Init_Setup << endl;
  
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
    
    int nbLignes = 0;
    
    TString Name_Folder_temp =   Name_Folder_Intput + "_Process";
    //     ofstream fichier_temp(Name_Folder_temp, ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
    
    std::map <int,vector <string>> Category_comment;
    std::map<int,  vector <string> > map_Run;
    
    int Category_comment_Counter=0;
    
    while (fichier)
    {
      nbLignes++;
      
      string s;
      
      if (!getline( fichier, s )) break;
      
      istringstream ss( s );
      
      string str = "#";
      string strd ="d"; 
      string strDose = "# iX, iY, iZ, value [Gy]";
      string strEdep = "# iX, iY, iZ, value [MeV]";
      string strDate = "#d";
      string strCase="## RUN_ID MATERIAL Energy_Min[MeV] Energy_Max[MeV] PAS_ENERGY[MeV] EVENT_NB BOXSIZE BINSIZE V_GEANT4 Time[s]";
      
      //       cout << s << endl ;
      Int_t Comment=0;
      if(s.at(0) == str){/*cout << s << endl;*/ Comment=1; }
      if(s.at(1) == strd && Comment==1) Comment = 2;
      if(s.at(1) == str && Comment==1) Comment = 3;
      
      vector <string> v_record;
      
      	cout << "s  "<< s << endl;
      while (ss)
      {
	string s;
	if ((!getline( ss, s, ' ' ))) break;
	v_record.push_back( s );
      }
      
      if(Comment == 3){
	for (auto& x: v_record) { Category_comment[Category_comment_Counter].push_back(x); /*cout << x << endl;*/ } 
	Category_comment_Counter++;
      }
      
      if(Comment ==0){
	int i_v_record_1 = ToInteger(v_record[0]);
	cout << "Run_Number "<< i_v_record_1 << endl;
	map_Run[i_v_record_1]= v_record;
      }
      
    } // end while
    fichier.close();  // on ferme le fichier
    
    cout << "\n";
    
//         for (auto& x: Category_comment) {  cout << x << endl; }
    
    //*******************************************************************************************
    //
    // List les runs et les différents paramètres
    //
    //*********************************************************************************************
    
    // List des runs possibilité de court circuiter les runs 
    
    Category_comment_Counter=0;
    ofstream Run_Process ("./Base_Data_Scoring/Run_Process.txt",ios::out | ios::app);
    vector <int> ListOfRun ;
    for (auto& x: map_Run) 
    {  
      cout <<"\nRun " <<x.first <<"\n\n" ; 
      
      
      
      if(x.first >= Begin_Run){ 
	ListOfRun.push_back(x.first);
	Run_Process << x.first << endl;
	
	Int_t i_position = 1;
	for(auto& y: x.second){
	  // For check
	  if(i_position < Category_comment[Category_comment_Counter].size())cout << Category_comment[Category_comment_Counter][i_position]<<" ";
	  cout << y << " "<< endl;
	  // For Save
	  if(i_position < Category_comment[Category_comment_Counter].size())Save_Run << Category_comment[Category_comment_Counter][i_position] << " " << y << endl;
	  
	  i_position++;
	  
	  
	}
	Save_Run << endl;
      }
      Category_comment_Counter++;
    }
    
    Run_Process.close();
    
    cout << "\n" << endl;
    //*******************************************************************************************
    //
    // Open runs/run_X/Setup.txt
    //
    //*********************************************************************************************
    
    
    for(auto& x_Run_Number: ListOfRun){
      TString Path_Name_Run = "./runs/run_";
      TString NameSetup = "/Setup.txt";
      TString Name_Run = Path_Name_Run+ std::to_string(x_Run_Number) + NameSetup;
      cout << Name_Run << endl;
      
      ifstream fichier (Name_Run,ios::in);// ouvre un nouveau fichier en lecture. On lui associe l'objet: fichier
      if(!fichier){cout << " L'erreur est humaine, admettre la sienne est surhumain. [Doug Larson]" << endl;}
      else 
      {
	cout << "Run "<< x_Run_Number << " Setup.txt is open"  << endl;
	
	vector <string> v_CommentsOfRunCategories;
	
	// POSSIBILITE d'inclure plusieurs energy dans le même run !!!
	std::map<int, vector <string>  > map_Run_Setup;
	
	while (fichier)
	{
	  nbLignes++;
	  
	  string s;
	  
	  if (!getline( fichier, s )) break;
	  
	  istringstream ss( s );
	  
	  string str = "#";
	  string strd ="d"; 
	  string strDose = "# iX, iY, iZ, value [Gy]";
	  string strEdep = "# iX, iY, iZ, value [MeV]";
	  string strDate = "#d";
	  string strCase="## MATERIAL Energy[MeV] EVENT_NB BOXSIZE_X BOXSIZE_Y BOXSIZE_Z BOXSIZE_UNIT BINSIZE_X BINSIZE_Y BINSIZE_Z V_GEANT4 Time[s]";
	  
	  // 	  cout << s << endl ;
	  Int_t Comment=0;
	  if(s.at(0) == str){cout << s << endl; Comment=1; }
	  if(s.at(1) == strd && Comment==1) Comment = 2;
	  if(s.at(1) == str && Comment==1) Comment = 3;
	  
	  vector <string> v_record;
	  
	  // 	cout << "s  "<< s << endl;
	  while (ss)
	  {
	    string s;
	    if ((!getline( ss, s, ' ' ))) break;
	    v_record.push_back( s );
	  }
	  
	  if(Comment == 3){
	    map_Run_Setup[-1]=v_record; // Fill comment names in -1
	    for (auto& x: v_record) { v_CommentsOfRunCategories.push_back(x); /*cout << x << endl;*/} 
	  }
	  
	  if(Comment ==0){
	    int i_v_record_1 = ToInteger(v_record[0]);
	    	    cout << v_CommentsOfRunCategories[1]<<" "<< i_v_record_1 << endl;
	    map_Run_Setup[i_v_record_1]=v_record;
	  }
	} // end while ./runs/run_X/Setup
	fichier.close();
	
	//***************************************************************************************
	//
	// Filename for open File
	//
	//****************************************************************************************
	
	//************************************************************************************
	map< int , map <string,string> >  map_ListOfRun_Setup; // map < #Setup , map < category comment , #value> >
	//*************************************************************************************
	
	cout << "filename" << endl;
	
	// 	std::map<Int_t, vector <string>  > map_Run_Setup;
	vector <string> v_CommentsOfRunCategories_temp;
	
	for (auto& x: map_Run_Setup) 
	{  
	  
	  if(x.first==-1){ 
	    // 	    cout <<"\nSetup Comment " <<x.first <<"\n\n" ; 
	    string str = "#";
	    for(auto& y: x.second){
	      if(y.at(0) != str)v_CommentsOfRunCategories_temp.push_back(y);
	    }
	  }
	  else{ 
	    // 	    cout <<"\nSetup " <<x.first <<"\n\n" ; 
	    int i_position =0;
	    for(auto& y: x.second){
	      map_ListOfRun_Setup[x.first][v_CommentsOfRunCategories_temp[i_position]]=y;
	      i_position++;
	    }
	    
	  }
	}
	
	
	// Verification map_Run_Setup
	cout << "\nVerifications map_ListOfRun_Setup " << endl;
	for (auto& x: map_ListOfRun_Setup) {
	  
	  cout <<"\nSetup " <<x.first <<"\n\n";
	  for(auto& y: x.second){
	    cout << y.first << " : "<< y.second << endl; 
	  }
	  
	  
	}
	cout << "\n";
	
	
	
	//*******************************************************************************************
	//
	// Open runs/run_X/Dose_Results/Dose_XXX
	//
	//*********************************************************************************************
	
	vector <TH1D*> v_EdepZ;
	vector <TH1D*> v_hTH1D;
	
	vector <TH2D*> v_hTH2D;
	
	vector<string> v_version_GEANT4;
	
	
	
	for(auto& x: map_ListOfRun_Setup){
	  
	  cout<< "Setup : " << x.first << endl;
	  cout <<"Name of File "<< x.second["NAMEDOSEEDEP"] << endl;
	  
	  
	  
	  TString Path_Name_Run = "./runs/run_" +std::to_string(x_Run_Number) + "/Dose_Results/";
	  TString NameSetup =  x.second["NAMEDOSEEDEP"];
	  TString Name_Run = Path_Name_Run+ NameSetup;
	        cout << "\n" << Name_Run << endl;
	  
		if(x.second["NAMEDOSEEDEP"] == ""){cout << "Pb NAMEDOSEEDEP " << endl; continue;}
		
	  bool flag_One_Run = false;
	  
	  string V_geant4=x.second["V_GEANT4"];
	  
// 	  v_version_GEANT4.push_back(V_geant4);
	  
	  ifstream fichier (Name_Run,ios::in);// ouvre un nouveau fichier en lecture. On lui associe l'objet: fichier
	  if(!fichier){cout << " L'erreur est humaine, admettre la sienne est surhumain. [Doug Larson]" << endl;}
	  else 
	  {
	    cout << "\nFile  "<< NameSetup << " is open\n"  << endl;
	    map <string , string> map_Setup_Comment;
	    vector < string > v_CommentsOfDoseCategories;
	    
	    TH1D *hEdep_Z = NULL;
	    TH1D *hDose_Z = NULL;
	    TH2D *hEdep_XY = NULL;
	    TH2D *hDose_XY = NULL;
	    
	    TH3D *h_Edep_XYZ = NULL;
	    TH3D *h_Edep_XYZ_Entries = NULL;
	    
	    map <int, double [2] > Error_Bin_Z_Edep;
	    map <int, double [2] > Error_Bin_Z_Dose;
	    
	    map < int,map<int,double [2] > > Error_Bin_XY_Edep;
	    map < int,map<int,double [2] > > Error_Bin_XY_Dose;
	    
	    TH2D *hEdep_Z_Error = NULL;
	    
	    nbLignes = 0;
	    
	    bool Flag_Dose=0;
	    double MultiplyFactor = 1;
	    double Bin_X_Per_mm=1;
	    double Bin_Y_Per_mm=1;
	    double Bin_Z_Per_mm=1;
	    
	    
	    while (fichier)
	    {
	      nbLignes++;
	      
	      string s;
	      if (!getline( fichier, s )) break;
	      
	      istringstream ss( s );
	      
	      string str = "#";
	      string strd ="d"; 
	      
	      
	      string strDose = "# iX  iY  iZ  value [Gy]";
	      string strEdep = "# iX iY iZ value [MeV]";
	      
	      if(V_geant4=="10.3"){
		strDose = "# iX  iY  iZ  total(value) [Gy]  total(val^2)  entry";
		strEdep = "# iX  iY  iZ  total(value) [MeV]  total(val^2)  entry";
	      }
	      else /*cout << "vGeant "<< V_geant4 << " Default version "<<endl*/;
	      
	      string strDate = "#d";
	      string strCase="## MATERIAL Energy[MeV] EVENT_NB BOXSIZE_X BOXSIZE_Y BOXSIZE_Z BOXSIZE_UNIT BINSIZE_X BINSIZE_Y BINSIZE_Z V_GEANT4 Time[s]";
	      
	      
	      
	      // 			  cout << s << endl ;
	      Int_t Comment=0;
	      if(s.at(0) == str){/*cout << s << endl;*/ Comment=1; }
	      if(s.at(1) == strd && Comment==1) Comment = 2;
	      if(s.at(1) == str && Comment==1) Comment = 3; // Name Of Setup conditions
	      if(s.at(0) == str && s.at(1) == str  && s.at(2) == str )Comment =4; // Setup condition
	      
	      if(s==strDose){Flag_Dose = true;cout << "Flag_Dose "<<Flag_Dose << endl; }
	      
	      vector <string> v_record;
	      
	      // 	    	cout << "s  "<< s << endl;
	      
	      while (ss)
	      {
		string s;
		// 		  if ((!getline( ss, s, ',' ))) break;
		if ((!getline( ss, s, ' ' ))) break;
		
		v_record.push_back( s );
	      }
	      
	      // 	    cout << "Before Break" << endl;
	      
	      if(Comment == 3){for (auto& y: v_record) { v_CommentsOfDoseCategories.push_back(y); /*cout << y << endl;*/}}
	      
	      // 	    cout << "Before Break" << endl;
	      
	      if(Comment==4){
		int i_position=0;
		for (auto& y: v_record) { 
		  map_Setup_Comment[v_CommentsOfDoseCategories[i_position]]=y;
		  i_position++;
		}
		// 	      cout << "Before Break" << endl;
		// # NUMBERSETUP NAMEDOSEEDEP ENERGY MATERIAL EVENT_NB BOXSIZE_X BOXSIZE_Y BOXSIZE_Z BOXSIZE_UNIT BINSIZE_X BINSIZE_Y BINSIZE_Z V_GEANT4
		
		double ENERGY 	=  ToDouble(map_Setup_Comment["ENERGY"]);
		string MATERIAL	=  map_Setup_Comment["MATERIAL"];
		
		int EVENT_nb	=  ToInteger(map_Setup_Comment["EVENT_NB"]);
		
		double bOXSIZE_x 	=  ToDouble(map_Setup_Comment["BOXSIZE_X"]);
		double bOXSIZE_y 	=  ToDouble(map_Setup_Comment["BOXSIZE_Y"]);
		double bOXSIZE_z 	=  ToDouble(map_Setup_Comment["BOXSIZE_Z"]);
		
		double Real_bOXSIZE_x 	= bOXSIZE_x*2;
		double Real_bOXSIZE_y 	= bOXSIZE_y*2;
		double Real_bOXSIZE_z 	= bOXSIZE_z*2;
		
		string bOXSIZE_unit 	=  map_Setup_Comment["BOXSIZE_UNIT"];
		
		int bINSIZE_x 	=  ToInteger(map_Setup_Comment["BINSIZE_X"]);
		int bINSIZE_y 	=  ToInteger(map_Setup_Comment["BINSIZE_Y"]);
		int bINSIZE_z 	=  ToInteger(map_Setup_Comment["BINSIZE_Z"]);
		string V_geant4 	=  (map_Setup_Comment["V_GEANT4"]) ;
		
// 		cout << "Verification bin X " << bINSIZE_x << " Y " << bINSIZE_y << " Z " << bINSIZE_z << endl;
// 		cout << "Verification size X " << Real_bOXSIZE_x << " Y " << Real_bOXSIZE_y << " Z " << Real_bOXSIZE_z << endl;
		
		// Multiply factor 
		
		if(bOXSIZE_unit=="mm"){MultiplyFactor = 1;}
		else if(bOXSIZE_unit=="cm"){MultiplyFactor = 10;}
		else{cout << "FATAL ERROR implement unit exit 0" << endl; exit(0);} 
		
		Bin_X_Per_mm 	 = (static_cast<double>(bINSIZE_x)/(Real_bOXSIZE_x))*MultiplyFactor;
		Bin_Y_Per_mm 	 = (static_cast<double>(bINSIZE_y)/(Real_bOXSIZE_y))*MultiplyFactor;
		Bin_Z_Per_mm 	 = (static_cast<double>(bINSIZE_z)/(Real_bOXSIZE_z))*MultiplyFactor;
		
		
		map_Setup_Comment["Bin_X_Per_mm"]=DoubleToString(Bin_X_Per_mm);
		map_Setup_Comment["Bin_Y_Per_mm"]=DoubleToString(Bin_Y_Per_mm);
		map_Setup_Comment["Bin_Z_Per_mm"]=DoubleToString(Bin_Z_Per_mm);
		
		map_Setup_Comment["Real_bOXSIZE_x"]=(DoubleToString(Real_bOXSIZE_x));
		map_Setup_Comment["Real_bOXSIZE_y"]=DoubleToString(Real_bOXSIZE_y);
		map_Setup_Comment["Real_bOXSIZE_z"]=DoubleToString(Real_bOXSIZE_z);
		
		map_Setup_Comment["Run"]=std::to_string(x_Run_Number);
		
		cout << MATERIAL << " Energy " << ENERGY << " boxsizex " << bOXSIZE_x<< " unit " << bOXSIZE_unit<< " binsizey " << bINSIZE_x<< " vgeant " << V_geant4 << " binperz "<< Bin_Z_Per_mm << " binperx "<< Bin_X_Per_mm << " binpery "<< Bin_Y_Per_mm<< endl;
		
		
		// TODO include data texte file + add file in ROOT file
		
		// 		TH1D* Tab_Name[1]={hEdep_Z};
// 		cout <<"new " << endl; 
		
		hEdep_Z = new TH1D("hEdep_Z","Edep Z; Z(mm);Edep",bINSIZE_z,0,Real_bOXSIZE_z);
		hDose_Z = new TH1D("hDose_Z","Dose (Gy) Z(mm)); Z;Dose (Gy)",bINSIZE_z,0,Real_bOXSIZE_z);
		hEdep_XY = new TH2D("hEdep_XY","Edep Z; X(mm);Y(mm)",bINSIZE_x,0,Real_bOXSIZE_x,bINSIZE_y,0,Real_bOXSIZE_y);
		hDose_XY = new TH2D("hDose_XY","Dose (Gy) ;X(mm);Y(mm)",bINSIZE_x,0,Real_bOXSIZE_x,bINSIZE_y,0,Real_bOXSIZE_y);
		
		h_Edep_XYZ= new TH3D ("h_Edep_XYZ","Edep XYZ; z(mm);x;y",bINSIZE_z,0,Real_bOXSIZE_z,bINSIZE_x,0,Real_bOXSIZE_x,bINSIZE_y,0,Real_bOXSIZE_y);
		
		h_Edep_XYZ->SetOption("hist,LEGO");
		
		if(V_geant4=="10.3"){
		  
// 		  cout << "Geant4 10.3" << endl;
		  h_Edep_XYZ_Entries = new TH3D ("h_Edep_XYZ_Entries","Edep XYZ entires; z(mm);x;y",bINSIZE_z,0,Real_bOXSIZE_z,bINSIZE_x,0,Real_bOXSIZE_x,bINSIZE_y,0,Real_bOXSIZE_y);
		  
		  h_Edep_XYZ_Entries->SetOption("hist,LEGO");
		  
		  hEdep_Z_Error = new TH2D("hEdep_Z_Error","Edep Z_ERROR; Z(mm); % erreur",bINSIZE_z,0,Real_bOXSIZE_z,100,0,100);
		}
		
		
		
		string config="";
		
		for (auto& y: map_Setup_Comment) { 
		  // 			    cout << y.first <<" "<< y.second<< endl;
		  if(y.first != "NAMEDOSEEDEP"){
		    config+= "_";
		    config += y.second;
		  }
		}	
		
		TString Temp_hEdep_Z = hEdep_Z->GetName()+config;
		hEdep_Z->SetName(Temp_hEdep_Z.Data());
		map_Setup_Comment["Name_hEdep_Z"]=Temp_hEdep_Z;
		
		
		TString Temp_hDose_Z = hDose_Z->GetName()+config;
		hDose_Z->SetName(Temp_hDose_Z.Data());
		map_Setup_Comment["Name_hDose_Z"]=Temp_hDose_Z;
		
		TString Temp_hEdep_XY = hEdep_XY->GetName()+config;
		hEdep_XY->SetName(Temp_hEdep_XY.Data());
		map_Setup_Comment["Name_hEdep_XY"]=Temp_hEdep_XY;
		
		TString Temp_hDose_XY = hDose_XY->GetName()+config;
		hDose_XY->SetName(Temp_hDose_XY.Data());
		map_Setup_Comment["Name_hDose_XY"]=Temp_hDose_XY;
		
		TString Temp_h_Edep_XYZ = h_Edep_XYZ->GetName()+config;
		h_Edep_XYZ->SetName(Temp_h_Edep_XYZ.Data());
		map_Setup_Comment["Name_h_Edep_XYZ"]=Temp_h_Edep_XYZ;
		
		
		if(V_geant4=="10.3"){
		  TString Temp_hEdep_Z_Error = hEdep_Z_Error->GetName()+config;
		  hEdep_Z_Error->SetName(Temp_hEdep_Z_Error.Data());
		  map_Setup_Comment["Name_hEdep_Z_Error"]=Temp_hEdep_Z_Error;
		  
		  TString Temp_h_Edep_XYZ_Entries = h_Edep_XYZ_Entries->GetName()+config;
		  h_Edep_XYZ_Entries->SetName(Temp_h_Edep_XYZ_Entries.Data());
		  map_Setup_Comment["Name_h_Edep_XYZ_Entries"]=Temp_h_Edep_XYZ_Entries;
		  
		}
		
		if(flag_One_Run==false){
		  int i_position_map_Setup_Comment = 1;
		  for (auto& y: map_Setup_Comment) { 
		    // 			    cout << y.first <<" "<< y.second<< endl;
		    if(y.first != "NAMEDOSEEDEP"){
		      Save_Setup << y.first;
		      if(i_position_map_Setup_Comment<map_Setup_Comment.size()) Save_Setup << " ";
		      i_position_map_Setup_Comment++;
		    }
		  }	
		  Save_Setup << endl;
		  
		}
		int i_position_map_Setup_Comment = 1;
		for (auto& y: map_Setup_Comment) { 
		  // 			    cout << y.first <<" "<< y.second<< endl;
		  if(y.first != "NAMEDOSEEDEP"){
		    Save_Setup << y.second;
		    if(i_position_map_Setup_Comment<map_Setup_Comment.size()) Save_Setup << " ";
		    i_position_map_Setup_Comment++;
		  }
		}	
		Save_Setup << endl;
		
		
		
	      }
	      
	      if(Comment ==0 && v_record.size()>0)
	      {  
		// print record
		// 		  		if(Flag_Dose==false)  for (auto& x: v_record) {cout << x << endl;} 
		
		if(v_record.size() <4){ cout << " Pb Dose File check data column exit(0)" << endl; exit(0);}
		
		double File_bin_x = ToDouble(v_record[0])/Bin_X_Per_mm;
		double File_bin_y = ToDouble(v_record[1])/Bin_Y_Per_mm;
		double File_bin_z = ToDouble(v_record[2])/Bin_Z_Per_mm;
		double File_bin_value = ToDouble( v_record[3]);
		
		if(Flag_Dose==0){
		  hEdep_Z->Fill(File_bin_z,File_bin_value);
		  hEdep_XY->Fill(File_bin_x,File_bin_y,File_bin_value);
		  
		  h_Edep_XYZ->Fill(File_bin_z,File_bin_x,File_bin_y,File_bin_value);
		  
		  if(V_geant4=="10.3"){
		    double error_bin =  (File_bin_value/sqrt(ToDouble( v_record[5])));
		    
		    h_Edep_XYZ_Entries->Fill(File_bin_z,File_bin_x,File_bin_y,ToInteger( v_record[5]));
		    
		    Error_Bin_Z_Edep[(ToInteger(v_record[2])+1)][0]+=error_bin*File_bin_value;
		    Error_Bin_Z_Edep[(ToInteger(v_record[2])+1)][1]+=File_bin_value;
		    Error_Bin_XY_Edep[(ToInteger(v_record[0])+1)][(ToInteger(v_record[1])+1)][0]+=error_bin*File_bin_value;
		    Error_Bin_XY_Edep[(ToInteger(v_record[0])+1)][(ToInteger(v_record[1])+1)][1]+=File_bin_value;
		    
		    // To test 
		    double error_bin_pourcent = (error_bin/File_bin_value)*100;
		    hEdep_Z_Error->Fill(File_bin_z,error_bin_pourcent,File_bin_value);
		    
		  }// end if v geant4
		  
		}
		else{   
		  // 		
		  hDose_Z->Fill(File_bin_z,File_bin_value);
		  hDose_XY->Fill(File_bin_x,File_bin_y,File_bin_value);
		  if(V_geant4=="10.3"){
		    double error_bin =  (File_bin_value/sqrt(ToDouble( v_record[5])));
		    Error_Bin_Z_Dose[(ToInteger(v_record[2])+1)][0]+=error_bin*File_bin_value;
		    Error_Bin_Z_Dose[(ToInteger(v_record[2])+1)][1]+=File_bin_value;
		    Error_Bin_XY_Dose[(ToInteger(v_record[0])+1)][(ToInteger(v_record[1])+1)][0]+=error_bin*File_bin_value;
		    Error_Bin_XY_Dose[(ToInteger(v_record[0])+1)][(ToInteger(v_record[1])+1)][1]+=File_bin_value;
		  }// end if v geant4
		}
		
	      }
	    } // end while ./runs/run_X/Dose_Results_X
	    fichier.close();
	    
	    cout << "v "<< V_geant4 << endl;
	    
	    // Calcul Error
	    if(V_geant4=="10.3"){
	      for (auto& y: Error_Bin_Z_Edep) {if(y.second[1]>0){double error_bin_ponderate_mean = y.second[0]/y.second[1];hEdep_Z->SetBinError(y.first,error_bin_ponderate_mean);}}
	      
	      for(auto& y: Error_Bin_XY_Edep){for(auto& z: y.second){
		if(z.second[1]>0){double error_bin_ponderate_mean = z.second[0]/z.second[1];hEdep_XY->SetBinError(y.first,z.first,error_bin_ponderate_mean);}
	      } }
	      
	      for (auto& y: Error_Bin_Z_Dose) {if(y.second[1]>0){double error_bin_ponderate_mean = y.second[0]/y.second[1];hDose_Z->SetBinError(y.first,error_bin_ponderate_mean);}}
	      
	      for(auto& y: Error_Bin_XY_Dose){for(auto& z: y.second){
		if(z.second[1]>0){double error_bin_ponderate_mean = z.second[0]/z.second[1];hDose_XY->SetBinError(y.first,z.first,error_bin_ponderate_mean);}
	      } }
	      
	    }
	    
	    
// 	    v_EdepZ.push_back(hEdep_Z);
// 	    v_hTH2D.push_back(hEdep_Z_Error);
	    
	    // for test
// 	    for (auto& y: map_Setup_Comment) { 
// // 	      cout << y.first <<" "<< y.second<< endl;
// 	      
// 	    }
	  
	  Output_File->Add(hEdep_Z);
	  Output_File->Add(hDose_Z);
	  Output_File->Add(hEdep_XY);
	  Output_File->Add(hDose_XY);
	  Output_File->Add(h_Edep_XYZ);
	  if(V_geant4=="10.3"){Output_File->Add(h_Edep_XYZ_Entries);Output_File->Add(hEdep_Z_Error);}
	  
	  Output_File->Write();gSystem->ProcessEvents();
	  delete hEdep_Z; delete hDose_Z; delete hEdep_XY; delete hDose_XY; delete h_Edep_XYZ;
          if(V_geant4=="10.3"){delete h_Edep_XYZ_Entries;  delete hEdep_Z_Error;}
	    
	     fichier.close();
	  } // end else IF OPEN FILE SUCESS
	 
	  
	  
	} // end for map_ListOfRun_Setup
	
// 	int i_position = 0;
// // 	TCanvas* c1= new TCanvas("c1", "BraggPeak", 5,5,1200,1000);
// 	
// 	  NameCanvas = constants->NameCanvas(Number_temp, "BraggPeak");
//           C_temp= new TCanvas(NameCanvas, NameCanvas, 5,5,1200,1000);
// 	
// 	cout << v_EdepZ.size() << endl;
// 	i_position = 0;
// 	for(auto& x: v_EdepZ){
// 	  
// 	  
// 	  if(v_version_GEANT4[i_position]=="10.3"){
// 	    if(i_position==0)x->Draw();
// 	    else x->Draw("same");
// 	  }
// 	  else{
// 	    if(i_position==0)x->Draw("hist");
// 	    else x->Draw("hist same");
// 	  }
// 	  
// // 	  cout << "i position " << i_position << endl;
// 	  i_position++;
// 	}
// 	
// 	if (C_temp) { C_temp->Close(); gSystem->ProcessEvents(); delete C_temp; C_temp = 0; } 
// 	
// 	  NameCanvas = constants->NameCanvas(Number_temp, "hEdepEroor");
//           C_temp= new TCanvas(NameCanvas, NameCanvas, 5,5,1200,1000);
// 	
// // 	TCanvas* c2= new TCanvas("c2", "hEdepEroor", 5,5,1200,1000);
// 	
// 	i_position = 0;
// 	for(auto& x: v_hTH2D){
// 	  
// 	  if(v_version_GEANT4[i_position]=="10.3"){
// 	    if(i_position==0)x->Draw();
// 	    else x->Draw("same");
// 	  }
// 	  else{
// 	  }
// 	  
// 	  cout << "i position " << i_position << endl;
// 	  i_position++;
// 	}
// 	
// 
// 	// 	for (auto& y: v_EdepZ){/*y-> SetDirectory (Output_File);*/Output_File->Add(y);}
// 	
// 	  
// 	if (C_temp) { C_temp->Close(); gSystem->ProcessEvents(); delete C_temp; C_temp = 0; } 
	

	v_EdepZ.clear();
	v_hTH2D.clear();
	  
      } // end else ./runs/run_X/Setup open Run file 
    } // end ListOfRun
  } // end else if ./runs/Setup is OPEN
  
  //   Output_File->cd(); 

   Output_File->Write();
  Output_File->Close();
  
  Save_Setup.close();
  Save_Run.close();
  
  if(Begin_Run>1)cout << "warning : Analyse start at run " <<  Begin_Run << endl;
  
  system("pwd");
  
} // end Main