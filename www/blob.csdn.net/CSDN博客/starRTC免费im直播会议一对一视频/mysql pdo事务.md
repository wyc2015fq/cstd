# mysql pdo事务 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月31日 11:12:13[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：35
/* 开始一个事务，关闭自动提交 */直到调用commit结束事务时才提交
$dbh->beginTransaction();
bool PDO::commit ( void ) 提交一个事务，数据库连接返回到自动提交模式直到下次调用 [PDO::beginTransaction()](http://php.net/manual/zh/pdo.begintransaction.php) 开始一个新的事务为止。
rollBack — 回滚一个事务
当在一个事务内有类似删除或创建数据表等 DLL 语句时，将无法回滚此事务范围内的任何更改。
当脚本结束或连接即将被关闭时，如果尚有一个未完成的事务，那么 PDO 将自动回滚该事务。这种安全措施有助于在脚本意外终止时避免出现不一致的情况——如果没有显式地提交事务，那么假设是某个地方出错了，所以执行回滚来保证数据安全。
try {  
  $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
  $dbh->beginTransaction();
  $dbh->exec("insert into staff (id, first, last) values (23, 'Joe', 'Bloggs')");
  $dbh->exec("insert into salarychange (id, amount, changedate) 
      values (23, 50000, NOW())");
  $dbh->commit();
} catch (Exception $e) {
  $dbh->rollBack();
  echo "Failed: " . $e->getMessage();
}
