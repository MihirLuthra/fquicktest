#! /bin/sh

if [ -n "$__SOURCE_GUARD_CLIUM__" ] ; then
	return 0
else
	readonly __SOURCE_GUARD_CLIUM__="1"
fi


if [ -n "$BASH_VERSION" ] ; then
	TEMP_CLIUM_PATH="$( cd "$( dirname "$BASH_SOURCE" )" ; pwd )"
elif [ -n "$ZSH_VERSION" ] ; then
	TEMP_CLIUM_PATH=$( dirname "$0:A" )
else
	>&2 echo "Clium not supported as both \$BASH_VERSION and \$ZSH_VERSION are null"
	>&2 echo "Clium only works on zsh or bash."
	return 1
fi

if [ -z "$CLIUM_PATH" ] ; then
	export CLIUM_PATH=$TEMP_CLIUM_PATH
	PATH="$CLIUM_PATH/commands/user_commands:$PATH"
	PATH="$CLIUM_PATH/commands/program_commands:$PATH"
fi

if [ -z "$CLIUM_BASH" ] ; then
	export CLIUM_BASH="bash"
fi

readonly CLIUM_TTY_DATE="$( date "+%d%m%Y%H%M%S" )" ; export CLIUM_TTY_DATE
readonly CLIUM_TTY_NAME="$( basename "$(tty)" )" ; export CLIUM_TTY_NAME
readonly CLIUM_SOURCE_PATH="$CLIUM_PATH/src/source" ; export CLIUM_SOURCE_PATH
