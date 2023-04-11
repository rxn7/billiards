#!/usr/bin/env bash

target="linux"
procedure="all"

while getopts "t:p" arg; do
	case $arg in
		t) target=$OPTARG;;
		p) procedure=$OPTARG;;
	esac
done

makefile="make/$target.mk"

echo "Building using makefile $makefile..." 

make -f "$makefile" "$procedure"
