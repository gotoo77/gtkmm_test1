#!/usr/bin/env bash

readonly TRUE=0
readonly FALSE=1

readonly progname=$(basename $0)

function bool () {
    (( $1 == TRUE ))
}

function perr () {
    echo "${progname}: $@" >&2
}

arg_template=
arg_output=
while getopts ':t:o:' option; do
    case "$option" in
        t) arg_template="$OPTARG";;
        o) arg_output="$OPTARG";;
        :) perr "option '$OPTARG' requires on argument"; exit 1;;
        ?) perr "illegal option '$OPTARG'"; exit 1;;
        *) perr "unhandled option '$option'"; exit 1;;
    esac
done
readonly arg_template
readonly arg_output

function print_usage () {
    echo "USAGE: $progname -t <TEMPLATE> -o <OUTPUT>" &>2
}

if [[ ! -f "$arg_template" ]]; then
    perr "invalid template file $arg_template"
    exit 1
fi
if [[ -z "$arg_output" ]]; then
    perr "invalid output file"
    exit 1
fi

shift $((OPTIND - 1))

function parse_key_value () {
    if [[ "$1" =~ ^([A-Z0-9_]+)=(.*)$ ]]; then
        local key=${BASH_REMATCH[1]}
        local value=${BASH_REMATCH[2]}
        env_definitions[${key}]="${value}"
    else
        parse_key_value_errors=$TRUE
        perr "invalid key-value entry: $1"
    fi
}

declare -A env_definitions
parse_key_value_errors=$FALSE
while (($# > 0)); do
    parse_key_value "$1"
    shift
done

if bool ${parse_key_value_errors}; then
    perr "key-value errors are not ignored"
    exit 1
fi

count_definitions=${#env_definitions[@]}
if (( count_definitions == 0 )); then
    exit 0
fi

cp "$arg_template" "$arg_output"
for key in "${!env_definitions[@]}"; do
    replace -s "%${key}%" "${env_definitions[$key]}" -- "$arg_output"
done