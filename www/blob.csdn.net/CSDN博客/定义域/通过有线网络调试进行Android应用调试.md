# 通过有线网络调试进行Android应用调试 - 定义域 - CSDN博客
2016年04月07日 13:15:17[定义域](https://me.csdn.net/u010278882)阅读数：379
通过有线网络调试进行Android应用调试
- 
开启 adb client 的监听 
stop adbd 
setprop service.adb.tcp.prot 5555 
start adbd
- 
连接 adb client 
adb connect 10.30.17.127
