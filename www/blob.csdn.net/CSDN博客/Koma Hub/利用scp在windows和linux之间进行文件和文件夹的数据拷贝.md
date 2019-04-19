# 利用scp在windows和linux之间进行文件和文件夹的数据拷贝 - Koma Hub - CSDN博客
2018年04月07日 18:02:00[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1116
首先要有一台联网良好的windows和一个联网良好的linux系统的机器（PC，工作站），使用windows系统向linux拷贝数据、使用windows系统从linux中拷贝数据到本地。windows系统要安装好ssh、scp。已知linux系统的IP地址为***.***.***.***：
![](https://img-blog.csdn.net/20180407173955508)
***在windows的cmd下，向linux拷贝文件：***
```
D:\scp-dir>scp file1 username@***.***.***.***:/home/username/test/scp
username@***.***.***.***'s password:
file1                100%    0     0.0KB/s   00:00
```
链接linux验证是否拷贝成功：
```
$ cd /home/username/test/scp/
$ ls
file1
```
***在windows的cmd下，向linux拷贝文件夹：***
```
D:\>scp -r scp-dir username@***.***.***.***:/home/username/test/scp
username@***.***.***.***'s password:
file1                100%    0     0.0KB/s   00:00
```
链接linux验证是否拷贝成功：
```
$ ls
file1  scp-dir
```
注意，不可以使用如下命令拷贝scp-dir文件内的file1文件：
`D:\>scp scp-dir\file1 username@***.***.***.***:/home/username/test/scp`
而要用linux的文件夹路径“/”才可以：
`D:\>scp scp-dir/file1 username@***.***.***.***:/home/username/test/scp`
***在windows的cmd下，从linux拷贝文件到本地：***
随便在linux的terminal下用可以生成文件的指令生成一个文件，这里用的CWP的suplane指令：
```
$suplane > scptest.su
$ ls
scptest.su
$ pwd
/home/username/test/scp
```
进行拷贝：
`D:\>scp username@***.***.***.***:/home/username/test/scp/scptest.su scptest.su`
检查拷贝是否成功：
```
D:\>dir
2018/04/07  17:54            15,872 scptest.su
```
***在windows的cmd下，从linux拷贝文件夹即文件夹内所有内容到本地：***
```
D:\>scp -r username@***.***.***.***:/home/username/test/scp scp-dir-fromlinux
username@***.***.***.***'s password:
scptest.su           100%   16KB 265.2KB/s   00:00
```
验证是否拷贝成功：
```
D:\>dir
2018/04/07  17:58    <DIR>          scp-dir-fromlinux
```
