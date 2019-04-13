
# LSF系统介绍 - Augusdi的专栏 - CSDN博客


2015年05月08日 23:19:28[Augusdi](https://me.csdn.net/Augusdi)阅读数：9406


﻿﻿
LSF系统介绍
http://scc.ustc.edu.cn/zh_CN/ 中科大超算中心
http://www.sccas.cn/gb/index.html 中科院超算中心
http://www.ssc.net.cn/ 上涨超算中心
LSF简介
LSF(Load Sharing Facility)是分布资源管理的工具，用来调度、监视、分析联网计算机的负载。
目的
通过集中监控和调度，充分共享计算机的CPU、内存、磁盘、License等资源
一组安装了LSF软件的计算机组成一个Cluster
Cluster内的资源统一监控和调度
LSF Cluster的组成
LSF 术语
Cluster
一组运行LSF软件的计算机（当然是用TCP/IP网络互连的），跟计算Cluster术语无关
命令
bhosts 列出cluster中的机器
lsid 显示cluster名字
lsclusters 显示cluster状态和规模
LSF 术语
Server Host
Cluster内提交和执行Job的计算机
Client Host
Cluster内仅仅提交Job的计算机
在科大的Cluster中，node1-node32是ServerHost
LSF 术语
Job
提交给LSF 的命令
LSF负责调度、控制、跟踪job
命令
bjobs 查看系统的Job
bsub 提交作业
bhist 查看作业历史
bkill kill一个作业

使用Platform LSF

LSF使用综述
故障分析
作业提交与管理
资源管理
系统监视
LSF使用综述
设置LSF 环境变量
% login as: test
Using keyboard-interactive authentication.
Password:
Last login: Mon Dec 21 09:31:29 2009 from 11.11.11.241
test@node69:~> env | grep LSF
LSF_SERVERDIR=/public/software/lsf/7.0/linux2.6-glibc2.3-x86_64/etc
LSF_LIBDIR=/public/software/lsf/7.0/linux2.6-glibc2.3-x86_64/lib
LSF_VERSION=7.0
LSF_BINDIR=/public/software/lsf/7.0/linux2.6-glibc2.3-x86_64/bin
XLSF_UIDDIR=/public/software/lsf/7.0/linux2.6-glibc2.3-x86_64/lib/uid
LSF_ENVDIR=/public/software/lsf/conf

作业提交:普通并行作业（要求程序为并行才能并行提交）
作业提交:Gauss作业
test@node69:~/gauss-test> bsub -W 60 -n 32 -q QN_Norm g03.lsf test397.com
Job <716> is submitted to queue <QN_Norm>.
参数说明: g03.lsf, Gauss运行关键字
-W 60 作业最长运行60分钟
- n 32 需要32个CPU
-q QN_Norm 指定QN_Norm队列
隐藏参数:output.%J, 标准输出文件(包括相关错误提示)
作业提交:Dock作业
test@node69:~/dock6-test> bsub –W 12:00 -a openmpi -n 4 mpirun.lsf /public/software/dock6-openmpi/bin/dock6.mpi -i test.in -o test.out
Job <818> is submitted to default queue <QS_Norm>.
参数说明: -a openmpi 指定用openmpi运算
-W 12:00 运行12小时
- n 4 需要32个CPU
-mpirun.lsf, 使用openmpi时的关键字
无-q 参数 指定QS_Norm队列
隐藏参数:output.%J, 标准输出文件(包括相关错误提示)

作业提交:blast作业
test@node69:~/mpiblast-test> bsub -W 360 -n 32 -q QN_Norm -a openmpi mpirun.lsf ./blast.sh
Job <819> is submitted to queue <QN_Norm>.
参数说明: -a openmpi 指定用openmpi运算
-W 360 运行360分钟
- n 32 需要32个CPU
-mpirun.lsf, 使用openmpi时的关键字
-q QN_Norm 指定QN_Norm队列
隐藏参数:output.%J, 标准输出文件(包括相关错误提示)

作业提交:普通串行作业
test@node69:~> bsub –W 60 a.out ./bowtie-build.sh 参数
Job <820> is submitted to default queue <QS_Norm>.
参数说明: -W 60 运行60分钟
无 - n 参数 使用1个CPU
无-q 参数 指定QS_Norm队列
隐藏参数:output.%J, 标准输出文件(包括相关错误提示)

作业提交:普通使用openmpi的MPI作业
与DOCK和blastmpi 相同:
test@node69:~/mpiblast-test> bsub -W 360 -n 32 -q QN_Norm -a openmpi mpirun.lsf ./blast.sh
Job <819> is submitted to queue <QN_Norm>.
参数说明: -a openmpi 指定用openmpi运算
-W 360 运行360分钟
- n 32 需要32个CPU
-mpirun.lsf, 使用openmpi时的关键字
-q QN_Norm 指定QN_Norm队列
隐藏参数:output.%J, 标准输出文件(包括相关错误提示)

作业提交:普通使用openmpi的MPI作业
与DOCK和blastmpi 相同:
test@node69:~/mpiblast-test> bsub -W 360 -n 32 -q QN_Norm -a openmpi mpirun.lsf ./blast.sh
Job <819> is submitted to queue <QN_Norm>.
参数说明: -a openmpi 指定用openmpi运算
-W 360 运行360分钟
- n 32 需要32个CPU
-mpirun.lsf, 使用openmpi时的关键字
-q QN_Norm 指定QN_Norm队列
隐藏参数:output.%J, 标准输出文件(包括相关错误提示)
作业提交:普通使用openmpi的MPI作业
与DOCK和blastmpi 相同:
test@node69:~/mpiblast-test> bsub -W 360 -n 32 -q QN_Norm -a openmpi mpirun.lsf ./blast.sh
Job <819> is submitted to queue <QN_Norm>.
参数说明: -a openmpi 指定用openmpi运算
-W 360 运行360分钟
- n 32 需要32个CPU
-mpirun.lsf, 使用openmpi时的关键字
-q QN_Norm 指定QN_Norm队列
隐藏参数:output.%J, 标准输出文件(包括相关错误提示)
作业提交:交互式图形作业和作业数组
test@node69:~/mpiblast-test> bsub –Ip xclock
Job <819> is submitted to queue <QS_Norm>.
参数说明: -Ip 需要伪终端的交互方式图形支持
作业数组:
> Bsub –J Jobname[1-100] –i input.%I –o outpout.%I Exec.out
作业在线监视
test@node69:~> bjobs -w
JOBID USER STAT QUEUE FROM_HOST EXEC_HOST JOB_NAME SUBMIT_TIME
test RUN QS_Norm node69 4*node10 mpirun.lsf /public/software/dock6-openmpi/bin/dock6.mpi -i test.in -o test.out Dec 21 19:29
test@node69:~> bjobs -l 818
Job <818>, User <test>, Project <default>, Status <RUN>, Queue <QS_Norm>, Comma
nd <mpirun.lsf /public/software/dock6-openmpi/bin/dock6.mp
i -i test.in -o test.out>, Share group charged </test>
Mon Dec 21 19:29:35: Submitted from host <node69>, CWD <$HOME/dock6-test>, Outp
ut File </home/test/dock6-test/output.%J>, 4 Processors Re
quested;
Mon Dec 21 19:29:39: Started on 4 Hosts/Processors <4*node10>, Execution Home <
/home/test>, Execution CWD </home/test/dock6-test>;
Mon Dec 21 19:47:18: Resource usage collected.
The CPU time used is 4174 seconds.
MEM: 96 Mbytes; SWAP: 703 Mbytes; NTHREAD: 23
PGID: 28118; PIDs: 28131 28133 28137 28138 28139 28118
28140 28141 28129
PGID: 28143; PIDs: 28143
PGID: 28142; PIDs: 28142
PGID: 28144; PIDs: 28144
PGID: 28145; PIDs: 28145
SCHEDULING PARAMETERS:
r15s r1m r15m ut pg io ls it tmp swp mem
loadSched - - - - - - - - - - -
loadStop - - - - - - - - - - -
作业在线监视
test@node69:~> bjobs -aw
JOBID USER STAT QUEUE FROM_HOST EXEC_HOST JOB_NAME SUBMIT_TIME
818 test RUN QS_Norm node69 4*node10 mpirun.lsf /public/software/dock6-openmpi/bin/dock6.mpi -i test.in -o test.out Dec 21 19:29
115 test DONE lost_and_found node70 node1 sleep 1000 Dec 19 16:31
116 test DONE lost_and_found node70 node62 sleep 100 Dec 19 17:07
117 test DONE lost_and_found node70 node62 sleep 100 Dec 19 17:07
119 test DONE lost_and_found node70 node61 sleep 100 Dec 19 17:07
118 test DONE lost_and_found node70 node69 sleep 100 Dec 19 17:07
120 test DONE lost_and_found node70 node70 sleep 100 Dec 19 17:07
-
作业在线监视
检查作业历史状态
test@node69:~> bhist -aw
Summary of time in seconds spent in various states:
JOBID USER JOB_NAME PEND PSUSP RUN USUSP SSUSP UNKWN TOTAL
223 test mpirun.lsf ./cpi-openmpi 6 0 5 0 0 0 11
224 test mpirun.lsf ./cpi-openmpi 5 0 5 0 0 0 10
225 test mpirun.lsf openmpi 3 0 4 0 0 0 7
226 test mpirun.lsf ./cpi-openmpi 5 0 7 0 0 0 12
227 test mpirun.lsf ./cpi-mpich 2 0 0 0 0 0 2
228 test mpirun.lsf ./cpi-mpich 4 0 1994 0 0 0 1998
229 test mpirun.lsf ./cpi-openmpi 6 0 7 0 0 0 13
作业在线监视
检查作业历史状态
test@node69:~> bhist -l 223
Job <223>, User <test>, Project <default>, Command <mpirun.lsf ./cpi-openmpi>
Sun Dec 20 15:05:40: Submitted from host <node69>, to Queue <default>, CWD <$HO
ME>, Output File </home/test/output.%J>, 16 Processors Req
uested;
Sun Dec 20 15:05:46: Dispatched to 16 Hosts/Processors <16*node62>;
Sun Dec 20 15:05:46: Starting (Pid 30493);
Sun Dec 20 15:05:46: Running with execution home </home/test>, Execution CWD </
home/test>, Execution Pid <30493>;
Sun Dec 20 15:05:51: Done successfully. The CPU time used is 9.6 seconds;
Sun Dec 20 15:05:51: Post job process done successfully;
Summary of time in seconds spent in various states by Sun Dec 20 15:05:51
PEND PSUSP RUN USUSP SSUSP UNKWN TOTAL
6 0 5 0 0 0 11
作业在线监视
检查作业历史状态
test@node69:~> bjobs
JOBID USER STAT QUEUE FROM_HOST EXEC_HOST JOB_NAME SUBMIT_TIME
818 test RUN QS_Norm node69 4*node10 * test.out Dec 21 19:29
test@node69:~> bpeek -f 818
<< output from stdout >>
Initializing MPI Routines...
Initializing MPI Routines...
Initializing MPI Routines...
Initializing MPI Routines...
杀掉：
test@node69:~> bkill 1480
作业管理
检查作业历史状态
机器负载状态

机器作业状态

机器分组

队列状态

队列状态

故障分析
Job exit analysis
LSF Keep the job exit as it does
“bhist –l <jobid>” and “bjobs –l <jobid>” check the job exit code
Submit a job with “-o %J.out” and check the output file <jobid>.out
Typical User Problems (cont.d)
“My job dies under LSF”
Check resource limits on queues
Check that the application and its data files are accessible from the execution host(s)
Is an application license available from the execution host?
Check the exit code reported by bjobs –l
Common exit codes
127 – Command not found
128 – Command invoked cannot execute
130 – Scripts terminated by Control-C
Typical User Problems
“My job was rejected by LSF”
Check resource requirement string, run time limit
Submission to an unauthorized queue or host
Requested soft limits exceeding a queues hard limits
Typical User Problems (cont.d)
“My job PENDs forever under LSF”
Has the user requested unrealistic resources?
More memory than any host has
Resource requirements may be too stringent
Is the users id valid on the execution host(s)?
The user may have requested exclusive execution
If FCFS scheduling is used, the user may be last
If fairshare scheduling is used, the user may have exhausted their fairshare allocation
Use bjobs –lp to see the pending reason
When all else fails….

下表对常见的作业状态解释：
状态 含义
作业正在队列中排队
PEND
作业正在被执行
RUN
作业已经执行完毕，并且正常退出
DONE
作业非正常退出
EXITED
作业在排队过程中被挂起
PSUSP
作业在运行过程中被人为强制挂起
USUSP
作业在运行过程中被系统挂起
SSUSP

附 1：LSF 作业管理系统和原有 LJRS 作业管理系统命令对照表
LJRS LSF
提交作业 qsub bsub
提交名为 run.sh 的作业脚本，使用
x 结点，每结点 y 个 CPU，总共需 qsub -l nodes=x:ppn=y -P default bsub -n z -R “span[ptile=y]” -q
要 z 个 CPU， 将作业提交到名称为 run.sh default run.sh
default 的队列（结点池）中
应 用 -o 参 数 指定输 出信 息文
保存在作业所提交目录的
件，否 则将 在作业 运行 主结点
作业输出信息
xxx.o(jobid)下
以 mail 的形式保存
查看尚未完成的作业信息 qstat（显示全部用户作业） bjobs（显示当前用户作业）
查看所有用户的作业信息 qstat bjobs -u all
查看指定用户 user1 的作业信息 qstat -u user1 bjobs -u user1
查看作业的详细信息 qstat -f jobid bjobs -l jobid
查看刚运行完毕的作业信息 无 bjobs -a
bstop JOBID（可以挂起正在运
挂起作业 qhold JOBID
行的作业）
释放作业 qrls JOBID bresume JOBID
移 动未运 行 的作业到 队 列（结点
qmov queue JOBID bswitch queue JOBID
池）queue 当中
删除一个作业 qdel jobid bkill jobid
查看作业已产生的输出信息 无 bpeek jobid
查看作业历史 无 bhist jobid
作业历史详细信息 无 bhist -l jobid
附 2、LSF 作业管理系统其他命令简介
1、bsub –I –m 参数
bsub –I 参数用于提交交互式作业。
bsub –m 参数用于向指定结点提交作业，下列命令将作业提交到指定结点 c0305 和 c0807 上
例如：bsub –m "c0305 c0807" -n 8 mpijob_qs ./mytest bqueue
2、bqueues 命令
bqueues 命令用于查看队列信息， 默认情况下， ueues 命令列出 LSF 系统中定义的全部队列信息，
bq
包括队列名、优先级、状态信息、最大可用资源数、排队作业数、运行作业数等信息。
“bqueues –l 队列名” 命令可以列出指定队列的详细信息，包括描述信息、该队列优先级、最大运
行时间限制、作业规模限制、队列所包含的结点、队列所包含的用户以及这些用户的动态优先级（提
交作业越多、规模也大，优先级越低）等信息。
3、brequeue
brequeue 命令用于作业重新排队，用户可以使用该命令终止指定的、隶属于自己的、并正在运行
的作业，该作业将以原有的作业号重新进行排队，重新获得调度、运行。
4、btop/bbot
btop/bbot 命令用于改变处于“PEND”状态的作业获得调度的次序，用户只能改变自己处于同一队
列内的作业的相对次序， p 使指定作业在同一队列内的， 所有同优先级的作业中最先获得调度。 ot
bto bb
则相反。
5、bhosts
bhosts 命令用于查看结点状态，处于“ok”状态的结点表示该节点可以接收用户作业。结点上已
经有作业运行或者负载过高都会导致“closed”状态。

