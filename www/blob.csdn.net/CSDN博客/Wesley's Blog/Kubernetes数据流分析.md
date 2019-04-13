
# Kubernetes数据流分析 - Wesley's Blog - CSDN博客


2018年07月25日 08:53:10[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：1715


# 以一个网页应用的访问为例
首先在部署阶段，需要启动两个容器WebApp容器和Mysql容器。
整个部署过程：
启动Mysql，mysql-rc.yaml在之后给出,创建对应的Service
启动myweb， myweb-rc.yaml在之后给出，创建对应的Service，并通过NodePort方式暴露服务
通过浏览器访问网页
![这里写图片描述](https://img-blog.csdn.net/20180725085041217?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
此时，如果从外部访问服务，则流程如图：
![这里写图片描述](https://img-blog.csdn.net/20180725085053914?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
有三种访问方式：
![这里写图片描述](https://img-blog.csdn.net/20180725085120984?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 过程原理
## 从Service到Pod
k8s在创建服务的时候会为服务分配一个虚拟的IP地址，然后客户端通过访问这个虚拟的IP地址来访问服务，而服务则负责将请求转发到后端的Pod上。
而真正起到作用的就是**kube-proxy**，每个Node都会运行一个kube-proxy进程。对每个TCP类型的Service，kube-proxy都会在本地Node上建立一个SocketServer来负责接收请求，然后转发都后端某个Pod的端口上，这个过程默认采用**Round Robin负载均衡算法**。
此外，Service的ClusterIP与NodePort等概念是kube-proxy服务**通过iptables的NAT转换**实现的，kube-proxy在运行过程中动态创建与Service相关的Iptables规则
![这里写图片描述](https://img-blog.csdn.net/20180725085217203?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 容器与容器间通信
同一个Pod内共享同一个网络命名空间，可以直接使用Linux本地IPC进行通信
## Pod之间通信
#### 同一个Node上通信
![这里写图片描述](https://img-blog.csdn.net/20180725085240129?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180725085240129?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Pod1和Pod2都是通过Veth连接在同一个docker0网桥上的，它们的IP地址IP1，IP2都是从docker0的网段上动态获取的，它们和网桥本身的IP3是同一个网段的。
另外，在Pod1, Pod2的Linux协议栈上，默认路由都是docker0的地址，也就是说所有非本地地址的网络数据，都会被默认发送到docker0网桥上，由docker0网桥直接中专
#### 不同Node上的Pod通信
## 从NodePort到ServicePort
# 具体文件
mysql-rc.yaml文件：
`apiVersion: v1
kind: ReplicationController
metadata:
    name: mysql
spec:
    replicas:1
    selector:
        app:mysql
    template:
        metadata:
            labels:
                app:mysql
        spec:
            containers:
            - name:mysql
              image:mysql
              ports:
              - containerPort:3306
              env:
              - name: MYSQL_ROOT_PASSWORD
                value:"123456"`Mysql Service创建：
`apiVersion: v1
kind: Service
metadata:
    name: mysql
spec:
    ports:
        -port: 3306
    selector:
        app: mysql`myweb-rc.yaml文件：
`apiVersion: v1
kind: ReplicationController
metadata:
    name: myweb
spec:
    replicas:1
    selector:
        app:myweb
    template:
        metadata:
            labels:
                app:myweb
        spec:
            containers:
            - name: myweb
              image: kubeguide/tomcat-app:v1
              ports:
              - containerPort:8080`Myweb Service创建：
`apiVersion: v1
kind: Service
metadata:
    name: myweb
spec:
    type: NodePort
    ports:
        -port: 8080
        nodePort: 30001
    selector:
        app: mysql`
[
						](https://img-blog.csdn.net/20180725085240129?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
	](https://img-blog.csdn.net/20180725085240129?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
