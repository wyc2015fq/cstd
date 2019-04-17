# Android No active compatible AVD's or devices found Exception - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年05月19日 17:53:26[boonya](https://me.csdn.net/boonya)阅读数：13285








## Exception:No active compatible AVD's or devices found. Relaunch this configuration after connecting a device or starting an AVD.

如题所述：遇到此问题估计大多数初学者都会头大，但是仔细看错误提示说的是——没有活动的AVD或者设备，此时你就应该从以下几个方面着手检查了。

### 1、应用程序项目使用的SDK版本是否与AVD的保持一致，且AVD已经创建

如果你的项目是4.4.2的，那么你创建的模拟器也应该是4.4.2的。

![](https://img-blog.csdn.net/20140519174943515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





### 2、AVD已创建，启动仍然报此异常，此时可能是AndroidManifest.xml文件下的SDK限定有问题

![](https://img-blog.csdn.net/20140519175030312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 3、若以上修改之后还是不行，则需要指定程序运行的环境（指定是在模拟器，还是真机上运行）

"$project">鼠标右键>Run As>Run Configurations.....之后你可以看到如下界面，选定自己是要在模拟器（AVD）上运行还是在设备（Device）上运行。

![](https://img-blog.csdn.net/20140519175057421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**注：若你经以上步骤仍不能处理，可能是你的环境有问题或者插件存在缺陷。**






