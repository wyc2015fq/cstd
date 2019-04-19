# 用ssh和socat分分钟搞定Linux各种端口转发 - xqhrs232的专栏 - CSDN博客
2018年07月30日 09:47:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1892
原文地址::[http://baijiahao.baidu.com/s?id=1581577504614740481&wfr=spider&for=pc](http://baijiahao.baidu.com/s?id=1581577504614740481&wfr=spider&for=pc)
本地端口映射
小明写了一个Web服务，监听hosta上8080端口，小王现在想访问这个服务，有没有快速的方法？
![](http://t12.baidu.com/it/u=3450353624,1999045889&fm=173&s=7010E4328D3062135E5DACC6000090B3&w=640&h=222&img.JPEG)
SSH本地端口转发
其实很简单，小明只需要做这两件事情：
申请一台带公网IP的公有云服务器hostb，我们假设ip地址是12.34.56.78
在hosta上执行下面的命令，它会建立一个从hosta到hostb的ssh会话，同时将8080端口映射到hostb的1080端口。
ssh -R 1080:localhost:8080 root@12.34.56.78
这样，小王就可以愉快的用http://12.34.66.78:1080访问小明的服务了。
远程Unix Socket映射
除了将我们本地服务通过端口映射提供给其它人访问，我们还可以通过端口转发玩一些更high的。比如下面这条命令，它把监听在远程主机12.34.56.78上的mysql服务unix socket映射到本地的/var/run/mysqld.temp.sock，这样，小明就可以用mysql -S /var/run/mysqld/mysqld.temp.sock来访问远程主机的mysql服务了。
socat "UNIX-LISTEN:/var/run/mysqld.temp.sock,reuseaddr,fork" EXEC:"ssh root@12.34.56.78 socat STDIO UNIX-CONNECT\:/var/run/mysqld/mysqld.sock"
当然，小明如果不喜欢本地unix socket，他还可以用下面的命令把12.34.56.78上的mysql映射到本地的5500端口，然后使用mysql -p 5500命令访问。
socat TCP-LISTEN:5500 EXEC:'ssh root@12.34.56.78 "socat STDIO UNIX-CONNECT:/var/run/mysqld/mysqld.sock"'
UDP也不在话下，比如下面这条命令可以把12.34.56.78的udp 161端口映射到本地的1611端口：
socat udp-listen:1611 system:'ssh root@12.34.56.78 "socat stdio udp-connect:remotetarget:161"'
上面代码和使用方法，我都已经上传到我的github：https://github.com/luohao-brian/funny_linux_cmds/tree/master/portfwd
