#include <array>
#include <cstdio>
#include <cstdlib>
#include <error/error.h>
#include <filesystem>
#include <preprocess/preprocess.hpp>
#include <qtc/qtc.hpp>
#include <vector>

namespace fs = std::filesystem;

struct ArgParser {
	std::string program_name;

	int func_dirs_cnt;
	std::vector<std::string> func_dirs;

	int alias_file_cnt;
	std::vector<std::string> alias_files;

	int rename_file_cnt;
	std::vector<std::string> rename_files;

	void parse(int argc, char *argv[]);
} args;


[[noreturn]] void usage();

/*
 * executes a command and grabs the stdout in
 * a string and returns it.
 */
std::string execute(const char* cmd);

/*
 * Goes over all func dirs and
 * creates a replace_map with mappings:
 * func_name -> execute(func_path)
 */
replace_map make_func_replace_map();

/*
 * Calls qtc::import_to_map() on all alias files
 * and adds those aliases to rmap. If key already exists,
 * it's overwritten.
 */
void add_aliases_to_rmap(replace_map &rmap);

/*
 * Calls qtc::import_to_map() on all rename files
 * and renames all keys present in rmap to respective
 * values obtained for rename files.
 */
void rename_in_rmap(replace_map &rmap);


int main(int argc, char *argv[])
{
	args.parse(argc, argv);

	auto rmap = make_func_replace_map();

	add_aliases_to_rmap(rmap);

	rename_in_rmap(rmap);

	Preprocessor p(std::cin, std::cout);

	try {
		p.preprocess(rmap);
	} catch (std::exception &e) {
		P_ERR("Preprocessor::preprocess() failed : %s", e.what());
		exit(1);
	}

	return 0;
}

[[noreturn]] void usage()
{
	std::cerr << "usage: " << args.program_name << '\n'
		<< "\t<file_to_preprocess>\n"
		<< "\t<func_dirs_count> [<func_dirs>...]\n"
		<< "\t<alias_file_count> [<alias_files>...]\n"
		<< "\t<rename_file_count> [<rename_files>...]" << std::endl;
	exit(1);
}

/*
 * Thanks waqas for posting this
 * on https://stackoverflow.com/a/478960/11498773
 * under CC0 license.
 */
std::string execute(const char* cmd) {
	std::array<char, 4096> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
	if (!pipe) {
		P_ERR_NA("popen() failed!");
		exit(1);
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

replace_map make_func_replace_map()
{
	replace_map rmap;

	for (auto func_dir : args.func_dirs) {
		for (auto func: fs::directory_iterator(func_dir)) {
			rmap.insert({func.path().filename(), [func](std::string args) -> std::string {
				std::string cmd = std::string(std::getenv("QTBASH")) + " " + std::string(func.path()) + " \'" + args + "\'";
				return execute(cmd.c_str());
			}});
		}
	}

	return rmap;
}

void add_aliases_to_rmap(replace_map &rmap)
{
	for (auto alias_file_name : args.alias_files) {

		if (!fs::exists(alias_file_name)) {
			continue;
		}

		qtc::ConfigFile afile(alias_file_name);
		std::unordered_map<std::string, std::string> amap;

		try {
			amap = afile.import_to_map();
		} catch (std::exception &e) {
			QTC_EXCEPTION_NM(afile);
			exit(1);
		}

		for (auto alias : amap) {
			rmap.insert_or_assign(alias.first, [&rmap, alias]([[maybe_unused]] std::string args) -> std::string {
				std::istringstream value(alias.second);
				std::ostringstream preprocessed_value;
				Preprocessor p(value, preprocessed_value);
				try {
					p.preprocess(rmap);
				} catch (std::exception &e) {
					P_ERR("Preprocessor::preprocess() failed : %s", e.what());
					exit(1);
				}
				std::string processed_value_str = preprocessed_value.str();
				return processed_value_str;
			});
		}
	}
}

void rename_in_rmap(replace_map &rmap)
{
	for (const auto &rename_file_name : args.rename_files) {

		if (!fs::exists(rename_file_name)) {
			continue;
		}

		qtc::ConfigFile rnfile(rename_file_name);

		std::unordered_map<std::string, std::string> rnmap;

		try {
			rnmap = rnfile.import_to_map();
		} catch (std::exception &e) {
			QTC_EXCEPTION_NM(rnfile);
			exit(1);
		}

		for (auto rnmap_elem : rnmap) {
			auto nh = rmap.extract(rnmap_elem.first);

			if (nh) {
				nh.key() = rnmap_elem.second;
				rmap.insert(std::move(nh));
			}
		}
	}
}

void ArgParser::parse(int argc, char *argv[])
{
	args.program_name = argv[0];

	/*
	 * has to be at least 4
	 * First being the file to be preprocessed followed by
	 * func dir cnt, alias file cnt
	 * and rename file cnt, each of which can be 0.
	 */
	if (argc < 3) {
		P_ERR_NA("Insufficient args");
		usage();
	}

	int arg_no = 1;
	int remaining = 3;

	// gets_args() follows the pattern <count> [<count_names>...] to fill up global variable args
	auto get_args =
	[&arg_no, argc, argv, &remaining](int &cnt, std::vector<std::string> &vec, const char *name) {

		// remaining indicates the number of <count> [<count_names>...] remaining
		remaining--;

		try {
			cnt = std::stoi(argv[arg_no++]);
		} catch (const std::invalid_argument& ia) {
			P_ERR("%s: Invalid argument, std::stoi() failed for %s: %s", argv[arg_no - 1], name, ia.what());
			exit(1);
		} catch (const std::out_of_range& oor) {
			P_ERR("%s: Out of range error, std::stoi() failed for %s: %s", argv[arg_no - 1], name, oor.what());
			exit(1);
		}

		if (arg_no + cnt <= argc - remaining) {
			vec.reserve(cnt);

			int last = arg_no + cnt;
			while (arg_no != last) {
				vec.push_back(argv[arg_no++]);
			}
		} else {
			P_ERR("Expected %d %s but only %d args following it," \
				" %d of which are for other args", \
				cnt, name, argc - arg_no, remaining);
			usage();
		}
	};

	get_args(args.func_dirs_cnt, args.func_dirs, "func dirs");
	get_args(args.alias_file_cnt, args.alias_files, "alias files");
	get_args(args.rename_file_cnt, args.rename_files, "rename files");
}
