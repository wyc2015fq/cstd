
# Kubernetes Volume - Wesley's Blog - CSDN博客


2018年07月25日 08:47:38[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：115


# Volume（存储卷）
Volume是在Pod中能够被多个容器访问的**共享目录**。k8s中的Volume与Pod的生命周期相同，但是与容器的生命周期不同。
Volume的使用比较简单，大多数情况下在Pod上声明一个Volume，然后在容器引用该Volume并Mount到容器里的某个目录上。
k8s提供了丰富的Volume类型：
1. emptyDir: Pod分配到Node时创建，由k8s自动分配，作为临时目录
2. hostPath：为在Pod上挂载宿主机上的文件或目录，可以用于日志文件的永久保存，访问宿主机docker引擎内部数据结构的容器应用
3. gcePersistentDisk：表示使用了谷歌公有云提供的永久磁盘存放Volume数据
4. awsElastiBlockStore：表示使用了亚马逊公有云提供的EBS Volume存储数据
5. NFS：使用NFS（Network File System, 网络文件系统）
5. 其他类型的Volume
# Persistent Volume
PV可以理解成k8s中某个网络存储中对应的一块存储，与Volume类似，但是有以下区别：
PV只能是网络存储，不属于任何Node，但可以在每个Node上访问
PV并不是定义在Pod上的，而是独立于Pod之外定义
我们通常会先定义一个网络存储，然后从中划出一个“网盘”并挂接到虚拟机上。PV是有状态的，有如下几种状态：
Available: 空闲状态
Bound：已经绑定到某个PVC上
Released：对应的PVC已经删除，但资源还没有被集群收回
Failed：PV自动回收失败
下面例子声明了一个PV:
`apiVersion: v1
kind: PersistentVolume
metadata:
    name: pv1
spec:  
    capacity:
        storage: 5Gi
    accessModes:
        - ReadWriteOnce
    persistentVolumeReclainmPolicy: Recycle
    storageClassName: slow
    nfs:
        path: /tmp
        server: 172.17.0.2`
# PV 和 PVC 的生命周期
![这里写图片描述](https://img-blog.csdn.net/20180725084653912?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180725084653912?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
1. 资源供应（Provisioning）
分为静态模式和动态模式
- 静态模式手动创建PV，
- 动态模式午需管理员手动创建PV，而是通过StorageClass的设置对后端存储进行描述，标记为某种“类型”
2. 资源绑定（Binding）
系统根据PVC对资源的请求（存储空间storage和访问模式Access Mode）在已存在的PV中选择一个满足PVC要求的PV，一旦找到，就将该PV与用户定义的PVC进行绑定，然后用户的应用就可以使用这个PVC了。如果不存在，则PVC会无限期处于Pending状态。
3. 资源使用（Using）
Pod使用volume的定义，将PVC挂载到容器内的某个路径进行使用，在容器挂载了一个PVC后，就能被持续独占使用。不过，多个Pod可以同时挂载同一个PVC，应用程序需要考虑多个实例共同访问一块存储空间的问题。
4. 资源释放（Releasing）
当用户对存储资源使用完毕后，用户可以删除PVC，与该PVC绑定的PV将会被标记为“已释放”
5. 资源回收（Reclaiming）
对于PV，管理员可以设定回收策略(Reclaim Policy)

