#include <cstdio>
#include <clium_identifier/clium_identifier.hpp>

namespace qi = clium_identifier;

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s <identifier_name>\n", argv[0]);
		return -1;
	}

	if (qi::is_invalid(argv[1])) {
		return 1;
	}

	return 0;
}
