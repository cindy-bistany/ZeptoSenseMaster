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
# the risky assumption that pwd is the project root ZDIR.
if [ "${ZDIR}" == "" ]; then
    ZDIR=${PWD}
    echo "ZDIR = "${ZDIR}
fi


git config --global user.name "Bill White"
git config --global user.email william.a.white.iii@zeptive.com
GITREPO='https://cindy-bistany:ZeptoSense2019!@github.com/cindy-bistany/ZeptoSenseMaster.git'

function z_() { . ${ZDIR}/z_functions.sh; }	#reload this file
function z_rm~() { rm ${ZDIR}/*~ ${ZDIR}/src/*~; } #Delete emacs backup files


#
# run the Particle "make clean:" script
#
function z_make_clean() {
    zrm~
    for d in `ls -1 ${ZDIR}/target`; do
	(
	    cd ${ZDIR}/target/${d}
	    rm -rf *
	)
    done
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
     git remote --verbose add origin "${GITREPO}"
     git push --verbose origin master
    )
}

