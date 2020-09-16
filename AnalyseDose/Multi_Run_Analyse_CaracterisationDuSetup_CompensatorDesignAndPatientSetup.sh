#!/bin/bash


START=$(date +%s)

echo Running on host `hostname`
HOSTNAME=`hostname`


time ls -lsah

PathFile="/media/sf_PartageVbox8/GATE/All_Simulation/z_Projet_Arronax_Compensator_design/"

Coef_Event="10"
Unit_Event="M" 
  
 
 




#https://www.developpez.net/forums/d1528120/general-developpement/langages-programmation/vecteur-boucle-shell/


 #std::vector<string> Category_Of_Analyse {"Epaisseur", "Pos"};
#option=("68MeV_0_" "68MeV_1_" "68MeV_0.68_" "v1_68MeV_0.68_")  # initialisation du tableau

option=("v1_68MeV_0.68_") 
len3=${#option[@]}  # nombre d'éléments dans le tableau


 #std::vector<string> Category_Of_Analyse {"Epaisseur", "Pos"};
z=(Epaisseur Pos)  # initialisation du tableau
len=${#z[@]}  # nombre d'éléments dans le tableau

# std::vector<string> type_Of_Analyse_2 {"Min", "Max"};
z2=(Min Max)  # initialisation du tableau
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
#             PathFile_Total="${PathFile}output_${z[i]}_${z2[j]}_${option[o]}${Coef_Event}${Unit_Event}/"
#             echo ${PathFile_Total}
#         
#             root -l -q "Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C(\"${PathFile_Total}\")"
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

 PathFile_Total="${PathFile}output_Ref_${option[o]}${Coef_Event}${Unit_Event}/"
 root -l -q "Analyse_CaracterisationDuSetup_CompensatorDesignAndPatientSetup.C(\"${PathFile_Total}\")"

done

echo "sucess"

