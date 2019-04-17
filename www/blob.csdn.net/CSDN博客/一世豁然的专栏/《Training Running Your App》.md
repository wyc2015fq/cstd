# 《Training:Running Your App》 - 一世豁然的专栏 - CSDN博客





2017年04月10日 21:19:30[一世豁然](https://me.csdn.net/Explorer_day)阅读数：302标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方说明。




在上一课中，您创建了一个Android项目。 该项目包含一个显示“Hello World”的默认app。 在本课程中，您将在设备或模拟器上运行app。




一、在真实设备上运行

设置您的设备如下：


1、使用USB电缆将设备连接到开发机器。 如果您正在Windows上开发，则可能需要为您的设备安装相应的USB驱动程序。 有关安装驱动程序的帮助，请参阅OEM USB驱动程序文档。




2、通过（在手机上）转到设置>开发人员选项，启用设备上的USB调试。

注：在Android 4.2和更新版本中，默认情况下隐藏开发人员选项。 要使其可用，请转到设置>关于手机，然后点击构建号码七次。 返回上一个屏幕以查找开发人员选项。




从Android Studio运行app，如下所示：

1、在Android Studio中，选择您的项目，然后从工具栏中单击![](https://img-blog.csdn.net/20170410210458534?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)运行。




2、在“Select Deployment Target”窗口中，选择您的设备，然后单击“OK”




Android Studio会在连接的设备上安装该app并启动它。








二、在仿真器上运行

在模拟器上运行app之前，您需要创建一个Android虚拟设备（AVD）定义。 AVD定义了要在Android模拟器中进行模拟的Android手机，平板电脑，Android Wear或Android TV设备的特征。





创建AVD定义如下：


1、通过选择Tools > Android > AVD Manager或单击工具栏中的“AVD Manager”图标![](https://img-blog.csdn.net/20170410211200606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，启动Android虚拟设备管理器。




2、在AVD Manager主屏幕上，单击Create Virtual Device。




3、在Select Hardware页面中，选择手机设备，如Nexus 6，然后单击Next。




4、在Select Image页面中，选择所需的AVD系统图像，然后单击 Next。




5、验证配置设置（对于您的第一个AND，保留所有设置），然后单击Finish。




有关使用AVD的更多信息，请参阅Create and Manage Virtual Devices。





从Android Studio运行app，如下所示：


1、在Android Studio中，选择您的项目，然后从工具栏中单击运行![](https://img-blog.csdn.net/20170410210458534?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。




2、在Select Deployment Target窗口中，选择您的模拟器，然后单击OK。




仿真器可能需要几分钟才能启动。 您可能需要解锁屏幕。 当您这样做时， My First App出现在仿真器屏幕上。




