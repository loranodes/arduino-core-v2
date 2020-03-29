#!/bin/bash

if [ -z "$1" ]
then
	echo "No version number supplied"
	exit
fi

rm ./*.tar.bz2

tar -jcvf microNodeV2-$1.tar.bz2 microNode/

a=$(openssl dgst -sha256 microNodeV2-$1.tar.bz2 | cut -d ' ' -f 2)

echo -e "\nDone\n"
echo -e "version number = $1"
echo -e "SHA256 sum = $a"

echo -e "\nPlease run: \n"
echo -e "python index.py $1 $a"
