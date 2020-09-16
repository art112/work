#!/bin/bash


START=$(date +%s)

echo Running on host `hostname`
HOSTNAME=`hostname`


time ls -lsah

# PathFile="/media/sf_PartageVbox8/GATE/All_Simulation/z_Projet_Arronax_Compensator_design/"

PathFile="/home/gate/nantes_sps/abongran/All_Results/z_Projet_Manip_ZebraFish_11_03_20/9.0/"


Coef_Event="1"
Unit_Event="M" 
  
 
 
#       vec_Option.push_back("vDefault2_Ligne_ICO_v1_68MeV_0_");
#          vec_Option.push_back("vDefault2_Ligne_ICO_v1_68MeV_0.68_");
#          vec_Option.push_back("vDefault2_Ligne_ICO_v1_68MeV_1_");



#https://www.developpez.net/forums/d1528120/general-developpement/langages-programmation/vecteur-boucle-shell/


 #std::vector<string> Category_Of_Analyse {"Epaisseur", "Pos"};
#option=("68MeV_0_" "68MeV_1_" "68MeV_0.68_" "v1_68MeV_0.68_")  # initialisation du tableau

# option=("v1_68MeV_0_") 

# option=("vACDv4.2_Ligne_ICO_v2T_68MeV_0_") 
# 
#               vec_Option.clear();
#          NameSetup="vACDv4.3_Ligne_ICO_v2T";
#          CompleteNameSetup= NameSetup +"_68MeV_0_" ; vec_Option.push_back(CompleteNameSetup);
#          CompleteNameSetup= NameSetup +"_68MeV_0.68_" ; vec_Option.push_back(CompleteNameSetup);
#          CompleteNameSetup= NameSetup +"_68MeV_1_" ; vec_Option.push_back(CompleteNameSetup);


# Output_Ref_Cube_G4_PLEXIGLASS_vMZfv3_0.1_Manip_ZebraFish_110320_v1_68MeV_0.68_1M


# option=("Cube_G4_PLEXIGLASS_vMZfv2_0.1_Manip_ZebraFish_110320_v1_68MeV_0_" "Cube_G4_PLEXIGLASS_vMZfv2_0.1_Manip_ZebraFish_110320_v1_68MeV_0.68_" "Cube_G4_PLEXIGLASS_vMZfv2_0.1_Manip_ZebraFish_110320_v1_68MeV_1_") 


option=("Cube_G4_PLEXIGLASS_vMZfv3_0.1_Manip_ZebraFish_110320_v1_68MeV_0_" "Cube_G4_PLEXIGLASS_vMZfv3_0.1_Manip_ZebraFish_110320_v1_68MeV_0.68_" "Cube_G4_PLEXIGLASS_vMZfv3_0.1_Manip_ZebraFish_110320_v1_68MeV_1_") 

# option=("vMZfv2_Ligne_ICO_v2T_colli_68MeV_0_") 



len3=${#option[@]}  # nombre d'éléments dans le tableau

 #std::vector<string> Category_Of_Analyse {"Epaisseur", "Pos"};
# z=(Colli2 Colli4 Colli4Ext)  # initialisation du tableau

# z=(Colli4Ext)  # initialisation du tableau

len=${#z[@]}  # nombre d'éléments dans le tableau

# std::vector<string> type_Of_Analyse_2 {"Min", "Max"};
z2=("")  # initialisation du tableau
len2=${#z[@]}  # nombre d'éléments dans le tableau


# for ((o=0; o<$len3; o++)); do
# 
# echo ${option[o]}
# 
#     for ((i=0; i<$len; i++)); do
#    
#         echo ${z[i]}
# 
#         for ((j=0; j<$len2; j++)); do
#    
#             echo "${z2[j]}"
#    
#             #string PathFile_Total = PathFile + "output_"+ x + "_" + y + "_" + Coef_Event + Unit_Event + "/";
#    
#             PathFile_Total="${PathFile}output_${z[i]}${z2[j]}_${option[o]}${Coef_Event}${Unit_Event}/"
#             echo ${PathFile_Total}
#         
#             root -l -q "Analyse_CaracterisationDuSetup_Collimator.C(\"${PathFile_Total}\")"
#         
#    
#         done
#    
# 
#     done
# 
# done

for ((o=0; o<$len3; o++)); do

echo ${option[o]}

# /media/sf_PartageVbox8/work/AnalyseDose/Macro_ok/Analyse_CaracterisationDuSetup_Collimator.C

 PathFile_Total="${PathFile}output_Ref_${option[o]}${Coef_Event}${Unit_Event}/"
  root -l -q "Analyse_CaracterisationDuSetup_z_Projet_Zebrafish.C(\"${PathFile_Total}\")" 

 
done

echo "sucess"

