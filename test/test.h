#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST(name) \
        static int test_##name##_result; \
	static int test_##name (void)

#define RUN_TEST(name, type) \
	test_##name##(); \
        if (test_##name##_##type) { \
		++g_passed; \
		printf(#name "/" #type " OK\n"); \
	} else { \
		++g_failed; \
		fprintf(stderr, #name "/" #type " FAIL\n"); \
	}

#define ASSERT(expr) \
        { if (!(expr))  }

#define PRINT_RESULTS(unused) \
	printf("%d total, %d passed, %d failed\n", \
                g_passed + g_failed, g_passed, g_failed);

extern int g_passed;
extern int g_failed;

#endif
