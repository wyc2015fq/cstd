# openstack常用命令-安全组相关 - weixin_33985507的博客 - CSDN博客
2017年05月03日 10:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
查看安全组
> 
nova secgroup-list 
查看安全组内的规则
> 
nova secgroup-list-rules default
创建安全组
> 
nova secgroup-create *GroupName Describtion*
eg：nova secgroup-create* global “globalallow”*
添加安全组规则
> 
nova secgroup-add-rule secGroupName ip-protocol from-port to-port CIDR
eg: nova secgroup-add-rule global tcp 80 80 0.0.0.0/0
删除安全组
> 
nova secgroup-delete secGroupName
