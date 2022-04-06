#pragma once

#include <stdio.h>
#include <stdlib.h>
#include < string.h >

#define CLIENT_RIGHTS 0
#define OWNER_RIGHTS  1

#define ATTEMPTS_AMOUNT 3

const char* ownerLogin = "admin";
const char* ownerPassword = "111";
static int accessRights;


static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
