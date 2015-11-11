#!/bin/bash
# SHANNON ARCHER | SJA998
#read in command
com=$1

echo "The current path to the command $com is: "
echo `which $com`

echo "Other versions of the command $com are: "
ls -1 /*/$com | grep $com$ 
ls -1 /*/*/$com | grep $com$ 
ls -1 /*/*/*/$com | grep $com$ 
