# 3亿Docker容器部署的挑战及应对方案 - wishchinYang的专栏 - CSDN博客
2014年12月28日 17:47:13[wishchin](https://me.csdn.net/wishchin)阅读数：636
个人分类：[IAAS](https://blog.csdn.net/wishchin/article/category/2802193)
原文链接：[http://www.csdn.net/article/2014-11-03/2822455](http://www.csdn.net/article/2014-11-03/2822455)
IronWorker是一个面向开发者的任务队列服务，如今其已经部署了3亿多个Docker容器，本文中分享了IronWorker在使用基于Docker的基础架构时，遇到的挑战、解决方法，以及其中的收获。
**编者按：**IronWorker是一个面向开发者的任务队列服务，开发人员可以在不设置和管理任何基础设施的基础上，调度执行大规模的任务。几个月前，Iron开始试用Docker，如今其内部已经部署了3亿多个Docker容器，本文中分享了IronWorker在使用基于Docker的基础架构时，遇到的挑战、解决方法，以及其中的收获。
**以下为原文：**
![](http://cms.csdnimg.cn/article/201411/04/545842571b502_middle.jpg?_=4782)
[IronWorker](http://www.iron.io/worker)是一个任务队列服务，他让开发人员在不用设置和管理任何基础设施的基础上，调度执行大规模的任务。我们3年多前推出这项服务时，使用了包含所有的语言和代码包的LXC容器运行任务。Docker使我们能够轻松地升级和管理一组容器，为客户提供更多的语言环境和安装包。
我们刚开始使用的是v0.7.4版本的Dokcer,使用过程中遇到一些困难（[不能正常的关闭](https://github.com/docker/docker/search?q=rm&type=Issues&utf8=%E2%9C%93)是一个大的问题，但是后来已经被解决了），我们已经成功地克服了所有的困难，并且发现Docker不仅仅满足了我们的需求，更是超出了我们的预期。因此我们在我们的基础架构中推广使用Docker。基于我们的经验来看，这样做是有意义的。
![](http://cms.csdnimg.cn/article/201411/04/5458429277f8d_middle.jpg?_=27619)
## **优势**
下面列出几点我们意识到的Docker的优势：
***更新维护镜像非常容易***
Doker 使用类似git的非常强大的方法来管理Image，使得它能很方便地管理大量的、不断变化的环境，他的Image分层系统不仅节省空间而且使我们拥有更细区分度的images。
现在，我们能够跟上快速更新的语言的节奏，我们能够提供专门的例如一个新的专门为媒体处理而设计的[](http://blog.iron.io/2014/09/new-ffmpeg-ironworker-stack-for-easy.html)[ffmpeg
 stack](http://blog.iron.io/2014/09/new-ffmpeg-ironworker-stack-for-easy.html)。我们现在有多达15个不同的堆栈并且正在迅速扩大。
***资源分配***
基于Lxc的容器是操作系统级别的虚拟化方法，所有的容器共享系统内核，但是每个容器可以被约束使用指定的资源，比如CPU,内存和I/O.Docker提供REST API、环境版本控制、获取/提交镜像、轻松获取统计数据等功能。Docker支持使用[CoW文件系统](http://en.wikipedia.org/wiki/Copy-on-write)来更安全的隔离数据。这意味着，任务中对文件的所有改变都分开存储，并可以用一个命令清除。
 LXC是不能跟踪这种变化。
***Dockerfiles使得集成简单***
我们的团队遍布世界各地。只要发布一个简单的Dockerfile就可以下班，当你休息时，可以保证其他工作的人能够生成和你的一样的镜像。克服了不同地方的人有不同的作息时间的困难。干净的镜像使得它部署和测试更快。我们的迭代周期更快，团队里每个人更加开心。
***不断壮大的社区***
Docker在非常快得更新，甚至比chrome更快。更重要的是，参与增加新功能和修复bug的社区数量在大量增加。 无论是为为镜像贡献还是为Docker做贡献，甚至是为Docker的周边工具做贡献，有一大批聪明的人正在为其努力，因此我们也不能置身事外。我们发现[Docker的社区](https://docker.com/community/participate/)非常活跃有意义，我们很高兴能够成为其中一员。
***Docker + CoreOS***
我们也处在探索阶段，但我们发现Docker和[CoreOS](https://coreos.com/)的结合对于我们来说似乎是更好地选择。Docker提供了稳定的镜像管理和容器。CoreOS提供了一个精简的云操作系统、机器级别分布式编排和虚拟状态管理。这个组合关注问题的不同方面，是一个更合理的基础设施栈。
## **挑战**
每一个服务器端的技术需要微调和定制，尤其是大规模运行时，，Docker也不例外。（例如，我们跑不到5000万的任务，一个月50万小时计算，并且不断更新我们的镜像）。下面是我们使用大量Docker容器数时遇到的一些挑战：
***向后兼容性不够***
该领域的快速创新虽然是一个优势，但是也存在缺点。其中之一是向后兼容性差。在多数情况下，我们遇到的问题主要是是命令行语法、API的改变，从产品角度来说这不是一个严重的问题。
但在某些情况下，它影响了操作性能。例如，在任何启动容器后引发的Docker错误，我们要解析STDERR并根据错误类型进行响应（例如重试）。非常不幸的是，错误的输出格式随着版本不同变化，不得不在不断变化的结果中调试，使我们非常疲惫。
![](http://cms.csdnimg.cn/article/201411/04/5458433d57282.jpg)
**Docker的错误率**
这个问题相对来说还比较好解决，但是意味着每次的更新要经过多次验证，并且你需要一直开发直到这个更新的版本被发布到了系统大部分环境中。我们几个月前使用v0.7.4,现在我们的系统更新到v1.2.0.在这个领域我们已经有了一个很大的进步。
***有限的工具和库***
虽然Docker 有一个四个月前发布的稳定版本，围绕它的一些工具仍然不稳定。采用Docker生态圈中的大部分工具意味着需要投入更多的精力。为了使用最新的功能、修复bug，你团队中需要有人熬夜加班对这些功能，频繁的进行一些修改.也就是说，我们很高兴有一些Docker 周边的工具在开发，而且很期待能够有一个工具在其中脱颖而出。我们对etcd,fleet, kubernetes比较看好。
## 战胜困难
接下来根据我们的经验，更深入的讲我们讲我们遇到的问题和我们的解决方法。问题列表主要来自我们Ironworker的首席开发兼工程运营总监[Roman Kononov](https://www.linkedin.com/in/rkononov)和一直在调试和规范化我们Docker操作的[Sam
 Ward](http://www.iron.io/about)。
![](http://cms.csdnimg.cn/article/201411/04/545843866d89b.jpg)
**Debug时的一个异常**
说明一下，当我们遇到和Docker相关或者其它系统相关的问题，我们可以自动的重新执行任务，对用户没有任何影响（重试是平台的内置功能）。
***删除操作时间长***
起初删除容器时间长，需要太多的磁盘I/O操作。这导致我们的系统速度明显变慢，形成了瓶颈。我们不得不增加可用的内核数目，而这个数量远远超出我们所需的。
![](http://cms.csdnimg.cn/article/201411/04/545843d0673cc.jpg)
**快速删除Docker容器的解决方案**
通过研究使用[devicemapper](https://github.com/docker/docker/tree/master/daemon/graphdriver/devmapper)（一个Docker的文件系统驱动），我们发现设置一个选项有作用`--storage-optdm.blkdiscard=false`，这个选项告诉Docker 删除容器时跳过花费时间长的磁盘操作,大大加速了容器的关闭过程。当修改好删除脚本后，这个问题就没了。
![](http://cms.csdnimg.cn/article/201411/04/5458445b3103a.jpg)
***卷无法卸载***
由于Docker没有可靠地卸载卷，容器不能正确地停止。这导致容器永远在运行，即使已经完成了任务。解决办法就是显示地调用用户自己写得一些列脚本来卸载卷，删除文件夹。幸运的时，这个问题是之前我们使用Docker v0.7.6版本时遇到的，当Docker v0.9.0解决这个问题后我们就删除了那些冗长的脚本。
***内存限制开关***
Docker其中的一个发布的版本中突然新增了内存限制选项，删除了LXC中的选项。其结果是一些工作进程到达内存界限，然后了整体不响应。这弄得我们措手不及，因为即使使用了它不支持的设置，Docker也没有出错。解决方法很简单，即在Docker内部设置内存限制，但是这种变化让我们措手不及
## 未来计划
正如你所看到的，我们对Docker投入非常的多，我们在接下得每天会继续投入。除了用它来隔离用户在IronWorker中运行的代码，我们也准备在其他的一些领域使用它。 
这些领域包括：
***IronWorker 后台***
除了使用Docker作为任务的容器，我们也在使用它来管理每个服务器上运行的用来管理和启动任务的进称。每一进程着的主要任务是从队列中拿一个任务，把它放到合适的Docker容器中，运行，监测，运行完后删除环境。有趣的是同一台机器上我们有容器化的代码来管理其它容器。把我们所有的基础设施环境放到Docker的容器中让我们在CoreOS上的运行相当容易。
***IronWorker, IronMQ,以及 IronCache APIs***
我们和其他的ops团队一样，没有人喜欢部署。能够把我们的所有的服务打包Docker容器中，然后简单、确定地部署，我们非常地激动。不用再配置服务器。我们需要的就只是能够运行Dokcer容器的服务器。我们正在替换我们的服务器搭建，使用Docker容器在服务器上为我们发布的产品搭建环境。变得的灵活、简单，有更可靠的协议栈。
***生成和加载程序***
我们也在用Docker容器在IronWorker中生成和加载程序。一个显著的进步是为用户改进了，大规模、特定任务负载和工作流的创建、上传、运行任务的过程。还有一个好处是用户可以在本地测试程序，而测试环境和我们的生产服务一致。
***企业内部部署版***
使用Docker作为主要分发方法，IronMQ企业内部部署版简化了我们的分发工作，并且提供了一个简单通用的在几乎任何云环境中都能部署的方法。就像我们在共有云上运行的服务，客户需要的就是可以运行Docker容器的服务器，同时他们可以相对容易的获得在测试或生产环境中运行的多台服务器的云服务。
原文链接：[Docker in Production — What We’ve Learned Launching Over 300 Million Containers](http://blog.iron.io/2014/10/docker-in-production-what-weve-learned.html?m=1)（编译/王晓冉 审校/周小璐）
**如需要了解更多Docker相关的资讯或是技术文档可访问[Docker技术社区](http://docker.csdn.net/)；如有更多的疑问请在[Dcoker技术论坛](http://docker.csdn.net/m/zone/docker/bbs)提出，我们会邀请专家回答。购票等问题可咨询QQ群：303806405。**
![](http://cms.csdnimg.cn/article/201409/04/5408294f805e8.jpg)
Container技术日报公众账号已开启，欢迎关注！
- 顶
6- 踩
1
![](http://creatim.allyes.com.cn/imedia/csdn/20141106/09_47_54_5D2C2B0D.jpg)
推荐阅读相关主题：[云操作系统](http://www.csdn.net/tag/%E4%BA%91%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F/news)[基础架构](http://www.csdn.net/tag/%E5%9F%BA%E7%A1%80%E6%9E%B6%E6%9E%84/news)[开发人员](http://www.csdn.net/tag/%E5%BC%80%E5%8F%91%E4%BA%BA%E5%91%98/news)[文件系统](http://www.csdn.net/tag/%E6%96%87%E4%BB%B6%E7%B3%BB%E7%BB%9F/news)[版本控制](http://www.csdn.net/tag/%E7%89%88%E6%9C%AC%E6%8E%A7%E5%88%B6/news)[操作系统](http://www.csdn.net/tag/%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F/news)
- ![](http://avatar.csdn.net/8/F/E/3_bluky999.jpg)
[子江](http://my.csdn.net/bluky999)20小时前
牛，希望明年这个时候目前的应用开发部署方式有大的转变，切换到docker环境，从此轻松应对高并发高可靠 :)
回复

