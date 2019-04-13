
# Kubernetes安装系列之Node-Kubelet安装 - 知行合一 止于至善 - CSDN博客

2019年03月28日 21:51:14[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：168标签：[kubernetes																](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)[k8s																](https://so.csdn.net/so/search/s.do?q=k8s&t=blog)[kubelet																](https://so.csdn.net/so/search/s.do?q=kubelet&t=blog)[安装																](https://so.csdn.net/so/search/s.do?q=安装&t=blog)[部署																](https://so.csdn.net/so/search/s.do?q=部署&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=安装&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=kubelet&t=blog)个人分类：[Kubernetes																](https://blog.csdn.net/liumiaocn/article/category/6328275)
[
																					](https://so.csdn.net/so/search/s.do?q=kubelet&t=blog)所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)[
							](https://so.csdn.net/so/search/s.do?q=kubelet&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=k8s&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)

这篇文章整理以下Node节点的kubelet的安装与设定方法，本文以脚本的方式进行固化，内容仍然放在github的easypack上。
# 整体操作
[https://blog.csdn.net/liumiaocn/article/details/88413428](https://blog.csdn.net/liumiaocn/article/details/88413428)
# kubelet的设定文件
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# cat /etc/k8s/kubelet.conf
```
```python
KUBELET_OPTS
```
```python
=
```
```python
"--logtostderr=true \
--v=4 \
--log-dir=/var/log/kubernetes \
--root-dir=/var/lib/kubelet \
--cert-dir=/etc/ssl/k8s \
--fail-swap-on=false \
--hostname-override=192.168.163.131 \
--bootstrap-kubeconfig=/etc/ssl/k8s/bootstrap.kubeconfig \
--kubeconfig=/etc/k8s/kubelet.kubeconfig \
--config=/etc/k8s/kubelet-config.yaml \
--pod-infra-container-image=gcr.io/google_containers/pause-amd64:3.1 \
--allow-privileged=true \
--event-qps=0 \
--kube-api-qps=1000 \
--kube-api-burst=2000 \
--registry-qps=0 \
--image-pull-progress-deadline=30m"
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
# config设定文件
从1.10开始，很多参数都需要在config指定的文件中进行设定，设定示例如下
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# cat /etc/k8s/kubelet-config.yaml
```
```python
kind: KubeletConfiguration
apiVersion: kubelet.config.k8s.io/v1beta1
authentication:
  anonymous:
    enabled:
```
```python
false
```
```python
webhook:
    enabled:
```
```python
true
```
```python
x509:
    clientCAFile:
```
```python
"/etc/ssl/ca/ca.pem"
```
```python
authorization:
  mode: Webhook
clusterDomain:
```
```python
"cluster.local"
```
```python
clusterDNS:
  -
```
```python
"10.0.0.2"
```
```python
podCIDR:
```
```python
"172.200.0.0/16"
```
```python
maxPods: 2000
serializeImagePulls:
```
```python
false
```
```python
hairpinMode: promiscuous-bridge
cgroupDriver: cgroupfs
runtimeRequestTimeout:
```
```python
"15m"
```
```python
rotateCertificates:
```
```python
true
```
```python
serverTLSBootstrap:
```
```python
true
```
```python
readOnlyPort: 0
port: 10250
address:
```
```python
"192.168.163.131"
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
# Systemd服务配置文件
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# cat /usr/lib/systemd/system/kubelet.service
```
```python
[
```
```python
Unit
```
```python
]
```
```python
Description
```
```python
=
```
```python
Kubernetes Kubelet Service
Documentation
```
```python
=
```
```python
https://github.com/GoogleCloudPlatform/kubernetes
After
```
```python
=
```
```python
docker.service
Requires
```
```python
=
```
```python
docker.service
```
```python
[
```
```python
Service
```
```python
]
```
```python
WorkingDirectory
```
```python
=
```
```python
/var/lib/kubelet
EnvironmentFile
```
```python
=
```
```python
-/etc/k8s/kubelet.conf
ExecStart
```
```python
=
```
```python
/usr/local/bin/kubelet
```
```python
$KUBELET_OPTS
```
```python
Restart
```
```python
=
```
```python
always
RestartSec
```
```python
=
```
```python
5
StartLimitInterval
```
```python
=
```
```python
0
```
```python
[
```
```python
Install
```
```python
]
```
```python
WantedBy
```
```python
=
```
```python
multi-user.target
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
# 脚本示例
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# cat step8-2-install-kubelet.sh
```
```python
#!/bin/sh
```
```python
.
```
```python
./install.cfg
```
```python
echo
```
```python
-e
```
```python
"\n##  kubelet service"
```
```python
systemctl stop kubelet 2
```
```python
>
```
```python
/dev/null
```
```python
mkdir
```
```python
-p
```
```python
${ENV_KUBE_DIR_BIN}
```
```python
${ENV_KUBE_DIR_ETC}
```
```python
${ENV_KUBE_OPT_LOG_DIR}
```
```python
${ENV_KUBELET_DIR_WORKING}
```
```python
chmod
```
```python
755
```
```python
${ENV_HOME_K8S}
```
```python
/*
```
```python
cp
```
```python
-p
```
```python
${ENV_HOME_K8S}
```
```python
/kubelet
```
```python
${ENV_KUBE_DIR_BIN}
```
```python
if
```
```python
[
```
```python
$?
```
```python
-ne 0
```
```python
]
```
```python
;
```
```python
then
```
```python
echo
```
```python
"please check kubelet binary files existed in
```
```python
${ENV_HOME_K8S}
```
```python
/ or not"
```
```python
exit
```
```python
fi
```
```python
# create kubelet configuration file
```
```python
cat
```
```python
>
```
```python
${ENV_KUBE_DIR_ETC}
```
```python
/
```
```python
${ENV_KUBE_KUBELET_ETC}
```
```python
<<
```
```python
EOF
KUBELET_OPTS="--logtostderr=
```
```python
${ENV_KUBE_OPT_LOGTOSTDERR}
```
```python
\\
--v=
```
```python
${ENV_KUBE_OPT_LOG_LEVEL}
```
```python
\\
--log-dir=
```
```python
${ENV_KUBE_OPT_LOG_DIR}
```
```python
\\
--root-dir=
```
```python
${ENV_KUBELET_DIR_WORKING}
```
```python
\\
--cert-dir=
```
```python
${ENV_SSL_K8S_DIR}
```
```python
\\
--fail-swap-on=
```
```python
${ENV_KUBELET_OPT_FAIL_SWAP_ON}
```
```python
\\
--hostname-override=
```
```python
${ENV_KUBE_NODE_HOSTNAME}
```
```python
\\
--bootstrap-kubeconfig=
```
```python
${ENV_SSL_K8S_DIR}
```
```python
/
```
```python
${ENV_KUBECONFIG_BOOTSTRAP}
```
```python
\\
--kubeconfig=
```
```python
${ENV_KUBE_DIR_ETC}
```
```python
/
```
```python
${ENV_KUBELET_KUBECONFIG}
```
```python
\\
--config=
```
```python
${ENV_KUBE_DIR_ETC}
```
```python
/
```
```python
${ENV_KUBELET_OPT_CONFIG}
```
```python
\\
--pod-infra-container-image=
```
```python
${ENV_KUBE_OPT_PAUSE}
```
```python
\\
--allow-privileged=
```
```python
${ENV_KUBE_OPT_ALLOW_PRIVILEGE}
```
```python
\\
--event-qps=
```
```python
${ENV_KUBELET_OPT_EVENT_QPS}
```
```python
\\
--kube-api-qps=
```
```python
${ENV_KUBELET_OPT_KPI_QPS}
```
```python
\\
--kube-api-burst=
```
```python
${ENV_KUBELET_OPT_API_BRUST}
```
```python
\\
--registry-qps=
```
```python
${ENV_KUBELET_OPT_REG_QPS}
```
```python
\\
--image-pull-progress-deadline=
```
```python
${ENV_KUBELET_OPT_IMG_PULL_DEADLINE}
```
```python
"
EOF
```
```python
# create kubelet config yaml file for config option
```
```python
cat
```
```python
>
```
```python
${ENV_KUBE_DIR_ETC}
```
```python
/
```
```python
${ENV_KUBELET_OPT_CONFIG}
```
```python
<<
```
```python
EOF 
kind: KubeletConfiguration
apiVersion: kubelet.config.k8s.io/v1beta1
authentication:
  anonymous:
    enabled:
```
```python
${ENV_KUBELET_CONFIG_OPT_ANONYMOUS}
```
```python
webhook:
    enabled:
```
```python
${ENV_KUBELET_CONFIG_OPT_WEBHOOK}
```
```python
x509:
    clientCAFile: "
```
```python
${ENV_SSL_CA_DIR}
```
```python
/
```
```python
${ENV_SSL_FILE_CA_PEM}
```
```python
"
authorization:
  mode:
```
```python
${ENV_KUBELET_CONFIG_OPT_MODE}
```
```python
clusterDomain: "
```
```python
${ENV_KUBELET_CONFIG_OPT_CLUSTER_DOMAIN}
```
```python
"
clusterDNS:
  - "
```
```python
${ENV_KUBELET_CONFIG_OPT_CLUSTER_DNS}
```
```python
"
podCIDR: "
```
```python
${ENV_KUBE_OPT_CLUSTER_IP_RANGE}
```
```python
"
maxPods:
```
```python
${ENV_KUBELET_CONFIG_OPT_MAXPODS}
```
```python
serializeImagePulls:
```
```python
${ENV_KUBELET_CONFIG_OPT_SERIALIZE_IMG_PULL}
```
```python
hairpinMode:
```
```python
${ENV_KUBELET_CONFIG_OPT_HAIRPIN}
```
```python
cgroupDriver:
```
```python
${ENV_KUBELET_CONFIG_OPT_CGROUP_DRIVER}
```
```python
runtimeRequestTimeout: "
```
```python
${ENV_KUBELET_CONFIG_OPT_REQUEST_TMO}
```
```python
"
rotateCertificates:
```
```python
${ENV_KUBELET_CONFIG_OPT_ROTATE_CERT}
```
```python
serverTLSBootstrap:
```
```python
${ENV_KUBELET_CONFIG_OPT_TLS_BOOTSTRAP}
```
```python
readOnlyPort:
```
```python
${ENV_KUBELET_CONFIG_OPT_READONLY_PORT}
```
```python
port:
```
```python
${ENV_KUBELET_CONFIG_OPT_PORT}
```
```python
address: "
```
```python
${ENV_KUBE_NODE_HOSTNAME}
```
```python
"
EOF
```
```python
# Create the kubelet service.
```
```python
cat
```
```python
>
```
```python
${ENV_KUBE_KUBELET_SERVICE}
```
```python
<<
```
```python
EOF
[Unit]
Description=Kubernetes Kubelet Service
Documentation=https://github.com/GoogleCloudPlatform/kubernetes
After=docker.service
Requires=docker.service
[Service]
WorkingDirectory=
```
```python
${ENV_KUBELET_DIR_WORKING}
```
```python
EnvironmentFile=-
```
```python
${ENV_KUBE_DIR_ETC}
```
```python
/
```
```python
${ENV_KUBE_KUBELET_ETC}
```
```python
ExecStart=
```
```python
${ENV_KUBE_DIR_BIN}
```
```python
/kubelet \
```
```python
$KUBELET_OPTS
```
```python
Restart=always
RestartSec=5
StartLimitInterval=0
[Install]
WantedBy=multi-user.target
EOF
```
```python
echo
```
```python
-e
```
```python
"\n##  daemon reload service "
```
```python
systemctl daemon-reload
```
```python
echo
```
```python
-e
```
```python
"\n##  start kubelet service "
```
```python
systemctl start kubelet
```
```python
echo
```
```python
-e
```
```python
"\n##  enable kubelet service "
```
```python
systemctl
```
```python
enable
```
```python
kubelet
```
```python
echo
```
```python
-e
```
```python
"\n##  check  kubelet status"
```
```python
systemctl status kubelet
```
```python
echo
```
```python
echo
```
```python
-e
```
```python
"\n##  get csr information"
```
```python
kubectl get csr
```
```python
echo
```
```python
-e
```
```python
"##  kubectl get nodes "
```
```python
kubectl get nodes -o wide
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
# 执行示例
为了执行方便，在这些脚本外边在包一层，统一使用如下脚本进行管理
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# sh all-k8s-mgnt.sh install kubelet
```
```python
## ACTION: install  Service: kubelet begins ...
```
```python
2019/03/24 20:06:26
```
```python
[
```
```python
INFO
```
```python
]
```
```python
generate received request
2019/03/24 20:06:26
```
```python
[
```
```python
INFO
```
```python
]
```
```python
received CSR
2019/03/24 20:06:26
```
```python
[
```
```python
INFO
```
```python
]
```
```python
generating key: rsa-2048
2019/03/24 20:06:26
```
```python
[
```
```python
INFO
```
```python
]
```
```python
encoded CSR
2019/03/24 20:06:26
```
```python
[
```
```python
INFO
```
```python
]
```
```python
signed certificate with serial number 100213249864002235085413152226418981333611978799
2019/03/24 20:06:26
```
```python
[
```
```python
WARNING
```
```python
]
```
```python
This certificate lacks a
```
```python
"hosts"
```
```python
field. This makes it unsuitable
```
```python
for
```
```python
websites. For
```
```python
more
```
```python
information see the Baseline Requirements
```
```python
for
```
```python
the Issuance and Management
of Publicly-Trusted Certificates, v.1.1.6, from the CA/Browser Forum
```
```python
(
```
```python
https://cabforum.org
```
```python
)
```
```python
;
```
```python
specifically, section 10.2.3
```
```python
(
```
```python
"Information Requirements"
```
```python
)
```
```python
.
/etc/ssl/k8s/cert-kubeproxy-key.pem  /etc/ssl/k8s/cert-kubeproxy.pem
Cluster
```
```python
"kubernetes"
```
```python
set.
User
```
```python
"kubelet-bootstrap"
```
```python
set.
Context
```
```python
"default"
```
```python
created.
Switched to context
```
```python
"default"
```
```python
.
```
```python
Cluster
```
```python
"kubernetes"
```
```python
set.
User
```
```python
"kube-proxy"
```
```python
set.
Context
```
```python
"default"
```
```python
created.
Switched to context
```
```python
"default"
```
```python
.
```
```python
clusterrolebinding.rbac.authorization.k8s.io/kubelet-bootstrap created
```
```python
##  kubelet service
```
```python
##  daemon reload service
```
```python
##  start kubelet service
```
```python
##  enable kubelet service
```
```python
##  check  kubelet status
```
```python
● kubelet.service - Kubernetes Kubelet Service
   Loaded: loaded
```
```python
(
```
```python
/usr/lib/systemd/system/kubelet.service
```
```python
;
```
```python
enabled
```
```python
;
```
```python
vendor preset: disabled
```
```python
)
```
```python
Active: active
```
```python
(
```
```python
running
```
```python
)
```
```python
since Sun 2019-03-24 20:06:29 CST
```
```python
;
```
```python
388ms ago
     Docs: https://github.com/GoogleCloudPlatform/kubernetes
 Main PID: 1134
```
```python
(
```
```python
kubelet
```
```python
)
```
```python
CGroup: /system.slice/kubelet.service
           ├─1134 /usr/local/bin/kubelet --logtostderr
```
```python
=
```
```python
true --v
```
```python
=
```
```python
4 --log-dir
```
```python
=
```
```python
/var/log/kubernetes --root-dir
```
```python
=
```
```python
/var/lib/kubelet --cert-dir
```
```python
=
```
```python
/etc/ssl/k8s -
```
```python
..
```
```python
.
           └─1160 systemd-run --description
```
```python
=
```
```python
Kubernetes systemd probe --scope
```
```python
true
```
```python
Mar 24 20:06:29 host131 kubelet
```
```python
[
```
```python
1134
```
```python
]
```
```python
: I0324 20:06:29.272697    1134 flags.go:33
```
```python
]
```
```python
FLAG: --file-check-frequency
```
```python
=
```
```python
"20s"
```
```python
Mar 24 20:06:29 host131 kubelet
```
```python
[
```
```python
1134
```
```python
]
```
```python
: I0324 20:06:29.272704    1134 flags.go:33
```
```python
]
```
```python
FLAG: --global-housekeeping-interval
```
```python
=
```
```python
"1m0s"
```
```python
Mar 24 20:06:29 host131 kubelet
```
```python
[
```
```python
1134
```
```python
]
```
```python
: I0324 20:06:29.272712    1134 flags.go:33
```
```python
]
```
```python
FLAG: --hairpin-mode
```
```python
=
```
```python
"promiscuous-bridge"
```
```python
Mar 24 20:06:29 host131 kubelet
```
```python
[
```
```python
1134
```
```python
]
```
```python
: I0324 20:06:29.272719    1134 flags.go:33
```
```python
]
```
```python
FLAG: --healthz-bind-address
```
```python
=
```
```python
"127.0.0.1"
```
```python
Mar 24 20:06:29 host131 kubelet
```
```python
[
```
```python
1134
```
```python
]
```
```python
: I0324 20:06:29.272726    1134 flags.go:33
```
```python
]
```
```python
FLAG: --healthz-port
```
```python
=
```
```python
"10248"
```
```python
Mar 24 20:06:29 host131 kubelet
```
```python
[
```
```python
1134
```
```python
]
```
```python
: I0324 20:06:29.272733    1134 flags.go:33
```
```python
]
```
```python
FLAG: --help
```
```python
=
```
```python
"false"
```
```python
Mar 24 20:06:29 host131 kubelet
```
```python
[
```
```python
1134
```
```python
]
```
```python
: I0324 20:06:29.272739    1134 flags.go:33
```
```python
]
```
```python
FLAG: --host-ipc-sources
```
```python
=
```
```python
"[*]"
```
```python
Mar 24 20:06:29 host131 kubelet
```
```python
[
```
```python
1134
```
```python
]
```
```python
: I0324 20:06:29.272755    1134 flags.go:33
```
```python
]
```
```python
FLAG: --host-network-sources
```
```python
=
```
```python
"[*]"
```
```python
Mar 24 20:06:29 host131 kubelet
```
```python
[
```
```python
1134
```
```python
]
```
```python
: I0324 20:06:29.272762    1134 flags.go:33
```
```python
]
```
```python
FLAG: --host-pid-sources
```
```python
=
```
```python
"[*]"
```
```python
Mar 24 20:06:29 host131 kubelet
```
```python
[
```
```python
1134
```
```python
]
```
```python
: I0324 20:06:29.272774    1134 flags.go:33
```
```python
]
```
```python
FLAG: --hostname-override
```
```python
=
```
```python
"192.168.163.131"
```
```python
##  get csr information
```
```python
No resources found.
```
```python
##  kubectl get nodes
```
```python
No resources found.
```
```python
## ACTION: install  Service: kubelet ends  ...
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
设定之后可以进行bootstrap的机制会自动发出csr请求，而通过kubectl certificate approve则可手动发行证书。
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# kubectl certificate approve node-csr-ySkXjxhHO0w8zy39-YXzSSVxDtwnYJUCuFxhseDPoLk
```
```python
certificatesigningrequest.certificates.k8s.io/node-csr-ySkXjxhHO0w8zy39-YXzSSVxDtwnYJUCuFxhseDPoLk approved
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# kubectl get csr
```
```python
NAME                                                   AGE   REQUESTOR                     CONDITION
node-csr-ySkXjxhHO0w8zy39-YXzSSVxDtwnYJUCuFxhseDPoLk   40s   kubelet-bootstrap             Approved,Issued
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
再次确认get nodes，则可以看到此节点已被master所识别出来。
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# kubectl get nodes
```
```python
NAME              STATUS   ROLES    AGE   VERSION
192.168.163.131   Ready
```
```python
<
```
```python
none
```
```python
>
```
```python
15s   v1.13.4
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# kubectl get nodes -o wide
```
```python
NAME              STATUS   ROLES    AGE   VERSION   INTERNAL-IP       EXTERNAL-IP   OS-IMAGE                KERNEL-VERSION          CONTAINER-RUNTIME
192.168.163.131   Ready
```
```python
<
```
```python
none
```
```python
>
```
```python
19s   v1.13.4   192.168.163.131
```
```python
<
```
```python
none
```
```python
>
```
```python
CentOS Linux 7
```
```python
(
```
```python
Core
```
```python
)
```
```python
3.10.0-957.el7.x86_64   docker://17.3.2
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```

