#!/bin/bash

# Layout HPC2

#SBATCH --job-name=65
#SBATCH --ntasks=1
#SbATCH --ntasks-per-core=1
#SBATCH --cpus-per-task=2
#SbATCH --cpu-freq high[-high[:High]]
#SBATCH --output=/home/arbongra/work/Batch_log/BraggPeak-%j.stdout
#SBATCH --error=/home/arbongra/work/Batch_log/BraggPeak-%j.sdterr
#SBATCH --begin=now
#SBATCH --partition=long
#SBATCH --mail-user=arthur.bongrand@uca.fr
#SBATCH --mail-type=ALL

#SBATCH --mem=10g

START=$(date +%s)

echo Running on host `hostname`
HOSTNAME=`hostname`

Profil="Arthur" # Geoffrey

#NameFileToSave
NameMacro=Multi_DPGA_Medicyc.sh
Name_Of_File=runs_DPGA_Medycic_v2 # runs_DPGA_Medycic runs_Equation hpc2_runs_DPGA_Medycic

JOB_NB=1
Partition=normal

SIMULATION_DIR=$SimuDPGA
WORK_PWD=$work

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
FARM=1
echo "Probably on node but check Path !"
fi

echo "FARM (0 NO/ 1 YES / >1 NO) : ${FARM}"

if [ ${FARM} = "1" ] && [ ${Profil} = "Arthur" ]
then
shopt -s expand_aliases
source /home/arbongra/.bashrc

module load gate clhep root geant4 gate cmake/3.6.1 gcc/4.8.4

SIMULATION_DIR=/home/arbongra/geant4_DPGA/DPGA
WORK_PWD=/home/arbongra/work

vRoot='?'
elif [ ${FARM} = "0" ] && [ ${Profil} = "Arthur" ]
then
# create alias you need use with "$" 
root6='source /home/gate/Progs/root_6.14/bin/thisroot.sh'
geant42='source /home/gate/Progs/Geant4/geant4.10.02-install/bin/geant4.sh'
geant43='source /home/gate/Progs/Geant4/geant4.10.3.p02-build/InstallTreeFiles/geant4.sh'

vRoot='6.14.00'

SIMULATION_DIR=$SimuDPGA
WORK_PWD=$work

elif [ ${FARM} = "2" ] && [ ${Profil} = "Arthur" ]
then

source /newhome/bongrand/.bashrc
SIMULATION_DIR="/newhome/bongrand/geant4_DPGA/DPGA"
WORK_PWD="/newhome/bongrand/work"
elif [ ${FARM} = "2" ] && [ ${Profil} = "John_Doe" ]
then

source /home/bernard/.bashrc
SIMULATION_DIR="/home/bernard/NEW/AnalyseDataROOT6-master/work/DPGA"
WORK_PWD="/home/bernard/NEW/AnalyseDataROOT6-master/work"

else
echo "Check Farm Option"
fi

############# COMMENTS #################

#Comment="Proteus 180MeV PMMA [REF] option 2"
Comment="Medicyc 65 MeV PMMA [REF] 2 thread option 2"

############# COMMENTS #################

# 10.4
V_GEANT4="10.3"
PATCH="p02"

MULTITHREADING=2 # Mettre le nombre de coeurs

# G4_WATER PMMA G4_BRAIN_ICRP G4_BONE_COMPACT_ICRU

MATERIAL="PMMA"

############# MATERIALS #################
# "PMMA" C5H8O2                         #
# "BORE" (NIST Ref)                     #
# "BORE_PMMA" (50%,50%)                 #
# "Bore_Nat" (19.9%->B10, 80.1%->B11)   #
# "Galactic" (Vacuum)                   #
# "BSH"  (Na2B12H11SH)                  #
# "BSH_WAT_1"    (1%->BSH,  99%->water) #
# "BSH_WAT_30"   (30%->BSH, 70%->water) #
# "BSH_WAT_100"  (100%->BSH, 0%->water) #
# "G4_WATER" (NIST Ref)                 #
# "BORE10" (pur B10)                    #
# "BORE11" (pur B11)                    #
#########################################

#/B3/detector/setDetectorSetup Full_DPGA
#/B3/detector/setDetectorSetup Detector_Cylinder
# /B3/detector/setDetectorSetup Without_Detector
#/B3/detector/setDetectorSetup Scoring_Target
SetDetectorSetup=Full_DPGA


SetTargetSetup=Target_Cube

# HADRONTHERAPY_AllHP_opt4 HADRONTHERAPY_HP_opt4 HADRONTHERAPY_NOHP_opt4 HADRONTHERAPY_NOHP_opt3
Physics=HADRONTHERAPY_NOHP_opt3
Environmental_variable=0

KillSecondaryElectrons=false

## to CHANGE VAR PARAMETER ACTUALLY ONLY ENERGY IS AVAIL
VAR_PARAMETER=ENERGY
# if VAR_PARAMETER = ENERGY
ENERGY_MIN=65
ENERGY_MAX=500
PAS_ENERGY=5000
### proton gamma
PARTICLE_SOURCE=proton

Coef_EVENT_NB=10
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

fTimeStructure=2 # Beam Structure
echo "Time structure ${fTimeStructure}"
# Medicyc Proteus
Accelerator="Medicyc" 

if [ ${Accelerator} = "Medicyc" ] && [ ${fTimeStructure} = "2" ]
then
#Bunch 
BunchPeriod=40 #ns
BunchParticulesTimeDistribution=4 #ns
ParticulesPerBunch=-5 # <0 -> nA
#Burst
BurstPeriod=0 #ns if >0 -> BurstMod
BunchPerBurst=0 # > = 0
elif [ ${Accelerator} = "Proteus" ] && [ ${fTimeStructure} = "2" ]
then
#Bunch 
BunchPeriod=17 #ns
BunchParticulesTimeDistribution=3 #ns
ParticulesPerBunch=435374 # <0 -> nA 435374
#Burst
BurstPeriod=1000000 #ns if >0 -> BurstMod
BunchPerBurst=294 # > = 0
else
echo "fTimeStructure ${fTimeStructure}"
fi



if [ ${Physics} = "HADRONTHERAPY_AllHP_opt4" ] && [ ${Environmental_variable} = "1" ]
then
export DO_NOT_SET_PHP_AS_HP=1
export G4PHP_DO_NOT_ADJUST_FINAL_STATE=1
else
export DO_NOT_SET_PHP_AS_HP=0
export G4PHP_DO_NOT_ADJUST_FINAL_STATE=0
fi

########################### Biasing & Compress OPTION(s) #########################
# (D) Default option
BiasingForSaveData=Only_Event_With_SecondaryBetaPlus_HitsCrystalLeftAndRight # No
CompressTarget=DPGA_OnlyBeta_annihilation_option_2 # All No(D) OnlyBeta_annihilation_option_1 OnlyBeta_annihilation_option_2 
CompressDetector=No # All No(D) OnlyBeta_annihilation_option_1 OnlyBeta_annihilation_option_2 
########################### Biasing & Compress OPTION(s) #########################

########################### TARGET OPTION(s) #########################

BOXTARGETSIZE_Z=50

BOXTARGETSIZE="50 50 ${BOXTARGETSIZE_Z} mm"

# WARNING mm 
POSITION_BEAM_X=0
POSITION_BEAM_Y=0
POSITION_BEAM_Z='-100' #mm

# let "POSITION_TARGET_Z=($BOXTARGETSIZE_Z/2)"

POSITION_TARGET_Z="25" # unit just under
TARGETPOSITION="0 0 ${POSITION_TARGET_Z} mm"

########################### TARGET OPTION(s) #########################

########################### CAVITY OPTION(s) #########################
#TARGETOPTION="Cavity_Cube" / "NO" / "Cavity_Tube"
TARGETOPTION="NO"
# G4_B BORE
#CAVITYTARGETMATERIAL="G4_WATER"
#CAVITYTARGETMATERIAL="BSH_WAT_1"
#CAVITYTARGETMATERIAL="BSH_WAT_30"
CAVITYTARGETMATERIAL="Bore_Nat"

# WARNING LOCAL POSITION ! CAVITY IS INSIDE THE TARGET  
CavityTargetPosition="0 0 3.5 mm"
# if TARGETOPTION="Cavity_Cube"
CavityTargetFullSize="1 1 1 mm"
# if TARGETOPTION="Cavity_Tube"
RadiusCavity="20 mm"
LengthCavity="13 mm"
######################################################################


# WARNING PAY ATTENTION TO THE SIZE OF WORLD !!! WARNING

SETWORLD=Setup_World_Cube
# if Setup_World_Cube or Cube 
WORLD_CUBE_SIZE="500 500 500 mm"
# if world tubes by default
LenghtCylinder_World="1 m"
RadiusCylinder_World="1 m"

DISTANCE_BEAM_TO_CENTEROFTARGET=$(echo "sqrt((${POSITION_TARGET_Z}-(${POSITION_BEAM_Z}))^2)"  |bc -l )

ENERGY_NB=-1 
let "ENERGY_NB=($ENERGY_MAX - $ENERGY_MIN)/$PAS_ENERGY +1"

echo "PARTICLE_SOURCE	= ${PARTICLE_SOURCE}"
echo "Accelerator	= ${Accelerator}"
echo "ENERGY_MIN [MeV]	= ${ENERGY_MIN}"
echo "ENERGY_MAX [MeV]	= ${ENERGY_MAX}"
echo "PAS_ENERGY [MeV]	= ${PAS_ENERGY}"
echo "Nbr of Testing Energy	= ${ENERGY_NB}"
echo "Event Per Pas Energy [MeV]	= ${EVENT_NB}"
echo "MATERIAL	= ${MATERIAL}"
echo "TargetSetup	= ${SetTargetSetup}"

echo "BOXTARGETSIZE	= ${BOXTARGETSIZE}"
echo "TARGETPOSITION	= ${TARGETPOSITION}"

echo "Physics List	= ${Physics}"
echo "Environmental_variable	= ${Environmental_variable}"
echo "KillSecondaryElectrons	= ${KillSecondaryElectrons}"

if [ ${TARGETOPTION} != "NO" ]
then
echo "Cavity Setup	= ${TARGETOPTION}"
echo "Cavity MATERIAL	= ${CAVITYTARGETMATERIAL}"
else
echo "No Cavity"
fi

#echo "DISTANCE_BEAM_TO_CENTEROFTARGET	=${DISTANCE_BEAM_TO_CENTEROFTARGET}"
#echo "BOXSIZE_Z	=${BOXTARGETSIZE_Z}"

# TODO 
# Mettre ici les constantes || Donc pas energy utiliser "sed" pour ça !!!!
# Ne pas oublier de les ajouters dans le nom du fichier.mac et dans les echo()
# TODO


SETUP_COMMENT_NAME="MATERIAL BOXTARGETSIZE_X BOXTARGETSIZE_Y BOXTARGETSIZE_Z BOXTARGETSIZE_UNIT V_GEANT4 PATCH PARTICLE_SOURCE VAR_PARAMETER TargetSetup DetectorSetup TARGETPOSITION_X TARGETPOSITION_Y TARGETPOSITION_Z TARGETPOSITION_UNIT BiasingForSaveData fTimeStructure BunchPeriod BunchParticulesTimeDistribution ParticulesPerBunch CompressTarget CompressDetector BurstPeriod BunchPerBurst Accelerator HOSTNAME PhysicsList KillSecondaryElectrons Environmental_variable POSITION_BEAM_X POSITION_BEAM_Y POSITION_BEAM_Z"
SETUP_COMMENT="${MATERIAL} ${BOXTARGETSIZE} ${V_GEANT4} ${PATCH} ${PARTICLE_SOURCE} ${VAR_PARAMETER} ${SetTargetSetup} ${SetDetectorSetup} ${TARGETPOSITION} ${BiasingForSaveData} ${fTimeStructure} ${BunchPeriod} ${BunchParticulesTimeDistribution} ${ParticulesPerBunch} ${CompressTarget} ${CompressDetector} ${BurstPeriod} ${BunchPerBurst} ${Accelerator} ${HOSTNAME} ${Physics} ${Environmental_variable} ${KillSecondaryElectrons} ${POSITION_BEAM_X} ${POSITION_BEAM_Y} ${POSITION_BEAM_Z}"


TARGET_COMMENT_NAME=""
TARGET_COMMENT=""


if [ ${TARGETOPTION} = "Cavity_Cube" ]
then
TARGET_COMMENT_NAME=" CavityTargetFullSize_X CavityTargetFullSize_Y CavityTargetFullSize_Z CavityTargetFullSize_UNIT"
TARGET_COMMENT=" ${CavityTargetFullSize}"
elif [ ${TARGETOPTION} = "Cavity_Tube" ]
then
TARGET_COMMENT_NAME=" CavityLength CavityLength_UNIT RadiusCavity RadiusCavity_UNIT"
TARGET_COMMENT=" ${LengthCavity} ${RadiusCavity}"
elif [ ${TARGETOPTION} = "NO" ]
then
echo ""
else
echo "WARNING Unknow Cavity Type !!! WARNING !!!"
fi

if [ ${TARGETOPTION} != "NO" ]
then
TARGET_COMMENT_NAME+=" TARGETOPTION CAVITYTARGETMATERIAL CavityTargetPosition_X CavityTargetPosition_Y CavityTargetPosition_Z CavityTargetPosition_UNIT" 
TARGET_COMMENT+=" ${TARGETOPTION} ${CAVITYTARGETMATERIAL} ${CavityTargetPosition}"

SETUP_COMMENT_NAME+=${TARGET_COMMENT_NAME}
SETUP_COMMENT+=${TARGET_COMMENT}

else
echo "No Cavity Target"
fi

a=1
b=2

CMAKE_CMD="-DGeant4_DIR=/home/gate/Progs/Geant4/geant4.10.02-install/lib/Geant4-10.2.0 ../DPGA"

### For Option FARM HPC2

if [ ${FARM} = "1" ] && [ ${V_GEANT4} = "10.2" ] && [ ${PATCH} = "p02" ]
then
module load clhep/2.4.1.0
module load cmake/3.6.1
module load gcc/4.8.4
module load root/6.09.02
module load geant4/10.02.p02 
vRoot='6.14.00'
echo "Farm Geant ${V_GEANT4} sourced"

export G4PROTONHPDATA=/home/arbongra/disk/G4TENDL1.3.2/Proton

elif [ ${FARM} = "1" ] && [ ${V_GEANT4} = "10.3" ] && [ ${PATCH} = "p03" ] && [ ${MULTITHREADING} = "1" ]
then
module load gcc/4.8.4
module load clhep/2.4.1.0
module load cmake/3.6.1
module load root/6.14.00
module load geant4/10.03.p03
vRoot='6.14.00'
echo "Farm Geant ${V_GEANT4}${PATCH} sourced"

export G4PROTONHPDATA=/home/arbongra/disk/G4TENDL1.3.2/Proton

elif [ ${FARM} = "1" ] && [ ${V_GEANT4} = "10.3" ] && [ ${PATCH} = "p03" ] && [ ${MULTITHREADING} > ${a} ]
then
module load gcc/4.8.4
module load clhep/2.4.1.0
module load cmake/3.6.1
module load root/6.14.00
module load geant4/10.03.p03
vRoot='6.14.00'
echo "Farm Geant ${V_GEANT4}${PATCH}  sourced"

export G4PROTONHPDATA=/home/arbongra/disk/G4TENDL1.3.2/Proton

elif [ ${FARM} = "1" ] && [ ${V_GEANT4} = "10.4" ] && [ ${PATCH} = "p02" ] && [ ${MULTITHREADING}  = "1" ]
then
module load gcc/4.8.4
module load clhep/2.4.1.0
module load cmake/3.6.1
module load root/6.14.00
module load geant4/10.04.p02
vRoot='6.14.00'
echo "Farm Geant ${V_GEANT4}${PATCH} sourced"

export G4PROTONHPDATA=/home/arbongra/disk/G4TENDL1.3.2/Proton

elif [ ${FARM} = "1" ] && [ ${V_GEANT4} = "10.4" ] && [ ${PATCH} = "p02" ] && [ ${MULTITHREADING} > ${a} ]
then

module load clhep/2.4.1.0
module load cmake/3.6.1

module load gcc/4.8.4
module load root/6.14.00
module load geant4/10.04.p02
vRoot='6.14.00'
echo "Farm Geant ${V_GEANT4}${PATCH}  sourced"

export G4PROTONHPDATA=/home/arbongra/disk/G4TENDL1.3.2/Proton

elif [ ${FARM} = "1" ]
then
echo ""
else
echo "[FARM] Other NOT SOURCED G4 version ${V_GEANT4} ${PATCH}"
echo "Avail 10.03p03 10.04p02"
fi

###

### Local ARTHUR

if [ ${V_GEANT4} = "10.2" ] && [ ${FARM} = "0" ] && [ ${PATCH} = "p02" ]
then

  $root6
  $geant42
  CMAKE_CMD="-DGeant4_DIR=/home/gate/Progs/Geant4/geant4.10.02-install/lib/Geant4-10.2.0 ../DPGA"
  echo "vGeant 10.2"
  PATCH=p02
  
elif [ ${V_GEANT4} = "10.3" ] && [ ${FARM} = "0" ] && [ ${PATCH} = "p02" ]
then

  $root6
  $geant43
  CMAKE_CMD="-DGeant4_DIR=/home/gate/Progs/Geant4/geant4.10.3.p02-build ../DPGA"
  PATCH=p02
  echo "vGeant 10.3 p02"
  
  source /home/gate/Progs/Geant4/geant4.10.3.p02-build/InstallTreeFiles/geant4.sh

export G4PROTONHPDATA=/home/gate/Progs/Geant4/geant4.10.3.p02-build/data/G4TENDL1.3/Proton

  
else
echo "Other NOT SOURCED G4 version ${V_GEANT4}"
fi

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

NAMEBUILD=BuildDPGA_launcher_Medycic
# rm -r ${SIMULATION_DIR}/../${NAMEBUILD}

if (( ${MULTITHREADING} > ${a} ))
then
NAMEBUILD=BuildDPGA_launcher_Mt
# rm -r ${SIMULATION_DIR}/../${NAMEBUILD}


else
echo "No MT"
fi

# Changement des physiques list directement dans le .CC

cp -r ${SIMULATION_DIR} ./
cd DPGA
SIMULATION_DIR=`pwd`
echo "SIMULATION_DIR = "`pwd`

# HADRONTHERAPY_AllHP_opt4 HADRONTHERAPY_HP_opt4 HADRONTHERAPY_NOHP_opt4 HADRONTHERAPY_NOHP_opt3

if [ ${Physics} = "HADRONTHERAPY_AllHP_opt4" ]
then
sed -i "s/RegisterPhysics(new G4EmStandardPhysics_option3());/RegisterPhysics(new G4EmStandardPhysics_option4());/g"       ${SIMULATION_DIR}/src/PhysicsList.cc
sed -i "s/RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP());/RegisterPhysics(new G4HadronPhysicsQGSP_BIC_AllHP());/g"       ${SIMULATION_DIR}/src/PhysicsList.cc
sed -i "s/RegisterPhysics( new G4HadronElasticPhysics());/RegisterPhysics(new G4HadronElasticPhysicsHP());/g"       ${SIMULATION_DIR}/src/PhysicsList.cc

elif [ ${Physics} = "HADRONTHERAPY_HP_opt4" ]
then
sed -i "s/RegisterPhysics(new G4EmStandardPhysics_option3());/RegisterPhysics(new G4EmStandardPhysics_option4());/g"       ${SIMULATION_DIR}/src/PhysicsList.cc
sed -i "s/RegisterPhysics( new G4HadronElasticPhysics());/RegisterPhysics(new G4HadronElasticPhysicsHP());/g"       ${SIMULATION_DIR}/src/PhysicsList.cc

elif [ ${Physics} = "HADRONTHERAPY_NOHP_opt4" ]
then
sed -i "s/RegisterPhysics(new G4EmStandardPhysics_option3());/RegisterPhysics(new G4EmStandardPhysics_option4());/g"       ${SIMULATION_DIR}/src/PhysicsList.cc
sed -i "s/RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP());/RegisterPhysics(new G4HadronPhysicsQGSP_BIC());/g"       ${SIMULATION_DIR}/src/PhysicsList.cc

elif [ ${Physics} = "HADRONTHERAPY_NOHP_opt3" ]
then
sed -i "s/RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP());/RegisterPhysics(new G4HadronPhysicsQGSP_BIC());/g"       ${SIMULATION_DIR}/src/PhysicsList.cc
  
else
echo "Unknow PhysicsList ${Physics} : Use Default HP_Opt3 "
fi



# sed -i "s/RegisterPhysics(new G4EmStandardPhysics_option3());/RegisterPhysics(new G4EmStandardPhysics_option4());/g"       ${SIMULATION_DIR}/src/PhysicsList.cc
# sed -i "s/\/\/CONSTRUCTOR\_COMMENT\_MULTITHREADING\_END/*\//g"       ${SIMULATION_DIR}/src/PhysicsList.cc

# sed -i "s/\/\/ADD_OTHER_PHYSICLIST_WITH_REGISTER_METHOD/RegisterPhysics(new G4DecayPhysics());RegisterPhysics(new G4RadioactiveDecayPhysics());RegisterPhysics(new G4EmStandardPhysics_option4());RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP());  RegisterPhysics( new G4EmExtraPhysics()); RegisterPhysics( new G4HadronElasticPhysics());RegisterPhysics( new G4StoppingPhysics());RegisterPhysics( new G4IonBinaryCascadePhysics());RegisterPhysics( new G4NeutronTrackingCut());/g"       ${SIMULATION_DIR}/src/PhysicsList.cc

cd ${WORK_DIR}



if [ ${FARM} = "0" ]
then

mkdir ${WORK_DIR}/${NAMEBUILD}
cd ${WORK_DIR}/${NAMEBUILD}
echo "CMAKE_DIR = "`pwd`
# cmake ../DPGA

cmake ${CMAKE_CMD}

echo "Cmake on DPGA ${SIMULATION_DIR}"

elif [ ${FARM} = "1" ]
then

mkdir ${WORK_DIR}/${NAMEBUILD}
cd ${WORK_DIR}/${NAMEBUILD}

echo "CMAKE_DIR = "`pwd`
echo "SIMULATION_DIR = ${SIMULATION_DIR}"

cmake ${SIMULATION_DIR}
echo "Cmake on DPGA"

else
cmake ../DPGA
fi

if (( ${MULTITHREADING} > ${a} ))
then
echo "make Multi"
make -j${MULTITHREADING}
echo "MULTITHREADING = ${MULTITHREADING} "
else
make
fi

echo "Build is mounted"
cd ${WORK_DIR}
 
if [ ${FARM} != "1" ]
then
echo "No cp" 
# cp -r ${SIMULATION_DIR}/../${NAMEBUILD} ${WORK_DIR}/.
else
echo "FAMR NO cp"
fi

# Save .sh
cp -r ${WORK_DIR}/../../${NameMacro} ${WORK_DIR}/.

echo "Path run_launcher ${WORK_DIR}/${NAMEBUILD}"
#ls -lsah ${WORK_DIR}/${NAMEBUILD}
rm -r ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac

#ls -lsah ${WORK_DIR}/${NAMEBUILD}


/bin/cat <<EOM > ${WORK_DIR}/run_launcher.mac
   
# $Id: run_Medycic.mac 2017 abongrand $
#
# Macro file of "main_DPGA.cc"
#

/control/verbose 0
/tracking/verbose 0
/analysis/verbose 0
#
#
#/control/execute vis.mac

/run/verbose 0
/run/printProgress 0
/gps/verbose 0
# Dump particle processes
#

/particle/select proton
/particle/process/dump

/particle/select e+
/particle/process/dump

/particle/select e-
/particle/process/dump

/particle/select gamma
/particle/process/dump

#https://twiki.cern.ch/twiki/bin/view/Geant4/LoweAtomicDeexcitation
  /process/em/fluo false
  /process/em/auger false
  /process/em/augerCascade false
  /process/em/pixe false

  #  /process/msc/UseMottCorrection true # min v10.3 implementé directement dans PhysicsList.cc
  /process/msc/StepLimit UseSafetyPlus 
  /process/msc/Skin 3
  
  /B3/StackingAction/KillSecondaryElectrons ${KillSecondaryElectrons}

#   /phys/addPhysics ${Physics}

###################################################
### Geometry
###################################################

##############################################
##### Setup World 
##############################################

/B3/detector/setWorld ${SETWORLD}
/B3/detector/LenghtCylinder_World ${LenghtCylinder_World}
/B3/detector/RadiusCylinder_World ${RadiusCylinder_World}
/B3/detector/WorldCubeFullSize ${WORLD_CUBE_SIZE}

##############################################
##### Setup Target 
##############################################

/B3/detector/TargetPosition ${TARGETPOSITION}
/B3/detector/setTargetMaterial MATERIAL

/B3/detector/TargetFullSize ${BOXTARGETSIZE}

/B3/detector/OptionTarget ${TARGETOPTION}
/B3/detector/setCavityTargetMaterial ${CAVITYTARGETMATERIAL}
/B3/detector/CavityTargetPosition ${CavityTargetPosition}

# if Cavity_Tube
/B3/detector/LengthCavity ${LengthCavity}
/B3/detector/RadiusCavity ${RadiusCavity}
# if Cavity_Cube
/B3/detector/CavityTargetFullSize ${CavityTargetFullSize}


/B3/detector/setTargetSetup ${SetTargetSetup}
#/B3/detector/setTargetSetup Target_Cylinder

# if Target_Cylinder
/B3/detector/LenghtCylinder_Target 60 mm
/B3/detector/RadiusCylinder_Target 50 mm

##############################################

##############################################
##### Setup Detector 
##############################################

#/B3/detector/setDetectorSetup Full_DPGA
#/B3/detector/setDetectorSetup Detector_Cylinder
/B3/detector/setDetectorSetup ${SetDetectorSetup}

# if Target_Cylinder
/B3/detector/LenghtCylinder_Detector 800 mm
/B3/detector/LenghtCylinder_Detector 50 mm

##############################################
###################################################

################################################################################
### Temporal TimeStructure 
################################################################################

# 0->Default 1->Decay 2 -> Beam Bunch

/B3/generator/fTimeStructure ${fTimeStructure}
/B3/generator/StartWithADelay STARTWITHADELAY

# Decay command (1) 
/B3/generator/DecayTauPrimmary 10 ns

# Bunch command (2)
/B3/generator/BunchPeriod ${BunchPeriod} ns
/B3/generator/BunchParticulesTimeDistribution ${BunchParticulesTimeDistribution} ns
/B3/generator/ParticulesPerBunch ${ParticulesPerBunch}

# Burst command (2)
/B3/generator/BurstPeriod ${BurstPeriod} ns
/B3/generator/BunchPerBurst ${BunchPerBurst}

# if you enter a negativ ParticulesPerBunch it count like an intensity in nA

################################################################################

###################################################
# 1D accelerator beam
###################################################

#
/gps/particle ${PARTICLE_SOURCE}
/gps/pos/type Beam
#

# the incident surface is in the y-z plane
/gps/pos/rot1 0 1 0
/gps/pos/rot2 0 0 1
#
# the beam spot is centered at the origin and is 
# of 1d gaussian shape with a 3mm central plateau
/gps/pos/centre ${POSITION_BEAM_X} ${POSITION_BEAM_Y} ${POSITION_BEAM_Z} mm

#
# the beam is travelling along the z-axis with 5 degrees dispersion
/gps/ang/rot1 -1 0 0
#/gps/ang/rot2 0 1 0
/gps/ang/type beam1d 
/gps/ang/sigma_r 0. deg
# 
# the beam energy is in gaussian profile centered at 400 MeV
/gps/ene/type Gauss
/gps/ene/mono ENERGY_R MeV
/gps/ene/sigma 0. MeV



#########################################################
# Set particle production thresholds (cuts)
#########################################################
#
/run/setCut 0.01 mm
/run/setCutForAGivenParticle e- 0.1 mm
/run/setCutForAGivenParticle e+ 0.1 mm
/run/setCutForAGivenParticle gamma 0.1 mm
#
############################################################

##########################################################
## Scoring
##########################################################

# NO SCORING SEE Multi_Scoring

#########################################################
# EventAction Set Biasing for Save Data
#########################################################

# Put No if tyou don't want this option  
/B3/EventAction/BiasingForSaveData ${BiasingForSaveData} # Only_Event_With_SecondaryBetaPlus || No

/B3/EventAction/CompressTarget ${CompressTarget}
/B3/EventAction/CompressDetector ${CompressDetector}

############################################################


/analysis/setFileName ./NAMEFILE

############################################################
# Beam nbr of event
############################################################


/run/beamOn EVENT_NB

############################################################

EOM

#ls -lsah ${WORK_DIR}/${NAMEBUILD}

mkdir ${WORK_DIR}/G4_log

numbersetup_i=0
STARTWITHADELAY=0

exit 0

if [ ${VAR_PARAMETER} = "ENERGY" ]
then
 ### Loop over setup nb
for((energy_i=${ENERGY_MIN} ; energy_i<${ENERGY_MAX} ; energy_i+=${PAS_ENERGY}))
do
  echo "  "
  echo "Energy_"${energy_i}
  echo "  "
  
  ### TODO insert other parameter variable in a tank here
  
  
  let "numbersetup_i=($numbersetup_i+1)"
  NameFile="File_${numbersetup_i}.root"
 
   
   
   if (( ${JOB_NB} > ${a} ))
   then
   
   
   if [ ${numbersetup_i} = "1" ]
   then
   
    (
    echo "## JobID FileName Setup FileNameObjectif JOB_NB"
    ) > ${WORK_DIR}/InstructionsForMerge.txt
   
   fi
   
   #SaveBySetupBeforeMerge
   
   mkdir ${WORK_DIR}/Setup_${numbersetup_i}
   
   ### Loop over job nb
   for((job_i=0 ; job_i<${JOB_NB} ; job_i++))
   do
   
   
   echo "Job_"${job_i}
   
   
   NameOfJob="Setup_${numbersetup_i}_Job_${job_i}"
  
   
   mkdir ${WORK_DIR}/Setup_${numbersetup_i}/Job_${job_i}
   
   NameFile_Job="File_${numbersetup_i}_Job_${job_i}.root"
   

   cp -r ${WORK_DIR}/run_launcher.mac ${WORK_DIR}/${NAMEBUILD}/.
  
   ### Make batsh_script.sh modifications
  
   ## Constantes
   sed -i "s/MATERIAL/${MATERIAL}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   sed -i "s/EVENT_NB/${EVENT_NB}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   sed -i "s/BOXSIZE/${BOXSIZE}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   sed -i "s/BINSIZE/${BINSIZE}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   sed -i "s/V\_GEANT4/${V_GEANT4}/g"    ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   ## Variable
   sed -i "s/ENERGY_R/${energy_i}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   
    ## Name DOseFile NAMEFILE
   sed -i "s/NAMEFILE/${NameFile_Job}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   
   
   let "STARTWITHADELAY=($EVENT_NB * (${job_i} ) )"
   echo "Delay Event ${STARTWITHADELAY}"
   sed -i "s/STARTWITHADELAY/${STARTWITHADELAY}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   
   
   ###########
   # general #
   ###########
   ### Make new environement directories
   mkdir ${WORK_DIR}/${NameOfJob}
   mkdir ${WORK_DIR}/${NameOfJob}/G4_log
   
   ### Save script
   
   cp -r ${WORK_DIR}/${NAMEBUILD} ${WORK_DIR}/${NameOfJob}/.
   cp ${WORK_PWD}/Job.sh ${WORK_DIR}/${NameOfJob}/.
  
  sed -i "s/vgeant/${V_GEANT4}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
  sed -i "s/patch/${PATCH}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
  
  sed -i "s/multithreading/${MULTITHREADING}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
  sed -i "s/profil/${Profil}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
  
   sed -i "s/SETUPNAME/Setup_${numbersetup_i}/g" ${WORK_DIR}/${NameOfJob}/Job.sh
  
  Path="${NAMEBUILD}"
  sed -i "s/PATH/${Path}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
  
  PathG4Log=".\/..\/..\/G4_log\/"
   sed -i "s/PATHG4LOG/${PathG4Log}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
  
  NameG4Log="G4_${energy_i}_SETUP_${numbersetup_i}_Job_${job_i}.txt"
  
  sed -i "s/NAMEG4LOG/${NameG4Log}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
  
  sed -i "s/RUN\_ID/${NEXT_RUN_ID}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
  
  sed -i "s/job\_ID/${job_i}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
  sed -i "s/ENERGY/${energy_i}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
  
  sed -i "s/PHYSICS/${Physics}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
  sed -i "s/ENVIRONMENTALVARIABLE/${Environmental_variable}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
 
  sed -i "s/ReplacePartition/${Partition}/g"    ${WORK_DIR}/${NameOfJob}/Job.sh
   ##############
   # production #
   ##############
   ### Make new environement directories
   ### Save scrip

   ### Make main_batch.cc modifications

    (
    echo "${job_i} ${NameFile_Job} ${numbersetup_i} ${NameFile} ${JOB_NB}"
    ) >> ${WORK_DIR}/InstructionsForMerge.txt
   
  
   ##########
   # submit #
   ##########
   ### Submit job_i
   cd ${WORK_DIR}/${NameOfJob}
   
   if [ ${FARM} = "1" ]
   then
   sbatch ${WORK_DIR}/${NameOfJob}/Job.sh
   elif [ ${FARM} = "2" ]
   then
   nohup ./Job.sh &
   else
   ./Job.sh
   fi
   
   done
   
    (
    echo "${Name_Of_File} run_${NEXT_RUN_ID}"
    ) >> ${WORK_DIR}/../../RunToMerge.txt
   
   let "EVENT_NB=($EVENT_NB * ${JOB_NB})"
   
   #event number = event * nbr job
   
   else

   cp -r ${WORK_DIR}/run_launcher.mac ${WORK_DIR}/${NAMEBUILD}/.
  
   ### Make batsh_script.sh modifications
  
   ## Constantes
   sed -i "s/MATERIAL/${MATERIAL}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   sed -i "s/EVENT_NB/${EVENT_NB}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   sed -i "s/BOXSIZE/${BOXSIZE}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   sed -i "s/BINSIZE/${BINSIZE}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   sed -i "s/V\_GEANT4/${V_GEANT4}/g"    ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   ## Variable
   sed -i "s/ENERGY_R/${energy_i}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   
   sed -i "s/NAMEFILE/${NameFile}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   
   sed -i "s/STARTWITHADELAY/${STARTWITHADELAY}/g"       ${WORK_DIR}/${NAMEBUILD}/run_launcher.mac
   
   cd ${WORK_DIR}/${NAMEBUILD}
  ./main_DPGA run_launcher.mac >>${WORK_DIR}/G4_log/G4_${energy_i}_SETUP_${numbersetup_i}.txt
      # ./main_DPGA run_Medicyc.mac >>${WORK_DIR}/G4_log/G4_${energy_i}_SETUP_${numbersetup_i}.txt

   cp ${WORK_DIR}/${NAMEBUILD}/Crystals_Positions.txt ${WORK_DIR}/.
   cp ${WORK_DIR}/${NAMEBUILD}/Reactions.txt ${WORK_DIR}/.
   cp ${WORK_DIR}/${NAMEBUILD}/AllProcess.txt ${WORK_DIR}/.
   cp ${WORK_DIR}/${NAMEBUILD}/PDGLifeTime.txt ${WORK_DIR}/.
   
   fi
   
   cd ${WORK_DIR}
   
   
   END=$(date +%s)
   DIFF=$(( $END - $START ))
   
   
  if [ ${numbersetup_i} = "1" ]
  then
    (
    echo "## NUMBERSETUP NAMEROOTFILE ENERGY EVENT_NB ${SETUP_COMMENT_NAME} vRoot Time[s]"
    ) > ${WORK_DIR}/Setup.txt
  fi
    
    (
    echo "${numbersetup_i} ${NameFile} ${energy_i} ${EVENT_NB} ${SETUP_COMMENT} ${vRoot} ${DIFF}"
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
else
# TODO
#
#
echo "${VAR_PARAMETER} NOT IMPLEMENTED YET"
#
#
# TODO study Bore
fi
  
mv ${WORK_DIR}/${NAMEBUILD}/*.root ${WORK_DIR}
  
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
