# Ubuntu server 15.0.4安装Redis cluster - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年10月26日 11:11:26[boonya](https://me.csdn.net/boonya)阅读数：2214








#### 1.安装redis

        目前redis 3.0以上版本都支持集群（cluster）了，安装包可以到官网上去下载：[http://redis.io/download](http://redis.io/download)。在安装集群之前，必先安装redis（本文以redis-3.0.3为例），安装过程可以参考我的另一篇文章：[Ubuntu
 server 15.0.4安装Redis](http://blog.csdn.net/boonya/article/details/47808339)

#### 2.安装redis nodes

实现目标：新建6个instance，3个master，3个slave；

##### 2.1在redis-3.0.3路径下新建集群文件夹cluster
`# mkdir cluster`
![](https://img-blog.csdn.net/20151023163554215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


##### 2.2在cluster路径下创建集群节点目录
`# mkdir 7000 7001 7002 7003 7004 7005`
![](https://img-blog.csdn.net/20151023164026613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这里我们创建了6个集群实力的节点目录备用。

##### 2.3将redis-3.0.3文件夹下的redis.conf配置文件拷贝到cluster路径下

# cp redis.conf ./cluster/redis.conf

![](https://img-blog.csdn.net/20151023165502247?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

##### 2.4使用vi命令编辑cluster下的redis.conf文件

**打开文件编辑：**
`#sudo  vi  redis.conf`这里可以使用最简单的vi命令来实现编辑和保存修改。

（1）、i   光标前插入编辑；

（2）、按Esc停止编辑状态；

（3）、使用：wq保存并退出文件编辑。


**在port端口出修改port值并添加cluster的配置项，如下：**

```
port 7000
cluster-enabled yes
cluster-config-file nodes.conf
cluster-node-timeout 5000
appendonly yes
```

##### 2.5将cluster路径下redis.conf文件分别移动到各个节点文件下
`# cp redis.conf ./7000/redis.conf`
![](https://img-blog.csdn.net/20151023165654219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

后面几个节点都可以这样通过cp命令将文件复制到不同的节点下。

##### 2.6分别编辑每个节点下redis.conf对应的port

操作与2.4所述类似，这里只需要改port 就可以了。


#### 3.集群安装设置

##### 3.1 使用shell窗口将多个集群节点启动
`#redis-server redis.conf`![](https://img-blog.csdn.net/20151023173114981?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
依次启动其他剩余节点，这里需要启动7000-7005的节点。

##### 3.2查看启动的redis实例
`# ps -ef|grep redis`
![](https://img-blog.csdn.net/20151023173641596?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

##### 3.3将所有启动的节点设置为集群节点

src下面的ruby脚本，建立集群： --replicas 1 的意思是每个master有1个slave。

`./redis-trib.rb create --replicas 1 127.0.0.1:7000 127.0.0.1:7001 127.0.0.1:7002 127.0.0.1:7003 127.0.0.1:7004 127.0.0.1:7005`![](https://img-blog.csdn.net/20151023174005494?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20151023174044171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


##### 3.4测试集群是否生效

使用redis-cli命令，任意选择一个节点测试：


![](https://img-blog.csdn.net/20151023174242244?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


#### 4.集群节点检查、新增和删除

##### 4.1集群节点检查
`# ./redis-trib.rb check 127.0.0.1:7000`
![](https://img-blog.csdn.net/20151023175507875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里可以看到各个节点的角色是主(master)节点还是从（slave）节点。


##### 4.2集群节点新增

先在cluster路径下创建7006，然后将7006节点实例启动：

`# ./redis-trib.rb add-node 127.0.0.1:7006 127.0.0.1:7000 `
![](https://img-blog.csdn.net/20151023180540475?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

##### 4.3集群节点删除

这里以7006节点为例，先查询节点的信息：

![](https://img-blog.csdn.net/20151023175740653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

找到7006M所示的base64字符（M master节点的ID，S slave节点的ID），即上图红色区域值：

![](https://img-blog.csdn.net/20151023181120675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

再次chek，集群已经把7006节点移除了。

![](https://img-blog.csdn.net/20151023181521856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


#### 5.安装过程中遇到的问题QA

##### 5.1 不能执行集群安装命令

![](https://img-blog.csdn.net/20151023181648504?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注：这是因为没有安装ruby环境。

##### 5.2ruby环境安装

![](https://img-blog.csdn.net/20151026110824556?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


apt命令不能安装ruby，如果linux安装很久没用的情况下，需要执行sudo apt-get update。

![](https://img-blog.csdn.net/20151026110551951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：sudo apt-get update作用是更新Linux系统资源访问连接（主要是deb包）。

执行安装ruby出现以下错误：


![](https://img-blog.csdn.net/20151023182100269?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20151023182133084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注意：这里直接用sudo gem intall redis 资源请求地址会报错，可以使用淘宝的gemsruby资源库（https://ruby.taobao.org/），基于 HTTP 协议的镜像服务, 请在配置中使用 HTTPS 协议代替。

##### 5.3验证ruby环境

![](https://img-blog.csdn.net/20151026110950744?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


#### 6.相关参考资料

Redis集群官方英文资料：http://redis.io/topics/cluster-tutorial

Redis集群官方中文资料：http://www.redis.cn/topics/cluster-tutorial.html


ruby + gem 常用命令：http://www.2cto.com/os/201307/227238.html

RubyGems 镜像 - 淘宝网：https://ruby.taobao.org/


通过redis-trib.rb脚本构建并增删改查redis cluster集群：http://www.tuicool.com/articles/bUVRNn2

#### 7.研究方向

##### 7.1 集群在真实应用中的局限性（限制和问题）

待续。。。。。。。


##### 7.2简化使用集群操作

![](https://img-blog.csdn.net/20151023174948710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


集群的启动、停止，是否能通过一个命令来实现呢？英文文档上说了有，但是没测试成功，后续继续研究之，如果有深入研究的欢迎一起讨论！




