#include "App.h"
#include <sqlite3.h>
#include <iostream>

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main() {
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char* sql;
	const char* data = "Callback function called";

	/* Open database */
	rc = sqlite3_open("test.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Create SQL statement */
	//sql = "CREATE TABLE users(userID int, fname varchar, lname varchar,"
	//"dob date)";
	//sql = "Insert into users values(4, 'Bradley', 'Schmidt', '2004-05-22')";
	sql = "select * from users";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Operation done successfully\n");
	}
	sqlite3_close(db);
	return 0;


	App app;
	if(!app.init()) {
		ERROR("App", "init failed, exiting ...");
		return -1;
	}
	app.run();
	return 0;
}
