# Installing PostgreSQL on Linux/Unix/windows - Koma Hub - CSDN博客
2018年06月06日 22:00:40[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：56
个人分类：[PostgreSQL](https://blog.csdn.net/Rong_Toa/article/category/7718480)
[link: ][https://www.tutorialspoint.com/cgi-bin/printpage.cgi](https://www.tutorialspoint.com/cgi-bin/printpage.cgi)
To start understanding the PostgreSQL basics, first let us install the PostgreSQL. This chapter explains about installing the PostgreSQL on Linux, Windows and Mac OS platforms.
# [](https://github.com/RongToa/ML/blob/master/SQL/PostgreSQL/Installing-PostgreSQL-on-Linux-Unix.md#installing-postgresql-on-linuxunix)Installing PostgreSQL on Linux/Unix
Follow the given steps to install PostgreSQL on your Linux machine. Make sure you are logged in as root before you proceed for the installation.
- 
Pick the version number of PostgreSQL you want and, as exactly as possible, the platform you want from [EnterpriseDB][https://www.enterprisedb.com/products-services-training/pgdownload](https://www.enterprisedb.com/products-services-training/pgdownload)
- 
I downloaded postgresql-9.2.4-1-linux-x64.run for my 64 bit CentOS-6 machine. Now, let us execute it as follows −
[root@host]# chmod +x postgresql-9.2.4-1-linux-x64.run
[root@host]# ./postgresql-9.2.4-1-linux-x64.run
------------------------------------------------------------------------
Welcome to the PostgreSQL Setup Wizard.
------------------------------------------------------------------------
Please specify the directory where PostgreSQL will be installed.
Installation Directory [/opt/PostgreSQL/9.2]:
- Once you launch the installer, it asks you a few basic questions like location of the installation, password of the user who will use database, port number, etc. So keep all of them at their default values except password, which you can provide password as per your choice. It will install PostgreSQL at your Linux machine and will display the following message −
Please waitwhile Setup installs PostgreSQL on your computer.
 Installing
 0% ______________ 50% ______________ 100%
 #########################################
-----------------------------------------------------------------------
Setup has finished installing PostgreSQL on your computer.
- Follow the following post-installation steps to create your database −
[root@host]# su - postgres
Password:
bash-4.1$ createdb testdb
bash-4.1$ psql testdb
psql (8.4.13, server 9.2.4)
test=#
- You can start/restart postgres server in case it is not running using the following command −
[root@host]# service postgresql restart
Stopping postgresql service:                               [  OK  ]
Starting postgresql service:                               [  OK  ]
- If your installation was correct, you will have PotsgreSQL prompt test=# as shown above.
# [](https://github.com/RongToa/ML/blob/master/SQL/PostgreSQL/Installing-PostgreSQL-on-Linux-Unix.md#installing-postgresql-on-windows)Installing PostgreSQL on Windows
Follow the given steps to install PostgreSQL on your Windows machine. Make sure you have turned Third Party Antivirus off while installing.
- 
Pick the version number of PostgreSQL you want and, as exactly as possible, the platform you want from [EnterpriseDB][https://www.enterprisedb.com/products-services-training/pgdownload](https://www.enterprisedb.com/products-services-training/pgdownload)
- 
I downloaded postgresql-9.2.4-1-windows.exe for my Windows PC running in 32bit mode, so let us run postgresql-9.2.4-1-windows.exe as administrator to install PostgreSQL. Select the location where you want to install it. By default, it is installed within Program Files folder.
