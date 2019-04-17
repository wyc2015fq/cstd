# RocketMQ 客户端公共配置文件ClientConfig解读 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月21日 15:39:48[boonya](https://me.csdn.net/boonya)阅读数：4323








## **客户端公共配置**

DefaultMQProducer、 TransactionMQProducer、 DefaultMQPushConsumer、 DefaultMQPullConsumer 都继承于ClientConfig类，ClientConfig为客户端的公共配置类。

客户端的配置都是 get、 set 形式，每个参数都可以用 spring来配置，也可以在代码中配置，例如namesrvAddr返个参数可以返样配置，其他参数同理。



```java
producer.setNamesrvAddr("192.168.0.1:9876");
```


**ClientConfig客户端的公共配置参数说明：**


![](https://img-blog.csdn.net/20170321154701320)

![](https://img-blog.csdn.net/20170321154715711)


## ClientConfig源文件

**com.alibaba.rocketmq.client.ClientConfig.java**

```java
/**
 * Copyright (C) 2010-2013 Alibaba Group Holding Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.alibaba.rocketmq.client;

import com.alibaba.rocketmq.common.MixAll;
import com.alibaba.rocketmq.common.UtilAll;
import com.alibaba.rocketmq.remoting.common.RemotingUtil;


/**
 * Client Common configuration
 *
 * @author shijia.wxr<vintage.wang@gmail.com>
 * @author von gosling<fengjia10@gmail.com>
 * @since 2013-7-24
 */
public class ClientConfig {
    private String namesrvAddr = System.getProperty(MixAll.NAMESRV_ADDR_PROPERTY,
            System.getenv(MixAll.NAMESRV_ADDR_ENV));
    private String clientIP = RemotingUtil.getLocalAddress();
    private String instanceName = System.getProperty("rocketmq.client.name", "DEFAULT");
    private int clientCallbackExecutorThreads = Runtime.getRuntime().availableProcessors();
    /**
     * Pulling topic information interval from the named server
     */
    private int pollNameServerInteval = 1000 * 30;
    /**
     * Heartbeat interval in microseconds with message broker
     */
    private int heartbeatBrokerInterval = 1000 * 30;
    /**
     * Offset persistent interval for consumer
     */
    private int persistConsumerOffsetInterval = 1000 * 5;

    public String buildMQClientId() {
        StringBuilder sb = new StringBuilder();
        sb.append(this.getClientIP());

        sb.append("@");
        sb.append(this.getInstanceName());

        return sb.toString();
    }

    public void changeInstanceNameToPID() {
        if (this.instanceName.equals("DEFAULT")) {
            this.instanceName = String.valueOf(UtilAll.getPid());
        }
    }

    public void resetClientConfig(final ClientConfig cc) {
        this.namesrvAddr = cc.namesrvAddr;
        this.clientIP = cc.clientIP;
        this.instanceName = cc.instanceName;
        this.clientCallbackExecutorThreads = cc.clientCallbackExecutorThreads;
        this.pollNameServerInteval = cc.pollNameServerInteval;
        this.heartbeatBrokerInterval = cc.heartbeatBrokerInterval;
        this.persistConsumerOffsetInterval = cc.persistConsumerOffsetInterval;
    }

    public ClientConfig cloneClientConfig() {
        ClientConfig cc = new ClientConfig();
        cc.namesrvAddr = namesrvAddr;
        cc.clientIP = clientIP;
        cc.instanceName = instanceName;
        cc.clientCallbackExecutorThreads = clientCallbackExecutorThreads;
        cc.pollNameServerInteval = pollNameServerInteval;
        cc.heartbeatBrokerInterval = heartbeatBrokerInterval;
        cc.persistConsumerOffsetInterval = persistConsumerOffsetInterval;
        return cc;
    }

    public String getNamesrvAddr() {
        return namesrvAddr;
    }

    public void setNamesrvAddr(String namesrvAddr) {
        this.namesrvAddr = namesrvAddr;
    }

    public String getClientIP() {
        return clientIP;
    }

    public void setClientIP(String clientIP) {
        this.clientIP = clientIP;
    }

    public String getInstanceName() {
        return instanceName;
    }

    public void setInstanceName(String instanceName) {
        this.instanceName = instanceName;
    }

    public int getClientCallbackExecutorThreads() {
        return clientCallbackExecutorThreads;
    }

    public void setClientCallbackExecutorThreads(int clientCallbackExecutorThreads) {
        this.clientCallbackExecutorThreads = clientCallbackExecutorThreads;
    }

    public int getPollNameServerInteval() {
        return pollNameServerInteval;
    }

    public void setPollNameServerInteval(int pollNameServerInteval) {
        this.pollNameServerInteval = pollNameServerInteval;
    }

    public int getHeartbeatBrokerInterval() {
        return heartbeatBrokerInterval;
    }

    public void setHeartbeatBrokerInterval(int heartbeatBrokerInterval) {
        this.heartbeatBrokerInterval = heartbeatBrokerInterval;
    }

    public int getPersistConsumerOffsetInterval() {
        return persistConsumerOffsetInterval;
    }

    public void setPersistConsumerOffsetInterval(int persistConsumerOffsetInterval) {
        this.persistConsumerOffsetInterval = persistConsumerOffsetInterval;
    }

    @Override
    public String toString() {
        return "ClientConfig [namesrvAddr=" + namesrvAddr + ", clientIP=" + clientIP + ", instanceName="
                + instanceName + ", clientCallbackExecutorThreads=" + clientCallbackExecutorThreads
                + ", pollNameServerInteval=" + pollNameServerInteval + ", heartbeatBrokerInterval="
                + heartbeatBrokerInterval + ", persistConsumerOffsetInterval="
                + persistConsumerOffsetInterval + "]";
    }
}
```




上面的配置基本上是基于系统属性文件的配置，值得关注的几个方法是：

```java
public void resetClientConfig(final ClientConfig cc)；  //重新设置公共客户端配置

 public ClientConfig cloneClientConfig()；  //克隆一个公共客户端配置

 public String toString()；  //重写的toString方法可能会在其他地方被使用
```


## 自定义一个ClientConfig

**定义一个RMQClientConfig.java**




```java
package com.github.boonya.rocketmq;

import com.alibaba.rocketmq.client.ClientConfig;

/**
 * RocketMQ单例配置实现
 * 
 * @package com.github.boonya.rocketmq.RMQClientConfig
 * @date 2017年3月21日 下午12:55:38
 * @author pengjunlin
 * @comment
 * @update
 */
public class RMQClientConfig {

    private RMQClientConfig() {

    }

    private static class Singleton {
        
        private static ClientConfig clientConfig = new ClientConfig();
        
        static {
            // 客户端本机 IP 地址，某些机器会发生无法识别客户端IP地址情况，需要应用在代码中强制指定
            clientConfig.setClientIP("192.168.28.94");
            // Name Server 地址列表，多个 NameServer 地址用分号 隔开
            clientConfig.setNamesrvAddr("192.168.28.94:9876;192.168.28.95:9876;");
            // 客户端实例名称，客户端创建的多个 Producer、 Consumer 实际是共用一个内部实例（这个实例包含
            // 网络连接、线程资源等）,默认值:DEFAULT
            clientConfig.setInstanceName("DEFAULT");
            // 通信层异步回调线程数,默认值4
            clientConfig.setClientCallbackExecutorThreads(10);
            // 轮询 Name Server 间隔时间，单位毫秒,默认：30000
            clientConfig.setPollNameServerInteval(30000);
            // 向 Broker 发送心跳间隔时间，单位毫秒,默认：30000
            clientConfig.setHeartbeatBrokerInterval(30000);
            // 持久化 Consumer 消费进度间隔时间，单位毫秒,默认：5000
            clientConfig.setPersistConsumerOffsetInterval(10000);
        }
    }

    /**
     * 获取一个配置实例
     * 
     * @MethodName: getInstance 
     * @Description: 
     * @return
     * @throws
     */
    public ClientConfig getInstance() {
        return Singleton.clientConfig;
    }
    
    /**
     * 克隆一个客户端配置
     */
    public ClientConfig clone() {
        return Singleton.clientConfig.cloneClientConfig();
    }
    
    /**
     * 重置客户端配置
     * 
     * @MethodName: reset 
     * @Description: 
     * @param clientConfig
     * @throws
     */
    public void reset(ClientConfig clientConfig) {
         Singleton.clientConfig.resetClientConfig(clientConfig);;
    }

    /**
     * 重写toString方法
     */
    @Override
    public String toString() {
        return  Singleton.clientConfig.toString();
    }
    
    

}
```


注：上面的参数都可以从表格中见到，这是一个非常基础的设置。








