# 配置WebLogic Server 11g 集群、Session复制（Windows） - 零度的博客专栏 - CSDN博客
2016年04月25日 14:06:10[零度anngle](https://me.csdn.net/zmx729618)阅读数：1329
- 
# 预备知识
什么是 **Domain**，**Server**，**Machine**，**Cluster**
![](http://img1.ph.126.net/-OWVHvJE_fI4svurkHba4Q==/6597659104890922630.jpg)
图1
**Domain**
Domain 是WebLogic Server 实例的基本管理单元。所谓Domain 就是，由配置为
Administrator Server 的WebLogic Server 实例管理的逻辑单元，这个单元是有所有相关资源
的集合。
**为什么用域？**
域管理的特征：
- 对应用来说是透明的
- 可以出于技术或业务的理由来配置、管理
WLS(WebLogic Server)域可以用来分离：
- 开发/测试/上线的应用
- 管理和操作的任务
- 组织或业务分配
**Server**
Server 是一个相对独立的，为实现某些特定功能而结合在一起的单元。
服务器是执行在单一Java虚拟机（JVM）中weblogic.Server类的实例。
最多和一个WLS机器关联，占用一定数量的RAM，是多线程的。
**Machine**
Machine 是可以对应到服务器所在的物理硬件,可以是Unix 或non-Unix 类型，可以用来远程管理和监控，用于加强fail over 管理。
**Cluster**
Cluster 也是一个逻辑概念，用来分组用途相同的服务器实例，一个域中可以有多个集
群。集群用来实现负载均衡和容错。
WebLogic集群技术指通过一组服务器协同工作，在多台机器间复制应用表示层和应用逻辑层的能力，实现关键业务系统的负载分布，消除个别故障点。
**Domain and Server **的关系
一个 Domain 可以包含一个或多个WebLogic Server 实例，甚至是Server 集群。一个
Domain 中有一个且只能有一个Server 担任管理Server 的功能，其它的Server 具体实现一个特定的逻辑功能。
- 
# 本文环境
平台：Windows XP SP3, 4G RAM，Windows 2003 Enterprise，2GRAM
软件：WebLogic Server 11gR1
- 
# 配置说明
- 
## 集群定义
一个集群就是一组协同工作的WebLogic服务器实例。
集群提供：高可用性、负载均衡、扩展性。
集群图示：图2、图3都是集群在应用环境中的示例。
![](http://img2.ph.126.net/uSuKl82xPGMdi6s1wKZPGA==/6597874609169965541.jpg)
图2
![](http://img2.ph.126.net/TfI0BSjt-d-0PtnMO5JM1Q==/2050826680414148615.jpg)
图3
- 
## 集群的好处
- 扩展性
扩展性就是可以为应用提供更多容量的能力，在这种情况下，增加额外的服务器，而不必对主体架构进行修改。
- 高可用性
高可用性确保了集群中一台服务器发生故障时，其他服务器可以接管工作，因此对客户端没有影响。
- 
## 关键功能
- 应用故障接管
当应用中一个执行任务的对象变得不可用时，另外对象可以接管并完成工作。
- 站点故障接管
当单个站点上所有服务和应用都失败时，它们可以切换到其他站点，继续处理。
- 服务器迁移
当一台服务器失败时，钉住的服务可以迁移到集群中其他服务器上。
- 负载均衡
跨越多个服务器的平均任务分发与通讯。
- 
## 集群架构
- 通常的，应用被分隔成多个层次，每层代表了不同的功能，WebLogic对Web层、展现层、业务或对象层都提供集群支持。
- 
好架构某种程度上是主观的，但是有一些全局的考虑
- 性能
- 有效的状态持久性
- 最佳的负载均衡
- 有效的故障接管
- 可靠的通讯
- 
有两种主要集群架构可以选择
- 基本集群架构
基本集群架构将静态HTTP，展现逻辑，业务逻辑和对象组合在一个集群中。如图4所示。（本文配置采用的是该结构）
![](http://img2.ph.126.net/pNXtCs861-1qNJiVHGhZNQ==/2525956441019473283.jpg)
图4
- 多层集群架构
Web层和业务逻辑服务可以分布在两个集群中。如图5所示。
![](http://img2.ph.126.net/WzEP6NvtKNihtV7FZapzRg==/6599278685516892566.jpg)
图5
- 何时使用多层集群架构
多层集群架构推荐给具有如下需求的Web应用:
- 对集群化EJB方法调用的负载均衡
- 在提供HTTP内容的服务器与提供集群对象的服务器之间实现负载均衡灵活性
- 高可用性（更少的单点故障）
- 更灵活的安全性
- 
基本集群架构的优劣
基本集群架构具有如下优势：
- 易于管理
- 灵活的负载均衡
- 强壮的安全性
基本集群架构具有如下劣势：
- 不能负载均衡EJB方法调用
- 跨层的负载均衡可能变得不平衡
- 
多层集群架构的优劣
多层集群架构具有如下优势：
- 改进的负载均衡
- EJB方法的负载均衡
- 更高可用性
- 改进的安全选项
多层集群架构具有如下劣势：
- 当展现层频繁调用业务逻辑时会制造瓶颈
- 增加了防火墙配置复杂性
- 
## 配置集群应用的必要条件
- 集群中的所有 Server 必须位于同一网段，并且必须是IP 广播(UDP)可到达的
- 集群中的所有 Server 必须使用相同的版本,包括Service Pack
- 
集群中的 Server 必须使用永久的静态IP 地址。动态IP 地址分配不能用于集群环境。如果服务器位于防火墙后面，而客户机位于防火墙外面，那么服务器必须有公共的静态IP 地址，只有这样，客户端才能访问服务器
- 
## 配置前的准备工作
在配置集群应用前要对集群的配置信息有一个良好的设计，下面就是我们这次配置的集
群信息：在两台台机器上配置集群（每台机上都要安装相同版本的WebLogic 11g 产品）。见表1。
提示：建议您的测试服务器有4G 以上的内存，否则服务器启动过程将会很漫长。
|机器类型|操作系统|网络配置|角色|备注|
|----|----|----|----|----|
|Lenovo PC|Windows XP SP3|IP: 172.20.50.96PORT:7001|Administrator Server|管理服务器|
|Lenovo PC|Windows XP SP3|IP: 172.20.50.96PORT:8080|Proxy Server|代理服务器|
|Lenovo PC|Windows XP SP3|IP: 172.20.50.96PORT:7003|Managed Server|受管理服务器|
|Lenovo PC|Windows XP SP3|IP: 172.20.50.96PORT:7004|Managed Server|受管理服务器|
|Lenovo PC|Windows XP SP3|IP: 172.20.50.96PORT:7005|Managed Server|受管理服务器|
|Lenovo PC|Windows 2003 Enterprise|IP: 172.20.50.181PORT:7006|Managed Server|受管理服务器|
|Lenovo PC|Windows 2003 Enterprise|IP: 172.20.50.181PORT:7007|Managed Server|受管理服务器|
表1
- 
# 配置WebLogic Server 集群
- 
## 主控服务器配置
主控服务器在安装Window Xp的PC机下配置。
- 
- 
- 
- 
- 
- 
### 使用 Configuration Wizard 进行配置
选择"「开始」菜单à程序àOracle WebLogicàWebLogic
 Server 11gR1àToolsàConfiguration Wizard"
![](http://img2.ph.126.net/ggIUqYtB_Q4pkXtMiYLigA==/2114440025150771813.jpg)
图6
- 
### 创建新的 Domain
选择"创建新的Weblogic域"，单击"下一步"按钮。
![](http://img2.ph.126.net/z1ADGQaWqxc11JT8jEef_g==/3179259862046981561.jpg)
图7
- 
### 选择安装域源
默认选择安装"Basic WebLogic Server Domain", 单击"下一步"按钮。
![](http://img0.ph.126.net/NGUZbAvsRlfC4xijy5Livw==/709879891364515410.jpg)
图8
- 
### 指定域的名称和位置
输入要创建的域名和位置，此处域名输入为 MyCluster_Domain，输入完成后单击"下一步"按钮。本文安装位置为E:\study\WebLogic11g\user_projects\domains目录下。
![](http://img0.ph.126.net/vDdQKM5Fwfh81ae4bl1u8A==/3294664602498664920.jpg)
图9
- 
### 配置管理员用户名和密码
输入 WebLogic 管理员管理服务器（AdminServer）用户名和密码，在此以"weblogic"作用登录管理服务器的用户名，密码为"weblogic1"，单击"下一步"按钮。
![](http://img1.ph.126.net/7-v1H5nF0EACm-XHimPPXg==/6597236892426184564.jpg)
图10
- 
### 配置域启动模式和JDK
选择"生产模式"和"JRockit SDK"后，单击"下一步"按钮。
![](http://img1.ph.126.net/5Hz9Z0CYCqyeIqsop_oKkA==/6597605228821157320.jpg)
图11
- 
### 选择可选配置
选择"管理服务器"，"受管服务器、群集和计算机"后，单击"下一步"按钮。
![](http://img1.ph.126.net/F3Jgdv3E4YYdnXvSydwOjg==/2219711666440544167.jpg)
图12
- 
### 配置管理服务器
输入 Administrator Server 名称，监听地址（可选所有IP或者指定IP），监听端口，如果需要SSL支持的话可以在"SSL
 enabled"后面的复选取框上打勾，配置SSL 监听端口，单击"下一步"按钮。
![](http://img1.ph.126.net/MUedj70SsJjYFtqkrR_Ltg==/3242028781853677357.jpg)
图13
- 
### 配置受管理服务器
**注意：代理服务器实际上也是一个受管理服务器**
点击"添加"，输入 Managed Server 的名称，监听地址，监听端口，如果需要SSL 支持的话，可以在"SSL
 enabled"复选框上打勾，配置SSL 监听端口。
可以配置多个Managed Server，名称和端口都不要相同，如果IP地址在选择的下拉框中没有，可以手动填写，完成后单击"下一步"按钮。
![](http://img0.ph.126.net/aHP4AnRAfX3Dj5fyF-b3Iw==/1631428965115035306.jpg)
图14
- 
### 配置群集信息
单击"添加"，输入Cluster 的名称My_Cluster_1，Multicast address 组播地址（用来进行集群间的状态通信）和端口7777，Cluster 的各个服务器地址和端口（Cluster 地址可输可不输，本文进行了输入，地址为："172.20.50.96:7003,172.20.50.96:7004,172.20.50.96:7005,172.20.50.181:7006,172.20.50.181:7007"）。完成后单击"下一步"按钮。
**提示：当用Configuration Wizard 创建集群时，您只能选择multicast，虽然官方文档推荐新建集群时使用unicast 消息机制；若选择unicast，您只能在Administration Console 中修改集群的配置，参考Oracle Fusion Middleware Oracle WebLogic Server Administration Console 帮助文档中的："Create and configure clusters".**
**提示： multicast address 的IP 范围必须位于224.0.0.0 到 239.255.255.255 之间.**
**WebLogic Server 使用的multicast 默认值为 239.192.0.0. 您不能使用类似于x.0.0.1的多播地址.**
![](http://img2.ph.126.net/HlGxOVNc8tO95uDIqZkA0g==/6598250642146594541.jpg)
图15
- 
### 将"受管理服务器"添加到"集群"中
选择左面列表中的Managed Server，将其添加到右面的列表中，注意这里代理服务器Proxy_Server 不要添加到右边，之后单击"下一步"按钮。
![](http://img2.ph.126.net/XhJlWIwkd4wMRarhTkP01w==/6597957072542048423.jpg)
图16
- 
### 创建HTTP 代理应用程序
选中"创建HTTP 代理"复选框，My_Cluster_1 代理服务器选择Proxy_Server，单击"下一步"按钮。
![](http://img2.ph.126.net/9wyq6pksA0Nzc8P7iZPGUQ==/6597669000495568619.jpg)
图17
- 
### 配置计算机
点击"添加"，输入计算机的名称，监听地址，监听端口，单击"下一步"按钮。
![](http://img2.ph.126.net/QTi0EYy3Mu8IvKgMr-HKIw==/3320278825379142152.jpg)
图18
- 
### 配置向计算机分配服务器
选择左面列表中的Admin Server、Managed Servers、Proxy
 Server，将其添加到右面的计算机下，之后单击"下一步"按钮。
![](http://img0.ph.126.net/vK-9tqcpiHUJgDf0GcoNIg==/6597490879610714745.jpg)
图19
- 
### 检查WebLogic 域配置
在此检查前面所做的配置，如果没有问题单击"创建"按钮。
![](http://img0.ph.126.net/Hou4z1n1pLOKs3y4AgEZJQ==/1834935373277119731.jpg)
图20
- 
### 正在创建域
创建无误后，单击"完成"按钮
![](http://img0.ph.126.net/emFyrx3V5UP-CfWxg8bDRQ==/2153283571936850940.jpg)
图21
- 
### 编辑WebLogic 服务启动文件
- **编辑口令文件**
在刚才创建的域目录下建立 **boot.properties **文件，内容为：
username=weblogic
password=weblogic1
在本文的机器上，该文件的存储位置是：
E:\study\WebLogic11g\user_projects\domains\MyCluster_Domain\boot.properties。
- **编辑启动管理服务器的文件(可选，可直接运行startWebLogic.cmd 即可)**
在刚才创建的域目录下的bin 目录中创建startAdminServer.cmd，内容为：
@ECHO OFF
@REM 启动管理服务器.
start .\startWebLogic
在本文的机器上，该文件的存储位置是：
E:\study\WebLogic11g\user_projects\domains\MyCluster_Domain\bin\startAdminServer.cmd。
- **编辑启动代理服务器的文件**
在刚才创建的域目录下的 bin 目录中创建startProxyServer.cmd，内容为：
@ECHO OFF
@REM 启动代理服务器.
@REM 使用startManagedWeblogic 命令启动Managed
 Server，它的命令格式是：
@REM startManagedWeblogic.cmd Managed Server 的名称 Administrator Server 地址
@REM startManagedWeblogic 受管理服务器名称管理服务器 URL
start .\startManagedWebLogic Proxy_Server http://172.20.50.96:7001
在本文的机器上，该文件的存储位置是：
E:\study\WebLogic11g\user_projects\domains\MyCluster_Domain\bin\startProxyServer.cmd。
- **编辑启动受管理服务器(ManagedServer_1)的文件**
在刚才创建的域目录下的 bin 目录中创建startManagedServer_1.cmd，内容为：
@ECHO OFF
@REM 启动受管理服务 Cluster_ManagedServer_1.
@REM 使用startManagedWeblogic 命令启动Managed
 Server，它的命令格式是：
@REM startManagedWeblogic.cmd Managed Server 的名称 Administrator Server 地址
@REM startManagedWeblogic 受管理服务器名称管理服务器 URL
start .\startManagedWebLogic Cluster_ManagedServer_1 http://172.20.50.96:7001
在本文的机器上，该文件的存储位置是：
E:\study\WebLogic11g\user_projects\domains\MyCluster_Domain\bin\startManagedServer_1.cmd。
- **编辑启动代理服务器(ManagedServer_2)的文件**
在刚才创建的域目录下的 bin 目录中创建startManagedServer_2.cmd，内容为：
@ECHO OFF
@REM 启动受管理服务 Cluster_ManagedServer_2.
@REM 使用startManagedWeblogic 命令启动Managed
 Server，它的命令格式是：
@REM startManagedWeblogic.cmd Managed Server 的名称 Administrator Server 地址
@REM startManagedWeblogic 受管理服务器名称管理服务器 URL
start .\startManagedWebLogic Cluster_ManagedServer_2 http://172.20.50.96:7001
在本文的机器上，该文件的存储位置是：
E:\study\WebLogic11g\user_projects\domains\MyCluster_Domain\bin\startManagedServer_2.cmd。
- **编辑启动代理服务器(ManagedServer_3)的文件**
在刚才创建的域目录下的 bin 目录中创建startManagedServer_3.cmd，内容为：
@ECHO OFF
@REM 启动受管理服务 Cluster_ManagedServer_3.
@REM 使用startManagedWeblogic 命令启动Managed
 Server，它的命令格式是：
@REM startManagedWeblogic.cmd Managed Server 的名称 Administrator Server 地址
@REM startManagedWeblogic 受管理服务器名称管理服务器 URL
start .\startManagedWebLogic Cluster_ManagedServer_3 http://172.20.50.96:7001
在本文的机器上，该文件的存储位置是：
E:\study\WebLogic11g\user_projects\domains\MyCluster_Domain\bin\startManagedServer_3.cmd。
（注：若使用 startManagedWebLogic 命令启动Managed
 Server，它的命令格式是：
startManagedWebLogic.cmd Managed Server 的名称Administrator
 Server 地址)
附启动文件：
![](http://img2.ph.126.net/af17qJebqZwZfYvpEqVhRQ==/757167687451786451.jpg)
- 
## 受控服务器配置
受控服务器在安装Window 2003 Enterprise的PC机下配置。
- 
### 使用 Configuration Wizard 进行配置
选择"「开始」菜单à程序àOracle WebLogicàWebLogic
 Server 11gR1àToolsàConfiguration Wizard"
![](http://img0.ph.126.net/cV0NLenRdGVw_-Kz9eO9kg==/2219711666440544184.jpg)
图22
- 
### 创建新的 Domain
选择"创建新的Weblogic域"，单击"下一步"按钮。
![](http://img2.ph.126.net/SUy5s18fqMtEGL4w_YgBKg==/721701840386413499.jpg)
图23
- 
### 选择安装域源
默认选择安装"Basic WebLogic Server Domain", 单击"下一步"按钮。
![](http://img1.ph.126.net/7RRwn0FWGvLU8HGMw1nG3A==/6598300120169849776.jpg)
图24
- 
### 指定域的名称和位置
输入要创建的域名和位置，此处域名可输入为 MyCluster_Domain，输入完成后单击"下一步"按钮。本文安装位置为D:\weblogic\weblogic11g\user_projects\domains目录下。
![](http://img0.ph.126.net/li-c3f1bJ6_GIu81c7YDAQ==/2763239846386442901.jpg)
图25
- 
### 配置管理员用户名和密码
输入 WebLogic 管理员管理服务器（AdminServer）用户名和密码，在此以"weblogic"作用登录管理服务器的用户名，密码为"weblogic1"，单击"下一步"按钮。
![](http://img2.ph.126.net/u-QOonBaUNmP2vUY0EnLcw==/1500543100944764336.jpg)
图26
- 
### 配置域启动模式和JDK
选择"生产模式"和"JRockit SDK"后，单击"下一步"按钮。
![](http://img2.ph.126.net/0jfZVOsEWm3pgcROGh1kWg==/864409653578547443.jpg)
图27
- 
### 选择可选配置
选择"管理服务器"，"受管服务器、群集和计算机"后，单击"下一步"按钮。
![](http://img1.ph.126.net/C-c_-mRNXN8Gg1HptPl2Jw==/1413004383188032087.jpg)
图28
- 
### 配置管理服务器
输入 Administrator Server 名称，监听地址（可选所有IP或者指定IP），监听端口，如果需要SSL支持的话可以在"SSL
 enabled"后面的复选取框上打勾，配置SSL 监听端口，单击"下一步"按钮。
![](http://img0.ph.126.net/PGOVvdAAU4S_sl9oODi9Pg==/6597237991937812350.jpg)
图29
- 
### 配置受管理服务器
**注意：受管理服务器配置的名称，监听地址，监听端口要和在主控服务器配置中的一致(参见4.1.9.配置受管理服务器)。**
点击"添加"，输入 Managed Server 的名称，监听地址，监听端口，如果需要SSL 支持的话，可以在"SSL
 enabled"复选框上打勾，配置SSL 监听端口。
![](http://img2.ph.126.net/2_H5P73GNIGsuneEYhBDMg==/1754433529937754395.jpg)
图30
- 
### 配置群集信息
这里不做集群配置，单击"下一步"按钮。
![](http://img0.ph.126.net/ZqWwg9AhkYf3Aa71gxMS0g==/3783868112021576823.jpg)
图31
- 
### 配置计算机
这里不做计算机配置，单击"下一步"按钮。
![](http://img0.ph.126.net/vH9W9a1GDDI8StSAzNp6Kg==/6598061526146616915.jpg)
图32
- 
### 检查WebLogic 域配置
在此检查前面所做的配置，如果没有问题单击"创建"按钮。
![](http://img1.ph.126.net/ilV7i-aED-YxrqVVp_UFtA==/1449033180206688250.jpg)
图33
- 
### 正在创建域
创建无误后，单击"完成"按钮
![](http://img0.ph.126.net/o6BtKIVHD9mAUJ74OmWO4w==/6597961470588560886.jpg)
图34
- 
### 编辑WebLogic 服务启动文件
- **编辑口令文件**
在刚才创建的域目录下建立 **boot.properties **文件，内容为：
username=weblogic
password=weblogic1
在本文的机器上，该文件的存储位置是：
D:\weblogic\weblogic11g\user_projects\domains\MyCluster_Domain\boot.properties。
- **编辑启动管理服务器的文件(可选，可直接运行startWebLogic.cmd 即可)**
在刚才创建的域目录下的bin 目录中创建startAdminServer.cmd，内容为：
@ECHO OFF
@REM 启动管理服务器.
start .\startWebLogic
在本文的机器上，该文件的存储位置是：
D:\weblogic\weblogic11g\user_projects\domains\MyCluster_Domain\bin\startAdminServer.cmd。
- **编辑启动受管理服务器(ManagedServer_4)的文件**
在刚才创建的域目录下的 bin 目录中创建startManagedServer_4.cmd，内容为：
@ECHO OFF
@REM 启动受管理服务 Cluster_ManagedServer_4.
@REM 使用startManagedWeblogic 命令启动Managed
 Server，它的命令格式是：
@REM startManagedWeblogic.cmd Managed Server 的名称 Administrator Server 地址
@REM startManagedWeblogic 受管理服务器名称管理服务器 URL
start .\startManagedWebLogic Cluster_ManagedServer_4 http://172.20.50.96:7001
在本文的机器上，该文件的存储位置是：
D:\weblogic\weblogic11g\user_projects\domains\MyCluster_Domain\bin\startManagedServer_4.cmd。
- **编辑启动代理服务器(ManagedServer_5)的文件**
在刚才创建的域目录下的 bin 目录中创建startManagedServer_5.cmd，内容为：
@ECHO OFF
@REM 启动受管理服务 Cluster_ManagedServer_5.
@REM 使用startManagedWeblogic 命令启动Managed
 Server，它的命令格式是：
@REM startManagedWeblogic.cmd Managed Server 的名称 Administrator Server 地址
@REM startManagedWeblogic 受管理服务器名称管理服务器 URL
start .\startManagedWebLogic Cluster_ManagedServer_5 http://172.20.50.96:7001
在本文的机器上，该文件的存储位置是：
D:\weblogic\weblogic11g\user_projects\domains\MyCluster_Domain\bin\startManagedServer_5.cmd。
附启动文件：
![](http://img0.ph.126.net/bCaCnAUiEZ7u_8ge5FqCkg==/6597980162286225879.jpg)
- 
## 启动WebLogic 服务
建议您按照顺序启动服务器，只有当管理服务器首先启动完毕后，受管服务器再启动。
启动的过程中，会让输入管理员用户名（weblogic）和密码（weblogic1），当所有服务器的控制台上出现Server started in RUNNING mode 字样时，整个集群启动完毕。
- 
**启动主控服务器：**
- 
双击"startAdminServer.cmd"或者"startWebLogic.cmd"启动管理服务器
![](http://img2.ph.126.net/ehVeI2EgtsR1hYky_r0yNg==/3062729221606532360.jpg)
图35
- 
双击"startProxyServer.cmd"启动代理服务器
![](http://img0.ph.126.net/JZRcFfc5YuDf7uICFl3hzA==/864409653578547471.jpg)
图36
- 
双击"startManagedServer_1.cmd"启动受管理服务器1
![](http://img2.ph.126.net/pOWg3hzbfV0jMbBcAXQ_Hg==/6608195724817684337.jpg)
图37
- 
双击"startManagedServer_2.cmd"启动受管理服务器2
![](http://img2.ph.126.net/hAlhsXmlX8mStdN425VzLg==/138485688641853905.jpg)
图38
- 
双击"startManagedServer_3.cmd"启动受管理服务器3
![](http://img2.ph.126.net/52UyqNMMBcDDJJuZ_qSW8Q==/1451284980020445440.jpg)
图39
- 
**启动受控服务器：**
- 
双击"startManagedServer_4.cmd"启动受管理服务器4
![](http://img0.ph.126.net/N0raM_Zz7g7IJoj91ycQaw==/6608206719933960930.jpg)
图40
- 
双击"startManagedServer_5.cmd"启动受管理服务器5
![](http://img2.ph.126.net/pLk7gMwt1RP9ddB9BLQX2w==/6597699786821144030.jpg)
图41
- 
**验证集群启动**
- 
在浏览器地址栏里输入[http://172.20.50.96:7001/console](http://172.20.50.96:7001/console) 地址，输入用户名weblogic和密码weblogic1，进入
 Weblogic服务器管理控制台页面。
![](http://img2.ph.126.net/xhTtiF4C1b8_sbElU6tsFA==/3049781372760226808.jpg)
图42
![](http://img1.ph.126.net/q9xdG7L94GCYyf9XsK8pBA==/1391612284957688728.jpg)
图43
- 
点击左面的域结构中的MyCluster_Domainà环境à服务器，如下图，此时在界面主区域就能看到所有server的状态，如图45
![](http://img0.ph.126.net/4A9czBPAQEP9jGmG4L2Eww==/2519201041578552801.jpg)
图44
![](http://img2.ph.126.net/hgsyX-ncGEczPn-cKNOCXw==/1264385595484428824.jpg)
图45
- 
## 将应用部署到集群上
- 
- 
- 
- 
- 
- 
- 
- 
- 
### 点击"部署"菜单
登录进控制台后点击左边"域结构"中的"部署"菜单，如下图
![](http://img1.ph.126.net/-iKOzKMSJXOzQG4vSG-87w==/837669530791057642.jpg)
图46
此时在界面主区域可以看到已部署的应用，如下图
![](http://img0.ph.126.net/v_XhszlB9cVqLtAIpWwN7g==/6598172576821026822.jpg)
图47
- 
### 锁定并编辑
点击更改中心里的"锁定并编辑"按钮，此时在界面主区域中的"安装"按钮会变成可用状态，如下图
![](http://img1.ph.126.net/EiqSl3pn2c-mhqVZyHBLlQ==/1080582435610365055.jpg)![](http://img2.ph.126.net/gHb5k4vteIXDriEUoc0vig==/197313958774481204.jpg)
图48
- 
### 开始安装
点击"安装"按钮，进入部署步骤，选择要部署的文件，点击下一步按钮
![](http://img2.ph.126.net/0ETR5m5n5cw1QcjaLAvTfQ==/1264385595484428848.jpg)
图49
按照默认设置，点击下一步按钮
![](http://img1.ph.126.net/7bqSDUtQPkrKUHA1LyvF9g==/3785556961881840775.jpg)
图50
选择集群的一部分进行安装，这里为被管理的服务器都进行了安装，点击下一步按钮
![](http://img2.ph.126.net/oFlz3pRLnqPTu1nkDL79PA==/1919377866290194919.jpg)
图51
按照默认设置，点击下一步按钮
![](http://img2.ph.126.net/e3etwwvZ5FGQQatNMWYwlg==/148337312827054697.jpg)
图52
点击完成按钮
![](http://img2.ph.126.net/pw1Xi3qd0lKwItRHOF9jWg==/6597833927239738019.jpg)
图53
点击保存按钮，界面主区域中上方显示"![](http://img2.ph.126.net/w-FUgtcF8ftr8DUbcCpEQw==/757730637405208712.jpg)设置更新成功"字样
![](http://img2.ph.126.net/x0ns9tBmOmQv9kizpEkY5Q==/2005790684140414088.jpg)
图54
点击左上角"更改中心"中的"激活更改"按钮，完成后，界面主区域中上方显示"![](http://img2.ph.126.net/w-FUgtcF8ftr8DUbcCpEQw==/757730637405208712.jpg)已激活所有更改。不需要重新启动。"消息
![](http://img0.ph.126.net/57MHSXzGWDQNOlAAwDd2ng==/845550830138932767.jpg)
图55
本文部署使用的例子：
![](http://img1.ph.126.net/Te3UPEDmK9CMK1RBhqxcCg==/6597714080472305488.jpg)
- 
### 部署成功验证
点击左面"域结构"中的"部署链接"，在主界面中会显示刚刚部署成功的"ShoppingCart"工程，选择该工程左面的checkbox，点击"启动"按钮，选择"为所有请求提供服务"
![](http://img2.ph.126.net/BK7EG7P3JwMhMrP0VSMJXA==/303992974947736412.jpg)
图56
点击"是"按钮，进入下一页面，ShoppingCart工程的状态会显示"start running"字样，过一会，再点击左边"域结构"中的"部署"菜单，ShoppingCart工程的状态会显示"活动"字样，可以点击"活动"链接查看具体部署情况。
![](http://img0.ph.126.net/QdPaJ9pekW4REYxTQMAkYw==/2147654072402539521.jpg)
图57
在浏览器中输入[http://172.20.50.96:8080/ShoppingCart/ClusterTest.jsp](http://172.20.50.96:8080/ShoppingCart/ClusterTest.jsp)，页面上会显示"Cluster
 Test!"字样。
![](http://img0.ph.126.net/m7JyVYV7f3Es5XpymfKMfA==/105271641307848807.jpg)
图58
本文在Windows 2003 Enterprise上的被管理服务器的控制台打出：Cluster Test! 在哪台被控服务器上打印，每次会不一样。
![](http://img2.ph.126.net/Tv2RO25gqQAmruMHyFWdwQ==/1858297796343870437.jpg)
图59
ClusterTest.jsp文件内容：
<HTML>
<HEAD><TITLE>Cluster Test Page</TITLE></HEAD>
<BODY>
<%
out.println("Cluster Test!");
System.out.println("Cluster Test!");
%>
</BODY>
</HTML>
- 
### 负载均衡测试
通过 Apache 中所带的ab包（只需要单个文件ab.exe 即可，本文安装了Apache2.2，ab.exe所在目录为C:\Program Files\Apache Software Foundation\Apache2.2\bin）来进行并发访问的模拟测试，使用如下的命令就可以完成压力测试。
ab.exe -n 100 -c 10 http://172.20.50.96:8080/ShoppingCart/ClusterTest.jsp
参数 n 代表请求的总数量
参数 c 代表并发的请求数
url 为要测试压力的地址
![](http://img0.ph.126.net/vHHIpkiU1ajSmn66sgAAmQ==/303992974947736422.jpg)
图60
压力测试完成后，我们从Managed Servers 的控制台上可以看到打印输出，ManagedServer_1，ManagedServer_2，ManagedServer_4 都打印出了"Cluster Test!"字样(这里ManagedServer_3，ManagedServer_5没有打出，不同的测试环境，哪个受管理服务器打印可能不同)，这说明，在并发请求的情况下，集群能够将请求进行分发，以达到负载平衡的目的。
- 
## 关闭WebLogic 服务
- 
- 
### 在控制台中关闭服务
进入控制台，在左侧"控制台"点击"服务器"链接，在界面主区域中点击"控制"tab按钮，选择要关闭的服务器，如下图。
![](http://img0.ph.126.net/T4eZ2KDi_iu7c8NqO8SfRQ==/2457276546784252383.jpg)
图61
点击"关闭"按钮，选择"当工作完成时"或者"立即强制关闭"，建议您按照顺序关闭服务器，当受管理服务器和代理服务器关闭后，再关闭管理服务器。
![](http://img2.ph.126.net/jANeYn9DrNiVvuqVjCKBEA==/2121758374545114764.jpg)
图62
下图是一个受管理服务器关闭后的控制台消息。
![](http://img2.ph.126.net/SQh15J7XBrwKHcZjKDAsxA==/6597546954704886512.jpg)
图63
- 
### 在cmd命令行下关闭服务
这里以关闭一个受管理服务器为例，关闭代理服务器和管理服务器一样，都要用到stopManagedWebLogic.cmd命令，格式为"stopManagedWebLogic.cmd 要关闭服务的名字"。打开windows的cmd命令行工具，进入172.20.50.96上的E:\study\WebLogic11g\user_projects\domains\MyCluster_Domain\bin路径，打以下命令回车：
"stopManagedWebLogic.cmd Cluster_ManagedServer_3"，中间提示输入用户名和密码，这里分别是weblogic，weblogic1。关闭后如下图：
![](http://img0.ph.126.net/4srVuatFSYXmKdpFDBlXsw==/6597156628077359157.jpg)
图64
- 
### 按下Ctrl+C强制关闭
点击正在运行的服务器控制台，按下Ctrl+C组合键，输出几行文字后，控制台会打出"终止批处理操作吗(Y/N)? "消息，输入"Y"，回车，该服务器将被强制关闭。
![](http://img1.ph.126.net/GfrnSu6TY78mRAW-YTw9hg==/6597280872890765929.jpg)
图65
- 
# Session复制
- 
## HTTP会话状态复制介绍
- 
- 
- 
### 对JSP和Servlet提供集群支持
WebLogic服务器通过复制HTTP会话状态，对JSP和Servlet提供集群支持。
- 
### 实现方式
- 内存复制
- JDBC复制
- 文件系统复制
本文只做了"内存复制"的配置。
- 
### Session复制要求条件
- 
Session必须序列化
为了支持in-memory http session复制，所有的servlet和jsp会话数据必须可序列化，实现java.io.Serializable接口。
- 
用setAttribute 修改 Session 状态
在实现了javax.servlet.http.HttpSession的http servlet中，用HttpSession.setAttribute代替putValue方法去修改session 对象。putValue方法不建议用，同样用removeAttribute代替removeValue去删除session对象。
- 
避免大的session对象
因为往session中存放的数据比较大时，系统的响应速度明显变慢，有时会出现内存溢出的情况。
- 
框架的使用
在特定的框架集（frameset）中，确保只有一个框架（frame）创建和修改会话数据；必须确保只在第一个框架集的一个框架中创建会话,其他框架集访问该session。
- 
## Weblogic配置要求
- 
- 
### 机器
WebLogic服务器中，机器名用来指明一个受管服务器运行在一个特定的硬件上。机器定义是WebLogic选择另一个服务器作为它的会话信息备份时的一个考量因素。
- 
### 复制组
复制组是集群中相关服务器的逻辑组，WLS允许你使用复制组，决定在哪里放置备份对象。WLS尝试以下方式进行session复制：
- 如果配置过，发送备份对象给预定义次要复制组
- 发送备份对象给不同机器
- 避免发送备份对象给同一个复制组中的服务器
- 
### 分级服务器
WebLogic服务器对集群中的服务器分级，以决定哪个服务器作为备份：
![](http://img2.ph.126.net/Y4Kl3srwGlm_IHi-WWZ6WQ==/1304917992048648734.jpg)图66
- 
### weblogic.xml部署描述配置
会话持久性通过weblogic.xml部署描述符文件中的<session-descriptor>元素配置的，每种持久性方法都有自己的配置参数集。
- 
![](http://img0.ph.126.net/Y2IxoztWh4utr1cCBTi8lw==/3134223865773507738.jpg)
## 内存复制方式
- 
- 
### 服务器和会话对象
WLS可以复制HttpSession对象和
有状态会话EJB，会话对象仅存在两
台服务器上，称作主要服务器和次
要服务器。次要服务器由复制组和机
器定义决定，主要对象创建后，次要
对象马上被创建。如右图所示

图67
主要服务器失败，使得备份对象成为主要对象，如下图。
![](http://img1.ph.126.net/9GoOc5wQDH2zT81LwD3LjA==/119345390225627596.jpg)
图68
- 
### 复制组故障接管
![](http://img2.ph.126.net/6kNhQGWODWn516p6Lw6ANg==/3289035102964442315.jpg)
图69
- 
### 内存复制的要求
- 来自同一个客户端的后续请求必须访问同一个主要对象
- 
为了使用HTTP会话状态内存复制，客户端必须使用下面方法之一访问集群：
- 负载均衡硬件（WLS知晓）
- Web服务器集合，或单个Web服务器，带有WebLogic代理插件（配置相同）
- 配置有HTTPClusterServlet的WebLogic服务器（本文采用方式）
- 
### 配置内存复制
- 配置代理服务器（如果适用）
- 配置复制组和/或机器（可选）
- 在weblogic.xml部署描述符中指定持久类型
<persistent-store-type>replicated_if_clustered</persistent-store-type>
- 
## 内存复制配置步骤
本文是在上面配置好集群的基础上做内存session复制，其中代理服务器和机器已经配置好了，下面主要做复制组的配置和weblogic.xml的配置。
- 
- 
### 复制组规划
复制组规划图：
![](http://img0.ph.126.net/ED5hNLRWX0XeBBsQF6_uIw==/1923318515964276774.jpg)
![](http://img0.ph.126.net/Fos1ju7pE_iPS3zRSijF-w==/6598175875355910147.jpg)
![](http://img0.ph.126.net/2HP5He1GqHvqR5OC0g31Iw==/6598175875355910153.jpg)![](http://img1.ph.126.net/UB4i7bG7YUOjnYGnNYvxpQ==/2051108155391116099.jpg)
![](http://img1.ph.126.net/KoBt3CbGCjMGYm-FYphanw==/1620732916000478524.jpg)![](http://img1.ph.126.net/TkvqmEjYPEyCLn9Lfrpc2w==/1464514303925744155.jpg)
![](http://img0.ph.126.net/YqnTQ10f7pp1An_BrvoSpg==/3050062847736979749.jpg)
![](http://img1.ph.126.net/OIv1bLYXZhft7J9QJGXcww==/2740721848249585444.jpg)
![](http://img1.ph.126.net/fdzARqW5kLPtm8HzvxzZrg==/3801882510531111577.jpg)
![](http://img0.ph.126.net/uBbfFl2mGNzz4lMe0h5WNA==/2051108155391116101.jpg)![](http://img1.ph.126.net/SGwBAHllOFuoupt6bNLHbQ==/897623700830630437.jpg)
图70
复制组和服务器的分配：
|Server|Replication Groups|Preferred secondary replication group|
|----|----|----|
|ManagedServer_1|MyGroupA|MyGroupB|
|ManagedServer_2|MyGroupA|MyGroupB|
|ManagedServer_3|MyGroupB|MyGroupA|
|ManagedServer_4|MyGroupA|MyGroupB|
|ManagedServer_5|MyGroupB|MyGroupA|
表2
如果一个客户端连接到服务器ManagedServer3并创造一个HTTP会话状态，按照WebLogic选作备份服务器对集群中服务器的分级，下面是复制该session的优先级：
优先级1：ManagedServer4，因为它们处在不同的机器上，并且它是ManagedServer3的预定义首选次级组成员。
优先级2：ManagedServer1，ManagedServer2，它是ManagedServer3的预定义首选次级组成员，虽然在一台机器上。
优先级3：ManagedServer5，它和ManagedServer3不在一台机器上，虽然同在一个复制组中。
- 
### 复制组配置
- 
启动weblogic
启动weblogic管理服务器、代理服务器。
- 
登陆[http://172.20.50.96:7001/console](http://172.20.50.96:7001/console)进入控制台，点击左边"域结构"中的"环境à服务器"菜单。
![](http://img0.ph.126.net/Zu-Ct3LCNjZ-QXnL2zBhYw==/1841972247694678263.jpg)
图71
- 
点击一个被管理服务器名称的链接，这里选择"Cluster_ManagedServer_1"，进入Cluster_ManagedServer_1的设置页面，点击"集群"tab按钮，并点击左上角的"锁定并编辑"按钮，如下图：
![](http://img1.ph.126.net/oh9WS1BnPcvKNCS8UAjlGQ==/2614058108729917834.jpg)
图72
- 
在复制组后面的文本框中输入"MyGroupA"，首选次级组后面的文本框中输入"MyGroupB"。
![](http://img1.ph.126.net/9Y7vXebcaDk8LP85AiUTVw==/4858539573102860794.jpg)
图73
- 
点击"保存"按钮，保存成功后，点击画面左上角的"激活更改"按钮，激活成功后如下图所示：
![](http://img0.ph.126.net/cGWj7eQZJrvrJaNOn80Qgg==/791226159633773217.jpg)
图74
按照"表2"中的复制组和服务器的分配，以同样地方式设置其他被管理服务器。
- 
### Session复制验证
- 
启动被管理服务器Cluster_ManagedServer_3，Cluster_ManagedServer_4，在浏览器中输入[http://172.20.50.96:8080/ShoppingCart/](http://172.20.50.96:8080/ShoppingCart/)，进入购物商店界面：
![](http://img0.ph.126.net/MoUTjPVtp_f015RUdMElyw==/6597959271565303968.jpg)
图75
- 
点击"Go Shopping"链接进入购物界面
![](http://img2.ph.126.net/QDFMd-bQEWO1QSluy10mQA==/6597147831984337043.jpg)
图76
- 
购物两次(买了价格4.99,6.99两种商品)，返回首页，点击"[View
 Shopping Cart](http://172.20.50.96:8080/ShoppingCart/viewshoppingcart)"链接进入购物车页面，查看已购买的商品
![](http://img0.ph.126.net/XoxGAusxEYzwxzAupXca0w==/1267481820228295377.jpg)
图77
- 
被管理服务器Cluster_ManagedServer_3控制台输出内容
![](http://img0.ph.126.net/Ux0q9U8nuJKU9Oo_Kwk4dg==/2031404907021058467.jpg)
图78
- 
启动被管理服务器Cluster_ManagedServer_1。
在Cluster_ManagedServer_3停掉后，Cluster_ManagedServer_4中session对象的状态会由Secondary State变成Primary State，现在Cluster_ManagedServer_1用作次要服务器，请参见本文"5.3.1服务器和会话对象"一节。
![](http://img2.ph.126.net/yC_kffsFbqj7Wv8lya4I5A==/3139290415354299533.jpg)
图79
- 
停掉管理服务器Cluster_ManagedServer_3
![](http://img1.ph.126.net/GfrnSu6TY78mRAW-YTw9hg==/6597280872890765929.jpg)
图80
- 
继续购物
本次购买价格"15.99"的商品，并返回购物车页面查看已购买的商品：
![](http://img1.ph.126.net/DFNadATnvHrPjX-g5i8v7Q==/2484579619443117193.jpg)
图81
原来购买的商品仍在购物车里，证明session已经成功复制，因为商品信息是保存在session中。
- 
被管理服务器Cluster_ManagedServer_4已经接管原来的web请求，控制台输出内容
![](http://img0.ph.126.net/ONHNKpzE5ZUGd2wkCFH3Ag==/4928063892350326350.jpg)
图82
- 
停掉管理服务器Cluster_ManagedServer_4
![](http://img2.ph.126.net/45_lBLMkfW9tv3QrBXGD5w==/1881660219411045359.jpg)
图83
- 
继续购物
本次购买价格"199.99"的商品，并返回购物车页面查看已购买的商品
![](http://img1.ph.126.net/Z613Zlamkz6zZhLxDsvbLw==/2048856355577153497.jpg)
图84
原来购买的商品仍在购物车里，证明第二次session已经成功复制，session对象由服务器Cluster_ManagedServer_4复制到服务器Cluster_ManagedServer_1中。
- 
被管理服务器Cluster_ManagedServer_1控制台输出内容
![](http://img1.ph.126.net/1nLWdcZI6OMoY1LMwEUCdA==/6597234693402929065.jpg)
图85
