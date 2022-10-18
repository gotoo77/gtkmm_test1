#!/usr/bin/env bash
# script to generate some basic git info to be used and displayed in program
readonly TRUE=0
readonly SCRIPT_NAME=$(basename "$0")
#readonly SCRIPT_DIR=$(dirname "$0")
readonly SCRIPT_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
echo "SCRIPT_DIR=$SCRIPT_DIR"
readonly ROOT_DIR="$SCRIPT_DIR/../.."
echo "ROOT_DIR=$ROOT_DIR"

function bool () {
  (( $1 == TRUE ))
}

function not () {
  (( $1 != TRUE ))
}

function pout () {
  echo "[${SCRIPT_NAME}] $*" >&1
}

function perr () {
  echo "${SCRIPT_NAME} -?> $*" >&2
}


get_project_git_dependencies(){
  local DEP_DIR="$ROOT_DIR/dependencies"
  arrDepFound=($(ls "$DEP_DIR"))
  local depbranch
  local depcommit
  local dep_dataline
  local res
  for depname in "${arrDepFound[@]}"
  do :
      if depbranch=$(cd "$DEP_DIR/$depname" && git rev-parse --abbrev-ref HEAD); then
         depbranch=$depbranch
      else
         depbranch="failed_to_retrieve"
      fi

      if depcommit=$(cd "$DEP_DIR/$depname" && git rev-parse HEAD); then
         depcommit=$depcommit
      else
         depcommit="failed_to_retrieve"
      fi
      res=$(printf '{"%s","%s","%s"},\n' "$depname" "$depbranch" "$depcommit")
      dep_dataline="$dep_dataline $res"
      depname=""
  done
  echo "$dep_dataline"
}

readonly gen_header_file="$SCRIPT_DIR/../../generated/git_info.h"
readonly tmp_header_file="$SCRIPT_DIR/../../generated/git_info.previous.h"
readonly template_file="$SCRIPT_DIR/../template/git_info_template.h"

if [ -f "${gen_header_file}" ]; then
  pout "${gen_header_file} exists. "
  #  create copy of existing
  cp "${gen_header_file}" "${tmp_header_file}"
else
  pout "${gen_header_file} did not exist."
  cp "${template_file}" "${gen_header_file}"
fi

DATE_GEN=$(date +%d-%m-%Y)
WHO_GEN=$(whoami)
PROGNAME="TlcFicVADCB"
REPO_PROGNAME="tlc-tlcficvadcb"
REPO_URL="\"https:\/\/bitbucket.verifone.com:8443\/projects\/GTOEM\/repos\/\""

# get git infos using git commands
GIT_COMMIT=$(git rev-parse HEAD)
GIT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
PROG_GIT_INFO=$(printf '{"%s","%s","%s"}' "$REPO_PROGNAME" "$GIT_BRANCH" "$GIT_COMMIT")
DEP_GIT_INFO=$(get_project_git_dependencies)  || exit 1
# replace all slash '/'
DEP_GIT_INFO=${DEP_GIT_INFO////|}

pout "PROGNAME=$PROGNAME"
pout "REPO_PROGNAME=$REPO_PROGNAME"
pout "REPO_URL=$REPO_URL"
pout "DATE_GEN=$DATE_GEN"
pout "WHO_GEN=$WHO_GEN"
pout "GIT_COMMIT=$GIT_COMMIT"
pout "GIT_BRANCH=$GIT_BRANCH"
pout "PROG_GIT_INFO=$PROG_GIT_INFO"
pout "DEP_GIT_INFO (after replacement of '/' by '|'=$DEP_GIT_INFO"

# copy template file then do replacements using sed
cp "${template_file}" "${tmp_header_file}"
# replacements
sed -i "s/%GIT_COMMIT_INFO%/$GIT_COMMIT/" "${tmp_header_file}" || exit 2
sed -i "s/%GIT_BRANCH_INFO%/$GIT_BRANCH/" "${tmp_header_file}" || exit 2
sed -i "s/%WHO_GEN%/$WHO_GEN/" "${tmp_header_file}" || exit 2
sed -i "s/%DATE_GEN%/$DATE_GEN/" "${tmp_header_file}" || exit 2
sed -i "s/%PROGNAME%/$PROGNAME/" "${tmp_header_file}" || exit 2
sed -i "s/%REPO_URL%/$REPO_URL/" "${tmp_header_file}" || exit 2
sed -i "s/%PROG_GIT_INFO%/$PROG_GIT_INFO/" "${tmp_header_file}" || exit 2
sed -i "s/%DEP_GIT_INFO%/$DEP_GIT_INFO/" "${tmp_header_file}" || exit 2
#sed -i "s/git\/template\/git_info_template.h/generated\/git_info.h/" "${tmp_header_file}" || exit 2

#diff between previous and current ?
diff=$(diff "${gen_header_file}" "${tmp_header_file}")
echo "$diff"
if [ "$diff" != "" ];then
    pout "GIT INFO modified since last generation -> updating ${tmp_header_file}"
    mv  "${tmp_header_file}" "${gen_header_file}"
else
    pout "GIT INFO has not been modified."
fi

##reformat for indent the generated file
#clang-format "${gen_header_file}" > previous_gen_header_filefile && mv previous_gen_header_filefile "${gen_header_file}"  || exit 3

exit 0
