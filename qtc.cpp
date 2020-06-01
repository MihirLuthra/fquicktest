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
	return "";
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
	return "";
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
	return "";
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
	return "";
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
