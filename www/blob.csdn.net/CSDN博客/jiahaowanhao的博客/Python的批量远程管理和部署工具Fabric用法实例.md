
# Python的批量远程管理和部署工具Fabric用法实例 - jiahaowanhao的博客 - CSDN博客


2018年05月24日 19:02:29[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：239


[Python的批量远程管理和部署工具Fabric用法实例](http://cda.pinggu.org/view/25634.html)
本文实例讲述了Python的批量远程管理和部署工具Fabric用法。分享给大家供大家参考。具体如下：
Fabric是Python中一个非常强大的批量远程管理和部署工具,常用于在多个远程PC上批量执行SSH任务.
常见的使用方法大概总结如下:
1. 首先,要将批量执行的任务写入到一个fabfile.py中,
代码如下:
\# -*- coding:utf-8 -*-
from fabric.api import run, local, roles, env, cd
env.hosts=[
'192.168.1.110',
'192.168.1.111',
'192.168.1.112'
]
env.user="username"
env.password="password"
env.port=22
\#env.parallel=True
\#env.skip_bad_hosts=True
\#env.timeout=1
\#env.warn_only=True
\# local用于在本地PC执行命令.
\# run用于在远程PC执行命令.
def ls():
with cd('/home/workspace/project'):
local('touch 1.log')
with cd('/home/workspace/project2'):
local('touch 2.log')
\#@parallel, 可以设置是否并行执行
\#@serial
def pull():
with cd('/home/workspace/project'):
run('git pull')
def clean():
with cd('/home/workspace/project'):
run('bash clean.sh')
@hosts('192.168.1.113')
def robot(device):
with cd('/home/workspace/project'):
run('bash run.sh %s robot && sleep 1' % device)
以上就是一个简单的fabfile.py, 其中定义的函数均对应一个fab中的可执行命令.
其中有两个小的注意事项：
A.在远程机器的run.sh中如果要执行一些非系统常见的工具,最好指定为绝对路径. 且可以适当地使用nohup的方式.
B.执行其他脚本或者命令后最好加上sleep,以防止Fabric过早地关闭与远程PC连接的session,而导致执行任务失败.
2. 执行过程: fabric执行会默认选取当前目录下的fabfile.py文件,
代码如下:
fab clean
fab pull
fab robot:hosts="192.168.1.115",device=5560
可以通过hosts参数给fabric传入指定的远程PC, 该hosts参数的优先级比env.hosts的要高.
也可以给fab中的命令传递参数,如device.
此外,还可以通过fab -f otherFabFile.py clean来指定其他的fabric文件.
如果需要并行执行的话,也可以传递参数如fab -P -z 15 pull, 15表示并行执行的PC数量.
以上,只是一些简单的用法,希望本文所述对大家的Python程序设计有所帮助。

