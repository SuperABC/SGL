#include "winsgl.h"
#include "db.h"

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	initKey();

	MYSQL * con;
	MYSQL_RES *res;
	MYSQL_ROW row;

	int count = 0, t;

	con = mysql_init((MYSQL*)0);

	connectMysql(con, "localhost", "root", "wangyilin", "test", 3306);
	useDatabase(con, "test");

	executeQuery(con, "insert into book values(\'abc\')");
	executeQuery(con, "select * from book");

	res = queryResult(con);
	while (row = nextRow(res)) {
		for (t = 0; t < fieldNum(res); t++) {
			debugf("%s ", row[t]);
		}
		count++;
	}

	debugf("number of rows %d\n", count);
}
void sgLoop() {
	return;
}