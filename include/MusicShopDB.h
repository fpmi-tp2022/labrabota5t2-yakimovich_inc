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

	printf("\n_____________________________________________________________\n");

    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
	printf("_____________________________________________________________\n");
    return 0;
}



int DBrequest(sqlite3* db, char* sql)
{
	char* zErrMsg = 0;
	printf("%s", sql);
	int rc = sqlite3_exec(db, sql, callback, (void*)NULL, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		printf("Error\n");
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		return 1;
	}
	return 0;
}
void Request2(sqlite3* db, int id, char* date1, char* date2) {
	char sql[512];
	sprintf(sql, "SELECT compactID, (SUM(amount) * CompactDisk.price) FROM Trade, CompactDisk "\
		"WHERE compactID = %d AND code = 1 AND CompactDisk.id = %d AND Trade.date >= '%s' AND  Trade.date <= '%s';\n\0", id, id, date1, date2);
	DBrequest(db, sql);
	
}

void Requests(sqlite3* db)
{
	printf("\n\nChoose requests: \n"\
		"1. Information about amount of sold & remaining compacts \n"\
		"2. Information about compact (amount, cost) that were sold in period \n"\
		"3. Information about compact, that was sold max times \n"\
		"4. Information about amount of sold compacts of most popular performer \n"\
		"5. Information about authors (amount & sum of sold compacts of authors) \n"\
		"6. Information of trade in period\n"\
		"7. Information of compact disk trade\n");
	int answer;

	scanf("%d", &answer);
	switch (answer)
	{
	case 1:
		
		break;
	case 2: {
		char date1[10], date2[10];
		int id;
		printf("Enter id of compact:\n");
		scanf("%d", &id);
		printf("Enter start of period in format HHHH.MM.DD:\n");
		scanf("%s", date1);
		printf("Enter end of period in format HHHH.MM.DD:\n");
		scanf("%s", date2);
		// check of date
		Request2(db, id, date1, date2);
		break;
	}
	case 3:
		
		break;
	case 4:
		
		break;
	case 5:
		
		break;
	case 6:
		
		break;
	case 7:
		
		break;
	default:
		break;
	}
}