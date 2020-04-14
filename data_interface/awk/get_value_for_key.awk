#! /usr/bin/env awk

#
# This script needs the following variables
# set from the caller:
#  key -> the key whose value is needed
#

#
# This script goes through quicktest type config file
# and prints value for the desired key.

function trim_leading_and_trailing_blanks(arg) {

	# trim leading spaces
	sub(/^ +/, "", arg)
	sub(/^\t+/, "", arg)

	# trim trailing spaces
	sub(/ +$/, "", arg)
	sub(/\t+$/, "", arg)

	#
	# Well I know [[:blank:[] exists but portability
	# issues.
	#


	return arg
}


function print_value(line) {

	line = trim_leading_and_trailing_blanks(line)

	if (line != "") {
		print line
		return 0
	}
	return 1
}

some_key_is_active == "1" && $1 == ">" {
	next
}

#
# MAIN
#
{
	some_key_is_active=""

	# This is when key is found and next line is
	# part of the key
	if (key_found == "1") {

		if ($1 == ">") {
			$1=""
			print_value($0)
		} else {
			exit 0
		}
	}

	# strip off comments
	gsub(/\/\/.*$/, "", $0)

	if ($1 == key) {
		$1=""
		print_value($0)
		key_found="1"
	}

	if ($1 != "") {
		some_key_is_active="1"
	}
}

END {
	if (key_found != "1") {
		exit 1
	}
}
