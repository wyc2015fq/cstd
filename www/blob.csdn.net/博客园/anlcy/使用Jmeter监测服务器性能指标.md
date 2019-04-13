
# 使用Jmeter监测服务器性能指标 - anlcy - 博客园






# [使用Jmeter监测服务器性能指标](https://www.cnblogs.com/camilla/p/10043065.html)
jmeter监控服务器CPU、内存等性能参数，需要安装一些插件
插件名：JMeterPlugins-Extras，JMeterPlugins-Standard 以及ServerAgent。
下载地址：[https://jmeter-plugins.org/downloads/old/](https://jmeter-plugins.org/downloads/old/)和[https://jmeter-plugins.org/wiki/PerfMonAgent/](https://jmeter-plugins.org/wiki/PerfMonAgent/)
1.ServerAgent：
ServerAgent放在被压测的服务器上，windows服务器中启动startAgent.bat，Linux服务器启动startAgent.sh即可，是用来收集服务器相关性能指标（cpu、memory、tcp等）、以及jmx等metrics信息，然后通过 TCP or UD协议来发送给jmeter。启动后，默认监听4444端口，ServerAgent是一个文本协议，可以使用任何客户端发送/接收metrics数据信息。
在linux中启动 ./startAgent.sh 是，可能会提示：“-bash: ./startAgent.sh: 权限不够”，那么我们需要执行命令： chmod +x startAgent.sh
只需要将SeverAgent添加到服务器上然后再启动即可。
**验证**：为了保证测试过程的顺畅，我们可以先行确认JMeter压力机与被测服务器上部署的ServerAgent的通信是否正常。一个简便的方法是在JMeter压力机使用telnet像ServerAgent发送”test”，如telnet 192.168.18.10 4444，连通后，输入test，正常情况下ServerAgent会输出类似INFO 2017-07-29 23:10:52.430 [kg.apc.p] (): Yep, we received the 'test' command的日志。
2.将JMeterPlugins-Extras，JMeterPlugins-Standard的jar包拷贝到jmeter的lib/ext目录下面，1.4版本的插件要再jdk1.6以及jmeter2.4以上版本使用。
3.重启jmeter发现多了很多组件，常用组件说明：
a）jp@gc - Bytes Throughput Over Time:不同时间吞吐量展示（图表）
聚合报告里，Throughput是按请求个数来展示的，比如说1.9/sec，就是每s发送1.9个请求；而这里的展示是按字节Bytes来展示的图表
b）jp@gc - Composite Graph： 混合图表
在它的Graphs里面可以设置多少个图表一起展示，它可以同时展示多个图表
c）jp@gc - Hits per Second：每秒点击量
d）jp@gc - PerfMon Metrics Collector：服务器性能监测控件，包括CPU，Memory，Network，I/O等等
e）jp@gc - Reponse Latencies Over Time：记录客户端发送请求完成后，服务器端返回请求之前这段时间
f）jp@gc - Reponse Times Distribution： 显示测试的响应时间分布，X轴显示由时间间隔分组的响应时间，Y轴包含每个区间的样本数
g）jp@gc - Transactions per Second： 每秒事务数，服务器每秒处理的事务数
平常我们主要用到jp@gc - PerfMon Metrics Collector组件，在jmeter的添加>监听器中，可以选在perfMon选项可以选择到。
其中，HostIP为被监控服务器的IP，Port为端口号，默认为4444，Metric parameter则是具体的监控设置指标（CPU/内存/磁盘IO/网络IO/TCP/SWAP分区等性能指标）。
指标结果分析：纵轴为性能结果指标，单位为百分比，横轴为运行时间，单位为秒。结果图表可以导出成csv文件。
性能结果图表中的setting常用有：
1.use relative times用于配置曲线图x轴表示相对时间(测试开始时为0)还是实际系统时间。
2.Auto-zoom rows for best fit默认勾选，则会有上一节讲数值时提到的展示数据自动放大的功能，取消勾选则全部展示采集的实际数值。
3.Limit number of points in row to xx points：勾选后可以设定曲线图展示的采样点数量，我们的测试报告会有不同的角色查看，其中一些角色可能不具备也不需要对监控数据的细节理解能力，此时我们提供的监控曲线图应该是易读的，如果按照实际的所有采样点来渲染出曲线图，可能会有很多偏离趋势的噪点数据，这对于不了解的人来说可能会有很多疑惑，所以当我们有了分析结论，最后报告呈现的时候，可以考虑通过调整采样点，来让曲线图更好的展示资源使用趋势，消除其他不必要的信息。
4.Force maximum Y axis value to xx，实际上我更多会选择不勾选，不勾选的情况下，插件在描绘曲线图的时候，会根据数值大小自动调整Y轴最大值，以达到更佳可读性，如图7和图8，分别是不勾选，和勾选后设置最大值为100时的曲线图效果，显然图7可以更容易的观察到变化的细节。不过与上一项类似，可能在对外出具报告时，为了更少的解释说明，可能需要某个指定的数值。





