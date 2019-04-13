
# Kubernetes在游戏业务应用调研 - Wesley's Blog - CSDN博客


2018年07月29日 22:49:25[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：106


# 游戏服务器现状
## 普遍痛点
更高并发、更高性能:大量玩家并发访问，对服务器运算能力、IO能力和稳定性有更高的要求PVE、PVP特性，对网络延迟的容忍度低
攻击手段多、安全风险高:成为大流量DDoS攻击和CC攻击的首要对象攻击方式的变化多样，导致游戏停服、玩家流失；恶意注册、撞库盗号、外挂破解等严重影响游戏体验
研发成本高、管理协同难:游戏行业竞争加剧，需要加快游戏产品上线速度，抢占市场；手游测试需要不断适配各种不同厂家、不同类型的终端，测试成本高
发行渠道多、精准定位难:游戏发行运营渠道泛滥、不稳定，无法快速有效引流新用户；游戏部署与游戏运营平台分离，产品同质化严重，很难精准定位游戏玩家
针对不同种类的游戏：
MMO（Massively Multiplayer Online）：高并发，延时容忍度低，高数据读写速度，运维繁重工作量，运维自动化要求高
MOBA（Multiplayer Online Battle Arena）：网络环境要求非常高，服务端计算量大，通常是单点服务，对单机可靠性和单机故障恢复能力要求高，需要批量部署
FPS游戏：网络延时要求非常高，服务端计算量大，通常是单点服务，对单机可靠性和单机故障恢复能力要求高，
卡牌，棋牌游戏：数据库读频率高，网络稳定性要求高，更迭快，开服，合服频率高，容灾要求高
对于不同端的游戏，可以分为手游，页游，端游几类。对于页游而言开服合服频率极高。
# 国内外主要云市场游戏解决方案
## Google
### 游戏解决方案
[Game Server Reference Architecture with App Engine and Compute Engine](https://cloud.google.com/solutions/gaming/dedicated-server-gaming-solution/)
![这里写图片描述](https://img-blog.csdn.net/20180729224633255?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### Google KUBERNETES ENGINE
[Kubernetes Engine 为 Pokémon GO 提供技术支持](https://cloudplatform.googleblog.com/2016/09/bringing-Pokemon-GO-to-life-on-Google-Cloud.html)
## AWS
### 游戏解决方案
![这里写图片描述](https://img-blog.csdn.net/20180729224650754?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180729224650754?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[核心游戏后端](https://aws.amazon.com/cn/gaming/backend-services/)
客户案例：
Turbine：使用微服务方法应对一天三十亿游戏请求
Riot Games：使用 Amazon ECS 标准化应用程序部署
### Amazon EKS (Elastic Container Service for Kubernetes)
### Amazon GameLift
使用 Amazon GameLift 在云中部署、扩展和管理专用游戏服务器，从而为全球玩家提供低延迟、有趣且可靠的游戏体验。专注于为玩家创建出色的多玩家游戏，而不是构建可扩展的服务器基础设施。
## 华为
### 游戏解决方案
![这里写图片描述](https://img-blog.csdn.net/20180729224707705?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 网易
容器服务（基于kubernetes搭建）为用户提供了无服务器容器，让企业能够快速部署业务，轻松运维服务。容器服务支持弹性伸缩、垂直扩容、灰度升级、服务发现、服务编排、错误恢复及性能监测等功能。
### 游戏解决方案
![这里写图片描述](https://img-blog.csdn.net/20180729224722363?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180729224722363?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180729224732160?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 阿里巴巴
### 游戏解决方案
![这里写图片描述](https://img-blog.csdn.net/20180729224750115?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 腾讯
### 游戏解决方案
![这里写图片描述](https://img-blog.csdn.net/20180729224807478?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
使用的游戏：《PUBG Mobile》等
### 蓝鲸平台
蓝鲸，是一套由腾讯游戏运营部沉淀多年，并承载着数百款业务支撑运营系统的基于PaaS的技术解决方案；它提供了完善的前后台开发框架、调度引擎、公共组件等模块，帮助业务的产品和技术人员得以快速、低成本、免运维的构建支撑工具和运营系统。
游戏部署架构，以MMO类型游戏为例：
### Dolphin
游戏更新（Dolphin）是腾讯专为游戏设计的应用程序和游戏资源更新服务，根据玩家本地资源动态计算差异，一次解决跨版本更新，轻松实现多渠道多版本运营。支持灰度发布(游戏高版本选择性更新到部分用户）、HTTPS协议，满足游戏推广拉新、版本更新、活动运营、BUG修复、客户端校验等多种运营策略和应用场景。
需求： 资源热更新，省流量更新，灰度发布；多渠道管理，轻松应对国内游戏渠道繁杂问题。
应用场景
一键发布
智能开区
智能扩缩容
故障自愈
移动运维

[
  ](https://img-blog.csdn.net/20180729224722363?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)