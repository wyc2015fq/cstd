
# Kubernetes Basic - Wesley's Blog - CSDN博客


2018年07月22日 11:01:08[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：46标签：[Kubernetes																](https://so.csdn.net/so/search/s.do?q=Kubernetes&t=blog)[云技术																](https://so.csdn.net/so/search/s.do?q=云技术&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Kubernetes&t=blog)个人分类：[云技术																](https://blog.csdn.net/yinanmo5569/article/category/7808067)



# Create a Deployment
要在k8s上部署容器化应用，需要创建一个k8s配置文件。
使用kubectl API来创建部署，创建部署需要指明所使用地容器镜像和创建数量。
通过`kubectl run`来创建一个部署：
![这里写图片描述](https://img-blog.csdn.net/20180722105447291?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`--port`指定了端口号
![这里写图片描述](https://img-blog.csdn.net/20180722105515404?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### Deployment vs Service
Deployment的主要目的是管理相似的Pods，除此之外，升级pods也是通过deployment进行的。
Service是逻辑上定义了一个或多个pods，是它们的高层抽象。它也定义了访问内部pods（也称作“微服务”）的方法，但是对于deployment却没有。Service都有Label，也决定了pods是哪（几）个services的，但是deployment却可以不依赖于selector。
在启动Deployment和Service的时候并没有太多的差别。
在寻找ReplicaSet的时候，Deployment只有通过label进行筛选，因为属于Deployment的ReplicaSet远比属于Service的多。
对于服务来说，Pods的唯一缺点是依赖于Nodes， 当一个node消失的时候，pods也紧接着自动消失，在另一方面， pods在deployment中不依赖于nodes。但是deployment没办法将内部pods暴露在外，只能通过service来实现
# Explore APP
### Pods
共享Volumes
共享Network
包含如何运行各个container的information，例如image信息和ports信息
![这里写图片描述](https://img-blog.csdn.net/20180722105552508?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### Nodes
每个Nodes至少运行了：
Kubelet，用于与kuberletes其他节点（包括主节点）进行交流的进程，它管理着Pods和Containers
一个容器管理进程（例如Docker）
![这里写图片描述](https://img-blog.csdn.net/20180722105633698?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
使用如下几条指令来查看集群状态：
kubectl get - list resources
kubectl describe - show detailed information about a resource
kubectl logs - print the logs from a container in a pod
kubectl exec - execute a command on a container in a pod
# Using Service to Expose APP
每个Pod有个自己的IP地址，但是只能内部访问，需要通过Service来暴露给外部。
服务可以通过不同的方式暴露给外部，通过确定ServiceSpec中的type来达到，type分为4类：
ClusterIP(默认): 在集群内部IP中暴露服务，这让服务只能在cluster内部访问
NodePort: 在每个选中的集群中的Node上，相同的Port，暴露服务。让服务可以通过<NodeIP>:<NodePort>进行访问，是ClusterIP的更高级的形式
LoadBalancer: 在现有云中创建一个外部负载均衡器，指定一个稳定的外部IP来暴露服务，是NodePort的更高级的形式
ExternalName: 使用任意的名字暴露服务（在spec中指定externalName）通过返回一个CNAME record。
然后对于Service来说，Port有x种：
Port: 服务监听的端口号（本身的端口）
targetPort：需要转发到后端Pod的端口号（与Service Port形成映射）
nodePort：当spec.type=NodePort时，指定映射到物理机的端口号
![这里写图片描述](https://img-blog.csdn.net/2018072210571416?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Services通过**Labels and Selectors**来与多个Pods相匹配
![这里写图片描述](https://img-blog.csdn.net/20180722105750299?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 举个栗子
首先考虑简单情况，一个单节点（Node）的集群，在这个节点上运行着单个Pod（kubernetes-bootcamp-5c69669756-8bprt），里面只有单个container，根据下图可以看出，这个`ContainerPort`是8080端口，`PodIP`为172.18.0.2，`NodeIP`为172.17.0.65：
![这里写图片描述](https://img-blog.csdn.net/20180722105821906?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后我们再通过service来expose服务，指定`Port`为8080（这个为ServicePort），这个操作将Service的8080端口映射到了`NodePort`：30041，然后Service的8080端口通过`kube-proxy`转发到后端的Pod实例上，注意Service的8080端口，和Container的8080端口是两个不一样的东西，但是它们形成了映射（如何形成的可能是因为相同而默认或者是有设置）：
![这里写图片描述](https://img-blog.csdn.net/2018072210584258?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后，我们就可以通过三种不同的方式来访问cluster内部：
通过NodePort访问，即<NodeIP, NodePort>，再type为“NodePort”情况下，只有这种方式可以通过外部进行访问
内部访问，通过ClusterIP，ServicePort，通过服务转发访问，clusterIP也成为ServiceIP，无法ping通
内部访问，通过PodIP， ContainerPort访问，直接定位到容器（PodIP是可以ping通的)
![这里写图片描述](https://img-blog.csdn.net/20180722105911704?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# Scaling an application
对cluster进行操作都是通过deployment来进行操作的，Service只是一个高层抽象，例如scale部署到4个replicas：
`#kubectl scale deployments/kubernetes-bootcamp --replicas=4`![这里写图片描述](https://img-blog.csdn.net/20180722105936669?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
用图片表示如图：
![这里写图片描述](https://img-blog.csdn.net/20180722105952413?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# Rolling Update
滚动更新使集群能够进行运行时更新，而不需要停止，在更新期间，Service会自动`load-balance`到可用的Pods，具体步骤如下：
通过容器镜像创建一个应用
Rollback之前的版本
零停机时间的应用程序的连续集成和连续传递
例如可以通过如下指令进行部署更新：
`#kubectl set image deployments/kubernetes-bootcamp kubernetes-bootcamp=jocatalin/kubernetes-bootcamp:v2`![这里写图片描述](https://img-blog.csdn.net/20180722110016180?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考：[kubernetes-basics](https://kubernetes.io/docs/tutorials/kubernetes-basics/)

