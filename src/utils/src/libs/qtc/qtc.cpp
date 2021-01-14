#include <fstream>
#include <regex>
#include <unordered_map>

#include "qtc.hpp"
#include <clium_identifier/clium_identifier.hpp>

namespace qi = clium_identifier;

bool tab_or_space(char ch)
{
	return (ch == '\t' || ch == ' ');
}

void qtc::ConfigFile::open_file()
{
	if (!file_reader.is_open()) {
		file_reader.open(file_name);
		if (!file_reader) {
			err_line_no = 0;
			throw qtc::FileOpenFailed(file_name);
		}
	}
}

void qtc::ConfigFile::print_key_val(std::ostream &out, std::string key, std::string value)
{
	std::string value_to_print = std::regex_replace(value, std::regex("\n"), "\n\t");
	out << key << " =\n";
	out << '\t' << value_to_print;
	out << std::endl;
}

/*
 * This parses the untabbed values and returns the key name.
 * Further in_block_parse() should be called to go
 * through the value of key.
 *
 * This function checks the correctness of the file
 * until the point it parses and throws exceptions
 * accordingly.
 *
 * possible exceptions:
 * 	EqualToWithoutAKey
 * 	InvalidCharacter
 * 	MissingEqualTo
 * 	NoValueFoundForKey
 * 	TrailingCharsAfterEqualTo
 * 	ValueWithoutAKey
 *
 * retval:
 *	It returns the next key found.
 *	If the key is empty, means there is
 *	no data further in file and in_block_parse()
 *	should NOT be called in such a case.
 */
std::string qtc::ConfigFile::out_block_parse()
{
	bool next_is_value_start = false;
	bool next_is_equal_to = false;
	unsigned char ch;
	std::string key;

	file_reader >> std::noskipws;

	while (file_reader >> ch) {

		if (ch == '\n') {

			if (next_is_value_start) {
				if (file_reader.peek() != '\t') {
					throw qtc::NoValueFoundForKey(key);
				}
				return key;
			} else if (!key.empty()) {
				throw qtc::MissingEqualTo();
			}

			err_line_no++;
		} else if (ch == '=') {
			next_is_equal_to = false;
			next_is_value_start = true;

			if (key.empty()) {
				throw qtc::EqualToWithoutAKey();
			}
		} else if (next_is_equal_to && !tab_or_space(ch)) {
			throw qtc::MissingEqualTo();
		} else if (next_is_value_start && !tab_or_space(ch)) {
			// this occurs after =
			throw qtc::TrailingCharsAfterEqualTo();
		} else if (!key.empty() && tab_or_space(ch) && !next_is_value_start) {
			// This cond is if a tab/space is used after the key
			next_is_equal_to = true;
		} else if (std::isspace(ch)) {
			continue;
		} else if (qi::is_invalid(ch)) {
			// chars except 0-9, A-Z, a-z, _ and - are invalid.
			throw qtc::InvalidCharacter(ch);
		} else {
			// this is key's char, add it.
			key += ch;
		}
	}

	if (next_is_value_start) {
		throw qtc::NoValueFoundForKey(key);
	} else if (next_is_equal_to || !key.empty()) {
		throw qtc::MissingEqualTo();
	}

	return key;
}

/*
 * This parses the tabbed values for a key.
 * Should strictly be called after out_block_parse() iff
 * key returned from it isn't empty.
 * It goes till EOF or the line not starting with tab or '+'.
 *
 * retval:
 *  returns the value
 */
std::string qtc::ConfigFile::in_block_parse()
{
	unsigned char ch;
	std::string value;

	file_reader >> std::noskipws;

	file_reader >> ch;

	while (file_reader >> ch) {
		if (ch == '\n') {
			char next = file_reader.peek();
			if (next == '\t') {
				value += ch; 
			} else if (next != '+') {
				return value;
			}
			err_line_no++;
			file_reader >> ch;
			continue;
		}
		value += ch;
	}

	return "";
}

/*
 * It returns the value for the key param1.
 * It calls `in_block_parse()` to get key's
 * value. Read the desc of that func.
 *
 * possible exceptions:
 * 	EqualToWithoutAKey
 * 	FileOpenFailed
 * 	InvalidCharacter
 * 	MissingEqualTo
 * 	NoValueFoundForKey
 * 	TrailingCharsAfterEqualTo
 * 	ValueWithoutAKey
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
				return in_block_parse();
			}
		}
	} catch (...) {
		throw;
	}

	err_line_no = 0;
	throw qtc::KeyNotFound(key);
}

/*
 * It sets the value for the key param1 to value param2.
 * It outputs the resulting file ouput to param3.
 *
 * param1:
 * 	key whose value needs to be changes
 *
 * param2:
 * 	new value for param1
 *
 * param3:
 * 	ostream to output the updated contents
 *
 * possible exceptions:
 * 	EqualToWithoutAKey
 * 	FileOpenFailed
 * 	InvalidCharacter
 * 	InvalidOutputStream
 * 	MissingEqualTo
 * 	NoValueFoundForKey
 * 	TrailingCharsAfterEqualTo
 * 	ValueWithoutAKey
 */
void qtc::ConfigFile::set_value_for_key(std::string key, std::string new_value, std::ostream &out)
{
	bool key_exists = false;

	try {
		open_file();
	} catch(...) {
		throw;
	}

	if (!out) {
		err_line_no = 0;
		throw qtc::InvalidOutputStream();
	}

	try {
		auto key_vals = import_to_map();

		for (auto &pair : key_vals) {
			if (pair.first == key) {
				print_key_val(out, key, new_value);
				key_exists = true;
			} else {
				print_key_val(out, pair.first, pair.second);
			}

			out << std::endl;
		}

		if (!key_exists) {
			print_key_val(out, key, new_value);
		}
	} catch (...) {
		throw;
	}

}

/*
 * It sets the value for the key param1 to value param2
 * in the same file.
 *
 * param1:
 * 	key whose value needs to be changes
 *
 * param2:
 * 	new value for param1
 *
 * possible exceptions:
 * 	EqualToWithoutAKey
 * 	FileOpenFailed
 * 	InvalidCharacter
 * 	MissingEqualTo
 * 	NoValueFoundForKey
 * 	TrailingCharsAfterEqualTo
 * 	ValueWithoutAKey
 */
void qtc::ConfigFile::set_value_for_key(std::string key, std::string new_value)
{
	bool key_exists = false;
	bool file_open_failed = false;
	std::unordered_map<std::string, std::string> key_vals;

	try {
		open_file();
	} catch(...) {
		file_open_failed = true;
	}

	try {
		if (!file_open_failed) {
			key_vals = import_to_map();
			file_reader.close();
		}

		std::ofstream out(file_name);

		if (!out) {
			err_line_no = 0;
			throw qtc::FileOpenFailed(file_name);
		}

		out.seekp(0, std::ios::beg);

		for (auto &pair : key_vals) {
			if (pair.first == key) {
				print_key_val(out, key, new_value);
				key_exists = true;
			} else {
				print_key_val(out, pair.first, pair.second);
			}

			out << std::endl;
		}

		if (!key_exists) {
			print_key_val(out, key, new_value);
		}
	} catch (...) {
		throw;
	}

}

/*
 * It removes the key param1.
 * It outputs the resulting file ouput to param2.
 *
 * param1:
 * 	key to remove
 *
 * param2:
 * 	ostream to output the updated contents
 *
 * possible exceptions:
 * 	EqualToWithoutAKey
 * 	FileOpenFailed
 * 	InvalidCharacter
 * 	InvalidOutputStream
 * 	MissingEqualTo
 * 	NoValueFoundForKey
 * 	TrailingCharsAfterEqualTo
 * 	ValueWithoutAKey
 */
void qtc::ConfigFile::remove_key(std::string key, std::ostream &out)
{
	try {
		open_file();
	} catch(...) {
		throw;
	}

	if (!out) {
		err_line_no = 0;
		throw qtc::InvalidOutputStream();
	}

	try {
		auto key_vals = import_to_map();

		for (auto &pair : key_vals) {
			if (pair.first != key) {
				print_key_val(out, pair.first, pair.second);
				out << std::endl;
			}
		}
	} catch (...) {
		throw;
	}
}

/*
 * It removes the key param1 in the same file itself.
 *
 * param1:
 * 	key to remove
 *
 * possible exceptions:
 * 	EqualToWithoutAKey
 * 	FileOpenFailed
 * 	InvalidCharacter
 * 	MissingEqualTo
 * 	NoValueFoundForKey
 * 	TrailingCharsAfterEqualTo
 * 	ValueWithoutAKey
 */
void qtc::ConfigFile::remove_key(std::string key)
{
	try {
		open_file();
	} catch(...) {
		throw;
	}

	try {
		auto key_vals = import_to_map();

		file_reader.close();

		std::ofstream out(file_name);

		if (!out) {
			err_line_no = 0;
			throw qtc::FileOpenFailed(file_name);
		}

		out.seekp(0, std::ios::beg);

		for (auto &pair : key_vals) {
			if (pair.first != key) {
				print_key_val(out, pair.first, pair.second);
				out << std::endl;
			}
		}
	} catch (...) {
		throw;
	}
}

/*
 * It goes though all the keys in the qtc file
 * and store the key value pairs in an unordered_map
 * and returns.
 *
 * possible Exceptions:
 * 	EqualToWithoutAKey
 * 	FileOpenFailed
 * 	InvalidCharacter
 * 	MissingEqualTo
 * 	NoValueFoundForKey
 * 	TrailingCharsAfterEqualTo
 * 	ValueWithoutAKey
 */
std::unordered_map<std::string, std::string> qtc::ConfigFile::import_to_map()
{
	std::unordered_map<std::string, std::string> imported_map;
	std::string key, value;

	try {
		open_file();
	} catch(...) {
		throw;
	}

	file_reader.seekg(0, std::ios::beg);
	err_line_no = 1;

	try {
		while (file_reader.peek() != EOF) {
			key = out_block_parse();

			if (key.empty()) {
				// no more keys left to read
				break;
			}

			value = in_block_parse();

			imported_map.insert({key, value});
		}
	} catch (...) {
		throw;
	}

	return imported_map;
}
