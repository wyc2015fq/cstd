# torch学习（一） torch使用 - xmdxcsj的专栏 - CSDN博客





2015年10月16日 20:48:56[xmucas](https://me.csdn.net/xmdxcsj)阅读数：4296








一、安装问题

1.Only CentOS 7 is supported for now, aborting.

修改原来安装文件

[](https://github.com/torch/ezinstall/blob/master/install-deps)[https://github.com/torch/ezinstall/blob/master/install-deps](https://github.com/torch/ezinstall/blob/master/install-deps)

把“ if [[ $centos_major_version == '7']];then”判断去掉




二、使用

1. 安装package

$luarocks install image

$luarocks list

2.交互界面TREPL

$th

交互界面执行lua文件

th> dofile "file.lua"

tab    补全

?        帮助

$cmd 执行shell命令

shell界面执行lua文件

$th file.lua



