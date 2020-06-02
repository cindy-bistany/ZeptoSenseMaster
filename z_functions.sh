#!/bin/bash
#
#     Admin functions for zeptive project.
#
# This file should reside in the project root directory, where, both
# the .git and .vscode project reside
#
# This file should be sourced, not executed.
# If you execute it, the functions get defined in the executed subshell but
# the expire when it exits.
# Sourcing the file defines them in the current shell.

# Since we require this file to be in the project directory, let us make
# the bad assumption that pwd is the project root ZDIR.
if [ "${ZDIR}" == "" ]; then
    ZDIR=${PWD}
fi

git config --global user.name "Bill White"
git config --global user.email william.a.white.iii@zeptive.com

#
# run the Particle "make clean:" script
#
function z_make_clean() {
    echo "z_make_clean does not work yet."
    #probably just delete all the target directories

    #none of this stuff works
    #/home/white3/.particle/toolchains/deviceOS/1.5.2/modules/argon/system-part1/makefile
    #/home/white3/.particle/toolchains/deviceOS/1.5.2/modules/argon/user-part/makefile
    #make -f '/home/white3/.particle/toolchains/buildscripts/1.9.2/Makefile' clean-all

}

#
#commit all changes in the project directory to the cloud repo
#
function z_backup() {
    if [ "${ZDIR}" == "" ]; then
	echo ZDIR is not set
	return
    fi
    
    (cd $ZDIR
     local answer
     echo -n "Enter the commit message: "
     read answer
     z_make_clean
     git add --verbose .
     #git diff --cached
     git commit --verbose -a -m "${answer}"
     git remote remove origin
     git remote --verbose add origin https://cindy-bistany:Zeptosense2019\!@github.com/cindy-bistany/ZeptoSenseMaster.git
     git push --verbose origin master
    )
}

