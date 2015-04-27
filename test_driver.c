/*
 *  CUnit - A Unit testing framework library for C.
 *  Copyright (C) 2001        Anil Kumar
 *  Copyright (C) 2004, 2005  Anil Kumar, Jerry St.Clair
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

/* WARNING - MAINTENANCE NIGHTMARE AHEAD
 *
 * If you change any of the tests & suites below, you also need
 * to keep track of changes in the result statistics and reflect
 * any changes in the result report counts in print_example_results().
 *
 * Yes, this could have been designed better using a more
 * automated mechanism.  No, it was not done that way.
 */

static void testSuccess1(void) {
    CU_ASSERT(1);
}
static void testSuccess2(void) {
    CU_ASSERT(2);
}
static void testSuccess3(void) {
    CU_ASSERT(3);
}

static void testFailure1(void) {
    CU_ASSERT(0);
}
static void testFailure2(void) {
    CU_ASSERT(0);
}
static void testFailure3(void) {
    CU_ASSERT(0);
}

/* Suite initialization/cleanup functions */
static int suite_success_init(void) {
    return 0;
}
static int suite_success_clean(void) {
    return 0;
}

static int suite_failure_init(void) {
    return 1;
}
static int suite_failure_clean(void) {
    return 1;
}

static CU_TestInfo tests_success[] = {
    { "testSuccess1", testSuccess1 },
    { "testSuccess2", testSuccess2 },
    { "testSuccess3", testSuccess3 },
    CU_TEST_INFO_NULL,
};

static CU_TestInfo tests_failure[] = {
    { "testFailure1", testFailure1 },
    { "testFailure2", testFailure2 },
    { "testFailure3", testFailure3 },
    CU_TEST_INFO_NULL,
};


static CU_SuiteInfo suites[] = {
    { "suite_success_both",  suite_success_init, suite_success_clean, tests_success },
    { "test_failure",        suite_failure_init, suite_failure_clean, tests_failure },
    CU_SUITE_INFO_NULL,
};

void AddTests(void)
{
    assert(NULL != CU_get_registry());
    assert(!CU_is_test_running());

    /* Register suites. */
    if (CU_register_suites(suites) != CUE_SUCCESS) {
        fprintf(stderr, "suite registration failed - %s\n",
                CU_get_error_msg());
        exit(EXIT_FAILURE);
    }
}

void print_example_results(void)
{
    fprintf(stdout, "\n\nExpected Test Results:"
            "\n\n  Error Handling  Type      # Run   # Pass   # Fail"
            "\n\n  ignore errors   suites%9u%9u%9u"
            "\n                  tests %9u%9u%9u"
            "\n                  asserts%8u%9u%9u"
            "\n\n  stop on error   suites%9u%9u%9u"
            "\n                  tests %9u%9u%9u"
            "\n                  asserts%8u%9u%9u\n\n",
            14, 14, 3,
            31, 10, 21,
            89, 47, 42,
            4, 4, 1,
            12, 9, 3,
            12, 9, 3);
}

/*
	Main test program base on Cunit testing framework.
*/
int main(int argc, char* argv[])
{
    CU_BasicRunMode mode = CU_BRM_VERBOSE;
    CU_ErrorAction error_action = CUEA_IGNORE;
    int i;

    setvbuf(stdout, NULL, _IONBF, 0);

    for (i=1 ; i<argc ; i++) {
        if (!strcmp("-i", argv[i])) {
            error_action = CUEA_IGNORE;
        }
        else if (!strcmp("-f", argv[i])) {
            error_action = CUEA_FAIL;
        }
        else if (!strcmp("-A", argv[i])) {
            error_action = CUEA_ABORT;
        }
        else if (!strcmp("-s", argv[i])) {
            mode = CU_BRM_SILENT;
        }
        else if (!strcmp("-n", argv[i])) {
            mode = CU_BRM_NORMAL;
        }
        else if (!strcmp("-v", argv[i])) {
            mode = CU_BRM_VERBOSE;
        }
        else if (!strcmp("-e", argv[i])) {
            print_example_results();
            return 0;
        }
        else {
            printf("\nUsage:  BasicTest [options]\n\n"
                   "Options:   -i   ignore framework errors [default].\n"
                   "           -f   fail on framework error.\n"
                   "           -A   abort on framework error.\n\n"
                   "           -s   silent mode - no output to screen.\n"
                   "           -n   normal mode - standard output to screen.\n"
                   "           -v   verbose mode - max output to screen [default].\n\n"
                   "           -e   print expected test results and exit.\n"
                   "           -h   print this message and exit.\n\n");
            return 0;
        }
    }

    if (CU_initialize_registry()) {
        printf("\nInitialization of Test Registry failed.");
    }
    else {
        AddTests();
        CU_basic_set_mode(mode);
        CU_set_error_action(error_action);
        printf("\nTests completed with return value %d.\n", CU_basic_run_tests());
        CU_cleanup_registry();
    }

    return 0;
}

