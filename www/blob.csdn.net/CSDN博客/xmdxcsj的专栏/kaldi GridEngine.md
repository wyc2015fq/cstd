# kaldi GridEngine - xmdxcsj的专栏 - CSDN博客





2017年02月25日 17:13:24[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2601








## 概况

queue.ql: gridengine 多机运行 

run.ql: 本地多进程，忽略除了JOB以外的其他参数
`queue.pl test.log  echo foo \| awk 's/f/F/';`
基本格式
`queue.pl <options> <log-file> <command>`
其中command对于”|>;”这些shell会解析的符号需要做转义 

queue.pl传入参数到qsub参数的对应关系 
![这里写图片描述](https://img-blog.csdn.net/20170225171301559?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## GridEngine

Grid Engine是分布式的资源管理工具，类似于Torque/SLURM/LAVA等工具。一开始是sun公司，后被oracle收购以后相关资料变少，而后oracle又将其移交给[Univa](http://www.univa.com/oracle)。 

最后一个开源版本是6.2u5，后面有两个主要分支： 

- 商业版本：oracle收购sun以后，继续出售商业版的Grid Engine，后交由Univa，称之为Univa Grid Engine 

- 开源版本：分别是Son of Grid Engine和Open Grid Scheduler
master节点作为queue management，master本身也可以作为node，shadow master相当于master的backup。 

安装操作参考[http://gridscheduler.sourceforge.net/CompileGridEngineSource.html](http://gridscheduler.sourceforge.net/CompileGridEngineSource.html)

### 安装部分

```
% export SGE_ROOT=< Your Target Directory >
% mkdir $SGE_ROOT
% scripts/distinst -all -local -noexit
% cd $SGE_ROOT
% ./install_qmaster
% ./install_execd
```

### 配置信息

#### Adding Nodes
- 安装grid engine 

将编译好的程序由master复制到其他机器 

保证default/common/act_qmaster是前面master的hostname 

运行 default/common/sgeexecd start- 将client的机器设置成三种host 

三种hosts分别是administrative hosts, execution hosts and submit hosts，每台机器都设置成这三种形式。

```
#查看三种类型host命令
qconf -sh
qconf -sel
qconf -ss
#添加host命令
qconf -ah <your-fqdn>
qconf -ae <your-fqdn> #修改complex_values字段        ram_free=20G,gpu=4 
qconf -as <your-fqdn>
```
- 将client组成group对应到queue

```
#add a group of hosts
qconf -ahgrp @allhosts
#add your clients to the group
qconf -mhgrp @allhosts
#set the group and slot  to the queue,like:
#hostlist @allhosts
#slots  20 #告知queue该节点分配多少slot，slot的个数跟cpu的核数有关
qconf -mq all.q
```

#### Configuring GridEngine
- 对job信息增加以下两行信息 

qconf -mc

```
gpu                 g           INT         <=    YES         YES        0        10000
ram_free            ram_free    MEMORY      <=    YES         JOB        1G       0
```
- 增加parallel environment 

qconf -ap smp

```
pe_name            smp
slots              9999
```
- 新增queue，修改相关字段 

qconf -aq all.q

```
hostlist              @allhosts
pe_list               make smp
rerun                 TRUE
slots                 25
prolog                <dir>/prolog.sh
```
- 全局配置sge的路径

```
cat /etc/profile
. <SGE_ROOT>/default/common/settings.sh
```

### 常用命令

```
##查看修改增加配置
qconf [a|d|m|s][m|q|conf|c|p]
#m: manager
#q: queue，查看所有queue使用qconf -sql
#conf: global conf, not connected with any queue
#c: GridEngine has the concept of "resources" which can be requested or specified by your jobs
#p: It's a simple parallel environment where GridEngine doesn't really do anything, it just reserves you a certain number of slots, so if you do qsub -pe smp 10 <your-script> you will get 10 CPU slots reserved; this can be useful for multi-threaded or multi-process jobs.首先通过qconf -ap smp增加一个名字为smp的pe，然后使用qconf -mq all.q修改对应的pe_list参数（make smp）将pe需要映射到all.q对应的queue。

queue.pl -q all.q -l arch=*64 -l gpu=1 -l mem_free=10G,ram_free=10G -pe smp 10
-q: name of queue, which is added by qconf -aq all.q
-l: params related to job, which is added by qconf -ac
-pe: set number of cpu slots

##关闭/启动node
qmod -d all.q@gpu04 #disable
qmod -e all.q@gpu04 #enable
```

## 参考

[http://kaldi-asr.org/doc/queue.html](http://kaldi-asr.org/doc/queue.html)






