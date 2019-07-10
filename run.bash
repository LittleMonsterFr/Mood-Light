#!/bin/bash

#-------------------------------------------------------------------------------
# Bash script handling the compilation and the copy of the binaries and files
# which are destined to be on the Raspberry Pi.
#-------------------------------------------------------------------------------

function my_echo
{
    SUFFIX="\e[0m"

    case $1 in

    "blue")
        PREFIX="\e[0;34m"
        ;;

    "yellow")
        PREFIX="\e[0;33m"
        ;;

    "red")
        PREFIX="\e[0;31m"
        ;;

    "green")
        PREFIX="\e[0;32m"
        ;;

    "orange")
        PREFIX="\e[1;31m"
        ;;

    "purple")
        PREFIX="\e[1;35m"
        ;;
    *)
        my_echo red "my_echo: Wrong color code. Got \"$1\"."
        my_echo red "         Values allowed are : \"blue\", \"yellow\", \"red\", \"green\", \"orange\", \"purple\"."
        exit 6
    esac

    echo -e "$PREFIX$2$SUFFIX"
}

function usage
{
    NAME=$(basename $0)
    my_echo yellow "$NAME : Custom shell script running the cross compilation toolchain for the Raspberry Pi."
    echo ""
    my_echo yellow "Usage  : $NAME [-h] [-v level] [-c] [-f] [-j jobs] [-d] [-z]"
    my_echo yellow "    -h : Display this help"
    my_echo yellow "    -v : Compile in verbose mode with the given level. Each level implies all the lower levels."
    my_echo yellow "         1 : Display the output of the cmake invocation."
    my_echo yellow "         2 : Display the output of the make invocation."
    my_echo yellow "         3 : Display the verbose output of the make invocation."
    my_echo yellow "    -c : Copy the executable produced by the build to the Raspberry Pi."
    my_echo yellow "    -f : Run the script from scratch erasing the build folder."
    my_echo yellow "    -j : Compiles with the strictly positive given number of jobs. Default is 1."
    my_echo yellow "    -d : Compiles with debug flags."
    my_echo yellow "    -z : Bundles the files in a zip."
}

TOOLCHAIN_FILE="toolchain.cmake"
ERR=0
VERBOSE=""
VERBOSE_LEVEL=0
DEBUG=""
ZIP=false
COPY_TO_PI=false
CMAKE_OUT="cmake_out"
CMAKE_ERR="cmake_err"
MAKE_OUT="make_out"
MAKE_ERR="make_err"
PING_ERR="ping_err"
SCP_ERR="scp_err"
FULL=false
JOBS_LEVEL=1
DEST_FOLDER="mood-light"
RPI_HOSTNAME="raspberrypi.local"

while getopts ":hv:cfj:dz" opt; do
    case "$opt" in
    \?)
        my_echo red "Unrecognised option : \"$OPTARG\""
        usage
        exit 1
        ;;
    h)
        usage
        exit 1
        ;;
    v)
        VERBOSE_LEVEL=$OPTARG
        if [[ ${VERBOSE_LEVEL} -gt 3 ]] || [[ ${VERBOSE_LEVEL} -lt 0 ]]; then
            my_echo red "Unhandled verbose level."
            usage
            exit 1
        fi
        if [[ ${VERBOSE_LEVEL} -ge 3 ]]; then
            VERBOSE="-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON"
        fi
        ;;
    c)
        COPY_TO_PI=true
        ;;
    f)
        FULL=true
        ;;
    j)
        JOBS_LEVEL=$OPTARG
        if [[ ${JOBS_LEVEL} -le 0 ]]; then
            my_echo red "Cannot give a negative or null jobs number."
            usage
            exit 1
        fi
        ;;
    d)
        DEBUG="-DDEBUG:BOOL=ON"
        ;;
    z)
        ZIP=true
        ;;
    :)
        if [[ $OPTARG == "v" ]]; then
            my_echo red "Missing verbose level."
        elif [[ $OPTARG == "j" ]]; then
            my_echo red "Missing jobs number."
        fi
        usage
        exit 1
        ;;
    esac
done

if [[ ${FULL} = true ]]; then
    if [[ -d  "build/" ]]; then
      my_echo blue "Former build folder present.Deleting...\n"
      rm -rf build/
    fi
fi

if [[ ! -d  "build/" ]]; then
    my_echo blue "Creating build folder...\n"
    mkdir build/
fi

my_echo blue "Entering in build folder...\n"
cd build/

my_echo blue "Generating makefiles...\n"
if [[ ${VERBOSE_LEVEL} -ge 1 ]]; then
    cmake -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_FILE} ${DEBUG} ${VERBOSE} .. > >(tee ${CMAKE_OUT}) 2> >(tee ${CMAKE_ERR} >&2)
    ERR=${PIPESTATUS[0]}
    echo ""
else
    cmake -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_FILE} ${DEBUG} ${VERBOSE} .. > ${CMAKE_OUT} 2> ${CMAKE_ERR}
    ERR=$?
fi

if [[ ${ERR} -eq 0 ]]; then
    my_echo blue "Compiling...\n"

    if [[ ${VERBOSE_LEVEL} -ge 2 ]]; then
        script -q -e -c "make -j${JOBS_LEVEL} 2> >(tee ${MAKE_ERR})" > >(tee ${MAKE_OUT}) /dev/null
        ERR=${PIPESTATUS[0]}
        echo""
    else
        make -j${JOBS_LEVEL} > ${MAKE_OUT} 2> ${MAKE_ERR}
        ERR=${PIPESTATUS[0]}
    fi
else
    my_echo red "Project configuration failed. Aborting...\n"
    if [[ ${VERBOSE_LEVEL} -eq 0 ]]; then
        my_echo red "The following output has been caught :\n"
        my_echo red "$(cat ${CMAKE_ERR})"
    fi
    exit 2
fi

if [[ ${ERR} -eq 0 ]]; then
    my_echo green "Compilation succeeded !\n"
else
    my_echo red "Compilation failed. Aborting...\n"
    if [[ ${VERBOSE_LEVEL} -eq 0 ]]; then
        my_echo red "The following output has been caught :\n"
        my_echo red "$(cat ${MAKE_ERR})"
    fi
    exit 3
fi

if [[ ${ZIP} == "true" ]]; then

    mkdir Mood-Light
    cp bin/mood-light ../auxiliary/patterns.json Mood-Light
    zip -r ../Mood-Light.zip Mood-Light
    ERR=$?

    if [[ ${ERR} -eq 0 ]]; then

        my_echo green "Mood-Light zip created.\n"

    else
        my_echo red "Couldn't zip files.\n"
        exit 4
    fi
fi

if [[ ${COPY_TO_PI} == "true" ]]; then

    # Copy the binaries
    scp -rp bin ../auxiliary/ pi@${RPI_HOSTNAME}:${DEST_FOLDER}/ > /dev/null 2> ${SCP_ERR}
    ERR=$?

    if [[ ${ERR} -eq 0 ]]; then

        my_echo green "Files successfully copied to the Raspberry Pi in /home/${DEST_FOLDER} folder!\n"

    else
        my_echo red "Raspberry Pi not found or scp error. Aborting...\n"
        my_echo red "The following output has been caught :\n"
        my_echo red "$(cat ${SCP_ERR})"
        exit 5
    fi
fi
