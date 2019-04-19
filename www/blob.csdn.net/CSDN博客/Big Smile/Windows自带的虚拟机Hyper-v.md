# Windows自带的虚拟机Hyper-v - Big Smile - CSDN博客
2016年06月27日 00:04:45[王啸tr1912](https://me.csdn.net/tr1912)阅读数：11538标签：[windows																[虚拟机																[hyper-v](https://so.csdn.net/so/search/s.do?q=hyper-v&t=blog)](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[操作系统																[Ubuntu学习](https://blog.csdn.net/tr1912/article/category/6286487)](https://blog.csdn.net/tr1912/article/category/6143675)
        这两天跟着师哥捣鼓流媒体服务器，有幸在Linux系统Ubuntu下面小试了一下Linux的魅力，结果发现还真的是挺不错的，于是就想把这个系统也装在自己的电脑上面，于是就想到了以前发现的Windows自带的虚拟机Hyper-V，由于是Windows自带的虚拟机，所以和Windows的兼容性很好，值得我们去试一下。
# 一、虚拟机管理器的启用
        其实从系统方面来说，这个虚拟机也是Windows8及以上系统的一个特色功能，只要你是核心版以上的系统，就会在添加功能中看到这个项（默认Windows是不启用这个虚拟机的），然后就可以自己添加这个功能（需重启计算机）。
        首先我们需要进入控制面板的程序菜单下，点击“启用或关闭Windows功能”。
![](https://img-blog.csdn.net/20160626232910295?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        然后选择Hyper-V这一项打勾即可。
![](https://img-blog.csdn.net/20160626232919899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
     最后重启电脑即可。至此虚拟机管理器的安装就已经结束了。Windows自带，就是这么的简单。
# 二、创建虚拟机以及系统的安装
        首先我们要找到虚拟机的管理器，一般在Windows8或者8.1的开始屏幕上，win10可以去开始菜单里面找：
![](https://img-blog.csdn.net/20160626233954215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后再右面的已连接的服务器上面（一般是自己的电脑，如果别人电脑上面有这个虚拟机且和你是同一个局域网也可以连接）右键，选择新建虚拟机：
![](https://img-blog.csdn.net/20160626234218544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后就是Windows特有的向导模式，一般只需要下一步：
![](https://img-blog.csdn.net/20160626235020001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20160626235037732?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        这里可以填写虚拟机的名称，方便查找，然后就是虚拟机文件的位置，默认为C盘，建议改到别的盘。
![](https://img-blog.csdn.net/20160626235208124?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里选择的是虚拟机的代数，如果是装非高版本Windows或者别的系统，建议选择第一代，如果选择第二代会有启动不了的情况。
![](https://img-blog.csdn.net/20160626235345689?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
按你要装什么版本的系统来分配内存的大小。一般xp 512mb，win7及以上2G，Linux1G等
![](https://img-blog.csdn.net/20160626235535299?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        这里是创建虚拟机所需要的磁盘文件的，一般只能创建vhd或者是vhdx两种格式的文件，Windows7以上的系统可以在磁盘管理中 加载这个文件。文件位置和大小随系统要求和个人喜好。
![](https://img-blog.csdn.net/20160626235744050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后就是系统的镜像文件，一般都是安装关盘，可以直接装系统，也可以稍后再装。
![](https://img-blog.csdn.net/20160626235840587?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
最后就是确认信息了，确认完成之后就可以等待程序自己生成文件和建立虚拟机了，生成完成之后如图：
![](https://img-blog.csdn.net/20160627000130913?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
最后通过右面的选项就可以进行开机或者配置的操作了。
        系统怎么安装相信大家在别的地方已经学会了，这虚拟机上装系统 和在实体机上装系统没有什么两样。在这里不在叙述。
        下一篇博客将会讲解怎样创建虚拟交换机和如何让虚拟机联网。
