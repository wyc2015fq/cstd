# 1、Xamarin 环境准备 - ggabcda的博客 - CSDN博客





2017年09月08日 20:37:55[ggabcda](https://me.csdn.net/ggabcda)阅读数：155










**一、安装环境**

1、安装VS2017

2、安装Xamarin扩展包

3、安装NDK

![img](http://shp.qpic.cn/collector/363929497/444c7832-9183-4518-8266-70ee0d292063/0)

   工具栏中，一个为AVD（模拟器管理）一个为NDK（管理Andoid SDK）

    3.1 先打开NDK，在Tools\Options中设置代理（因是要从Goolge下载，所以需要设置代理）   ，这里我使用的是mirrors.neusoft.edu.cn，网上还有其它很多国内镜像。下载相应版本的API，如果电脑支持Hyper V，那下载时可以把 Intel X86 Atom下载上，后面创建模拟器时使用速度更快。

![img](http://shp.qpic.cn/collector/363929497/a6cf7e28-e383-42b1-8c9c-afdc7162d93f/0)



4、新建Andorid空白应用程序

![img](http://shp.qpic.cn/collector/363929497/5b56e62a-f913-4557-9d37-d7755b8bab09/0)

 4.1 AndroidManifest.xml 为程序配置相关文件

      Assets 目前没发现有什么用

      Resources 为资源相关文件

      drawable 为图片视频等静态资源文件

      layout 为页面布局文件

      values 为程序资源文件，如界面显示文本，宽度等相关设置数据

      Resource.Designer.cs 程序自行维护类，提供在CS代码中访问相关资源

5、配置模拟器，打开AVD

![img](http://shp.qpic.cn/collector/363929497/720285d4-3611-4fca-a9cb-b29bc8587a50/0)

  5.1  AVD Name：模拟器名称

  5.2 CUP/ABI 如果有就选择 Intel Atom 会快很多



6、选择模拟器启动调试

![img](http://shp.qpic.cn/collector/363929497/a3acb60c-8f6b-4912-8043-2cdad0d3527d/0)

   工具栏中选择对应模拟器，注意版本要和app设置的api版本兼容。

   启动调试VS会启动模拟器并将app部署好。接下来就可以像以往开

   发程序一样断点调试了。



7、界面说明，点击 layout中的相应axml文件，会打开设计视图，与

     wpf类似，工具箱中有相应控件，可以拖动到界面中（不过不长用，基本上都是手写xml）

![img](http://shp.qpic.cn/collector/363929497/7c434b4d-5ccf-49ef-bd2c-244e40ba52f1/0)



8、模拟器中日志查看

     调试时经常一点app就退出了，这时就需要看对应的异常日志了，VS中提供了Device Log窗口，可以跟踪到模拟器中日志信息。在工具栏\Andorid\设备日志 打开

准备工作基本就序，可以开始进入开发工作了。





