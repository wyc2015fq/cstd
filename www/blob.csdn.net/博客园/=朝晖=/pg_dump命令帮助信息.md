# pg_dump命令帮助信息 - =朝晖= - 博客园
# [pg_dump命令帮助信息](https://www.cnblogs.com/dhcn/p/7106439.html)
仅为参考查阅方便，完全命令行帮助信息，**无阅读价值。**
pg_dump dumps a database as a text file or to other formats.
Usage:
  pg_dump [OPTION]... [DBNAME]
General options:
  -f, --file=FILENAME         output file name
  -F, --format=c|t|p          output file format (custom, tar, plain text)
  -v, --verbose               verbose mode
  -Z, --compress=0-9          compression level for compressed formats
  --lock-wait-timeout=TIMEOUT fail after waiting TIMEOUT for a table lock
  --help                      show this help, then exit
  --version                   output version information, then exit
Options controlling the output content:
  -a, --data-only             dump only the data, not the schema
  -b, --blobs                 include large objects in dump
  -c, --clean                 clean (drop) database objects before recreating
  -C, --create                include commands to create database in dump
  -E, --encoding=ENCODING     dump the data in encoding ENCODING
  -n, --schema=SCHEMA         dump the named schema(s) only
  -N, --exclude-schema=SCHEMA do NOT dump the named schema(s)
  -o, --oids                  include OIDs in dump
  -O, --no-owner              skip restoration of object ownership in
                              plain-text format
  -s, --schema-only           dump only the schema, no data
  -S, --superuser=NAME        superuser user name to use in plain-text format
  -t, --table=TABLE           dump the named table(s) only
  -T, --exclude-table=TABLE   do NOT dump the named table(s)
  -x, --no-privileges         do not dump privileges (grant/revoke)
  --binary-upgrade            for use by upgrade utilities only
  --inserts                   dump data as INSERT commands, rather than COPY
  --column-inserts            dump data as INSERT commands with column names
  --disable-dollar-quoting    disable dollar quoting, use SQL standard quoting
  --disable-triggers          disable triggers during data-only restore
  --no-tablespaces            do not dump tablespace assignments
  --role=ROLENAME             do SET ROLE before dump
  --use-set-session-authorization
                              use SET SESSION AUTHORIZATION commands instead of
                              ALTER OWNER commands to set ownership
Connection options:
  -h, --host=HOSTNAME      database server host or socket directory
  -p, --port=PORT          database server port number
  -U, --username=NAME      connect as specified database user
  -w, --no-password        never prompt for password
  -W, --password           force password prompt (should happen automatically)
If no database name is supplied, then the PGDATABASE environment
variable value is used.
Report bugs to <pgsql-bugs@postgresql.org>.
样例语句：
```
pg_dump -h localhost -p 5432 -U postgres -W -F t -b -v -f backup_filename.backup database_name
```

