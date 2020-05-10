#! /bin/sh

is_bashv4() {
	if [ -n "$BASH_VERSINFO" ]
	then
		if [ $BASH_VERSINFO -lt 4 ]
		then
			return 1
		fi
		
		return 0
	fi

	return 1
}

is_zsh() {
	test -n "${ZSH_VERSION}"
}

if [ -z "${shell_name}" ]; then
	echo '${SHELL##*/} is null' >&2
elif ! is_zsh && ! is_bashv4 ; then
	echo "Only supported for bash-v4+ and zsh" >&2
	exit 1
fi

actions_for_zsh() {
	QUICKTEST_PATH="$( dirname "$0:A" )"
	
	echo "${QUICKTEST_PATH}/to_source.sh" >> "$HOME/.zlogin"
	echo "${QUICKTEST_PATH}/to_source.sh" >> "$HOME/.zshrc"
}

actions_for_bash() {
	QUICKTEST_PATH="$( cd "$( dirname "${BASH_SOURCE}" )" && pwd )"
	
	echo "${QUICKTEST_PATH}/to_source.sh" >> "$HOME/.bashrc"
	echo "${QUICKTEST_PATH}/to_source.sh" >> "$HOME/.bash_profile"
}

if is_bashv4; then
	actions_for_bash
elif is_zsh ; then
	actions_for_zsh
fi
