
# Jenkins实践基础：JNLP方式搭建Master-Slave分布式构建环境 - 知行合一 止于至善 - CSDN博客

2019年01月12日 07:12:31[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：312所属专栏：[DevOps系列之：持续构建与集成](https://blog.csdn.net/column/details/27057.html)



![在这里插入图片描述](https://img-blog.csdn.net/20160809065608330)
Jenkins支持多种方式实现分布式构建，主要方式之一为传统的一主多从（Master/Slave）。Master主要用于作业（job）的分配以及用户操作的交互等，而slave则用于处理实际构建作业等，在集中化的构建需要较高性能支持以及横向扩展性的时候，Master-Slave方式构建Jenkins持续集成环境是一个常见的选择。这篇文章主要介绍一下如何使用JNLP方式的Slave与Jenkins的Master进行沟通的环境构建。
[
](https://img-blog.csdn.net/20160809065608330)
# Master-Slave构成
[
](https://img-blog.csdn.net/20160809065608330)Master节点和Slave节点构成如下主从结构，下图为Master下连接两个Slave节点的示例图。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190109194001253.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 搭建Master/Slave方式的Jenkins环境
## Step 1: 启动Jenkins的Master节点
普通方式启动Jenkins即可，本文使用Easypack中集成的alpine的Jenkins镜像，只需要将JENKINS_MODE设定为master即可，详细可参看下文：
[https://blog.csdn.net/liumiaocn/article/details/86098125](https://blog.csdn.net/liumiaocn/article/details/86098125)
## Step 2: 创建Slave节点
在Jenkins的Master节点上创建Slave节点，选择Manage Jenkins -> Manage Nodes
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190109195512794.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
选择New Node，选择Permanent Agent，设定Node名称为agent001
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190109195631693.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
设定：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112064516670.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
agent必须设定的除了Name还有Remote root direcotry，设定一个路径与此agent进行关联，最好是绝对路径，比如/var/jenkins。此路径应该是agent机器上的一个目录，没有必要在master上可见。
一般来说，agent不会保存中药的数据信息，所有的job配置和构建日志以及构建结果都保存在master上，所以此目录可以使用一个临时目录，但是最好保证机器重启也不会对其产生影响，以避免诸如使用cache的功能产生影响。当然也可以使用相对目录，这样需要结合工作目录来使用。本文示例仅仅设定必须选项，设定后信息如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112065337928.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
成功创建之后在管理的节点列表中可以看到除了master之外，添加了新的节点agent001，但此时还是不可用状态。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112065454284.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
确认一下agent001的详细信息，可以看到支持如下两种方式启动slave节点：
[
](https://img-blog.csdnimg.cn/20190112065454284.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)方式1: 通过浏览器来启动
方式2: 通过命令行来启动
因为后续需要进行进一步的自动化，因此本文此处直接使用命令行的方式来启动。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112065934709.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
Step 3：启动slave节点
在agent所在机器上创建相关目录
[root@platform slave]\# ls
docker-compose.yml
[root@platform slave]\# mkdir -p /tmp/agent/jenkins
[root@platform slave]\#设定启动参数如下：
[root@platform slave]\# cat docker-compose.yml 
version: '2'
services:
  \# jenkins service based on Jenkins LTS version
  jenkins:
    image: liumiaocn/jenkins:2.150.1
    environment:
      - JENKINS_MODE=slave
      - JENKINS_MASTER_URL=http://192.168.163.118:32002/computer/agent001/slave-agent.jnlp
      - JENKINS_SLAVE_SECRET=48de37b3bb2cbf61b2ff7eca62b692a436f54d6bf930e4e6a6f8b1d5f64fa4be
      - JENKINS_SLAVE_WORKDIR=/tmp/agent/jenkins
    volumes:
      - ./data/:/data/jenkins
    restart: "no"
[root@platform slave]\#启动jenkins的slave节点
[root@platform slave]\# docker-compose up -d
Creating network "slave_default" with the default driver
Creating slave_jenkins_1 ... 
Creating slave_jenkins_1 ... done
[root@platform slave]\# docker-compose ps
     Name                    Command               State          Ports        
------------------------------------------------------------------------------
slave_jenkins_1   /bin/tini -- /usr/local/bi ...   Up      50000/tcp, 8080/tcp 
[root@platform slave]\#结果确认，此时刷新一下agent001的页面，即可看到成功连接的信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112070926177.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
整体管理的节点也被添加了进来，因为缺省的ofexecutor是1，所以此节点最大并行度的job为1，可根据cpu的个数进行实际设定。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112071107119.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
这样则可以根据实际需要进行slave节点添加，以适应不断增大的并行构建的速度需求。
[
](https://img-blog.csdnimg.cn/20190112071107119.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)参考文档
[
](https://img-blog.csdnimg.cn/20190112071107119.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)Arnold Okoth, Joseph Muli 著. Jenkins Fundamentals. 2018-08
[https://github.com/jenkinsci/docker](https://github.com/jenkinsci/docker)
[
            ](https://img-blog.csdnimg.cn/20190112071107119.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

[
  ](https://img-blog.csdnimg.cn/20190112070926177.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)