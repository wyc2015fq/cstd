# 搭建基于Mesos的Kubernetes集群 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年07月25日 14:09:09[boonya](https://me.csdn.net/boonya)阅读数：2744








文章来源：    [http://geek.csdn.net/news/detail/90931](http://geek.csdn.net/news/detail/90931)


        Kubernetes是一个跨多个计算节点的管理容器化应用的系统，它提供了一系列基本的功能，如应用的自动化部署，维护和扩展等。Mesos是Apache下的开源分布式资源管理框架，它被称为是分布式系统的内核。把Kubernetes运行在Mesos集群之上，可以和其他的框架共享集群资源，提高集群资源的利用率。本文是“Kubernetes和Mesos集成指南”系列文章第一篇：实战部署。

        现在Kubernetes官方提供的部署基于Mesos的Kubernetes集群的文档相对简单，对于不熟悉Mesos的Kubernetes开发者或者用户来说，按照那个文档提供的说明进行部署，可能会比较困难，并且会遇到一些坑。文本是基于作者长期贡献Mesos和Kubernetes社区的开发经验， 分享给大家实战部署基于Mesos的Kubernetes集群。同时部署Kubernetes附加的一些服务，比如DNS和Dashboard等。


## 搭建基于Mesos的Kubernetes集群

### 部署架构

本文笔者将利用三台虚拟机来演示如何部署基于Mesos的Kubernetes集群，各个节点上的集群服务组件如下图所示：

![图片描述](https://img-blog.csdn.net/20160725085155312)

另外为了让大家可以尽快体验kubernetes和Mesos最新的特性，以及同时对kubernetes用户和开发者同时具有借鉴意义，我选择最新的社区代码来编译安装基于Mesos的kubernetes集群，如果读者想使用一个稳定的release版本，你可以下载对应的tar包，然后按照相同的步骤进行。

另外因为集群会有多个节点，开发者可能会修改部分源代码，为了在修改了源码之后，使修改可以快速便捷的在所有的机器上生效，我们将在wyq01.ibm.com这个机器上架设NFS服务，把它作为编译的机器，用来编译Mesos和kubernetes源代码。然后将编译的安装包目录 mount到其他的计算节点上。这样在修改了Mesos或者kubernetes的代码之后，只需要在wyq01.ibm.com进行编译然后只需要在另外两台计算节点上重启相应的服务就可以生效。

### 安装步骤

**1.准备环境**

准备三台Ubuntu 14.40的环境（物理机和虚拟机都可以），配置DNS或者/etc/hosts文件来保证相互通过机器名可以访问，并且关闭防火墙。

**2.在每一个节点上安装Docker**

最新版本的kubernetes支持多种容器的运行时，比如Docker，Rocket，CNI等，本文以最主流的Docker最为例子来演示。分别登陆这三台机器，执行以下命令安装Docker：

```
# apt-get update 
# apt-get install wget
# wget -qO- https://get.docker.com/ | sh
# docker version
```

Client:

```
Version:      1.11.1
 API version:  1.23
 Go version:   go1.5.4
 Git commit:   5604cbe
 Built:        Tue Apr 26 23:30:23 2016
 OS/Arch:      linux/amd64
```

Server:

```
Version:      1.11.1
 API version:  1.23
 Go version:   go1.5.4
 Git commit:   5604cbe
 Built:        Tue Apr 26 23:30:23 2016
 OS/Arch:      linux/amd64
```

在本演示环境中，笔者安装的是当时最新的docker版本，建议使用最新版本，如果你的机器上已经安装Docker，请检查kubernetes官方文档，查看你安装的Docker是不是符合要求。

另外由于你所在环境的限制，可能需要对Docker配置网络代理才可以在docker hub或者其他仓库中下载镜像：

编辑/etc/default/docker文件，在此文件中添加http_proxy的配置， 如下所示：

```
# vim /etc/default/docker 
export http_proxy=http://9.21.63.156:3128/
# service docker restart
```

**3.在master节点上编译 Mesos源代码**

Mesos官方目前没有提供Mesos的安装包，需要自己下载源码包进行build。本文以Mesos最新的代码为例进行编译安装， 登陆master节点wyq01.ibm.com执行以下步骤进行编译：

首先需要安装运行 Mesos 必须的第三方软件包：

```
# apt-get update
# apt-get install -y tar wget git
# apt-get install -y openjdk-7-jdk
# apt-get install -y autoconf libtool
# apt-get -y install build-essential python-dev python-boto libcurl4-nss-dev libsasl2-dev maven libapr1-dev libsvn-dev
```

下载源码包进行编译安装：

```
# cd /opt
# git clone https://git-wip-us.apache.org/repos/asf/mesos.git
# cd mesos
# ./bootstrap
# mkdir build && cd build
# ../configure --enable-install-module-dependencies --prefix=/opt/mesosinstall
# make –j3
# make install
```

编译安装完成之后，就可以在如下目录看到编译之后的二进制包和相关的管理脚本：

```
# ll /opt/mesosinstall/usr/local/sbin
total 5008
drwxr-xr-x 2 root root    4096 Jun 30 07:29 ./
drwxr-xr-x 9 root root    4096 Jun 27 13:46 ../
-rwxr-xr-x 1 root root 1659016 Jun 27 13:46 mesos-agent*
-rwxr-xr-x 1 root root     676 Jun 30 07:29 mesos-daemon.sh*
-rwxr-xr-x 1 root root 1753811 Jun 27 13:46 mesos-master*
-rwxr-xr-x 1 root root 1659016 Jun 27 13:46 mesos-slave*
-rwxr-xr-x 1 root root    1349 Jun 27 13:46 mesos-start-agents.sh*
-rwxr-xr-x 1 root root     888 Jun 27 13:46 mesos-start-cluster.sh*
-rwxr-xr-x 1 root root    1366 Jun 27 13:46 mesos-start-masters.sh*
-rwxr-xr-x 1 root root    1349 Jun 27 13:46 mesos-start-slaves.sh*
-rwxr-xr-x 1 root root    1185 Jun 27 13:46 mesos-stop-agents.sh*
-rwxr-xr-x 1 root root     635 Jun 27 13:46 mesos-stop-cluster.sh*
-rwxr-xr-x 1 root root    1200 Jun 27 13:46 mesos-stop-masters.sh*
-rwxr-xr-x 1 root root    1185 Jun 27 13:46 mesos-stop-slaves.sh*
```

我们将用mesos-daemon.sh脚本来启动Mesos服务，之前需要对配置：

编辑/opt/mesosinstall/usr/local/sbin/mesos-daemon.sh文件：

- 修改prefix变量的值为：/opt/mesosinstall/usr/local
- 在prefix的下一行添加行：

```bash
export LD_LIBRARY_PATH=${prefix}/lib
export MESOS_LAUNCHER_DIR=${prefix}/libexec/mesos
export MESOS_EXECUTOR_ENVIRONMENT_VARIABLES="{\"PATH\": \"${PATH}\",\"LD_LIBRARY_PATH\": \"${LD_LIBRARY_PATH}\"}"
export MESOS_WEBUI_DIR=${prefix}/share/mesos/webui
```

**4.在master节点上编译Kubernetes源代码**

登陆master节点wyq01.ibm.com执行以下步骤进行编译：

安装Golang：

由于我们需要在wyq01.ibm.com上编译kubernetes，它是由GO语言编写，所以需要在wyq01.ibm.com上安装Golang来支持kubernetes的编译。

注： 对不同的kubernetes版本，它对golang的版本都有所要求，建议安装前查看官方文档。 本文笔者使用最新的版本：

```
# wget https://storage.googleapis.com/golang/go1.6.2.linux-amd64.tar.gz
# tar -C /usr/local -xzf go1.6.2.linux-amd64.tar.gz
```

编辑/etc/profile,添加环境变量： 

export PATH=$PATH:/usr/local/go/bin

安装验证：

```
# go version
go version go1.6.2 linux/amd64
```

下载Kubernetes源码，进行编译：

```
# export GOPATH=/opt/k8s-workspace
# mkdir ${GOPATH}/src/k8s.io && cd ${GOPATH}/src/k8s.io 
# git clone https://github.com/kubernetes/kubernetes
# cd kubernetes
# export KUBERNETES_CONTRIB=mesos
# make
```

这个编译可能需要几分钟时间，根据你的机器性能而定。编译完成之后，可以在如下目录中看到软件包：

```
# ll _output/bin/
total 1911416
drwxr-xr-x 2 root root      4096 Jul 18 13:29 ./
drwxr-xr-x 3 root root      4096 Jul 18 13:16 ../
-rwxr-xr-x 1 root root   9297409 Jul 18 13:16 conversion-gen*
-rwxr-xr-x 1 root root   9272483 Jul 18 13:16 deepcopy-gen*
-rwxr-xr-x 1 root root  80621170 Jul 18 13:28 e2e.test*
-rwxr-xr-x 1 root root  66869805 Jul 18 13:28 e2e_node.test*
-rwxr-xr-x 1 root root 104306400 Jul 18 13:26 federation-apiserver*
-rwxr-xr-x 1 root root  60414440 Jul 18 13:27 federation-controller-manager*
-rwxr-xr-x 1 root root  58035190 Jul 18 13:22 gendocs*
-rwxr-xr-x 1 root root 101838179 Jul 18 13:23 genfeddocs*
-rwxr-xr-x 1 root root 106438886 Jul 18 13:22 genkubedocs*
-rwxr-xr-x 1 root root  58207386 Jul 18 13:22 genman*
-rwxr-xr-x 1 root root   6869800 Jul 18 13:22 genswaggertypedocs*
-rwxr-xr-x 1 root root  58011521 Jul 18 13:22 genyaml*
-rwxr-xr-x 1 root root  11587624 Jul 18 13:23 ginkgo*
-rwxr-xr-x 1 root root 140002296 Jul 18 13:21 hyperkube*
-rwxr-xr-x 1 root root 101576611 Jul 18 13:22 k8sm-controller-manager*
-rwxr-xr-x 1 root root 112922880 Jul 18 13:22 k8sm-executor*
-rwxr-xr-x 1 root root  60827412 Jul 18 13:21 k8sm-scheduler*
-rwxr-xr-x 1 root root 136713702 Jul 18 13:22 km*
-rwxr-xr-x 1 root root 110888496 Jul 18 13:26 kube-apiserver*
-rwxr-xr-x 1 root root 101163264 Jul 18 13:26 kube-controller-manager*
-rwxr-xr-x 1 root root  51172592 Jul 18 13:24 kube-dns*
-rwxr-xr-x 1 root root  51638520 Jul 18 13:24 kube-proxy*
-rwxr-xr-x 1 root root  59924816 Jul 18 13:27 kube-scheduler*
-rwxr-xr-x 1 root root  57873992 Jul 18 13:27 kubectl*
-rwxr-xr-x 1 root root 108958934 Jul 18 13:19 kubelet*
-rwxr-xr-x 1 root root 106313244 Jul 18 13:19 kubemark*
-rwxr-xr-x 1 root root   8646288 Jul 18 13:22 linkcheck*
-rwxr-xr-x 1 root root   4386176 Jul 18 13:22 mungedocs*
-rwxr-xr-x 1 root root   8832688 Jul 18 13:22 src*
-rwxr-xr-x 1 root root   3581608 Jul 18 13:16 teststale*
```

注： 在编译的时候，如果你按照官方的步骤进行编译可能会遇到如下的错误：

```
# make
make: *** No rule to make target `/*.go', needed by `_output/bin/deepcopy-gen'.  Stop.
```

现在社区中有一个issue在跟踪这个问题，参见[https://github.com/kubernetes/kubernetes/issues/28890](https://github.com/kubernetes/kubernetes/issues/28890)

获取详细的解决方案。

**5.在master节点上安装NFS**

为了节省安装时间，我们不需要在三台机器上分别编译 Mesos，可以直接将build的安装目录拷贝到其他两台机器上即可。但是特别是对于一个Mesos贡献者来说，我们需要定期的和社区最新的代码进行同步和重新build，所以为了避免每次重复的拷贝，我们采用共享文件的方式。

登陆wyq01.ibm.com执行如下命令安装配置NFS：

```
# apt-get install -y nfs-common nfs-kernel-server
# vim /etc/exports
/opt/mesosinstall *(rw,sync,no_root_squash,no_subtree_check)
# /etc/init.d/nfs-kernel-server start
# showmount -e localhost
Export list for localhost:
/opt/mesosinstall                     *
```

**6.在两台计算节点配置Mesos运行的环境和NFS**

登陆wyq02.ibm.com和wyq03.ibm.com，执行如下命令：

在两台计算节点上安装运行Mesos需要的安装包：

```
# apt-get update
# apt-get install -y openjdk-7-jdk
# apt-get install -y autoconf libtool
# apt-get -y install build-essential python-dev python-boto libcurl4-nss-dev libsasl2-dev maven libapr1-dev libsvn-dev
```

Mount Mesos安装目录：

```
# mkdir /opt/mesosinstall
# mount -t nfs -o nolock  wyq01.ibm.com:/opt/mesosinstall /opt/mesosinstall
# ll /opt/mesosinstall/usr/local/sbin/
total 2632
drwxr-xr-x 2 root root    4096 Jan 28 16:41 ./
drwxr-xr-x 9 root root    4096 Jan 15 20:02 ../
-rwxr-xr-x 1 root root     406 Jan 28 16:41 mesos-daemon.sh*
-rwxr-xr-x 1 root root 1698505 Jan 28 16:41 mesos-master*
-rwxr-xr-x 1 root root  954864 Jan 28 16:41 mesos-slave*
-rwxr-xr-x 1 root root     888 Jan 28 16:41 mesos-start-cluster.sh*
-rwxr-xr-x 1 root root    1366 Jan 28 16:41 mesos-start-masters.sh*
-rwxr-xr-x 1 root root    1349 Jan 28 16:41 mesos-start-slaves.sh*
-rwxr-xr-x 1 root root     635 Jan 28 16:41 mesos-stop-cluster.sh*
-rwxr-xr-x 1 root root    1200 Jan 28 16:41 mesos-stop-masters.sh*
-rwxr-xr-x 1 root root    1185 Jan 28 16:41 mesos-stop-slaves.sh*
```

**7.启动Mesos集群**

登陆wyq01.ibm.com启动Mesos master：

```
# cd /opt/mesosinstall/usr/local/sbin/
# rm -rf /var/lib/mesos /opt/mesoslog && mkdir /opt/mesoslog 
# ./mesos-daemon.sh mesos-master --work_dir=/var/lib/mesos --log_dir=/opt/mesoslog --registry_fetch_timeout=10mins
# ps -ef | grep mesos-master
root     12788     1  1 08:13 pts/0    00:00:00 /opt/mesosinstall/usr/local/sbin/mesos-master --work_dir=/var/lib/mesos --log_dir=/opt/mesoslog --registry_fetch_timeout=10mins
root     12807 11020  0 08:14 pts/0    00:00:00 grep --color=auto mesos-master
```

登陆wyq02.ibm.com和wyq03.ibm.com启动Mesos agent：

```
# cd /opt/mesosinstall/usr/local/sbin/
# rm -rf /var/lib/mesos /opt/mesoslog && mkdir /opt/mesoslog 
# ./mesos-daemon.sh mesos-slave --master=wyq01.ibm.com:5050 --work_dir=/var/lib/mesos --log_dir=/opt/mesoslog --executor_registration_timeout=10mins
# ps -ef | grep mesos-slave
root     11856 10060  0 08:15 pts/1    00:00:00 grep --color=auto mesos-slave
root     22678     1  0 Jul18 ?        00:00:11 /opt/mesosinstall/usr/local/sbin/mesos-slave --master=gradyhost1.eng.platformlab.ibm.com:5050 --work_dir=/var/lib/mesos --log_dir=/opt/mesoslog --executor_registration_timeout=10mins
```

注意：在kubernetes集成Mesos的环境中，在启动task的时候，Mesos fetcher会首先从Master节点下载kubernetes对应的exectuor km的二进制包，因为这个包相对比较大，当你的网络质量比较差的时候可能会导致下载或者executor接入超时，所以请务必调整如下两个参数:

–registry_fetch_timeout：下载的超时时间，默认为1分钟，建议修改。 

–executor_registration_timeout：Mesos executor注册到Mesos Agent的时间，默认为1分钟，建议修改。

验证Mesos集群是否安装成功：打开Mesos portal：

[http://wyq01.ibm.com:5050](http://wyq01.ibm.com:5050) 查看Agents节点的信息。

以上的步骤只是部署了一个非高可用的Mesos集群，如果你想部署一个高可用的Mesos集群，请继续参考如下步骤，如果不需要，请直接到下章节。

因为Mesos高可用集群依赖一个zookeeper集群，所以首先需要安装一个zookeeper集群，本文笔者将以一个节点的zookeeper集群为例，如果你想安装一个多个节点的高可用的zookeeper集群，请参见笔者的纪录:

[https://github.com/gradywang/Dockerfiles/tree/master/zookeeper/official](https://github.com/gradywang/Dockerfiles/tree/master/zookeeper/official)

执行如下命令在事先准备好的机器上，部署zookeeper服务，比如也可以是集群的一个机器wyq01.ibm.com:

```
# docker run -d \
  --name=zookeeper \
  --net=host \
  --restart=always mesoscloud/zookeeper
```

然后在事先准备好的多个运行Mesos master的机器上，执行以下命令在这些机器上启动Mesos master：
`# ./mesos-daemon.sh mesos-master --work_dir=/var/lib/mesos --log_dir=/opt/mesoslog --registry_fetch_timeout=10mins --quorum=2 --zk=zk://wyq01.ibm.com:2181/mesos`
运行以下命令，启动所有的Mesos Agent：

```
# ./mesos-daemon.sh mesos-slave --master=zk://wyq01.ibm.com:2181/mesos --work_dir=/var/lib/mesos --log_dir=/opt/mesoslog --executor_registration_timeout=10mins
```

**8.在Master节点上启动和配置kubernetes服务**

登陆wyq01.ibm.com节点，配置kubernetes管理服务：
- 设置环境变量：

```
# export PATH="/opt/k8s-workspace/src/k8s.io/kubernetes/_output/local/go/bin:$PATH"
# export API_SERV_PORT=8888
# export KUBERNETES_MASTER_IP=$(hostname -i)
# export KUBERNETES_MASTER=http://${KUBERNETES_MASTER_IP}:${API_SERV_PORT}
# export API_SERV_URL=${KUBERNETES_MASTER_IP}:${API_SERV_PORT}
# export SERVICE_CLUSTER_IP_RANGE=192.168.3.0/24
# export CLUSTER_CIDR=172.16.0.0/16
# export CLUSTER_DNS=192.168.3.10
# export CLUSTER_DOMAIN=cluster.local
# export MESOS_MASTER_HOST_NAME=$(hostname)
# export ETCD_HOST_IP=$(hostname -i)
# export MESOS_MASTER=${MESOS_MASTER_HOST_NAME}:5050
# export ETCD_CLIENT_PORT=4001
# export ETCD_URL=http://${ETCD_HOST_IP}:${ETCD_CLIENT_PORT}
# export MESOS_CLOUD_CONF=/tmp/mesos-cloud.conf
# cat <<EOF >${MESOS_CLOUD_CONF}
[mesos-cloud]
mesos-master = ${MESOS_MASTER}
EOF
```

注意：如果你的kubernetes将运行在一个高可用的Mesos集群上，那么请修改MESOS_MASTER的环境变量，如下：
`# export MESOS_MASTER= zk://wyq01.ibm.com:2181/mesos`- 启动etcd服务：

```
# docker run -d \
    --hostname $(uname -n) \
    --name etcd \
    --restart=always \
    --net host quay.io/coreos/etcd:v2.2.1 \
    --listen-client-urls http://0.0.0.0:${ETCD_CLIENT_PORT} \
    --advertise-client-urls ${ETCD_URL}
# docker ps
CONTAINER ID        IMAGE                        COMMAND                  CREATED             STATUS              PORTS               NAMES
cedce7ad31a1        quay.io/coreos/etcd:v2.2.1   "/etcd --listen-clien"   3 seconds ago       Up 3 seconds                            etcd 
# curl -L ${ETCD_URL}/v2/keys
{"action":"get","node":{"dir":true}}
```
- 安装flannel：

```
# wget https://github.com/coreos/flannel/releases/download/v0.5.5/flannel-0.5.5-linux-amd64.tar.gz
# tar -C /usr/local/ -xvf flannel-0.5.5-linux-amd64.tar.gz
# export PATH=/usr/local/flannel-0.5.5/:$PATH
```

在ETCD中配置flannel网络：

```
# curl -L ${ETCD_URL}/v2/keys/coreos.com/network/config -XPUT -d value='{ "Network": "'${CLUSTER_CIDR}'" }'

# nohup flanneld -etcd-endpoints ${ETCD_URL} > /tmp/flannel.log 2>&1 &
```
- 配置Docker：

```
# source /run/flannel/subnet.env
# ifconfig docker0 ${FLANNEL_SUBNET}
```

配置DOCKER_OPTS:

```
# echo --bip=${FLANNEL_SUBNET} --mtu=${FLANNEL_MTU}
--bip=172.16.54.1/24 --mtu=1472
# vim /etc/default/docker
DOCKER_OPTS="--bip=172.16.54.1/24 --mtu=1472"
# service docker restart
```
- 启动kubernetes服务：

启动Apiserver：

```
# nohup km apiserver \
    --address=${KUBERNETES_MASTER_IP} \
    --etcd-servers=${ETCD_URL} \
    --service-cluster-ip-range=${SERVICE_CLUSTER_IP_RANGE} \
    --port=${API_SERV_PORT} \
    --cloud-provider=mesos \
    --cloud-config=${MESOS_CLOUD_CONF} \
    --secure-port=0 \
    --v=2 >/tmp/apiserver.log 2>&1 &
```

启动controller manager：

```
# nohup km controller-manager \
    --master=${API_SERV_URL} \
    --cloud-provider=mesos \
    --cloud-config=${MESOS_CLOUD_CONF} \
    --v=2 >/tmp/controller.log 2>&1 &
```

启动scheduler：

```
# nohup km scheduler \
    --address=${KUBERNETES_MASTER_IP} \
    --mesos-master=${MESOS_MASTER} \
    --etcd-servers=${ETCD_URL} \
    --mesos-user=root \
    --api-servers=${API_SERV_URL} \
    --cluster-dns=${CLUSTER_DNS} \
    --cluster-domain=${CLUSTER_DOMAIN} \
    --v=2 >/tmp/scheduler.log 2>&1 &
```

查看进程是否启动成功，如果失败，请检查对应的日志：

```
# ps -ef | grep km
root     13347 11020  1 08:21 pts/0    00:00:00 km apiserver --address=9.111.255.10 --etcd-servers=http://9.111.255.10:4001 --service-cluster-ip-range=192.168.3.0/24 --port=8888 --cloud-provider=mesos --cloud-config=/tmp/mesos-cloud.conf --secure-port=0 --v=2
root     13354 11020  1 08:21 pts/0    00:00:00 km controller-manager --master=9.111.255.10:8888 --cloud-provider=mesos --cloud-config=/tmp/mesos-cloud.conf --v=2
root     13361 11020  2 08:21 pts/0    00:00:00 km scheduler --address=9.111.255.10 --mesos-master=wyq01.ibm.com:5050 --etcd-servers=http://9.111.255.10:4001 --mesos-user=root --api-servers=9.111.255.10:8888 --cluster-dns=192.168.3.10 --cluster-domain=cluster.local --v=2
root     13372 11020  0 08:21 pts/0    00:00:00 grep --color=auto km
```

配置kubectl ~/.kube/config，验证集群是否安装成功

```
# kubectl get service
NAME CLUSTER-IP EXTERNAL-IP PORT(S) AGE
k8sm-scheduler 172.16.22.63 <none> 10251/TCP 9s
kubernetes 172.16.0.1 <none> 443/TCP 7m
# kubectl get nodes
NAME                                 STATUS    AGE
wyq02.ibm.com   Ready     3m
wyq03.ibm.com   Ready     3m
```

**9.在Compute节点上配置kubernetes相关服务**

登陆wyq02.ibm.com和wyq03.ibm.com节点，配置kubernetes相关服务：
- 设置环境变量：

```
# export ETCD_HOST_IP=9.111.255.10   -wyq01.ibm.com的IP地址。
# export ETCD_CLIENT_PORT=4001
# export ETCD_URL=http://${ETCD_HOST_IP}:${ETCD_CLIENT_PORT}
```
- 安装flannel：

在Compute几点上安装flannel，可以让kubernetes集群中的不同计算节点上的服务或者POD相互连通：

```
# wget https://github.com/coreos/flannel/releases/download/v0.5.5/flannel-0.5.5-linux-amd64.tar.gz
# tar -C /usr/local/ -xvf flannel-0.5.5-linux-amd64.tar.gz
# export PATH=/usr/local/flannel-0.5.5/:$PATH
# nohup flanneld -etcd-endpoints ${ETCD_URL} > /tmp/flannel.log 2>&1 &
```
- 配置Docker：

```
# source /run/flannel/subnet.env
# ifconfig docker0 ${FLANNEL_SUBNET}
```

配置DOCKER_OPTS:

```
# echo --bip=${FLANNEL_SUBNET} --mtu=${FLANNEL_MTU}
--bip=172.16.54.1/24 --mtu=1472
# vim /etc/default/docker
DOCKER_OPTS="--bip=172.16.54.1/24 --mtu=1472"
# service docker restart
```

**10.安装部署Kubernetes重要的附加服务**

默认情况下，kubernetes不带DNS和Dashboard服务，我们需要在安装完Kubernetes之后另外进行部署。Kubernetes的DNS和Dashboard是以POD的方式运行的，这样的化它们就可以被kubernetes进行管理。

登陆wyq01.ibm.com 节点，安装部署Kubernetes DNS和Dashboard：

安装部署DNS：

```
# export DNS_REPLICAS=1
# export DNS_DOMAIN="cluster.local"   -和之前设置的CLUSTER_DOMAIN变量相同
# export DNS_SERVER_IP="192.168.3.10" –和之前设置的CLUSTER_DNS变量相同
# export KUBE_ROOT="/opt/k8s-workspace/src/k8s.io/kubernetes"
```

创建kubernetes DNS的RC：
`# sed -e "s/\\\$DNS_REPLICAS/${DNS_REPLICAS}/g;s/\\\$DNS_DOMAIN/${DNS_DOMAIN}/g;" "${KUBE_ROOT}/cluster/saltbase/salt/kube-dns/skydns-rc.yaml.sed" > /tmp/skydns-rc.yaml`
编辑/tmp/skydns-rc.yaml文件，为kubernetes DNS容器指定kubernetes master的URL，例如：

```
args:
        # command = "/kube-dns"
        - --domain=cluster.local.
        - --dns-port=10053
        - --kube-master-url=http://9.111.255.10:8888
        ports:

# kubectl create -f /tmp/skydns-rc.yaml
```

创建kubernetes DNS的Service：

```
# sed -e "s/\\\$DNS_SERVER_IP/${DNS_SERVER_IP}/g" "${KUBE_ROOT}/cluster/saltbase/salt/kube-dns/skydns-svc.yaml.sed" > /tmp/skydns-svc.yaml
# kubectl create -f /tmp/skydns-svc.yaml
```

安装部署Dashboard：
`# cp ${KUBE_ROOT}/cluster/addons/dashboard/dashboard-controller.yaml /tmp/dashboard-controller.yaml`
编辑/tmp/dashboard-controller.yaml文件，为Dashboard容器指定Kubernetes Apiserver的地址，例如：

```
requests:
cpu: 100m
memory: 50Mi
args:
- --apiserver-host=http://9.111.255.10:8888
ports:
```

创建Dashboard的RC：
`# kubectl create -f /tmp/dashboard-controller.yaml`
创建Dashboard的Service：
`# kubectl create -f ${KUBE_ROOT}/cluster/addons/dashboard/dashboard-service.yaml`
获取Dashboard的URL进行验证：

```
# kubectl cluster-info
Kubernetes master is running at http://9.111.255.10:8888
KubeDNS is running at http://9.111.255.10:8888/api/v1/proxy/namespaces/kube-system/services/kube-dns
kubernetes-dashboard is running at http://9.111.255.10:8888/api/v1/proxy/namespaces/kube-system/services/kubernetes-dashboard
```

访问Kubernetes的Dashboard：[http://9.111.255.10:8888/api/v1/proxy/namespaces/kube-system/services/kubernetes-dashboard](http://9.111.255.10:8888/api/v1/proxy/namespaces/kube-system/services/kubernetes-dashboard)
 如果页面访问成功，则安装成功。


作者简介：王勇桥，80后的IT工程师，供职于IBM多年，主要从事云计算领域相关的工作，Mesos、Docker、Kubernetes社区的贡献者。平时喜欢在业余时间研究DevOps相关的应用， 对自动化部署，持续集成，资源调度有较深的研究。



