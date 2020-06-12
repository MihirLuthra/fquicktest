#ifndef __PREPROCESS_HPP__
#define __PREPROCESS_HPP__

#include <exception>
#include <iostream>
#include <unordered_map>

using replace_map = std::unordered_map<std::string, std::function<std::string(std::string)>>;

class Preprocessor {
private:
	std::istream &inp;
	std::ostream &out;

	std::string parse_block(replace_map rmap);
	std::string parse_func_block(replace_map rmap);
public:
	Preprocessor(std::istream &inp, std::ostream &out) : inp(inp), out(out) {}

	void preprocess(replace_map rmap);
};

class InvalidInputStream : public std::exception {
public:
	const char *what() const noexcept {
		return "Invalid input stream";
	}
};

class InvalidOutputStream : public std::exception {
public:
	const char *what() const noexcept {
		return "Invalid output stream";
	}
};

class UnterminatedFuncBlock : public std::exception {
public:
	const char *what() const noexcept {
		return "Unterminated function block";
	}
};

#endif /* __PREPROCESS_HPP__ */
