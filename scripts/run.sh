#!/usr/bin/env bash

platform="linux"
target="debug"

while getopts "rp:" arg; do
	case $arg in
		r) target="release";;
		p) platform=$OPTARG;;
	esac
done

pushd "bin/$platform/$target" > /dev/null 2>&1

case $platform in
	"linux") ./billiards;;
	"windows") wine ./billiards.exe;;
esac

popd > /dev/null 2>&1
