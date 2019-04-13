
# docker pull提示x509错误的对应方法 - 知行合一 止于至善 - CSDN博客

2019年02月18日 20:40:17[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：597


在一台虚拟机上使用docker pull时出现了x509错误，相关原因与对应方法简单memo如下。
# 错误现象
在使用docker pull从dockerhub上下载镜像时提示如下错误
`docker: error pulling image configuration: Get https://production.cloudflare.docker.com/registry-v2/docker/registry/v2/blobs/sha256/4c/4cbebbc9d3846093f6de718a9b273ca7e8731f85ff971c603e75c188c6cd61f9/data?verify=1550473606-D3BPaHfBCjGMlwzZoDAUpbexs%2BI%3D: x509: certificate has expired or is not yet valid.
See 'docker run --help'.`
# 原因
虽然x509为认证相关的问题，但是经过确认为虚拟机的时间与实际时间发生过大偏差所导致。这个问题在进行k8s集群创建时多个节点时间如果不同步也会经常出现。
# 对应方法
使用ntpdate进行同步或者直接设定时间即可。
> 联网状态下的设定命令：ntpdate
> [cn.pool.ntp.org](http://cn.pool.ntp.org)


