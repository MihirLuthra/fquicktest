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
		auto value = file.get_value_for_key(args.key);
		std::cout << value;
	} catch(std::exception &e) {
		QTC_EXCEPTION_NM(file);
		exit(1);
	}

	return 0;
}
