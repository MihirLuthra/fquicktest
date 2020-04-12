#! /usr/bin/env awk

#
# This script expects the caller to set
# the following variables:
#  key
#  value
#
# The script goes through a quicktest type config file
# and prints every line.
# When it finds the key, it goes till the end of key scope
# and appends the value by a ">".
#
# If key doesn't exist, it is created with the given value
#

function print_key_val_pair(key, val) {
	printf "%s\t%s\n", key, val
}

function append_val(val) {
	printf "\t> %s\n", val
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
			print
			next
		} else {
			appending_done="1"
			continued_statements="0"
			append_val(value)
		}

	}

	#
	# If key is found we set continued_statements=1
	# and we skip all subsequent lines starting with >.
	#
	if ($1 == key && appending_done != "1") {
		continued_statements="1"
	}

	if ($1 != "") {
		some_key_is_active="1"
	}

	print
}

END {
	# required if key was on the last line
	if (appending_done != "1") {
		if (continued_statements == "1") {
			append_val(value)
		} else {
			printf "\n"
			print_key_val_pair(key, value)
		}
	}
}
