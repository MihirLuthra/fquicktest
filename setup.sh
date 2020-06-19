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
	test -n "$ZSH_VERSION"
}

shell_name=${SHELL##*/}

if [ -z "$shell_name" ]; then
	echo '${SHELL##*/} is null' >&2
elif ! is_zsh && ! is_bashv4 ; then
	>&2 printf "%s\n"                                                    \
		"Only supported for bash-v4+ and zsh"                            \
		"Make sure you are sourcing the script and not executing it."

	# no foolproof way to detect in /bin/sh if script is sourced
	# so we first return and then exit.
	return 1 2> /dev/null
	exit 1
fi

actions_for_zsh() {
	QUICKTEST_PATH="$( dirname "$0:A" )"

	echo "source \"$QUICKTEST_PATH/to_source.sh\"" >> "$HOME/.zlogin"
	echo "source \"$QUICKTEST_PATH/to_source.sh\"" >> "$HOME/.zshrc"
}

actions_for_bash() {
	QUICKTEST_PATH="$( cd "$( dirname "$BASH_SOURCE" )" && pwd )"

	echo "source \"$QUICKTEST_PATH/to_source.sh\"" >> "$HOME/.bashrc"
	echo "source \"$QUICKTEST_PATH/to_source.sh\"" >> "$HOME/.bash_profile"
}

if is_bashv4; then
	if ! (return 0 2>/dev/null)
	then
		>&2 printf "%s\n"                \
		    "Script needs to be sourced" \
		    "usage: source $BASH_SOURCE"
		exit 1
	fi
	actions_for_bash
elif is_zsh ; then
	if ! [[ $ZSH_EVAL_CONTEXT =~ :file$ ]]
	then
		>&2 printf "%s\n"                \
		    "Script needs to be sourced" \
		    "usage: source ${(%):-%x}"
		exit 1
	fi
	actions_for_zsh
fi

install_qtc_vim_syntax_highlighting() {
	mkdir -p "$HOME/.vim/syntax" || return 1
	mkdir -p "$HOME/.vim/ftdetect" || return 1

	if [ -e "$HOME/.vim/syntax/qtc.vim" ]
	then
		return 1
	fi

	if [ -e "$HOME/.vim/ftdetect/qtc.vim" ]
	then
		return 1
	fi

	cp "$QUICKTEST_PATH/vim/syntax/qtc.vim" "$HOME/.vim/syntax/"
	cp "$QUICKTEST_PATH/vim/ftdetect/qtc.vim" "$HOME/.vim/ftdetect/"
}

install_qtc_vim_syntax_highlighting || return 1
