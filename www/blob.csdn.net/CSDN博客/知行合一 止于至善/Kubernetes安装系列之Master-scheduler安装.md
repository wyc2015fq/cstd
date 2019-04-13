
# Kubernetes安装系列之Master-scheduler安装 - 知行合一 止于至善 - CSDN博客

2019年03月24日 09:22:53[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：237所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)



这篇文章整理以下Master节点的scheduler的安装与设定方法，本文以脚本的方式进行固化，内容仍然放在github的easypack上。
# 整体操作
[https://blog.csdn.net/liumiaocn/article/details/88413428](https://blog.csdn.net/liumiaocn/article/details/88413428)
# scheduler的设定文件
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
# cat /etc/k8s/kube-scheduler.conf
```
```python
KUBE_SCHEDULER_OPTS
```
```python
=
```
```python
"--logtostderr=true \
--v=4 \
--log-dir=/var/log/kubernetes \
--master=127.0.0.1:8080 \
--leader-elect"
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
# cat /usr/lib/systemd/system/kube-scheduler.service
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
Kubernetes Scheduler
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
-/etc/k8s/kube-scheduler.conf
ExecStart
```
```python
=
```
```python
/usr/local/bin/kube-scheduler
```
```python
$KUBE_SCHEDULER_OPTS
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
# cat step4-install-scheduler.sh
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
"\n##  kube-scheduler service"
```
```python
systemctl stop kube-scheduler 2
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
/kube-scheduler
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
"please check kube-scheduler binary files existed in
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
# create kube-scheduler configuration file
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
${ENV_KUBE_SCH_ETC}
```
```python
<<
```
```python
EOF
KUBE_SCHEDULER_OPTS="--logtostderr=true \
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
--leader-elect"
EOF
```
```python
# Create the kube-scheduler service.
```
```python
cat
```
```python
>
```
```python
${ENV_KUBE_SCH_SERVICE}
```
```python
<<
```
```python
EOF
[Unit]
Description=Kubernetes Scheduler
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
${ENV_KUBE_SCH_ETC}
```
```python
ExecStart=
```
```python
${ENV_KUBE_DIR_BIN}
```
```python
/kube-scheduler \
```
```python
$KUBE_SCHEDULER_OPTS
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
"\n##  start kube-scheduler service "
```
```python
systemctl start kube-scheduler
```
```python
echo
```
```python
-e
```
```python
"\n##  enable kube-scheduler service "
```
```python
systemctl
```
```python
enable
```
```python
kube-scheduler
```
```python
echo
```
```python
-e
```
```python
"\n##  check  kube-scheduler status"
```
```python
systemctl status kube-scheduler
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
# sh step4-install-scheduler.sh
```
```python
##  kube-scheduler service
```
```python
##  daemon reload service
```
```python
##  start kube-scheduler service
```
```python
##  enable kube-scheduler service
```
```python
##  check  kube-scheduler status
```
```python
● kube-scheduler.service - Kubernetes Scheduler
   Loaded: loaded
```
```python
(
```
```python
/usr/lib/systemd/system/kube-scheduler.service
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
since Sat 2019-03-23 16:35:18 CST
```
```python
;
```
```python
277ms ago
     Docs: https://github.com/kubernetes/kubernetes
 Main PID: 10732
```
```python
(
```
```python
kube-scheduler
```
```python
)
```
```python
CGroup: /system.slice/kube-scheduler.service
           └─10732 /usr/local/bin/kube-scheduler --logtostderr
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
127.0.0.1:8080 --leader-elect
```
```python
..
```
```python
.
Mar 23 16:35:19 host131 kube-scheduler
```
```python
[
```
```python
10732
```
```python
]
```
```python
: I0323 16:35:19.029064   10732 flags.go:33
```
```python
]
```
```python
FLAG: --tls-cert-file
```
```python
=
```
```python
""
```
```python
Mar 23 16:35:19 host131 kube-scheduler
```
```python
[
```
```python
10732
```
```python
]
```
```python
: I0323 16:35:19.029070   10732 flags.go:33
```
```python
]
```
```python
FLAG: --tls-cipher-suites
```
```python
=
```
```python
"[]"
```
```python
Mar 23 16:35:19 host131 kube-scheduler
```
```python
[
```
```python
10732
```
```python
]
```
```python
: I0323 16:35:19.029074   10732 flags.go:33
```
```python
]
```
```python
FLAG: --tls-min-version
```
```python
=
```
```python
""
```
```python
Mar 23 16:35:19 host131 kube-scheduler
```
```python
[
```
```python
10732
```
```python
]
```
```python
: I0323 16:35:19.029079   10732 flags.go:33
```
```python
]
```
```python
FLAG: --tls-private-key-file
```
```python
=
```
```python
""
```
```python
Mar 23 16:35:19 host131 kube-scheduler
```
```python
[
```
```python
10732
```
```python
]
```
```python
: I0323 16:35:19.029083   10732 flags.go:33
```
```python
]
```
```python
FLAG: --tls-sni-cert-key
```
```python
=
```
```python
"[]"
```
```python
Mar 23 16:35:19 host131 kube-scheduler
```
```python
[
```
```python
10732
```
```python
]
```
```python
: I0323 16:35:19.029091   10732 flags.go:33
```
```python
]
```
```python
FLAG: --use-legacy-policy-config
```
```python
=
```
```python
"false"
```
```python
Mar 23 16:35:19 host131 kube-scheduler
```
```python
[
```
```python
10732
```
```python
]
```
```python
: I0323 16:35:19.029095   10732 flags.go:33
```
```python
]
```
```python
FLAG: --v
```
```python
=
```
```python
"4"
```
```python
Mar 23 16:35:19 host131 kube-scheduler
```
```python
[
```
```python
10732
```
```python
]
```
```python
: I0323 16:35:19.029100   10732 flags.go:33
```
```python
]
```
```python
FLAG: --version
```
```python
=
```
```python
"false"
```
```python
Mar 23 16:35:19 host131 kube-scheduler
```
```python
[
```
```python
10732
```
```python
]
```
```python
: I0323 16:35:19.029107   10732 flags.go:33
```
```python
]
```
```python
FLAG: --vmodule
```
```python
=
```
```python
""
```
```python
Mar 23 16:35:19 host131 kube-scheduler
```
```python
[
```
```python
10732
```
```python
]
```
```python
: I0323 16:35:19.029112   10732 flags.go:33
```
```python
]
```
```python
FLAG: --write-config-to
```
```python
=
```
```python
""
```
```python
##  get cs
```
```python
NAME                 STATUS      MESSAGE                                                                                     ERROR
controller-manager   Unhealthy   Get http://127.0.0.1:10252/healthz: dial tcp 127.0.0.1:10252: connect: connection refused   
scheduler            Healthy     ok                                                                                          
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
安装之后可以通过kubectl get cs确认到scheduler的Status变成了Healthy状态，接下来只需要设定一下controller-manager，Master节点所需的主要部分就完成了。

