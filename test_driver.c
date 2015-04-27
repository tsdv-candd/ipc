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

/* These header is from CUnit */
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <signal.h>
#include "logservice.h"

/* WARNING - MAINTENANCE NIGHTMARE AHEAD
 *
 * If you change any of the tests & suites below, you also need
 * to keep track of changes in the result statistics and reflect
 * any changes in the result report counts in print_example_results().
 *
 * Yes, this could have been designed better using a more
 * automated mechanism.  No, it was not done that way.
 */
int msqid_global;

/*
 * Testing case 1 for the API logServiceInit
 */
static void test_logServiceInit_Success1(void) {
    int result = 0;
    result = logServiceInit();
    CU_ASSERT_NOT_EQUAL(result, -1);
}

/*
 * Testing case 2 for the API logServiceInit
 */
static void test_logServiceInit_Success2(void) {
    int result = 0;
    result = logServiceInit();
    result = logServiceInit();
    CU_ASSERT_NOT_EQUAL(result, -1);
}

/*
 * Testing case 1 for the API logMessage
 */
static void test_logMessage_Success1(void) {
    int result = 0;
    int msqid;
    char str1[] = "test send";
    struct message rbuf;

    msqid = logServiceInit();
    CU_ASSERT_NOT_EQUAL(result, -1);

    result = logMessage(msqid, str1);
    CU_ASSERT_NOT_EQUAL(result, -1);

    memset(rbuf.message, 0, MSGCHARS + 1);
    if (msgrcv(msqid, &rbuf, MSGCHARS, client_pid, 0) < 0) {
        perror("SERVER: ERROR msgrcv");
        exit(1);
    }

    CU_ASSERT_NSTRING_EQUAL(rbuf.message, str1, strlen(str1));

}

/*
 * Testing case 2 for the API logMessage
 */
static void test_logMessage_Success2(void) {
    int result = 0;
    result = logMessage(0, "test2");
    CU_ASSERT_EQUAL(result, 0);
}

/*
 * Testing case 3 for the API logMessage
 * Test abnormal case
 */
static void test_logMessage_abnormal1(void) {
    int result = 0;
    result = logMessage(0, NULL);
    CU_ASSERT_EQUAL(result, -1);
}

/*
 * Testing case 4 for the API logMessage
 * Test abnormal case
 */
static void test_logMessage_abnormal2(void) {
    int result = 0;
    result = logMessage(1, "test3");
    CU_ASSERT_EQUAL(result, -1);
}

/* Suite initialization/clean-up functions */
static int suite_logServiceInit_init(void) {
    /*
    * For creating message queue
    */
    int msgflg = IPC_CREAT | 0666;

    if ((msqid_global = msgget(KEY, msgflg)) < 0) {
        perror("SERVER: ERROR msgget");
        return 1;
    }
    return 0;
}
static int suite_logServiceInit_clean(void) {
    int resutl = 0;
    resutl=msgctl(msqid_global, IPC_RMID,NULL);
    if (resutl < 0) {
        perror( strerror(errno) );
        printf("msgctl (return queue) failed, resutl=%d\n", resutl);
        return 1;
    }
    return 0;
}

static int suite_logMessage_init(void) {
    /*
     * For creating message queue
     */
    int msgflg = IPC_CREAT | 0666;

    if ((msqid_global = msgget(KEY, msgflg)) < 0) {
        perror("SERVER: ERROR msgget");
        return 1;
    }
    return 0;
}
static int suite_logMessage_clean(void) {
    int resutl = 0;
    resutl=msgctl(msqid_global, IPC_RMID, NULL);
    if (resutl < 0) {
        perror( strerror(errno) );
        printf("msgctl (return queue) failed, resutl=%d\n", resutl);
        return 1;
    }
    return 0;
}

static CU_TestInfo tests_logServiceInit[] = {
    { "test_logServiceInit_Success1", test_logServiceInit_Success1 },
    { "test_logServiceInit_Success2", test_logServiceInit_Success2 },
    CU_TEST_INFO_NULL,
};

static CU_TestInfo tests_logMessage[] = {
    { "test_logMessage_Success1", test_logMessage_Success1 },
    { "test_logMessage_Success2", test_logMessage_Success2 },
    { "test_logMessage_abnormal1", test_logMessage_abnormal1 },
    { "test_logMessage_abnormal2", test_logMessage_abnormal2 },
    CU_TEST_INFO_NULL,
};


static CU_SuiteInfo suites[] = {
    { "suite_logServiceInit",  suite_logServiceInit_init, suite_logServiceInit_clean, tests_logServiceInit },
    { "suite_logMessage",      suite_logMessage_init, suite_logMessage_clean, tests_logMessage },
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

/*
	Main test program base on Cunit testing framework.
*/
int main(int argc, char* argv[])
{
    CU_BasicRunMode mode = CU_BRM_VERBOSE;
    CU_ErrorAction error_action = CUEA_IGNORE;

    setvbuf(stdout, NULL, _IONBF, 0);
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

