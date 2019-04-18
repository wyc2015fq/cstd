# CentOS6.2部署qt开发环境 - 专注于数据挖掘算法研究和应用 - CSDN博客





2012年01月04日 18:58:42[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：8947








1.gcc-c++安装：

   1)方法一：root身份命令：yum install gcc-c++

    2) 方法二：图形化菜单：系统->管理->添加/删除软件->Development->开发工具下选择gcc-c++安装，未验证；

2.qt sdk(含qt creator)安装：

    下载地址[http://qt.nokia.com/downloads/](http://qt.nokia.com/downloads/)

    选择linux-32位，有online(在线联网安装)和offline两种方式，此处下载online版本：Qt_SDK_Lin32_online_v1_1_4_en.run，运行后联网安装；



3. poco安装：

    下载地址[http://pocoproject.org/download/index.html](http://pocoproject.org/download/index.html)

    选择Complete Edition->sources for linux下载，版本：poco-1.4.2p1-all.tar.gz

      解压缩后进入目录执行下面安装命令：

       1) ./configure

        2)gmake -s

       3) sudo gmake -s install



       安装之前要先补充安装下面三个环境：

        1)openssl：yum install openssl-devel

        2)UnixODBC：yum install unixODBC-devel

        3)mysqlclient：

          命令：yum install mysql-devel

          rpm包下载：[http://dev.mysql.com/downloads/mysql/#downloads](http://dev.mysql.com/downloads/mysql/#downloads) 选择oralce linux6平台，未验证；



安装成功后即用qt在centos下开发前后台，实现跨平台C++开发！



