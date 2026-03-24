#!/usr/bin/env bash

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo "Usage: $0 [OPTION]... [DIRECTORY]..."
    echo "Recursively count executable files in directories."
    echo ""
    echo "  -h, --help     display this help and exit"
    echo "  -v, --verbose  print each executable file found"
    exit 0
fi

verbose=0
if [ "$1" = "-v" ] || [ "$1" = "--verbose" ]; then
    verbose=1
    shift
fi

dir="${1:-.}"

if [ ! -d "$dir" ]; then
    echo "Error: '$dir' is not a directory"
    exit 1
fi

if [ $verbose -eq 1 ]; then
    find "$dir" -type f -executable 2>/dev/null
fi

count=$(find "$dir" -type f -executable 2>/dev/null | wc -l)

echo "Total executable files: $count"
