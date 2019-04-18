# Perl语言入门--1--perl的包安装 - 默槑 - 博客园







# [Perl语言入门--1--perl的包安装](https://www.cnblogs.com/modaidai/p/6690425.html)





**A.  手工安装**

从CPAN下载了DBI模块1.13版的压缩文件DBI-1.13.tar.gz。假设放在/usr/local/src（一般需要编译安装的软件，都放在此目录下安装）。　
- # cd　/usr/local/src　 　   
- # tar　xvzf　DBI-1.13.tar.gz　　(这时候会创建一个去掉tar.gz这部分的文件夹)　 　   
- # cd　DBI-1.13   
- # perl　Makefile.PL 　　(生成makefile，无论什么包都是这个命令，什么包都会生成一个这个Makefile文件)   
- # make #(建立模块)
- #make　test (测试模块)  如果测试结果报告“all　test　ok”，您就可以放心地安装编译好的模块了。　
- # make　install　 （安装模块前，先要确保您对perl5安装目录有可写权限（通常以su命令获得），执行**make　install）**

**B.  通过安装CPAN后，通过CPAN安装包**



**安装**CPAN可能系统不同，方式也会不一样

对于 CentOS, Fedora 或者 RHEL:

```
$ sudo yum -y install perl-CPAN
```

对于Ubuntu 和 Debian:

```
$ sudo apt-get install perl-modules
```

现在应该可以**打开**CPAN了



#perl -MCPAN -e shell（进入cpan的shell）

#h（获取帮助信息）

#m（获取包列表）

#install XX包名（安装某个包）














