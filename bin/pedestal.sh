#!/bin/bash

SRCUSER=eudet
SRCHOST=eudetpc001
SCRIPTPATH=/RAID/ilc/Eutelescope/v00-00-08-plugin/pysub
ENVFILE=./build_env.sh
DATAPATH=/RAID/ilc/data
DESTUSER=eudet
DESTHOST=mvme6100
DESTPATH=eudaq/pedestal
LASTFILE=pedestal.dat

RUNNUM=$1
TMP=0000000000
RUNNUM0=${TMP:0:$((${#RRUNNUM} > 6 ? 0 : 6 - ${#RUNNUM}))}${RUNNUM}

if [ $# != 1 ]; then
  echo "usage $0 runnumber"
  echo "Converts a run, calculates pedestals, uploads for use in the DAQ, and updates the latest pedestal setting"
  echo "You should make sure you have an ssh key set up to allow you to log into ${SRCUSER}@${SRCHOST} without a password"
  exit 1
fi

echo "Converting run ${RUNNUM}"
ssh -x ${SRCUSER}@${SRCHOST} "cd ${SCRIPTPATH} && source ${ENVFILE} && ./submit-converter ${RUNNUM}" || exit 1

echo "Calculating pedestals for run ${RUNNUM}"
ssh -x ${SRCUSER}@${SRCHOST} "cd ${SCRIPTPATH} && source ${ENVFILE} && ./submit-pedestal ${RUNNUM}" || exit 1

# Display some histograms here...
# Ask if they look OK...

echo "Copying pedestals for run ${RUNNUM}"
ssh -x ${SRCUSER}@${SRCHOST} "echo scp ${DATAPATH}/db/run${RUNNUM0}-ped-db-* ${DESTHOST}:${DESTPATH}/." || exit 1

echo "Updating latest pedestal info"
# Do it via $SRCHOST, because we know ssh keys are set up from there
echo ${RUNNUM} | ssh -x ${SRCUSER}@${SRCHOST} "ssh -x ${DESTUSER}@${DESTHOST} tee ${DESTPATH}/${LASTFILE}" || exit 1

echo "OK"
