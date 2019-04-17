# 关于App的cpu/内存/流量 /电量的方法 - zhusongziye的博客 - CSDN博客





2018年05月20日 17:23:09[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：386








怎么对安卓以及iOS app进行流量以及电量进行测试，今天就带大家带来进行测试。这篇文章也是小编从网上的资料整理过来的，觉得写的很全就带给大家，我们借用工具腾讯的GT工具进行测试。

腾讯GT简介：

此apk是一款可以对APP进行测试的软件，可以在任何情况下快速测试手机app的CPU、内存、流量、电量、帧率/流畅度等性能测试。有安卓版本和iOS版本，分别下载


1.腾讯GT下载地址

http://gt.tencent.com/download.html


安卓版本：GT_2.1.9.APK

iOS 版本：GT.embeddedframework_v2.2.3.tar.gz

2、将安卓下载带安卓手机进行安卓。今天我们安卓版的为例子

3.在安卓手机上打开GT,点击接受

![](https://img-blog.csdn.net/2018052017192793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


4.点击选择被测应用，选中自己要测的app

![](https://img-blog.csdn.net/20180520171936904?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


5.选择关注的测试点，cup、net(流量)、内存（pss）等，如果没有启动可以，点击lunch进行启动，这里应用已经在运行了。

![](https://img-blog.csdn.net/20180520171945800?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

6.进入参数标签，点击编辑，拖拽要时时监控的性能指标到悬浮窗展示的参数这，如要监控cup（cpu）、内存（pss）、流量（pnet）（以后在运行目标app时可实时看到这些参数变化）

![](https://img-blog.csdn.net/20180520171953140?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


7.拖拽完成后，点击完成后返回到参数标签

![](https://img-blog.csdn.net/20180520172005582?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


8.在参数标签继续勾选需要监控的参数，只是不实时看到而已。如电量

![](https://img-blog.csdn.net/20180520172024436?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


9.打开要测试的app，点击红色圆点启动监控，开始测试。

![](https://img-blog.csdn.net/20180520172039798?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


10.测试完成后，点击红色的停止按钮

![](https://img-blog.csdn.net/20180520172119959?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


11.打开gt界面，查看各个性能指标数据。单击性能指标，可以查看该性能指标的详情。如点击cpu

![](https://img-blog.csdn.net/2018052017213080?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180520172137804?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


12.批量保存监控的数据：单击保存，将勾选的性能指标数据保存到该文件夹，点击ok。（点击删除按钮可以将监控的数据批量删除）

![](https://img-blog.csdn.net/20180520172147210?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


13.手机使用usb利用360手机助手连接电脑，找到刚才的文件夹。默认在sd卡的GT-GW-xx-GW-DATA下

![](https://img-blog.csdn.net/2018052017215790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


14.使用excel或文本文件打开.csv文件，使用自带的统计图标功能生成统计图。安卓4.0以上系统查看fps前要注意在开发者选项里选中“禁用硬件叠加”，才能得到准确的fps值。


![](https://img-blog.csdn.net/20180520172208227?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


15.进行监控结果的判断

a.查看内存pss判断有无内存泄漏（使用完后没有释放内存）

Native/Dalvik ：主要看这项，它分别给出的是JNI层和Java层的内存分配情况，如果发现这个值一直增长，则代表程序可能出现了内存泄漏。

Total：该应用实时占用的真正内存

![](https://img-blog.csdn.net/2018052017222053?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


b.查看电量：一个系统走一遍基础功能耗电不应超过20%。在gt的插件标签里有耗电测试。这里已知对个别手机比较准确，可以考虑使用其他的工具来测耗电量。

![](https://img-blog.csdn.net/20180520172231684?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


c.查看流量pnet：流量应稳步或较小的递增，如果一下变大，就要查看是哪费流量了进行优化

![](https://img-blog.csdn.net/20180520172243288?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


d.cpu查看：

![](https://img-blog.csdn.net/20180520172255738?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



