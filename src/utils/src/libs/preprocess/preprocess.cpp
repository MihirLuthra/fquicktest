#include <iostream>
#include "preprocess.hpp"
#include <quicktest_identifier/quicktest_identifier.hpp>
#include <unordered_map>

namespace qi = quicktest_identifier;

const char block_start = '[', block_end = ']', function_block_start = '(', function_block_end = ')';
const char ignore_next_char = '\\';

std::string Preprocessor::parse_block(replace_map rmap)
{
	std::string replacee, args;

	char ch;

	replacee += '[';

	while (inp >> ch) {

		if (ch == block_end) {

			auto replacee_itr = rmap.find(replacee.substr(1));

			if (replacee_itr == rmap.end()) {
				replacee += ch;
				return replacee;
			}

			if (inp.peek() == function_block_start) {
				inp >> ch;
				args = parse_func_block(rmap);
			}
			return replacee_itr->second(args);
		}

		if (qi::is_invalid(ch)) {
			inp.unget();
			return replacee;
		}

		replacee += ch;
	}

	return replacee;
}

std::string Preprocessor::parse_func_block(replace_map rmap)
{
	std::string args;
	char ch;

	while (inp >> ch) {
		if (ch == function_block_end) {
			if (args.back() == ignore_next_char) {
				args.back() = ch;
				continue;
			} else {
				return args;
			}
		} else if (ch == block_start) {
			args += parse_block(rmap);
			continue;
		}

		args += ch;
	}

	throw UnterminatedFuncBlock();
}

void Preprocessor::preprocess(replace_map rmap)
{
	if (!inp) {
		throw InvalidInputStream();
	}

	if (!out) {
		throw InvalidOutputStream();
	}

	char ch;

	inp >> std::noskipws;

	try {
		while (inp >> ch) {
			if (ch == block_start) {
				out << parse_block(rmap);
			} else {
				out << ch;
			}
		}
	} catch (...) {
		throw;
	}
}
