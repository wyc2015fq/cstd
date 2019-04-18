# kubelet Pod 的状态分析 - weixin_33985507的博客 - CSDN博客
2018年05月18日 14:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：38
pod从创建到最后的创建成功会分别处于不同的阶段，在源码中用PodPhase来表示不同的阶段：
![12042804-bca95c5c355f6921.png](https://upload-images.jianshu.io/upload_images/12042804-bca95c5c355f6921.png)
> 
运行中（PodRunning）：该 Pod 已经绑定到了一个节点上，Pod 中所有的容器都已被创建。至少有一个容器正在运行，或者正处于启动或重启状态。
等待中（PodPending）: Pod 已被 Kubernetes 系统接受，但有一个或者多个容器镜像尚未创建。等待时间包括调度 Pod 的时间和通过网络下载镜像的时间，这可能需要花点时间。
创建pod的请求已经被k8s接受，但是容器并没有启动成功，可能处在：写数据到etcd，调度，pull镜像，启动容器这四个阶段中的任何一个阶段，pending伴随的事件通常会有：ADDED, Modified这两个事件的产生。
正常终止（PodSucceeded）：pod中的所有的容器已经正常的自行退出，并且k8s永远不会自动重启这些容器，一般会是在部署job的时候会出现。
异常停止（PodEailed）：Pod 中的所有容器都已终止了，并且至少有一个容器是因为失败终止。也就是说，容器以非0状态退出或者被系统终止。
未知状态（PodUnkonwn）：出于某种原因，无法获得Pod的状态，通常是由于与Pod主机通信时出错。
![12042804-df5ae65bc6ec6bd0.png](https://upload-images.jianshu.io/upload_images/12042804-df5ae65bc6ec6bd0.png)
api/core/v1
一个pod的完整创建，通常会伴随着各种事件的产生，k8s种事件的种类总共只有4种：
> 
Added    EventType = "ADDED"
Modified EventType = "MODIFIED"
Deleted  EventType = "DELETED"
Error    EventType = "ERROR"
PodStatus 有一组PodConditions。 PodCondition中的ConditionStatus，它代表了当前pod是否处于某一个阶段（PodScheduled，Ready，Initialized，Unschedulable），“true” 表示处于，“false”表示不处于。
> 
PodCondition数组的每个元素都有一个类型字段和一个状态字段。
类型字段 PodConditionType  是一个字符串，可能的值是
PodScheduled：pod正处于调度中，刚开始调度的时候，hostip还没绑定上，持续调度之后，有合适的节点就会绑定hostip，然后更新etcd数据
Ready: pod 已经可以开始服务，譬如被加到负载均衡里面
Initialized：所有pod 中的初始化容器已经完成了
Unschedulable：限制不能被调度，譬如现在资源不足
状态字段 ConditionStatus  是一个字符串，可能的值为True，False和Unknown
![12042804-2a88e20d59602583.png](https://upload-images.jianshu.io/upload_images/12042804-2a88e20d59602583.png)
![12042804-f5cec6dd1a3b90ac.png](https://upload-images.jianshu.io/upload_images/12042804-f5cec6dd1a3b90ac.png)
容器的状态：
> 
    ContainerStateCreated ContainerState = "created"
    ContainerStateRunning ContainerState = "running"
    ContainerStateExited  ContainerState = "exited"
    // This unknown encompasses all the states that we currently don't care.
    ContainerStateUnknown ContainerState = "unknown"
![12042804-46d3c6f5bab82a6f.png](https://upload-images.jianshu.io/upload_images/12042804-46d3c6f5bab82a6f.png)
![12042804-29d62b68cd4e8a30.png](https://upload-images.jianshu.io/upload_images/12042804-29d62b68cd4e8a30.png)
举一个例子来说明一下情况
kubectl apply -f busybox.yaml
1. 写入数据到etcd
> 
event type: ADDED event object:
{
    "phase": "Pending",
    "qosClass": "BestEffort"
}
2. 开始被调度，但是还未调度到具体node上，请注意：PodScheduled的status=“true”
> 
event type: MODIFIED
{
      "phase": "Pending",
      "conditions": [
        {
              "type": "PodScheduled",
              "status": "True",
              "lastProbeTime": null,
              "lastTransitionTime": "2017-06-06T07:57:06Z"
   }  ],
      "qosClass": "BestEffort"
}
3.  被调度到了具体的node上hostip绑定了，并且被所有初始化容器已经启动完毕（注意busybox.yaml中pod没有指定init container，所以这里很快就被设置成true）,被调度到的节点watch到并开始创建容器（此阶段是在拉去镜像）然后创建容器 ，而此时Ready的status是false，仔细看会发现，containerStatus的状态为waiting
> 
event type:MODIFIED{
  "phase": "Pending",
  "conditions": [
    {
      "type": "Initialized",
      "status": "True",
      "lastProbeTime": null,
      "lastTransitionTime": "2017-06-06T07:57:06Z"    },
    {
      "type": "Ready",
      "status": "False",
      "lastProbeTime": null,
      "lastTransitionTime": "2017-06-06T07:57:06Z",
      "reason": "ContainersNotReady",
      "message": "containers with unready status: [busybox]"    },
    {
      "type": "PodScheduled",
      "status": "True",
      "lastProbeTime": null,
      "lastTransitionTime": "2017-06-06T07:57:06Z"    }
  ],
  "hostIP": "10.39.1.35",
  "startTime": "2017-06-06T07:57:06Z",
  "containerStatuses": [
    {
      "name": "busybox",
      "state": {
        "waiting": {
          "reason": "ContainerCreating"        }
      },
      "lastState": {},
      "ready": false,
      "restartCount": 0,
      "image": "busybox",
      "imageID": ""    }
  ],
  "qosClass": "BestEffort"
}
4. 容器创建成功，Ready的status=“true”，此时容器的status也为running，这个时候，对应的pod的PodPhase也应该为running
> 
event type:MODIFIED{
  "phase": "Running",
  "conditions": [
    {
      "type": "Initialized",
      "status": "True",
      "lastProbeTime": null,
      "lastTransitionTime": "2017-06-06T07:57:06Z"    },
    {
      "type": "Ready",
      "status": "True",
      "lastProbeTime": null,
      "lastTransitionTime": "2017-06-06T07:57:08Z"    },
    {
      "type": "PodScheduled",
      "status": "True",
      "lastProbeTime": null,
      "lastTransitionTime": "2017-06-06T07:57:06Z"    }
  ],
  "hostIP": "10.39.1.35",
  "podIP": "192.168.107.204",
  "startTime": "2017-06-06T07:57:06Z",
  "containerStatuses": [
    {
      "name": "busybox",
      "state": {
        "running": {
          "startedAt": "2017-06-06T07:57:08Z"        }
      },
      "lastState": {},
      "ready": true,
      "restartCount": 0,
      "image": "busybox:latest",
      "imageID": "docker-pullable://busybox@sha256:c79345819a6882c31b41bc771d9a94fc52872fa651b36771fbe0c8461d7ee558",
      "containerID": "docker://a6af9d58c7dabf55fdfe8d4222b2c16349e3b49b3d0eca4bc761fdb571f3cf44"    }
  ],
  "qosClass": "BestEffort"
}
CrashLoopBackOff： 容器退出，kubelet正在将它重启
InvalidImageName： 无法解析镜像名称
ImageInspectError： 无法校验镜像
ErrImageNeverPull： 策略禁止拉取镜像
ImagePullBackOff： 正在重试拉取
RegistryUnavailable： 连接不到镜像中心
ErrImagePull： 通用的拉取镜像出错
CreateContainerConfigError： 不能创建kubelet使用的容器配置
CreateContainerError： 创建容器失败
m.internalLifecycle.PreStartContainer  执行hook报错
RunContainerError： 启动容器失败
PostStartHookError： 执行hook报错
ContainersNotInitialized： 容器没有初始化完毕
ContainersNotReady： 容器没有准备完毕
ContainerCreating：容器创建中
PodInitializing：pod 初始化中
DockerDaemonNotReady：docker还没有完全启动
NetworkPluginNotReady： 网络插件还没有完全启动
