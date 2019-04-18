# Matlab学习笔记 变量的保存与清除 - fighting！！！ - CSDN博客
2018年05月13日 14:57:43[dujiahei](https://me.csdn.net/dujiahei)阅读数：705
每次用matlab跑上万条的数据，都要花费很长很长时间，昨晚 跑8万条数据、训练100次的CNN跑了两个半小时，今天继续执行2G的电脑内存不断溢出，删了几个后面用不到的大数据后还是溢出，于是决定用东哥的服务器来跑。但不能让之前两个多小时的功夫白费啊，突然想到，将已经跑出来的变量存储起来再拿到他的电脑上用不就好了嘛！
记录下工作空间变量操作的几个方法：
（1）清除工作空间变量
清除全部变量：clear
清除当某些变量：clear 变量名1 变量名2 ...
清除除某些变量之外所有变量：clearvars -except 变量名1 变量名2 ...
可按照一定的命名规则清除变量，如清除以字母m开头的全部变量：clear m*
（2）保存工作空间变量
假设当前运行的m文件为filename.m，则直接save filename.mat就可以把workspace中的所有变量都保存到 filename.mat中。save name x y 保存工作空间变量x、y到name.mat（3）列出工作空间变量
who     列出当前工作空间的变量
whos   列出当前工作空间的变量及信息
（4）加载工作空间变量
load name        加载name.mat中的所有变量到工作空间
load name x y   加载name.mat中的x、y变量到工作空间
