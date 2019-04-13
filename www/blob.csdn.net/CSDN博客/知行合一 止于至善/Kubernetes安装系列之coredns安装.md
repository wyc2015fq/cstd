
# Kubernetes安装系列之coredns安装 - 知行合一 止于至善 - CSDN博客

2019年04月02日 19:45:14[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：123所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)



这篇文章整理一下coredns插件的安装与设定方法，本文以脚本的方式进行固化，内容仍然放在github的easypack上。
# 整体操作
[https://blog.csdn.net/liumiaocn/article/details/88413428](https://blog.csdn.net/liumiaocn/article/details/88413428)
# 事前准备
使用快速安装和部署的一键脚本，快速搭建kubernetes环境，使用命令为
> sh
> [all-k8s-mgnt.sh](http://all-k8s-mgnt.sh)
> install all
注意：目前版本中已经将coredns直接集成到一键安装之中，上述命令最后会自动安装coredns插件，这里为了说明，将重新安装一遍。
# 安装coredns
使用如下命令即可安装coredns插件，这里使用k8s所提供的yaml文件，只需要注意如下两点，一般即可完成coredns的安装与设定：
镜像提前docker load，或者保证网络能够docker pull此镜像成功
替换DNS Domain和DNS SERVER IP信息
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
# sh all-k8s-mgnt.sh install coredns
```
```python
## Tue Apr  2 19:27:53 CST 2019 ACTION: install  Service: coredns begins ...
```
```python
## please make sure you can get the following images
```
```python
image: k8s.gcr.io/coredns:1.2.6
```
```python
## the following keyword needs to be replaced
```
```python
## replace __PILLAR__DNS__DOMAIN__
```
```python
## replace __PILLAR__DNS__SERVER__
```
```python
## delete service first
```
```python
/root/easypack/k8s/shell/etc/plugins/coredns
```
```python
## create service
```
```python
serviceaccount/coredns created
clusterrole.rbac.authorization.k8s.io/system:coredns created
clusterrolebinding.rbac.authorization.k8s.io/system:coredns created
configmap/coredns created
deployment.extensions/coredns created
service/kube-dns created
```
```python
## begin check coredns, wait for 3s ...
```
```python
NAME                          READY   STATUS        RESTARTS   AGE
pod/coredns-fff89c9b9-mm586   1/1     Running       0          3s
pod/coredns-fff89c9b9-t687l   0/1     Terminating   0          4m
NAME               TYPE        CLUSTER-IP   EXTERNAL-IP   PORT
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
service/kube-dns   ClusterIP   10.254.0.2
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
53/UDP,53/TCP   3s
NAME                      READY   UP-TO-DATE   AVAILABLE   AGE
deployment.apps/coredns   1/1     1            1           3s
NAME                                DESIRED   CURRENT   READY   AGE
replicaset.apps/coredns-fff89c9b9   1         1         1       3s
```
```python
## Tue Apr  2 19:27:57 CST 2019 ACTION: install  Service: coredns ends  ...
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
从上述信息中可以看到coredns是重新设定了一下，由于sleep的时间3秒之内，之前的pod的terminating还没有完成，所以上述的kubectl get all -n kube-system的信息中关于coredns的pod信息有两个。再次执行就没有问题了
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
# kubectl get all -n kube-system |grep dns
```
```python
pod/coredns-fff89c9b9-mm586   1/1     Running   0          6m33s
service/kube-dns   ClusterIP   10.254.0.2
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
53/UDP,53/TCP   6m33s
deployment.apps/coredns   1/1     1            1           6m33s
replicaset.apps/coredns-fff89c9b9   1         1         1       6m33s
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
# 简单验证coredns
这里简单使用nginx镜像进行确认dns的解析过程，这里只是确认一下/etc/resolv.conf的内容。总所周知，resolv.conf文件用于加载dns服务器的信息，这里我们来确认一下在coredns启动后，nginx所启动的pod中，是否有相关的dns服务器的信息。
## Step 1: 准备Nginx镜像
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
# docker images |grep nginx
```
```python
nginx                                              latest              2bcb04bdb83f        6 days ago          109 MB
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
# kubectl get pod
```
```python
No resources found.
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
## Step 2: 启动一个pod
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
# kubectl run --generator=run-pod/v1 nginx-test --image=nginx
```
```python
pod/nginx-test created
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
# kubectl get pod
```
```python
NAME         READY   STATUS    RESTARTS   AGE
nginx-test   1/1     Running   0          7s
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
## Step 3 :进入pod中确认resolv.conf
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
# kubectl exec -it nginx-test sh
```
```python
# cat /etc/resolv.conf
```
```python
nameserver 10.254.0.2
search default.svc.cluster.local svc.cluster.local cluster.local
options ndots:5
```
```python
#
```
可以看到：nameserver的设定为10.254.0.2，这正是dns的ClusterIP地址
```python
service/kube-dns   ClusterIP   10.254.0.2
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
53/UDP,53/TCP   6m33s
```
而搜索的路径也是在前面的安装中所设定的内容。
当然仅凭这一点还无法验证dns是正常动作的，接下来在heapster的设定中则能完全确认这一点，在后面的文章中将会进一步展开说明。

