#ifndef __ERROR_H__
#define __ERROR_H__

#ifdef __cplusplus
#	include <cstdio>
#else
#	include <stdio.h>
#endif

#define RED         "\033[31m"
#define BRIGHT_RED  "\033[91m"
#define RESET_COLOR "\033[0m"

#define BOLD_ON  "[1m"
#define BOLD_OFF "[0m"

#define P_ERR(fmt, ...) \
	fprintf(stderr, BOLD_ON "%s: Line %d: " BOLD_OFF fmt "\n", __FILE__, __LINE__, __VA_ARGS__)

#define P_ERR_NA(fmt) \
	fprintf(stderr, BOLD_ON "%s: Line %d: " BOLD_OFF fmt "\n", __FILE__, __LINE__)


#define C_P_ERR(fmt, ...) \
	fprintf(stderr, BOLD_ON RED "%s: Line %d: " BOLD_OFF BRIGHT_RED fmt "\n" RESET_COLOR, __FILE__, __LINE__, __VA_ARGS__)

#define C_P_ERR_NA(fmt) \
	fprintf(stderr, BOLD_ON RED "%s: Line %d: " BOLD_OFF BRIGHT_RED fmt "\n" RESET_COLOR, __FILE__, __LINE__)

#endif /* __ERROR_H__ */
