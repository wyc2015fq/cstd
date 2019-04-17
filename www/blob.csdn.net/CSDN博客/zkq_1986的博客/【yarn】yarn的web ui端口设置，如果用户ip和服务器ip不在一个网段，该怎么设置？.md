# 【yarn】yarn的web ui端口设置，如果用户ip和服务器ip不在一个网段，该怎么设置？ - zkq_1986的博客 - CSDN博客





2019年03月14日 15:53:19[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：101








yarn的web ui端口设置中，如果用户ip和服务器ip不在一个网段，该怎么设置？

把ip改成0.0.0.0，适配所有ip

<property>

<name>yarn.resourcemanager.webapp.address</name>

<value>0.0.0.0:8088</value>

</property>



