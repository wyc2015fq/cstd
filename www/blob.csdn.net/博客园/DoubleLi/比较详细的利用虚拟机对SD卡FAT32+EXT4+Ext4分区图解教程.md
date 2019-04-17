# 比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程 - DoubleLi - 博客园






如果大家嫌虚拟机复杂，我这里提供一个我没用虚拟机之前的分区方法：
这个方法实际是可行的

我在没有用虚拟机之前，我是这样操作的
1、首先在分区软件分好fat32+ext2+ext2
2、然后用recovery进入SD卡分区选项，升级到ext3，再升级到ext4，然后开始安装ROM
3、装好后用分区软件看，recovery只帮我将第三个分区升级到了ext4
4、不过没关系，在我能正常使用系统后，我再次进入recovery，再进入分区选项，升级到ext3，再升级到ext4，一样成功将两个分区升级到ext4
不过用了虚拟分区，就简单多了

**1、VirtualBox(虚拟机)下载地址：【VirtualBox(虚拟机)V4.0 beta2】**
![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/98/1b/981b117ba8e9839fe9b0279f77f6c93f_thumb.gif)

**2、下载完成后，进行VirtualBox的安装**![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/44/8f/448fe2139636c6f0c61783381460000b.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/34/62/346251539e4fc8bf9b21ffbe5eb3d195.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/3f/52/3f521fbdab775964795da2402c0700d4.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/a4/cc/a4cca0a330497b3b83cdb8c2072eeb91.gif)

**安装过程基本一直NEXT下载就OK了，下面是安装好后运行虚拟机的图片：**
![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/81/5e/815e5a2fdefa774dd52dc071731aa266_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/00/94/00942c9d9f8e5fcaca6be1a915718227_thumb.gif)

**新建名称这里随意命名，默认选择安装WINDOWS XP，以下步骤按我图片操作**![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/26/fe/26fe2e914393c26f103785c49ad930e9_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/95/4b/954b652d52c5f92cac28113ab9368ef7_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/36/98/3698446a700b03399c960fe49c787609_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/a9/b6/a9b6b88b9f75eec3befca990dfa0fb3a_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/56/5c/565c8bacb901f4f55d7d8e02da26ff6c_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/7e/71/7e715e91e223c36489e32038c72659ce_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/f0/20/f02056c07d91e210b29459708a1c2ae8_thumb.gif)

**完成以上步骤后，需要选择“设置”选项对USB进行设置，发现很多朋友装好虚拟机后读不了SD卡是因为此步没做好！**

**第一次运行时，USB里的“启用USB2.0（EHCT）控制器”是没打勾的，这里需要打上勾**![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/90/f8/90f8c0aedbb6e780b889fb88e5ed1577_thumb.gif)

**然后在USB筛选器右边点第二个图标，选择你的U盘所显示的设备，如我的显示为“未知设备 FFEE：0100[0100]”**![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/d9/f5/d9f5bc21e6a12f0706c44b5ac98ed659_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/1b/e4/1be405c86119fa4bf533dba1f85de773_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/11/a8/11a8296cfbb4f520ffd51c9d42546f96_thumb.gif)

**这一步选择你下载好的LIUNX分区镜像【下载地址在二楼】**![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/dc/40/dc40e88fa748f264078a6038a018c6ce_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/21/69/216969b0faea84d00b941230a5ae3031_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/1b/da/1bda6d5343c7d782d1a5ebf16d68f832_thumb.gif)

**完成以上步骤后，电脑会弹出它装硬件对话框，自动安装完毕即可！**![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/a9/be/a9bee2a8b4aeeec1eb235de35e70bae3_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/d9/89/d98977142c560d7708170cf35ca043ee_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/ee/cd/eecda7ad72f93ee18dc7cbd93fd9b8c0_thumb.gif)

**至此，虚拟机及相关配置到此结束！**

备注：进入虚拟机后，有些朋友的鼠标会可能不能正常使用（就是不能移动出虚拟机外），两种方法：
A：用键盘ALt+？键
B ：Ctrl+Alt+Delete，这时会弹出任务管理器对话框，这里，你的鼠标就能用了

**3、使用VirtualBox虚拟机进行对内存卡分区：****必备工具：1、已经安装好了虚拟机；2、LINUX专用分区工具：gparted-live-0.7.0-7.iso【放在任意你找得到的位置】3、内存卡及读卡器****=======================**

**好了，上图：**
**出现以下界面，选第一个，按回车键**![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/39/d6/39d615ae47f09985ebb54675bb1ad40b_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/84/81/84814c26b0707684bc453a82c8083213.gif)

**出现以下界面，选第二个，按回车键**
![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/9b/69/9b69e94cd9a63092f3f2fe63eac5c47b_thumb.gif)

**出现以下界面，输入26，按回车键**
![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/1e/e4/1ee4dacb98c15c741687b10ae26afcfa_thumb.gif)

**出现以下界面，输入0，按回车键**
![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/d1/a8/d1a81d6b7fefca49ff1bdb839a63e8d1_thumb.gif)

**弹出以下页面，说明成功加载**

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/b8/cc/b8ccbf9672a9058854ace7a09dea783b_thumb.gif)

**选择“设备”选项，在分配USB设置栏中，如果你的USB设备是被选中打勾状态说明装载SD卡成功，否则不成功，可重启重试一下！**
![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/8f/5b/8f5bf8a67986187053117557ad57e0a7_thumb.gif)

**选择你的U盘，注意，比如我的8G是第二个7.3GB的，第一个是虚拟机的虚拟硬盘，如果有朋友只显示第一个10G的盘就是说明加载不成功！**

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/fe/ba/feba39146ef7c561484b31436227c7db_thumb.gif)

**以下图片为具体的分区图片：**

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/6d/8d/6d8d378900fea18c2f0067d46402cd0f_thumb.gif)

**这里我给出了一下分区公式（我是按我的8G盘分的，大家可以参考）第一分区：5.5G （7604-1536-512=5556）第二分区：1.5G （1024*1.5=1536）第三分区：500M （1024*0.5=512）**

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/a0/08/a0089c3956a94a15454e0779ff662ae6_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/12/d3/12d3443e93958c5e345dea1f7219e027_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/2a/26/2a26f8ee611fae8eebadecd4dd55b3b6_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/62/14/6214c84644aa85d4766c06c609d2fc3d_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/c0/13/c013dcdf17e5d67c638c573aca7981f1_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/2a/c2/2ac29eead49808bd6266e2c66932bd3a_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/77/e9/77e96e530e5159643eedf04a23be7894_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/c7/7c/c77c465590b95389e127bde1d9295688_thumb.gif)

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/7f/22/7f227b1d0845b92745e227bea6a58a1e_thumb.gif)




Can't get file width OR height



![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/ab/35/ab35c68f635c89fa09949a18ae417cba_thumb.gif)

**最后，分区成功后，用回DiskGenius查看，如果SD卡第二和第三个分区显示的是ext3说明成功了，因为在windows系统中识别不出ext4**

![比较详细的利用虚拟机对SD卡FAT32+EXT4+Ext4分区图解教程](http://fj.ikafan.com/pic/61/a8/61a84b784a3e10a6748b3c009f6f6bc6_thumb.gif)









