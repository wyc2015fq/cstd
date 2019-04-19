# Kali Linux Live USB初始化+使用日记 - =朝晖= - 博客园
# [Kali Linux Live USB初始化+使用日记](https://www.cnblogs.com/dhcn/p/7865898.html)
      1、Live USB制作官方guide：Making a Kali Bootable USB Drive：https://docs.kali.org/downloading/kali-linux-live-usb-install ，额外说两句：1、这个制作方法做出来的的系统，你在其他系统里面因为文件系统的问题，那个分区你根本看不到。2、如果你想还原U盘，你需要下个分区助手之类的软件把U盘重新分区格式化
     2、让U盘系统的文件和系统修改能保存下来，需要Adding Persistence to a Kali Linux “Live” USB Drive，官方指南地址：https://docs.kali.org/downloading/kali-linux-live-usb-persistence ，不过这个指南已经out了，有两个问题：第一个是下面这条指令
```
read start _ < <(du -bcm kali-linux-2016.2-amd64.iso | tail -1); echo $start
```
       并不一定能读出正确的分区start位置，有可能你在执行下面这条指令
```
parted /dev/sdb mkpart primary $start $end
```
      时，系统会提醒你，你分区的起始分支超过了管理空间的区间，这个时候根据错误提示修改start大小
```
start=3212 #tip的那个起始点
```
      第二个问题就是其中关于U盘持久化分区的大小推荐并不能满足最新版的Kail的大小，你对系统做一个apt update&upgrade，分区直接就不够，我个人建议用一个32G的USB3甚至3.1的U盘，然后把U盘剩余大绝大多数空间全部送给这个持久化空间，如果你用的是32G优盘，可以把end位置直接改为
```
end=28gb
```
     按照官方指导完成相关的持久化分区制作以后，就可以重启机器，然后启动时选择对应的持久化启动选项。
     剩下的动作,先把系统更新一下
```
apt update
apt upgrade
```
      装个中文输入法
```
apt install ibus ibus-pinyin
apt install fcitx fcitx-googlepinyin
```
      中文输入法生效好像需要重启，如果找不见，去系统配置入口里面对输入法做一下修改，顺便对系统语言、时区等配置修改一下，把那个current language的选项取消才能看到中文输入法。然后把它加到系统使用输入法列表里面。如果还不新可以去系统工具里面手动启动fcitx。
     Kali还有默认配置操作:修改root密码、新加一个用户，装bleachbit工具清理使用痕迹
     使用提醒：在U盘上操作整个系统其实是个很脆弱的行为，很多的异常操作会导致数据文件不可用，因为在使用U盘 Kalix时要坚持以下原则：
      1. 尽量不要同时开过多GUI写操作性的工具
      2. 使用一段时间关机休息一下，U盘估计已经烫得不行了。
      3.最后有个备份U盘，或者操作能够分布式降低风险。
      4.千万不要根据系统提示重启更新，那种系统启动前在更新在U盘系统上可能会出问题。

