# 服务器数据库系列 - Nginx添加Auth密码验证 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月20日 15:52:50[initphp](https://me.csdn.net/initphp)阅读数：1179








```
#1. 新建一个pw.pl文件专门用来生成密码
#!/usr/bin/perl
use strict;
my $pw=$ARGV[0];
print crypt($pw,$pw)."\n";

#2. 修改Nginx配置文件，添加，用户名和密码存放的文职
auth_basic  "root";
auth_basic_user_file /usr/local/web/nginx.0.8.15/conf/htpassword;

#3. 用pw.pl生成密码
chmod +x pw.pl #可执行
./pw.pl 123456

#4. 新增htpassword文件
root:加密后的密码
```





