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
#include "TGLHistPainter.h"
#include <TMath.h>

// A simple TTreeReader use: read data from hsimple.root (written by hsimple.C)
#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TMultiGraph.h"

#include <sstream>
#include <string>
#include "TSystem.h"

// #include "./Include/rootlogon.C"

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

double GetNDC(double x) {
  gPad->Update();//this is necessary!
  return (x - gPad->GetX1())/(gPad->GetX2()-gPad->GetX1());
}

double GetNDC_Y(double x) {
  gPad->Update();//this is necessary!
  return (x - gPad->GetY1())/(gPad->GetY2()-gPad->GetY1());
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

string IntToString (int var )
{
  string str = std::to_string(var); 
  str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
  //   if(strncmp(&str.back(),".",1)==0)cout <<"str before " <<str << endl;
  if(strncmp(&str.back(),".",1)==0){str.pop_back(); 
    /*cout <<"str after " <<str << endl;*/}
    return str;
}

TString fNameCanvas (int &var , TString str )
{
  var +=1;
  str= "c"+IntToString(var);
  return str;
}

void Process_Draw()
{
  TString Path="./Base_Data_Scoring/";
  TString Name = "Data_Base.txt";
  
  TString Name_Folder_ROOT="./Base_Data_Scoring/Data_Base_histo.root";
  
  // initialisation
  TString NameCanvas = "C1";
  TCanvas * C_temp = NULL;
  Int_t Number_temp = 0;
  TString NameForGet="";
  
  int NumberOfRunData = 0;
  map <int, map <string, string> > Load_Base_Data;
  TString NameRun= Path+Name;
  
  ifstream fichier (NameRun,ios::in);// ouvre un nouveau fichier en lecture. On lui associe l'objet: fichier
  if(!fichier){cout << " L'erreur est humaine, admettre la sienne est surhumain. [Doug Larson]" << endl;}
  else 
  {
    cout << NameRun << " is open"  << endl;
    
    
    
    vector <string> v_CommentsOfRunCategories;
    
    while (fichier)
    {
      
      string s;
      
      if (!getline( fichier, s )) break;
      
      istringstream ss( s );
      
      string str = "#";
      string strd ="d"; 
      string separator = " ";
      
      // 	  cout << s << endl ;
      Int_t Comment=0;
      if(s.at(0) == str){/*cout << s << endl;*/ Comment=1; }
      if(s.at(0) == str && s.at(1) == str && s.at(2) == separator) Comment = 2;
      if(s.at(0) == str && s.at(1) == str && s.at(2) == str && s.at(3) == separator ) Comment = 3;
      
      vector <string> v_record;
      
      // 	cout << "s  "<< s << endl;
      while (ss)
      {
	string s;
	if ((!getline( ss, s, ' ' ))) break;
	v_record.push_back( s );
      }
      
      if(Comment == 2){
	v_CommentsOfRunCategories.clear();
	for (auto& x: v_record) { v_CommentsOfRunCategories.push_back(x); /*cout << x << endl;*/} 
      }
      
      if(Comment ==3){
	NumberOfRunData++;
	int i_position =0;
	for(auto& x: v_record){
	  Load_Base_Data[NumberOfRunData][v_CommentsOfRunCategories[i_position]]=x;
	  // 	      cout << x << endl;
	  i_position++;
	}
      }
      
    } // end while ./runs/run_X/Setup
    fichier.close();
    
    //*************************************************************************************************************
    // 
    // Read Loaded Base Data
    //
    //***************************************************************************************************************
    
    
    TFile *Input_File = new TFile(Name_Folder_ROOT.Data(), "READ");
    //     Input_File->ls(); 
    vector <TH1D*> v_BraggPeakDifferentEnergy_PMMA;
    
    // Comment + Map // Proton /* MATERIAL or VAR PARAMETER
    map < /* Particle (p,C..) */ string , map < /* MATERIAL*/ string ,map < /*Energy (MeV)*/ double ,vector <TH1D*>> >> map_RangeVsEnergyVsMaterial; // 
    map <string ,vector <string> > map_RangeVsEnergyVsMaterial_Comment;
    
    // WARNING for test
    map  < string, TH1D*> BoreOrNotBore ;
    
    for(auto& x: Load_Base_Data){
      cout << "# " << x.first << " Run" << x.second["Run"] <<" Setup " <<x.second["NUMBERSETUP"] << endl;
      // 	 for(auto& y: x.second){
      // 	   cout << y.first << " " << y.second << endl;
      // 	 }
      
      
      int Run_Number = ToInteger(x.second["Run"]);
      double ENERGY_BEAM = ToDouble(x.second["ENERGY"]); 
      string TARGETOPTION = x.second["TARGETOPTION"]; if(TARGETOPTION =="")TARGETOPTION="NO";
      
      
      int Print_gamma = 0;
      
      if(Run_Number==10 && ENERGY_BEAM==6 && Print_gamma == 1){
		     
		  // Test gamma
	
		  NameCanvas = fNameCanvas(Number_temp, NameCanvas);
		  C_temp= new TCanvas(NameCanvas, "BraggPeak 65 MeV Proton", 5,5,1200,1000);
	  
		  C_temp->Divide(2,1);
		   C_temp->cd(1);  
		   
		   NameForGet = x.second["Name_hEdep_XY"];
		   TH2D* Name_hEdep_XY = (TH2D*)Input_File->Get(NameForGet.Data());
		   Name_hEdep_XY->GetXaxis()->SetRange(0,400);
		   cout << Name_hEdep_XY << endl;
		   Name_hEdep_XY->Draw("box,colz");
		  
		   C_temp->cd(2);  
		  
		   NameForGet = x.second["Name_hEdep_Z"];
		   TH1D* Name_hEdep_Z = (TH1D*)Input_File->Get(NameForGet.Data());
		   Name_hEdep_Z->GetXaxis()->SetRange(0,400);
		   Name_hEdep_Z->SetMarkerSize(0.1);
		   cout << Name_hEdep_Z << endl;
		   Name_hEdep_Z->Draw("box,colz");
		  
	// 	   gStyle->SetCanvasPreferGL(true);
		   NameCanvas = fNameCanvas(Number_temp, NameCanvas);
		   C_temp= new TCanvas(NameCanvas, "BraggPeak 65 MeV Proton 3D", 5,5,1200,1000);
	  
		   C_temp->cd(1);  
		    
		   NameForGet = x.second["Name_h_Edep_XYZ"];
		   TH3D* hEdep_XYZ = (TH3D*)Input_File->Get(NameForGet.Data());
		   cout << hEdep_XYZ << endl;
	// 	   gStyle->SetPalette(1);
		   hEdep_XYZ->GetXaxis()->SetRange(0,400);
		   hEdep_XYZ->SetMarkerSize(0.1);
	// 	   hEdep_XYZ->SetLineColor(4);
		    hEdep_XYZ->SetFillStyle(0);
		   hEdep_XYZ->Draw("BOX2,PLC,COLZ");
	// 	   hEdep_XYZ->Draw("GLCOL");
		 
	
		   NameForGet = x.second["Name_h_Edep_XYZ_Entries"];
		   TH3D* h_Edep_XYZ_Entries = (TH3D*)Input_File->Get(NameForGet.Data());
		   cout << h_Edep_XYZ_Entries << endl;
		   h_Edep_XYZ_Entries->Draw("lego,colz");
	
	
	
	
	
	
	
      }
            if(Run_Number==24){
		  
	      cout << "I am Here" << endl;
	      
		  // Test gamma
	
		  NameCanvas = fNameCanvas(Number_temp, NameCanvas);
		  C_temp= new TCanvas(NameCanvas, "BraggPeak 110 MeV Proton", 5,5,1200,1000);
	  
		  C_temp->Divide(2,1);
		   C_temp->cd(1);  
		   
		   NameForGet = x.second["Name_hEdep_XY"];
		   TH2D* Name_hEdep_XY = (TH2D*)Input_File->Get(NameForGet.Data());
		   Name_hEdep_XY->GetXaxis()->SetRange(0,400);
		   cout << Name_hEdep_XY << endl;
		   Name_hEdep_XY->Draw("box,colz");
		  
		   C_temp->cd(2);  
		  
		   NameForGet = x.second["Name_hEdep_Z"];
		   TH1D* Name_hEdep_Z = (TH1D*)Input_File->Get(NameForGet.Data());
		   Name_hEdep_Z->GetXaxis()->SetRange(0,400);
		   Name_hEdep_Z->SetMarkerSize(0.1);
		   cout << Name_hEdep_Z << endl;
		   Name_hEdep_Z->Draw("box,colz");
		  
	// 	   gStyle->SetCanvasPreferGL(true);
		   NameCanvas = fNameCanvas(Number_temp, NameCanvas);
		   C_temp= new TCanvas(NameCanvas, "BraggPeak 65 MeV Proton 3D", 5,5,1200,1000);
	  
		   C_temp->cd(1);  
		    
		   NameForGet = x.second["Name_h_Edep_XYZ"];
		   TH3D* hEdep_XYZ = (TH3D*)Input_File->Get(NameForGet.Data());
		   cout << hEdep_XYZ << endl;
	// 	   gStyle->SetPalette(1);
		   hEdep_XYZ->GetXaxis()->SetRange(0,400);
		   hEdep_XYZ->SetMarkerSize(0.1);
	// 	   hEdep_XYZ->SetLineColor(4);
		    hEdep_XYZ->SetFillStyle(0);
		   hEdep_XYZ->Draw("BOX2,PLC,COLZ");
	// 	   hEdep_XYZ->Draw("GLCOL");
		 
	
		   NameForGet = x.second["Name_h_Edep_XYZ_Entries"];
		   TH3D* h_Edep_XYZ_Entries = (TH3D*)Input_File->Get(NameForGet.Data());
		   cout << h_Edep_XYZ_Entries << endl;
		   h_Edep_XYZ_Entries->Draw("lego,colz");
	
	
	
	
	
	
	
      }
      
            if(Run_Number==14 || Run_Number==3 && ENERGY_BEAM==65){
		   
	      
		 
		 
		 NameForGet = x.second["Name_hEdep_Z"];
		 BoreOrNotBore[TARGETOPTION]= (TH1D*)Input_File->Get(NameForGet.Data());
		  
		  int binmax = BoreOrNotBore[TARGETOPTION]->GetMaximumBin(); // Find the Bin corresponding to the BraggPeak
		  double binmax_X_max = BoreOrNotBore[TARGETOPTION]->GetXaxis()->GetBinCenter(binmax); // Find the coressponding in user scale (mm) on X axis
		  double binmax_Value = BoreOrNotBore[TARGETOPTION]->GetBinContent(binmax);
		  
		  cout << TARGETOPTION << " Max:" << binmax_X_max << " mm" << endl;
		  
		if(BoreOrNotBore.size()==2){
		  
		  NameCanvas = fNameCanvas(Number_temp, NameCanvas);
		  C_temp= new TCanvas(NameCanvas, "cOMPARAISON bORE NO BORE", 5,5,1200,1000);
	  
		  C_temp->Divide(2,2);
		   C_temp->cd(1);  
		   
		   
		    BoreOrNotBore["Cavity_Cube"]->SetTitle("Cavity de Bore");
		   BoreOrNotBore["Cavity_Cube"]->GetXaxis()->SetRange(0,400);
		   BoreOrNotBore["Cavity_Cube"]->SetMarkerSize(0.1);
		   cout << BoreOrNotBore["Cavity_Cube"] << endl;
		   BoreOrNotBore["Cavity_Cube"]->Draw("hist PLC PMC");
		  
		   C_temp->cd(2);  
		  
		   BoreOrNotBore["NO"]->SetTitle("NO cavity");
		   BoreOrNotBore["NO"]->GetXaxis()->SetRange(0,400);
		   BoreOrNotBore["NO"]->SetMarkerSize(0.1);
		   BoreOrNotBore["NO"]->Draw("hist PLC PMC");
		  
		   C_temp->cd(3);
		     BoreOrNotBore["Cavity_Cube"]->Draw("hist PLC PMC");
		     BoreOrNotBore["NO"]->Draw("same hist PLC PMC");
		     
		     
		     

		}
	
      }
      
      if( Run_Number>2 && Run_Number !=5 && Run_Number !=10 && Run_Number < 11 ){
	
	// 		   cout << x.second["Run"] << endl;
	NameForGet = x.second["Name_hEdep_Z"];
	// 		      TH1DTemp->SetDirectory(0);
	TH1D *TH1DTemp = ((TH1D*)Input_File->Get(NameForGet.Data()));
	// 		 TH1DTemp->Draw();
	// 	cout << TH1DTemp << endl;
	TString Title = x.second["ENERGY"];
	TH1DTemp->SetTitle(Title);
	v_BraggPeakDifferentEnergy_PMMA.push_back(TH1DTemp);
	map_RangeVsEnergyVsMaterial[x.second["PARTICLE_SOURCE"]][x.second["MATERIAL"]][ToDouble(x.second["ENERGY"])].push_back(TH1DTemp);
	
      }
      
            if( Run_Number >29 ){
	
	// 		   cout << x.second["Run"] << endl;
	NameForGet = x.second["Name_hEdep_Z"];
	// 		      TH1DTemp->SetDirectory(0);
	TH1D *TH1DTemp = ((TH1D*)Input_File->Get(NameForGet.Data()));
	// 		 TH1DTemp->Draw();
	// 	cout << TH1DTemp << endl;
	TString Title = x.second["ENERGY"];
	TH1DTemp->SetTitle(Title);
	v_BraggPeakDifferentEnergy_PMMA.push_back(TH1DTemp);
	map_RangeVsEnergyVsMaterial[x.second["PARTICLE_SOURCE"]][x.second["MATERIAL"]][ToDouble(x.second["ENERGY"])].push_back(TH1DTemp);
	
      }
      
      
    } // end load base data
    
    //
    // Print Multi Bragg Peak in PMMA
    //
    
    
    map < /* Particle (p,C..) */ TString , map < /* MATERIAL*/ TString, map </*Energy */ double,/* Range */ double > > > map_RangeVsEnergyVsMaterial_XY; // GetMaxBraggPeak et take Range;  
    
    //     map_RangeVsEnergyVsMaterial_XY[x.second["PARTICLE_SOURCE"]][x.second["MATERIAL"]][ToDouble(x.second["ENERGY"])]= /*Range mm */ ;
    
    for (auto& PARTICLE_SOURCE: map_RangeVsEnergyVsMaterial){
      
      
      
      cout << PARTICLE_SOURCE.first << endl;
      TString PARTICLE_SOURCE_str = PARTICLE_SOURCE.first;
      if(PARTICLE_SOURCE_str =="proton") PARTICLE_SOURCE_str = "Proton";
      
      for (auto& MATERIAL: PARTICLE_SOURCE.second){
	
	cout << MATERIAL.first << endl;
	TString MATERIAL_str = MATERIAL.first;
	
	
	NameCanvas = fNameCanvas(Number_temp, NameCanvas);
	C_temp= new TCanvas(NameCanvas, Name, 5,5,1200,1000);
	//   
	C_temp->Divide(1,1);
	C_temp->cd(1);  
	
	// 	   std::reverse(x.second.begin(),x.second.end());
	bool First = true;
	
	for (auto& ENERGY: MATERIAL.second){
	  // 	  cout << ENERGY.first << endl;
	  string ENERGY_str = DoubleToString(ENERGY.first) /*+ " MeV"*/;
	  for(auto& TH1D_pointeur: ENERGY.second)
	  {
	    int binmax = TH1D_pointeur->GetMaximumBin(); // Find the Bin corresponding to the BraggPeak
	    double binmax_X_max = TH1D_pointeur->GetXaxis()->GetBinCenter(binmax); // Find the coressponding in user scale (mm) on X axis
	    double binmax_Value = TH1D_pointeur->GetBinContent(binmax);
	    
	    map_RangeVsEnergyVsMaterial_XY[PARTICLE_SOURCE_str][MATERIAL_str][ENERGY.first]=binmax_X_max /*Range mm */ ;
	    
	    
	    double Range_Max = 300; // Default PMMA
	    if(MATERIAL_str=="G4_WATER")Range_Max = 350;
	    if(MATERIAL_str=="G4_BRAIN_ICRP")Range_Max = 330;
	    if(MATERIAL_str=="G4_ADIPOSE_TISSUE_ICRP")Range_Max = 350;
	    
	    TH1D_pointeur->SetAxisRange(0,(Range_Max+50),"X");
	    
	    if(First==true){
	      TH1D_pointeur->Draw("PLC PMC hist");
	      // 	    First=false;
	    }
	    else{
	      TH1D_pointeur->Draw("SAME PLC PMC hist");
	    }
	    
	    // 	  cout << binmax_X_max <<"  "<<binmax_Value << endl;
	    
	    //PutText(binmax, Y_max, kBlack, "toto");
	    if(First==true){
	      
	      // 	  PutText(GetNDC(Range_Max), GetNDC_Y(binmax_Value+100), kBlack, "Beam energy (MeV)" ,0.030);
	      // 	      gStyle->SetPalette(kRainBow);
	      
	      TString Title = PARTICLE_SOURCE_str+ " range in " + MATERIAL_str + " for Different Beam Energy (MeV)" ;
	      TH1D_pointeur->SetTitle(Title);
	      TH1D_pointeur->GetXaxis()->SetTitle("Depth [mm]");
	      TH1D_pointeur->GetYaxis()->SetTitle("Energy Deposited [a.u.]");
	      // 	   myHisto->GetXaxis()->SetLabelOffset(999);
	      
	      
	      double size_graph = 0.04;
	      
	      TH1D_pointeur->GetXaxis()->SetTitleSize(size_graph);
	      TH1D_pointeur->GetYaxis()->SetTitleSize(size_graph);
	      TH1D_pointeur->GetXaxis()->SetLabelSize(size_graph);
	      TH1D_pointeur->GetYaxis()->SetLabelSize(0);
	      
	      TH1D_pointeur->GetXaxis()->SetTitleOffset(1.30);
	      TH1D_pointeur->GetYaxis()->SetTitleOffset(0.55);
	      TH1D_pointeur->GetYaxis()->SetTickLength(0);   
	      
	      gStyle->SetOptStat(0);
	      First=false;
	    }
	    PutText(GetNDC(binmax_X_max-5), GetNDC_Y(binmax_Value+150), kBlack, ENERGY_str ,0.015);
	    
	    
	  } 
	}
	// 	       gPad->BuildLegend();
      }    
    }
    
    
    cout << "\n" << endl;
    
    //     Multigraph
    
    TMultiGraph* multi = new TMultiGraph(); TLegend *leg = new TLegend(0.1888058,0.6379379,0.3999438,0.8562011,NULL,"brNDC");
    vector <TGraph*> StockageGraphForMulti;
    
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    int counter = 0;
    for (auto& PARTICLE_SOURCE: map_RangeVsEnergyVsMaterial_XY){
      
      cout << PARTICLE_SOURCE.first << endl;
      TString PARTICLE_SOURCE_str = PARTICLE_SOURCE.first;
      if(PARTICLE_SOURCE_str =="proton") PARTICLE_SOURCE_str = "Proton";
      
      //       leg->AddEntry((TObject*)0, PARTICLE_SOURCE_str, "");
      
      for (auto& MATERIAL: PARTICLE_SOURCE.second){
	
	cout << MATERIAL.first << endl;
	TString MATERIAL_str = MATERIAL.first;
	
	// 	   std::reverse(x.second.begin(),x.second.end());
	bool First = true;
	
	std::vector<Double_t> x_TGraph;
	std::vector<Double_t> y_TGraph;
	
	
	
	for (auto& ENERGY: MATERIAL.second){
	  x_TGraph.push_back(ENERGY.first);
	  // 	  cout <<  x_TGraph.back() << endl;
	  y_TGraph.push_back(ENERGY.second);
	  cout << "Energy " << ENERGY.first << " Depth "<< ENERGY.second << endl;
	  
	  
	}
	Int_t n = x_TGraph.size();
	StockageGraphForMulti.push_back ( new TGraph(n,&x_TGraph[0],&y_TGraph[0]));
	
	// 	  NameCanvas = fNameCanvas(Number_temp, NameCanvas);
	//           C_temp= new TCanvas(NameCanvas, "Test BraggPeak RANGE MeV Proton", 5,5,1200,1000);
	//           //   
	//           C_temp->Divide(1,1);
	//           C_temp->cd(1);  
	// 	  StockageGraphForMulti.back()->Draw();
	
	leg->AddEntry(StockageGraphForMulti.back(), MATERIAL_str, "alp");
	// 	 leg->AddEntry(StockageGraphForMulti.back(),TString::Format("%d MeV",PosY),"ap"); // 
	
	multi->Add(StockageGraphForMulti.back(),"PLC PMC");
	
      }
      //        if(counter < PARTICLE_SOURCE.second.size()-1)  leg->AddEntry((TObject*)0, "", "");
      counter++;
    }
    
    TGraph * gr_control = new TGraph();
    gr_control->SetPoint(0,138,132.068101923121);
    gr_control->SetPoint(1,160,173.34654179311);
    gr_control->SetPoint(2,177,207.799412865699);
    gr_control->SetPoint(3,214,285.48088632182);
    
//     leg->AddEntry(gr_control,  "Water Pedroni et al 2005", "ap");
//     multi->Add(gr_control,"PLC PMC");
    
    //     multi->Draw;
    NameCanvas = fNameCanvas(Number_temp, NameCanvas);
    C_temp= new TCanvas(NameCanvas, "Test BraggPeak RANGE MeV Proton", 5,5,1200,1000);
    //   
//     C_temp->SetCanvasSize(3072 ,  1920); // 16:10
    C_temp->Divide(1,1);
    C_temp->cd(1);  
    
    
    multi->Draw("a PLC PMC");
    
    multi->SetTitle("Proton range in function of beam energy for different matters");
    multi->GetXaxis()->SetTitle("Beam energy [MeV]");
    multi->GetYaxis()->SetTitle("Depth [mm]");
    
    // cosmetic
    double size_graph = 0.04;
    
    multi->GetXaxis()->SetTitleSize(size_graph);
    multi->GetYaxis()->SetTitleSize(size_graph);
    multi->GetXaxis()->SetLabelSize(size_graph);
    multi->GetYaxis()->SetLabelSize(size_graph);
    
    multi->GetXaxis()->SetTitleOffset(1.30);
    multi->GetYaxis()->SetTitleOffset(1.35);
    
                 multi->GetXaxis()->SetNdivisions(20, 10, 0, kTRUE);
		 multi->GetYaxis()->SetNdivisions(20, 10, 0, kTRUE);
		 
		   multi->GetXaxis()->SetMoreLogLabels(kTRUE);
     multi->GetXaxis()->SetNoExponent(kFALSE);
		 
    string TitleSimulation = " #splitline{Simulation G4 10.3p02}{Proton (p) no beam extension)}";
    PutText(0.54, 0.81, kBlack, TitleSimulation ,0.030);
    
    //     leg->SetTextAlign(22);
    
    leg->SetHeader("Target 5x5x50 cm^{3}");
    
      leg->SetBorderSize(1);
      leg->SetTextAlign(22);
      leg->SetTextFont(42);
      leg->SetTextSize(0.025);
      leg->SetLineColor(1);
      leg->SetLineStyle(1);
      leg->SetLineWidth(2);
      leg->SetFillColor(10);
      leg->SetFillStyle(1001);
    leg->Draw();
    TLegendEntry *header = (TLegendEntry*)leg->GetListOfPrimitives()->First();
    //     header->SetTextAlign(22);
    //     header->SetTextColor(2);
    header->SetTextSize(.030);
    
    gPad->SetGrid();
    gPad->Modified();
    gPad->Update();
    
    //     std::cout << ' ' << *rit;
    //   std::cout << '\n';
    
    
    //    h1->SetMarkerStyle(kFullCircle);
    //    h2->SetMarkerStyle(kFullSquare);
    //    h3->SetMarkerStyle(kFullTriangleUp);
    //    h4->SetMarkerStyle(kFullTriangleDown);
    //    h5->SetMarkerStyle(kOpenCircle);
    //    h1->Draw("PLC PMC");
    //    h2->Draw("SAME PLC PMC");
    //    h3->Draw("SAME PLC PMC");
    //    h4->Draw("SAME PLC PMC");
    //    h5->Draw("SAME PLC PMC");
    //    gPad->BuildLegend();
    
    
    // 	 for (auto rit = myvector.crbegin(); rit != myvector.crend(); ++rit)
    //     std::cout << ' ' << *rit;
    //   std::cout << '\n';
    // 	
    
    
    
    
  }
}