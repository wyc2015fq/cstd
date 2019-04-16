# mysql 慢查询分析工具：pt-query-digest 在mac 上的安装使用 - 我相信...... - CSDN博客





2016年06月08日 22:11:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：4575








系统性能调优尤其是mysql数据库性能调优一个主要依据就是慢查询日志了。原来在ubantu 或centos 上有一个强大工具就是 percona toolkit，那么在mac 上如何使用pt-query-digest呢？

## 下载 percona toolkit

访问 [http://www.percona.com/downloads/percona-toolkit](http://www.percona.com/downloads/percona-toolkit) , 采用源代码安装，下载最新稳定版2.2.7。

![这里写图片描述](https://img-blog.csdn.net/20160608212319520)

## 安装 并预先填好安装过程中的那些坑

进入下载的目录，开练：

```
$ tar xvfz percona-toolkit-2.2.7.tar.gz
$ cd percona-toolkit-2.2.17
```

注意一下perl 的版本要是5.18以上。

Perl，多年前热衷的语言，呵呵。试运行一下 perl Makefile.PL 看一下输出：

```
Checking if your kit is complete...

Looks good

Warning: prerequisite DBD::mysql 3 not found.

Warning: prerequisite DBI 1.46 not found.

Warning: NAME must be a package name

Generating a Unix-style Makefile

Writing Makefile for percona-toolkit

Writing MYMETA.yml and MYMETA.json
```

关注一下这几个warning，都可能是坑，尤其是DBD和DBI，没有它们，怎么实现数据库连接呢？ 安装一下perl 的相关模块吧。

```
$ perl -MCPAN -e "install DBI”
$ perl -MCPAN -e "install DBD::mysql"
```

注意，如果出现了permission 问题，要用一下sudo.

现在，可以安装 percona toolkit了，尤其主要是指定 mysql_config 的PATH。
`$ perl Makefile.PL --mysql_config=/local/mariadb/bin/mysql_config`
然后，就是熟知的 make， make install 了，至此，安装完成。

## 检验与使用

在perl 中检验安装的模块，有两种方法：一种是执行命令perldoc perllocal，一种是执行 instmodsh，简单起见，这里用instmodsh，运行及输出如下：

```
$ instmodsh
Available commands are:
   l            - List all installed modules
   m <module>   - Select a module
   q            - Quit the program
cmd? l
Installed modules are:
   Authen::SASL
   CGI
   Cwd
   DBD::mysql
   DBI
   Digest::HMAC
   Digest::SHA1
   Error
   File::Slurp::Tiny
   GSSAPI
   Git
   HTML::Parser
   HTML::Tagset
   IO::Socket::SSL
   Locale::gettext
   Net::LibIDN
   Net::SMTP::SSL
   Net::SSLeay
   ...
```

现在，就可以在Mac 上象其它Linux 平台那样使用percona toolkit 了。 回到目标主题，分析慢查询：
`$ pt-query-digest ~/mysql-slow.log`
远程执行会有的慢，这里是将慢查询日志拖到本地运行的，可以得到比较有价值的分析结果。



