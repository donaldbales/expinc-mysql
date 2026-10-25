# expinc-mysql
Experiments in C and MySQL


Reference: MySQL 8.0 C API Developer Guide, Chapter 3

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

Find MySQl on my Development Machine
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