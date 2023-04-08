#!/bin/bash

rm -rf shaders_out
mkdir shaders_out

FILES="./shaders/*.glsl"

for f in $FILES
do
	if [ -f "$f" ]
	then
		BASENAME=$(basename -- $f)
		OUTFILE="./shaders_out/$BASENAME"

		echo "Compiling file $BASENAME to $OUTFILE" 

		while read -r line
		do
			if [[ ! -n "$line" ]]; then
				continue
			fi

			if [[ "$line" =~ (/\*([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*\*+/)|(//.*) ]] ; then
				continue
			fi

			echo "\"$line\n\"" >> $OUTFILE
		done < "$f"
	fi
done
