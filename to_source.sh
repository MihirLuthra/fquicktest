#! /bin/sh

if [ -n "${__SOURCE_GUARD_QUICKTEST__}" ] ; then
	return 0
else
	readonly __SOURCE_GUARD_QUICKTEST__="1"
fi


if [ -n "${BASH_VERSION}" ] ; then
	QPATH="$( cd "$( dirname "${BASH_SOURCE}" )" ; pwd )"
elif [ -n "${ZSH_VERSION}" ] ; then
	QPATH=$( dirname "$0:A" )
fi


if [ -z "${QUICKTEST_PATH}" ] ; then
	export QUICKTEST_PATH="$QPATH"
	unset QPATH
	PATH="${QUICKTEST_PATH}/commands/user_commands:${PATH}"
	PATH="${QUICKTEST_PATH}/commands/program_commands:${PATH}"
fi

if [ -z "${QTBASH}" ] ; then
	export QTBASH="bash"
fi
