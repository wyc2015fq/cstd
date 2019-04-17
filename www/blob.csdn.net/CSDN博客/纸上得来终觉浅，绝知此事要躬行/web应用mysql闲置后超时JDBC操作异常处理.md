# web应用mysql闲置后超时JDBC操作异常处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年03月25日 11:46:35[boonya](https://me.csdn.net/boonya)阅读数：1488








#### 异常问题情况描述

第一天应用服务器正常运行并且保持这种运行（系统中只有测试人员，应用处于Active-busy状态），（中间晚间没有任何人使用该系统，那么相当于Sleep&wait）第二天测试用户从C/S或B/S访问后台报错。错误信息如下：

![](https://img-blog.csdn.net/20150325112128575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20150325112224998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如果用户每次访问之前都重启一次Tomcat server，这个是可以保证没问题的，所以可以排除是非服务端的问题，那么就可以得出是数据库在某种情况的发起的数据库连机失效了。

#### MySQL的会话超时设置

**interactive_timeout****：服务器关闭交互式连接前等待活动的秒数。**交互式客户端定义为在mysql_real_connect()中使用CLIENT_INTERACTIVE选项的客户端。又见wait_timeout
 。
**wait_timeout****:服务器关闭非交互连接之前等待活动的秒数。**在线程启动时，根据全局wait_timeout值或全局interactive_timeout值初始化会话wait_timeout值，取决于客户端类型(由mysql_real_connect()的连接选项CLIENT_INTERACTIVE定义)，又见interactive_timeout
 。
如此看来，两个变量是共同控制的，那么都必须对他们进行修改了。在MySQL中，应用会话连接有超时设置，之前没有遇到过此类问题，所以今天在此总结一下。

MySQL超时有好几种，如下图所示：（注：这里的interactive_timeout 值是我修改过的,一般设置超时时间为一年(365days)，也就是31536000s.）

![](https://img-blog.csdn.net/20150325113320417?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


#### 修改interative_timeout和wait_timeout

![](https://img-blog.csdn.net/20150325113754167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


修改后：

![](https://img-blog.csdn.net/20150325113811420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：这里的wait_timeout可能溢出了允许范围，所以值为2147483，这个只有MySQL知道是怎么回事。

另外，还可以修改配置文件windows下my.ini文件，在文件下加两行interactive_timeout=设置值     wait_timeout=设置的值  然后重启mysql服务。


参考文章地址：[http://www.shuyangyang.com.cn/jishuliangongfang/shujuku/2013-03-10/63.html](http://www.shuyangyang.com.cn/jishuliangongfang/shujuku/2013-03-10/63.html)






