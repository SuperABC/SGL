/*
* Copyright (c) 2016-2018, Super GP Individual.
* All Rights Reserved.
*
* Permission to use, copy, modify, and distribute this library for any
* purpose and without fee is hereby granted, provided that the above
* copyright notice appear in all copies and that both the copyright
* notice and this permission notice appear in supporting documentation,
* and that the name of SGL not be used in advertising or publicity
* pertaining to distribution of the software without specific,
* written prior permission.
*/



#ifndef SGL_DB_H
#define SGL_DB_H


#pragma comment(lib, "libmysql.lib")

#include "mysql.h"


MYSQL *connectMysql(MYSQL *con,
	const char *host,
	const char *user,
	const char *passwd,
	const char *db,
	unsigned int port);
int useDatabase(MYSQL *mysql, const char *db);
int executeQuery(MYSQL *mysql, const char *query);
MYSQL_RES *queryResult(MYSQL *mysql);
MYSQL_ROW nextRow(MYSQL_RES *res);
int fieldNum(MYSQL_RES *res);
void freeSQLResult(MYSQL_RES *res);
void closeMysql(MYSQL *mysql);


#endif



