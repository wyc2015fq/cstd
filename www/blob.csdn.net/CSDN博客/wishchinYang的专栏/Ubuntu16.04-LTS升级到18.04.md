# Ubuntu16.04-LTS升级到18.04 - wishchinYang的专栏 - CSDN博客
2019年02月08日 12:42:56[wishchin](https://me.csdn.net/wishchin)阅读数：488
       参考此文成功：[如何将Ubuntu的版本从16.04升级到18.04]() ？
       注意每一个细节，不要点击错了。
Ubuntu 18.04 LTS(Bionic Beaver)即将发布, 如果您正在使用Ubuntu 16.04LTS 那么可以轻松的升级到18.04LTS...
## 工具/原料
- 
Ubuntu 16.04LTS
## 方法/步骤
先进入Welcome页面，查看当前的版本，可以看到现在的版本号是16.04
     1.首先刷新一下本地源：**sudo apt-get update**
     2.有必要升级一下当前的软件**：sudo apt-get upgrade**，但是这仅仅是升级了软件包和系统内核，不会升级当前Ubuntu的发行版本号；
     3.输入命令：**sudo update-manager -d**  系统会检查当前是否是最新的发行版，如果有新的会提醒用户
        点击upgrade按钮，进入升级模式
![](https://img-blog.csdnimg.cn/20190208123951247.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
   4.大概下载1G的文件，可完成升级。
 善后：最后出现老的软件处理选项，你可以自行选择保留或者删除。
