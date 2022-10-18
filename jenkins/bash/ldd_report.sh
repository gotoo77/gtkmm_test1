#!/usr/bin/env bash

readonly TRUE=0
readonly FALSE=1

readonly progname=$(basename $0)

function bool () {
    (( $1 == TRUE ))
}

function not () {
    (( $1 != TRUE ))
}

readonly report_script="$(dirname $0)/report.sh"
readonly template_file="$(dirname $0)/../template/ldd_report.html"

function perr () {
    echo "${progname}: $@" >&2
}

arg_artifact=
arg_directory=
arg_output=
while getopts ':a:d:o:' option; do
    case "$option" in
        a) arg_artifact="$OPTARG";;
        d) arg_directory="$OPTARG";;
        o) arg_output="$OPTARG";;
        :) perr "option '$OPTARG' requires on argument"; exit 1;;
        ?) perr "illegal option '$OPTARG'"; exit 1;;
        *) perr "unhandled option '$option'"; exit 1;;
    esac
done
if [[ -n "$arg_directory" ]]; then
    arg_directory="$arg_directory/"
fi
if [[ -n "$arg_output" ]]; then
    arg_output="$arg_output/"
fi
readonly arg_artifact
readonly arg_directory
readonly arg_output

    echo "arg_artifact=${arg_artifact}."
    echo "arg_directory=${arg_directory}. "
    echo "arg_output=${arg_output}. "

readonly artifact_file="${arg_directory}${arg_artifact}"
if [[ ! -f "$artifact_file" ]]; then
    perr "invalid artifact file '$artifact_file'"
    exit 1
fi

if [[ -n "${arg_output}" ]] && [[ ! -d "${arg_output}" ]]; then
    mkdir -p "${arg_output}"
    if not $?; then
        perr "unable to create output directory"
        exit 1
    fi
fi

readonly artifact_ldd="${arg_output}${arg_artifact}.ldd.txt"
readonly artifact_elfd="${arg_output}${arg_artifact}.elfd.txt"
readonly report_file="${arg_output}${arg_artifact}.ldd.html"

/usr/bin/readelf -d "$artifact_file" | grep --color=never -e 'NEEDED' -e 'RPATH' > "$artifact_elfd"
/usr/bin/ldd "$artifact_file" > "$artifact_ldd"

${report_script} -t "$template_file" -o "$report_file" -- \
 "ARTIFACT_NAME=${arg_artifact}" \
 "ARTIFACT_ELF_D=$(cat "$artifact_elfd")" \
 "ARTIFACT_LDD=$(cat "$artifact_ldd")"