#ifndef __QTC_HPP__
#define __QTC_HPP__

#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace qtc {

/*
 * The following class can be made much more efficient
 * but given the current use-case, their is no need for
 * such performance at the cost of simplicity.
 *
 * But, just in case, if ever needed:
 *
 * Currently it calls its methods such that they perform
 * their jobs from scratch every time they are called.
 * 
 * What could be done is that upon initialization of
 * an instance of the class, the file can be bought into a
 * private std::unordered_map variable and further servings can be
 * done through that. It could track file's last inode change time
 * to discard this cache and reset the hash_map. Changes to file
 * via set/insert key val could be lazyily written.
 *
 * A possible way to optimise it further is to start a background process
 * that creates an instance for every file and further communication
 * could be done through it with named pipes.
 */
class ConfigFile {
private:
	std::ifstream file_reader;
	std::string file_name;
	unsigned int err_line_no;

	void open_file();
	void print_key_val(std::ostream &out, std::string key, std::string value, bool add_spaces);
	std::string out_block_parse();
	std::string in_block_parse();
	std::string in_block_parse_and_strip_blanks();

public:

	ConfigFile(std::string file_name) : file_name(file_name){}

	unsigned int get_err_line() {
		return err_line_no;
	}

	std::string get_file_name() {
		return file_name;
	}

	void print_value_for_key(std::string key);
	void set_value_for_key(std::string key, std::string new_value, std::ostream &out_file);
	std::string get_value_for_key(std::string key);
	void remove_key(std::string key, std::ostream &out_file);
	std::unordered_map<std::string, std::string> import_to_map(bool exact_value = false);

	// set value in the same file
	void set_value_for_key(std::string key, std::string new_value);
	void remove_key(std::string key);
};

class BlockWithoutAKey : public std::exception {
public:
	const char *what() const noexcept {
		return "Block without a key";
	}
};

class EqualToWithoutAKey : public std::exception {
public:
	const char *what() const noexcept {
		return "Found equal to without a key";
	}
};

class FileOpenFailed : public std::exception {
	std::string file_name;
	mutable char *ret_str = nullptr;
public:
	FileOpenFailed(std::string file_name) : file_name(file_name) {}

	~FileOpenFailed() {
		if (ret_str != nullptr) {
			delete ret_str;
		}
	}
	const char *what() const noexcept {
		std::ostringstream ret;		
		ret << "Failed to open file " << file_name;
		ret_str = strdup(ret.str().c_str());
		return ret_str;
	}
};

class InvalidCharacter : public std::exception {
private:
	char c;
	mutable char *ret_str = nullptr;
public:
	InvalidCharacter(char c) : c(c) {}

	~InvalidCharacter() {
		if (ret_str != nullptr) {
			delete ret_str;
		}
	}

	const char *what() const noexcept {
		std::ostringstream ret;
		ret << "Invalid character " << this->c;
		ret_str = strdup(ret.str().c_str());
		return ret_str;
	}
};

class InvalidOutputStream : public std::exception {
public:
	const char *what() const noexcept {
		return "Invalid output stream";
	}
};

class KeyNotFound : public std::exception {
private:
	std::string key;
	mutable char *ret_str = nullptr;
public:
	KeyNotFound(std::string key) : key(key) {}

	~KeyNotFound() {
        if(ret_str != nullptr) {
            delete ret_str;
        }
    }

	const char *what() const noexcept {
		std::ostringstream ret;
		ret << "\"" << this->key << "\"" << ": key not found";
		ret_str = strdup(ret.str().c_str());
		return ret_str;
	}
};

class MissingEqualTo : public std::exception {
public:
	const char *what() const noexcept {
		return "Missing '=' between key and '{'";
	}
};

class NoValueFoundForKey : public std::exception {
private:
	std::string key;
	mutable char *ret_str = nullptr;
public:
	NoValueFoundForKey(std::string key) : key(key) {}

	~NoValueFoundForKey() {
        if(ret_str != nullptr) {
            delete ret_str;
        }
    }

	const char *what() const noexcept {
		std::ostringstream ret;
		ret << "\"" << this->key << "\"" << ": No value found for key.";
		ret_str = strdup(ret.str().c_str());
		return ret_str;
	}
};

class TrailingCharsAfterEqualTo : public std::exception {
public:
	const char *what() const noexcept {
		return "Trailing characters after '='";
	}
};

class UnterminatedBlock : public std::exception {
public:
	const char *what() const noexcept {
		return "Unterminated block";
	}
};

}

#endif /* __QTC_HPP__ */
