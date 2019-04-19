# pg_restore数据库恢复指令 - =朝晖= - 博客园
# [pg_restore数据库恢复指令](https://www.cnblogs.com/dhcn/p/7116201.html)
pg_restore restores a PostgreSQL database from an archive created by pg_dump.
Usage:
  pg_restore [OPTION]... [FILE]
General options:
  -d, --dbname=NAME        connect to database name
  -f, --file=FILENAME      output file name
  -F, --format=c|d|t       backup file format (should be automatic)
  -l, --list               print summarized TOC of the archive
  -v, --verbose            verbose mode
  --help                   show this help, then exit
  --version                output version information, then exit
Options controlling the restore:
  -a, --data-only          restore only the data, no schema
  -c, --clean              clean (drop) database objects before recreating
  -C, --create             create the target database
  -e, --exit-on-error      exit on error, default is to continue
  -I, --index=NAME         restore named index
  -j, --jobs=NUM           use this many parallel jobs to restore
  -L, --use-list=FILENAME  use table of contents from this file for
                           selecting/ordering output
  -n, --schema=NAME        restore only objects in this schema
  -O, --no-owner           skip restoration of object ownership
  -P, --function=NAME(args)
                           restore named function
  -s, --schema-only        restore only the schema, no data
  -S, --superuser=NAME     superuser user name to use for disabling triggers
  -t, --table=NAME         restore named table
  -T, --trigger=NAME       restore named trigger
  -x, --no-privileges      skip restoration of access privileges (grant/revoke)
  -1, --single-transaction
                           restore as a single transaction
  --disable-triggers       disable triggers during data-only restore
  --no-data-for-failed-tables
                           do not restore data of tables that could not be
                           created
  --no-security-labels     do not restore security labels
  --no-tablespaces         do not restore tablespace assignments
  --use-set-session-authorization
                           use SET SESSION AUTHORIZATION commands instead of
                           ALTER OWNER commands to set ownership
Connection options:
  -h, --host=HOSTNAME      database server host or socket directory
  -p, --port=PORT          database server port number
  -U, --username=NAME      connect as specified database user
  -w, --no-password        never prompt for password
  -W, --password           force password prompt (should happen automatically)
  --role=ROLENAME          do SET ROLE before restore
If no input file name is supplied, then standard input is used.
Report bugs to <pgsql-bugs@postgresql.org>.
额外参考：https://xly3000.wordpress.com/2012/03/07/postgresql%E8%87%AA%E5%8A%A8%E5%A4%87%E4%BB%BDbackup%E4%B8%8E%E6%81%A2%E5%A4%8Drestore%E6%95%B0%E6%8D%AE%E5%BA%93%E5%9B%BE%E8%A7%A3/
