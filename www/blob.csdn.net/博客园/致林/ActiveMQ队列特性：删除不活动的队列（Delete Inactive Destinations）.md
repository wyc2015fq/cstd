
# ActiveMQ队列特性：删除不活动的队列（Delete Inactive Destinations） - 致林 - 博客园






# [ActiveMQ队列特性：删除不活动的队列（Delete Inactive Destinations）](https://www.cnblogs.com/bincoding/p/8254093.html)

### 方法一
通过 ActiveMQ Web 控制台删除。

### 方法二
通过 Java 代码删除。
ActiveMQConnection.destroyDestination(ActiveMQDestination destination)
### 方法三
通过配置 conf/activemq.xml， 当 broker 探测到闲置的队列或主题，删除、回收资源。
<brokerxmlns="http://activemq.apache.org/schema/core"brokerName="localhost"dataDirectory="${activemq.data}"schedulePeriodForDestinationPurge="3600000"><destinationPolicy><policyMap><policyEntries><policyEntrytopic=">"><pendingMessageLimitStrategy><constantPendingMessageLimitStrategylimit="1000"/></pendingMessageLimitStrategy></policyEntry><policyEntryqueue=">"gcInactiveDestinations="true"inactiveTimoutBeforeGC="30000"/></policyEntries></policyMap></destinationPolicy></broker>
schedulePeriodForDestinationPurge = 3600000，表示每一小时检查一次，默认为 0，此功能关闭
gcInactiveDestinations，true 表示删除回收闲置的队列，默认为 false
inactiveTimoutBeforeGC = 600000，表示当队列或主题闲置 10 分钟后被删除，默认为 60 秒。





