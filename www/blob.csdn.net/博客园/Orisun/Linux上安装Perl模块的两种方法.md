# Linux上安装Perl模块的两种方法 - Orisun - 博客园







# [Linux上安装Perl模块的两种方法](https://www.cnblogs.com/zhangchaoyang/articles/2610573.html)





　Linux/Unix下安装Perl模块有两种方法：手工安装和自动安装。第一种方法是从CPAN上下载　 
您需要的模块，手工编译、安装。第二种方法是联上internet，使用一个叫做CPAN的模块自动完　 
成下载、编译、安装的全过程。　 

a、手工安装的步骤：　 

从CPAN下载了DBI模块1.13版的压缩文件DBI-1.13.tar.gz，假设放在/usr/local/src/下。　 
cd　/usr/local/src　 
解压缩这个文件：　 
tar　xvzf　DBI-1.13.tar.gz　 
这时会新建一个DBI-1.13的目录。　 
cd　DBI-1.13　 

生成makefile：　 
perl　Makefile.PL　 
建立模块　 
make　 
测试模块　 
make　test　 
如果测试结果报告“all　test　ok”，您就可以放心地安装编译好的模块了。安装模块前，先要　 
确保您对perl5安装目录有可写权限（通常以su命令获得），执行：　 
make　install　 

现在，写个程序试试吧。　 

#!/usr/bin/perl　-w　 
use　strict;　 
use　DBI;　 
..　 


　　　　上述步骤适合于Linux/Unix下绝大多数的Perl模块。可能还有少数模块的安装方法略有差别，　 
所以最好先看看安装目录里的README或INSTALL。另外，上述过程是针对动态链接的Perl编译　 
器（所有Linux下预安装的Perl都是动态链接的），如果您在使用一个静态链接的Perl，您需要将　 
新的模块静态链接到perl编译器中，可能还需要重启机器。　 


b、使用CPAN模块自动安装：　 

安装前需要先联上线，并且您需要取得root权限。　 
perl　-MCPAN　-e　shell　 
初次运行CPAN时需要做一些设置，如果您的机器是直接与internet相联（拨号上网、专线，etc.），　 
那么一路回车就行了，只需要在最后选一个离您最近的CPAN镜像站点。例如我选的是位于国内的　 
中国自由软件库ftp://freesoft.cgi.gov.cn/pub/languages/perl/CPAN　。否则，如果您的机器　 
位于防火墙之后，还需要设置ftp代理或http代理。　 

获得帮助　 
cpan>h　 

列出CPAN上所有模块的列表　 
cpan>m　 

根据关键字在CPAN上查找某个模块：

cpan[1]> i /scws/
CPAN: Storable loaded ok (v2.20)
Going to read '/home/orisun/.cpan/Metadata'
  Database was generated on Sat, 17 Nov 2012 08:07:03 GMT
Distribution    XUERON/Text-Scws-0.01.tar.gz
Module    Text::Scws             (XUERON/Text-Scws-0.01.tar.gz)
2 items found


安装模块　 
cpan>install　DBI　 
自动完成DBI模块从下载到安装的全过程。　 

退出　 
cpan>q　



perldoc  -f  func 　　　　　　#查看perl函数的使用方法












