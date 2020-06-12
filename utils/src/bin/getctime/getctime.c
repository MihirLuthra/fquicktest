#include <sys/stat.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define P_ERR(fmt, ...) \
	fprintf(stderr, "%s: Line %d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

uintmax_t getctime(const char * path);

int main(int argc, char *argv[])
{
	uintmax_t ctime;
	char *path;

	if (argc < 2) {
		P_ERR("Insufficient args");
		fprintf(stderr, "usage: %s <path>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	path = argv[1];

	ctime = getctime(path);

	printf("%ju\n", ctime);

	return (EXIT_SUCCESS);
}

uintmax_t getctime(const char * path)
{
	struct stat s;

	if (stat(path, &s) == -1) {
		P_ERR("stat(2) failed for %s", path);
		return (-1);
	}

	// tv_sec is guaranteed to be whole number
	return ((uintmax_t)s.st_ctimespec.tv_sec);
}
