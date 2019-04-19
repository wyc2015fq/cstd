# 远程连接ubuntu mysql出现2003错误 cant connect to mysql - shine302的博客 - CSDN博客
2018年04月29日 18:06:10[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：33
不多说直接上代码 
1.在控制台输入,进入mysql目录下，
```bash
sudo su  //进入root权限
cd /etc/mysql
```
- 1
- 2
2.打开my.cnf文件，找到 bind-address = 127.0.0.1 在前面加上#注释掉，如下：
#bind-address = 127.0.0.1 
3.然后在添加如下代码；OK。
```
skip-external-locking
skip-name-resolve
```
- 1
- 2
若在my.cnf文件中找不到#bind-address = 127.0.0.1 如图所示： 
![这里写图片描述](https://img-blog.csdn.net/20170308215708724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzIxNDQzNDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
那么进入 第二个路径 /etc/mysql/mysql.conf.d/ 文件夹中打开 mysqld.cnf文件修改即可。 
4.重新启动mysql服务
`service mysql restart`
