#include <sys/stat.h>

#include <error/error.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uintmax_t getctime(const char * path);

int main(int argc, char *argv[])
{
	uintmax_t ctime;
	char *path;

	if (argc != 2) {
		P_ERR_NA("Insufficient args");
		fprintf(stderr, "usage: %s <path>\n", argv[0]);
		exit(1);
	}

	path = argv[1];

	ctime = getctime(path);

	printf("%ju\n", ctime);

	return (0);
}

uintmax_t getctime(const char * path)
{
	struct stat s;

	if (stat(path, &s) == -1) {
		P_ERR("stat(2) failed for %s", path);
		exit(1);
	}

	// tv_sec is guaranteed to be whole number
	return ((uintmax_t)s.st_ctimespec.tv_sec);
}
