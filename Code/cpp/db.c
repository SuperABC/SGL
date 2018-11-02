#include "..\include\db.h"


MYSQL *connectMysql(MYSQL *con,
	const char *host,
	const char *user,
	const char *passwd,
	const char *db,
	unsigned int port) {

	MYSQL *ret;
	ret = mysql_init((MYSQL*)0);
	if (!ret)return NULL;

	mysql_real_connect(con, host, user, passwd, db, port, NULL, 0);
	return con;
}
int useDatabase(MYSQL *mysql, const char *db) {
	return mysql_select_db(mysql, db);
}
int executeQuery(MYSQL *mysql, const char *query) {
	return mysql_real_query(mysql, query, (unsigned long)strlen(query));
}
MYSQL_RES *queryResult(MYSQL *mysql) {
	return mysql_store_result(mysql);
}
MYSQL_ROW nextRow(MYSQL_RES *res) {
	return mysql_fetch_row(res);
}
int fieldNum(MYSQL_RES *res) {
	return mysql_num_fields(res);
}
void freeSQLResult(MYSQL_RES *res) {
	mysql_free_result(res);
}
void closeMysql(MYSQL *mysql) {
	mysql_close(mysql);
}
