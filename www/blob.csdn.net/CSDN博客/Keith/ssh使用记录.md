# ssh使用记录 - Keith - CSDN博客





2018年03月22日 22:56:07[ke1th](https://me.csdn.net/u012436149)阅读数：622








**指定端口登录**

ssh -p port username@ip

**将远程端口映射到本地**

ssh  -N -f -L localhost:8889:localhost:8888 username@ip 

这时访问本地的 8889端口 等同与 访问 远程的 8888 端口 
[对应windows putty的配置方法](https://www.chenyudong.com/archives/putty-ssh-port-forwarding-setting.html)
ssh  -N -f -L localhost:8889:ip2:8888 username@ip1 

如果 ip2 只能由ip1 访问，可以这么执行端口映射 
[对应windows putty的配置方法](http://www.111cn.net/sys/linux/94719.htm)
## 参考资料

[http://blog.51cto.com/lynnteng0/1066795](http://blog.51cto.com/lynnteng0/1066795)
[https://blog.csdn.net/zbgjhy88/article/details/55289785](https://blog.csdn.net/zbgjhy88/article/details/55289785)





