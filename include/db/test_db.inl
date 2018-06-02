
#include "sqlite.inl"

static int _sql_callback(void* notused, int argc, char** argv, char** szColName)
{
  int i = 0;

  printf("notused:0x%x, argc:%d\n", notused, argc);

  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", szColName[i], argv[i] == 0 ? "NULL" : argv[i]);
  }

  printf("\n");
  return 0;
}

#include <direct.h>

int test_sqlite()
{
  const char* sSQL1 = "create table users(userid varchar(20) PRIMARY KEY, age int, birthday datetime);";
  const char* sSQL2 = "insert into users values('wang', 20, '1989-5-4');";
  const char* sSQL3 = "select * from users;";

  sqlite3* db = 0;
  char* pErrMsg = 0;
  int ret = 0;
  _chdir("E:/soft/数据库/sqlitebrowser_200_b1_win");

  //连接数据库
  ret = sqlite3_open("./1.db", &db);

  if (ret != SQLITE_OK) {
    fprintf(stderr, "无法打开数据库：%s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  printf("数据库连接成功\n");

  sSQL1 = "select * from TWORD;";
  //执行建表SQL
  ret = sqlite3_exec(db, sSQL1, _sql_callback, 0, &pErrMsg);

  if (ret != SQLITE_OK) {
    fprintf(stderr, "SQL create error: %s\n", pErrMsg);
    sqlite3_free(pErrMsg); //这个要的哦，要不然会内存泄露的哦！！！
    sqlite3_close(db);
    return 1;
  }

  printf("数据库建表成功！！\n");

  //执行插入数据
  ret = sqlite3_exec(db, sSQL2, _sql_callback, 0, &pErrMsg);

  if (ret != SQLITE_OK) {
    fprintf(stderr, "SQL insert error: %s\n", pErrMsg);
    sqlite3_free(pErrMsg); //这个要的哦，要不然会内存泄露的哦！！！
    sqlite3_close(db);
    return 1;
  }

  printf("数据库插入数据成功！\n");

  //执行查询操作
  ret = sqlite3_exec(db, sSQL3, _sql_callback, 0, &pErrMsg);

  if (ret != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", pErrMsg);
    sqlite3_free(pErrMsg);
    sqlite3_close(db);
    return 1;
  }

  printf("数据库查询成功！！\n");

  //关闭数据库
  sqlite3_close(db);
  db = 0;

  return 0;
}

int test_db()
{
  test_sqlite();
  return 0;
}
