# Haskell语言学习笔记（68）HDBC - zwvista - 博客园

## [Haskell语言学习笔记（68）HDBC](https://www.cnblogs.com/zwvista/p/8228283.html)

### 安装 HDBC-Sqlite3

```
$ cabal install HDBC-Sqlite3
Installed HDBC-sqlite3-2.3.3.1
Prelude> :m Database.HDBC Database.HDBC.Sqlite3
Prelude Database.HDBC Database.HDBC.Sqlite3>
```

### DB 操作

```
Prelude Database.HDBC Database.HDBC.Sqlite3> conn <- connectSqlite3 "test1.db" -- 连接或创建数据库
Prelude Database.HDBC Database.HDBC.Sqlite3> run conn "CREATE TABLE test (id INTEGER NOT NULL, desc VARCHAR(80))" [] -- 建表
0
Prelude Database.HDBC Database.HDBC.Sqlite3> run conn "INSERT INTO test (id) VALUES (0)" [] -- 插入记录
1
Prelude Database.HDBC Database.HDBC.Sqlite3> commit conn
Prelude Database.HDBC Database.HDBC.Sqlite3> run conn "INSERT INTO test VALUES (?, ?)" [toSql 0, toSql "zero"] -- 插入记录，使用参数
1
Prelude Database.HDBC Database.HDBC.Sqlite3> commit conn
Prelude Database.HDBC Database.HDBC.Sqlite3> stmt <- prepare conn "INSERT INTO test VALUES (?, ?)" -- 插入记录，使用语句
Prelude Database.HDBC Database.HDBC.Sqlite3> execute stmt [toSql 1, toSql "one"] 
1
Prelude Database.HDBC Database.HDBC.Sqlite3> execute stmt [toSql 2, toSql "two"] 
1
Prelude Database.HDBC Database.HDBC.Sqlite3> execute stmt [toSql 3, toSql "three"]
1
Prelude Database.HDBC Database.HDBC.Sqlite3> execute stmt [toSql 4, SqlNull]
1
Prelude Database.HDBC Database.HDBC.Sqlite3> commit conn
Prelude Database.HDBC Database.HDBC.Sqlite3> stmt <- prepare conn "INSERT INTO test VALUES (?, ?)"
Prelude Database.HDBC Database.HDBC.Sqlite3> executeMany stmt [[toSql 5, toSql "five's nice"], [toSql 6, SqlNull]] -- 插入多条记录，使用语句
Prelude Database.HDBC Database.HDBC.Sqlite3> commit conn
Prelude Database.HDBC Database.HDBC.Sqlite3> quickQuery' conn "SELECT * from test where id < 2" [] -- 查询记录
[[SqlInt64 0,SqlNull],[SqlInt64 0,SqlByteString "zero"],[SqlInt64 1,SqlByteString "one"]]
Prelude Database.HDBC Database.HDBC.Sqlite3> quickQuery' conn "SELECT id, desc from test where id <= ? ORDER BY id, desc" [toSql 2] -- 查询记录，使用参数
[[SqlInt64 0,SqlNull],[SqlInt64 0,SqlByteString "zero"],[SqlInt64 1,SqlByteString "one"],[SqlInt64 2,SqlByteString "two"]]
Prelude Database.HDBC Database.HDBC.Sqlite3> stmt <- prepare conn "SELECT * from test where id < 2" -- 查询记录
Prelude Database.HDBC Database.HDBC.Sqlite3> execute stmt []
0
Prelude Database.HDBC Database.HDBC.Sqlite3> results <- fetchAllRowsAL stmt -- 惰性读取
Prelude Database.HDBC Database.HDBC.Sqlite3> mapM_ print results
[("id",SqlInt64 0),("desc",SqlNull)]
[("id",SqlInt64 0),("desc",SqlByteString "zero")]
[("id",SqlInt64 1),("desc",SqlByteString "one")]
```

### DB 操作 2

```
Prelude Database.HDBC Database.HDBC.Sqlite3> getTables conn
["test"]
Prelude Database.HDBC Database.HDBC.Sqlite3> proxiedClientName conn
"sqlite3"
Prelude Database.HDBC Database.HDBC.Sqlite3> dbServerVer conn
"3.19.3"
Prelude Database.HDBC Database.HDBC.Sqlite3> dbTransactionSupport conn
True
Prelude Database.HDBC Database.HDBC.Sqlite3> quickQuery' conn "SELECT * from test2" []
*** Exception: SqlError {seState = "", seNativeError = 1, seErrorMsg = "prepare 20: SELECT * from test2: no such table: test2"}
Prelude Database.HDBC Database.HDBC.Sqlite3> handleSqlError $ quickQuery' conn "SELECT * from test2" []
*** Exception: user error (SQL error: SqlError {seState = "", seNativeError = 1, seErrorMsg = "prepare 20: SELECT * from test2: no such table: test2"})
Prelude Database.HDBC Database.HDBC.Sqlite3> disconnect conn
```


