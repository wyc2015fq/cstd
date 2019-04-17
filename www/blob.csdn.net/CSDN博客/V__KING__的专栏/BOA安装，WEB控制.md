# BOA安装，WEB控制 - V__KING__的专栏 - CSDN博客





2015年12月05日 11:03:51[v__king__](https://me.csdn.net/V__KING__)阅读数：1457








# WEB控制树莓派硬件资源&BOA在树莓派的安装

# 1．下载Boa源码

下载地址: [http://www.boa.org/](http://www.boa.org/)

解压：# tar xzf boa-0.94.13.tar.gz

# 2.安装需要工具bison，flex

sudo apt-get install bison flex 

否则会出现如下错误 

make: yacc：命令未找到 

make: lex：命令未找到
# 3.修改文件

### (1)修改 src/compat.h

找到

```
#define TIMEZONE_OFFSET(foo) foo##->tm_gmtoff
修改成
#define TIMEZONE_OFFSET(foo) (foo)->tm_gmtoff
```

> 
否则会出现错误： 

  util.c:100:1: error: pasting “t” and “->” does not give a valid preprocessing token make: *** [util.o] 


### (2)修改 src/log.c

```bash
注释掉
if (dup2(error_log, STDERR_FILENO) == -1) {
DIE("unable to dup2 the error log");
}
为：
/*if (dup2(error_log, STDERR_FILENO) == -1) {
DIE("unable to dup2 the error log");
}*/
```

> 
否则会出现错误： 

  log.c:73 unable to dup2 the error log:bad file descriptor


### (3)修改src/boa.c

```php
注释掉下面两句话：
if (passwdbuf == NULL) {
DIE(”getpwuid”);
}
if (initgroups(passwdbuf->pw_name, passwdbuf->pw_gid) == -1) {
DIE(”initgroups”); }
为
#if 0
if (passwdbuf == NULL) {
DIE(”getpwuid”);
}
if (initgroups(passwdbuf->pw_name, passwdbuf->pw_gid) == -1) {
DIE(”initgroups”);
}
#endif
```

> 
否则会出现错误：boa.c:211 - getpwuid: No such file or directory


```
注释掉下面语句：
if (setuid(0) != -1) {
DIE(”icky Linux kernel bug!”);
}
为
#if 0
if (setuid(0) != -1) {
DIE(”icky Linux kernel bug!”);
}
#endif
```

> 
否则会出现问题：boa.c:228 - icky Linux kernel bug!: No such file or directory


### 5、生成Makefile文件

执行：

> 
cd boa-0.94.13/src 

  ./configure


编译

> 
make


通过ls -l boa 可以看到生成的二进制文件比较大，然后为生成的二进制文件boa瘦身

> 
arm-softfloat-linux-gnu-strip boa


再查看一下

> 
ls -l boa


可以发现boa的大小前后差距很大这为我们节省了很大的空间

# 6、Boa的配置

在boa-0.94.13目录下已有一个boa.conf，可以在其基础上进行修改。如下：

> 
vi boa.conf

- (1)Group的修改 

修改 Group nogroup 

为 Group 0- (2)user的修改 

修改 User nobody 

为 User 0- (3)ScriptAlias的修改 

修改ScriptAlias /cgi-bin/ /usr/lib/cgi-bin/ 

为 ScriptAlias /cgi-bin/ /www/cgi-bin/- (5)DocumentRoot的修改 

修改DocumentRoot /var/www 

为DoucmentRoot /www- (6)ServerName的设置 

修改#ServerName www.your.org.here 

为 ServerName www.your.org.here 

否则会出现错误“gethostbyname::No such file or directory”- (7)AccessLog修改 

修改AccessLog /var/log/boa/access_log 

为#AccessLog /var/log/boa/access_log 

否则会出现错误提示：“unable to dup2 the error log: Bad file descriptor”- (8)以下配置和boa.conf的配置有关 

创建目录/etc/boa并且把boa 和 boa.conf拷贝到这个目录下 

mkdir /etc/boa 

创建HTML文档的主目录/www 

mkdir /www 

创建CGI脚本所在录 /www/cgi-bin 

mkdir /www/cgi-bin 

cp boa.conf  / etc/boa 

将boa拷贝到开发板根文件系统的/etc/boa下 

cp src/boa  / etc/boa 

将你的主页index.html拷贝到www目录下
# 7、编写网页和cgi文件

在index.html中写入：

```xml
<html>
<head>
<title>Welcome to nginx on Debian!</title>
<style>
    body {
        width: 35em;
        margin: 0 auto;
        font-family: Tahoma, Verdana, Arial, sans-serif;
    }
</style>
</head>
<body>
<h1>Boa cgi: shell Program</h1>
<form method="get" action="cgi-bin/test.sh" name="LED_TEST"/>
<input type="submit" value="ok" name="submit">
</body>
</html>
```

然后在cgi-bin文件中编写test.sh文件来做任何事情了，当然也包括执行gcc编译好的`二进制程序`啦：

```bash
#!/bin/sh
# 记住一定要加上面这一句话#!/bin/sh
echo `date`>>log.txt
echo "Content-type:text/html"
echo ""
echo "<html>"
echo"<head><title>cgiShellHello</title></head>"
echo "<body>"
echo '<h1>Hello World! <fontcolor="red">"CGI Shell"</font>  </h1>'
echo "</body>"
echo "</html>"
```

# 8、测试

测试之前可以将boa命令加入到PATH中 

* 1. 在/usr/bin目录中加入一个软链接：sudo ln -s /etc/boa/boa /usr/bin/boa 

* 2. 使这个命令立马生效：source /etc/profile 

打开boa，输入sudo boa 

如果有其他端口占用80端口会报错（因为boa默认的绑定端口是80），此时需要找到占用80端口的进程，然后kill掉在打开boa.
> 
service nginx stop


在客户端输入XXX.XXX.XXX.XXX（IP），会出现预先放置好的index.html网页,点击提交的action












