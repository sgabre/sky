#!/bin/bash


# CPKG - Configuration Package
# + CPKG-<BOARD>-<VERSION>-<TIMESTAMP>-<REVISION>.tar
# |- Pack.json
# |- bin (optionel)
# |  \- upgrade.bin.enc
# |  \- upgrade.bin.sgn
# |  \- application.bin.enc
# |  \- application.bin.sgn
# |- config
# |  \- <configuration>.json
# |- keys
# |  \- Device-<UID>.key

# UPKG - Upgrade Package
# + UPKG<BOARD>-<VERSION>-<TIMESTAMP>-<REVISION>.tar
# |- Pack.json
# |- bin
# |  \- upgrade.bin.enc
# |  \- upgrade.bin.sgn
# |  \- application.bin.enc
# |  \- application.bin.sgn
# |- config (optionel)
# |  \- <configuration>.json
# |- keys
# |  \- Device-<UID>.key

# PPKG - Product Package
# + PPKG-<BOARD>-<VERSION>-<TIMESTAMP>-<REVISION>.tar
# |- Pack.json
# |- Hash.xml (SHA or MD5)
# |- bin
# |  \- upgrade.bin.enc
# |  \- upgrade.bin.sgn
# |  \- application.bin.enc
# |  \- application.bin.sgn
# |- config
# |  \- <configuration>.json
# |- keys
# |  \- Device-<UID>.key

# SPKG - Support Package
# + PPKG-<BOARD>-<VERSION>-<TIMESTAMP>-<REVISION>.tar
# |- Pack.json
# |- config
# |  \- <configuration>.json
# |- logs
# |  \- <System>.log
# |- keys
# |  \- Device-<UID>.key

# DPKG - Delivery/Deployement Package
# |- Pack.json
# |- Software-Release-Note.pdf
# |- bin
# |  \- bootloader.bin
# |  \- upgrader.bin
# |  \- production.bin
# |  \- application.bin
# |  \- application.dfu (boot, upgrader, application)
# |  \- production.dfu (boot, upgrader, production)
# |- resources
# |  \-  (resources files)
# |- Packs
# |  \-  PPKG-<BOARD>-<VERSION>-<TIMESTAMP>-<REVISION>.tar
# |  \-  UPKG-<BOARD>-<VERSION>-<TIMESTAMP>-<REVISION>.tar
# |  \-  CPKG-<BOARD>-<VERSION>-<TIMESTAMP>-<REVISION>.tar

# SCP - Software Component Package, simplify the re-use of software and the management of 3rd party software. 
# |  \- Pack Description (*.pdsc)
# |  \- A collection of source modules, header and configuration files as well as libraries.
# |  \- Documentation of the software, including features and APIs.
# |  \- Application Programming Interfaces specify one or more APIs for software components that are part of another Software Pack.
# |  \- Example Projects can be used as a reference and starting point for similar applications. 
# |  \- User Code Templates help to understand the basic concepts of a software component better and to give the user a good starting point for implementing his application.

# BSP - Board Support Package for rapid development in the prototyping stage:
# |  \- Pack Description (*.pdsc)
# |  \- Information about the development board and it's features.
# |  \- Parameters, technical information, and data sheets about the board, the mounted microcontroller and peripheral devices.
# |  \- Drivers for on-board peripheral devices

# Pack Description (*.pdsc) or pack.json
# Supplier and release information
# <vendor>: name of the supplier or vendor of the Software Pack.
# <name>: name of the Software Pack. It is the vendor's responsibility to ensure unique package names.
# <release version="X.Y.Z">: version number of the Software Pack.
# dependency
#
# components
#


# Tools
# zip
# md5
# shasum
# jq (json parser)

# default values
SCRIPT_DIR=$(realpath $(dirname "${0}"))

PACK_WAREHOUSE=pack/
PACK_BUILD=build/

#KEYS_DIR=$ROOT_DIR/Keys/Readers
#SYMMETRIC_KEY_FULL_PATH=$KEYS_DIR/symmetric.key
#SERVER_PRIVATE_KEY_FULL_PATH=$KEYS_DIR/server-private-key.pem
#set SERVER_PUBLIC_KEY_FULL_PATH=%KEYS_DIR%\server-public-key.pem

PACK_TYPE=""
PLATFORM="unknown"
VERSION="0.0.0"
MAJOR_VERSION="0"
MINOR_VERSION="0"
BUGFIX_VERSION="0"
CONFIG_FILE=""

function die
{
	echo "ERROR: ${@}" 1>&2
	exit 1
}

function _Usage
{
	echo 'Usage:'
	echo "${0} -h"
	echo "${0} -p PLATFORM [-i INSTALL_PATH] [-t TOOLCHAIN] [-j JOBS] [-b BUILD_DIR] [-c] [-s] [-r]"
	
	echo
	echo 'Details:'
	echo '-h              - print this help'
	echo '-v              - build version <MAJOR_VERSION>.<MINOR_VERSION>.<BUGFIX_VERSION>'
	echo '-c              - clean build workspace'
	echo '-p              - platform (e.g: WIN32, LINUX, MacOS, <BoardName>)'
	echo
	echo 'Example:'
	echo "${0} -p coffee             - build coffee platform agents and put results to default location '${INSTALL}/coffee'"
	echo
}

function _CheckArgs
{
	hash realpath &>/dev/null || die 'Please install realpath by this command "sudo apt-get install realpath"'
	
	while getopts 'p:crhv:i:T:' opt; do
		case ${opt} in
			v)
				VERSION=${OPTARG}
				MAJOR_VERSION=$(echo $VERSION |  cut -f 1 -d "." )
				MINOR_VERSION=$(echo $VERSION |  cut -f 2 -d "." )
				BUGFIX_VERSION=$(echo $VERSION |  cut -f 3 -d "." );;
			p)
				PLATFORM=${OPTARG};;
			T)
				PACK_TYPE=$(echo "${OPTARG}" | xargs);;
			i)
				CONFIG_FILE=${OPTARG}
				ConfigurationFile;;
			c)
				CLEAN=true;;
			r)
				BUILD_TYPE='Release';;
			h)
				_Usage
				exit 0;;
		esac
	done
}

function _CheckTool
{
	# Zip utility check 
	TOOL=$1
	if [ -n "$TOOL" ]
	then
		type -a $TOOL
		errorlevel=$?
		if [ $errorlevel -gt 0 ]
		then
			echo "Error: No $TOOL Utility found"
			echo "Action: Add $TOOL to your path"
			echo " "
			exit
		fi
	fi
}

function _Clean
{
	# clean build artifacts, if needed
	if ${CLEAN}; then
		echo "Clean build directory '${BUILD_DIR}"
		rm -rf "${BUILD_DIR}" || die
	fi
}

function _ImportArtefact
{
	cp ../Resources/CFG/$CFG/Eligibility.json $PACK_BUILD/data
	cp ../Resources/CFG/$CFG/Config/*.fcon $PACK_BUILD/config
	cp ../Resources/CFG/$CFG/Pricelists/*.fcon $PACK_BUILD/Pricelists
	cp ../Resources/CFG/$CFG/Selections/*.fcon $PACK_BUILD/Selections
	cp ../Resources/CFG/$CFG/Products/*.fcon $PACK_BUILD/Products
}

function LongHash
{
	git rev-parse HEAD 2> /dev/null | sed "s/\(.*\)/\1/"
}

function ShortHash
{
	git rev-parse --short HEAD 2> /dev/null | sed "s/\(.*\)/\1/"
}

function CreateVersionFile
{
	local MAJOR_VERSION=$1
	local MINOR_VERSION=$2
	local BUGFIX_VERSION=$3
	
	local OFILE="../Version/Version.h"
	echo "#ifndef __VERSION_H__">$OFILE
	echo "#define __VERSION_H__">>$OFILE
	echo "#define   MAJOR_VERSION      "$MAJOR_VERSION>>$OFILE
	echo "#define   MINOR_VERSION      "$MINOR_VERSION>>$OFILE
	echo "#define   BUGFIX_VERSION     "$BUGFIX_VERSION>>$OFILE
	echo "#endif  // __VERSION_H__">>$OFILE
}

cfg_parser()
{
	ini="$(<$1)"                # read the file
	ini="${ini//[/\[}"          # escape [
	ini="${ini//]/\]}"          # escape ]
	IFS=$'\n' && ini=( ${ini} ) # convert to line-array
	ini=( ${ini[*]//;*/} )      # remove comments with ;
	ini=( ${ini[*]/\    =/=} )  # remove tabs before =
	ini=( ${ini[*]/=\   /=} )   # remove tabs after =
	ini=( ${ini[*]/\ =\ /=} )   # remove anything with a space around =
	ini=( ${ini[*]/#\\[/\}$'\n'cfg.section.} ) # set section prefix
	ini=( ${ini[*]/%\\]/ \(} )    # convert text2function (1)
	ini=( ${ini[*]/=/=\( } )    # convert item to array
	ini=( ${ini[*]/%/ \)} )     # close array parenthesis
	ini=( ${ini[*]/%\\ \)/ \\} ) # the multiline trick
	ini=( ${ini[*]/%\( \)/\(\) \{} ) # convert text2function (2)
	ini=( ${ini[*]/%\} \)/\}} ) # remove extra parenthesis
	ini[0]="" # remove first element
	ini[${#ini[*]} + 1]='}'    # add the last brace
	eval "$(echo "${ini[*]}")" # eval the result
}
	
cfg_writer ()
{
	IFS=' '$'\n'
	fun="$(declare -F)"
	fun="${fun//declare -f/}"
	for f in $fun; do
		[ "${f#cfg.section}" == "${f}" ] && continue
		item="$(declare -f ${f})"
		item="${item##*\{}"
		item="${item%\}}"
		item="${item//=*;/}"
		vars="${item//=*/}"
		eval $f
		echo "[${f#cfg.section.}]"
		for var in $vars; do
			echo $var=\"${!var}\"
		done
	done
}
				
function ConfigurationFile
{
	echo "CONFIG_FILE:"$CONFIG_FILE
	if [ -n "$CONFIG_FILE" ]; then
		SECTION=database
		PARAM=server
		server=$(awk -F "=" '/server/ {print $2}' $CONFIG_FILE)
		echo "Server:"$server
	fi
}

function CreateArchive
{
	# Archiving
	# $ZIP a $PACKNAME
	echo creating pack file $PACKNAME
	#if $PACK_WAREHOUSE directory does not exist create it
	
	if [ ! -d $PACK_WAREHOUSE ]; then
		mkdir -p $PACK_WAREHOUSE
	fi
	
	pushd $PACK_WAREHOUSE
	PACK_WAREHOUSE=`pwd`
	popd
	pushd $PACK_BUILD
	"$ZIP" a $PACK_WAREHOUSE/$PACKNAME -tzip
	popd
	errorlevel=$?
	if [ $errorlevel -ne 0 ]; then
		echo "build aborted: archiving failed"
		exit
	fi
	
	echo "build of pack succeeded"
}



function Encrypting  
{
	echo "Encrypting"
}

function Decrypting
{
	echo "Decrypting"
}

function Signature
{
	echo "Signature"
}

function main
{
	echo " "
	_CheckArgs ${@}
	echo "1"
	#_CheckTool zip
	
	local TIMESTAMP=`date "+%Y%m%d-%H%M%S"`
	local LREVISION=$(echo "$(LongHash)" |  tr '[:lower:]' '[:upper:]' )
	local SREVISION=$(echo "$(ShortHash)" |  tr '[:lower:]' '[:upper:]' )

	CreateVersionFile $MAJOR_VERSION $MINOR_VERSION $BUGFIX_VERSION
	echo "2"
	# parse the config file called 'myfile.ini', with the following
	# contents::
	#   [sec2]
	#   var2='something'
	cfg.parser 'parameters.ini'
	
	# enable section called 'sec2' (in the file [sec2]) for reading
	cfg.section.database
	
	# read the content of the variable called 'var2' (in the file
	# var2=XXX). If your var2 is an array, then you can use
	# ${var[index]}

	echo "Port:"$port
	
	if test $PLATFORM = "unknown";then
		case "$OSTYPE" in
			solaris*) PLATFORM="SOLARIS" ;;
			darwin*)  PLATFORM="OSX" ;; 
			linux*)   PLATFORM="LINUX" ;;
			bsd*)     PLATFORM="BSD" ;;
			msys*)    PLATFORM="WINDOWS" ;;
			cygwin*)  PLATFORM="ALSO WINDOWS" ;;
			*)        PLATFORM="unknown: $OSTYPE" ;;
		esac
	fi
	
	if [ -z "$PACK_TYPE" ];then
		echo "Package type undefined"
	elif test $PACK_TYPE = "DPKG";then
		echo "Delivery Package"
		PACK_NAME=DPKG-$PLATFORM-V$VERSION-$SREVISION-$TIMESTAMP.zip
	elif test $PACK_TYPE = "UPKG";then
		echo "Upgrade Package"
		PACK_NAME=UPKG-$PLATFORM-V$VERSION-$SREVISION-$TIMESTAMP.zip
	elif test $PACK_TYPE = "CPKG";then
		echo "Configuration Package"
		PACK_NAME=DCPKG-$PLATFORM-V$VERSION-$SREVISION-$TIMESTAMP.zip
	elif test $PACK_TYPE = "PPKG";then
		echo "Product Package"
		PACK_NAME=PPKG-$PLATFORM-V$VERSION-$SREVISION-$TIMESTAMP.zip
	elif test $PACK_TYPE = "SCP";then
		echo "Software Component Package"
		PACK_NAME=SCP-$PLATFORM-V$VERSION-$SREVISION-$TIMESTAMP.zip
		#PACK_NAME=SCP-$SOFTWARE_COMPONENT-V$VERSION-$SREVISION-$TIMESTAMP.zip
	elif test $PACK_TYPE = "BSP";then
		echo "Board Support Package"
		PACK_NAME=BSP-$PLATFORM-V$VERSION-$SREVISION-$TIMESTAMP.zip
	else
		echo "Package type Unknown"
	fi
	
	echo "PACK NAME:"$PACK_NAME
	echo "PLATFORM:"$PLATFORM
	echo "PACK NAME:"$VERSION
	echo "REVISION:"$SREVISION
	echo "REVISION:"$LREVISION
	
	echo " "
}

if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
	main ${@}
fi