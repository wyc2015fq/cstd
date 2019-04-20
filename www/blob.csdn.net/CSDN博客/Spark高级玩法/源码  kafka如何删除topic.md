# 源码 | kafka如何删除topic - Spark高级玩法 - CSDN博客
2018年08月21日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：2749
**本文是以kafka0.8.2.2为例讲解**
**一，如何删除一个topic**
删除一个topic有两个关键点：
1，配置删除参数
delete.topic.enable这个Broker参数配置为True。
2，执行
bin/kafka-topics.sh --zookeeper zk_host:port/chroot --delete --topic my_topic_name
假如不配置删除参数为true的话，topic其实并没有被清除，只是被标记为删除。此时，估计一般人的做法是删除topic在Zookeeper的信息和日志，其实这个操作并不会清除kafkaBroker内存的topic数据。所以，此时最佳的策略是配置删除参数为true然后，重启kafka。
**二，重要的类介绍**
**1，PartitionStateMachine**
该类代表分区的状态机。决定者分区的当前状态，和状态转移。四种状态
NonExistentPartition
NewPartition
OnlinePartition
OfflinePartition
**2，ReplicaManager**
负责管理当前机器的所有副本，处理读写、删除等具体动作。
读写：写获取partition对象，再获取Replica对象，再获取Log对象，采用其管理的Segment对象将数据写入、读出。
**3，ReplicaStateMachine**
副本的状态机。决定者副本的当前状态和状态之间的转移。一个副本总共可以处于一下几种状态的一种
NewReplica：Crontroller在分区重分配的时候可以创建一个新的副本。只能接受变为follower的请求。前状态可以是NonExistentReplica
OnlineReplica：新启动的分区，能接受变为leader或者follower请求。前状态可以是NewReplica, OnlineReplica or OfflineReplica
OfflineReplica：死亡的副本处于这种状态。前状态可以是NewReplica, OnlineReplica
ReplicaDeletionStarted：分本删除开始的时候处于这种状态，前状态是OfflineReplica
ReplicaDeletionSuccessful：副本删除成功。前状态是ReplicaDeletionStarted
ReplicaDeletionIneligible：删除失败的时候处于这种状态。前状态是ReplicaDeletionStarted
NonExistentReplica：副本成功删除之后处于这种状态，前状态是ReplicaDeletionSuccessful
**4，TopicDeletionManager**
该类管理着topic删除的状态机
1)，TopicCommand通过创建/admin/delete_topics/<topic>，来发布topic删除命令。
2)，Controller监听/admin/delete_topic子节点变动，开始分别删除topic
3)，Controller有个后台线程负责删除Topic
**三，源码彻底解析topic的删除过程**
此处会分四个部分：
A),客户端执行删除命令作用
B),不配置delete.topic.enable整个流水的源码
C),配置了delete.topic.enable整个流水的源码
D),手动删除zk上topic信息和磁盘数据
**1，客户端执行删除命令**
bin/kafka-topics.sh --zookeeper zk_host:port/chroot --delete --topic my_topic_name
进入kafka-topics.sh我们会看到
*exec *$*(dirname $0)*/kafka-run-class.sh kafka.admin.TopicCommand $@
进入TopicCommand里面，main方法里面
else if(opts.*options*.has(opts.*deleteOpt*))
*deleteTopic*(zkClient, opts)
实际内容是
val topics = *getTopics*(zkClient, opts)
if (topics.length == 0) {
*println*("Topic %s does not exist".format(opts.*options*.valueOf(opts.*topicOpt*)))
}
topics.foreach { topic =>
  try {
    ZkUtils.*createPersistentPath*(zkClient, ZkUtils.*getDeleteTopicPath*(topic))
在"/admin/delete_topics"目录下创建了一个topicName的节点。
**2，假如不配置delete.topic.enable整个流水是**
总共有两处listener会响应：
A),TopicChangeListener
B),DeleteTopicsListener
使用topic的删除命令删除一个topic的话，指挥触发DeleteTopicListener。
var topicsToBeDeleted = {
  import JavaConversions._
  (children: Buffer[String]).toSet
}
val nonExistentTopics = topicsToBeDeleted.filter(t => !*controllerContext*.*allTopics*.contains(t))
topicsToBeDeleted --= nonExistentTopics
if(topicsToBeDeleted.size > 0) {
  info("Starting topic deletion for topics " + topicsToBeDeleted.mkString(","))
  // mark topic ineligible for deletion if other state changes are in progress
topicsToBeDeleted.foreach { topic =>
    val preferredReplicaElectionInProgress =
*controllerContext*.*partitionsUndergoingPreferredReplicaElection*.map(_.topic).contains(topic)
    val partitionReassignmentInProgress =
*controllerContext*.*partitionsBeingReassigned*.keySet.map(_.topic).contains(topic)
    if(preferredReplicaElectionInProgress || partitionReassignmentInProgress)
      controller.*deleteTopicManager*.markTopicIneligibleForDeletion(Set(topic))
  }
  // add topic to deletion list 
  controller.*deleteTopicManager*.enqueueTopicsForDeletion(topicsToBeDeleted)
}
由于都会判断delete.topic.enable是否为true，假如不为true就不会执行，为true就进入执行
controller.*deleteTopicManager*.markTopicIneligibleForDeletion(Set(topic))
controller.*deleteTopicManager*.enqueueTopicsForDeletion(topicsToBeDeleted)
**3，delete.topic.enable配置为true**
此处与步骤2的区别，就是那两个处理函数。
controller.*deleteTopicManager*.markTopicIneligibleForDeletion(Set(topic))
controller.*deleteTopicManager*.enqueueTopicsForDeletion(topicsToBeDeleted)
markTopicIneligibleForDeletion函数的处理为
if(*isDeleteTopicEnabled*) {
  val newTopicsToHaltDeletion = *topicsToBeDeleted *& topics
*topicsIneligibleForDeletion *++= newTopicsToHaltDeletion
  if(newTopicsToHaltDeletion.size > 0)
    info("Halted deletion of topics %s".format(newTopicsToHaltDeletion.mkString(",")))
}
主要是停止删除topic，假如存储以下三种情况
** Halt delete topic if -*** 1. replicas being down*** 2. partition reassignment in progress for some partitions of the topic*** 3. preferred replica election in progress for some partitions of the topic*
enqueueTopicsForDeletion主要作用是更新删除topic的集合，并激活TopicDeleteThread
def enqueueTopicsForDeletion(topics: Set[String]) {
  if(*isDeleteTopicEnabled*) {
*topicsToBeDeleted *++= topics
*partitionsToBeDeleted *++= topics.flatMap(*controllerContext*.partitionsForTopic)
    resumeTopicDeletionThread()
  }
}
在删除线程DeleteTopicsThread的doWork方法中
topicsQueuedForDeletion.foreach { topic =>
// if all replicas are marked as deleted successfully, then topic deletion is done
  if(controller.*replicaStateMachine*.areAllReplicasForTopicDeleted(topic)) {
    // clear up all state for this topic from controller cache and zookeeper
    completeDeleteTopic(topic)
    info("Deletion of topic %s successfully completed".format(topic))
  }
进入completeDeleteTopic方法中
// deregister partition change listener on the deleted topic. This is to prevent the partition change listener
// firing before the new topic listener when a deleted topic gets auto created
*partitionStateMachine*.deregisterPartitionChangeListener(topic)
val replicasForDeletedTopic = controller.*replicaStateMachine*.replicasInState(topic, ReplicaDeletionSuccessful)
// controller will remove this replica from the state machine as well as its partition assignment cache
*replicaStateMachine*.handleStateChanges(replicasForDeletedTopic, NonExistentReplica)
val partitionsForDeletedTopic = *controllerContext*.partitionsForTopic(topic)
// move respective partition to OfflinePartition and NonExistentPartition state
*partitionStateMachine*.handleStateChanges(partitionsForDeletedTopic, OfflinePartition)
*partitionStateMachine*.handleStateChanges(partitionsForDeletedTopic, NonExistentPartition)
*topicsToBeDeleted *-= topic
*partitionsToBeDeleted*.retain(_.topic != topic)
*controllerContext*.zkClient.deleteRecursive(ZkUtils.*getTopicPath*(topic))
*controllerContext*.zkClient.deleteRecursive(ZkUtils.*getTopicConfigPath*(topic))
*controllerContext*.zkClient.delete(ZkUtils.*getDeleteTopicPath*(topic))
*controllerContext*.removeTopic(topic)
**主要作用是解除掉监控分区变动的listener，删除Zookeeper具体节点信息，删除磁盘数据，更新内存数据结构，比如从副本状态机里面移除分区的具体信息。**
**其实，最终要的是我们的副本磁盘数据是如何删除的。我们重点介绍这个部分。**
**首次清除的话，在删除线程DeleteTopicsThread的doWork方法中**
{
  // if you come here, then no replica is in TopicDeletionStarted and all replicas are not in
  // TopicDeletionSuccessful. That means, that either given topic haven't initiated deletion
  // or there is at least one failed replica (which means topic deletion should be retried).
  if(controller.*replicaStateMachine*.isAnyReplicaInState(topic, ReplicaDeletionIneligible)) {
    // mark topic for deletion retry
    markTopicForDeletionRetry(topic)
  }
进入markTopicForDeletionRetry
val failedReplicas = controller.replicaStateMachine.replicasInState(topic, ReplicaDeletionIneligible)
info("Retrying delete topic for topic %s since replicas %s were not successfully deleted"
  .format(topic, failedReplicas.mkString(",")))
controller.*replicaStateMachine*.handleStateChanges(failedReplicas, OfflineReplica)
在ReplicaStateMachine的handleStateChanges方法中，调用了handleStateChange，处理OfflineReplica
// send stop replica command to the replica so that it stops fetching from the leader
*brokerRequestBatch*.addStopReplicaRequestForBrokers(*List*(replicaId), topic, partition, deletePartition = false)
接着在handleStateChanges中
*brokerRequestBatch*.sendRequestsToBrokers(controller.epoch, *controllerContext*.*correlationId*.getAndIncrement)
给副本数据存储节点发送StopReplicaKey副本指令，并开始删除数据
stopReplicaRequestMap foreach { case(broker, replicaInfoList) =>
  val stopReplicaWithDelete = replicaInfoList.filter(p => p.deletePartition == true).map(i => i.replica).toSet
  val stopReplicaWithoutDelete = replicaInfoList.filter(p => p.deletePartition == false).map(i => i.replica).toSet
  debug("The stop replica request (delete = true) sent to broker %d is %s"
    .format(broker, stopReplicaWithDelete.mkString(",")))
  debug("The stop replica request (delete = false) sent to broker %d is %s"
    .format(broker, stopReplicaWithoutDelete.mkString(",")))
  replicaInfoList.foreach { r =>
    val stopReplicaRequest = new StopReplicaRequest(r.deletePartition,
      Set(*TopicAndPartition*(r.replica.topic, r.replica.partition)), *controllerId*, controllerEpoch, correlationId)
    controller.sendRequest(broker, stopReplicaRequest, r.callback)
  }
}
stopReplicaRequestMap.clear()
Broker的KafkaApis的Handle方法在接受到指令后
case RequestKeys.*StopReplicaKey *=> handleStopReplicaRequest(request)
val (response, error) = replicaManager.stopReplicas(stopReplicaRequest)
接着是在stopReplicas方法中
{
*controllerEpoch *= stopReplicaRequest.controllerEpoch
  // First stop fetchers for all partitions, then stop the corresponding replicas
*replicaFetcherManager*.removeFetcherForPartitions(stopReplicaRequest.partitions.map(r => *TopicAndPartition*(r.topic, r.partition)))
  for(topicAndPartition <- stopReplicaRequest.partitions){
    val errorCode = stopReplica(topicAndPartition.topic, topicAndPartition.partition, stopReplicaRequest.deletePartitions)
    responseMap.put(topicAndPartition, errorCode)
  }
  (responseMap, ErrorMapping.*NoError*)
}
进一步进入stopReplica方法，正式进入日志删除
getPartition(topic, partitionId) match {
  case *Some*(partition) =>
    if(deletePartition) {
      val removedPartition = *allPartitions*.remove((topic, partitionId))
      if (removedPartition != null)
        removedPartition.delete() // this will delete the local log
    }
以上就是kafka的整个日志删除流水。
**4，手动删除zk上topic信息和磁盘数据**
TopicChangeListener会监听处理，但是处理很简单，只是更新了
val deletedTopics = *controllerContext*.*allTopics *-- currentChildren
*controllerContext*.*allTopics *= currentChildren
val addedPartitionReplicaAssignment = ZkUtils.*getReplicaAssignmentForTopics*(*zkClient*, newTopics.toSeq)
*controllerContext*.*partitionReplicaAssignment *= *controllerContext*.*partitionReplicaAssignment*.filter(p =>
**四，总结**
Kafka的topic的删除过程，实际上就是基于Zookeeper做了一个订阅发布系统。Zookeeper的客户端创建一个节点/admin/delete_topics/<topic>，由kafka Controller监听到事件之后正式触发topic的删除：解除Partition变更监听的listener，清除内存数据结构，删除副本数据，删除topic的相关Zookeeper节点。
delete.topic.enable配置该参数为false的情况下执行了topic的删除命令，实际上未做任何动作。我们此时要彻底删除topic建议修改该参数为true，重启kafka，这样topic信息会被彻底删除，已经测试。
一般流行的做法是手动删除Zookeeper的topic相关信息及磁盘数据但是这样的话会造成部分内存数据未清除。至于是否会有隐患，未测试。
**推荐阅读：**
[干货:Flink+Kafka 0.11端到端精确一次处理语义实现](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485150&idx=1&sn=ba0570a9f631501a189d8184807d518b&chksm=9f38e5f6a84f6ce0dd805eae87d6c62e2c96670f14c2bac1607961614ba49f310a4630e31a3b&scene=21#wechat_redirect)
[消息队列服务Kafka揭秘：痛点、优势以及适用场景](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485214&idx=1&sn=343ce3ed2fff8ea6b2c4a74018a093c4&chksm=9f38e436a84f6d2026217420caf0ce71713f1524eb416d4b7dde9a859a4919fd22ffcbccdef8&scene=21#wechat_redirect)
更多经验干货，浪尖知识星球，值得你加入。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640?wx_fmt=jpeg)
