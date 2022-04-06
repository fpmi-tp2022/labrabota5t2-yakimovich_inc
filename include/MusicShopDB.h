
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define CLIENT_RIGHTS 0
#define OWNER_RIGHTS  1

#define ATTEMPTS_AMOUNT 3

static int accessRights;

static int callback(void* NotUsed, int argc, char** argv, char** azColName);
int DBrequest(sqlite3* db, char* sql);
void AllCompactTradeInfo(sqlite3* db);
void GetCostAndAmountonPeriod(sqlite3* db, int id, char* date1, char* date2);
void GetMostSoldCompactInfo(sqlite3* db);
void GetMostPopularPerformerTradeInfo(sqlite3* db);
void AllAuthorsInfo(sqlite3* db);
void GetPeriodTrade(sqlite3* db, char* date1, char* date2);
void GetCompactSoldInfo(sqlite3* db, int id);
void Requests(sqlite3* db, int a);
int checkDate(char date[]);
