#! /usr/bin/env awk

#
# This script expects the caller to set
# the following variables:
#  key
# The script goes through a quicktest user config file
# and prints every line. When it finds the key, it deletes
# it entirely.
#

function print_key_val_pair(key, val) {
	printf "%s\t%s\n", key, val
}

some_key_is_active == "1" && $1 == ">" {
	print
	next
}

#
# MAIN
#
{
	some_key_is_active=""

	# This is when key is found and next line is
	# part of the key
	if (continued_statements == "1") {

		if ($1 == ">") {
			next
		} else {
			key_removing_done="1"
			continued_statements="0"
		}

	}

	#
	# If key is found we set continued_statements=1
	# and we skip all subsequent lines starting with >.
	#
	if ($1 == key && key_removing_done != "1") {
		continued_statements="1"
		next
	}

	if ($1 != "") {
		some_key_is_active="1"
	}

	print
}

END {
	if (key_removing_done == "1") {
		exit 0
	}

	exit 1
}
