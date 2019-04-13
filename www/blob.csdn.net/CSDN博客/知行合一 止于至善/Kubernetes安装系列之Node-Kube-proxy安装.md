
# Kubernetes安装系列之Node-Kube-proxy安装 - 知行合一 止于至善 - CSDN博客

2019年03月28日 21:56:14[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：175标签：[kubernetes																](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)[kube-proxy																](https://so.csdn.net/so/search/s.do?q=kube-proxy&t=blog)[安装																](https://so.csdn.net/so/search/s.do?q=安装&t=blog)[部署																](https://so.csdn.net/so/search/s.do?q=部署&t=blog)[easypack																](https://so.csdn.net/so/search/s.do?q=easypack&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=部署&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=安装&t=blog)个人分类：[Kubernetes																](https://blog.csdn.net/liumiaocn/article/category/6328275)
[
																					](https://so.csdn.net/so/search/s.do?q=安装&t=blog)所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)[
							](https://so.csdn.net/so/search/s.do?q=安装&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=kube-proxy&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)

这篇文章整理以下Node节点的kube-proxy的安装与设定方法，本文以脚本的方式进行固化，内容仍然放在github的easypack上。
# 整体操作
[https://blog.csdn.net/liumiaocn/article/details/88413428](https://blog.csdn.net/liumiaocn/article/details/88413428)
# kube-proxy的设定文件
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
# cat /etc/k8s/kube-proxy.conf
```
```python
KUBE_PROXY_OPTS
```
```python
=
```
```python
"--logtostderr=true \
--v=4 \
--log-dir=/var/log/kubernetes \
--config=/etc/k8s/kube-proxy-config.yaml"
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
# cat /etc/k8s/kube-proxy-config.yaml
```
```python
kind: KubeProxyConfiguration
apiVersion: kubeproxy.config.k8s.io/v1alpha1
clientConnection:
  kubeconfig:
```
```python
"/etc/ssl/k8s/kube-proxy.kubeconfig"
```
```python
bindAddress: 192.168.163.131
clusterCIDR: 172.200.0.0/16
healthzBindAddress: 192.168.163.131:10256
hostnameOverride: 192.168.163.131
metricsBindAddress: 192.168.163.131:10249
mode:
```
```python
"ipvs"
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
# cat /usr/lib/systemd/system/kube-proxy.service
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
Kubernetes Kube-Proxy Service
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
After
```
```python
=
```
```python
network.target
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
/var/lib/k8s/kube-proxy
EnvironmentFile
```
```python
=
```
```python
-/etc/k8s/kube-proxy.conf
ExecStart
```
```python
=
```
```python
/usr/local/bin/kube-proxy
```
```python
$KUBE_PROXY_OPTS
```
```python
Restart
```
```python
=
```
```python
on-failure
RestartSec
```
```python
=
```
```python
5
LimitNOFILE
```
```python
=
```
```python
65536
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
# cat step8-3-install-kubeproxy.sh
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
"\n##  kube-proxy service"
```
```python
systemctl stop kube-proxy 2
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
${ENV_KUBE_PROXY_DIR_WORKING}
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
/kube-proxy
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
"please check kube-proxy binary files existed in
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
# create kube-proxy configuration file
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
${ENV_KUBE_PROXY_ETC}
```
```python
<<
```
```python
EOF
KUBE_PROXY_OPTS="--logtostderr=
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
--config=
```
```python
${ENV_KUBE_DIR_ETC}
```
```python
/
```
```python
${ENV_KUBE_PROXY_PROXY_CONFIG}
```
```python
"
EOF
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
${ENV_KUBE_PROXY_PROXY_CONFIG}
```
```python
<<
```
```python
EOF
kind: KubeProxyConfiguration
apiVersion: kubeproxy.config.k8s.io/v1alpha1
clientConnection:
  kubeconfig: "
```
```python
${ENV_SSL_K8S_DIR}
```
```python
/
```
```python
${ENV_KUBECONFIG_KUBEPROXY}
```
```python
"
bindAddress:
```
```python
${ENV_KUBE_NODE_HOSTNAME}
```
```python
clusterCIDR:
```
```python
${ENV_KUBE_OPT_CLUSTER_IP_RANGE}
```
```python
healthzBindAddress:
```
```python
${ENV_KUBE_NODE_HOSTNAME}
```
```python
:
```
```python
${ENV_KUBE_PROXY_CONFIG_PORT_HEALTH}
```
```python
hostnameOverride:
```
```python
${ENV_KUBE_NODE_HOSTNAME}
```
```python
metricsBindAddress:
```
```python
${ENV_KUBE_NODE_HOSTNAME}
```
```python
:
```
```python
${ENV_KUBE_PROXY_CONFIG_PORT_METRICS}
```
```python
mode: "
```
```python
${ENV_KUBE_PROXY_CONFIG_MODE}
```
```python
"
EOF
```
```python
# Create the kube-proxy service.
```
```python
cat
```
```python
>
```
```python
${ENV_KUBE_PROXY_SERVICE}
```
```python
<<
```
```python
EOF
[Unit]
Description=Kubernetes Kube-Proxy Service
Documentation=https://github.com/GoogleCloudPlatform/kubernetes
After=docker.service
After=network.target
[Service]
WorkingDirectory=
```
```python
${ENV_KUBE_PROXY_DIR_WORKING}
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
${ENV_KUBE_PROXY_ETC}
```
```python
ExecStart=
```
```python
${ENV_KUBE_DIR_BIN}
```
```python
/kube-proxy \
```
```python
$KUBE_PROXY_OPTS
```
```python
Restart=on-failure
RestartSec=5
LimitNOFILE=65536
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
"\n##  start kube-proxy service "
```
```python
systemctl start kube-proxy
```
```python
echo
```
```python
-e
```
```python
"\n##  enable kube-proxy service "
```
```python
systemctl
```
```python
enable
```
```python
kube-proxy
```
```python
echo
```
```python
-e
```
```python
"\n##  check  kube-proxy status"
```
```python
systemctl status kube-proxy
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
# sh all-k8s-mgnt.sh install kubeproxy
```
```python
## ACTION: install  Service: kubeproxy begins ...
```
```python
##  kube-proxy service
```
```python
##  daemon reload service
```
```python
##  start kube-proxy service
```
```python
##  enable kube-proxy service
```
```python
##  check  kube-proxy status
```
```python
● kube-proxy.service - Kubernetes Kube-Proxy Service
   Loaded: loaded
```
```python
(
```
```python
/usr/lib/systemd/system/kube-proxy.service
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
since Sun 2019-03-24 20:08:23 CST
```
```python
;
```
```python
333ms ago
     Docs: https://github.com/GoogleCloudPlatform/kubernetes
 Main PID: 1456
```
```python
(
```
```python
kube-proxy
```
```python
)
```
```python
CGroup: /system.slice/kube-proxy.service
           ├─1456 /usr/local/bin/kube-proxy --logtostderr
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
/var/log/kubernetes --config
```
```python
=
```
```python
/etc/k8s/kube-proxy-config.yaml
           └─1493 modprobe -va ip_vs
Mar 24 20:08:23 host131 kube-proxy
```
```python
[
```
```python
1456
```
```python
]
```
```python
: I0324 20:08:23.494868    1456 flags.go:33
```
```python
]
```
```python
FLAG: --proxy-port-range
```
```python
=
```
```python
""
```
```python
Mar 24 20:08:23 host131 kube-proxy
```
```python
[
```
```python
1456
```
```python
]
```
```python
: I0324 20:08:23.494874    1456 flags.go:33
```
```python
]
```
```python
FLAG: --resource-container
```
```python
=
```
```python
"/kube-proxy"
```
```python
Mar 24 20:08:23 host131 kube-proxy
```
```python
[
```
```python
1456
```
```python
]
```
```python
: I0324 20:08:23.494878    1456 flags.go:33
```
```python
]
```
```python
FLAG: --skip-headers
```
```python
=
```
```python
"false"
```
```python
Mar 24 20:08:23 host131 kube-proxy
```
```python
[
```
```python
1456
```
```python
]
```
```python
: I0324 20:08:23.494882    1456 flags.go:33
```
```python
]
```
```python
FLAG: --stderrthreshold
```
```python
=
```
```python
"2"
```
```python
Mar 24 20:08:23 host131 kube-proxy
```
```python
[
```
```python
1456
```
```python
]
```
```python
: I0324 20:08:23.494887    1456 flags.go:33
```
```python
]
```
```python
FLAG: --udp-timeout
```
```python
=
```
```python
"250ms"
```
```python
Mar 24 20:08:23 host131 kube-proxy
```
```python
[
```
```python
1456
```
```python
]
```
```python
: I0324 20:08:23.494895    1456 flags.go:33
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
Mar 24 20:08:23 host131 kube-proxy
```
```python
[
```
```python
1456
```
```python
]
```
```python
: I0324 20:08:23.494899    1456 flags.go:33
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
Mar 24 20:08:23 host131 kube-proxy
```
```python
[
```
```python
1456
```
```python
]
```
```python
: I0324 20:08:23.494905    1456 flags.go:33
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
Mar 24 20:08:23 host131 kube-proxy
```
```python
[
```
```python
1456
```
```python
]
```
```python
: I0324 20:08:23.494911    1456 flags.go:33
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
Mar 24 20:08:23 host131 kube-proxy
```
```python
[
```
```python
1456
```
```python
]
```
```python
: I0324 20:08:23.526599    1456 feature_gate.go:206
```
```python
]
```
```python
feature gates:
```
```python
&
```
```python
{
```
```python
map
```
```python
[
```
```python
]
```
```python
}
```
```python
## ACTION: install  Service: kubeproxy ends  ...
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
这样所有的组件基本安装完毕，至于多节点，目前的脚本拷贝过去即可使用，后续会继续简化操作方式。

