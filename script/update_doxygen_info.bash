#!/usr/bin/env bash
# petit script pour MAJ le fichier Doxygen avec les infos de versions du programme
readonly DOXYGEN_FILE="doc/TlcFicVADCB.doxy"
#readonly VERSION_FILE="generated/Version.h"
readonly INFO_FILE="CMakeLists.txt"

PROGRAM_NAME=''
PROGRAM_VERSION=''
GIT_BRANCH=''
GIT_LAST_COMMIT=''


read_version_info(){
  info_file=$(<$INFO_FILE)
  # on extrait entre "project(" et "VERSION"
  PROGRAM_NAME=$(echo "$info_file" | grep 'project(' | grep -o -P '(?<=project\().*(?=VERSION)'|| echo "failed_to_extract")
  # on extrait entre "VERSION" et ")"
  PROGRAM_VERSION=$(echo "$info_file" | grep 'project(' | grep -o -P '(?<=VERSION ).*(?=\))' || echo "failed_to_extract")
}

get_git_current_branch(){
  GIT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
  echo "$GIT_BRANCH"
}

get_git_last_commit(){
  GIT_LAST_COMMIT=$(git rev-parse HEAD)
  echo "$GIT_LAST_COMMIT"
}

read_version_info
get_git_current_branch
get_git_last_commit

echo "$PROGRAM_NAME"
echo "$PROGRAM_VERSION"

#replace some variables in doxygen document
sed -i "/PROJECT_NAME.*=/s/^.*$/PROJECT_NAME = $PROGRAM_NAME/" $DOXYGEN_FILE
sed -i "/PROJECT_NUMBER.*=/s/^.*$/PROJECT_NUMBER = $PROGRAM_VERSION/" $DOXYGEN_FILE
sed -i "/PROJECT_BRIEF.*=/s/^.*$/PROJECT_BRIEF = \"Program used for ecom 'CB2A-like File' settlements (TLC) (GIT : branch=$GIT_BRANCH | last_commit=$GIT_LAST_COMMIT)\"/" $DOXYGEN_FILE
