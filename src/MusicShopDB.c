
#include "../include/MusicShopDB.h"

#define SQL_SIZE 600

const int MAX_VALID_YR = 9999;
const int MIN_VALID_YR = 2000;

char* MyCallAns = "NULL";
char LastDate[11];
char LastDate2[11];

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
	sprintf(sql, "SELECT CompactDisk.id, SUM(CUR.amount) as SOLD FROM CompactDisk "\
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


void Requests(sqlite3* db, int accessRights)
{
	printf("\n\nChoose requests: \n"\
		"1. Information about amount of sold & remaining compacts \n"\
		"2. Information about compact (amount, cost) that were sold in period \n"\
		"3. Information about compact, that was sold max times (*) \n"\
		"4. Information about amount of sold compacts of most popular performer (*) \n"\
		"5. Information about authors (amount & sum of sold compacts of authors) \n"\
		"6. Information of trade in period\n"\
		"7. Information of compact disk trade (*) \n");
	int answer;

	scanf("%d", &answer);
	switch (answer)
	{
	case 1:{
		if (accessRights == 0){
				printf("You don't have access for this request \n");
				break;
		}
		AllCompactTradeInfo(db);	
		break;
	}
	case 2: {

		if (accessRights == 0){
				printf("You don't have access for this request \n");
				break;
		}
		char date1[11], date2[11];
		int id;
		printf("Enter id of compact:\n");
		scanf("%d", &id);
		printf("Enter start of period in format HHHH.MM.DD:\n");
		scanf("%s", date1);
		printf("Enter end of period in format HHHH.MM.DD:\n");
		scanf("%s", date2);
		if(!checkDate(date1) || !checkDate(date2)) {
                printf("Invalid date!");
                break;
                }
		GetCostAndAmountonPeriod(db, id, date1, date2);
		break;
	}
	case 3:
		GetMostSoldCompactInfo(db);	
		break;
	case 4:
		GetMostPopularPerformerTradeInfo(db);	
		break;
	case 5:{
		if (accessRights == 0){
				printf("You don't have access for this request \n");
				break;
		}
		AllAuthorsInfo(db);	
		break;
	}
	case 6:{
		if (accessRights == 0){
				printf("You don't have access for this request \n");
				break;
		}
		char date1[11], date2[11];
		printf("Enter start of period in format HHHH.MM.DD:\n");
		scanf("%s", date1);
		printf("Enter end of period in format HHHH.MM.DD:\n");
		scanf("%s", date2);
		if(!checkDate(date1) || !checkDate(date2)) {
		printf("Invalid date!");
		break;
		}
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
	default:
		break;
	}
}

int checkDate(char date[]){
    int d = 0, m = 0, y = 0, counter = 0;
    char c[11];
    for(int i = 0; i < strlen(date); i++) {
        counter++;
        if(counter == 5 || counter == 8){
            if(date[i] != '.')
                return 0;
        }
        if(counter < 5) {
            if(date[i] > '9' || date[i] < '0')
                return 0;
            y *= 10;
            y += date[i] - '0';
        }
        if(counter > 5 && counter < 8) {
            if(date[i] > '9' || date[i] < '0')
                return 0;
            m *= 10;
            m +=  date[i] - '0';
        }
        if(counter > 8 && counter < 11) {
            if(date[i] > '9' || date[i] < '0')
                return 0;
            d *= 10;
            d +=  date[i] - '0';
        }
    }
    if (y > MAX_VALID_YR ||
        y < MIN_VALID_YR)
        return 0;
    if (m < 1 || m > 12)
        return 0;
    if (d < 1 || d > 31)
        return 0;
    if (m == 2)
    {
        if ((((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0)))
            if(d <= 29)
		return 1;
	    else
		return 0;
        else
            if(d <= 28)
		return 1;
	    else
		return 0;
    }
    
    if (m == 4 || m == 6 || m == 9 || m == 11)
        if(d <= 30)
	  return 1;
	else
	  return 0;
    return 1;
}

int Insert(sqlite3 *db, char *zErrMsg, int ret){
    char sql[SQL_SIZE] = "INSERT INTO ";
    char table[100];
    char columns[100];
    char values[100];
    int go = 1;

    while (go){
        //sprintf(sql, "%sINSERT INTO ", sql);
        printf("Enter the table: ");
	getchar();     
	fgets(table, 20, stdin);
        printf("Enter values, separated by ',': ");
	fgets(values, 100, stdin);
	if(!strncmp(table, "CompactDisk", 11))
	  sprintf(sql, "%s%s (id,date,company,price) VALUES (%s); ", sql, table, values);
	if(!strncmp(table, "MusicalComposition", 5))
	  sprintf(sql, "%s%s (name,author,performer,compactID) VALUES (%s); ", sql, table, values);
        if(!strncmp(table, "Trade", 5)) {
	  sprintf(sql, "%s%s (date,compactID,amount,code) VALUES (%s); ", sql, table, values);
		char date[13];
		int compactID = 0, amount = 0, code = 0;
		int pos = 0;
		int last;
		for(int i = 0; values[i] != '\0' && values[i] != '\n'; i++) {
		  if(values[i] == ',') {
		    last = i;
		    pos++;
		    continue;
		  }
		  if(pos == 0) {
		    date[i] = values[i];
		  }
		  if(pos == 1) {
		    compactID *= 10;	
		    compactID += values[i] - '0';
		  }
		  if(pos == 2) {
		    amount *= 10;	
		    amount += values[i] - '0';
		  }
		  if(pos == 3) {
		    code *= 10;	
		    code += values[i] - '0';
		  }	
		}
		GetLastDate(db);
		for(int i = 1; i < 11; i++)
			date[i-1] = date[i];
		date[10] = '\0';
		for(int i = 0; date[i] != '\0' ;i++) {
		  if(date[i] < LastDate2[i]) {
			printf("Invalid date!\n");
			return 0;
		  }
		}
		if(code == 2 && amount > HaveDisks(date, db)) {
			printf("Invalid amount!");
			return 0;
		}
	}
      	printf("Press 1 to continue, press 0 to exit: ");
        scanf("%d", &go);
        
    }
    ret = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (ret != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else{
        fprintf(stdout, "Done successfully!\n");
    }
    return 0;
}

static int callback2(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        MyCallAns = (argv[i] ? argv[i] : "NULL");
    }	
	return 0;
}

static int callback3(void* NotUsed, int argc, char** argv, char** azColName) {
    char* str = "2000.01.01";
    for (int i = 0; i < argc; i++) {
        str = (argv[i] ? argv[i] : "2000.01.01");
    }	
    for(int i = 0; str[i] != '\0'; i++)
	LastDate2[i] = str[i];
    return 0;
}

char* GetLastDate(sqlite3* db){
	char sql[256];
	sprintf(sql, 
	"SELECT MAX(date) FROM Trade;");
	char* zErrMsg = 0;
	int rc = sqlite3_exec(db, sql, callback3, (void*)NULL, &zErrMsg);
	return LastDate;
}

int HaveDisks(char *date, sqlite3* db){
	char sql[256];
	char* zErrMsg = 0; 
	sprintf(sql, 
	"SELECT SUM(amount) FROM Trade "\
	"WHERE Trade.code = 2 AND Trade.date <= '%s';", date
	);
	int rc = sqlite3_exec(db, sql, callback2, (void*)NULL, &zErrMsg);
	if (MyCallAns[0] == 'N'){
		return 0;
	}
	int ot = 0;
	for(int i = 0; MyCallAns[i] != '\0'; i++){
		ot *= 10;
		ot += (MyCallAns[i] - '0');
	}
	sprintf(sql, 
	"SELECT SUM(amount) FROM Trade "\
	"WHERE Trade.code = 1 AND Trade.date <= '%s';", date
	);
	zErrMsg = 0;
	rc = sqlite3_exec(db, sql, callback2, (void*)NULL, &zErrMsg);
	int ans =ot;
	ot = 0;
	for(int i = 0; MyCallAns[i] != '\0'; i++){
		ot *= 10;
		ot += (MyCallAns[i] - '0');
	}
	
	return ans - ot;
}	
int Delete(sqlite3 *db, char *zErrMsg, int ret){
    char sql[SQL_SIZE] = "DELETE FROM ";
    char table[20];
    char column[60];
    char value[40];
    printf("Enter the table: ");
    scanf("%s", table);
    if(!strncmp(table, "Trade", 5) || !strncmp(table, "TradeCodeInfo", 13)) {
	printf("You can't change this table!\n");
	return 0;
    }
    else {
    printf("Enter column: ");
    scanf("%s", &column);
    printf("Enter value: ");
    scanf("%s", &value);    
    sprintf(sql, "%s%s WHERE %s = %s", sql, table, column, value);
    ret = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (ret != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else{
        fprintf(stdout, "Done successfully!\n");
    }
    }
    return 0;
}
int Update(sqlite3* db, char *zErrMsg, int ret) {
    char sql[SQL_SIZE] = "UPDATE ";
    char table[20];
    char column[60];
    char value[40];
    char condition[60];
    char conValue[40];
    printf("Enter the table: ");
    scanf("%s", table);
    if(!strncmp(table, "Trade", 5) || !strncmp(table, "TradeCodeInfo", 13)) {
	printf("You can't change this table!\n");
	return 0;
    }
    else {
    printf("Enter column: ");
    scanf("%s", &column);
    printf("Enter value: ");
    scanf("%s", &value);    
    printf("Enter condition column: ");
    scanf("%s", &condition);
    printf("Enter condition value: ");
    scanf("%s", &conValue);
    sprintf(sql, "%s%s set %s = %s WHERE %s = %s", sql, table, column, value, condition, conValue);
    ret = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (ret != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else{
        fprintf(stdout, "Done successfully!\n");
    }
    }
    return 0;    
}
