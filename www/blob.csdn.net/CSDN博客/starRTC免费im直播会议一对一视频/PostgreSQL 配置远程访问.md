# PostgreSQL 配置远程访问 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年04月17日 14:03:27[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：73
配置远 程连接PostgreSQL数据库的步骤很简单，只需要修改data目录下的**pg_hba.conf**和**postgresql.conf。**
**pg_hba.conf：**配置对数据库的访问权限，
**postgresql.conf：**配置PostgreSQL数据库服务器的相应的参数。
**修改pg_hba.conf文件**
- host  all    all    192.168.1.0/24    md5
新添加的内容，表示允许网段192.168.1.0上的所有主机使用所有合法的数据库用户名访问数据库，并提供加密的密码验证。
其中，数字24是子网掩码，表示允许192.168.1.0--192.168.1.255的计算机访问！
.**修改postgresql.conf文件**
定位到#listen_addresses=’localhost’。PostgreSQL安装完成后，默认是只接受来在本机localhost的连接请 求。
将行开头都#去掉，将行内容修改为listen_addresses=’*'来允许数据库服务器监听来自任何主机的连接请求

