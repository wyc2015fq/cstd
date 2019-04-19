# 从ftp服务器获取文件 - sxf_123456的博客 - CSDN博客
2018年01月16日 17:38:11[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：258
                
1、登录ftp
[root@manage wdkjnet]# lftp -u sppz_itmsjdh,Nm_sppz@123 0.0.0.0 -p 21
用户名：sppz_itmsjdh
密码：Nm_sppz@123 
ftp服务器:0.0.0.0
端口:21
2、定位本地目录
lcd  /tmp
3、下载到本地目录
get SPPZ_ITMS_JDH_01W_20180114.tar.gz
ftp 常用命令
上传至ftp服务器
put  test.txt
删除ftp服务器上文件
delete b.txt
批量下载所有文件
mget *
结束回话
close
结束并退出
quit
            
