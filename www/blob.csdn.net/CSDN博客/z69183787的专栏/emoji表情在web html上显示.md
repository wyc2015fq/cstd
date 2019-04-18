# emoji表情在web html上显示 - z69183787的专栏 - CSDN博客
2017年01月07日 12:49:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11922
[iOS](http://lib.csdn.net/base/ios)或[Android](http://lib.csdn.net/base/android)客户的输入法支持emoji表情输入，系统管理后台需要显示用户实际输入的效果,因此处理emoji表情符
**1.[MySQL](http://lib.csdn.net/base/mysql)需要设置支持emoji编码为utf8mb4，具体如下：**
(a) 配置my.cnf:
[mysql]
default-character-set = utf8mb4
[mysqld]
character-set-client-handshake = FALSE
character-set-server = utf8mb4
collation-server = utf8mb4_unicode_ci
init_connect='SET NAMES utf8mb4'
(b) 重启mysql
(c) //若是[Java](http://lib.csdn.net/base/javase),还得修改mysql的包大于升级或确保你的mysql connector版本高于5.1.13  
(d) 请根据自己数据表对应的字段，进行设置编码
ALTER TABLE t_user_basic MODIFY COLUMN title VARCHAR(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL COMMENT '内容';
(e) 查看 SHOW VARIABLES LIKE 'character%'; 
**2.下载emoji的js相关内容和图片,里面包括demo使用,同时注意图片的路径，以免错误。**
下载地址：http://download.csdn[.NET](http://lib.csdn.net/base/dotnet)/detail/huangxingzhe/9089255
**3.从[数据库](http://lib.csdn.net/base/mysql)查出数据后，需要把它转换成unicode编码，如：\ud83d\ude05**
**4.js获取到unicode字符之后，会对它进行正则过滤，转换成图片名称对应的字符，然后组装图片路径，这样就可以显示了**
