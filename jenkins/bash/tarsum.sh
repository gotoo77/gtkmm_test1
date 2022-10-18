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

function perr () {
    echo "${progname}: $@" >&2
}

arg_compress=
arg_input_file=
arg_output_file=
arg_hash=
while getopts ':jzZf:o:s:' option; do
    case "$option" in
        j) arg_compress=-j;;
        z) arg_compress=-z;;
        Z) arg_compress=-Z;;
        f) arg_input_file="$OPTARG";;
        o) arg_output_file="$OPTARG";;
        s) arg_hash="$OPTARG";;
        :) perr "option '$OPTARG' requires on argument"; exit 1;;
        ?) perr "illegal option '$OPTARG'"; exit 1;;
        *) perr "unhandled option '$option'"; exit 1;;
    esac
done
readonly arg_compress
readonly arg_input_file
readonly arg_output_file
readonly arg_hash

function print_usage () {
    echo "USAGE: $0 -j|-z|-Z -s <HASH> -f <INFILE> -o <OUTFILE>" >&2
    echo "HASH: sha1 sha224 sha256 sha384 sha512"
}

if [[ -z "$arg_hash" ]] || [[ -z "$arg_input_file" ]] || [[ -z "$arg_output_file" ]]; then
    print_usage
    exit 1
fi

if [[ -z "$arg_compress" ]]; then
    perr "compression type missing"
    print_usage
    exit 1
fi

cmd_sha1=/usr/bin/sha1sum
cmd_sha224=/usr/bin/sha224sum
cmd_sha256=/usr/bin/sha256sum
cmd_sha384=/usr/bin/sha384sum
cmd_sha512=/usr/bin/sha512sum
cmd_hash_var=cmd_${arg_hash}
readonly cmd_hash=${!cmd_hash_var}
if [[ ! -e "$cmd_hash" ]]; then
    echo "option '-s' requires a hash function. '$arg_hash' is unknown" >&2
    exit 1
fi

if [[ ! -f "$arg_input_file" ]]; then
    echo "option '-f' requires a gzip archive" >&2
    exit 1
fi

tar -tf "$arg_input_file" \
| grep -v -e '/$' \
| xargs -I% sh -c "tar $arg_compress -xf '$arg_input_file' --to-command=$cmd_hash % | awk '{print \$1 \"  %\"}'" \
> "$arg_output_file"