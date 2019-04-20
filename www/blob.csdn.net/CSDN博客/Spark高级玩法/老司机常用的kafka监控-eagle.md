# 老司机常用的kafka监控-eagle - Spark高级玩法 - CSDN博客
2018年10月11日 00:02:44[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：298
前面有文章说到了一个叫kafka manager的kafka管理工具，这个工具管理kafka确实很强大，但是没有安全认证，随便都可以创建，删除，修改topic，而且告警系统，流量波动做的不好。所以，在这里浪尖，再给大家推荐一款kafka 的告警监控管理工具，kafka-eagle。
kafka-eagle主要是有几个我们关注 但kafkamanager不存在的点，值得一提：
- 
流量，最长可以查看最近七天的流量波动图
- 
lag size邮件告警
- 
可以用kafkasql分析
**1.下载解压**
直接下载压缩包
http://download.smartloli.org/
源码地址
https://github.com/smartloli/kafka-eagle
建议直接下载压缩包，而不是使用源码编译，编译过程中容易出错。有兴趣的可以下载源码阅读一下。
解压
tar -zxf ~/Downloads/kafka-eagle-bin-1.2.4.tar.gz -C ./
给文件加充命名为kafka-eagle。
**2. 配置**
配置环境变量。
vi /etc/profile
export KE_HOME=/opt/hadoop/kafka-eagle
export PATH=$PATH:$KE_HOME/bin
**3. 配置system-config.properties**
浪尖这里运行了多个kafka版本，以此测试多版本多集群，具体配置方式可以参考下面配置。
######################################
# multi zookeeper&kafka cluster list
######################################
kafka.eagle.zk.cluster.alias=cluster1,cluster2
cluster1.zk.list=localhost:2181/kafka010
cluster2.zk.list=localhost:2181/kafka082
######################################
# zk client thread limit
######################################
kafka.zk.limit.size=25
######################################
# kafka eagle webui port
######################################
kafka.eagle.webui.port=8048
######################################
# kafka offset storage
######################################
cluster1.kafka.eagle.offset.storage=kafka
cluster2.kafka.eagle.offset.storage=zookeeper
######################################
# enable kafka metrics
######################################
kafka.eagle.metrics.charts=false
######################################
# alarm email configure
######################################
kafka.eagle.mail.enable=false
kafka.eagle.mail.sa=alert_sa
kafka.eagle.mail.username=alert_sa@163.com
kafka.eagle.mail.password=mqslimczkdqabbbh
kafka.eagle.mail.server.host=smtp.163.com
kafka.eagle.mail.server.port=25
######################################
# delete kafka topic token
######################################
kafka.eagle.topic.token=keadmin
######################################
# kafka sasl authenticate
######################################
kafka.eagle.sasl.enable=false
kafka.eagle.sasl.protocol=SASL_PLAINTEXT
kafka.eagle.sasl.mechanism=PLAIN
######################################
# kafka jdbc driver address
######################################
#kafka.eagle.driver=com.mysql.jdbc.Driver
#kafka.eagle.url=jdbc:mysql://127.0.0.1:3306/ke?useUnicode=true&characterEncoding=UTF-8&zeroDateTimeBehavior=convertToNull
#kafka.eagle.username=root
#kafka.eagle.password=mt2018@#
kafka.eagle.driver=org.sqlite.JDBC
kafka.eagle.url=jdbc:sqlite:/opt/hadoop/kafka-eagle/db/ke.db
kafka.eagle.username=root
kafka.eagle.password=smartloli
**启动运行**
在启动运行之前要做下面一步，
export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_171.jdk/Contents/Home
否则，会报错误如下：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXH3osLsJJwTdf64OtnvYKiaVsickcsuqaic84Oyv9F0WIKazjiatrpu7cPvkSI7dPHJcPFIzMgEZkDIw/640)
正式启动kafka-eagle
bin/ke.sh start
启动成功之后，会有如下日志，注意红框处。![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXH3osLsJJwTdf64OtnvYKia51PRc6UrIibicfoia5BkEMicVPTrBOv9omqnVo0gYKLJ9PUb9WP4wQjBcw/640)
打开浏览器，输入上面截图红框处提示的url
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXH3osLsJJwTdf64OtnvYKiakvpNF4zHFiaYBWc4PPbFKkuHIa3YOuPuktuFz9zRaicZibwqM1PzZbtfw/640)
点击登陆之后进入
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXH3osLsJJwTdf64OtnvYKia6qk4Nk1ERElSxIuXcQUFS56vNvy05nJGKKRc51mia7MNVcvn0FrhXwg/640)
具体使用，可以参考手册
https://ke.smartloli.org/1.Overview/1.Overview.html
推荐阅读：
[最常见的Kafka面试题及答案](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485515&idx=1&sn=d1a33b86adfef9305fa22d4528f184fe&chksm=9f38eb63a84f6275b476193d011e708f7ec2a976bc1c0180606e0627e580c965d5c42f70fec2&scene=21#wechat_redirect)
[kafka管理神器-kafkamanager](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485370&idx=2&sn=406e4e799928460f9cfbafe0665b4f58&chksm=9f38e492a84f6d849fcb40eaf504cfda61daecd3f14129b4f939391bfea237b9a763cdf0db57&scene=21#wechat_redirect)
[干货|kafka流量监控的原理及实现](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485616&idx=1&sn=68b76f206dddeca045363413f783a986&chksm=9f38eb98a84f628ea8562c2a57225cedeea22159e96888705751c540ff3762282650ce8ce1e6&scene=21#wechat_redirect)
[如何获取kafka的broker保存的消费者信息？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485462&idx=1&sn=4cf0c42c5091f16a89294971c9cbd840&chksm=9f38eb3ea84f62281f6881007a3b76acd001bd61331420d7e12a11b0b7837cceaca6d17ef88c&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
