#!/usr/bin/env bash

platform="linux"
release=false

while getopts "rp:" arg; do
	case $arg in
		p) platform=$OPTARG;;
		r) release=true;;
	esac
done

target=$([ "$release" == "true" ] && echo "release" || echo "debug")
makefile="make/$platform.mk"

printf "Building for %s:%s\n\n" "$platform" "$target"
make -f "$makefile" all "TARGET=$target"
