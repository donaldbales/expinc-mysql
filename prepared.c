#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	gcc prepared.c -o prepared -I/usr/include/mysql -lmysqlclient
 */

int main(int argc, char const *argv[])
{
	char *plant_name;
	unsigned long plant_name_length;
	bool plant_name_is_null;
	bool plant_name_error;
	char *mysql_user;
	char *mysql_host;
	char *mysql_pwd;
	char *mysql_db;
	char *sql;
	int rc = 0;
	MYSQL *mysql = NULL;
	MYSQL_STMT *stmt = NULL;
	MYSQL_BIND bind[1];
	uint64_t affected_rows;

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
	printf("mysql_db=\"%s\"\n", mysql_db);

	mysql = mysql_init(NULL);

	printf("mysql=%p\n", (void *)mysql);

	if (!mysql_real_connect(mysql, mysql_host, mysql_user, mysql_pwd, mysql_db, 0, NULL, 0))
	{
		fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(mysql));
	}

	/*
		INSERT
	*/
	printf("Initialize a new INSERT prepared statement.\n");
	stmt = mysql_stmt_init(mysql);
	if (mysql_stmt_errno(stmt) != 0)
	{
		printf("%s\n", mysql_stmt_error(stmt));
	}
	printf("New prepared statement: %p\n", stmt);

	printf("Prepare the statement.\n");
	sql = malloc(81);
	strcpy(sql , "insert into ghdb (plant_name) values (?)");
	rc = mysql_stmt_prepare(stmt, sql, strlen(sql));
	if (mysql_stmt_errno(stmt) != 0)
	{
		printf("%s\n", mysql_stmt_error(stmt));
	}

	printf("Build the bind variable.\n");
	plant_name = malloc(31);
	strcpy(plant_name, "Strawberry");
	plant_name_length = strlen(plant_name);

	printf("Set the bind array to zeroes.\n");
	memset(bind, 0, sizeof(bind));

	printf("Initialize the bind structure.\n");
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = plant_name;
	bind[0].length = &plant_name_length;
	bind[0].is_null = (bool *) 0;

	printf("Bind the parameters.\n");
	mysql_stmt_bind_param(stmt, bind);
	if (mysql_stmt_errno(stmt) != 0)
	{
		printf("%s\n", mysql_stmt_error(stmt));
	}

	printf("Execute the prepared statement.\n");
	rc = mysql_stmt_execute(stmt);
	if (mysql_stmt_errno(stmt) != 0)
	{
		printf("%s\n", mysql_stmt_error(stmt));
	}

	affected_rows = mysql_stmt_affected_rows(stmt);
	printf("%ld row inserted.\n", affected_rows);

	printf("Close the prepared statement.\n");
	mysql_stmt_close(stmt);

	/*
		SELECT
	*/
	printf("Initialize a new SELECT prepared statement.\n");
	stmt = mysql_stmt_init(mysql);
	if (mysql_stmt_errno(stmt) != 0)
	{
		printf("%s\n", mysql_stmt_error(stmt));
	}
	printf("New prepared statement: %p\n", stmt);

	printf("Prepare the statement.\n");
	free(sql);
	sql = malloc(81);
	strcpy(sql , "select plant_name from ghdb");
	rc = mysql_stmt_prepare(stmt, sql, strlen(sql));
	if (mysql_stmt_errno(stmt) != 0)
	{
		printf("%s\n", mysql_stmt_error(stmt));
	}

	printf("Execute the prepared statement.\n");
	rc = mysql_stmt_execute(stmt);
	if (mysql_stmt_errno(stmt) != 0)
	{
		printf("%s\n", mysql_stmt_error(stmt));
	}

	printf("Build the bind variable.\n");
	free(plant_name);
	plant_name = NULL;
	plant_name_length = 0;
	plant_name_error = 0;

	printf("Set the bind array to zeroes.\n");
	memset(bind, 0, sizeof(bind));

	printf("Initialize the bind structure.\n");
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = 0;
	bind[0].buffer_length = 0;
	bind[0].length = &plant_name_length;
	bind[0].is_null = &plant_name_is_null;
	bind[0].error = &plant_name_error;

	printf("Bind the parameters.\n");
	mysql_stmt_bind_result(stmt, bind);
	if (mysql_stmt_errno(stmt) != 0)
	{
		printf("%s\n", mysql_stmt_error(stmt));
	}

	for (;;)
	{
		rc = mysql_stmt_fetch(stmt);

		if (rc == 1 || rc == MYSQL_NO_DATA)
		{
			break;
		}

		if (plant_name_length > 0)
		{
			plant_name = malloc(plant_name_length);
			bind[0].buffer = plant_name;
			bind[0].buffer_length = plant_name_length;
			mysql_stmt_fetch_column(stmt, bind, 0, 0);
			if (plant_name_is_null)
			{
				printf("plant_name is NULL\n");
			}
			else
			{
				printf("plant_name = \"%s\"\n", plant_name);
			}
		}
	}

	printf("Close the prepared statement.\n");
	mysql_stmt_close(stmt);

	mysql_close(mysql);

	return 0;
}