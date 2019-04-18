# sqlite3数据库-常用命令 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:09:31[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：416

# 用终端创建sqlite3数据库：
- sqlite3 database_name 创建database，例如：
`> sqlite3 d:\test.db 回车，就生成了一个test.db在d盘。    这样同时也SQLite3挂上了这个test.db`
- sqlite中命令以.开头,大小写敏感（数据库对象名称是大小写不敏感的），例如:
.exit 退出
.help 查看帮助 针对命令
.database 显示数据库信息；包含当前数据库的位置
.tables 或者 .table 显示表名称  没有表则不显示
`>select * from sqlite_master WHERE type = "table";   可以查询到当前数据库中所有表的详细结构信息`
.schema 命令可以查看创建数据对象时的SQL命令；
.schema database_name查看创建该数据库对象时的SQL的命令；如果没有这个数据库对象就不显示内容，不会有错误提示
.read FILENAME 执行指定文件中的SQL语句
.headers on/off  显示表头 默认off
.output 把查询输出到文件
`>.output 文件名`
`>查询语句； 查询结果就输出到了文件名指定的文件中`
`>.output stdout   把查询结果用屏幕输出`
- **可视化工具：sqliteman**
用sudo apt-get install sqliteman就可以安装。使用时用终端进入到数据库文件下，输入sqliteman 数据库名，直接打开数据库查看。
# python创建sqlite3数据库：
1、载入sqlite3包
import sqlite3
2、创建数据库
conn = sqlite3.connect(‘database_name.db’) # 如果当前路径下不存在，则创建一个database_name数据库
3、获得游标，执行sql命令，获取数据
curs = conn.cursor()
query=’’’ some sql codes here ‘’’
curs.execute(query) # 执行sql命令
data = curs.fetchall() # 获取数据
4、将pandas的DataFrame数据保存为sqlite数据库：
conn = sqlite3.connect('data_base.db')
df = pd.read_csv('data.csv') # 读取csv文件
df.columns = column_index # 读入sql后成为索引表头
df.to_sql('table_name',conn) # 保存在table_name表中
- **常见操作：**
1、删除存在的表：DROP TABLE IF EXISTS TableName
2、判断表是否存在，不存在则创建：
CREATE TABLE IF NOT EXISTS TableName(
  Id          INTEGER         PRIMARY KEY AUTOINCREMENT,
  Url         VARCHAR( 150 ),
  IsCrawled   BOOLEAN         NOT NULL,
  CreatedOn   DATETIME        NOT NULL,
  CrawledDate DATETIME
);
3、将数据载入表
query="INSERT INTO insitu VALUES(NULL,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"
curs.execute(query,data_to_store)
