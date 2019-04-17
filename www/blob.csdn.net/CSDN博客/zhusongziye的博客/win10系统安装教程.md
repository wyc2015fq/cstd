# win10系统安装教程 - zhusongziye的博客 - CSDN博客





2017年09月09日 21:00:05[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：4086








        win10发布很久了。之前有过一段时间，觉得用着不习惯，就卸载了。现在出于某种原因需要使用win10。由于时间久了，忘记上次win10具体怎么安装了。于是又重新研究了一次，记录如下：

**      本小编的软硬件环境：**

电脑：台式机（本小编自己组装的【技嘉主板（支持win8 UEFI、AHCI），Intel E2-1225 八核心处理器，8G内存，集成显卡（本小编不玩游戏，独显不需要啦）】）

大U盘1个（8G及以上）：用于制作启动盘（如有数据，请备份，后面制作启动盘时会被格式化）

移动硬盘一个：用于存储下载的Win10系统镜像（你放在上面的那个启动U盘里就可以，你试下。我的U盘是4G的放不下所以把镜像放在移动硬盘里了）

![](https://img-blog.csdn.net/20170909211914533?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**下面讲下具体步骤：**

**一、准备一个win10的IOS镜像。**可以去网上下载（比如：系统之家等等），富裕的同学也可去微软逛网购买正版系统。

![](https://img-blog.csdn.net/20170909212405767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上面是我的win10镜像，已经有啦。根据你的需要，可以选择32位和64位版本的。简单的说，你电脑内存4G以下的就选32位的，4G以上的就选64位的。

把镜像拷到移动硬盘里（如果你和小编一样），或者第二步等做好启动盘后，拷到启动盘里。




**二、制作U盘启动盘。**这个工具有很多，老毛桃啊，大白菜啊，等等在这就不说了。小编用的是老毛桃，用了好多年了，顺手。

先去老毛桃官网下载：http://www.laomaotaoupan.cn/[点击打开链接  ，](http://www.laomaotaoupan.cn/)下载老毛桃选择UEFI下载:

![](https://img-blog.csdn.net/20170909215858816?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


下载完成后，安装老毛桃，选择默认配置即可，安装完成后打开：

![](https://img-blog.csdn.net/20170909220540739?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





选择设备：选择你的U盘，其他保持默认，然后点击“一件制作”，就开始制作启动盘了。

接下来弹出框选“确定”

![](https://img-blog.csdn.net/20170909221005552?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后就可以看到开始制作了

![](https://img-blog.csdn.net/20170909221149048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

完成后，会弹出如下框，选择"**否**"就可以了！！图片截图标错了！！然后关闭老毛桃即可。

![](https://img-blog.csdn.net/20170909221327760?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


你也可以选择“是”，接着模拟启动器便会进入老毛桃主菜单页面，需要提醒大家的是，在电脑模拟器所所模拟出来的测试环境中请不要测试相应pe系统等其他功


能。成功打开老毛桃主菜单页面后,按下键盘上的“Ctrl+Alt”组合键释放鼠标，然后点击右上角的关闭窗口按钮即可。

![](https://img-blog.csdn.net/20170909221815629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**三、使用制作好的老毛桃启动盘安装win10系统。**

把你的U盘插到电脑上，准备好你的win10 IOS镜像。

在安装之系统前需要进到BIOS，更改BIOS boot的第一启动项为我们的启动盘。（此部分不会的童鞋自行百度或者留言给我，我告诉你）

然后选择windows 8 features为windows 8（需要主板支持），保存变更重启。

进入到老毛桃主菜单页面，接着将光标移至“【02】老毛桃WIN8 PE标准版（新机器）”，按回车键确认，如下图所示：


![](https://img-blog.csdn.net/20170909223025466?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

即可进入pe系统，鼠标左键双击打开桌面上的老毛桃PE装机工具。打开工具主窗口后，点击映像文件路径后面的“浏览”按钮，如下图所示


![](https://img-blog.csdn.net/20170909223252710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

接着找到并选中u盘启动盘中的win10系统iso镜像文件，点击“打开”即可，如下图

![](https://img-blog.csdn.net/20170909223408646?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

映像文件添加成功后，我们只需在分区列表中选择C盘作为系统盘，然后点击“确定”即可:

![](https://img-blog.csdn.net/20170909223530287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

随后会弹出一个询问框，提示用户即将开始安装系统。确认还原分区和映像文件无误后，点击“确定”

![](https://img-blog.csdn.net/20170909223807034?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

完成上述操作后，程序开始释放系统镜像文件，安装ghost win10系统。

重启电脑后，我们即可进入ghost win10系统桌面了。

![](https://img-blog.csdn.net/20170909223919512?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

此处小编遇到一个问题：重启后进到了老毛桃优盘PE了，小编进到BIOS后把U盘启动盘和移动一盘移除（或更改BIOS boot启动项也可），有点暴力哈。再重启后，即可进入wn10系统桌面了。

哈哈。。。终于完成了。。。快来体验下。。。









