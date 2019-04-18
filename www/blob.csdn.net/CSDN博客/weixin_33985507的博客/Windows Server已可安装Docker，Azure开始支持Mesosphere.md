# Windows Server已可安装Docker，Azure开始支持Mesosphere - weixin_33985507的博客 - CSDN博客
2015年04月30日 09:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
在4月29日举行的[微软Build 2015开发者大会](%5C)上，微软首次对外演示了[在Windows Server上运行的Docker引擎](%5C)。去年10月15日，微软[宣布](%5C)与Docker公司建立了新的合作伙伴关系，以便在Windows Server中增加对Docker的支持。在短短的6个月时间里，微软和Docker公司就让这一预想变为了现实。从其他媒体渠道[得知](%5C)，Windows Server vNext正式版本将于2016年发布，不过预览版本[今年5月就可能会放出](%5C)。
\\
微软对Docker的跟进速度非常快，在去年建立合作后，微软就发布了ASP.NET的Docker镜像，使得开发者可以基于Docker容器部署ASP.NET 5的应用程序。紧接着，Azure又提供了对Docker Machine和Docker Swarm的[支持](%5C)。在上个月发布的Docker 1.6版本中，Docker已经发布了Windows客户端预览版，开发者可以使用官方的版本直接在Windows上与Docker Daemon交互。
\\
Microsoft Azure的CTO Mark Russinovich在大会上做了演示，演示内容包括使用Docker将.NET应用部署到Windows Server，通过Visual Studio将相同的应用部署到Azure上的Linux服务器上，在Visual Studio中远程连接和调试Docker中的应用程序。从例子中可以看到，Visual Studio对Docker的支持，绝对领先于其它的IDE。
\\
另外，在大会上，Mark Russinovich还演示了开发者如何使用Mesosphere在Azure上启动一个集群，例子中，他很快就启动了一个200节点的集群，同时创建了2000个Docker容器。随后，AWS也宣布了他们对Mesosphere的支持。至此，Mesosphere的数据中心操作系统已经获得两大云厂商AWS和Azure的支持。
