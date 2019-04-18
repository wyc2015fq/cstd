# QT5+android_ubuntu软件源 - wishchinYang的专栏 - CSDN博客
2013年11月02日 14:17:16[wishchin](https://me.csdn.net/wishchin)阅读数：857
个人分类：[QT./Linux](https://blog.csdn.net/wishchin/article/category/1721683)
（1）：安卓版QT开发环境下载：[http://qt-project.org/downloads](http://qt-project.org/downloads)
最终未能成功安装编译器，错误:Qt Creator 需要设置一个编译器来构建。在构建套…
（2）：ubuntu13.04 软件源，原文链接：[http://chenrongya.blog.163.com/blog/static/87474196201332584815505/](http://chenrongya.blog.163.com/blog/static/87474196201332584815505/)
不同的网络状况连接以下源的速度不同, 建议在添加前手动验证以下源的连接速度(ping下就行),选择最快的源可以节省大批下载时间。
- 首先备份源列表:
sudo cp /etc/apt/sources.list /etc/apt/sources.list_backup
- 而后用gedit或其他编辑器打开:
[](http://wiki.ubuntu.org.cn/File:Qref_Ubuntu_Logo.png) gksu gedit /etc/apt/sources.list
[](http://wiki.ubuntu.org.cn/File:Qref_Kubuntu_Logo.png) kdesudo kate /etc/apt/sources.list  
[](http://wiki.ubuntu.org.cn/File:Qref_Xubuntu_Logo.png) gksu mousepad /etc/apt/sources.list
[](http://wiki.ubuntu.org.cn/File:Qref_Xubuntu_Logo.png) gksu leafpad /etc/apt/sources.list (13.04版)
[](http://wiki.ubuntu.org.cn/File:Qref_Edubuntu_Logo.png) gksu gedit /etc/apt/sources.list
- 从下面列表中选择合适的源，替换掉文件中所有的内容，保存编辑好的文件:
[](http://wiki.ubuntu.org.cn/File:Attention_niels_epting.svg)注意：一定要选对版本- 然后，刷新列表:
sudo apt-get update
[](http://wiki.ubuntu.org.cn/File:Attention_niels_epting.svg)注意：一定要执行刷新
## 源列表
### Raring(13.04)版本 
Ubuntu 官方更新服务器（欧洲，此为官方源，国内较慢，但无同步延迟问题，电信、移动/铁通、联通等公网用户可以使用)：deb http://archive.ubuntu.com/ubuntu/ raring main restricted universe multiverse
deb http://archive.ubuntu.com/ubuntu/ raring-security main restricted universe multiverse
deb http://archive.ubuntu.com/ubuntu/ raring-updates main restricted universe multiverse
deb http://archive.ubuntu.com/ubuntu/ raring-proposed main restricted universe multiverse
deb http://archive.ubuntu.com/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://archive.ubuntu.com/ubuntu/ raring main restricted universe multiverse
deb-src http://archive.ubuntu.com/ubuntu/ raring-security main restricted universe multiverse
deb-src http://archive.ubuntu.com/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://archive.ubuntu.com/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://archive.ubuntu.com/ubuntu/ raring-backports main restricted universe multiverseUbuntu官方提供的其他软件（第三方闭源软件等）：deb http://archive.canonical.com/ubuntu/ raring partner
deb http://extras.ubuntu.com/ubuntu/ raring main
[骨头兄](http://luy.li/about/)亲自搭建并维护的 Ubuntu 源（该源位于浙江杭州百兆共享宽带的电信机房)，[包含
 Deepin 等镜像](http://ubuntu.srt.cn/)：deb http://ubuntu.srt.cn/ubuntu/ raring main restricted universe multiverse
deb http://ubuntu.srt.cn/ubuntu/ raring-security main restricted universe multiverse
deb http://ubuntu.srt.cn/ubuntu/ raring-updates main restricted universe multiverse
deb http://ubuntu.srt.cn/ubuntu/ raring-proposed main restricted universe multiverse
deb http://ubuntu.srt.cn/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://ubuntu.srt.cn/ubuntu/ raring main restricted universe multiverse
deb-src http://ubuntu.srt.cn/ubuntu/ raring-security main restricted universe multiverse
deb-src http://ubuntu.srt.cn/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://ubuntu.srt.cn/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://ubuntu.srt.cn/ubuntu/ raring-backports main restricted universe multiverse
网易163更新服务器（广东广州电信/联通千兆双线接入），[包含其他开源镜像](http://mirrors.163.com/)：deb http://mirrors.163.com/ubuntu/ raring main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ raring-security main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ raring-updates main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ raring-proposed main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ raring main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ raring-security main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ raring-backports main restricted universe multiverse
搜狐更新服务器（山东联通千兆接入，[官方中国大陆地区镜像](http://cn.archive.ubuntu.com)跳转至此） ，[包含其他开源镜像](http://mirrors.sohu.com/)：deb http://mirrors.sohu.com/ubuntu/ raring main restricted universe multiverse
deb http://mirrors.sohu.com/ubuntu/ raring-security main restricted universe multiverse
deb http://mirrors.sohu.com/ubuntu/ raring-updates main restricted universe multiverse
deb http://mirrors.sohu.com/ubuntu/ raring-proposed main restricted universe multiverse
deb http://mirrors.sohu.com/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://mirrors.sohu.com/ubuntu/ raring main restricted universe multiverse
deb-src http://mirrors.sohu.com/ubuntu/ raring-security main restricted universe multiverse
deb-src http://mirrors.sohu.com/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://mirrors.sohu.com/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://mirrors.sohu.com/ubuntu/ raring-backports main restricted universe multiverse
LupaWorld 更新服务器（浙江杭州电信/联通双线服务器），[包含其他开源镜像](http://mirror.lupaworld.com/)：deb http://mirror.lupaworld.com/ubuntu raring main restricted universe multiverse
deb http://mirror.lupaworld.com/ubuntu raring-security main restricted universe multiverse
deb http://mirror.lupaworld.com/ubuntu raring-updates main restricted universe multiverse
deb http://mirror.lupaworld.com/ubuntu raring-backports main restricted universe multiverse
deb http://mirror.lupaworld.com/ubuntu raring-proposed main restricted universe multiverse
deb-src http://mirror.lupaworld.com/ubuntu raring main restricted universe multiverse
deb-src http://mirror.lupaworld.com/ubuntu raring-security main restricted universe multiverse
deb-src http://mirror.lupaworld.com/ubuntu raring-updates main restricted universe multiverse
deb-src http://mirror.lupaworld.com/ubuntu raring-backports main restricted universe multiverse
deb-src http://mirror.lupaworld.com/ubuntu raring-proposed main restricted universe multiverse
[常州贝特康姆软件技术有限公司](http://bitcomm.cn/) 更新服务器（江苏常州电信服务器），[包含其他开源镜像](http://centos.bitcomm.cn/)。( [CN99域名](http://www.cn99.com/) 系 [“常州市亚科技术有限公司” 所注册](http://www.miibeian.gov.cn/icp/publish/query/icpMemoInfo_login.action?id=23145237)，但此公司与常州贝特康姆的法人代表和营业地址相同。所以本镜像实际即原 CN99。但现在 [CN99镜像的域名](http://mirrors.cn99.com/)已改为跳转至[网易镜像](http://mirrors.163.com/)）:deb http://centos.bitcomm.cn/ubuntu raring main restricted universe multiverse
deb http://centos.bitcomm.cn/ubuntu raring-security main restricted universe multiverse
deb http://centos.bitcomm.cn/ubuntu raring-updates main restricted universe multiverse
deb http://centos.bitcomm.cn/ubuntu raring-backports main restricted universe multiverse
deb http://centos.bitcomm.cn/ubuntu raring-proposed main restricted universe multiverse
deb-src http://centos.bitcomm.cn/ubuntu raring main restricted universe multiverse
deb-src http://centos.bitcomm.cn/ubuntu raring-security main restricted universe multiverse
deb-src http://centos.bitcomm.cn/ubuntu raring-updates main restricted universe multiverse
deb-src http://centos.bitcomm.cn/ubuntu raring-backports main restricted universe multiverse
deb-src http://centos.bitcomm.cn/ubuntu raring-proposed main restricted universe multiverse
**以下为有教育网接入的服务器（推荐教育网用户使用，部分非教育网用户也有可观的速度。教育网用户请优先使用IPv6地址。**
中国科学技术大学更新服务器（位于合肥，千兆教育网接入，百兆电信/联通线路智能路由），由[中科大 Linux 用户协会](http://lug.ustc.edu.cn/)和[中科大学网络信息中心](http://ustcnet.ustc.edu.cn/)维护，[包含其他开源镜像](http://mirrors.ustc.edu.cn/)，[Deepin
 官方服务器](http://packages.linuxdeepin.com/) 实际亦指向此处： deb http://debian.ustc.edu.cn/ubuntu/ raring main multiverse restricted universe
deb http://debian.ustc.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb http://debian.ustc.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb http://debian.ustc.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb http://debian.ustc.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
deb-src http://debian.ustc.edu.cn/ubuntu/ raring main multiverse restricted universe
deb-src http://debian.ustc.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb-src http://debian.ustc.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb-src http://debian.ustc.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb-src http://debian.ustc.edu.cn/ubuntu/ raring-updates main multiverse restricted universeIPv6-Only 地址deb http://mirrors6.ustc.edu.cn/ubuntu/ raring main multiverse restricted universe
deb http://mirrors6.ustc.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb http://mirrors6.ustc.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb http://mirrors6.ustc.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb http://mirrors6.ustc.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
deb-src http://mirrors6.ustc.edu.cn/ubuntu/ raring main multiverse restricted universe
deb-src http://mirrors6.ustc.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb-src http://mirrors6.ustc.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb-src http://mirrors6.ustc.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb-src http://mirrors6.ustc.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
清华大学更新服务器，（教育网核心节点百兆接入，已计划提高到千兆）由[清华大学学生网管会](http://tuna.tsinghua.edu.cn/)维护。[包含其他开源镜像](http://mirrors.tuna.tsinghua.edu.cn/)：deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ raring main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ raring main restricted universe multiverse
deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ raring-proposed main restricted universe multiverseIPv4-Only 地址deb http://mirrors.4.tuna.tsinghua.edu.cn/ubuntu/ raring main restricted universe multiverse
deb http://mirrors.4.tuna.tsinghua.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb http://mirrors.4.tuna.tsinghua.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb http://mirrors.4.tuna.tsinghua.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb http://mirrors.4.tuna.tsinghua.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://mirrors.4.tuna.tsinghua.edu.cn/ubuntu/ raring main restricted universe multiverse
deb-src http://mirrors.4.tuna.tsinghua.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb-src http://mirrors.4.tuna.tsinghua.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://mirrors.4.tuna.tsinghua.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://mirrors.4.tuna.tsinghua.edu.cn/ubuntu/ raring-proposed main restricted universe multiverseIPv6-Only 地址
deb http://mirrors.6.tuna.tsinghua.edu.cn/ubuntu/ raring main multiverse restricted universe
deb http://mirrors.6.tuna.tsinghua.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb http://mirrors.6.tuna.tsinghua.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb http://mirrors.6.tuna.tsinghua.edu.cn//ubuntu/ raring-security main multiverse restricted universe
deb http://mirrors.6.tuna.tsinghua.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
deb-src http://mirrors.6.tuna.tsinghua.edu.cn/ubuntu/ raring main multiverse restricted universe
deb-src http://mirrors.6.tuna.tsinghua.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb-src http://mirrors.6.tuna.tsinghua.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb-src http://mirrors.6.tuna.tsinghua.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb-src http://mirrors.6.tuna.tsinghua.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
北京交通大学更新服务器（教育网/电信百兆接入），由[北京交通大学信息中心](http://ic.bjtu.edu.cn/)赞助，[包含其他开源镜像](http://mirror.bjtu.edu.cn/)。（域名中的 bjtu 可以替换为 njtu ，即北交旧名“北方交通大学”对应域名） ：deb http://mirror.bjtu.edu.cn/ubuntu/ raring main multiverse restricted universe
deb http://mirror.bjtu.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb http://mirror.bjtu.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb http://mirror.bjtu.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb http://mirror.bjtu.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
deb-src http://mirror.bjtu.edu.cn/ubuntu/ raring main multiverse restricted universe
deb-src http://mirror.bjtu.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb-src http://mirror.bjtu.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb-src http://mirror.bjtu.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb-src http://mirror.bjtu.edu.cn/ubuntu/ raring-updates main multiverse restricted universeIPv6-Only 地址
deb http://mirror6.bjtu.edu.cn/ubuntu/ raring main multiverse restricted universe
deb http://mirror6.bjtu.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb http://mirror6.bjtu.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb http://mirror6.bjtu.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb http://mirror6.bjtu.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
deb-src http://mirror6.bjtu.edu.cn/ubuntu/ raring main multiverse restricted universe
deb-src http://mirror6.bjtu.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb-src http://mirror6.bjtu.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb-src http://mirror6.bjtu.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb-src http://mirror6.bjtu.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
北京理工大学更新服务器（仅教育网可以访问），[包含其他开源镜像](http://mirror.bit.edu.cn/)：deb http://mirror.bit.edu.cn/ubuntu/ raring main restricted universe multiverse
deb http://mirror.bit.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb http://mirror.bit.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb http://mirror.bit.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb http://mirror.bit.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://mirror.bit.edu.cn/ubuntu/ raring main restricted universe multiverse
deb-src http://mirror.bit.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb-src http://mirror.bit.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://mirror.bit.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://mirror.bit.edu.cn/ubuntu/ raring-proposed main restricted universe multiverseIPv6-Only 地址deb http://mirror.bit6.edu.cn/ubuntu/ raring main restricted universe multiverse
deb http://mirror.bit6.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb http://mirror.bit6.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb http://mirror.bit6.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb http://mirror.bit6.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://mirror.bit6.edu.cn/ubuntu/ raring main restricted universe multiverse
deb-src http://mirror.bit6.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb-src http://mirror.bit6.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://mirror.bit6.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://mirror.bit6.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
天津大学更新服务器，由天津大学信息与网络协会和[天津大学自由软件联盟](http://tju.zyrj.org/)维护，[包含其他开源镜像](http://mirror.tju.edu.cn/)
 (校外限 IPv6 访问)（域名可以替换为 jx.tju.zyrj.org 或 jx6.tju.zyrj.org）deb http://mirror.tju.edu.cn/ubuntu/ raring main restricted universe multiverse
deb http://mirror.tju.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb http://mirror.tju.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb http://mirror.tju.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb http://mirror.tju.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://mirror.tju.edu.cn/ubuntu/ raring main restricted universe multiverse
deb-src http://mirror.tju.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb-src http://mirror.tju.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://mirror.tju.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://mirror.tju.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
青岛大学更新服务器（教育网/山东青岛移动专线双线服务器），由 [OSQDU](http://blog.osqdu.org/)维护，位于青岛大学。[包含开源镜像和Opera、马克思主义文库等其他镜像](http://mirror.qdu.edu.cn/)。：deb http://mirror.qdu.edu.cn/ubuntu raring main restricted universe multiverse
deb http://mirror.qdu.edu.cn/ubuntu raring-security main restricted universe multiverse
deb http://mirror.qdu.edu.cn/ubuntu raring-updates main restricted universe multiverse
deb http://mirror.qdu.edu.cn/ubuntu raring-backports main restricted universe multiverse
deb http://mirror.qdu.edu.cn/ubuntu raring-proposed main restricted universe multiverse
deb-src http://mirror.qdu.edu.cn/ubuntu raring main restricted universe multiverse
deb-src http://mirror.qdu.edu.cn/ubuntu raring-security main restricted universe multiverse
deb-src http://mirror.qdu.edu.cn/ubuntu raring-updates main restricted universe multiverse
deb-src http://mirror.qdu.edu.cn/ubuntu raring-backports main restricted universe multiverse
deb-src http://mirror.qdu.edu.cn/ubuntu raring-proposed main restricted universe multiverse
东北大学更新服务器，[包含其他开源镜像](http://ftp.neu.edu.cn/mirrors/)：deb http://ftp.neu.edu.cn/mirrors/ubuntu/ raring main multiverse restricted universe
deb http://ftp.neu.edu.cn/mirrors/ubuntu/ raring-backports main multiverse restricted universe
deb http://ftp.neu.edu.cn/mirrors/ubuntu/ raring-proposed main multiverse restricted universe
deb http://ftp.neu.edu.cn/mirrors/ubuntu/ raring-security main multiverse restricted universe
deb http://ftp.neu.edu.cn/mirrors/ubuntu/ raring-updates main multiverse restricted universe
deb-src http://ftp.neu.edu.cn/mirrors/ubuntu/ raring main multiverse restricted universe
deb-src http://ftp.neu.edu.cn/mirrors/ubuntu/ raring-backports main multiverse restricted universe
deb-src http://ftp.neu.edu.cn/mirrors/ubuntu/ raring-proposed main multiverse restricted universe
deb-src http://ftp.neu.edu.cn/mirrors/ubuntu/ raring-security main multiverse restricted universe
deb-src http://ftp.neu.edu.cn/mirrors/ubuntu/ raring-updates main multiverse restricted universeIPv6-Only 地址deb http://ftp.neu6.edu.cn/mirrors/ raring main multiverse restricted universe
deb http://ftp.neu6.edu.cn/mirrors/ raring-backports main multiverse restricted universe
deb http://ftp.neu6.edu.cn/mirrors/ raring-proposed main multiverse restricted universe
deb http://ftp.neu6.edu.cn/mirrors/ raring-security main multiverse restricted universe
deb http://ftp.neu6.edu.cn/mirrors/ raring-updates main multiverse restricted universe
deb-src http://ftp.neu6.edu.cn/mirrors/ raring main multiverse restricted universe
deb-src http://ftp.neu6.edu.cn/mirrors/ raring-backports main multiverse restricted universe
deb-src http://ftp.neu6.edu.cn/mirrors/ raring-proposed main multiverse restricted universe
deb-src http://ftp.neu6.edu.cn/mirrors/ raring-security main multiverse restricted universe
deb-src http://ftp.neu6.edu.cn/mirrors/ raring-updates main multiverse restricted universe
哈尔滨工业大学更新服务器，[包含其他开源镜像](http://run.hit.edu.cn/)：deb http://run.hit.edu.cn/ubuntu/ raring main restricted universe multiverse
deb http://run.hit.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb http://run.hit.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb http://run.hit.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb http://run.hit.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://run.hit.edu.cn/ubuntu/ raring main restricted universe multiverse
deb-src http://run.hit.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb-src http://run.hit.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://run.hit.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://run.hit.edu.cn/ubuntu/ raring-proposed main restricted universe multiverseIPv6-Only地址deb http://run6.hit.edu.cn/ubuntu/ raring main restricted universe multiverse
deb http://run6.hit.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb http://run6.hit.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb http://run6.hit.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb http://run6.hit.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://run6.hit.edu.cn/ubuntu/ raring main restricted universe multiverse
deb-src http://run6.hit.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb-src http://run6.hit.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://run6.hit.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://run6.hit.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
大连理工大学更新服务器，[包含其他开源镜像](http://mirror.dlut.edu.cn/)：
( IPv4/v6 Both )
deb http://mirror.dlut.edu.cn/ubuntu/ raring main restricted universe multiverse
deb http://mirror.dlut.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb http://mirror.dlut.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb http://mirror.dlut.edu.cn/ubuntu/ubuntu raring-backports main restricted universe multiverse
deb http://mirror.dlut.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
deb-src http://mirror.dlut.edu.cn/ubuntu/ raring main restricted universe multiverse
deb-src http://mirror.dlut.edu.cn/ubuntu/ raring-security main restricted universe multiverse
deb-src http://mirror.dlut.edu.cn/ubuntu/ raring-updates main restricted universe multiverse
deb-src http://mirror.dlut.edu.cn/ubuntu/ raring-backports main restricted universe multiverse
deb-src http://mirror.dlut.edu.cn/ubuntu/ raring-proposed main restricted universe multiverse
上海交通大学更新服务器（教育网千兆接入，联通/电信线路情况不详），[包含其他开源镜像](http://ftp.sjtu.edu.cn/)：deb http://ftp.sjtu.edu.cn/ubuntu/ raring main multiverse restricted universe
deb http://ftp.sjtu.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb http://ftp.sjtu.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb http://ftp.sjtu.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb http://ftp.sjtu.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
deb-src http://ftp.sjtu.edu.cn/ubuntu/ raring main multiverse restricted universe
deb-src http://ftp.sjtu.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb-src http://ftp.sjtu.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb-src http://ftp.sjtu.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb-src http://ftp.sjtu.edu.cn/ubuntu/ raring-updates main multiverse restricted universeIPv6-Only 地址deb http://ftp6.sjtu.edu.cn/ubuntu/ raring main multiverse restricted universe
deb http://ftp6.sjtu.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb http://ftp6.sjtu.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb http://ftp6.sjtu.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb http://ftp6.sjtu.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
deb-src http://ftp6.sjtu.edu.cn/ubuntu/ raring main multiverse restricted universe
deb-src http://ftp6.sjtu.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb-src http://ftp6.sjtu.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb-src http://ftp6.sjtu.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb-src http://ftp6.sjtu.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
厦门大学更新服务器，由[厦门大学信息与网络中心](http://net.xmu.edu.cn/)维护，[包含其他开源镜像](http://mirrors.xmu.edu.cn/)：
( IPv4/v6 Both )
deb http://mirrors.xmu.edu.cn/ubuntu/archive/ raring main restricted universe multiverse
deb http://mirrors.xmu.edu.cn/ubuntu/archive/ raring-backports restricted universe multiverse
deb http://mirrors.xmu.edu.cn/ubuntu/archive/ raring-proposed main restricted universe multiverse
deb http://mirrors.xmu.edu.cn/ubuntu/archive/ raring-security main restricted universe multiverse
deb http://mirrors.xmu.edu.cn/ubuntu/archive/ raring-updates main restricted universe multiverse
deb-src http://mirrors.xmu.edu.cn/ubuntu/archive/ raring main restricted universe multiverse
deb-src http://mirrors.xmu.edu.cn/ubuntu/archive/ raring-backports main restricted universe multiverse
deb-src http://mirrors.xmu.edu.cn/ubuntu/archive/ raring-proposed main restricted universe multiverse
deb-src http://mirrors.xmu.edu.cn/ubuntu/archive/ raring-security main restricted universe multiverse
deb-src http://mirrors.xmu.edu.cn/ubuntu/archive/ raring-updates main restricted universe multiverse
成都市电子科技大学更新服务器，由[电子科大学生宿舍网管会](http://ubuntu.dormforce.net/about.html)维护。[仅包括
 Ubuntu 镜像](http://ubuntu.dormforce.net/)： 
( IPv4-Only )
（如下列地址不通，可换用 uestc.edu.cn 域名尝试）deb http://ubuntu.dormforce.net/ubuntu/ raring main multiverse restricted universe
deb http://ubuntu.dormforce.net/ubuntu/ raring-backports main multiverse restricted universe
deb http://ubuntu.dormforce.net/ubuntu/ raring-proposed main multiverse restricted universe
deb http://ubuntu.dormforce.net/ubuntu/ raring-security main multiverse restricted universe
deb http://ubuntu.dormforce.net/ubuntu/ raring-updates main multiverse restricted universe
deb-src http://ubuntu.dormforce.net/ubuntu/ raring main multiverse restricted universe
deb-src http://ubuntu.dormforce.net/ubuntu/ raring-backports main multiverse restricted universe
deb-src http://ubuntu.dormforce.net/ubuntu/ raring-proposed main multiverse restricted universe
deb-src http://ubuntu.dormforce.net/ubuntu/ raring-security main multiverse restricted universe
deb-src http://ubuntu.dormforce.net/ubuntu/ raring-updates main multiverse restricted universe
兰州大学更新服务器，由[兰大开源社区](http://oss.lzu.edu.cn/)维护，[包含其他开源镜像](http://mirror.lzu.edu.cn/)：
deb http://mirror.lzu.edu.cn/ubuntu/ raring main multiverse restricted universe
deb http://mirror.lzu.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb http://mirror.lzu.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb http://mirror.lzu.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb http://mirror.lzu.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
deb-src http://mirror.lzu.edu.cn/ubuntu/ raring main multiverse restricted universe
deb-src http://mirror.lzu.edu.cn/ubuntu/ raring-backports main multiverse restricted universe
deb-src http://mirror.lzu.edu.cn/ubuntu/ raring-proposed main multiverse restricted universe
deb-src http://mirror.lzu.edu.cn/ubuntu/ raring-security main multiverse restricted universe
deb-src http://mirror.lzu.edu.cn/ubuntu/ raring-updates main multiverse restricted universe
**以下为大陆地区外的源**
香港中文大学更新服务器，由[香港中文大学资讯科技服务处](http://www.cuhk.edu.hk/itsc/chinese/)维护，[包含其他开源镜像](http://ftp.cuhk.edu.hk/pub/Linux/)：
deb http://ftp.cuhk.edu.hk/pub/Linux/ubuntu raring main restricted universe multiverse
deb http://ftp.cuhk.edu.hk/pub/Linux/ubuntu raring-security main restricted universe multiverse
deb http://ftp.cuhk.edu.hk/pub/Linux/ubuntu raring-updates main restricted universe multiverse
deb http://ftp.cuhk.edu.hk/pub/Linux/ubuntu raring-backports main restricted universe multiverse
deb http://ftp.cuhk.edu.hk/pub/Linux/ubuntu raring-proposed main restricted universe multiverse
deb-src http://ftp.cuhk.edu.hk/pub/Linux/ubuntu raring main restricted universe multiverse
deb-src http://ftp.cuhk.edu.hk/pub/Linux/ubuntu raring-security main restricted universe multiverse
deb-src http://ftp.cuhk.edu.hk/pub/Linux/ubuntu raring-updates main restricted universe multiverse
deb-src http://ftp.cuhk.edu.hk/pub/Linux/ubuntu raring-backports main restricted universe multiverse
deb-src http://ftp.cuhk.edu.hk/pub/Linux/ubuntu raring-proposed main restricted universe
台湾的官方源。速度有时甚至快于内地的，[包含 Debian 等其他镜像](http://tw.archive.ubuntu.com/)：
deb http://tw.archive.ubuntu.com/ubuntu raring main restricted universe multiverse
deb http://tw.archive.ubuntu.com/ubuntu raring-security main restricted universe multiverse
deb http://tw.archive.ubuntu.com/ubuntu raring-updates main restricted universe multiverse
deb http://tw.archive.ubuntu.com/ubuntu raring-backports main restricted universe multiverse
deb http://tw.archive.ubuntu.com/ubuntu raring-proposed main restricted universe multiverse
deb-src http://tw.archive.ubuntu.com/ubuntu raring main restricted universe multiverse
deb-src http://tw.archive.ubuntu.com/ubuntu raring-security main restricted universe multiverse
deb-src http://tw.archive.ubuntu.com/ubuntu raring-updates main restricted universe multiverse
deb-src http://tw.archive.ubuntu.com/ubuntu raring-backports main restricted universe multiverse
deb-src http://tw.archive.ubuntu.com/ubuntu raring-proposed main restricted universe multiverse
原文地址：
[http://wiki.ubuntu.org.cn/Qref/Source](http://wiki.ubuntu.org.cn/Qref/Source)
