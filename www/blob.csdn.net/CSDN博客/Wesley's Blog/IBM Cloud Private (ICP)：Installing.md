
# IBM Cloud Private (ICP)：Installing - Wesley's Blog - CSDN博客


2018年07月17日 19:24:22[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：361


[系统配置要求](#系统配置要求)
[集群安装准备](#集群安装准备)[配置集群](#配置集群)
[为IBM Cloud Private设置Docker](#为ibm-cloud-private设置docker)[手动安装Docker](#手动安装docker)
[配置Docker Engine](#配置docker-engine)
[使用bind mount挂载Docker默认存储位置](#使用bind-mount挂载docker默认存储位置)[给手动安装Docker确定默认存储目录](#给手动安装docker确定默认存储目录)
[给自动安装Docker确定默认存储目录](#给自动安装docker确定默认存储目录)
[使用bind mount挂载Docker其他默认存储位置](#使用bind-mount挂载docker其他默认存储位置)
[安装IBM Cloud Private](#安装ibm-cloud-private)[安装ICP Cloud Native环境](#安装icp-cloud-native环境)[Step 1: 在boot node上安装Docker](#step-1-在boot-node上安装docker)
[Step 2: 设置安装环境](#step-2-设置安装环境)
[Step 3: 手动配置集群(可选)](#step-3-手动配置集群可选)
[Step 4: 为所有集群节点配置Docker](#step-4-为所有集群节点配置docker)
[Step 5: 部署环境](#step-5-部署环境)
[访问集群](#访问集群)
[安装之后](#安装之后)

[安装过程中配置选项](#安装过程中配置选项)

# 系统配置要求
链接：
[ICP配置要求](https://www.ibm.com/support/knowledgecenter/SSBS6K_2.1.0.3/supported_system_config/hardware_reqs.html)
[支持Docker版本](https://www.ibm.com/support/knowledgecenter/SSBS6K_2.1.0.3/supported_system_config/supported_docker.html)
[默认端口](https://www.ibm.com/support/knowledgecenter/SSBS6K_2.1.0.3/supported_system_config/required_ports.html)
# 集群安装准备
## 配置集群
在安装ICP之前，首先要决定两点：
检查系统配置要求
决定集群架构，并且获得所有节点的IP地址
准备每个节点：
确保默认端口没有被使用
在每个节点配置/etc/hosts文件
对于OpenStack环境，/etc/hosts是由cloud-init service管理，需要防止它修改/etc/hosts文件.在/etc/cloud/cloud.cfg文件中, 确保：
manage_etc_hosts: false
保证所有节点相互连接
保证每个节点支持Python 2.6 or 2.7
同步所有节点的世家，从而能够使用NTP(Network Time Protocol)
保证每个node都安装了SSH Client
在集群每个节点上安装socat。[安装Socat](http://www.dest-unreach.org/socat/doc/README)
## 为IBM Cloud Private设置Docker
ICP需要Docker，必须手动安装Docker在boot node。 installer可以自动安装Docker在剩余的配置好的
节点（master, worker, proxy, management, VA)
手动安装ICP提供的Docker安装包，或者安装ICP支持的Docker版本
### 手动安装Docker
From Docker Website：
For Ubuntu：[https://docs.docker.com/engine/installation/](https://docs.docker.com/engine/installation/)
From IBM Could Private：
确保各个节点包管理工具（RPM， APT）允许package update
下载Docker package：[ICP Docker packages](https://www.ibm.com/support/knowledgecenter/SSBS6K_2.1.0.3/supported_system_config/supported_docker.html#packages)
安装Docker
对Linux 64-bit, run this command:
chmod +x icp-docker-17.12.1_x86_64.bin
sudo ./icp-docker-17.12.1_x86_64.bin –install
要卸载的话就把–install改为–uninstall
检验安装
使用指令：
sudo systemctl start docker
配置Docker Engine
[配置步骤](https://www.ibm.com/support/knowledgecenter/SSBS6K_2.1.0.3/installing/install_docker.html#docker_web)
使用bind mount挂载Docker默认存储位置
如果需要更改默认存储位置，需要在安装ICP之前使用bind mount进行更改
给手动安装Docker确定默认存储目录
移除所有的Docker容器和镜像
docker rm -f $(docker ps -aq); docker rmi -f $(docker images -q)
停止Docker
systemctl stop docker
移除Docker存储目录
rm -rf /var/lib/docker
创建新的/var/lib/docker目录
mkdir /var/lib/docker
使用bind mount来设置新的位置，例如要设置新的位置/mnt/docker：
mkdir /mnt/docker
mount --rbind /mnt/docker /var/lib/docker
重新开始Docker服务：
systemctl start docker
给自动安装Docker确定默认存储目录
创建新的/var/lib/docker目录
mkdir /var/lib/docker
使用bind mount来设置新的位置，例如要设置新的位置/mnt/docker：
mkdir /mnt/docker
mount --rbind /mnt/docker /var/lib/docker
使用bind mount挂载Docker其他默认存储位置
[使用bind mount挂载Docker其他默认存储位置](https://www.ibm.com/support/knowledgecenter/SSBS6K_2.1.0.3/installing/storage_dir.html)
安装IBM Cloud Private
ICP Native environment设置了一个单节点或者多节点集群，这个版本只能使用一部分的额外组件
ICP Enterprise environment设置了一个单节点或者多节点集群，这个版本能使用所有的额外组件
安装ICP Cloud Native环境
主要有五个步骤：
在boot node上安装Docker
设置安装环境
手动配置集群(可选)
为所有集群节点配置Docker
部署环境
Step 1: 在boot node上安装Docker
可参考之前的步骤：[手动安装Docker](https://www.ibm.com/support/knowledgecenter/SSBS6K_2.1.0.3/installing/install_docker.html#manual)
Step 2: 设置安装环境
使用root权限登陆 boot node
下载安装文件：[IBM Passport Advantage](https://www-01.ibm.com/software/passportadvantage/)
对于Linux 64bit， 下载ibm-cloud-private-x86_64-2.1.0.3.tar.gz
解压
tar xf ibm-cloud-private-x86_64-2.1.0.3.tar.gz -O | sudo docker load
创建安装目录，存放配置文件
mkdir /opt/ibm-cloud-private-2.1.0.3;  \
 cd /opt/ibm-cloud-private-2.1.0.3
从安装image中提取配置文件
sudo docker run -v $(pwd):/data -e LICENSE=accept \
 ibmcom/icp-inception:2.1.0.3-ee \
 cp -r cluster /data
…
有点多，直接看官方文档吧：[官方文档](https://www.ibm.com/support/knowledgecenter/SSBS6K_2.1.0.3/installing/install_containers.html)
Step 3: 手动配置集群(可选)
Step 4: 为所有集群节点配置Docker
Step 5: 部署环境
访问集群
可以通过浏览器访问集群。想获取更多信息可以通过ICP management console和Kubernetes command line访问
安装之后
如果禁止了防火墙，则重启防火墙
保证ICP默认端口开启
备份boot node。 复制/<installation_directory>/cluster到一个安全的位置
从bundled中安装其他软件
安装过程中配置选项
[详细](https://www.ibm.com/support/knowledgecenter/SSBS6K_2.1.0.3/installing/install_configuration.html)
在集群节点中分享SSH keys
配置集群节点密码验证
在hosts file中设置node角色
Customizing安装
使用config.yaml文件Customizing集群
配置集群节点来自动化安装Docker
在proxy nodes中配置资源限制
节点任务分配和交流
配置监控服务
配置GlusterFS存储
配置vSphere Cloud Provider
动态存储规定
加密集群数据网络with IPsec
给ICP确定CA（certificate authority）
使用dm-crypt加密volumes
用ICP集成VMware NSX-T 2.0
Enabling VA
设置外部负载均衡
生成K8s日志

