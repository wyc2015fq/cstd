# linux没有权限操作文件 - miner_zhu的博客 - CSDN博客





2018年08月09日 15:00:39[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：3199








没有权限的话最好是通过命令来解决，linux下有[超级用户](https://www.baidu.com/s?wd=%E8%B6%85%E7%BA%A7%E7%94%A8%E6%88%B7&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)（root）和普通用户，普通用户不能直接操作没有权限的目录。对于你的问题以下分两种解决办法介绍：


打开终端：alt+f2，输入gnome-terminal,回车，在弹出的[界面](https://www.baidu.com/s?wd=%E7%95%8C%E9%9D%A2&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)操作：


1.输入sudo nautilus回车，输入你的用户的密码，这样就打开了一个[超级用户](https://www.baidu.com/s?wd=%E8%B6%85%E7%BA%A7%E7%94%A8%E6%88%B7&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)权限的资源管理器，然后你直接创建你的目录就行了。

2.用命令创建你的目录，先cd到你要创建目录的地址，比如cd /opt，然后sudo mkdir 目录名,按提示输入用户密码即可



