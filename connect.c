#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	gcc connect.c -o connect -I/usr/include/mysql -lmysqlclient
 */

int main(int argc, char const *argv[])
{
	char *mysql_user;
	char *mysql_host;
	char *mysql_pwd;
	char *mysql_db;
	char *sql = "select * from information_schema.tables order by table_name";
	int r = 0;
	MYSQL *mysql;

	mysql_user = malloc(31);
	mysql_host = malloc(31);
	mysql_pwd = malloc(31);
	mysql_db = malloc(31);

	strcpy(mysql_user, getenv("MYSQL_USER"));
	strcpy(mysql_host, getenv("MYSQL_HOST"));
	strcpy(mysql_pwd, getenv("MYSQL_PWD"));
	strcpy(mysql_db, getenv("MYSQL_DB"));

	printf("mysql_user=\"%s\"\n", mysql_user);
	printf("mysql_host=\"%s\"\n", mysql_host);
	printf("mysql_pwd=\"%s\"\n", mysql_pwd);
	printf("mysql_db=\"%s\"\n", mysql_db);

	mysql = mysql_init(NULL);

	printf("mysql=%p\n", mysql);

	if (!mysql_real_connect(mysql, mysql_host, mysql_user, mysql_pwd, mysql_db, 0, NULL, 0))
	{
		fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(mysql));
	}

	r = mysql_real_query(mysql, sql, strlen(sql));
	if (r == 0)
	{
		printf("mysql_field_count=%ul\n", mysql_field_count(mysql));
	}

	mysql_close(mysql);

	return 0;
}