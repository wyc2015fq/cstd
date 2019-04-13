
# Kubernetes安装系列之heapster安装 - 知行合一 止于至善 - CSDN博客

2019年04月02日 20:33:58[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：123标签：[kubernetes																](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)[heapster																](https://so.csdn.net/so/search/s.do?q=heapster&t=blog)[grafana																](https://so.csdn.net/so/search/s.do?q=grafana&t=blog)[influxdb																](https://so.csdn.net/so/search/s.do?q=influxdb&t=blog)[dashboard																](https://so.csdn.net/so/search/s.do?q=dashboard&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=influxdb&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=grafana&t=blog)个人分类：[Kubernetes																](https://blog.csdn.net/liumiaocn/article/category/6328275)
[
																					](https://so.csdn.net/so/search/s.do?q=grafana&t=blog)所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)[
							](https://so.csdn.net/so/search/s.do?q=grafana&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=heapster&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)

虽然heapster已经即将退休，为了纪念一下，这篇文章整理一下heapster+grafana+Influxdb组合对于kubernetes的node与资源进行监控的插件安装与设定方法，本文以脚本的方式进行固化，内容仍然放在github的easypack上。
# 整体操作
[https://blog.csdn.net/liumiaocn/article/details/88413428](https://blog.csdn.net/liumiaocn/article/details/88413428)
# 事前准备
使用快速安装和部署的一键脚本，快速搭建kubernetes环境，使用命令为
> sh
> [all-k8s-mgnt.sh](http://all-k8s-mgnt.sh)
> install all
为了验证结果，同时需要确认dashboard能够正常动作，相关设定可参看：
[https://liumiaocn.blog.csdn.net/article/details/88957901](https://liumiaocn.blog.csdn.net/article/details/88957901)
在heapster安装之前，可以看到kube-system的namespace相关的dashboard的展示页面如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402195926378.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 设定方式
之所以为了纪念一下，实在是到现在的版本为之heapster的设定和使用确实非常容易入手，可轻松实现heapster采集，influxdb存储，grafana显示的组合功能，同时与dashboard进行结合显示，而所有这些，基本都没有太多的设定作业要做，一般只需注意如下即可：
相关镜像事前docker load或者pull完毕，或者网络条件能够直接在运行时pull下来
事前coredns或者其他的dns插件能够正常动作
设定相关的RBAC信息
设定heapster如下：
```python
--
```
```python
source
```
```python
=
```
```python
kubernetes
```
```python
:
```
```python
https
```
```python
:
```
```python
/
```
```python
/
```
```python
kubernetes
```
```python
.
```
```python
default
```
```python
?
```
```python
kubeletHttps
```
```python
=
```
```python
true
```
```python
&
```
```python
kubeletPort
```
```python
=
```
```python
10250
```
从这里可以看出，kubernetes.default的转化是需要dns服务器的，如果dns插件如果不设定的话，至少此处需要改成ip方式才能访问，另外其他还有几处需要设定。如果这里能够正常动作，从侧面也能说明前面创建的coredns是能够正常动作的。
# 安装设定
以heapster官方的设定文件为基础，将上述几点注意事项反映进去，形成相关的yaml文件，然后使用kubectl create，即可完成heapster的安装与设定，在easypack中可以这样的语句进行安装即可：
> sh
> [all-k8s-mgnt.sh](http://all-k8s-mgnt.sh)
> install heapster
安装日志示例
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
# sh all-k8s-mgnt.sh install heapster
```
```python
## Tue Apr  2 19:59:45 CST 2019 ACTION: install  Service: heapster begins ...
```
```python
## please make sure you can get the following images
```
```python
image: gcr.io/google_containers/heapster-amd64:v1.5.3
```
```python
## please make sure you can get the following images
```
```python
image: gcr.io/google_containers/heapster-grafana-amd64:v4.4.3
```
```python
## please make sure you can get the following images
```
```python
image: gcr.io/google_containers/heapster-influxdb-amd64:v1.3.3
```
```python
## the following keyword needs to be replaced
```
```python
## replace __HEAPSTER_GRAFANA_NODE_PORT__
```
```python
## delete service first
```
```python
/root/easypack/k8s/shell/etc/plugins/heapster/1.5.4
```
```python
## create service
```
```python
deployment.extensions/monitoring-grafana created
service/monitoring-grafana created
clusterrolebinding.rbac.authorization.k8s.io/heapster created
clusterrolebinding.rbac.authorization.k8s.io/heapster-kubelet-api created
serviceaccount/heapster created
deployment.extensions/heapster created
service/heapster created
deployment.extensions/monitoring-influxdb created
service/monitoring-influxdb created
```
```python
## begin check heapster, wait for 3s ...
```
```python
NAME                                       READY   STATUS    RESTARTS   AGE
pod/heapster-57996f88b4-gqmnd              1/1     Running   0          3s
pod/monitoring-grafana-b575bb8ff-7ltx8     1/1     Running   0          4s
pod/monitoring-influxdb-6d65f866f5-wvxh4   1/1     Running   0          4s
NAME                           TYPE        CLUSTER-IP      EXTERNAL-IP   PORT
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
service/heapster               ClusterIP   10.254.240.42
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
80/TCP          5s
service/monitoring-grafana     NodePort    10.254.61.98
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
80:33308/TCP    5s
service/monitoring-influxdb    ClusterIP   10.254.65.95
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
8086/TCP        5s
NAME                                   READY   UP-TO-DATE   AVAILABLE   AGE
deployment.apps/heapster               1/1     1            1           5s
deployment.apps/monitoring-grafana     1/1     1            1           5s
deployment.apps/monitoring-influxdb    1/1     1            1           5s
NAME                                             DESIRED   CURRENT   READY   AGE
replicaset.apps/heapster-57996f88b4              1         1         1       5s
replicaset.apps/monitoring-grafana-b575bb8ff     1         1         1       5s
replicaset.apps/monitoring-influxdb-6d65f866f5   1         1         1       5s
```
```python
## Tue Apr  2 19:59:51 CST 2019 ACTION: install  Service: heapster ends  ...
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
# 结果确认
再次确认dashboard，可看到如下信息，已经可以看到CPU和内存的使用状况
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402201321440.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
pod的详细信息中也包含相关信息了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402201403473.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 确认启动后信息
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
# kubectl cluster-info
```
```python
Kubernetes master is running at https://192.168.163.131:6443
Heapster is running at https://192.168.163.131:6443/api/v1/namespaces/kube-system/services/heapster/proxy
CoreDNS is running at https://192.168.163.131:6443/api/v1/namespaces/kube-system/services/kube-dns:dns/proxy
monitoring-grafana is running at https://192.168.163.131:6443/api/v1/namespaces/kube-system/services/monitoring-grafana/proxy
monitoring-influxdb is running at https://192.168.163.131:6443/api/v1/namespaces/kube-system/services/monitoring-influxdb/proxy
To further debug and diagnose cluster problems, use
```
```python
'kubectl cluster-info dump'
```
```python
.
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
可以看到相关组件都已正常动作，同时可以使用上述信息通过grafana确认更加详细的资源监控信息。
# 通过grafana确认监控信息
## 通过33308访问grafana
使用grafana需要三个步骤，登录即可看到，此处设定均已完成，heapster采集的数据存储到influxdb中，而influxdb的数据成为grafana显示的数据源，而这些设定在前面均已完成。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402202652239.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
可以选择cluster或者pod的状况，这里确认kube-system的namespace下的coredns的pod相关的资源信息，如下所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402202936828.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
可以选择不同的pod，确认资源的使用状况
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402203047263.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
也可以选择不同的namespace，比如这里选择default的namespace进行确认
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040220314794.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)

[
](https://img-blog.csdnimg.cn/20190402203047263.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
