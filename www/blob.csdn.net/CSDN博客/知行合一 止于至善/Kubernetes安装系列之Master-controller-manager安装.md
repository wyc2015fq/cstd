
# Kubernetes安装系列之Master-controller-manager安装 - 知行合一 止于至善 - CSDN博客

2019年03月24日 09:59:00[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：280所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)



这篇文章整理以下Master节点的controller-manager的安装与设定方法，本文以脚本的方式进行固化，内容仍然放在github的easypack上。
# 整体操作
[https://blog.csdn.net/liumiaocn/article/details/88413428](https://blog.csdn.net/liumiaocn/article/details/88413428)
# controller-manager的设定文件
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
# cat /etc/k8s/controller-manager.conf
```
```python
KUBE_CONTROLLER_MANAGER_OPTS
```
```python
=
```
```python
"--logtostderr=true \
--v=4 \
--log-dir=/var/log/kubernetes \
--master=127.0.0.1:8080 \
--leader-elect=true \
--address=127.0.0.1 \
--service-cluster-ip-range=172.200.0.0/16 \
--cluster-name=kubernetes \
--cluster-signing-cert-file=/etc/ssl/ca/ca.pem \
--cluster-signing-key-file=/etc/ssl/ca/ca-key.pem  \
--root-ca-file=/etc/ssl/ca/ca.pem \
--service-account-private-key-file=/etc/ssl/ca/ca-key.pem"
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
# cat /usr/lib/systemd/system/kube-controller-manager.service
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
Kubernetes Controller Manager
Documentation
```
```python
=
```
```python
https://github.com/kubernetes/kubernetes
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
-/etc/k8s/controller-manager.conf
ExecStart
```
```python
=
```
```python
/usr/local/bin/kube-controller-manager
```
```python
$KUBE_CONTROLLER_MANAGER_OPTS
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
# cat step5-install-controller-manager.sh
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
"\n##  kube-controller-manager service"
```
```python
systemctl stop kube-controller-manager 2
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
/kube-controller-manager
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
"please check kube-controller-manager binary files existed in
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
# create kube-controller-manager configuration file
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
${ENV_KUBE_CM_ETC}
```
```python
<<
```
```python
EOF
KUBE_CONTROLLER_MANAGER_OPTS="--logtostderr=
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
\
--master=
```
```python
${ENV_KUBE_MASTER_ADDR}
```
```python
\
--leader-elect=
```
```python
${ENV_KUBE_OPT_LEADER_ELECT}
```
```python
\
--address=
```
```python
${ENV_KUBE_OPT_LOCALHOST}
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
--cluster-name=
```
```python
${ENV_KUBE_OPT_CLUSTER_NAME}
```
```python
\
--cluster-signing-cert-file=
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
--cluster-signing-key-file=
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
--root-ca-file=
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
--service-account-private-key-file=
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
"
EOF
```
```python
# Create the kube-controller-manager service.
```
```python
cat
```
```python
>
```
```python
${ENV_KUBE_CM_SERVICE}
```
```python
<<
```
```python
EOF
[Unit]
Description=Kubernetes Controller Manager
Documentation=https://github.com/kubernetes/kubernetes
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
${ENV_KUBE_CM_ETC}
```
```python
ExecStart=
```
```python
${ENV_KUBE_DIR_BIN}
```
```python
/kube-controller-manager \
```
```python
$KUBE_CONTROLLER_MANAGER_OPTS
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
"\n##  start kube-controller-manager service "
```
```python
systemctl start kube-controller-manager
```
```python
echo
```
```python
-e
```
```python
"\n##  enable kube-controller-manager service "
```
```python
systemctl
```
```python
enable
```
```python
kube-controller-manager
```
```python
echo
```
```python
-e
```
```python
"\n##  check  kube-controller-manager status"
```
```python
systemctl status kube-controller-manager
```
```python
# sleep 2 seconds for checking
```
```python
sleep
```
```python
2
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
# sh step5-install-controller-manager.sh
```
```python
##  kube-controller-manager service
```
```python
##  daemon reload service
```
```python
##  start kube-controller-manager service
```
```python
##  enable kube-controller-manager service
```
```python
Created symlink from /etc/systemd/system/multi-user.target.wants/kube-controller-manager.service to /usr/lib/systemd/system/kube-controller-manager.service.
```
```python
##  check  kube-controller-manager status
```
```python
● kube-controller-manager.service - Kubernetes Controller Manager
   Loaded: loaded
```
```python
(
```
```python
/usr/lib/systemd/system/kube-controller-manager.service
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
since Sat 2019-03-23 17:09:32 CST
```
```python
;
```
```python
298ms ago
     Docs: https://github.com/kubernetes/kubernetes
 Main PID: 10838
```
```python
(
```
```python
kube-controller
```
```python
)
```
```python
CGroup: /system.slice/kube-controller-manager.service
           └─10838 /usr/local/bin/kube-controller-manager --logtostderr
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
/var/log/kubernetes --master
```
```python
=
```
```python
127.0.0.1:8080 --lea
```
```python
..
```
```python
.
Mar 23 17:09:32 host131 kube-controller-manager
```
```python
[
```
```python
10838
```
```python
]
```
```python
: I0323 17:09:32.263505   10838 flags.go:33
```
```python
]
```
```python
FLAG: --horizontal-pod-autoscaler-ini
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
"30s"
```
```python
Mar 23 17:09:32 host131 kube-controller-manager
```
```python
[
```
```python
10838
```
```python
]
```
```python
: I0323 17:09:32.263512   10838 flags.go:33
```
```python
]
```
```python
FLAG: --horizontal-pod-autoscaler-syn
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
"15s"
```
```python
Mar 23 17:09:32 host131 kube-controller-manager
```
```python
[
```
```python
10838
```
```python
]
```
```python
: I0323 17:09:32.263518   10838 flags.go:33
```
```python
]
```
```python
FLAG: --horizontal-pod-autoscaler-tol
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
"0.1"
```
```python
Mar 23 17:09:32 host131 kube-controller-manager
```
```python
[
```
```python
10838
```
```python
]
```
```python
: I0323 17:09:32.263528   10838 flags.go:33
```
```python
]
```
```python
FLAG: --horizontal-pod-autoscaler-ups
```
```python
..
```
```python
.
```
```python
"3m0s"
```
```python
Mar 23 17:09:32 host131 kube-controller-manager
```
```python
[
```
```python
10838
```
```python
]
```
```python
: I0323 17:09:32.263535   10838 flags.go:33
```
```python
]
```
```python
FLAG: --horizontal-pod-autoscaler-use
```
```python
..
```
```python
.
```
```python
"true"
```
```python
Mar 23 17:09:32 host131 kube-controller-manager
```
```python
[
```
```python
10838
```
```python
]
```
```python
: I0323 17:09:32.263541   10838 flags.go:33
```
```python
]
```
```python
FLAG: --http2-max-streams-per-connection
```
```python
=
```
```python
"0"
```
```python
Mar 23 17:09:32 host131 kube-controller-manager
```
```python
[
```
```python
10838
```
```python
]
```
```python
: I0323 17:09:32.263549   10838 flags.go:33
```
```python
]
```
```python
FLAG: --kube-api-burst
```
```python
=
```
```python
"30"
```
```python
Mar 23 17:09:32 host131 kube-controller-manager
```
```python
[
```
```python
10838
```
```python
]
```
```python
: I0323 17:09:32.263556   10838 flags.go:33
```
```python
]
```
```python
FLAG: --kube-api-content-type
```
```python
=
```
```python
"applic...tobuf"
```
```python
Mar 23 17:09:32 host131 kube-controller-manager
```
```python
[
```
```python
10838
```
```python
]
```
```python
: I0323 17:09:32.263563   10838 flags.go:33
```
```python
]
```
```python
FLAG: --kube-api-qps
```
```python
=
```
```python
"20"
```
```python
Mar 23 17:09:32 host131 kube-controller-manager
```
```python
[
```
```python
10838
```
```python
]
```
```python
: I0323 17:09:32.263570   10838 flags.go:33
```
```python
]
```
```python
FLAG: --kubeconfig
```
```python
=
```
```python
""
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
##  get cs
```
```python
NAME                 STATUS    MESSAGE             ERROR
scheduler            Healthy   ok                  
controller-manager   Healthy   ok                  
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
安装之后可以通过kubectl get cs确认到controller-manager的Status也变成了Healthy状态，这样Master节点所需的主要部分就完成了。

