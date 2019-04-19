# MySQL官方压缩包安装 - walkabc - 博客园
# [MySQL官方压缩包安装](https://www.cnblogs.com/hitfire/articles/6398613.html)
　　操作系统:Ubuntu 14.04 64位
　　1.下载对应的MySQL压缩包
　　下载地址页面:https://dev.mysql.com/downloads/mysql/
　　2.解压缩文件包
　　tar -xvf mysql.tar.gz
　　3.建立软链接
　　假设/fullpath是已解压的mysql目录的绝对路径
　　sudo ln -s /fullparh /usr/local/mysql
　　4.创建用户组mysql和用户mysql
　　sudo groupadd mysql
　　sudo useradd -r -g mysql -s /bin/false mysql
　　5.执行数据库初始化操作
　　sudo mysqld --initialize --datadir=/fullpath --basedir=/fullpath/data --user=mysql
　　注意，mysql在初始化的时候，会自动生成一个密码，记下这个密码
　　6.把mysql的目录权限交给用户mysql
　　sudo chown -R mysql:mysql /usr/local/mysql
　　7.启动mysql
　　sudo /fullpath/bin/mysqld_safe --user=mysql
　　8.修改密码
　　mysqladmin -u root password 新密码 -p
　　执行这个命令系统会提示输入旧密码，把之前的老密码输入进去就可以成功修改了
　　此时,MySQL成功安装！

