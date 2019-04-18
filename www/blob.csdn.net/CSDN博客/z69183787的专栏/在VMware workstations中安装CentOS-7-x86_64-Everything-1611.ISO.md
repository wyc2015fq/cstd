# 在VMware workstations中安装CentOS-7-x86_64-Everything-1611.ISO - z69183787的专栏 - CSDN博客
2018年02月07日 18:47:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2423
[http://www.178linux.com/80064](http://www.178linux.com/80064)
对于最近在Linux学习的过程中，许多同学对CentOS系统的详细配置还不熟悉，于是我编写此详细步骤供大家参考。
工具：
VMware workstations；
CentOS-7-x86_64-Everything-1611.iso;
准备工作：
 1、先要下载一个系统镜像centOS-7-x86_64-Everything-1611.iso
2、首先你要确保自己的主机已经在BIOS中开启了内存虚拟化，详细介绍请参考[（http://jingyan.baidu.com/article/335530daa55d7e19cb41c3c2.html）](http://jingyan.baidu.com/article/335530daa55d7e19cb41c3c2.html)
如果虚拟内存没有开启，在安装的过程中会出现模式不兼容或者CPU架构请求不到等现象。
开始：
1、打开VMware 点击创建新的虚拟机；
![h](http://www.178linux.com/wp-content/uploads/2017/07/h-300x122.png)
2、选择典型类型就行；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711210252_92591.png)
3、选择稍后安装操作系统；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711210412_84470.png)
4、这里一定要选择与之相对应的版本，我们要操作的是Linux操作系统的centOS64位版本；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711210413_74577.png)
5、虚拟机名称可以自定义；此虚拟机存放的位置可以通过浏览选项进行自己安置；点击浏览，自己可以创建一个文件夹，将虚拟机存放进这个文件夹中，可以有效的进行分类。完成后点击下一步；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711210413_61231.png)
6、这里的磁盘大小可以自定义，根据自己的主机物理内存大小可以适当定义，但是不建议太小，起码不低于建议的20G内存吧，可以将虚拟磁盘存储为单个文件，也是可以移植的；点击下一步，点击完成；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711210413_14571.png)
7、完成之后就会进入此界面，这里我们可以对虚拟硬件设备进行再次定义；选择编辑虚拟机设置；进入如下界面；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711210413_37013.png)
8、进入如下界面，这里比较重要的一点是，CD/DVD（IDE）选项，选择使用ISO映像文件，在浏览里选择自己下载的系统文件，内存这里可以根据自己物理机的实际内存大小自定义划分，但是会提示有上限，不过1G的就够用了，
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211219_52617.png)
网络适配器什么的都可以自行添加，这里的网络适配器选择仅主机模式，完成后点击确定，点击开启此虚拟机，就可以进入启动页面了；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211219_27778.png)![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211219_25028.png)
9、在虚拟机操作中一定要将鼠标点进虚拟机中，要不然操作就不是在虚拟机中进行了，移动上下键，选择Install CentOS Linux 7，回车；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211220_99431.png)
10、选择安装步骤语言，你也可以下滑选择中文，继续；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211220_38079.png)
11、一些主要操作就在这里了，如图所示，对其一一进行了编号，下面序号就对应相对的选项，选择Date&time，进入时间界面
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211842_70535.png)
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711212054_91720.png)
鼠标点击地图上的位置，选择自己所在的正确时区，将时间调整正确，确定；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211842_71281.png)
12、操作完时间后，选择Language support，这一个是选择支持的语言，要把中文选上，避免我们浏览一些中文网站的时候出现乱码的现象；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211842_25416.png)
13、选择SOFTWARE SELECTION； 选择Server with GUI，或者可以选择下面的GNOME Desktop；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211843_95573.png)
14、最重要的步骤来了！这里是选择分区4号。赶紧点进去！ 这里选择I will configure partitioning（自定义分区），点击done；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211843_54959.png)
进入后将partitioning scheme选择为standard Partition；点击“+”创建分区；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211843_94691.png)
出现如下界面，选择根“/”大小根据自己分配的内存进行设定，点击Add mount point完成，根据这个步骤依次完成“/boot1G”、“/app  自定义大小”、“swap  最好是分配内存的两倍”
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211843_46421.png)
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211843_16063.png)
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211843_51305.png)
     都分好后的结果：  点击Done；出现如下界面，点击Accept Change；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711211843_18753.png)
15、选择KDUMP5号，将选项勾掉，这个是内核检测，当内核崩溃是，会记录错误信息，让维护人员有分析依据，这个比较占内存，可以先不要；
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711212759_80062.png)
16：接下来进入6号，进行网络的配置，将Ethernet开关打开，就可以获得IP地址，点击下图中的2位置configuration，将General标签下的两项勾选上，是自由连接可用网络的，点save保存
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711212758_90252.png)
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711212759_56840.png)
完成后返回，可以Begin
installed。
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711212759_33961.png)
16、在安装过程中，可以进入Root Password设置，和用户创建环节，分别点进去。
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711212759_90715.png)
如果密码长度不够的话，点击Done两次就可以了，用户名及密码设置同样。
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711212759_32536.png)![](http://www.178linux.com/wp-content/uploads/2017/07/20170711212759_29816.png)
17： 完成后返回，等待安装完成可以看到Complete，这时候需要重启才行，点击Reboot。
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711212759_22650.png)
     18：在头一次重启的过程中，会出现一个界面，是让同意GPL协议的，有个叹号的标志，点进去，同意选项勾上，Done。
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711212759_24419.png)
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711213504_96344.png)
可以点击继续：
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711213504_34971.png)
   19：稍等片刻就进入开机界面了，大功告成！
![](http://www.178linux.com/wp-content/uploads/2017/07/20170711213504_74651.png)
