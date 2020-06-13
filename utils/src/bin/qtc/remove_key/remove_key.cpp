#include <cstdio>
#include <error/error.h>
#include <iostream>
#include <qtc/qtc.hpp>

struct {
	std::string file_name, key;
} args;


int main(int argc, char *argv[])
{
	if (argc != 3) {
		P_ERR("usage: %s <qtc_file_name> <key>", argv[0]);
		exit(-1);
	}

	args.file_name = argv[1];
	args.key = argv[2];

	qtc::ConfigFile file(args.file_name);

	try {
		file.remove_key(args.key);
	} catch(std::exception &e) {
		P_ERR("%s: Line %d: %s", args.file_name.c_str(), file.get_err_line(), e.what());
		exit(1);
	} catch(...) {
		P_ERR("%s: Line %d: Unknown exception", args.file_name.c_str(), file.get_err_line());
		exit(1);
	}

	return 0;
}
