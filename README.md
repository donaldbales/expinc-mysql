expinc-mysql
# Experiments in C and MySQL

## Introduction
This is a set of preliminary experiments using the C programming language and the MySQL C API.

Before I coded SQL statements using C and the MySQL C API for some C cgi-bin programs, I wanted to refresh my skills on the use of prepared SQL statements using those two technologies. Why? "Using prepared statements in MySQL prevents SQL injection by separating the SQL query structure from user input, treating the input strictly as data rather than executable code. This ensures that even if malicious input is provided, it cannot alter the intended SQL command." --SA

For my cgi-bin applications, I need to INSERT, UPDATE, SELECT and DELETE data using SQL. These experiments help me clarify my coding, before I code the cgi-bin programs.

## Reference

MySQL 8.0 C API Developer Guide <https://dev.mysql.com/doc/c-api/8.0/en/>

## Find MySQL on my Development Machine (Raspberry Pi)
```
find / -name \*mysql\* -type f -exec ls -lap {} \; 2>/dev/null

# Find the MySQL client library
find / -name libmysqlclient\* -type f -exec ls -lap {} \; 2>/dev/null

...
-rw-r--r-- 1 root root 6805112 Aug 26 05:41 /usr/lib/aarch64-linux-gnu/libmysqlclient.so.21.2.46
-rw-r--r-- 1 root root 8777528 Aug 26 05:41 /usr/lib/aarch64-linux-gnu/libmysqlclient.a

# Which header file
find /usr/include/mysql -name \*.h -type f -exec grep -H mysql_real_connect {} \;

```

## Notes on Compiling and Linking
```
# Include the MySQL header files 
-I/usr/include/mysql

# Link the MySQL client library
-lmysqlclient

$ mysql_config
Usage: /usr/bin/mysql_config [OPTIONS]
Compiler: GNU 13.3.0
Options:
        --cflags         [-I/usr/include/mysql ]
        --cxxflags       [-I/usr/include/mysql ]
        --include        [-I/usr/include/mysql]
        --libs           [-L/usr/lib/aarch64-linux-gnu -lmysqlclient -lz  -lzstd -lssl -lcrypto -lresolv -lm]
        --libs_r         [-L/usr/lib/aarch64-linux-gnu -lmysqlclient -lz  -lzstd -lssl -lcrypto -lresolv -lm]
        --plugindir      [/usr/lib/mysql/plugin]
        --socket         [/var/run/mysqld/mysqld.sock]
        --port           [0]
        --version        [8.0.46]
        --variable=VAR   VAR is one of:
                pkgincludedir [/usr/include/mysql]
                pkglibdir     [/usr/lib/aarch64-linux-gnu]
                plugindir     [/usr/lib/mysql/plugin]

# Use back tics to include inline
$ mysql_config --cflags
-I/usr/include/mysql

$ mysql_config --libs
-L/usr/lib/aarch64-linux-gnu -lmysqlclient -lz  -lzstd -lssl -lcrypto -lresolv -lm

# example:
gcc -o progname progname.o `mysql_config --libs`

# You may need to add the path of the directory where libmysqlclient.so 
# is located to the LD_LIBRARY_PATH or LD_LIBRARY environment variable.
```

## API Commands in Linear Order of Use
```
MYSQL * mysql_init(MYSQL *mysql)

MYSQL * mysql_real_connect(MYSQL *mysql,
                           char *host,
                           char *user,
                           char *passwd,
                           char *db,
                           unsigned int port, // 0
                           char *unix_scoket, // NULL
                           unsigned long client_flag) // 0

bool mysql_autocommit(MYSQL *mysql,
                      bool mode)

int mysql_ping(MYSQL *mysql)

bool mysql_commit(MYSQL *mysql)

bool mysql_rollback(MYSQL *mysql)

void mysql_close(MYSQL *mysql)

void mysql_library_end()

#
# Prepared statements
#
MYSQL_STMT * mysql_stmt_init()

bool mysql_stmt_attr_get(MYSQL_STMT *stmt,
                         enum enum_stmt_attr_type option,
                         void *arg)
bool mysql_stmt_attr_set(MYSQL_STMT *stmt,
                         enum enum_stmt_attr_type option,
                         void *arg)

int mysql_stmt_prepare(MYSQL_STMT *stmt,
                       char *stmt_str,
                       unsiged long length);

unsigned int mysql_stmt_field_count(MYSQL_STMT *stmt)

MYSQL_BIND bind[3]
memset(bind, 0, sizeof(bind));
bool mysql_stmt_bind_param(MYSQL_STMT *stmt,
                           MYSQL_BIND *bind)
bool mysql_stmt_bind_result(MYSQL_STMT *stmt,
                            MYSQL_BIND *bind)

mysql_stmt_send_long_data() // for large binary or text

int mysql_stmt_exec(*stmt)

MYSQL_RES * mysql_stmt_result_metadata() // if needed

uint64_t mysql_stmt_affected_rows(MYSQL_STMT *stmt)

unsiged int mysql_stmt_errno(MYSQL_STMT *stmt)
char * mysql_stmt_error(MYSQL_STMT *stmt)


int mysql_stmt_fetch(MYSQL_STMT *stmt) // next

int mysql_stmt_fetch_column(MYSQL_STMT *stmt,
                            MYSQL_BIND *bind,
                            unsignedint column,
                            unsigned long offset) //

mysql_stmt_close() // only when done with stmt

bool mysql_free_result(MYSQL_STMT *stmt)

#
# Query
#
int mysql_real_query(MYSQL *mysql,
                     char *stmt_str,
                     unsigned long length)


unsigned int mysql_field_count(MYSQL *mysql)

# these two work together
MYSQL_RES * mysql_store_result(MYSQL *mysql)
uint64_t mysql_num_rows(MYSQL_RES *result)

-OR-

# these next two work together
MYSQL_RES * mysql_use_result(MYSQL *mysql)
MYSQL_ROW mysql_fetch_row(MYSQL_RES *result) // next

uint64_t mysql_insert_id(MYSQL *mysql)

uint64_t mysql_affected_rows(MYSQL *mysql)

MYSQL_FIELD * mysql_fetch_field(MSQL_RES *result) // next

MYSQL_FIELD * mysql_fetch_field_direct(MSQL_RES *result,
                                       unsigned int fieldnr)

MYSQL_FIELD * mysql_fetch_fields(MYSQL_RES *result)

unsigned long * mysql_fetch_length(MYSQL_RES *result)

void mysql_free_result(MYSQL_RES *result)

```