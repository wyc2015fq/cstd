# ubuntuKylin17.04重装KDE - wishchinYang的专栏 - CSDN博客
2017年11月15日 13:33:04[wishchin](https://me.csdn.net/wishchin)阅读数：566
        不小心安装了一个不知道什么的东西，把libROS的那一套都给卸载了，然后删除掉了KDE的很多库。然后KDM也丢失了。KDE界面启动之后，plasma-desktop界面也启动不了。选择重装。
       然而出现 无法修正错误 因为您要求某些软件包保持现状
解决方法：
     删除掉出错的软件源。
解决冲突错误：
        参考页面：[http://forum.ubuntu.org.cn/viewtopic.php?p=3156522](http://forum.ubuntu.org.cn/viewtopic.php?p=3156522)
        二楼给了答案：
考虑先用 sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade 更新所有软件包，再试
如还有问题可以直接强制覆盖，但 telepathy 账户可能无法正常工作
**代码:**
apt-get -o Dpkg::Options::="--force-overwrite" -f install
然后，根据步骤，
重新命令行：
 $sudo apt-get install kubuntu-desktop
或者
$sudo apt-get install  plasma-desktop
KDE界面终于又回来了。
