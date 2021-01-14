#ifndef __CLIUM_IDENTIFIER_HPP__
#define __CLIUM_IDENTIFIER_HPP__

#include <cctype>
#include <cstring>
#include <string>

/*
 * identifier validity is checked at various places,
 * like checked key in qtc file, checking qtc func name
 * etc.
 * This file defines the rules for its validity.
 */

namespace clium_identifier {
	bool is_invalid(char ch)
	{
		return (!std::isalnum(ch) && ch != '_' && ch != '-');
	}

	bool is_invalid(std::string str)
	{
		if (str.empty()) {
			return true;
		}

		for (auto &c : str) {
			if (is_invalid(c)) {
				return true;
			}
		}

		return false;
	}

	bool is_invalid(const char *str)
	{
		if (!std::strcmp(str, "")) {
			return true;
		}

		for (int i = 0 ; str[i] != '\0' ; ++i) {
			if (is_invalid(str[i])) {
				return true;
			}
		}

		return false;
	}

	bool is_valid(char ch)
	{
		return (std::isalnum(ch) || ch == '_' || ch == '-');
	}

	bool is_valid(std::string str)
	{
		if (str.empty()) {
			return false;
		}

		for (auto &c : str) {
			if (is_invalid(c)) {
				return false;
			}
		}

		return true;
	}

	bool is_valid(const char *str)
	{
		if (!std::strcmp(str, "")) {
			return false;
		}

		for (int i = 0 ; str[i] != '\0' ; ++i) {
			if (is_invalid(str[i])) {
				return false;
			}
		}

		return true;
	}
}

#endif /* __CLIUM_IDENTIFIER_HPP__ */
