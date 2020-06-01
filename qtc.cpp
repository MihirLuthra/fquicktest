#include <fstream>
#include <unordered_map>

#include "qtc.hpp"

const char block_start = '{', block_end = '}', ignore_next_char = '\\';

static inline bool tab_or_space(char c)
{
	return (c == ' ' || c == '\t');
}

void qtc::ConfigFile::open_file()
{
	if (!this->file_reader.is_open()) {
		this->file_reader.open(this->file_name);
		if (!this->file_reader) {
			throw qtc::FileOpenFailed(file_name);
		}
	}
}

void qtc::ConfigFile::print_key_val(std::ostream &out, std::string key, std::string value)
{
	out << key << " = {\n";
	out << "\t" << value << "\n";
	out << "}\n";
}

/*
 * This parses outside blocks i.e. {}.
 * When it finds a key, it goes inside its block
 * and returns the key name.
 * Further in_block_parse() should be called to go
 * through the block.
 *
 * This function checks the correctness of the file
 * until the point it parses and throws exceptions
 * accordingly.
 *
 * possible exceptions:
 * 	BlockWithoutAKey
 * 	EqualToWithoutAKey
 * 	InvalidCharacter
 * 	MissingEqualTo
 * 	TrailingCharsAfterEqualTo
 *
 * retval:
 *	It returns the next key found.
 *	If the key is empty, means there is
 *	no data further in file and in_block_parse()
 *	should NOT be called in such a case.
 */
std::string qtc::ConfigFile::out_block_parse()
{
	bool next_is_block_start = false;
	bool next_is_equal_to = false;
	unsigned char ch;
	std::string key;

	file_reader >> std::noskipws;

	auto is_char_invalid = [](char c) -> bool {
		return (!std::isalnum(c) && c != '_' && c != '-');
	};

	while (file_reader >> ch) {

		if (ch == '\n') err_line_no++;

		if (ch == block_start) {
			if (!next_is_block_start) {
				if (!key.empty()) {
					throw qtc::MissingEqualTo();
				} else {
					throw qtc::BlockWithoutAKey();
				}
			}

			next_is_block_start = false;

			return key;
		} else if (ch == '=') {
			next_is_equal_to = false;
			next_is_block_start = true;

			if (key.empty()) {
				throw qtc::EqualToWithoutAKey();
			}
		} else if (next_is_equal_to && !tab_or_space(ch)) {
			throw qtc::MissingEqualTo();
		} else if (next_is_block_start && !std::isspace(ch)) {
			// this occurs after =
			throw qtc::TrailingCharsAfterEqualTo();
		} else if (!key.empty() && tab_or_space(ch) && !next_is_block_start) {
			// This cond is if a tab/space is used after the key
			next_is_equal_to = true;
		} else if (std::isspace(ch)) {
			continue;
		} else if (is_char_invalid(ch)) {
			// chars except 0-9, A-Z, a-z, _ and - are invalid.
			throw qtc::InvalidCharacter(ch);
		} else {
			// this is key's char, add it.
			key += ch;
		}
	}

	if (next_is_block_start) {
		throw qtc::NoValueFoundForKey(key);
	} else if (next_is_equal_to || !key.empty()) {
		throw qtc::MissingEqualTo();
	}

	return key;
}

/*
 * This parses inside blocks i.e. {}.
 * It should be called after out_block_parse().
 * It assumes being inside a block and goes until
 * a closing } is found.
 *
 * possible exceptions:
 * 	UnterminatedBlock
 *
 * retval:
 *  returns the contents inside {}
 */
std::string qtc::ConfigFile::in_block_parse()
{
	bool ignore_next = false;
	unsigned char ch;
	std::string value;

	file_reader >> std::noskipws;

	while (file_reader >> ch) {
		if (ch == '\n') err_line_no++;
		if (!ignore_next) {
			if (ch == block_end) {
				return value;
			} else if (ch == ignore_next_char) {
				ignore_next = true;
			}	

			value += ch;
		} else {
			ignore_next = false;
		}
	}

	throw qtc::UnterminatedBlock();
}


/*
 * This works like in_block_parse() but
 * returns the value inside block with
 * leading and trailing tabs/spaces removed from every line.
 * Blank lines are also removed.
 *
 * possible exceptions:
 * 	UnterminatedBlock
 *
 * retval:
 * 	returns the value inside the block
 * 	with leading and trailing spaces
 * 	removed from each line of value.
 */
std::string qtc::ConfigFile::in_block_parse_and_strip_blanks()
{
	bool ignore_next = false;
	bool skip_leading_spaces = true;
	std::string spaces, value;
	char ch;

	file_reader >> std::noskipws;

	while (file_reader >> ch) {
		if (ch == '\n') err_line_no++;
		if (!ignore_next) {
			if (ch == block_end) {
				return value;
			} else if (ch == ignore_next_char) {
				ignore_next = true;
				continue;
			} else if (ch == '\n') {
				if (skip_leading_spaces) {
					// blank line
					continue;
				}

				skip_leading_spaces = true;
				spaces = "\n";
				continue;
			}
		} else {
			ignore_next = false;
		}

		if (skip_leading_spaces && tab_or_space(ch)) {
			continue;
		}

		skip_leading_spaces = false;

		if (std::isspace(ch)) {
			spaces += ch;
			continue;
		}

		value += spaces;
		spaces = "";
		value += ch;
	}
	
	throw qtc::UnterminatedBlock();
}

/*
 * It returns the value for the key param1.
 * It calls `in_block_parse_and_strip_blanks()` to get key's
 * value. Read the desc of that func.
 *
 * possible exceptions:
 *	BlockWithoutAKey
 *	EqualToWithoutAKey
 *	FileOpenFailed
 *	InvalidCharacter
 *	MissingEqualTo
 *	TrailingCharsAfterEqualTo
 *	UnterminatedBlock 
 *
 * param1:
 * 	key whose value should be returned.
 *
 * retval:
 * 	value for key param1.
 */
std::string qtc::ConfigFile::get_value_for_key(std::string key)
{
	bool key_found = false;
	std::string cur_key;

	try {
		open_file();
	} catch(...) {
		throw;
	}

	// necessay cuz if some other func calls out_block_parse()
	// it may leave file_reader to EOF
	file_reader.seekg(0, std::ios::beg);
	err_line_no = 1;

	try {
		while (file_reader.peek() != EOF) {
			cur_key = out_block_parse();
			key_found = key == cur_key ? true : false;

			if (cur_key.empty()) {
				// no more keys left to read
				break;
			}
			if (!key_found) {
				in_block_parse();
			} else {
				return in_block_parse_and_strip_blanks();
			}
		}
	} catch (...) {
		throw;
	}

	throw qtc::KeyNotFound(key);
}

void qtc::ConfigFile::set_value_for_key(std::string key, std::string new_value, std::ostream &out)
{
}

void qtc::ConfigFile::remove_key(std::string key, std::ostream &out)
{
}

std::unordered_map<std::string, std::string> qtc::ConfigFile::import_to_map()
{
	std::unordered_map<std::string, std::string> imported_map;
	return imported_map;
}
