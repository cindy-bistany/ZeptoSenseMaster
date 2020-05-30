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
ZDIR=${PWD}

git config --global user.name "Bill White"
git config --global user.email william.a.white.iii@zeptive.com

#
#commit all changes in the project directory
#
function z_git_add_commit_all() {
    local answer
    (cd $ZDIR
     echo -n "Enter the commit message: "
     read answer
     git add .
     git diff --cached
     git commit -a -m "${answer}"
    )
}

