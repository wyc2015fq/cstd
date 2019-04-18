# PostgreSQL的 initdb 源代码分析之二十四 - weixin_33985507的博客 - CSDN博客
2013年07月09日 10:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
继续分析：
```
make_template0();
```
展开：
无需再作解释，就是创建template0数据库
```
/*
 * copy template1 to template0
 */
static void
make_template0(void)
{
    PG_CMD_DECL;
    const char **line;
    static const char *template0_setup[] = {
        "CREATE DATABASE template0;\n",
        "UPDATE pg_database SET "
        "    datistemplate = 't', "
        "    datallowconn = 'f' "
        "    WHERE datname = 'template0';\n",
        /*
         * We use the OID of template0 to determine lastsysoid
         */
        "UPDATE pg_database SET datlastsysoid = "
        "    (SELECT oid FROM pg_database "
        "    WHERE datname = 'template0');\n",
        /*
         * Explicitly revoke public create-schema and create-temp-table
         * privileges in template1 and template0; else the latter would be on
         * by default
         */
        "REVOKE CREATE,TEMPORARY ON DATABASE template1 FROM public;\n",
        "REVOKE CREATE,TEMPORARY ON DATABASE template0 FROM public;\n",
        "COMMENT ON DATABASE template0 IS 'unmodifiable empty database';\n",
        /*
         * Finally vacuum to clean up dead rows in pg_database
         */
        "VACUUM FULL pg_database;\n",
        NULL
    };
    fputs(_("copying template1 to template0 ... "), stdout);
    fflush(stdout);
    snprintf(cmd, sizeof(cmd),
             "\"%s\" %s template1 >%s",
             backend_exec, backend_options,
             DEVNULL);
    PG_CMD_OPEN;
    for (line = template0_setup; *line; line++)
        PG_CMD_PUTS(*line);
    PG_CMD_CLOSE;
    check_ok();
}
```
 由此而知，在initdb工作的时候，是先创建的template1数据库，再创建template01数据库。
