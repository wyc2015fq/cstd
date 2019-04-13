
# Kubernetes安装系列之Master-Apiserver安装 - 知行合一 止于至善 - CSDN博客

2019年03月23日 21:03:44[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：234所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)



这篇文章整理以下Master节点的apiserver的安装与设定方法，本文以脚本的方式进行固化，内容仍然放在github的easypack上。
# 整体操作
[https://blog.csdn.net/liumiaocn/article/details/88413428](https://blog.csdn.net/liumiaocn/article/details/88413428)
# ApiServer设定文件
```python
KUBE_APISERVER_OPTS
```
```python
=
```
```python
"--logtostderr=true \
--v=4 \
--log-dir=/var/log/kubernetes \
--etcd-servers=https://192.168.163.131:2379 \
--bind-address=192.168.163.131 \
--secure-port=6443 \
--advertise-address=192.168.163.131 \
--allow-privileged=true \
--service-cluster-ip-range=172.200.0.0/16 \
--enable-admission-plugins=NamespaceLifecycle,LimitRanger,SecurityContextDeny,ServiceAccount,ResourceQuota,NodeRestriction \
--authorization-mode=RBAC,Node \
--enable-bootstrap-token-auth \
--token-auth-file=/etc/k8s/token.csv \
--service-node-port-range=30000-50000 \
--tls-cert-file=/etc/ssl/k8s/cert-k8s.pem  \
--tls-private-key-file=/etc/ssl/k8s/cert-k8s-key.pem \
--client-ca-file=/etc/ssl/ca/ca.pem \
--service-account-key-file=/etc/ssl/ca/ca-key.pem \
--etcd-cafile=/etc/ssl/ca/ca.pem \
--etcd-certfile=/etc/ssl/etcd/cert-etcd.pem \
--etcd-keyfile=//etc/ssl/etcd/cert-etcd-key.pem"
```
# Systemd服务设定文件
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
# cat /usr/lib/systemd/system/kube-apiserver.service
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
Kubernetes API Server
Documentation
```
```python
=
```
```python
https://github.com/kubernetes/kubernetes
After
```
```python
=
```
```python
etcd.service
Wants
```
```python
=
```
```python
etcd.service
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
EnvironmentFile
```
```python
=
```
```python
-/etc/k8s/apiserver.conf
ExecStart
```
```python
=
```
```python
/usr/local/bin/kube-apiserver
```
```python
$KUBE_APISERVER_OPTS
```
```python
Restart
```
```python
=
```
```python
on-failure
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
# cat step3-install-apiserver.sh
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
"\n##  kube-apiserver service"
```
```python
systemctl stop kube-apiserver 2
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
/
```
```python
{
```
```python
kubectl,kube-apiserver
```
```python
}
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
"please check kubectl and kube-apiserver binary files existed in
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
# create kube token file
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
${ENV_KUBE_API_TOKEN}
```
```python
<<
```
```python
EOF
```
```python
$(
```
```python
head
```
```python
-c 16 /dev/urandom
```
```python
|
```
```python
od -An -t x
```
```python
|
```
```python
tr
```
```python
-d
```
```python
' '
```
```python
)
```
```python
,kubelet-bootstrap,10001,"system:kubelet-bootstrap"
EOF
```
```python
# create kube-apiserver configuration file
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
${ENV_KUBE_API_CONF}
```
```python
<<
```
```python
EOF
KUBE_APISERVER_OPTS="--logtostderr=
```
```python
${ENV_KUBE_OPT_LOGTOSTDERR}
```
```python
\
--v=
```
```python
${ENV_KUBE_OPT_LOG_LEVEL}
```
```python
\
--log-dir=
```
```python
${ENV_KUBE_OPT_LOG_DIR}
```
```python
\\
EOF
```
```python
echo
```
```python
${ENV_ETCD_HOSTS}
```
```python
|
```
```python
awk
```
```python
-v etcd_names
```
```python
=
```
```python
"
```
```python
${ENV_ETCD_NAMES}
```
```python
"
```
```python
\
-v port
```
```python
=
```
```python
${ENV_ETCD_CLIENT_PORT}
```
```python
-F
```
```python
" "
```
```python
'BEGIN{
    printf("--etcd-servers=");
}
{
    for(cnt=1; cnt<NF; cnt++){
        printf("https://%s:%s,",
```
```python
$cnt
```
```python
,port);
    }
    printf("https://%s:%s ",
```
```python
$cnt
```
```python
,port);
}'
```
```python
>>
```
```python
${ENV_KUBE_DIR_ETC}
```
```python
/
```
```python
${ENV_KUBE_API_CONF}
```
```python
cat
```
```python
>>
```
```python
${ENV_KUBE_DIR_ETC}
```
```python
/
```
```python
${ENV_KUBE_API_CONF}
```
```python
<<
```
```python
EOF
--bind-address=
```
```python
${ENV_CURRENT_HOSTIP}
```
```python
\
--secure-port=
```
```python
${ENV_KUBE_OPT_API_SSL_PORT}
```
```python
\
--advertise-address=
```
```python
${ENV_CURRENT_HOSTIP}
```
```python
\
--allow-privileged=
```
```python
${ENV_KUBE_OPT_ALLOW_PRIVILEGE}
```
```python
\
--service-cluster-ip-range=
```
```python
${ENV_KUBE_OPT_CLUSTER_IP_RANGE}
```
```python
\
--enable-admission-plugins=
```
```python
${ENV_KUBE_ADM_PLUGINS}
```
```python
\
--authorization-mode=
```
```python
${ENV_KUBE_OPT_AUTH_MODE}
```
```python
\
--enable-bootstrap-token-auth \
--token-auth-file=
```
```python
${ENV_KUBE_DIR_ETC}
```
```python
/
```
```python
${ENV_KUBE_API_TOKEN}
```
```python
\
--service-node-port-range=
```
```python
${ENV_KUBE_OPT_CLUSTER_PORT_RANGE}
```
```python
\
--tls-cert-file=
```
```python
${ENV_SSL_K8S_DIR}
```
```python
/
```
```python
${ENV_SSL_K8S_CERT_PRIFIX}
```
```python
.pem  \
--tls-private-key-file=
```
```python
${ENV_SSL_K8S_DIR}
```
```python
/
```
```python
${ENV_SSL_K8S_CERT_PRIFIX}
```
```python
-key.pem \
--client-ca-file=
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
\
--service-account-key-file=
```
```python
${ENV_SSL_CA_DIR}
```
```python
/
```
```python
${ENV_SSL_FILE_CA_KEY}
```
```python
\
--etcd-cafile=
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
\
--etcd-certfile=
```
```python
${ENV_SSL_ETCD_DIR}
```
```python
/
```
```python
${ENV_SSL_ETCD_CERT_PRIFIX}
```
```python
.pem \
--etcd-keyfile=/
```
```python
${ENV_SSL_ETCD_DIR}
```
```python
/
```
```python
${ENV_SSL_ETCD_CERT_PRIFIX}
```
```python
-key.pem"
EOF
```
```python
# Create the kube-apiserver service.
```
```python
cat
```
```python
>
```
```python
${ENV_KUBE_API_SERVICE}
```
```python
<<
```
```python
EOF
[Unit]
Description=Kubernetes API Server
Documentation=https://github.com/kubernetes/kubernetes
After=etcd.service
Wants=etcd.service
[Service]
EnvironmentFile=-
```
```python
${ENV_KUBE_DIR_ETC}
```
```python
/
```
```python
${ENV_KUBE_API_CONF}
```
```python
ExecStart=
```
```python
${ENV_KUBE_DIR_BIN}
```
```python
/kube-apiserver \
```
```python
$KUBE_APISERVER_OPTS
```
```python
Restart=on-failure
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
"\n##  start kube-apiserver service "
```
```python
systemctl start kube-apiserver
```
```python
echo
```
```python
-e
```
```python
"\n##  enable kube-apiserver service "
```
```python
systemctl
```
```python
enable
```
```python
kube-apiserver
```
```python
echo
```
```python
-e
```
```python
"\n##  check  kube-apiserver status"
```
```python
systemctl status kube-apiserver
```
```python
echo
```
```python
-e
```
```python
"\n##  kubectl version"
```
```python
kubectl version
```
```python
echo
```
```python
-e
```
```python
"\n##  get cs"
```
```python
kubectl get cs
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
# sh step3-install-apiserver.sh
```
```python
##  kube-apiserver service
```
```python
##  daemon reload service
```
```python
##  start kube-apiserver service
```
```python
##  enable kube-apiserver service
```
```python
##  check  kube-apiserver status
```
```python
● kube-apiserver.service - Kubernetes API Server
   Loaded: loaded
```
```python
(
```
```python
/usr/lib/systemd/system/kube-apiserver.service
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
since Sat 2019-03-23 15:33:17 CST
```
```python
;
```
```python
287ms ago
     Docs: https://github.com/kubernetes/kubernetes
 Main PID: 10007
```
```python
(
```
```python
kube-apiserver
```
```python
)
```
```python
CGroup: /system.slice/kube-apiserver.service
           └─10007 /usr/local/bin/kube-apiserver --logtostderr
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
/var/log/kubernetes --etcd-servers
```
```python
=
```
```python
https://192.168.163.131
```
```python
..
```
```python
.
Mar 23 15:33:18 host131 kube-apiserver
```
```python
[
```
```python
10007
```
```python
]
```
```python
: I0323 15:33:18.173826   10007 flags.go:33
```
```python
]
```
```python
FLAG: --authentication-token-webhook-config-file
```
```python
=
```
```python
""
```
```python
Mar 23 15:33:18 host131 kube-apiserver
```
```python
[
```
```python
10007
```
```python
]
```
```python
: I0323 15:33:18.173830   10007 flags.go:33
```
```python
]
```
```python
FLAG: --authorization-mode
```
```python
=
```
```python
"[RBAC,Node]"
```
```python
Mar 23 15:33:18 host131 kube-apiserver
```
```python
[
```
```python
10007
```
```python
]
```
```python
: I0323 15:33:18.173836   10007 flags.go:33
```
```python
]
```
```python
FLAG: --authorization-policy-file
```
```python
=
```
```python
""
```
```python
Mar 23 15:33:18 host131 kube-apiserver
```
```python
[
```
```python
10007
```
```python
]
```
```python
: I0323 15:33:18.173841   10007 flags.go:33
```
```python
]
```
```python
FLAG: --authorization-webhook-cache-authorize
```
```python
..
```
```python
.
```
```python
=
```
```python
"5m0s"
```
```python
Mar 23 15:33:18 host131 kube-apiserver
```
```python
[
```
```python
10007
```
```python
]
```
```python
: I0323 15:33:18.173845   10007 flags.go:33
```
```python
]
```
```python
FLAG: --authorization-webhook-cache-unauthori
```
```python
..
```
```python
.l
```
```python
=
```
```python
"30s"
```
```python
Mar 23 15:33:18 host131 kube-apiserver
```
```python
[
```
```python
10007
```
```python
]
```
```python
: I0323 15:33:18.173849   10007 flags.go:33
```
```python
]
```
```python
FLAG: --authorization-webhook-config-file
```
```python
=
```
```python
""
```
```python
Mar 23 15:33:18 host131 kube-apiserver
```
```python
[
```
```python
10007
```
```python
]
```
```python
: I0323 15:33:18.173853   10007 flags.go:33
```
```python
]
```
```python
FLAG: --basic-auth-file
```
```python
=
```
```python
""
```
```python
Mar 23 15:33:18 host131 kube-apiserver
```
```python
[
```
```python
10007
```
```python
]
```
```python
: I0323 15:33:18.173857   10007 flags.go:33
```
```python
]
```
```python
FLAG: --bind-address
```
```python
=
```
```python
"192.168.163.131"
```
```python
Mar 23 15:33:18 host131 kube-apiserver
```
```python
[
```
```python
10007
```
```python
]
```
```python
: I0323 15:33:18.173862   10007 flags.go:33
```
```python
]
```
```python
FLAG: --cert-dir
```
```python
=
```
```python
"/var/run/kubernetes"
```
```python
Mar 23 15:33:18 host131 kube-apiserver
```
```python
[
```
```python
10007
```
```python
]
```
```python
: I0323 15:33:18.173867   10007 flags.go:33
```
```python
]
```
```python
FLAG: --client-ca-file
```
```python
=
```
```python
"/etc/ssl/ca/ca.pem"
```
```python
Hint: Some lines were ellipsized, use -l to show
```
```python
in
```
```python
full.
```
```python
##  kubectl version
```
```python
Client Version: version.Info
```
```python
{
```
```python
Major:
```
```python
"1"
```
```python
, Minor:
```
```python
"13"
```
```python
, GitVersion:
```
```python
"v1.13.4"
```
```python
, GitCommit:
```
```python
"c27b913fddd1a6c480c229191a087698aa92f0b1"
```
```python
, GitTreeState:
```
```python
"clean"
```
```python
, BuildDate:
```
```python
"2019-02-28T13:37:52Z"
```
```python
, GoVersion:
```
```python
"go1.11.5"
```
```python
, Compiler:
```
```python
"gc"
```
```python
, Platform:
```
```python
"linux/amd64"
```
```python
}
```
```python
Server Version: version.Info
```
```python
{
```
```python
Major:
```
```python
"1"
```
```python
, Minor:
```
```python
"13"
```
```python
, GitVersion:
```
```python
"v1.13.4"
```
```python
, GitCommit:
```
```python
"c27b913fddd1a6c480c229191a087698aa92f0b1"
```
```python
, GitTreeState:
```
```python
"clean"
```
```python
, BuildDate:
```
```python
"2019-02-28T13:30:26Z"
```
```python
, GoVersion:
```
```python
"go1.11.5"
```
```python
, Compiler:
```
```python
"gc"
```
```python
, Platform:
```
```python
"linux/amd64"
```
```python
}
```
```python
##  get cs
```
```python
NAME                 STATUS      MESSAGE                                                                                     ERROR
scheduler            Unhealthy   Get http://127.0.0.1:10251/healthz: dial tcp 127.0.0.1:10251: connect: connection refused   
controller-manager   Unhealthy   Get http://127.0.0.1:10252/healthz: dial tcp 127.0.0.1:10252: connect: connection refused   
etcd-0               Healthy
```
```python
{
```
```python
"health"
```
```python
:
```
```python
"true"
```
```python
}
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
可以看到，在ETCD的基础之上，apiserver正常启动，而通过kubectl get cs也能确认到etcd服务的正常。另外scheduler和controller-manager安装部署之后也会正常显示。

