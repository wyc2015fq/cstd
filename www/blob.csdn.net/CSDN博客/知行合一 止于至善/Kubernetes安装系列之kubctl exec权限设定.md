
# Kubernetes安装系列之kubctl exec权限设定 - 知行合一 止于至善 - CSDN博客

2019年04月01日 19:51:36[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：73所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)



RBAC对于权限的控制更加方便，同时入门使用时需要了解的内容也进一步增多。这篇文章memo一下kubectl exec设定权限的方法。
# 环境准备
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# docker pull nginx
```
```python
Using default tag: latest
latest: Pulling from library/nginx
27833a3ba0a5: Pull complete 
e83729dd399a: Pull complete 
ebc6a67df66d: Pull complete 
Digest: sha256:c8a861b8a1eeef6d48955a6c6d5dff8e2580f13ff4d0f549e082e7c82a8617a2
Status: Downloaded newer image
```
```python
for
```
```python
nginx:latest
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
使用此下载的nginx镜像创建一个pod
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# kubectl get pods
```
```python
No resources found.
```
```python
[
```
```python
root@host131 ~
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
root@host131 ~
```
```python
]
```
```python
# kubectl get pods
```
```python
NAME         READY   STATUS              RESTARTS   AGE
nginx-test   0/1     ContainerCreating   0          3s
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# kubectl get pods
```
```python
NAME         READY   STATUS    RESTARTS   AGE
nginx-test   1/1     Running   0          5s
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
# 现象
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# kubectl get pods
```
```python
NAME                         READY   STATUS    RESTARTS   AGE
nginx-test-76bc85c85-skl5t   1/1     Running   0          149m
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
启动的上述的pod，当希望使用kubectl exec进入到其中的时候提示了Forbidden信息：
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# kubectl exec -it nginx-test sh
```
```python
error: unable to upgrade connection: Forbidden
```
```python
(
```
```python
user
```
```python
=
```
```python
kubernetes, verb
```
```python
=
```
```python
create, resource
```
```python
=
```
```python
nodes, subresource
```
```python
=
```
```python
proxy
```
```python
)
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
# 原因
使用kubectl exec命令时，由于会转到kubelet，需要对apiserver调用kubelet API的授权。
# 对应方法
使用kubectl create clusterrolebinding进行设定即可，使用示例如下：
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# kubectl create clusterrolebinding kube-apiserver:kubelet-apis --clusterrole=system:kubelet-api-admin --user kubernetes
```
```python
clusterrolebinding.rbac.authorization.k8s.io/kube-apiserver:kubelet-apis created
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
设定完毕之后再次执行则会发现已经可以正常进入pod之中了
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# kubectl exec -it nginx-test sh
```
```python
# hostname
```
```python
nginx-test
```
```python
#
```
# 其他问题
node的csr需要手动approve，提示信息如下所示
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# kubectl exec -it nginx-test sh
```
```python
Error from server: error dialing backend: remote error: tls: internal error
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
对应方法示例如下：
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
NAME                                                   AGE     REQUESTOR                     CONDITION
csr-pjv59                                              9m25s   system:node:192.168.163.131   Pending
node-csr-_n7VaWDb8ivj0Fk5oISj1Vqm5jVrISEFGTUs837vOqM   15m     kubelet-bootstrap             Approved,Issued
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
# kubectl certificate approve csr-pjv59
```
```python
certificatesigningrequest.certificates.k8s.io/csr-pjv59 approved
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

