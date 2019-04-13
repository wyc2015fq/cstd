
# 再探Kubernetes - 知行合一 止于至善 - CSDN博客

置顶2019年03月12日 21:00:05[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：6520标签：[kubernetes																](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)[k8s																](https://so.csdn.net/so/search/s.do?q=k8s&t=blog)[安装																](https://so.csdn.net/so/search/s.do?q=安装&t=blog)[入门																](https://so.csdn.net/so/search/s.do?q=入门&t=blog)[部署																](https://so.csdn.net/so/search/s.do?q=部署&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=入门&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=安装&t=blog)个人分类：[Kubernetes																](https://blog.csdn.net/liumiaocn/article/category/6328275)
[
																					](https://so.csdn.net/so/search/s.do?q=安装&t=blog)所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)[
							](https://so.csdn.net/so/search/s.do?q=安装&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=k8s&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)

简单整理最新Kubernetes部署到使用的一些memo，顺便将其固化成脚本和Ansible方式。同步到github的Easypack中，有兴趣的可以加入。
# 安装系列
|序号|内容|链接|
|---|---|---|
|1|Kubernetes安装系列之事前准备|[https://liumiaocn.blog.csdn.net/article/details/88163677](https://liumiaocn.blog.csdn.net/article/details/88163677)|
|2|Kubernetes安装系列之NTP时间同步服务创建|[https://liumiaocn.blog.csdn.net/article/details/88417298](https://liumiaocn.blog.csdn.net/article/details/88417298)|
|3|Kubernetes安装系列之chrony时间同步服务创建|[https://blog.csdn.net/liumiaocn/article/details/88421217](https://blog.csdn.net/liumiaocn/article/details/88421217)|
|4|Kubernetes安装系列之证书创建|[https://liumiaocn.blog.csdn.net/article/details/88755820](https://liumiaocn.blog.csdn.net/article/details/88755820)|
|5|Kubernetes安装系列之ETCD安装|[https://liumiaocn.blog.csdn.net/article/details/88757389](https://liumiaocn.blog.csdn.net/article/details/88757389)|
|6|Kubernetes安装系列之Master-Apiserver安装|[https://liumiaocn.blog.csdn.net/article/details/88768347](https://liumiaocn.blog.csdn.net/article/details/88768347)|
|7|Kubernetes安装系列之Master-scheduler安装|[https://liumiaocn.blog.csdn.net/article/details/88773571](https://liumiaocn.blog.csdn.net/article/details/88773571)|
|8|Kubernetes安装系列之Master-controller-manager安装|[https://liumiaocn.blog.csdn.net/article/details/88773834](https://liumiaocn.blog.csdn.net/article/details/88773834)|
|9|Kubernetes安装系列之网络组件-Flannel安装设定|[https://blog.csdn.net/liumiaocn/article/details/88835527](https://blog.csdn.net/liumiaocn/article/details/88835527)|
|10|Kubernetes安装系列之Docker安装与设定|[https://liumiaocn.blog.csdn.net/article/details/88843105](https://liumiaocn.blog.csdn.net/article/details/88843105)|
|11|Kubernetes安装系列之证书与kubeconfig设定|[https://liumiaocn.blog.csdn.net/article/details/88849945](https://liumiaocn.blog.csdn.net/article/details/88849945)|
|12|Kubernetes安装系列之Node-Kubelet安装|[https://liumiaocn.blog.csdn.net/article/details/88879322](https://liumiaocn.blog.csdn.net/article/details/88879322)|
|13|Kubernetes安装系列之Node-Kube-proxy安装|[https://liumiaocn.blog.csdn.net/article/details/88879433](https://liumiaocn.blog.csdn.net/article/details/88879433)|
|14|1分钟部署单机版kubernetes 1.13.4|[https://liumiaocn.blog.csdn.net/article/details/88882893](https://liumiaocn.blog.csdn.net/article/details/88882893)|
|15|Kubernetes安装系列之tls方式下的kubectl设定|[https://liumiaocn.blog.csdn.net/article/details/88908796](https://liumiaocn.blog.csdn.net/article/details/88908796)|
|16|Kubernetes安装系列之kubctl exec权限设定|[https://liumiaocn.blog.csdn.net/article/details/88944523](https://liumiaocn.blog.csdn.net/article/details/88944523)|
|17|Kubernetes设定之TLS客户端访问|[https://liumiaocn.blog.csdn.net/article/details/88945671](https://liumiaocn.blog.csdn.net/article/details/88945671)|
|18|Kubernetes安装系列之Dashboard安装|[https://liumiaocn.blog.csdn.net/article/details/88957901](https://liumiaocn.blog.csdn.net/article/details/88957901)|
|19|Kubernetes安装系列之Dashboard的kubeconfig登录方式|[https://liumiaocn.blog.csdn.net/article/details/88961823](https://liumiaocn.blog.csdn.net/article/details/88961823)|
|20|Kubernetes安装系列之coredns安装|[https://liumiaocn.blog.csdn.net/article/details/88979064](https://liumiaocn.blog.csdn.net/article/details/88979064)|
|21|Kubernetes安装系列之Kubelet指标获取方式|[https://liumiaocn.blog.csdn.net/article/details/88965862](https://liumiaocn.blog.csdn.net/article/details/88965862)|
|22|Kubernetes安装系列之heapster安装|[https://liumiaocn.blog.csdn.net/article/details/88979793](https://liumiaocn.blog.csdn.net/article/details/88979793)|
|23|Kubernetes安装系列之prometheus安装|TBD|
|24|TBD|TBD|
## 代码github地址：
[https://github.com/liumiaocn/easypack/tree/master/k8s/shell](https://github.com/liumiaocn/easypack/tree/master/k8s/shell)
# 常见问题
|序号|内容|链接|
|---|---|---|
|1|kubelet启动常见问题|[https://liumiaocn.blog.csdn.net/article/details/87070567](https://liumiaocn.blog.csdn.net/article/details/87070567)|
|2|HTTPS方式ETCD客户端连接提示bad certificate对应方法|[https://liumiaocn.blog.csdn.net/article/details/88760242](https://liumiaocn.blog.csdn.net/article/details/88760242)|
|3|flannel生成的vxlan设备的删除方法|[https://liumiaocn.blog.csdn.net/article/details/88835355](https://liumiaocn.blog.csdn.net/article/details/88835355)|
|4|使用flannel配置docker网络|[https://liumiaocn.blog.csdn.net/article/details/88839952](https://liumiaocn.blog.csdn.net/article/details/88839952)|
# 常用操作
|序号|内容|链接|
|---|---|---|
|1|创建和删除|[https://blog.csdn.net/liumiaocn/article/details/73913597](https://blog.csdn.net/liumiaocn/article/details/73913597)|
|2|故障排查|[https://blog.csdn.net/liumiaocn/article/details/73925301](https://blog.csdn.net/liumiaocn/article/details/73925301)|
|3|故障对应|[https://blog.csdn.net/liumiaocn/article/details/73997635](https://blog.csdn.net/liumiaocn/article/details/73997635)|
|4|docker命令 vs kubectl|[https://blog.csdn.net/liumiaocn/article/details/74007635](https://blog.csdn.net/liumiaocn/article/details/74007635)|
# 基础入门介绍
|序号|内容|链接|
|---|---|---|
|1|kubernetes1.4 基础篇：概要|[https://liumiaocn.blog.csdn.net/article/details/53017671](https://liumiaocn.blog.csdn.net/article/details/53017671)|
|2|kubernetes1.4 基础篇：Create a Kubernetes cluster|[https://liumiaocn.blog.csdn.net/article/details/53017671](https://liumiaocn.blog.csdn.net/article/details/53017671)|
|3|kubernetes1.4 基础篇： Using kubectl to Create a Deployment|[https://liumiaocn.blog.csdn.net/article/details/53112069](https://liumiaocn.blog.csdn.net/article/details/53112069)|
|4|kubernetes1.4 基础篇：Explore Your App|[https://liumiaocn.blog.csdn.net/article/details/53125541](https://liumiaocn.blog.csdn.net/article/details/53125541)|
|5|kubernetes1.4 基础篇：Expose Your App Publicly|[https://liumiaocn.blog.csdn.net/article/details/53137936](https://liumiaocn.blog.csdn.net/article/details/53137936)|
|6|kubernetes1.4 基础篇：Scale Your App|[https://liumiaocn.blog.csdn.net/article/details/53150288](https://liumiaocn.blog.csdn.net/article/details/53150288)|
# 操作基础
|内容|链接|
|---|---|
|入门篇|
|Kubernetes上的第一个HelloWorld例子|[http://blog.csdn.net/liumiaocn/article/details/52366063](http://blog.csdn.net/liumiaocn/article/details/52366063)|
|基础之Pod系列|
|单容器pod创建（RC方式）|[http://blog.csdn.net/liumiaocn/article/details/52437132](http://blog.csdn.net/liumiaocn/article/details/52437132)|
|单容器pod创建（Pod方式）|
|多容器pod创建|
|pod生命周期|
|Pod健康状况查询|
|Pod设定|
|…|
|基础之BatchJob系列|
|基础之服务发现|
|基础之负载均衡|
|基础之监控及日志|
|基础之Dashboard|
|基础之Deplyoment管理|
|基础之Kubernetes API|
|基础之Kubectl CLI|
|基础之Batch API|
# 定制开发
|序号|内容|链接|
|---|---|---|
|1|TBD|TBD|

