#!/bin/bash


START=$(date +%s) # Pour calculer le temps de calcul

echo Running on host `hostname` # Done le nom du system
HOSTNAME=`hostname`

Profil="Arthur" # Sophie Sophie_Farm

#NameFileToSave
NameMacro=Multi_Gate_Flash.sh
Name_Of_File=runs_Potentiel_Ioni_Water_Density # Mettre le nom du dossier de sauvegarde des données ~ nom du dossier de l'étude

### Mise en place des chemins vers la simulation et le "work"
### Automatique ne pas toucher
SIMULATION_DIR=$SimuDPGA
WORK_PWD=$work
### 

##### Detection du profil Farm - voir avec Sophie
#####
FARM=0
#####

if [ ${HOSTNAME} = "vgate" ]
then

FARM=0
Profil="Arthur"

elif [ ${HOSTNAME} = "hpc2" ]
then
FARM=1
elif [ ${HOSTNAME} = "clrpcsvsrv-bigpu" ]
then
FARM=2
echo "Welcome on BiGPU !"
else
FARM=0
echo "Probably not on Farm but check !!!"
fi

echo "FARM (0 NO/ 1 YES / >1 NO) : ${FARM}"

if [ ${FARM} = "0" ] && [ ${Profil} = "Arthur" ]
then
# create alias you need use with "$" 

# source software you need if it's necessary

#root6='source /home/gate/Progs/root_6.14/bin/thisroot.sh'
#geant42='source /home/gate/Progs/Geant4/geant4.10.02-install/bin/geant4.sh'
#geant43='source /home/gate/Progs/Geant4/geant4.10.3.p02-build/InstallTreeFiles/geant4.sh'

vRoot='6.14.00'
V_GATE="8.2"
PATCH="10.5.0"

SIMULATION_DIR="/media/sf_PartageVbox8/work/Simulation"
WORK_PWD="/media/sf_PartageVbox8/work"

elif [ ${FARM} = "0" ] && [ ${Profil} = "Sophie" ]
then

# source software you need if it's necessary
# source Gate ? 
#source root ?

# version de root ?
vRoot='6.14.00'
source ?/.bashrc
SIMULATION_DIR="?/Simulation"
WORK_PWD="?/work"

V_GATE="8.2"
PATCH="p01"

elif [ ${FARM} = "2" ] && [ ${Profil} = "stagiaire" ]
then

# source software you need if it's necessary
# source Gate ? 
#source root ?

# version de root ?
vRoot='6.14.00'
source ?/.bashrc
SIMULATION_DIR="?/Simulation"
WORK_PWD="?/work"

V_GATE="8.1"
PATCH="p01"

else
echo "FATAL ERROR !!! Check Farm Option !!!"
exit 1
fi


###########################################################################################################################################
###########################################################################################################################################

# Création du .mac

###########################################################################################################################################
###########################################################################################################################################





### Laisser 1 pour le moment à voir si le multithreading apport quelques chose et/ou est possible...
MULTITHREADING=1 # Mettre le nombre de coeurs 

#/control/execute physics_list/GateV8_BIC.mac
#/control/execute physics_list/GateV7.mac

### EMZ opt4 || EMY opt3

Physics=GateV8_BIC_EMZ.mac

## to CHANGE VAR PARAMETER ACTUALLY 
### ONLY Pos_Cuve_Z (ie position de la cuve ) IS available
##    BeamEnergy en MEv !!!
VAR_PARAMETER=BeamEnergy

# if VAR_PARAMETER = Pos_Cuve Varriation par rapport à la position de ref. en mm
Pos_Cuve_MIN=0
Pos_Cuve_MAX=100
PAS_CUVE=10


# if VAR_PARAMETER = BeamEnergy Varriationde l'énergie initale des protons en MeV
ENERGY_MIN=5
ENERGY_MAX=70
PAS_ENERGY=5

# if VAR_PARAMETER = BeamEnergy Varriationde l'énergie initale des protons en MeV
Beam_ene_sigma_MIN=0
Beam_ene_sigma_MAX=1
PAS_Beam_ene_sigma=0.1




####
# setTotalNumberOfPrimaries
###

Coef_EVENT_NB=100
EVENT_NB_Unit=k

if [ ${EVENT_NB_Unit} = "M" ]
then
EVENT_NB=1000000
elif [ ${EVENT_NB_Unit} = "k" ]
then
EVENT_NB=1000
else
echo "Event Number Default 1"
EVENT_NB=1
fi
let "EVENT_NB=($EVENT_NB * $Coef_EVENT_NB)"



########################### System OPTION(s) #########################

### Kapton_Collimateur.mac Kapton.mac Kapton_Collimateur_Tungsten.mac Naked.mac
System=Naked.mac


# /control/execute geometry/Kapton_Collimateur_Tungsten.mac
#/control/execute geometry/Kapton_Collimateur.mac
#/control/execute geometry/Kapton.mac


####################################################


########################### Target (Cuve) OPTION(s) #########################

Target_Material=RibBone_d_1_00

IonisationPotential=75


SetCutCuve=0.1

#/gate/geometry/setIonisationPotential Water 78 eV



Target_Lenght_Unit=cm 
Target_XLength=10.
Target_YLength=10.
Target_ZLength=10.


#
Target_Placement_Ref_Z=50
# Target_Placement_Ref_Z=105.075000 

Target_Placement_Ref_X=0.

Target_Placement_Ref_Y=0.

Target_Placement_Ref_Unit="mm"

Target_Placement_Ref="${Target_Placement_Ref_X} ${Target_Placement_Ref_Y} ${Target_Placement_Ref_Z} ${Target_Placement_Ref_Unit}"


####################################################


########################### Frite OPTION(s) #########################

Frite_Material=${Target_Material}

Frite_Lenght_Unit=cm 
Frite_XLength=10.
Frite_YLength=10.
Frite_ZLength=5.

#
Frite_Placement="0 0 -2.5 cm"

#Frite_Placement="0. 0. 0. cm"
####################################################


########################### SOURCE (beam) OPTION(s) #########################

#type

#/gate/source/addSource pbeam gps #parametre fixe
Beam_Particle_Source=proton

#forme
Beam_Type=Beam
Beam_shape=Circle

Beam_sigma_Unit="mm"
Beam_sigma_x="0."
Beam_sigma_y="0."

### 2 3

#localisation

Beam_centre="0 0 0 cm"
Beam_direction="0 0 1"

#energie

Beam_ene_mono="68 MeV"
Beam_ene_type="Gauss"
Beam_ene_sigma="0 MeV" 
## 0.68 MeV

####################################################


########################### Actor OPTION(s) #########################

#DOSE ACTOR

Actor_1_Name=dose
Actor_1_Save=output/dose-proton.txt

Actor_1_attachTo=frite

Actor_1_setResolution="1 1 500" ### /!\ 1e8 bin = 8.5 Go of RAM || 1e9 -> > 20 Go
Actor_1_setsaveEveryNSeconds="60" # 
Actor_1_stepHitType="random"

Actor_1_enableEdep="true"
Actor_1_enableUncertaintyEdep="true"

Actor_1_enableDose="true"
Actor_1_enableUncertaintyDose="true"

Actor_1_enableDoseToWater="true"
Actor_1_enableUncertaintyDoseToWater="true"

Actor_1_enableNumberOfHits="true"

## Attention Etre sur qu'on veut ce paramètre  /****//////////////////////////
Actor_1_normaliseDoseToIntegral="false"

#/gate/actor/dose/normaliseDoseToMax     true


####################################################

#DISTANCE_BEAM_TO_CENTEROFTARGET=$(echo "sqrt((${POSITION_TARGET_Z}-(${POSITION_BEAM_Z}))^2)"  |bc -l )

ENERGY_NB=-1 
echo "********** Verif Parameter ****************"
echo " "
echo "*** ${Name_Of_File} ***"
echo " "
echo "VAR_PARAMETER	= ${VAR_PARAMETER}"
echo " "
#let "Pos_Cuve_NB=($Pos_Cuve_MAX - $Pos_Cuve_MIN)/$PAS_CUVE +1"
echo " "
echo "Pos_Cuve_MIN [mm]	= ${Pos_Cuve_MIN}"
echo "Pos_Cuve_MAX [mm]	= ${Pos_Cuve_MAX}"
echo "PAS_Pos_Cuve [mm]	= ${PAS_CUVE}"
echo "Nbr of Testing Pos_Cuve	= ${PAS_CUVE}"

echo " "
echo "ENERGY_MIN [mm]	= ${ENERGY_MIN}"
echo "ENERGY_MAX [mm]	= ${ENERGY_MAX}"
echo "PAS_ENERGY     	= ${PAS_ENERGY}"


#echo "Event Per Pas Pos_Cuve	= ${Pos_Cuve_NB}"
echo " "
echo "***Target***"
echo " "
echo "MATERIAL	= ${Target_Material}"
echo "IonisationPotential = ${IonisationPotential}"
echo "Target_Lenght	= ${Target_XLength} ${Target_YLength} ${Target_ZLength} ${Target_Lenght_Unit}"
echo " "
echo "***Frite***"
echo "MATERIAL	= ${Frite_Material}"
echo "Target_Lenght	= ${Frite_XLength} ${Frite_YLength} ${Frite_ZLength} ${Frite_Lenght_Unit}"
echo " "
echo "***Setup***"
echo " "
echo "Beam_Particle_Source	= ${Beam_Particle_Source}"
echo "Physics List	= ${Physics}"
echo "System	= ${System}"
echo "EVENT_NB	= ${EVENT_NB}"
echo " "
echo "Beam_sigma	= ${Beam_sigma_x} ${Beam_sigma_y} ${Beam_sigma_Unit}"
echo "Beam_ene_mono	= ${Beam_ene_mono} ${Beam_ene_type} ${Beam_ene_sigma}"
echo " "
echo "***Actor_1***"
echo " "
echo "SaveFile	= ${Actor_1_Save}"
#echo "attachTo	= ${Actor_1_frite}"
echo " "
echo "setResolution	= ${Actor_1_setResolution}"
echo "setsaveEveryNSeconds	= ${Actor_1_setsaveEveryNSeconds}"
echo "stepHitType	= ${Actor_1_stepHitType}"
echo " "
echo "enableEdep Uncertain	= ${Actor_1_enableEdep} ${Actor_1_enableUncertaintyEdep}"
echo "enableDose Uncertain	= ${Actor_1_enableDose} ${Actor_1_enableUncertaintyDose}"
echo "enableDoseToWater Uncertain	= ${Actor_1_enableDoseToWater} ${Actor_1_enableUncertaintyDoseToWater}"
echo " "
echo "enableNumberOfHits	= ${Actor_1_enableNumberOfHits}"
echo "normaliseDoseToIntegral	= ${Actor_1_normaliseDoseToIntegral}"
echo " "

echo "Are the settings correct? Text 1 / 2"

select i in yes no; do
        if [ "$i" = "yes" ]; then
                echo "Perfect !"
                break
        elif [ "$i" = "no" ]; then
                echo "exit 0"
                exit 0
                break
        else
                echo "Ni yes ni no => no exit 0"
                exit 0
        fi
done

############# COMMENTS #################

#Comment="Proteus 180MeV PMMA [REF] option 2"
Comment="Variation_${VAR_PARAMETER}"
############# COMMENTS #################


# TODO 
# Mettre ici les constantes || Donc pas energy utiliser "sed" pour ça !!!!
# Ne pas oublier de les ajouters dans le nom du fichier.mac et dans les echo()
# TODO




SETUP_COMMENT_NAME="Physics VAR_PARAMETER EVENT_NB System Target_Material Target_Lenght_Unit Target_XLength Target_YLength Target_ZLength Target_Placement_Ref_X Target_Placement_Ref_Y Target_Placement_Ref_Z Target_Placement_Ref_Unit Frite_Material Frite_Lenght_Unit Frite_XLength Frite_YLength Frite_ZLength Frite_Placement_X Frite_Placement_Y Frite_Placement_Z Frite_Placement_Unit Beam_Particle_Source Beam_Type Beam_shape Beam_sigma_Unit Beam_sigma_x Beam_sigma_y Beam_centre_X Beam_centre_Y Beam_centre_Z Beam_centre_Unit Beam_direction_X Beam_direction_Y Beam_direction_Z Beam_ene_mono Beam_ene_mono_Unit Beam_ene_type Beam_ene_sigma Beam_ene_sigma_Unit Actor_1_Name Actor_1_Save Actor_1_attachTo Actor_1_setResolution_X Actor_1_setResolution_Y Actor_1_setResolution_Z Actor_1_setsaveEveryNSeconds Actor_1_stepHitType Actor_1_enableEdep Actor_1_enableUncertaintyEdep Actor_1_enableDose Actor_1_enableUncertaintyDose Actor_1_enableDoseToWater Actor_1_enableUncertaintyDoseToWater Actor_1_enableNumberOfHits Actor_1_normaliseDoseToIntegral IonisationPotential SetCutCuve"

SETUP_COMMENT="${Physics} ${VAR_PARAMETER} ${EVENT_NB} ${System} ${Target_Material} ${Target_Lenght_Unit} ${Target_XLength} ${Target_YLength} ${Target_ZLength} ${Target_Placement_Ref_X} ${Target_Placement_Ref_Y} ${Target_Placement_Ref_Z} ${Target_Placement_Ref_Unit} ${Frite_Material} ${Frite_Lenght_Unit} ${Frite_XLength} ${Frite_YLength} ${Frite_ZLength} ${Frite_Placement} ${Beam_Particle_Source} ${Beam_Type} ${Beam_shape} ${Beam_sigma_Unit} ${Beam_sigma_x} ${Beam_sigma_y} ${Beam_centre} ${Beam_direction} ${Beam_ene_mono} ${Beam_ene_type} ${Beam_ene_sigma} ${Actor_1_Name} ${Actor_1_Save} ${Actor_1_attachTo} ${Actor_1_setResolution} ${Actor_1_setsaveEveryNSeconds} ${Actor_1_stepHitType} ${Actor_1_enableEdep} ${Actor_1_enableUncertaintyEdep} ${Actor_1_enableDose} ${Actor_1_enableUncertaintyDose} ${Actor_1_enableDoseToWater} ${Actor_1_enableUncertaintyDoseToWater} ${Actor_1_enableNumberOfHits} ${Actor_1_normaliseDoseToIntegral} ${IonisationPotential} ${SetCutCuve}"


#### Creation des Runs !

mkdir ${Name_Of_File}
### Make new environement ie MasterRuns
RUN_ID=`ls ${Name_Of_File} | sed 's/[^0-9]//g' | sort -rn | sed q`
echo "Last run ID = "${RUN_ID}
let "NEXT_RUN_ID=${RUN_ID}+1"
echo "Next run ID = "${NEXT_RUN_ID}
WORK_DIR="${WORK_PWD}/${Name_Of_File}/run_${NEXT_RUN_ID}"
echo ${WORK_DIR}
mkdir ${WORK_DIR}

echo "Simulation DIR ${SIMULATION_DIR}"
BATCH_DIR=""
echo "Batch DIR ${BATCH_DIR}"





### Fast time ####
### Build Geant4_DPGA
### Open execution directory
cd ${WORK_DIR}
echo "Working directory = "`pwd`
### Source GEANT4 and crea./${PROD_WORK_DIR}/Build/main_DPGAte build

NAMEBUILD=BuildGate
# rm -r ${SIMULATION_DIR}/../${NAMEBUILD}


a=1
b=2
if (( ${MULTITHREADING} > ${a} ))
then
NAMEBUILD=BuildDPGA_launcher_Mt
# rm -r ${SIMULATION_DIR}/../${NAMEBUILD}

else
echo "No MT"
fi

# Changement des physiques list directement dans le .CC

cp -r ${SIMULATION_DIR} ./
cd Simulation
SIMULATION_DIR=`pwd`
echo "SIMULATION_DIR = "`pwd`



cd ${WORK_DIR}

# Save .sh
cp -r ${WORK_DIR}/../../${NameMacro} ${WORK_DIR}/.

echo "Path run_launcher ${WORK_DIR}/${NAMEBUILD}"
#ls -lsah ${WORK_DIR}/${NAMEBUILD}
#ls -lsah ${WORK_DIR}/${NAMEBUILD}


###########################################################################################################################################
###########################################################################################################################################

# Création du .mac

###########################################################################################################################################
###########################################################################################################################################
# on fait le ménage !
rm ${WORK_DIR}/run_launcher.mac

/bin/cat <<EOM > ${WORK_DIR}/run_launcher.mac
   
# $Id: run_Gate.mac 2019 abongrand $
#
# Macro file of "main_DPGA.cc"
#

# Arthur -> 16/12/19 | Origine -> Sophie

#=====================================================
# VERBOSITY
#=====================================================
/control/execute mac/verbose.mac

#=====================================================
# VISUALISATION
#=====================================================
#/control/execute mac/visu.mac

#=====================================================
#Geometry Material
#=====================================================

# warning custom GateMaterials !
/gate/geometry/setMaterialDatabase data/GateMaterials3.db
/gate/geometry/setIonisationPotential ${Target_Material} ${IonisationPotential} eV

#=====================================================
#World
#=====================================================
/gate/world/geometry/setXLength 0.5 m
/gate/world/geometry/setYLength 0.5 m
/gate/world/geometry/setZLength 0.5 m
/gate/world/setMaterial Air

#=====================================================
#System
#=====================================================

/control/execute geometry/${System}
#/control/execute geometry/Kapton_Collimateur.mac


#===========================================
# CIBLE 
#===========================================

/gate/world/daughters/name cuve
/gate/world/daughters/insert box

/gate/cuve/setMaterial ${Target_Material}

/gate/cuve/geometry/setXLength ${Target_XLength} ${Target_Lenght_Unit}
/gate/cuve/geometry/setYLength ${Target_YLength} ${Target_Lenght_Unit}
/gate/cuve/geometry/setZLength ${Target_ZLength} ${Target_Lenght_Unit}

/gate/cuve/vis/setColor blue
/gate/cuve/vis/forceWireframe


#TRANSLATION à partir du centre du volume
/gate/cuve/placement/setTranslation ${Target_Placement_Ref}

/gate/cuve/daughters/name frite
/gate/cuve/daughters/insert box
/gate/frite/setMaterial ${Frite_Material}
/gate/frite/geometry/setXLength ${Frite_XLength} ${Frite_Lenght_Unit}
/gate/frite/geometry/setYLength ${Frite_YLength} ${Frite_Lenght_Unit}
/gate/frite/geometry/setZLength ${Frite_ZLength} ${Frite_Lenght_Unit}
/gate/frite/placement/setTranslation ${Frite_Placement}
/gate/frite/vis/setColor magenta
/gate/frite/vis/forceSolid

#====================================
#Physics GATE
#====================================

/control/execute physics_list/${Physics}
#/control/execute physics_list/GateV7.mac


#====================================

#cut world
/gate/physics/Gamma/SetCutInRegion world 1 mm
/gate/physics/Electron/SetCutInRegion world 1 mm
/gate/physics/Proton/SetCutInRegion world 1 mm
/gate/physics/Positron/SetCutInRegion world 1 mm

/gate/physics/Gamma/SetCutInRegion cuve ${SetCutCuve} mm
/gate/physics/Electron/SetCutInRegion cuve ${SetCutCuve} mm
/gate/physics/Proton/SetCutInRegion cuve ${SetCutCuve} mm
/gate/physics/Positron/SetCutInRegion cuve ${SetCutCuve} mm

/gate/physics/displayCuts

#steplimiter
#/gate/physics/SetMaxStepSizeInRegion cuve 0.1 mm

/gate/physics/setEMin 0.1 keV
/gate/physics/setEMax 10 GeV

#/gate/physics/setDEDXBinning 350
#/gate/physics/setLambdaBinning 350


#====================================
#Actor
#====================================


#DOSE ACTOR
/gate/actor/addActor DoseActor ${Actor_1_Name}
/gate/actor/dose/save ${Actor_1_Save}
/gate/actor/dose/attachTo ${Actor_1_attachTo}

/gate/actor/dose/setResolution ${Actor_1_setResolution}
/gate/actor/dose/saveEveryNSeconds ${Actor_1_setsaveEveryNSeconds}
/gate/actor/dose/stepHitType ${Actor_1_stepHitType}

#/gate/actor/dose/enableEdep ${Actor_1_enableEdep}
#/gate/actor/dose/enableUncertaintyEdep ${Actor_1_enableUncertaintyEdep}

/gate/actor/dose/enableDose ${Actor_1_enableDose}
/gate/actor/dose/enableUncertaintyDose ${Actor_1_enableUncertaintyDose}

/gate/actor/dose/enableDoseToWater ${Actor_1_enableDoseToWater}
/gate/actor/dose/enableUncertaintyDoseToWater ${Actor_1_enableUncertaintyDoseToWater}

#/gate/actor/dose/normaliseDoseToWater    

#/gate/actor/dose/normaliseDoseToMax    
/gate/actor/dose/normaliseDoseToIntegral   ${Actor_1_normaliseDoseToIntegral}


/gate/actor/dose/enableNumberOfHits ${Actor_1_enableNumberOfHits}

#====================================
#Initialization
#====================================

/gate/run/initialize

#====================================
#Source
#====================================

#type
/gate/source/addSource pbeam gps
/gate/source/pbeam/gps/particle proton


#forme
/gate/source/pbeam/gps/type Beam
/gate/source/pbeam/gps/shape Circle

/gate/source/pbeam/gps/pos/sigma_x ${Beam_sigma_x} ${Beam_sigma_Unit}
/gate/source/pbeam/gps/pos/sigma_y ${Beam_sigma_y} ${Beam_sigma_Unit}

#localisation
/gate/source/pbeam/gps/centre ${Beam_centre}
/gate/source/pbeam/gps/direction ${Beam_direction}

#energie
/gate/source/pbeam/gps/ene/mono ${Beam_ene_mono}
/gate/source/pbeam/gps/ene/type ${Beam_ene_type}
/gate/source/pbeam/gps/ene/sigma ${Beam_ene_sigma}

#/gate/source/pbeam/gps/setTestFlag true

#====================================
#Initialization || pas necessaire pour la source 
#====================================

#/gate/run/initialize

#graine 
/gate/random/setEngineName MersenneTwister
/gate/random/setEngineSeed auto

#nb particule
/gate/application/setTotalNumberOfPrimaries ${EVENT_NB}
/gate/application/start

EOM


###########################################################################################################################################
###########################################################################################################################################


#ls -lsah ${WORK_DIR}/${NAMEBUILD}

# mkdir ${WORK_DIR}/G4_log

numbersetup_i=0

if [ ${VAR_PARAMETER} = "Pos_Cuve_Z" ]
then
 ### Loop over setup nb
for((pos_cuve_i=${Pos_Cuve_MIN} ; pos_cuve_i<${Pos_Cuve_MAX} ; pos_cuve_i+=${PAS_CUVE}))
do
  echo "  "
  echo "${VAR_PARAMETER}_"${pos_cuve_i}
  echo "  "
  
  ### TODO insert other parameter variable in a tank here
  
  
  mkdir ${WORK_DIR}/Gate_log
  
  let "numbersetup_i=($numbersetup_i+1)"
  mkdir ${WORK_DIR}/Setup_${numbersetup_i}
   
   # copie run launcher.mac
   cp -r ${WORK_DIR}/run_launcher.mac ${WORK_DIR}/Setup_${numbersetup_i}/.
   
   # copie simulation
   cp -r ${SIMULATION_DIR}/* ${WORK_DIR}/Setup_${numbersetup_i}/.
   ### Make batsh_script.sh modifications
  
   ## Constantes
   
   #echo "TargetPlacment_var_Z= $Target_Placement_Ref_Z + $pos_cuve_i" | bc
   
  # Computedval=$(bc <<< "scale=10; $val1-$val2")
   
   TargetPlacment_var_Z=$(bc <<< "scale=10; $Target_Placement_Ref_Z+$pos_cuve_i")
   
   #echo `expr $Target_Placement_Ref_Z + $pos_cuve_i`   
   
   #echo $[Target_Placement_Ref_Z + pos_cuve_i]
   
    echo "TargetPlacment_var_Z "${TargetPlacment_var_Z}
   
   
   ### TODO remplacer la position faire le calcul
   sed -i "s/${Target_Placement_Ref_Z}/${TargetPlacment_var_Z}/g"      ${WORK_DIR}/Setup_${numbersetup_i}/run_launcher.mac

   cd ${WORK_DIR}/Setup_${numbersetup_i}
   
   ### Gate -q 
   
   Gate run_launcher.mac > ../Gate_log/Setup_${numbersetup_i}.log
   
   
   ### Organisation des données produites
   
   
   mkdir ${WORK_DIR}/output
   mkdir ${WORK_DIR}/output/Setup_${numbersetup_i}
   
   
   rm -r output/Test_data
   mv output/* ${WORK_DIR}/output/Setup_${numbersetup_i}/
   mv run_launcher.mac ${WORK_DIR}/output/Setup_${numbersetup_i}/.
   rm -r ../Setup_${numbersetup_i}
      # ./main_DPGA run_Medicyc.mac >>${WORK_DIR}/G4_log/G4_${pos_cuve_i}_SETUP_${numbersetup_i}.txt

   
   
   cd ${WORK_DIR}
   
   
   END=$(date +%s)
   DIFF=$(( $END - $START ))
   
   

   
   
  if [ ${numbersetup_i} = "1" ]
  then
    (
    echo "## VAR_PARAMETER NUMBERSETUP NAMEROOTFILE Pos_cuve_Z Pos_Cuve_Real_Z Pos_Cuve_MIN Pos_Cuve_MAX PAS_CUVE EVENT_NB ${SETUP_COMMENT_NAME} vRoot Time[s]"
    ) > ${WORK_DIR}/Setup.txt
  fi
    
    (
    echo "# ${VAR_PARAMETER} ${numbersetup_i} Setup_${numbersetup_i} ${pos_cuve_i} ${TargetPlacment_var_Z} ${Pos_Cuve_MIN} ${Pos_Cuve_MAX} ${PAS_CUVE} ${EVENT_NB} ${SETUP_COMMENT} ${vRoot} ${DIFF}"
    ) >> ${WORK_DIR}/Setup.txt

    
  done # setup loop

  (
  echo "	"
  echo \#d `date`
  echo \#c ${Comment}
  echo "## RUN_ID Energy_Min[MeV] Energy_Max[MeV] PAS_ENERGY[MeV] EVENT_NB ${SETUP_COMMENT_NAME} vRoot Time[s]"
  echo ${NEXT_RUN_ID} ${ENERGY_MIN} ${ENERGY_MAX} ${PAS_ENERGY} ${EVENT_NB} ${SETUP_COMMENT} ${vRoot} ${DIFF}
  ) >> ${WORK_DIR}/../Setup.txt
  
  echo ${DIFF}
  
elif [ ${VAR_PARAMETER} = "BeamEnergy" ] 
then

 let "ENERGY_MAX_FOR=(${ENERGY_MAX} +1)"

 ### Loop over setup nb
for((energy_beam_i=${ENERGY_MIN} ; energy_beam_i<${ENERGY_MAX_FOR} ; energy_beam_i+=${PAS_ENERGY}))
do
  echo "  "
  echo "${VAR_PARAMETER}_"${energy_beam_i}
  echo "  "
  
  ### TODO insert other parameter variable in a tank here
  
  
  mkdir ${WORK_DIR}/Gate_log
  
  let "numbersetup_i=($numbersetup_i+1)"
  mkdir ${WORK_DIR}/Setup_${numbersetup_i}
   
   # copie run launcher.mac
   cp -r ${WORK_DIR}/run_launcher.mac ${WORK_DIR}/Setup_${numbersetup_i}/.
   
   # copie simulation
   cp -r ${SIMULATION_DIR}/* ${WORK_DIR}/Setup_${numbersetup_i}/.
   ### Make batsh_script.sh modifications
  
   ## Constantes
   
   #echo "TargetPlacment_var_Z= $Target_Placement_Ref_Z + $energy_beam_i" | bc
   
  # Computedval=$(bc <<< "scale=10; $val1-$val2")
   
   TargetPlacment_var_Z=$(bc <<< "scale=10; $Target_Placement_Ref_Z")
   
   #echo `expr $Target_Placement_Ref_Z + $energy_beam_i`   
   
   #echo $[Target_Placement_Ref_Z + energy_beam_i]
   
    echo "TargetPlacment_var_Z "${TargetPlacment_var_Z}
   
   
   ### TODO remplacer la position faire le calcul
   # sed -i "s/${Target_Placement_Ref_Z}/${TargetPlacment_var_Z}/g"      ${WORK_DIR}/Setup_${numbersetup_i}/run_launcher.mac

   cd ${WORK_DIR}/Setup_${numbersetup_i}
   
   ### Gate -q 
   
   Energie_Var="\/gate\/source\/pbeam\/gps\/ene\/mono\ ${energy_beam_i}\ MeV"
   
   Energie_Ref="\/gate\/source\/pbeam\/gps\/ene\/mono ${Beam_ene_mono}"
   
   sed -i "s/${Energie_Ref}/${Energie_Var}/g"      ${WORK_DIR}/Setup_${numbersetup_i}/run_launcher.mac
   
   
   Gate run_launcher.mac > ../Gate_log/Setup_${numbersetup_i}.log
   
   
   ### Organisation des données produites
   
   
   mkdir ${WORK_DIR}/output
   mkdir ${WORK_DIR}/output/Setup_${numbersetup_i}
   
   
   rm -r output/Test_data
   mv output/* ${WORK_DIR}/output/Setup_${numbersetup_i}/
   mv run_launcher.mac ${WORK_DIR}/output/Setup_${numbersetup_i}/.
   rm -r ../Setup_${numbersetup_i}
      # ./main_DPGA run_Medicyc.mac >>${WORK_DIR}/G4_log/G4_${pos_cuve_i}_SETUP_${numbersetup_i}.txt

   cd ${WORK_DIR}
   
   
   END=$(date +%s)
   DIFF=$(( $END - $START ))
   
   
  if [ ${numbersetup_i} = "1" ]
  then
    (
    echo "## VAR_PARAMETER NUMBERSETUP NAMEROOTFILE ENERGY Pos_Cuve_Real_Z Energy_Min[MeV] Energy_Max[MeV] PAS_ENERGY[MeV] EVENT_NB ${SETUP_COMMENT_NAME} vRoot Time[s]"
    ) > ${WORK_DIR}/Setup.txt
  fi
    
    (
    echo "# ${VAR_PARAMETER} ${numbersetup_i} Setup_${numbersetup_i} ${energy_beam_i} ${TargetPlacment_var_Z} ${ENERGY_MIN} ${ENERGY_MAX} ${PAS_ENERGY} ${EVENT_NB} ${SETUP_COMMENT} ${vRoot} ${DIFF}"
    ) >> ${WORK_DIR}/Setup.txt

    
  done # setup loop

  (
  echo "	"
  echo \#d `date`
  echo \#c ${Comment}
  echo "## RUN_ID Energy_Min[MeV] Energy_Max[MeV] PAS_ENERGY[MeV] EVENT_NB ${SETUP_COMMENT_NAME} vRoot Time[s]"
  echo ${NEXT_RUN_ID} ${ENERGY_MIN} ${ENERGY_MAX} ${PAS_ENERGY} ${EVENT_NB} ${SETUP_COMMENT} ${vRoot} ${DIFF}
  ) >> ${WORK_DIR}/../Setup.txt
  
  echo ${DIFF}

elif [ ${VAR_PARAMETER} = "Beam_ene_sigma" ] 
then


 #Beam_ene_sigma_MAX_FOR=$(echo "${Beam_ene_sigma_MAX}+0.0001)"  |bc -l )
 
 
 
 ### Loop over setup nb
for((energy_beam_i=0 ; energy_beam_i<10 ; energy_beam_i+=1))
do
  echo "  "
  echo "${VAR_PARAMETER}_"${energy_beam_i}
  echo "  "
  
  ### TODO insert other parameter variable in a tank here
  
  
  mkdir ${WORK_DIR}/Gate_log
  
  let "numbersetup_i=($numbersetup_i+1)"
  mkdir ${WORK_DIR}/Setup_${numbersetup_i}
   
   # copie run launcher.mac
   cp -r ${WORK_DIR}/run_launcher.mac ${WORK_DIR}/Setup_${numbersetup_i}/.
   
   # copie simulation
   cp -r ${SIMULATION_DIR}/* ${WORK_DIR}/Setup_${numbersetup_i}/.
   ### Make batsh_script.sh modifications
  
   ## Constantes
   
   #echo "TargetPlacment_var_Z= $Target_Placement_Ref_Z + $energy_beam_i" | bc
   
  # Computedval=$(bc <<< "scale=10; $val1-$val2")
   
   TargetPlacment_var_Z=$(bc <<< "scale=10; $Target_Placement_Ref_Z")
   
   #echo `expr $Target_Placement_Ref_Z + $energy_beam_i`   
   
   #echo $[Target_Placement_Ref_Z + energy_beam_i]
   
    echo "TargetPlacment_var_Z "${TargetPlacment_var_Z}
   
   
   ### TODO remplacer la position faire le calcul
   # sed -i "s/${Target_Placement_Ref_Z}/${TargetPlacment_var_Z}/g"      ${WORK_DIR}/Setup_${numbersetup_i}/run_launcher.mac

   cd ${WORK_DIR}/Setup_${numbersetup_i}
   
   ### Gate -q 
   
   Energie_Var="\/gate\/source\/pbeam\/gps\/ene\/sigma\ ${energy_beam_i}\ MeV"
   
   Energie_Ref="\/gate\/source\/pbeam\/gps\/ene\/sigma ${Beam_ene_sigma}"
   
   sed -i "s/${Energie_Ref}/${Energie_Var}/g"      ${WORK_DIR}/Setup_${numbersetup_i}/run_launcher.mac
   
   
   Gate run_launcher.mac > ../Gate_log/Setup_${numbersetup_i}.log
   
   
   ### Organisation des données produites
   
   
   mkdir ${WORK_DIR}/output
   mkdir ${WORK_DIR}/output/Setup_${numbersetup_i}
   
   
   rm -r output/Test_data
   mv output/* ${WORK_DIR}/output/Setup_${numbersetup_i}/
   mv run_launcher.mac ${WORK_DIR}/output/Setup_${numbersetup_i}/.
   rm -r ../Setup_${numbersetup_i}
      # ./main_DPGA run_Medicyc.mac >>${WORK_DIR}/G4_log/G4_${pos_cuve_i}_SETUP_${numbersetup_i}.txt

   cd ${WORK_DIR}
   
   
   END=$(date +%s)
   DIFF=$(( $END - $START ))
   
   
  if [ ${numbersetup_i} = "1" ]
  then
    (
    echo "## VAR_PARAMETER NUMBERSETUP NAMEROOTFILE Beam_ene_sigma_VAR Pos_Cuve_Real_Z Beam_ene_sigma_Min[MeV] Beam_ene_sigma_Max[MeV] PAS_Beam_ene_sigma[MeV] EVENT_NB ${SETUP_COMMENT_NAME} vRoot Time[s]"
    ) > ${WORK_DIR}/Setup.txt
  fi
    
    (
    echo "# ${VAR_PARAMETER} ${numbersetup_i} Setup_${numbersetup_i} ${energy_beam_i} ${TargetPlacment_var_Z} ${Beam_ene_sigma_MIN} ${Beam_ene_sigma_MAX} ${PAS_Beam_ene_sigma} ${EVENT_NB} ${SETUP_COMMENT} ${vRoot} ${DIFF}"
    ) >> ${WORK_DIR}/Setup.txt

    
  done # setup loop

  (
  echo "	"
  echo \#d `date`
  echo \#c ${Comment}
  echo "## RUN_ID Energy_Min[MeV] Energy_Max[MeV] PAS_Beam_ene_sigma[MeV] EVENT_NB ${SETUP_COMMENT_NAME} vRoot Time[s]"
  echo ${NEXT_RUN_ID} ${Beam_ene_sigma_MIN} ${Beam_ene_sigma_MAX} ${PAS_Beam_ene_sigma} ${EVENT_NB} ${SETUP_COMMENT} ${vRoot} ${DIFF}
  ) >> ${WORK_DIR}/../Setup.txt
  
  echo ${DIFF}


else
# TODO
#
#
echo "${VAR_PARAMETER} NOT IMPLEMENTED YET"
#
#
# TODO study Bore
fi






  
# rm -r ${WORK_DIR}/${NAMEBUILD}
# rm -r ${WORK_DIR}/DPGA

#rm -r ${WORK_DIR}/BuildDPGA_launcher_Mt
# rm -r ${WORK_DIR}/B3_Scoring.root



time ls -lsah

# WARNING CMD FOR PROCESSBRAGGPEAK NOT USE IF YOU LAUNCH MANY JOB
# cd $work
# root -l -q 'Process_Bragg_Peak.C()' > $work/Txt_Process_Bragg_Peak.txt

cd  ${BATCH_DIR}
#rm -r ${WORK_DIR}/${NAMEBUILD}
