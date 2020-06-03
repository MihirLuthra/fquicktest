#include <cstdlib>
#include <ctime>
#include <error/error.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include "print_type_name.hpp"
#include <qtc/qtc.hpp>
#include <sstream>
#include <unordered_map>

namespace fs = std::filesystem;

#define RESET       "\033[0m"
#define BLUE        "\033[34m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define CYAN        "\033[36m"
#define BRIGHT_RED  "\033[91m"

#define BOLD_ON  "[1m"
#define BOLD_OFF "[0m"

// This is for printing exceptions caused by funcs being tested
// which is why it's printed to stdout
#define EXCEPTION(fmt, ...) \
	printf(CYAN "%s: Line %d: " fmt "\n" RESET, __FILE__, __LINE__, __VA_ARGS__)

#define EXCEPTION_NA(fmt) \
	printf(CYAN "%s: Line %d: " fmt "\n" RESET, __FILE__, __LINE__)

void print_test_results(bool test_result, std::string file_name)
{
	if (test_result) {
		std::cout << GREEN << "---> Tests passed for " << file_name << RESET << "\n";
	} else {
		std::cout << RED << "---> Tests failed for " << file_name << RESET << "\n";
	}
}

void print_testing_file(std::string file_name)
{
	std::cout << "\n" << BOLD_ON << BLUE << "---> Testing file " << file_name << RESET << BOLD_OFF << "\n";
}

std::string random_string(size_t length)
{
	auto alnum_randchar = []() -> char {
		const char charset[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[std::rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, alnum_randchar);
	return str;
}

/*
 * It calls qtc::ConfigFile::get_value_for_key(file_name, key)
 * expecting exception of type `T`.
 * It prints if no/other exception was encountered instead.
 */
template <typename T>
void expect_exceptions_for_keys(std::string file_name, std::string key);

/*
 * This function can be called to check if methods throw correct exceptions
 *
 * e.g., to check exceptions thrown by file.get_value_for_key(),
 * we could create a lambda:
 *
 * auto gvfk = [](string file_name, string key) {
 * 		qtc::ConfigFile file(file_name);
 * 		try {
 * 			qtc::get_value_for_key(key);
 * 		} catch (...) {
 * 			throw;
 * 		}
 * }
 *
 * and pass this lambda to function as:
 *	expect_exceptions_for_funcs<expected_exception>(file_name, bind(gvfk, _1, "KEY");
 *
 * TODO: run this for every possible exception
 */
template <typename T>
void expect_exceptions_for_funcs(std::string file_name, std::function<void(std::string)> func);

/*
 * It calls `ret_value = qtc::ConfigFile::get_value_for_key(file_name, key)`
 * expecting `ret_value` to be equal to `value` where
 * `key` and `value` are given in param2(expected_values_for_keys)
 */
void expect_values_for_keys(std::string file_name, std::unordered_map<std::string, std::string> expected_values_for_keys);

/*
 * It calls `imported_map = qtc::ConfigFile::import_to_map(file_name)`
 * expecting `imported_map == expected_hash_map`
 */
void hash_map_test(std::string file_name, std::unordered_map<std::string, std::string> expected_hash_map);

/*
 * It calls `ret_value = qtc::ConfigFile::get_value_for_key(file_name, key)`,
 * expecting `ret_value == expected_value`
 */
std::tuple<std::string, bool> get_value_for_key_and_expect_value(std::string file_name, std::string key, std::string expected_value);

/*
 * It runs tests for all the files in sample_qtc_files/
 */
namespace sample_qtc_files {
	void run_tests();
	void run_tests_for_exception_checks();
	void run_tests_for_import_to_map();
	void run_tests_for_get_value_for_key();
	void run_tests_for_set_value_for_key();
	void run_tests_for_remove_key();
}

int main()
{
	sample_qtc_files::run_tests();
	return (EXIT_SUCCESS);
}

std::tuple<std::string, bool> get_value_for_key_and_expect_value(std::string file_name, std::string key, std::string expected_value)
{
	std::string value;

	qtc::ConfigFile file(file_name);

	try {
		value = file.get_value_for_key(key);
	} catch (std::exception &e) {
		EXCEPTION("qtc::ConfigFile::get_value_for_key() failed with exception: %s", e.what());
	} catch (...) {
		EXCEPTION_NA("qtc::ConfigFile::get_value_for_key() failed with an unknown exception\n");
		exit(EXIT_FAILURE);
	}

	if (value == expected_value) {
		return std::make_tuple(value, true);
	}

	return std::make_tuple(value, false);
}

template <typename T>
void expect_exceptions_for_funcs(std::string file_name, std::function<void(std::string)> func)
{
	print_testing_file(file_name);

	bool test_result = false;
	try {
		func(file_name);
		std::cout << "Expected exception " << type_name<T>() << " but no exception occured\n";
		test_result = false;
	} catch(T &e) {
		test_result = true;
	} catch(std::exception &e) {
		std::cout << "Expected exception " << type_name<T>() << " but some other exception occured with message: "
			<< e.what() << "\n";
		test_result = false;
	} catch(...) {
		std::cout << "Expected exception " << type_name<T>() << " but some unknown exception occured\n";
		test_result = false;
	}

	print_test_results(test_result, file_name);
}

void expect_values_for_keys(std::string file_name, std::unordered_map<std::string, std::string> expected_values_for_keys)
{
	print_testing_file(file_name);

	bool test_result = true;

	for (auto& key_eval : expected_values_for_keys) {
		std::string value;
		bool result;

		std::tie(value, result)
			= get_value_for_key_and_expect_value(file_name, key_eval.first, key_eval.second);

		if (!result) {
			std::cout << "Expected value \"" << key_eval.second << "\" for key " << "\"" << key_eval.first
				<< "\"" << " but found value \"" << value << "\"\n";
			test_result = false;
		}
	}

	print_test_results(test_result, file_name);
}

void hash_map_test(std::string file_name, std::unordered_map<std::string, std::string> expected_hash_map)
{
	std::cout << "\n" << BOLD_ON << BLUE << "---> Testing qtc::ConfigFile::import_to_map() on file "
		<< file_name << RESET << BOLD_OFF << "\n";

	qtc::ConfigFile file(file_name);

	std::unordered_map<std::string, std::string> imported_map;

	try {
		imported_map = file.import_to_map();
	} catch(std::exception &e) {
		EXCEPTION("Exception occured with qtc::ConfigFile::import_to_map(): %s", e.what());
	} catch(...) {
		EXCEPTION_NA("Unknown exception occured with qtc::ConfigFile::import_to_map()\n");
	}

	if (imported_map == expected_hash_map) {
		std::cout << GREEN << "---> Tests passed for " << file_name << RESET << "\n";
	} else {
		std::cout << BOLD_ON << "Expected map:\n" << BOLD_OFF;
		for (auto &i : expected_hash_map) {
			std::cout << "[\"" << i.first << "\"] = " << "\"" << i.second << "\"" << "\n";
		}
		std::cout << BOLD_ON << "Imported map:\n" << BOLD_OFF;
		for (auto &i : imported_map) {
			std::cout << "[\"" << i.first << "\"] = " << "\"" << i.second << "\"" << "\n";
		}

		std::cout << RED << "---> Tests failed for " << file_name << RESET << "\n";
	}
}

void sample_qtc_files::run_tests()
{
	sample_qtc_files::run_tests_for_import_to_map();
	sample_qtc_files::run_tests_for_get_value_for_key();
	sample_qtc_files::run_tests_for_set_value_for_key();
	sample_qtc_files::run_tests_for_remove_key();
}

void sample_qtc_files::run_tests_for_import_to_map()
{
	std::cout << BOLD_ON << "\n>>> Testing qtc::ConfigFile::import_to_map():\n" << BOLD_OFF;

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> expected_hash_maps_for_files = {
		{
			"sample_qtc_files/general.qtc",
			{
				{"HOW_TO_OPEN_EDITOR", "vim [FILE_TO_OPEN]"},
				{"BOILERPLATE", "general.cpp"},
				{"COMPILE", "g++ [INSTANCE] -o [OUTPUT_PATH]"},
				{"EXECUTE", "[OUTPUT_PATH]"}
			}
		},

		{
			"sample_qtc_files/wierdly_formatted.qtc",
			{
				{"HOW_ARE_YOU", "FINE"},
				{"HOW_WAS_YOUR_DAY", "GOOD"},
				{"LIKE_VIM", "YES"},
				{"LIKE_WINDOWS", "NO"},
				{"LIKE_LINUX", "ye}s"}

			}
		}
	};

	for (auto& m : expected_hash_maps_for_files) {
		hash_map_test(m.first, m.second);
	}
}

void sample_qtc_files::run_tests_for_get_value_for_key()
{
	std::cout << BOLD_ON << "\n>>> Testing qtc::ConfigFile::get_value_for_key():\n" << BOLD_OFF;

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> expected_hash_maps_for_files = {
		{
			"sample_qtc_files/general.qtc",
			{
				{"HOW_TO_OPEN_EDITOR", "vim [FILE_TO_OPEN]"},
				{"BOILERPLATE", "general.cpp"},
				{"COMPILE", "g++ [INSTANCE] -o [OUTPUT_PATH]"},
				{"EXECUTE", "[OUTPUT_PATH]"}
			}
		},

		{
			"sample_qtc_files/wierdly_formatted.qtc",
			{
				{"HOW_ARE_YOU", "FINE"},
				{"HOW_WAS_YOUR_DAY", "GOOD"},
				{"LIKE_VIM", "YES"},
				{"LIKE_WINDOWS", "NO"},
				{"LIKE_LINUX", "ye}s"}

			}
		}
	};

	for (auto& m : expected_hash_maps_for_files) {
		expect_values_for_keys(m.first, m.second);
	}
}

void sample_qtc_files::run_tests_for_set_value_for_key()
{
	std::cout << BOLD_ON << "\n>>> Testing qtc::ConfigFile::set_value_for_key():\n" << BOLD_OFF;

	std::string file_name = "sample_qtc_files/for_testing_set_value_for_key.qtc";

	print_testing_file(file_name);

	std::srand(time(NULL));
	std::string value = random_string(std::rand() % 100);
	std::string key = "KEY";

	bool test_result = true;
	char *set_value_for_key_tmp_file = std::tmpnam(nullptr);

	if (set_value_for_key_tmp_file == nullptr) {
		P_ERR_NA("Failed to create temporary file name");
		return;
	}

	qtc::ConfigFile file(file_name);
	std::ofstream out_file(set_value_for_key_tmp_file);

	if (!out_file) {
		P_ERR("Failed to open %s", set_value_for_key_tmp_file);
		return;
	}

	std::cout << "Setting key \"" << key << "\" to value \"" << value << "\"\n";

	try {
		file.set_value_for_key(key, value, out_file);
		fs::rename(set_value_for_key_tmp_file, file_name);
	} catch(std::exception &e) {
		EXCEPTION("Exception occured with qtc::ConfigFile::set_value_for_key(): %s", e.what());
	} catch(...) {
		EXCEPTION_NA("Unknown exception occured with qtc::ConfigFile::set_value_for_key()\n");
	}

	out_file.close();

	bool result;
	std::string val;

	std::tie(val, result)
		= get_value_for_key_and_expect_value(file_name, key, value);

	if (!result) {
		std::cout << "Expected value \"" << value << "\" for key " << "\"" << key
			<< "\"" << " but found value \"" << val << "\"\n";
		test_result = false;
	}

	print_test_results(test_result, file_name);
}

void sample_qtc_files::run_tests_for_remove_key()
{
	std::cout << BOLD_ON << "\n>>> Testing qtc::ConfigFile::remove_key():\n" << BOLD_OFF;

	std::string file_name = "sample_qtc_files/for_testing_remove_key.qtc";

	print_testing_file(file_name);

	std::srand(time(NULL));
	std::string value = random_string(std::rand() % 100);
	std::string key = "KEY";

	bool test_result = true;
	char * set_value_for_key_tmp_file = std::tmpnam(nullptr);

	if (set_value_for_key_tmp_file == nullptr) {
		P_ERR_NA("Failed to create temporary file name");
		return;
	}

	char * remove_key_tmp_file = std::tmpnam(nullptr);

	if (remove_key_tmp_file == nullptr) {
		P_ERR_NA("Failed to create temporary file name");
		return;
	}

	qtc::ConfigFile file(file_name);
	std::ofstream out_file(set_value_for_key_tmp_file);

	if (!out_file) {
		P_ERR("Failed to open %s", set_value_for_key_tmp_file);
		return;
	}

	std::cout << "Setting key \"" << key << "\" to value \"" << value << "\"\n";

	try {
		file.set_value_for_key(key, value, out_file);
		fs::rename(set_value_for_key_tmp_file, file_name);
	} catch(std::exception &e) {
		EXCEPTION("Exception occured with qtc::ConfigFile::set_value_for_key(): %s", e.what());
	} catch(...) {
		EXCEPTION_NA("Unknown exception occured with qtc::ConfigFile::set_value_for_key()\n");
	}

	out_file.close();

	std::cout << "Removing key \"" << key << "\"\n";

	out_file.open(remove_key_tmp_file);

	if (!out_file) {
		P_ERR("Failed to open %s", remove_key_tmp_file);
		return;
	}

	try {
		file.remove_key(key, out_file);
		fs::rename(remove_key_tmp_file, file_name);
	} catch(std::exception &e) {
		EXCEPTION("Exception occured with qtc::ConfigFile::remove_key(): %s", e.what());
	} catch(...) {
		EXCEPTION_NA("Unknown exception occured with qtc::ConfigFile::remove_key()\n");
	}

	out_file.close();

	try {
		std::cout << "Searching key \"" << key << "\"\n";
		file.get_value_for_key(key);
		test_result = false;
		std::cout << "Expected exception std::KeyNotFound but none occured\n";
	} catch(qtc::KeyNotFound &e) {
		test_result = true;
	} catch(std::exception &e) {
		test_result = false;
		std::cout << "Expected exception std::KeyNotFound but some other exception occured with message"
			<< e.what() << "\n";
	} catch(...) {
		test_result = false;
		std::cout << "Expected exception std::KeyNotFound but some unknown exception occured\n";
	}

	print_test_results(test_result, file_name);
}
