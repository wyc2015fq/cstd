# 四十六、利用yarn多队列实现hadoop资源隔离 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 12:00:18[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：7474












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_307_default_big.png)



大数据处理离不开hadoop集群的部署和管理，对于本来硬件资源就不多的创业团队来说，做好资源的共享和隔离是很有必要的，毕竟不像BAT那么豪，那么怎么样能把有限的节点同时分享给多组用户使用而且互不影响呢，我们来研究一下yarn多队列做资源隔离

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## CapacityScheduler

使用过第一代hadoop的同学应该比较熟悉mapred.job.map.capacity/mapred.job.reduce.capacity这个参数，无论是map还是reduce都可以配置capacity(也就是并发数)，表示同时可以有多少个map(或reduce)运行，通过这个参数可以限制一个任务同时占用的资源(节点)数，这样不至于影响其他任务的执行。

在这里有人会问：我把任务的priority设置成VERY LOW不就行了吗？其实这样在某些场景下不能解决全部问题，因为假如你一个VERY LOW的任务刚启动时没有其他人的任务，那么会先占用所有节点，如果你的每一个task运行时间都是1天，那么其他任务就算优先级再高也只能傻等一天，所以才有必要做资源隔离

第二代hadoop因为使用yarn做资源管理，没有了槽位的概念，所以就没有了capacity。但是在yarn中专门有了CapacityScheduler这个组件。这是一个可插装的调度器，它的用途就是对多用户实现共享大集群并对每个用户资源占用做控制

对于很豪的公司来说，每个用户(团队)自己有一个hadoop集群，这样可以提高自身的稳定性和资源供应，但是确降低了资源利用率，因为很多集群大多数时间都是空闲的。CapacityScheduler能实现这样的功能：每个组固定享有集群里的一部分资源，保证低保，同时如果这个固定的资源空闲，那么可以提供给其他组来抢占，但是一旦这些资源的固定使用者要用，那么立即释放给它使用。这种机制在实现上是通过queue（队列）来实现的。当然CapacityScheduler还支持子队列（sub-queue），



## hadoop资源分配的默认配置

我在《[教你成为全栈工程师(Full Stack Developer) 四十五-一文读懂hadoop、hbase、hive、spark分布式系统架构](http://www.shareditor.com/blogshow/?blogId=96)》中详细描述了整体一套hadoop搭建的方法。那么在搭建完成后我们发现对于资源分配方面，yarn的默认配置是这样的

![](http://www.shareditor.com/uploads/media/my-context/0001/01/03ef999220e8eaa4fc98394c4babf5e8063187a9.png)

也就是有一个默认的队列

事实上，是否使用CapacityScheduler组件是可以配置的，但是默认配置就是这个CapacityScheduler，如果想显式配置需要修改**conf/yarn-site.xml内容如下：**

```xml
<property>
<name>yarn.resourcemanager.scheduler.class</name>
<value>
org.apache.hadoop.yarn.server.resourcemanager.scheduler.capacity.CapacityScheduler
</value>
</property>
```

上面图中标明了默认队列是default，是使用了CapacityScheduler的默认配置

我们看一下有关这里的default是怎么配置的，见capacity-scheduler.xml配置：

```xml
<property>
    <name>yarn.scheduler.capacity.root.queues</name>
    <value>default</value>
    <description>
      The queues at the this level (root is the root queue).
    </description>
  </property>
```

这里的配置项格式应该是yarn.scheduler.capacity.<queue-path>.queues，也就是这里的root是一个queue-path，因为这里配置了value是default，所以root这个queue-path只有一个队列叫做default，那么有关default的具体配置都是形如下的配置项：

yarn.scheduler.capacity.root.default.capacity：一个百分比的值，表示占用整个集群的百分之多少比例的资源，这个queue-path下所有的capacity之和是100

yarn.scheduler.capacity.root.default.user-limit-factor：每个用户的低保百分比，比如设置为1，则表示无论有多少用户在跑任务，每个用户占用资源最低不会少于1%的资源

yarn.scheduler.capacity.root.default.maximum-capacity：弹性设置，最大时占用多少比例资源

yarn.scheduler.capacity.root.default.state：队列状态，可以是RUNNING或STOPPED

yarn.scheduler.capacity.root.default.acl_submit_applications：哪些用户或用户组可以提交人物

yarn.scheduler.capacity.root.default.acl_administer_queue：哪些用户或用户组可以管理队列

当然我们可以继续以root.default为queue-path创建他的子队列，比如：

```xml
<property>
    <name>yarn.scheduler.capacity.root.default.queues</name>
    <value>a,b,c</value>
    <description>
      The queues at the this level (root is the root queue).
    </description>
  </property>
```

这是一个树结构，一般和公司的组织架构有关

配置好上述配置后执行
`yarn rmadmin -refreshQueues`
生效后发现yarn队列情况类似下面的样子(配置了两个队列：research和default)：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/e6acad64f4161a61171c443ed5812397391cb58e.png)

如果希望自己的任务调度到research队列，只需在启动任务时指定：mapreduce.job.queuename参数为research即可




