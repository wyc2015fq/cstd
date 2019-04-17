# Windows  Mobile Smartphone 获取GPS坐标 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年06月25日 17:09:59[boonya](https://me.csdn.net/boonya)阅读数：927标签：[windows mobile																[sdk																[.net framework																[gps](https://so.csdn.net/so/search/s.do?q=gps&t=blog)
个人分类：[C#](https://blog.csdn.net/boonya/article/category/1776321)





**1、定位安装SDK文档目录的GPS示例**

Windows Mobile 6 SDK 安装后会给我们提供一些CPP和CS的程序编码的示例。在samples/ProcketPC/CS下我们可以找到GPS获取经纬度坐标的示例代码。

![](https://img-blog.csdn.net/20140625164708968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**2、下载FrakeGPS.CAB运行环境测试包**

        如果你想开发一个运行在 Windows Mobile 系统的 GPS 应用程序，但是找不到 GPS 设备用来调试，或者你在室内调试程序，GPS 设备接收不到卫星信号，那 Fake GPS 正是你急需的工具。

      GPSID 的所有设置信息都保存在注册表中，可以通过修改注册表来更改 GPSID 的设置。其中有个一个设置，可以让 GPSID 从一个包含 NMEA 命令的 .txt 文件读取 GPS 数据，而不需要从 GPS 设备读取数据。这个在张汝颇的 Windows Mobile 轻松接轨GPS 中也有介绍。Fake GPS 的工作原理就是修改注册表的 GPSID 设备，让 GPSID 从一个文本文件读取 GPS 数据。而对于我们开发的应用程序来说，跟访问真实的设备并没有两样。在使用 Fake GPS 之前需要将它安装到你
 Windows Mobile 设备上或者仿真器上，安装文件是 FakeGPS.CAB。这里我把 Fake GPS 安装到 Windows Mobile 6 Standard 的仿真器里。安装成功后在开始菜单中找到它并运行。


**3、在真实设备上测试GPS位置信息**

第一步：将安装包拷贝至智能机资源管理器下。

![](https://img-blog.csdn.net/20140625170349796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第二步：安装FakeGPS.CAB（图略）。

第三步：安装打包好的GPS测试示例程序（建议生成开始菜单）。

![](https://img-blog.csdn.net/20140625170417390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第四步：安装成功后打开GpsTest测试

![](https://img-blog.csdn.net/20140625170328390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**4、FakeGPS设置问题**

Fake GPS 的设置默认没有生效，我们可以在第一个输入框把它的状态改为 Enabled，现在可以在第二个输入框中选择从哪个文件读取 GPS 数据。Fake GPS 默认带了两个测试文件，fakegpsdata.txt 在打开后会让 GPSID 马上接收到位置信息，而 dixies.txt 需要等待一段时间才能接收到位置信息。如果你想使用自己的测试数据，可以将编写好的 .txt 文件复制到 /Program Files/FakeGPS/GPSFiles 目录下，在重新运行 Fake GPS 就可以在
 NMEA File 列表中看到了。


注：怎么在模拟器中安装FakeGPS没试过，有兴趣的话可以去研究。](https://so.csdn.net/so/search/s.do?q=.net framework&t=blog)](https://so.csdn.net/so/search/s.do?q=sdk&t=blog)](https://so.csdn.net/so/search/s.do?q=windows mobile&t=blog)




