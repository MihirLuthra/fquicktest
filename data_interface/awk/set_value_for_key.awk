#! /usr/bin/env awk

#
# This script expects the caller to set
# the following variables:
#  key
#  value
# The script goes through a quicktest user config file
# and prints every line. When it finds the key, it deletes
# it entirely and prints new key-val pair.
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
			key_val_printing_done="1"
			print_key_val_pair(key, value)
			continued_statements="0"
		}

	}

	#
	# If key is found we set continued_statements=1
	# and we skip all subsequent lines starting with >.
	#
	if ($1 == key && key_val_printing_done != "1") {
		original_line=$0
		continued_statements="1"
		next
	}

	if ($1 != "") {
		some_key_is_active="1"
	}

	print
}

END {
	if (key_val_printing_done != "1") {
		if (continued_statements != "1") {
			printf "\n"
		}
		print_key_val_pair(key, value)
	}
}
