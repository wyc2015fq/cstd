# 使用 ProxySQL 改进 MySQL SSL 的连接性能 - 文章 - 伯乐在线
原文出处： [Percona](https://www.percona.com/blog/2017/09/19/proxysql-improves-mysql-ssl-connections/)   译文出处：[开源中国](https://www.oschina.net/translate/proxysql-improves-mysql-ssl-connections)
在这篇博文里，我们准备来看一下 ProxySQL 是如何提高 MySQL 的 SSL 连接性能的。
当用 SSL 部署 MySQL 时，主要的问题是如果没有使用连接池（例如：PHP 中的 mysqlnd-mux，python中 的 mysql.connector.pooling 等等），初始握手会造成大量的开销。不断的关闭连接、创建新的连接会对你的查询时间造成很大的影响。客户和同事最近让我知道了，虽然你可以用现代的英特尔处理器 AES-NI 硬件扩展来提升 SSL 加密/解密性能，但实际上，建立 SSL 连接时的开销来自于多次服务器和客户端建立连接时需要的握手。
使用 ProxySQL 支持 SSL 的后端连接和连接池，我们可以将它应用在任何应用程序上、同一个服务器（说明如下）：
![](http://jbcdn2.b0.upaiyun.com/2017/10/0ff1c84684fcf49df0e2098f6d674cb8.png)
在这种情况下，ProxySQL 运行在同一台服务器上的一个应用下，并且通过本地套接字与 MySQL 连接。MySQL 数据就不需要通过不安全的 TCP 流进行传输了。
我使用一个 PHP 脚本简单地在一个单线程中创建了 1 万个连接，来看看性能表现如何：


```
<?php $i = 10000;
$user = 'percona';
$pass = 'percona';
while($i>=0) {
$mysqli = mysqli_init();
// Use SSL
//$link = mysqli_real_connect($mysqli, "192.168.56.110", $user, $pass, "", 3306, "", MYSQL_CLIENT_SSL)
// No SSL
//$link = mysqli_real_connect($mysqli, "192.168.56.110", $user, $pass, "", 3306 )
// OpenVPN
//$link = mysqli_real_connect($mysqli, "10.8.99.1",      $user, $pass, "", 3306 )
// ProxySQL
$link = mysqli_real_connect($mysqli, "localhost",      $user, $pass, "", 6033, "/tmp/proxysql.sock")
or die(mysqli_connect_error());
$info = mysqli_get_host_info($mysqli);
$i--;
mysqli_close($mysqli);
unset($mysqli);
}
?>
```
没有使用 SSL 直接连接 MySQL：


```
[root@ad ~]# time php php-test.php
real 0m20.417s
user 0m0.201s
sys 0m3.396s
```
使用 SSL 直接连接 MySQL：


```
[root@ad ~]# time php php-test.php
real1m19.922s
user0m29.933s
sys0m9.550s
```
在 OpenVPN 隧道下没有使用 SSL 直接连接 MySQL：


```
[root@ad ~]# time php php-test.php
real 0m15.161s
user 0m0.493s
sys 0m0.803s
```
接下来，使用 ProxySQL 通过本地套接字文件连接：


```
[root@ad ~]# time php php-test.php
real0m2.791s
user0m0.402s
sys0m0.436s
```
下面是这些数据的对比图:
![](http://jbcdn2.b0.upaiyun.com/2017/10/5d3dfb8dfe9f1d74d109dc32e17dea5c.png)
如你所见，使用 SSL 和不使用 SSL 情况下的开销差距大约是 400% —— 对某些负载来说还是很可怕的。
通过 OpenVPN 连接大体上都比 SSL 好。有趣的是，OpenVPN 服务器需要部署在另一台服务器上，与 MySQL 服务器和应用分开。
这种方法允许应用服务器和 MySQL 服务器（包括副本/集群节点）在同一个安全网络上进行通信，但会创建一个单一的故障点。另外，当有一个新的远程 master 的时候，在 MySQL 服务器上部署 OpenVPN 意味着如果你要新增一个额外的高可用性能的扩展会变得相当复杂。总之，OpenVPN 增加了许多额外的部件。
ProxySQL 的漂亮之处在于，你可以在所有应用程序服务器运行它，只要将其指向到正确的 MySQL 服务器（主服务器）即可正常工作，或者使用复制组功能来识别权威主机。
最后，需要注意的是，这些测试都是在 CentOS 7.3 + OpenSSL 1.0.1e + [Percona Server for MySQL 5.7.19](https://www.percona.com/software/mysql-database/percona-server) + ProxySQL 1.4.1 + PHP 5.4 + OpenVPN 2.4.3 上面进行的。
祝您使用愉快。
