# ssh登录问题出现Pseudo-terminal will not be allocated because stdin is not a terminal错误 - xqhrs232的专栏 - CSDN博客
2017年06月23日 09:35:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4610
原文地址::[http://www.jianshu.com/p/e66014b67506](http://www.jianshu.com/p/e66014b67506)
问题：
今天遇到一个ssh登录问题,当远程登录主机时出现Pseudo-terminal will not be allocated because stdin is not a terminal. 错误。
原因：用脚本调用或者程序调用ssh登陆远程的时候出现的这个情况。背后的机制和深层次的原因没有深究。
解决方案:
写登陆脚本ssh的时候加个参数
> 
ssh -t -t user@10.242.1.1
或者
ssh user@10.242.1.1 -tt
## 记录下ssh远程执行脚本
> 
cmd="echo 'hello'>/home/user/XL_WORK/test.bak"
ssh -t -t user@10.242.1.1 $cmd
另：
ssh在6.7版本以后，对一些加密算法不支持，需要手动添加
> 
应该是jsch默认使用diffie-hellman-group-exchange-sha1算法，请在主机ssh服务的sshd_config文件中添加下列加密算法并重启ssh服务验证。 KexAlgorithms diffie-hellman-group1-sha1,diffie-hellman-group14-sha1,diffie-hellman-group-exchange-sha1,diffie-hellman-group-exchange-sha256

