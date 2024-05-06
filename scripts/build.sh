#!/usr/bin/env bash
#SOURCE_DIR=$(realpath $(dirname "${0}"))
SOURCE_DIR=..
BUILD_TYPE='Debug'
#JOBS="$(grep -c processor /proc/cpuinfo)"
JOBS="$(sysctl -n hw.ncpu)"
MAKE='make'

function die()
{
    echo "ERROR: ${@}" 1>&2
    exit 1
}

function Usage()
{
    echo 'Usage:'
    echo "${0} -h"
    echo "${0} -p PLATFORM [-i INSTALL_PATH] [-t TOOLCHAIN] [-j JOBS] [-b BUILD_DIR] [-c] [-s] [-r]"
    
    echo
    echo 'Details:'
    echo '-h              - print this help'
    
    echo
    echo 'Example:'
    echo "${0} -p coffee             - build coffee platform agents and put results to default location '${INSTALL}/coffee'"
}

function CheckArguments()
{
    # verify for commands
    hash realpath &>/dev/null || die 'Please install realpath by this command "sudo apt-get install realpath"'
    
    USER_INSTALL='x'
    
    while getopts 'p:i:t:j:b:csrhmyn' opt; do
        case ${opt} in
            p)
                PLATFORM=${OPTARG};;
            i)
                USER_INSTALL=${OPTARG};;
            t)
                TOOLCHAIN=${OPTARG};;
            j)
                JOBS=${OPTARG};;
            b)
                BUILD_DIR=${OPTARG};;
            c)
                CLEAN=true;;
            s)
                STUB='stub';;
            r)
                BUILD_TYPE='Release';;
            m)
                BUILD_TYPE='MinSizeRel';;
            \?)
                Usage
                exit 1;;
            h)
                Usage
                exit 0;;
            y)
                YOCTO='recipe';;
            n)
                BUILD_UI=false;;
        esac
    done
    
    # verify platform name
    case ${PLATFORM} in
        windows);;
        linux);;
        macosx);;
        nucleo);;
        raspberry);;
        *)
            Usage
            die "Invalid platform '${PLATFORM}'";;
    esac
    
    # split fake roots?
    if [ 'x' = "${USER_INSTALL}" ]; then
        INSTALL="${INSTALL}/${PLATFORM}"
    else
        INSTALL="${USER_INSTALL}"
    fi
    
    # verify install path
    mkdir -p "${INSTALL}" || die "Verify install path '${INSTALL}'"
    INSTALL=$(realpath ${INSTALL})
    
    # verify toolchain
    
    if [ ! -f "${SOURCE_DIR}/cmake/${TOOLCHAIN}.cmake" ]; then
        die "Toolchain '${TOOLCHAIN}' is not supported!"
    fi
}

function PreBuild()
{
    echo Pre-Build: `date`
    # clean build artifacts, if needed
    if ${CLEAN}; then
        echo "Clean build directory '${BUILD_DIR}/${PLATFORM}'"
        rm -rf "${BUILD_DIR}/${PLATFORM}" || die
    fi
    
    # create build directory
    mkdir -p "${BUILD_DIR}/${PLATFORM}" || die
    
    # configure build by cmake
    #cmake -DCMAKE_TOOLCHAIN_FILE:PATH="${SOURCE_DIR}/cmake/${TOOLCHAIN}_toolchain.cmake" \
    #-DATLAS_CONFIGURATION:STRING="${PLATFORM} ${STUB} ${YOCTO}" \
    #-DBUILD_UI:BOOL=${BUILD_UI} \
    #-DCMAKE_BUILD_TYPE:STRING="${BUILD_TYPE}" \
    #-DCMAKE_INSTALL_PREFIX:PATH="${INSTALL}" \
    #-DCMAKE_INSTALL_BINDIR:PATH='usr/bin' \
    #-DCMAKE_INSTALL_SBINDIR:PATH='usr/sbin' \
    #-DCMAKE_INSTALL_LIBEXECDIR:PATH='usr/libexec' \
    #-DCMAKE_INSTALL_SYSCONFDIR:PATH='etc' \
    #-DCMAKE_INSTALL_SHAREDSTATEDIR:PATH='usr/share' \
    #-DCMAKE_INSTALL_LOCALSTATEDIR:PATH='var' \
    #-DCMAKE_INSTALL_LIBDIR:PATH='usr/lib' \
    #-DCMAKE_INSTALL_INCLUDEDIR:PATH='usr/include' \
    #-DCMAKE_INSTALL_DATAROOTDIR:PATH='usr/share' \
    #-B"${BUILD_DIR}/${PLATFORM}" \
    #-H"${SOURCE_DIR}"
    cmake -S ${SOURCE_DIR}\
    -B"${BUILD_DIR}/${PLATFORM}"\
    -DCMAKE_INSTALL_PREFIX:PATH="${INSTALL}" \
    -DCMAKE_BUILD_TYPE:STRING="${BUILD_TYPE}" 
}

function Build()
{
    echo Build: `date`
    # force rebuild of installed projects
    ${MAKE} -C"${BUILD_DIR}/${PLATFORM}" clean || die
        
    # build projects
    ${MAKE} -C"${BUILD_DIR}/${PLATFORM}" -j$((${JOBS}+1)) -l${JOBS}|| die
}

function PostBuild()
{
    echo Post-Build: `date`
    echo " "
    #cmake --install "${BUILD_DIR}/${PLATFORM}" --prefix ${INSTALL}
    #cmake --install "${BUILD_DIR}/${PLATFORM}" --prefix "/Users/sgabre/git/depository"
}

function main()
{
    CheckArguments ${@}
    
    PreBuild
    Build
    PostBuild
}

if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
    main ${@}
fi