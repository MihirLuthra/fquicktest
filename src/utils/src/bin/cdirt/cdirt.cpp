#include <sys/stat.h>

#include <error/error.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

uintmax_t getatime(const char * path);

/*
 * Traverses directory recusively, ignoring
 * hidden files(files starting with dot).
 *
 * param1:
 * 	directory to traverse.
 *
 * retval:
 * 	vector of all the files in
 * 	the subtree. (doesn't include directories)
 */
std::vector<std::string> traverse_dir(const char *dir);

int main(int argc, char *argv[])
{
	if (argc != 2) {
		P_ERR_NA("Insufficient args");
		fprintf(stderr, "usage: %s <directory>\n", argv[0]);
		return 1;
	}

	std::string path = argv[1];

	fs::current_path(path);

	std::vector<std::string> paths = traverse_dir(".");

	// sort paths by access time
	std::sort(paths.begin(), paths.end(), [](const auto& lhs, const auto& rhs)
	{
		return getatime(lhs.c_str()) > getatime(rhs.c_str());
	});

	for (auto& path: paths) {
		// file format is like ./this/is/file
		// array is printed after [2] to remove "./" from output
		std::cout << &path.c_str()[2] << '\0';
	}

	return 0;
}

std::vector<std::string> traverse_dir(const char *dir)
{
	std::vector<std::string> paths;
	for (auto& p: fs::directory_iterator(dir)) {
		if (p.path().stem().string().front() == '.') {
			continue;
		}
		if (fs::is_directory(p.path())) {
			auto sub_paths = traverse_dir(p.path().string().c_str());
			paths.reserve(paths.size() + distance(sub_paths.begin(),sub_paths.end()));
			paths.insert(paths.end(),sub_paths.begin(),sub_paths.end());
		} else {
			paths.push_back(p.path().string());
		}
	}

	return paths;
}

uintmax_t getatime(const char * path)
{
	struct stat s;

	if (stat(path, &s) == -1) {
		P_ERR("stat(2) failed for %s", path);
		exit(1);
	}

	// tv_sec is guaranteed to be whole number
	return ((uintmax_t)s.st_atime);
}
