# include、include_once与require、require_once区别 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月23日 17:03:26[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：413
include与require的区别
include在引入不存文件时产生一个警告且脚本还会继续执行，而require则会导致一个致命性错误且脚本停止执行。

<?php
include 'no.php';
echo '123';
?>
如果no.php文件不存在，echo '123'这句是可以继续执行的.
<?php
require 'no.php';
echo '123';
?>
如果no.php文件不存在，echo '123'这句是不执行的，在require时就停止了。
_once只会包含一次，性能有下降，但可以忽略不计。


[http://stackoverflow.com/questions/2418473/difference-between-require-include-and-include-once](http://stackoverflow.com/questions/2418473/difference-between-require-include-and-include-once)
[http://www.myxzy.com/post-389.html](http://www.myxzy.com/post-389.html)
