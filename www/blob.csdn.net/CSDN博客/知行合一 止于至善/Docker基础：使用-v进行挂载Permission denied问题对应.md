
# Docker基础：使用-v进行挂载Permission denied问题对应 - 知行合一 止于至善 - CSDN博客

2019年02月26日 19:34:53[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：522


Memo：使用-v进行挂载Permission denied问题对应方法。
# 现象
CentOS中docker容器运行时使用docker run -v进行挂载的卷，在容器中进行访问提示Permission denied, 比如如下nginx的设定目录的状况发现挂载的卷都无法进行ls操作
`# cd conf.d
# ls
ls: cannot open directory '.': Permission denied
#`
# 原因
这是因为CentOS中安全模块selinux所起的作用。
# 对应方式
## 方式1
使用docker run时加上–privileged=true
## 方式2
粗暴地禁掉selinux，临时关闭可以使用setenforce 0，长期起效需要修改设定文件
## 方式3
设定selinux相关规则，命令如下
> 设定命令：chcon -Rt svirt_sandbox_file_t 目录名称


