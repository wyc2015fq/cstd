# win10和ubuntu16双系统安装 - zhusongziye的博客 - CSDN博客





2017年09月10日 12:24:57[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：507








       本文安装教程是基于win10系统安装的ubuntu16，也就是说先安装好win10后，在安装ubuntu。

win10的安装教程请看小编的博客：http://blog.csdn.net/zhusongziye/article/details/77917871

      Ubuntu（乌班图）是一个以桌面应用为主的Linux操作系统，其名称来自非洲南部祖鲁语或豪萨语的“ubuntu”一词，意思是“人性”、“我


的存在是因为大家的存在”，是非洲传统的一种价值观，类似华人社会的“仁爱”思想。Ubuntu基于Debian发行版和GNOME桌面环境。


       好了，废话少说，下面开始干吧，嘿嘿。。。

**一、准备工作**

（1）去ubuntu官方网站下载IOS，网址https://www.ubuntu.com/download/desktop，这里我使用的是桌面版的。适合小白，大神绕道。

         （2）下载EasyBCD，这个像360软件管家之类的软件都有的，搜一下直接安装在win10上备用即可。下载地址：http://www.onlinedown.net/soft/58174.htm

         （3）4G及以上U盘一个（ubuntu IOS 不大不到2G）

         （4）下载U盘启动盘制作软件UltraISO，这个大家自行百度下载http://www.cr173.com/soft/15480.html

**二、用刻录ubuntu16系统到U盘**

       （1）安装下载的UltraISO，安装后打开（如果你的是免安装绿色版直接打开就可以）

![](https://img-blog.csdn.net/20170910125846023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        （2）点击文件-打开，找到ubuntu iso镜像文件，打开

![](https://img-blog.csdn.net/20170910130255110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        （3）开始制作启动盘，点击菜单栏的“启动”，然后再弹出才按中选择“写入硬盘映像...”，打开“写入硬盘映像”对话框

![](https://img-blog.csdn.net/20170910130440102?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


       （4）在写入硬盘映像对话框中，硬盘驱动器选择我们要写入的U盘，写入方式可以选择USB-HDD也可以选择USB-HDD+，这里我们使用默认值

![](https://img-blog.csdn.net/20170910130752574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        （5）这一步是非常关键的，关系到我们最后制作的硬盘映像能否启动电脑并安装系统，点击“便捷启动”，然后再弹出的菜单中依次选择“写入新的驱动器引导扇区”，再选择“Syslinux”，这一步的没有选择的话，那么我们最后制作的U盘映像将不能识别硬盘，不能安装系统。在选择“Syslinux”后，会弹出提示框，毫无疑问，我们应该选择“是”。


![](https://img-blog.csdn.net/20170910131108639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        将Syslinux引导神曲写入设置的过程非常快，写入完成后，会弹出写入成功的提示框。


       （6）将ISO内的文件写入到U盘，点击下面的“写入”按钮，会弹出警告提示框，点击“是”就开始U盘安装盘的写入了。


                 在制作完成后，会自动关闭这个“写入硬盘映像”的对话框。启动盘就做好了！

![](https://img-blog.csdn.net/20170910131613760?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**三、进入win10系统，打开磁盘管理器（不知道在哪里的面壁100分钟），为ubuntu分配磁盘空间。**

          如果你是刚买的win10电脑，那么你的磁盘也许只有一个C盘。不要紧，我们可以压缩C盘，分出D盘、E盘、F盘，

         方法在这不多说了，可以参考 https://jingyan.baidu.com/album/425e69e6bbd0c7be14fc164a.html?picindex=5

          压缩出来的盘，D盘建议留着给win10用（大小你自己根据你的硬盘大小定，留给linux80-100G就行），E、F盘给Linux用，

          E盘建议10-20G, F盘40-70G(如果你硬盘大可以多分)

**PS : 注意E盘和F盘，不要非配盘符和格式化**![](https://img-blog.csdn.net/20170910134049601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**四、从ubuntu启动盘启动**


          把制作好的U盘插入电脑，电脑重启，按对应的键进入BIOS。（不知道自己电脑哪个键进BIOS的面壁去，或按下F2或F10或del试试）

         选择我们的U盘作为第一启动盘。并把UEFI模式关闭（不会的百度或者面壁）。

![](https://img-blog.csdn.net/20170910135352254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170910135608240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


         设置完成后，保存，重启，即可计入ubuntu系统启动盘，可以选择使用和立即安装。我们选择使用。进入ubuntu桌面

![](https://img-blog.csdn.net/20170910140055620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       有木有感觉很清新，哈哈。。。你可以使用下，，感觉好点击桌面安装ubuntu16.04图标，即可进入ubuntu安装过程，如下图，语言我们选择中文

![](https://img-blog.csdn.net/20170910140400865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       接下来保持默认，我们不更新系统。选择继续

![](https://img-blog.csdn.net/20170910140843585?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       下面是linux分区，这里知识很多。建议大家好好找资料学习下。不懂的同学可以选择第一项（安装ubuntu与window10共存），这里我们选择“其他选项”

![](https://img-blog.csdn.net/20170910141917422?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


         点击继续按钮，之后就进入了一个很重要的环节---选择安装位置，分配分区。这里很重要哦！！！看到空闲的盘符，就是我们划分的F盘，这里图片不一致。我们选择空闲的这个，点击左下角的加号“+”，添加Linux分区为，根“/”分区，选择主分区(有人说逻辑分区也是可以的)

![](https://img-blog.csdn.net/20170910142248306?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

         ”/“分区：”主分区“、”空间起始位置“、”Ext4文件系统“、挂载点”/“：大小设置为10-20G


![](https://img-blog.csdn.net/20170910143018420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        同样的方法，我们创建“/home”分区，把这个盘剩下的空间都分配。”/home“分区：”逻辑分区“、”空间起始位置“、”Ext4文件系统“、挂载点”/home“，大小为40-70G

![](https://img-blog.csdn.net/20170910143453101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       接下来，我们创建交换空间swap分区，选中我们的E盘，把分区类型改为swap交换空间。图片不准哈。。。忘记拍图了，仅作参考

      （PS：swap 直接在 分区类型里选，不是在 分区名称里选。swap 是类型不是名称）


![](https://img-blog.csdn.net/20170910143656670?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        分区就好了，我们之分三个区。跟分区/ ，/home和swap分区。然后选择下方安装启动引导其的设备，这里选择我们跟分区对应的设备。




        之后选择安装，到如下让你选择位置的界面，这个不用选，直接“继续”就可以了（进入系统后可以设置）

![](https://img-blog.csdn.net/20170910144309396?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

         之后选择键盘布局，默认都已经选好了，都是“汉语”，这也可以直接“继续”：

![](https://img-blog.csdn.net/20170910144406342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

          之后进入“创建一个用户（也会是系统默认的管理员），填好之后，“继续”

![](https://img-blog.csdn.net/20170910144526288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

          上步点了“继续”之后，就开始安装了，耐心等待吧。安装完成重启。进到bios设置启动项为你的硬盘启动，我是直接重启是关机和开机中间直接移除U盘，懒人。。。

![](https://img-blog.csdn.net/20170910144725200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


          重启后你会发现进入了win10系统，呵呵，，是不是感到很奇怪啊！莫怕，现在叫你降妖大法，妖怪立刻现身！




**五、制造开机启动项菜单**


           在win10打开我们之前下载的EasyBCD工具，制作开机启动项。注意驱动器选择我们的根分区！！！！


![](https://img-blog.csdn.net/20170910150257714?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

         保存后退出。重启电脑，就会开看启动菜单：(图片是win7，我们是win10 ，图片制作参考)


![](https://img-blog.csdn.net/20170910150958810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

         好了，我们可以试着进入这2个系统，看看有没有问题。没问题就成功了。

          当你选择启动菜单为Ubuntu时，进入如下画面，选择最后一个win10(loader)，也可进入win10系统。

![](https://img-blog.csdn.net/20170910151409645?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**六、写在最后**


小编试了都能进入2个系统后，把BIOS UEFI打开也是可以正常进入2个系统的。。。哈哈。。。

         大家多多练习，，欢迎讨论。。。

         linux不熟悉的建议下载鸟哥的linux私房菜PDF看看，书有点厚，不过讲的详细。








