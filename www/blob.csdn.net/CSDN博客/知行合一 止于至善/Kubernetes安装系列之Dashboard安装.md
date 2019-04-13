
# Kubernetes安装系列之Dashboard安装 - 知行合一 止于至善 - CSDN博客

2019年04月01日 21:38:18[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：89


这篇文章整理一下dashboard plugin的安装与设定方法，本文以脚本的方式进行固化，内容仍然放在github的easypack上。
# 整体操作
[https://blog.csdn.net/liumiaocn/article/details/88413428](https://blog.csdn.net/liumiaocn/article/details/88413428)
# 事前准备
使用快速安装和部署的一键脚本，快速搭建kubernetes环境，使用命令为
> sh
> [all-k8s-mgnt.sh](http://all-k8s-mgnt.sh)
> install all
执行日志信息摘要
```python
## kubectl version
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
## kubectl cluster-info
```
```python
Kubernetes master is running at https://192.168.163.131:6443
To further debug and diagnose cluster problems, use
```
```python
'kubectl cluster-info dump'
```
```python
.
```
```python
## kubectl get node
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
4s    v1.13.4   192.168.163.131
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
## kubectl get cs
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
# dashboard设定脚本
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
# cat step9-2-install-plugin-dashboard.sh
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
.
```
```python
./common-util.sh
```
```python
echo
```
```python
"## please make sure you can get the following images"
```
```python
grep
```
```python
image etc/plugins/dashboard/kubernetes-dashboard.yaml
```
```python
echo
```
```python
"## getting the above image ready in advance is preferred, please press any key when ready"
```
```python
read
```
```python
echo
```
```python
"## create dashboard service"
```
```python
kubectl delete -f etc/plugins/dashboard/kubernetes-dashboard.yaml
```
```python
>
```
```python
/dev/null 2
```
```python
>
```
```python
&
```
```python
1
kubectl create -f etc/plugins/dashboard/kubernetes-dashboard.yaml
```
```python
# create and display dashboard token
```
```python
create_dashboard_token
```
```python
sleep
```
```python
2
display_dashboard_token
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
"## display pods information"
```
```python
sleep
```
```python
2 
kubectl get pods -n kube-system
```
```python
echo
```
```python
"## display service information"
```
```python
kubectl get
```
```python
service
```
```python
-n kube-system
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
# 执行日志
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
# sh all-k8s-mgnt.sh install dashboard
```
```python
## Sat Mar 30 03:53:06 CST 2019 ACTION: install  Service: dashboard begins ...
```
```python
## please make sure you can get the following images
```
```python
image: k8s.gcr.io/kubernetes-dashboard-amd64:v1.10.1
```
```python
## getting the above image ready in advance is preferred, please press any key when ready
```
```python
## create dashboard service
```
```python
secret/kubernetes-dashboard-certs created
serviceaccount/kubernetes-dashboard created
role.rbac.authorization.k8s.io/kubernetes-dashboard-minimal created
rolebinding.rbac.authorization.k8s.io/kubernetes-dashboard-minimal created
deployment.apps/kubernetes-dashboard created
service/kubernetes-dashboard created
```
```python
## create service account for dashboard
```
```python
serviceaccount/dashboard-admin created
```
```python
## create clusterrolebinding for dashboard
```
```python
clusterrolebinding.rbac.authorization.k8s.io/dashboard-admin created
```
```python
## dashboard_secrete: dashboard-admin-token-44hd5
```
```python
## dashboard_token:
```
```python
eyJhbGciOiJSUzI1NiIsImtpZCI6IiJ9.eyJpc3MiOiJrdWJlcm5ldGVzL3NlcnZpY2VhY2NvdW50Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9uYW1lc3BhY2UiOiJrdWJlLXN5c3RlbSIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VjcmV0Lm5hbWUiOiJkYXNoYm9hcmQtYWRtaW4tdG9rZW4tNDRoZDUiLCJrdWJlcm5ldGVzLmlvL3NlcnZpY2VhY2NvdW50L3NlcnZpY2UtYWNjb3VudC5uYW1lIjoiZGFzaGJvYXJkLWFkbWluIiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9zZXJ2aWNlLWFjY291bnQudWlkIjoiNTY5MGI2OWMtNTI1Yy0xMWU5LTkyYzQtMDgwMDI3ZjhiZTZmIiwic3ViIjoic3lzdGVtOnNlcnZpY2VhY2NvdW50Omt1YmUtc3lzdGVtOmRhc2hib2FyZC1hZG1pbiJ9.r7FfVZmCU9y3IpLV1VlKXeHGh7whI8J4gqD9r3lq4_AMKv1SSfvbryAJ3aa5bGfq5fidDlH4E2zk1tRk0YyhYjG9nQ-x_fPD7ZUz44PKQ-9an_aUjX5G-kiSz4AbIy-Vd0Ctfrq6kuAh7Nasv0cfwybuzZa8fTNQkC8RVIarcZ57AveKrLdNeCdhUYkHYYLBt11vtk6MiyFAeFy9H7v5IB9Rh82u2es9hDZCl3_cabEyIb4IH82OIBsnhE0EnowV6kEbSDkFSkYYAavTDkwvAt3HZ50Z3efo8CosO5x7sU8MTxhRfew2ZOws0Urx9EoE7GlC3kugI0QNJS_TEjCj6A
```
```python
## display pods information
```
```python
NAME                                   READY   STATUS    RESTARTS   AGE
kubernetes-dashboard-57df4db6b-hn4jr   1/1     Running   0          8s
```
```python
## display service information
```
```python
NAME                   TYPE       CLUSTER-IP       EXTERNAL-IP   PORT
```
```python
(
```
```python
S
```
```python
)
```
```python
AGE
kubernetes-dashboard   NodePort   10.254.183.151
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
443:33307/TCP   8s
```
```python
## Sat Mar 30 03:53:42 CST 2019 ACTION: install  Service: dashboard ends  ...
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
注意：在dashboard的yaml文件执行之前，需本地先行将相关的镜像load进来
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
# docker load -i ~/dashboard.tar
```
```python
fbdfe08b001c: Loading layer
```
```python
[
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
==
```
```python
>
```
```python
]
```
```python
122.3 MB/122.3 MB
Loaded image: k8s.gcr.io/kubernetes-dashboard-amd64:v1.10.1
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
# 使用生成dashboard的service端口进行访问
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190401213414901.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
输入上文生成的token，便可成功登录dashboard
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040121360028.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
由于目前还没有安装heapster等插件，所以资源信息还无法查看，但整体已经可用。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190401213923791.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)

[
](https://img-blog.csdnimg.cn/2019040121360028.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
