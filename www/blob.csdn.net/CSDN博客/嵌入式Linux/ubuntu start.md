
# ubuntu start - 嵌入式Linux - CSDN博客

2013年09月06日 21:43:24[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1288


首先用你当初装ubuntu的方法进入live cd
打开终端 输入以下命令：1：sudo -i
2：fdisk -l （找一找你的ubuntu安装在那个分区）
下面挂载原系统的根目录：
mount /dev/sdax /mnt（x代表你Ubuntu所在分区编号）
grub-install --root-directory=/mnt /dev/sda (注意:/mnt后面有 空格键)
如果看到
Installation finished.No Error Reported.
就表示已经成功啦！重启看看，熟悉的grub引导画面是不是又回来了？
进入ubuntu
sudo update-grub2
更新一下grub2就可以了。

Synaptic  在ubuntu软件中心里面找这个东西。然后安装


