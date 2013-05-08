#! /bin/bash

declare -a PASSWORDS=(\
	"We're not hit! We're not hit!" \
	"Where the hell do you think I'm going?" \
	"Shut up, shut up, shut up!" \
	"Oh! Elvis has left the building!" \
	"Where you at, huh? Huh? Where you at?" \
	"No, you did NOT shoot that green shit at me!" \
	"Didn't I promise you fireworks?" \
	"You really think you can do all that bullshit you just said?" \
	)

make -s clean generator

i=0
for p in "${PASSWORDS[@]}"; do
	password=`./generator "$p"`
	make -s password CFLAGS_PASSWORD="-DPASSWORD=\"$password\""
	binary=check_`md5sum password | cut -d' ' -f1`
	mv password "$binary"
	echo -e "$binary\t$p" >> list.txt
	if ! ./$binary "$p" || ./$binary x"$p"; then
		echo ERROR $binary
	fi
	echo -ne "INSERT INTO challenge_tests (id, sol_id, input, output) VALUES (%%%CHALLENGE_ID%%%, $i,\n  \"" >> list.sql
	base64 $binary >> list.sql
	echo -ne "\", \"$p\");\n" >> list.sql
	echo -ne "INSERT INTO challenge_solutions (id, sol_id, input, output) VALUES (%%%CHALLENGE_ID%%%, $i,\n  \"" >> list.sql
	base64 $binary >> list.sql
	echo -ne "\", \"$p\");\n" >> list.sql
	i=$((i+1))
done

