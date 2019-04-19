# Docker入门系列4：命令行小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月09日 12:55:30[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：146
百度Baidu App Engine(BAE)平台也是以Docker作为其PaaS云基础。
目的就是为了解决以下问题:
- 1) 环境管理复杂: 从各种OS到各种中间件再到各种App，一款产品能够成功发布，作为开发者需要关心的东西太多，且难
于管理，这个问题在软件行业中普遍存在并需要直接面对。Docker可以简化部署多种应用实例工作，比如Web应用、后台应
用、数据库应用、大数据应用比如Hadoop集群、消息队列等等都可以打包成一个Image部署。
- 2) 虚拟化手段的变化: 云时代采用标配硬件来降低成本，采用虚拟化手段来满足用户按需分配的资源需求以及保证可用
性和隔离性。然而无论是KVM还是Xen，在 Docker 看来都在浪费资源，因为用户需要的是高效运行环境而非OS, GuestOS既
浪费资源又难于管理, 更加轻量级的LXC更加灵活和快速。
## 目录
 [[隐藏](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93#)] 
- [1 Docker命令行](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93#Docker.E5.91.BD.E4.BB.A4.E8.A1.8C)- [1.1 环境信息相关](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93#.E7.8E.AF.E5.A2.83.E4.BF.A1.E6.81.AF.E7.9B.B8.E5.85.B3)
- [1.2 系统运维相关](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93#.E7.B3.BB.E7.BB.9F.E8.BF.90.E7.BB.B4.E7.9B.B8.E5.85.B3)
- [1.3 日志信息相关](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93#.E6.97.A5.E5.BF.97.E4.BF.A1.E6.81.AF.E7.9B.B8.E5.85.B3)
- [1.4 Docker
 Hub服务相关](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93#Docker_Hub.E6.9C.8D.E5.8A.A1.E7.9B.B8.E5.85.B3)
- [2 Boolean](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93#Boolean)
- [3 后台进程](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93#.E5.90.8E.E5.8F.B0.E8.BF.9B.E7.A8.8B)
- [4 艺搜参考](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93#.E8.89.BA.E6.90.9C.E5.8F.82.E8.80.83)
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93&action=edit&section=1)]Docker命令行
Docker的命令清单可以通过运行 docker ，或者 "docker help 命令"得到:
Docker的子命令已经达到34个之多，其中核心子命令(例如：run)还会有复杂的参数配置。
通过结合功能和应用场景方面的考虑，把命令行划分为4个部分:
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93&action=edit&section=2)]环境信息相关
- info
- version
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93&action=edit&section=3)]系统运维相关
- attach
$ ID=$**(****sudo** docker run -d ubuntu **/**usr**/**bin**/**top -b**)**
$ **sudo** docker attach $ID
$ **sudo** docker stop $ID
- build:从源码构建新Image的命令。因为Image是分层的，最关键的Base Image是如何构建的是用户比较关心的，Docker官方文档给出了构建方法，请参考
[http://docs.docker.com/articles/baseimages/](http://docs.docker.com/articles/baseimages/)
- commit:把有修改的container提交成新的Image
$ **sudo** docker **ps**
ID                  IMAGE               COMMAND             CREATED             STATUS              PORTS
c3f279d17e0a        ubuntu:12.04**/**bin**/****bash**7 days ago          Up 25 hours
197387f1b436        ubuntu:12.04**/**bin**/****bash**7 days ago          Up 25 hours
$ docker commit c3f279d17e0a  SvenDowideit**/**testimage:version3
f5283438590d
$ docker images **|****head**
REPOSITORY                        TAG                 ID                  CREATED             VIRTUAL SIZE
SvenDowideit**/**testimage            version3            f5283438590d        16 seconds ago
- cp:把容器內的文件复制到Host主机上,如;docker cp ff35dcd5168e:/home/2/cuc_ieschool.h264 .
- diff:列出3种容器内文件状态变化（A - Add, D - Delete, C - Change ）的列表清单。
$ **sudo** docker **diff** 7bb0e258aefe
 
C **/**dev
A **/**dev**/**kmsg
C **/**etc
A **/**etc**/**mtab
A **/**go
A **/**go**/**src
A **/**go**/**src**/**github.com
A **/**go**/**src**/**github.com**/**dotcloud
....
- export:把容器系统文件打包并导出来，方便分发给其他场景使用。
$ **sudo** docker **export** red_panda **>** latest.tar
- images
- import / save / load:这一组命令是系统运维里非常关键的命令。加载(两种方法: import, load)，导出(一种方法: save)容器系统文件。
- inspect:查看容器运行时详细信息的命令。
- kill:杀掉容器的进程。
- port:打印出Host主机端口与容器暴露出的端口的NAT映射关系
- pause / unpause
- ps:docker ps打印出正在运行的容器，docker ps -a打印出所有运行过的容器。
- rm:删除指定的容器。
- rmi:删除Image文件。
- run:这个命令是核心命令，可以配置的参数多达28个参数。详细的解释可以通过docker run --help列出。
- start / stop / restart:这组命令可以开启，停止一个容器。
- tag:使用用户名，Image名字，标签名来组织管理Image。
docker tag **[**OPTIONS**]** IMAGE**[**:TAG**]****[**REGISTRYHOST**/****]****[**USERNAME**/****]**NAME**[**:TAG**]**
- top:显示容器內运行的进程。
- wait
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93&action=edit&section=4)]日志信息相关
e*vents:打印容器实时的系统事件。
- history:打印指定Image中每一层Image命令行的历史记录。
docker **history****[**OPTIONS**]** IMAGE
- logs:批量打印出容器中进程的运行日志。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93&action=edit&section=5)]Docker Hub服务相关
- login
- pull / push
- search
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93&action=edit&section=6)]Boolean
Boolean参数形式如： -d=false。注意，当你声明这个Boolean参数时，比如 docker run -d=true，它将直接把启动的Container挂起放在后台运行。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93&action=edit&section=7)]后台进程
Docker后台进程是一个常驻后台的系统进程，值得注意的是Docker使用同一个文件来支持客户端和后台进程，其中角色切换通过-d来实现。这个后台进程是用来管理容器的，使用Docker --help可以得到更详细的功能参数配置
读者可以参考官方的命令行解释文档:[http://docs.docker.com/reference/commandline/cli/](http://docs.docker.com/reference/commandline/cli/)
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974%EF%BC%9A%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B0%8F%E7%BB%93&action=edit&section=8)]艺搜参考
[http://www.infoq.com/cn/articles/docker-command-line-quest](http://www.infoq.com/cn/articles/docker-command-line-quest)
