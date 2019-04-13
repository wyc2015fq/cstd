
# 再探kubernetes1.13: 1: 离线安装准备 - 知行合一 止于至善 - CSDN博客

2019年03月06日 07:05:30[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：302所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)



这篇文章整理以下kubernetes离线安装二进制文件所需地址。并以此为基础展开后续安装和特性的确认。
# 所需内容
安装需要如下二进制文件：
kubernetes-server相关二进制
docker二进制
etcd二进制
cfssl相关二进制文件
cni相关二进制文件
# 下载脚本
将下载连接进行管理，并将下载下的二进制文件中需要用到的部分进行分类存放，可以使用如下脚本，同时也放到github的easypack中了。
`[root@host shell]# cat step0-get-binaries.sh 
#!/bin/sh
TYPE_OS=linux
TYPE_ARCH=amd64
VER_CFSSL=R1.2
VER_KUBERNETES=v1.13.4
VER_ETCD=v3.3.12
VER_DOCKER=17.03.2-ce
VER_CNI=v0.7.4
DIR_DOWNLOAD=/tmp

mkdir -p ${DIR_DOWNLOAD}
cd ${DIR_DOWNLOAD}
echo "## clear previous download binaries"
rm -rf ${DIR_DOWNLOAD}/binaries
echo "## create binaries download dir"
mkdir -p binaries
cd binaries
echo "## download cfssl"
date
mkdir -p cfssl
cd cfssl
echo "## get cfssl_${TYPE_OS}-${TYPE_ARCH} with name cfssl"
curl -Lo cfssl https://pkg.cfssl.org/${VER_CFSSL}/cfssl_linux-amd64 >/dev/null 2>&1
echo "## get cfssljson_${TYPE_OS}-${TYPE_ARCH} with name cfssljson"
curl -Lo cfssljson https://pkg.cfssl.org/${VER_CFSSL}/cfssljson_linux-amd64 >/dev/null 2>&1
echo "## get cfssl-certinfo_${TYPE_OS}-${TYPE_ARCH} with name cfssljson"
curl -Lo cfssl-certinfo https://pkg.cfssl.org/${VER_CFSSL}/cfssl-certinfo_linux-amd64 >/dev/null 2>&1
cd ..
ls cfssl
echo
echo "## download kubernetes"
mkdir -p kubernetes
cd kubernetes
echo "curl -Lo kubernetes.tar.gz https://dl.k8s.io/${VER_KUBERNETES}/kubernetes-server-${TYPE_OS}-${TYPE_ARCH}.tar.gz"
curl -Lo kubernetes.tar.gz https://dl.k8s.io/${VER_KUBERNETES}/kubernetes-server-${TYPE_OS}-${TYPE_ARCH}.tar.gz >/dev/null 2>&1
tar zxf kubernetes.tar.gz
mv kubernetes/server/bin/kube-apiserver .
mv kubernetes/server/bin/kube-controller-manager .
mv kubernetes/server/bin/kubectl .
mv kubernetes/server/bin/kubelet .
mv kubernetes/server/bin/kube-proxy .
mv kubernetes/server/bin/kube-scheduler .
rm -rf kubernetes
rm kubernetes.tar.gz
cd ..
ls kubernetes
echo 
echo "## download etcd"
mkdir -p etcd
cd etcd
curl -Lo etcd-${VER_ETCD}-${TYPE_OS}-${TYPE_ARCH}.tar.gz https://github.com/coreos/etcd/releases/download/${VER_ETCD}/etcd-${VER_ETCD}-${TYPE_OS}-${TYPE_ARCH}.tar.gz >/dev/null 2>&1
tar zxf etcd-${VER_ETCD}-${TYPE_OS}-${TYPE_ARCH}.tar.gz
mv -f etcd-${VER_ETCD}-${TYPE_OS}-${TYPE_ARCH}/etcd* .
rm -rf etcd-${VER_ETCD}-${TYPE_OS}-${TYPE_ARCH}
rm etcd-${VER_ETCD}-${TYPE_OS}-${TYPE_ARCH}.tar.gz
cd ..
echo "## download docker"
mkdir -p docker
cd docker
curl -Lo docker-${VER_DOCKER}.tgz https://download.docker.com/${TYPE_OS}/static/stable/x86_64/docker-${VER_DOCKER}.tgz >/dev/null 2>&1
tar zxf docker-${VER_DOCKER}.tgz
mv docker tmpdocker
mv -f tmpdocker/docker* .
rm -rf tmpdocker
rm docker-${VER_DOCKER}.tgz
cd ..
ls docker
echo
echo "## download cni"
mkdir -p cni
cd cni
curl -Lo cni-plugins-${TYPE_ARCH}-${VER_CNI}.tgz https://github.com/containernetworking/plugins/releases/download/${VER_CNI}/cni-plugins-${TYPE_ARCH}-${VER_CNI}.tgz >/dev/null 2>&1
tar zxf cni-plugins-${TYPE_ARCH}-${VER_CNI}.tgz
rm cni-plugins-${TYPE_ARCH}-${VER_CNI}.tgz
cd ..
ls cni
date
echo "## download completed"
which tree >/dev/null 2>&1
if [ $? -eq 0 ]; then
  tree "${DIR_DOWNLOAD}/binaries"
else
  find ${DIR_DOWNLOAD}/binaries -type f
fi
[root@host shell]#`
# 执行确认
`[root@host shell]# sh step0-get-binaries.sh 
## clear previous download binaries
## create binaries download dir
## download cfssl
Wed Mar  6 07:02:11 CST 2019
## get cfssl_linux-amd64 with name cfssl
## get cfssljson_linux-amd64 with name cfssljson
## get cfssl-certinfo_linux-amd64 with name cfssljson
cfssl  cfssl-certinfo  cfssljson
## download kubernetes
curl -Lo kubernetes.tar.gz https://dl.k8s.io/v1.13.4/kubernetes-server-linux-amd64.tar.gz
kube-apiserver	kube-controller-manager  kube-proxy  kube-scheduler  kubectl  kubelet
## download etcd
## download docker
docker	docker-containerd  docker-containerd-ctr  docker-containerd-shim  docker-init  docker-proxy  docker-runc  dockerd
## download cni
bridge	dhcp  flannel  host-device  host-local	ipvlan	loopback  macvlan  portmap  ptp  sample  tuning  vlan
Wed Mar  6 07:02:53 CST 2019
## download completed
/tmp/binaries
|-- cfssl
|   |-- cfssl
|   |-- cfssl-certinfo
|   `-- cfssljson
|-- cni
|   |-- bridge
|   |-- dhcp
|   |-- flannel
|   |-- host-device
|   |-- host-local
|   |-- ipvlan
|   |-- loopback
|   |-- macvlan
|   |-- portmap
|   |-- ptp
|   |-- sample
|   |-- tuning
|   `-- vlan
|-- docker
|   |-- docker
|   |-- docker-containerd
|   |-- docker-containerd-ctr
|   |-- docker-containerd-shim
|   |-- dockerd
|   |-- docker-init
|   |-- docker-proxy
|   `-- docker-runc
|-- etcd
|   |-- etcd
|   `-- etcdctl
`-- kubernetes
    |-- kube-apiserver
    |-- kube-controller-manager
    |-- kubectl
    |-- kubelet
    |-- kube-proxy
    `-- kube-scheduler
5 directories, 32 files
[root@host shell]#`网络条件不好的话，可能需要根据脚本中的动作自己进行下载/解压和mv操作了。

