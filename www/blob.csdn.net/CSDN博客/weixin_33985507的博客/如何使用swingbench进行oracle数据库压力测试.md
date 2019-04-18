# 如何使用swingbench进行oracle数据库压力测试 - weixin_33985507的博客 - CSDN博客
2014年10月06日 08:09:02[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：18
    swingbench是一款网络上开源的oracle压力测试工具，支持oracle 11g版本，还能对rac进行测试。从官方页面http://dominicgiles.com/downloads.html上可以下载最新的软件版本。
![wKiom1Qx2GDhPRnSAAGf-N_yano842.jpg](http://s3.51cto.com/wyfs02/M00/4B/BB/wKiom1Qx2GDhPRnSAAGf-N_yano842.jpg)
   swingbench可以运行在windows和linux平台，本次测试采用linux平台，具体测试过程如下：
  1、首先使用VMVARE10搭建一个redhat6.5的虚拟机。
  2、在虚拟redhat6.5上安装ORACLE11G的数据库。
  3、使用oewizard导入测试数据，可以根据向导提示进行数据导入。   
  4、使用swingbench进行压力测试。
测试步骤：
1、导数据之前需要修改temp表空间大小，使其能够容纳下相应的导入数据
mkdir -p /home/oracle/oradata/orcl
create temporary tablespace temp1 tempfile '/home/oracle/oradata/orcl/temp1.dbf' size 100m; 
alter database default temporary tablespace temp1;  
drop tablespace TEMP;  
create temporary tablespace temp tempfile '/home/oracle/oradata/orcl/temp.dbf' size 1g;
alter database default temporary tablespace temp;
drop tablespace temp1;
(也可用rm删除temp1.dbf)
上述操作也可通过EM管理界面直接将TEMP表空间扩到1G.
2、安装swingbench测试软件，直接解压缩即可运行。
unzip -x swingbench25919.zip
3、进入swingbench/bin目录执行oewizard导入1G测试数据，并修改数据库连接名和DBA密码
![wKioL1Qx2h3TrIIGAAGyVq92CVA180.jpg](http://s3.51cto.com/wyfs02/M01/4B/BD/wKioL1Qx2h3TrIIGAAGyVq92CVA180.jpg)
输入导入数据文件存放位置：
![wKiom1Qx2nGTFbZFAAGBnRUybxU038.jpg](http://s3.51cto.com/wyfs02/M01/4B/BB/wKiom1Qx2nGTFbZFAAGBnRUybxU038.jpg)
选择导入1G数据：
![wKioL1Qx2tnyThWoAAE8qLLgIJI464.jpg](http://s3.51cto.com/wyfs02/M02/4B/BD/wKioL1Qx2tnyThWoAAE8qLLgIJI464.jpg)
数据导完之后在该目录下运行swingbench执行测试，修改数据库连接名，用户连接数设置为300，测试时间设置为10分钟
![wKiom1Qx2v6S6yjvAAKAl63J5mg784.jpg](http://s3.51cto.com/wyfs02/M02/4B/BB/wKiom1Qx2v6S6yjvAAKAl63J5mg784.jpg)
修改Distributed Controls用于测试过程中搜集测试监控信息，修改完之后测试连接是否正常，并可以统计主机的cpu  disk IO 信息
![wKioL1Qx24DDSssqAAFmiLc2VSU132.jpg](http://s3.51cto.com/wyfs02/M00/4B/BD/wKioL1Qx24DDSssqAAFmiLc2VSU132.jpg)
还可以拉出AWR报表
![wKioL1Qx3O2gzw45AADH_p4CEeQ979.jpg](http://s3.51cto.com/wyfs02/M01/4B/BD/wKioL1Qx3O2gzw45AADH_p4CEeQ979.jpg)
设置 insert，update ，select的比例
![wKioL1Qx3RHB32ITAAEK9ABjiPI261.jpg](http://s3.51cto.com/wyfs02/M00/4B/BD/wKioL1Qx3RHB32ITAAEK9ABjiPI261.jpg)
设置完成之后，点击左上角绿色按钮执行测试
![wKiom1Qx24zgsNQ-AAEJ51YcKSs614.jpg](http://s3.51cto.com/wyfs02/M00/4B/BB/wKiom1Qx24zgsNQ-AAEJ51YcKSs614.jpg)
测试过程截图
![wKiom1Qx3QaxFssnAAGpJeiYV6Q924.jpg](http://s3.51cto.com/wyfs02/M01/4B/BB/wKiom1Qx3QaxFssnAAGpJeiYV6Q924.jpg)
测试结果可保持为XML文档，最后查看显示如下
![wKioL1Qx3YTjl5FJAAHAP8c-C8k663.jpg](http://s3.51cto.com/wyfs02/M02/4B/BD/wKioL1Qx3YTjl5FJAAHAP8c-C8k663.jpg)
