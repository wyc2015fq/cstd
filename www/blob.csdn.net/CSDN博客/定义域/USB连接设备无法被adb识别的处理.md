# USB连接设备无法被adb识别的处理 - 定义域 - CSDN博客
2015年09月09日 17:17:04[定义域](https://me.csdn.net/u010278882)阅读数：11908标签：[adb](https://so.csdn.net/so/search/s.do?q=adb&t=blog)
个人分类：[配置](https://blog.csdn.net/u010278882/article/category/5802919)
如果下述方法1无法解决，则按照2-1的方式配置。
## 方法一 重启adb
方式一：eclipse-DDMS透视图-devices-Reset adb
方式二：命令行cmd
```
adb kill-server     // 关闭 adb server
adb start-server    // 启动 adb server
adb devices         // 查看当前连接的设备或模拟器
```
> 
**注意**
通常设备接入到计算机后，系统会自动搜索驱动并安装。前提是开启Window自动更新，否则不能自动寻找并安装驱动。首先保证驱动正确安装，如果仍然检测不到，再使用如下方法。（*如下方法仅针对Windows 7系统*）
通过 计算机-属性-设备管理器 观察病寻找带有黄色符号的未识别设备。找到后右键“更新驱动”，然后选择自动更新。如果驱动找不到，请继续阅读。
## 方法二 配置设备硬件id
对于设备无法被系统识别，而无法安装驱动的情况。可按如下方式尝试：
- 
打开 ~/.android/adb_usb.ini（如果没有可以通过 android update adb生成）
- 
计算机-属性-设备管理器-找到设备-右键属性-详细信息-下拉框选择**硬件Id**
寻找 USB\VID-XXXX…，并记录XXXX这4位十六进制数
- 
将4位数字写到 *步骤1.* 中提到的文件，写入内容为
> 
0x+4位数字
完成以上操作，然后尝试*方法一*。
## 方法三 使用Google USB Driver
如以上方式均不可用，尝试如下操作：
- 
打开 **SDK Manager**，下载并安装 **extra** 目录下的 **Google USB Driver**
- 
修改 **SDK_ROOT\extras\google\usb_driver\android_winusb.inf** 文件，在**[Google.NTx86]**和**[Google.NTamd64]**里面均添加如下内容
```
%SingleBootLoaderInterface% = USB_Install, USB\VID_XXXX&PID_XXXX // 这里的xxx即设备硬件id
%CompositeAdbInterface%     = USB_Install, USB\VID_XXXX&PID_XXXX&MI_XX
```
- 
按照方法一后的“**注意**”中的描述，右键”更新驱动程序”。这次**不要选择自动安装**，浏览至步骤2提到的驱动目录，点击安装驱动
完成以上操作后，尝试*方法一*。
> 
**注意**
对于能够识别出驱动的设备，最好使用厂商提供的驱动。这种方式并不支持所有设备。
