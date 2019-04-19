# 长期运行Kibana - BlueSky - CSDN博客
2016年11月23日 14:08:46[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：9530
最近在搭建ELK平台，kibana是用tar包直接解压缩使用的方式安装的。但是安装完成后遇到一个问题就是如何让kibana后台运行，并且当断开ssh连接后，还能继续运行。后来想到了nohup命令。
nohup用法：
```
```bash
root@localhost:~
```
```bash
# man nohup
```
```bash
NOHUP(1)                         User Commands                        NOHUP(1)
```
```bash
NAME
```
```bash
```
```bash
nohup
```
```bash
- run a
```
```bash
command
```
```bash
immune to hangups, with output to a non-
```
```bash
tty
```
```bash
SYNOPSIS
```
```bash
```
```bash
nohup
```
```bash
COMMAND [ARG]...
```
```bash
```
```bash
nohup
```
```bash
OPTION
```
```
通常需要让某个命令后台运行可以这样：
#nohup  COMMAND &
但是nohup在使用时还需要注意一些问题：
1、当输入nohup  COMMAND &  命令后，需要按任意键返回到shell窗口
2、退出当前登录窗口用exit命令，不要直接点击窗口上的叉来关闭
后台启动运行kibana演示：
```
```bash
root@localhost:~
```
```bash
# nohup /usr/local/kibana-4.5.0-linux-x64/bin/kibana &
```
```bash
[1] 9128
```
```bash
root@localhost:~
```
```bash
# nohup: ignoring input and appending output to `nohup.out'
```
```bash
root@localhost:~
```
```bash
#
```
```
 然后命令行输入exit，重新登录shell，看看kibana程序是否还在运行
```
```bash
root@localhost:~
```
```bash
# ps -ef | grep kibana
```
```bash
root      9128     1  2 07:49 ?        00:00:02
```
```bash
/usr/local/kibana-4
```
```bash
.5.0-linux-x64
```
```bash
/bin/
```
```bash
..
```
```bash
/node/b
```
```
