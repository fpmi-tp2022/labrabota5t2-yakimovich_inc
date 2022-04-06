
#include "../include/MusicShopDB.c"


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
void AllCompactTradeInfo(sqlite3* db) {
	DBrequest(db, "SELECT * FROM (SELECT CompactDisk.id AS ID1, SUM(Trade.amount) AS SOLD  FROM Trade, CompactDisk "\
		"WHERE Trade.code = 1 AND CompactDisk.id = Trade.compactID "\
		"GROUP BY CompactDisk.id) AS TABLE1 "\
		"LEFT JOIN"\
		"(SELECT * FROM(SELECT CompactDisk.id AS ID2, SUM(Trade.amount) AS HAVE  FROM Trade, CompactDisk "\
		"WHERE Trade.code = 2 AND CompactDisk.id = Trade.compactID "\
			" GROUP BY CompactDisk.id) AS TABLE2) "\
		"WHERE ID1 = ID2 "\
		"ORDER BY(HAVE - SOLD);"); 

}
void GetCostAndAmountonPeriod(sqlite3* db, int id, char* date1, char* date2) {
	char sql[512];
	sprintf(sql, "SELECT compactID, (SUM(amount) * CompactDisk.price) FROM Trade, CompactDisk "\
		"WHERE compactID = %d AND code = 1 AND CompactDisk.id = %d AND Trade.date >= '%s' AND  Trade.date <= '%s';\n\0", id, id, date1, date2);
	DBrequest(db, sql);
	
}
void GetMostSoldCompactInfo(sqlite3* db) {
	DBrequest(db, 
		"CREATE TABLE TABLE2 AS SELECT ID1, SOLD, CompactDisk.company, CompactDisk.date, CompactDisk.price "\
		"FROM((SELECT CompactDisk.id AS ID1, SUM(Trade.amount) AS SOLD  FROM Trade, CompactDisk "\
		"WHERE Trade.code = 1 AND CompactDisk.id = Trade.compactID "\
		"GROUP BY CompactDisk.id) "\
		"AS TABLE1), CompactDisk "\
		"WHERE CompactDisk.id = ID1; "\
		"SELECT * FROM TABLE2 "\
		"WHERE TABLE2.SOLD = (SELECT MAX(SOLD) FROM TABLE2); "\
		"DROP TABLE TABLE2; ");

}
void GetMostPopularPerformerTradeInfo(sqlite3* db) {
	DBrequest(db,
		"SELECT ID, MAX(CNT), MusicalComposition.performer FROM "\
		"(SELECT Trade.compactID AS ID, SUM(amount) AS CNT FROM Trade WHERE Trade.code = 1 GROUP BY Trade.compactID), MusicalComposition "\
		"WHERE MusicalComposition.compactID = ID;"
		);
}

void AllAuthorsInfo(sqlite3* db) {
	DBrequest(db,
		"SELECT AUTHOR, SUM(AMOUNT), sum(COST) FROM( "\
		"SELECT MusicalComposition.author AS AUTHOR, MusicalComposition.compactID, Trade.amount as AMOUNT, (Trade.amount * CompactDisk.price) as COST FROM MusicalComposition, CompactDisk "\
		"INNER JOIN Trade "\
		"WHERE Trade.code = 1 AND MusicalComposition.compactID = Trade.compactID AND CompactDisk.id = MusicalComposition.compactID) "\
		"GROUP BY AUTHOR;"
	);
}

void GetPeriodTrade(sqlite3* db, char* date1, char* date2) {
	char* sql[512];
	sprintf(sql, "SELECT CompactDisk.id, SUM(CUR.amount) as BOUGHT FROM CompactDisk "\
		"JOIN "\
		"(SELECT * FROM Trade WHERE '%s' <= Trade.date AND  Trade.date <= '%s') "\
		"as CUR ON CompactDisk.id = CUR.compactID "\
		"WHERE CUR.code = 1 "\
		"GROUP BY CompactDisk.id;", date1, date2);
	DBrequest(db, sql);
	sprintf(sql, "SELECT CompactDisk.id, SUM(CUR.amount) as BOUGHT FROM CompactDisk "\
		"JOIN "\
		"(SELECT * FROM Trade WHERE '%s' <= Trade.date AND  Trade.date <= '%s') "\
		"as CUR ON CompactDisk.id = CUR.compactID "\
		"WHERE CUR.code = 2 "\
		"GROUP BY CompactDisk.id;", date1, date2);
	DBrequest(db, sql);
	
}



void GetCompactSoldInfo(sqlite3* db, int id) {
	char* sql[256];
	sprintf(sql, "SELECT * FROM Trade WHERE Trade.code = 2 AND Trade.compactID = %d;", id);
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
		AllCompactTradeInfo(db);	
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
		GetCostAndAmountonPeriod(db, id, date1, date2);
		break;
	}
	case 3:
		GetMostSoldCompactInfo(db);	
		break;
	case 4:
		GetMostPopularPerformerTradeInfo(db);	
		break;
	case 5:
		AllAuthorsInfo(db);	
		break;
	case 6:{
		char date1[11], date2[11];
		printf("Enter start of period in format HHHH.MM.DD:\n");
		scanf("%s", date1);
		printf("Enter end of period in format HHHH.MM.DD:\n");
		scanf("%s", date2);
		// check of date
		GetPeriodTrade(db, date1, date2);
		
		break;
	}
	case 7:
	{
		int id;
		printf("Enter id of compact:\n");
		scanf("%d", &id);
		GetCompactSoldInfo(db, id);
		break;
	}
		break;
	default:
		break;
	}
}