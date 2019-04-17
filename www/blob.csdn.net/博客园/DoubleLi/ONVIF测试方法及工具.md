# ONVIF测试方法及工具 - DoubleLi - 博客园






设备是否支持[ONVIF](http://www.jiangyu.org/introduce-of-onvif/)验证
**1 ONVIF Test Tool安装**
1.1PC安装环境要求：装有Microsoft .Net Framework 3.5或以上版本。
1.2安装源文件请见：[ONVIF Conformance Test Tools.rar](http://vdisk.weibo.com/s/2_iNg)

**2 ONVIF Conformance Test Tools 使用**
2.1 ONVIF Conformance Test Tool 1.0

1）Setup
如图1：

![图1 ONVIF Conformance Test Tools 使用](http://pic.yupoo.com/mylaner/BNeq7Mfi/QxKU9.jpg)
图1 ONVIF Conformance Test Tools 使用



说明：Test Device Information和Test Information内的数字可以任意添。



2）搜索
进入Management栏，如图2所示：

![图2 ONVIF Conformance Test Tools 使用](http://pic.yupoo.com/mylaner/BNeq97tB/Gdvi6.jpg)
图2 ONVIF Conformance Test Tools 使用



点击“Discover Devices”按钮搜索即可。



3）搜索结果查看
如果可以搜到（有的厂家设备反应较慢，可以多搜几次看看），在“Discover Devices”按钮右侧就会列出搜索结果列表，如图3红色框内所示：

![图3 ONVIF Conformance Test Tools 使用](http://pic.yupoo.com/mylaner/BNeqaNYY/4OJLf.jpg)
图3 ONVIF Conformance Test Tools 使用



点击列表中任意一个设备，右下角会显示其对应信息。如图4红色框内所示：

![图4 ONVIF Conformance Test Tools 使用](http://pic.yupoo.com/mylaner/BNeqckDD/PWkdx.jpg)
图4 ONVIF Conformance Test Tools 使用



此时输入用户名和密码后（设备自身的用户名和密码，有的厂家设备不需要），点击“Check”按钮，一般情况下会跳出设备基本信息，如图5所示：

![图5 ONVIF Conformance Test Tools 使用](http://pic.yupoo.com/mylaner/BNeqcMqu/6RSKU.jpg)
图5 ONVIF Conformance Test Tools 使用



如图5可以看到此设备一些基本信息。如：Firmware version（固件版本）、Hardware ID（硬件ID）、Manufacturer（厂商）、Model（型号）、Serial Number（序列号）等。



2.2 ONVIF Conformance Test Tool 1.02.3.5
ONVIF Test Tool version 1.02.3.5使用方法与1.0类似，只是界面有些改动，如下图6与图7所示：

![图6 ONVIF Conformance Test Tools 使用](http://pic.yupoo.com/mylaner/BNeqemV1/hAuzZ.jpg)
图6 ONVIF Conformance Test Tools 使用




![图7 ONVIF Conformance Test Tools 使用](http://pic.yupoo.com/mylaner/BNeqfil7/eS0nY.jpg)
图7 ONVIF Conformance Test Tools 使用



2.3 结论
如果通过上述两个工具（或其一）可以搜到该设备，说明此设备支持ONVIF。
如果说明书或厂家说是该设备支持ONVIF，但是搜索不到。可以通过IE或厂家自己的配置工具登陆该设备，看ONVIF支持是否开启，有些厂家的设备ONVIF支持是可选的。









