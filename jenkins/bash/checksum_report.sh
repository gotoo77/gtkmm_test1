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
readonly tarsum_script="$(dirname $0)/tarsum.sh"
readonly template_file="$(dirname $0)/../template/checksum_report.html"

function perr () {
    echo "${progname}: $@" >&2
}

arg_compress=
arg_directory=
arg_archive_name=
arg_hash_suffix=
while getopts ':jzZd:a:s:' option; do
    case "$option" in
        j) arg_compress=-j;;
        z) arg_compress=-z;;
        Z) arg_compress=-Z;;
        d) arg_directory="$OPTARG" ;;
        a) arg_archive_name="$OPTARG" ;;
        s) arg_hash_suffix="$OPTARG" ;;
        :) perr "option '$OPTARG' requires on argument"; exit 1;;
        ?) perr "illegal option '$OPTARG'"; exit 1;;
        *) perr "unhandled option '$option'"; exit 1;;
    esac
done
if [[ -n "$arg_directory" ]]; then
    arg_directory="$arg_directory/"
fi
readonly arg_compress
readonly arg_directory
readonly arg_archive_name
readonly arg_hash_suffix

function print_usage () {
    echo "USAGE: $0 -j|-z|-Z -s <HASH> [-d <DIRECTORY>] -a <ARCHIVE>" >&2
}

if [[ -z "$arg_archive_name" ]] || [[ -z "$arg_hash_suffix" ]]; then
    print_usage
    exit 1
fi

if [[ -z "$arg_compress" ]]; then
    perr "compression type missing"
    print_usage
    exit 1
fi

if [[ -n "$arg_directory" ]] && [[ ! -d "$arg_directory" ]]; then
    perr "option '-d' required a directory"
    exit 1
fi

readonly archive_file="${arg_directory}$arg_archive_name"
readonly archive_checksum_file="${arg_directory}$arg_archive_name.$arg_hash_suffix"
readonly content_checksum_file="${arg_directory}$arg_archive_name.files.$arg_hash_suffix"
readonly report_file="$archive_checksum_file.html"

if [[ ! -f "$archive_file" ]]; then
    perr "invalid file '$archive_checksum_file'"
    exit 1
fi
if [[ ! -f "$archive_checksum_file" ]]; then
    perr "invalid file '$archive_checksum_file'"
    exit 1
fi

${tarsum_script} ${arg_compress} -s ${arg_hash_suffix} -f "${archive_file}" -o "${content_checksum_file}"
if not $?; then
    perr "Error in detailed checksum generation."
    exit 1
fi

${report_script} -t "$template_file" -o "$report_file" -- \
 "ARCHIVE_NAME=$arg_archive_name" \
 "ARCHIVE_CHECKSUM=$(cat "$archive_checksum_file")" \
 "ARCHIVE_FILES_CHECKSUM=$(cat "$content_checksum_file")"