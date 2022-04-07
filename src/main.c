#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "../include/MusicShopDB.h"

#define TRUE 1
#define FALSE 0

const char* ownerLogin = "admin";
const char* ownerPassword = "111";
const int CLIENT_RIGHTS = 0;
const int OWNER_RIGHTS = 1;
int accessRights;

#pragma warning(disable:4996)

void Authorization() {
	printf("Hello, who is there?\n");
	printf("Enter your name:\n");
	char name[10];
	scanf("%s", name);
	if (!strcmp(name, ownerLogin))  {
		printf("Enter your password:\n");
		char pass[10];
		scanf("%s", pass);
		int attempts = 0;
		while (strcmp(pass, ownerPassword) && attempts < ATTEMPTS_AMOUNT) {
			printf("Incorrect password, please try again:\n");
			scanf("%s", pass);
			attempts++;
		}
		if (!strcmp(pass, ownerPassword)) {
			printf("\nYou have all access to data base.\n\n");
			accessRights = OWNER_RIGHTS;
			return;
		}
	}
	printf("\nYou have access only for requests, marked with *. \n\n");
	accessRights = CLIENT_RIGHTS;
	return;
}

//Show menu and orginize work of system
int main(int argc, char* argv[]) {

	Authorization();

	sqlite3* db;
	char* zErrMsg = 0;
	int rc = sqlite3_open("src/Music_shop.db", &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1;
	}

	int go = TRUE;
	while (go) {
		int answer;
		printf("Choose action: \n1.INSERT\n2.DELETE\n3.Update\n4.Requests\n5.Exit\n");
		scanf("%d", &answer);
		switch (answer) {
		case 1:{
			if (accessRights == CLIENT_RIGHTS){
				printf("You don't have access for changing BD\n");
				break;
			}
			Insert(db, zErrMsg, rc);
			break;
		}
		case 2:{
			if (accessRights == CLIENT_RIGHTS){
				printf("You don't have access for changing BD\n");
				break;
			}
			Delete(db, zErrMsg, rc);
			break;
		}
		case 3:{
			if (accessRights == CLIENT_RIGHTS){
				printf("You don't have access for changing BD\n");
				break;
			}
			Update(db, zErrMsg, rc);
			break;
		}
		case 4: {
			Requests(db, accessRights);
			break;
		}
		case 5: {
			go = FALSE;
			break;
		}
		default: {
			break;
		}
		}
		
	}
	sqlite3_close(db);
}
