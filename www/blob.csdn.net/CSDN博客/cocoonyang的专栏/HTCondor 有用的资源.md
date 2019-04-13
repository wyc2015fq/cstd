
# HTCondor 有用的资源 - cocoonyang的专栏 - CSDN博客


2015年11月18日 06:00:49[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1507



[Getting started: Creating a multiple node Condor pool](https://spinningmatt.wordpress.com/2011/06/12/getting-started-creating-a-multiple-node-condor-pool/)

[1]HTCondor默认使用TCP向condor_collector服务器发送更新请求.*condor_collector*服务器 由CONDOR_VIEW_HOST设定
UPDATE_COLLECTOR_WITH_TCP

默认值为True. HTCondor 使用TCP更新 condor_collector.如设置为 False, the HTCondor daemons 将使用 UDP 来更新 condor_collector.

UPDATE_VIEW_COLLECTOR_WITH_TCP

When set to True, the HTCondor collector will use TCP to forward updates to condor_collector daemons specified by CONDOR_VIEW_HOST, instead of the default UDP. Defaults to False.

TCP_UPDATE_COLLECTORS

当 UPDATE_COLLECTOR_WITH_TCP 或 UPDATE_VIEW_COLLECTOR_WITH_TCP 被设置为 False时，.使用 TCP 更新 一组 condor_collector 服务器




[2]within the collector log of the form
DaemonCore: PERMISSION DENIED to host <xxx.xxx.xxx.xxx> for command 0 (UPDATE_STARTD_AD)indicates a permissions problem. Thecondor_startddaemons do not
 have write permission to thecondor_collectordaemon. This could be because you used domain names in yourHOSTALLOW_WRITEand/orHOSTDENY_WRITEconfiguration
 macros, but the domain name server (DNS) is not properly configured at your site. Without the proper configuration, HTCondor cannot resolve the IP addresses of your machines into fully-qualified domain names (an inverse look up). If this is the problem, then
 the solution takes one of two forms:Fix the DNS so that inverse look ups (trying to get the domain name from an IP address) works for your machines. You can either fix the DNS itself, or use theDEFAULT_DOMAIN_NAMEsetting in your HTCondor configuration file.
Use numeric IP addresses in theHOSTALLOW_WRITEand/orHOSTDENY_WRITEconfiguration macros instead of domain names. As an example of this, assume your site has a machine such as foo.your.domain.com, and it has two subnets, with IP addresses 129.131.133.10, and 129.131.132.10. If the configuration macro is set as
HOSTALLOW_WRITE = *.your.domain.comand this does not work, use
HOSTALLOW_WRITE = 192.131.133.*, 192.131.132.*
Alternatively, this permissions problem may be caused by being too restrictive in the setting of yourHOSTALLOW_WRITEand/orHOSTDENY_WRITEconfiguration macros. If it is, then
 the solution is to change the macros, for example from
HOSTALLOW_WRITE = condor.your.domain.comtoHOSTALLOW_WRITE = *.your.domain.comor possiblyHOSTALLOW_WRITE = condor.your.domain.com, foo.your.domain.com, \
 bar.your.domain.comAnother likely error messagewithin the collector log of the form
DaemonCore: PERMISSION DENIED to host <xxx.xxx.xxx.xxx> for command 5 (QUERY_STARTD_ADS)indicates a similar problem as above, but read permission is the problem (as opposed to write permission). Use the solutions given above.

**只在安装了特定软件的计算节点上运行计算任务，应如何配置？**

具体方法[4]分两步：
步骤1：
在安装了特定软件的计算节点的配置文件添加 参数
HAS_MY_SOFTWARE = True
然后 添加
STARTD_ATTRS = HAS_MY_SOFTWARE, $(STARTD_ATTRS)

步骤2：
在计算任务submit文件中添加
Requirements = (HAS_MY_SOFTWARE =?= True)

参考文献
[1][Using TCP to Send Updates to the condor_collector](https://research.cs.wisc.edu/htcondor/manual/v8.4.1/3_7Networking_includes.html#SECTION00475000000000000000)
[2 ][collector permission problem](http://research.cs.wisc.edu/htcondor/manual/v7.8/7_7Troubleshooting.html)
[3] https://github.com/htcondor/htcondor
[4 http://research.cs.wisc.edu/htcondor/manual/v7.8/7_2Setting_up.html
]


