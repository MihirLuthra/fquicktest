#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include <cstdio>

#define P_ERR(fmt, ...) \
	fprintf(stderr, "%s: Line %d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__)

#define P_ERR_NA(fmt) \
	fprintf(stderr, "%s: Line %d: " fmt "\n", __FILE__, __LINE__)

#endif /* __ERROR_HPP__ */
