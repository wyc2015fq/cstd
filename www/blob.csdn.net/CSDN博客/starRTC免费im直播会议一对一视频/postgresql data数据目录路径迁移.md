# postgresql  data数据目录路径迁移 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年04月24日 09:36:00[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：743
默认的数据库路径是/var/lib/pgsql/9.x/data
将现有的数据库文件全部拷贝到新的数据库路径下，然后重启
新建一个路径作为新的数据库数据路径，假如是/home/data
sudo mkdir /home/data
sudo chown -R postgres:postgres data
sudo chmod 700 data
3、文件拷贝，
首先要停止postgresql服务
sudo systemctl stop postgresql
sudo su - postgres
cp -rf /var/lib/pgsql/9.x/data/* /home/data
修改service文件
找到/usr/lib/systemd/system/postgresql*.service
修改这个文件中的
Environment=PGDATA=/var/lib/pgsql/9.4/data/
将其修改为自己的新的数据路径：
Environment=PGDATA=/home/data/
reboot系统
然后启动postgres服务
sudo systemctl restart posrgresql
