#!/bin/bash

if [ -e "$1" ]
    then
    for x in $(seq 1);
    do
        if "$x" % 2 == 0 
        then
            cat "$1" >&2
        else
            cat "$1" >&1
        fi
    done
fi