
# Loadrunner测试数据库性能，测试SQL语句的脚本例子 - anlcy - 博客园






# [Loadrunner测试数据库性能，测试SQL语句的脚本例子](https://www.cnblogs.com/camilla/p/10338884.html)
Loadrunner与SQL Server的操作可以通过录制的方式来实现，但本文还是通过直接调用loadrunner本身的function来实现sql语句的操作，
主要用到的是lr_db_connect和lr_db_executeSQLStatement两个函数.
1、脚本协议：选择web service
在函数lr_db_connect在协议选择的时候只能选择Web Services协议。
2、脚本，以测试MySQL为例：
/*需要的表结构如下
CREATE TABLE `test_data` (
`order_id` BIGINT UNSIGNED NOT NULL COMMENT 'Order numbers. Must be unique.',
`status` BOOL NOT NULL DEFAULT '0' COMMENT 'Whether data has been used or not. A value of 0 means FALSE.',
`date_used` DATETIME NULL COMMENT 'Date/time that the data was used.',
UNIQUE (
`order_id`
)
) ENGINE = innodb COMMENT = 'LoadRunner test data';
*/
Action()
{
int rc;
int db_connection; // 数据库连接
int query_result; // 查询结果集 MYSQL_RES
char** result_row; // 查询的数据衕
char *server = "localhost";
char *user = "root";
char *password = "123456789";
char *database = "test";
int port = 3306;
int unix_socket = NULL;
int flags = 0;
// 找到libmysql.dll的所在位置.
rc = lr_load_dll("C://Program Files//MySQL//MySQL Server 5.1//bin//libmysql.dll");
if (rc != 0) {
lr_error_message("Could not load libmysql.dll");
lr_abort();
}
// 创建MySQL对象
db_connection = mysql_init(NULL);
if (db_connection == NULL) {
lr_error_message("Insufficient memory");
lr_abort();
}
// 连接到MySQL数据库
rc = mysql_real_connect(db_connection, server, user, password, database, port, unix_socket, flags);
if (rc == NULL) {
lr_error_message("%s", mysql_error(db_connection));
mysql_close(db_connection);
lr_abort();
}
// 向数据库插入数据
// 此处的 {ORDER_ID} 是一个参数，简单测试时可以用一个常数代替
lr_save_string (lr_eval_string("INSERT INTO test_data (order_id) VALUES ({ORDER_ID})"),"paramInsertQuery");
rc = mysql_query(db_connection, lr_eval_string("{paramInsertQuery}"));
if (rc != 0) {
lr_error_message("%s", mysql_error(db_connection));
mysql_close(db_connection);
lr_abort();
}
// 从数据库读取一个数据并显示
rc = mysql_query(db_connection, "SELECT order_id FROM test_data WHERE status IS FALSE LIMIT 1");
if (rc != 0) {
lr_error_message("%s", mysql_error(db_connection));
mysql_close(db_connection);
lr_abort();
}
query_result = mysql_use_result(db_connection);
if (query_result == NULL) {
lr_error_message("%s", mysql_error(db_connection));
mysql_free_result(query_result);
mysql_close(db_connection);
lr_abort();
}
// 如果结果集包含多行数据，需要多次调用 mysql_fetch_row 直到返回NULL
result_row = (char **)mysql_fetch_row(query_result);
if (result_row == NULL) {
lr_error_message("Did not expect the result set to be empty");
mysql_free_result(query_result);
mysql_close(db_connection);
lr_abort();
}
// 保存参数，用于删除这行数据
lr_save_string(result_row[0], "paramOrderID");
lr_output_message("Order ID is: %s", lr_eval_string("{paramOrderID}"));
mysql_free_result(query_result);
// 在事务里更新一行数据，需要用InnoDB引擎
rc = mysql_query(db_connection, "BEGIN"); //启动事务
if (rc != 0) {
lr_error_message("%s", mysql_error(db_connection));
mysql_close(db_connection);
lr_abort();
}
// 使用 "FOR UPDATE" 锁住要更新的数据行
rc = mysql_query(db_connection, "SELECT order_id FROM test_data WHERE status IS FALSE LIMIT 1 FOR UPDATE");
if (rc != 0) {
lr_error_message("%s", mysql_error(db_connection));
mysql_close(db_connection);
lr_abort();
}
query_result = mysql_use_result(db_connection);
if (query_result == NULL) {
lr_error_message("%s", mysql_error(db_connection));
mysql_free_result(query_result);
mysql_close(db_connection);
lr_abort();
}
result_row = (char **)mysql_fetch_row(query_result);
if (result_row == NULL) {
lr_error_message("没有查询到结果");
mysql_free_result(query_result);
mysql_close(db_connection);
lr_abort();
}
lr_save_string(result_row[0], "paramOrderID");
lr_output_message("Order ID is: %s", lr_eval_string("{paramOrderID}"));
mysql_free_result(query_result);
lr_save_string(lr_eval_string("UPDATE test_data SET status=TRUE, date_used=NOW() WHERE order_id='{paramOrderID}'"),"paramUpdateQuery");
rc = mysql_query(db_connection, lr_eval_string("{paramUpdateQuery}"));
if (rc != 0) {
lr_error_message("%s", mysql_error(db_connection));
mysql_close(db_connection);
lr_abort();
}
rc = mysql_query(db_connection, "COMMIT"); // 提交事务
if (rc != 0) {
lr_error_message("%s", mysql_error(db_connection));
mysql_close(db_connection);
lr_abort();
}
// 再次查找数据，应该为空了，因为前面的事务更新了标志
rc = mysql_query(db_connection, "SELECT order_id FROM test_data WHERE status IS FALSE LIMIT 1");
if (rc != 0) {
lr_error_message("%s", mysql_error(db_connection));
mysql_close(db_connection);
lr_abort();
}
query_result = mysql_use_result(db_connection);
if (query_result == NULL) {
lr_error_message("%s", mysql_error(db_connection));
mysql_free_result(query_result);
mysql_close(db_connection);
lr_abort();
}
result_row = (char **)mysql_fetch_row(query_result);
if (result_row == NULL) {
lr_output_message("Result set is empty as expected");
mysql_free_result(query_result);
} else {
lr_error_message("Did not expect the result set to contain any rows");
mysql_free_result(query_result);
mysql_close(db_connection);
lr_abort();
}
// 删除数据
lr_save_string(lr_eval_string("DELETE FROM test_data WHERE order_id = '{paramOrderID}'"),"paramDeleteQuery");
rc = mysql_query(db_connection, lr_eval_string("{paramDeleteQuery}"));
if (rc != 0) {
lr_error_message("%s", mysql_error(db_connection));
mysql_close(db_connection);
lr_abort();
}
// 释放MySQL资源
mysql_close(db_connection);
return 0;
}
2.sql和OLEDB的例子，ConnectionType=OLEDB时ConnectionString后面必须指定Provider属性，lr_db_connect中
的ConnectionName可以随意指定，但是lr_db_executeSQLStatement中的ConnectionName必须和其保持一致。
Action()
{
int NumRows=0;
double times=0;
int i=1;
//建立连接,ConnectionType=SQL
lr_db_connect("StepName=DatabaseConnection",
"ConnectionString=Data Source=172.16.1.215;Initial Catalog=LayIM;Persist Security Info=True;User ID=sa;Password=sa",
"ConnectionName=MyCon",
"ConnectionType=SQL",
LAST );
/*建立连接，ConnectionType=OLEDB
lr_db_connect("StepName=Connect",
"ConnectionString=Provider=SQLOLEDB;Data Source=.;Initial Catalog=my_test;User Id=sa;Password=123456",
"ConnectionName=MyCon",
"ConnectionType=OLEDB", //ConnectionType=SQL时，ConnectionString不可以带有Provider；ConnectionType=OLEDB时，必须带Provider
LAST);
*/
//执行SQL
lr_start_transaction("SQL查询");
NumRows = lr_db_executeSQLStatement("StepName=PerformQuery",
"ConnectionName=MyCon",
// 数据库语句
"SQLStatement=SELECT * FROM layim_user ORDER BY id DESC ",
"DatasetName=MyDataSet",
LAST );
times=lr_get_transaction_duration("SQL查询");
lr_end_transaction("SQL查询", LR_AUTO);
lr_output_message("The query returned %d rows.", NumRows);
lr_output_message("SQL查询语句执行时间 %f ", times);
//打印查询结果
/*lr_db_dataset_action("StepName=PrintDataset",
"DatasetName=MyDataSet",
"Action=PRINT",
LAST );*/
//获取当前第一条记录
lr_db_getvalue("StepName=GetValue",
"DatasetName=MyDataSet",
"Column=nickname",
"Row=current",
"OutParam=MyOutputParam",
LAST );
// 输出当前查询记录
lr_output_message("The value is: %s", lr_eval_string("{MyOutputParam}") );
while (i<=10) {//输出10条记录
lr_db_getvalue("StepName=GetValue",
"DatasetName=MyDataSet",
"Column=nickname",
"Row=next",
"OutParam=MyOutputParam",
LAST);
lr_output_message("The value %d is: %s", i, lr_eval_string("{MyOutputParam}") );
i=i+1;
}
lr_output_message("The query returned %d rows.", NumRows);
//释放结果
/*lr_db_dataset_action("StepName=RemoveDataset",
"DatasetName=MyDataSet",
"Action=REMOVE",
LAST);*/
lr_start_transaction("SQL插入");
NumRows = lr_db_executeSQLStatement("StepName=Insert",
"ConnectionName=MyCon",
// Insert语句，本例中第一列id是自增主键
"SQLStatement=Insert into layim_user values ('test','123456','test888888','','/test/test',getdate(),'111111',0) ",
"DatasetName=MyDataSet",
LAST );
times=lr_get_transaction_duration("SQL插入");
lr_end_transaction("SQL插入", LR_AUTO);
lr_output_message("The insert date %d rows.", NumRows);
lr_output_message("SQL插入语句执行时间 %f ", times);
//关闭链接
lr_db_disconnect("stepname=Disconnect","connectionname=MyCon",LAST);
return 0;
}





