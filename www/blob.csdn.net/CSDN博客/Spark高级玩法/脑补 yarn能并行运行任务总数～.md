# 脑补|yarn能并行运行任务总数～ - Spark高级玩法 - CSDN博客
2019年04月11日 19:39:21[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：12
最近知识星球没动静主要原因是知识星球他们在做系统升级，我也很无奈，由此给球友带来的不安，深感抱歉。
前几天球友问了我一个问题：
```
请问浪总，集群400GB内存，提交了10个任务后就不能继续提交任务了，
资源还剩余300GB，CPU也很充足，完全满足新任务的资源，为啥就不能提交新任务了呢？？？
```
各位同仁也可以先思考一下可能的原因及解决方案。
估计很多人会说：
很明显，新任务申请的资源，大于了可提供的资源了～
但是这位球友说的很清楚了，剩余的资源很充足，完全可以提供新任务所需的资源。
**知识点小贴士～**
对spark on yarn研究比较多的朋友都应该发现过你明明给executor申请了1GB内存，结果发现该executor占用了yarn的2GB内存。
其实，对于spark的driver和executor在申请内存的时候有个计算公式：
```
spark.yarn.am.memoryOverhead 
除了指定的申请资源外额外申请（yarn-client模式）：
AM memory * 0.10, with minimum of 384
spark.driver.memoryOverhead 
除了指定的申请资源外，额外申请：
driverMemory * 0.10, with minimum of 384
spark.executor.memoryOverhead 
除了指定的申请资源外，额外申请：
executorMemory * 0.10, with minimum of 384
```
由于1GB*0.10才100MB，所以会是1GB+384MB<2GB，不符合预期。实际上这个还依赖于yarn的内存调度粒度。resourcemanager的参数：
最小值
<property>
<name>yarn.scheduler.minimum-allocation-mb</name>
<value>1024</value>
</property>
最大值
<property>
<name>yarn.scheduler.maximum-allocation-mb</name>
<value>20480</value>
</property>
默认yarn的调度最小单元就是1GB，所以结果就是使你原本申请1GB(+额外内存)的内存变为了2GB。
读到这里估计很多同学该说了，这个我了解但是貌似跟yarn最大并行度没什么关系呀？别急！
**重磅来袭～**
其实，yarn为了很方便控制在运行的任务数，也即是处于running状态任务的数目，提供了一个重要的参数配置，但是很容易被忽略。
<property>
<name>yarn.scheduler.capacity.maximum-am-resource-percent</name>
<value>0.1</value>
<description>      Maximum percent of resources in the cluster which can be used to run       application masters i.e. controls number of concurrent running      applications.    </description>
  </property>
配置文件是：hadoop-2.7.4/etc/hadoop/capacity-scheduler.xml
参数含义很明显就是所有AM占用的总内存数要小于yarn所管理总内存的一定比例，默认是0.1。
**也即是yarn所能同时运行的任务数受限于该参数和单个AM的内存。**
那么回归本话题，可以看看该同学所能申请的AM总内存的大小是：
400GB*0.1=40GB。
但是，该同学配置的yarn的内存调度最小单元是4GB，这样虽然他申请的任务AM每个都是1GB，但是由于调度单位是4GB，所以在这里实际内存就是4GB，刚好10个任务40GB，也就不能提交第11个任务了。
所以需要将调度内存调到默认值1GB，其实一般情况下没必要调整，然后将AM总内存占比提高，比如1，即可。
*好了，更多优良知识欢迎加入浪尖知识星球。*
推荐阅读：
[spark面试该准备点啥](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486541&idx=1&sn=bb1f5a2ccb775c84690cb5c821e43969&chksm=9f38ef65a84f6673f725e1c1ff6e8033ef5808a57c1e2ee4e4254361a451dd39e492d3827763&scene=21#wechat_redirect)
[不可不知的spark shuffle](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486590&idx=1&sn=4786718c13b49b332e23d5a700d73d32&chksm=9f38ef56a84f6640d4ed2960dc164c1772738f2b8e6f97c758a501e08b801b23a591e6e9ff8f&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
