# SSH远程连接:简单的连接 - Koma Hub - CSDN博客
2017年12月16日 13:41:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：213
在已知用户名和ip的情况下，简答的ssh如下：
```
rongtao@rongtao:~$ ssh Toa@172.18.174.84
Toa@172.18.174.84's password: 
Last login: Sat Dec 16 13:53:18 2017 from 172.18.175.51
-bash-4.1$ ls
algorithmsDataStructures  clean.sh  hc     mpich   qt             test
c                         cuda      intel  openmp  raytracingVTI  tomo
c++                       gtk       java   python  rtm
-bash-4.1$
```
退出：
```
-bash-4.1$ exit
logout
Connection to 172.18.174.84 closed.
rongtao@rongtao:~$
```
SSH默认的端口是22，会直接登录远程主机的22端口，当然可以对端口进行修改：
```
rongtao@rongtao:~$ ssh -p 22  Toa@172.18.174.84
Toa@172.18.174.84's password: 
Last login: Sat Dec 16 13:57:12 2017 from 172.18.175.51
-bash-4.1$
```
如果端口不存在：
```
rongtao@rongtao:~$ ssh -p 80  Toa@172.18.174.84
ssh: connect to host 172.18.174.84 port 80: Connection refused
rongtao@rongtao:~$
```
