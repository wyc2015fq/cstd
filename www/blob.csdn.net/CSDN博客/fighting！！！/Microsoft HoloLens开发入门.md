# Microsoft HoloLens开发入门 - fighting！！！ - CSDN博客
2017年11月02日 19:56:34[dujiahei](https://me.csdn.net/dujiahei)阅读数：206
Microsoft HoloLens开发入门 - CSDN博客 http://blog.csdn.net/u014025201/article/details/70145103
来源：游戏蛮牛
第一部分：开发要求
Hololens 运行与Win10，应用程序是与UWP(通用windows开发平台)构建的，开发Hololens 这样的全息体验对电脑的配置要求也是相当高的。
硬件配置：
1.64位Windows 10专业版，企业版或教育版(家庭版不支持Hyper-V)
2.64位CPU
3.8GB以上的RAM
4.在BIOS中，必须具备以下功能：
硬件辅助虚拟化
二级地址转换(SLAT)
基于硬件的数据执行保护(DEP)
5.对于GPU，需DirectX 11.0或更高版本，WDDM 1.2驱动程序或更高版本
关于Hyper-V，它是微软的一款虚拟化产品，采用类似Vmware和Citrix开源Xen一样的基于hypervisor的技术。
第二部分：安装
1.启用虚拟化，即在PC上启用硬件虚拟化。
![](https://img-blog.csdn.net/20170412174325364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
详细步骤请看：https://msdn.microsoft.com/library/windows/apps/jj863509(v=vs.105).aspx
2.启用Hyper-V
3.安装Visual Studio 2017或Visual Studio 2015 Update3(https://developer.microsoft.com/en-us/windows/downloads)
![](https://img-blog.csdn.net/20170412174336443?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4.安装HoloLens emulator(https://developer.microsoft.com/en-us/windows/mixed-reality/hololens_emulator_archive)
5.安装Unity (https://unity3d.com/cn/get-unity/download)
第三部分：关于Hololens 模拟器
![](https://img-blog.csdn.net/20170412174354037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
HoloLens模拟器允许你在没有Hololens的情况下在PC上测试全息应用程序，并附带Hololens开发工具集。仿真器使用Hyper-V虚拟机。
关于输入：
向前，向后，向左和向右走 - 使用键盘上的W，A，S和D键或Xbox控制器上的左键。
查找向上，向下，向左和向右 - 单击并拖动鼠标，使用键盘上的箭头键或Xbox控制器上的右键。
空气敲击手势 - 右键单击鼠标，按键盘上的Enter键，或使用Xbox控制器上的A按钮。
绽放手势 - 按键盘上的Windows键或F2键，或按Xbox控制器上的B按钮。手动移动滚动 - 按住Alt键，按住鼠标右键，向上/向下拖动鼠标，或者在Xbox控制器中按住右侧触发器和A按钮，向上和向下移动右侧手柄。
关于工具栏：
在主窗口的右侧，您将找到仿真器工具栏。工具栏包含以下按钮：
关闭：关闭模拟器。
最小化：最小化仿真器窗口。
人工输入：鼠标和键盘用于模拟模拟器的人工输入。
键盘和鼠标输入：键盘和鼠标输入直接传递到HoloLens操作系统作为键盘和鼠标事件，就像连接了蓝牙键盘和鼠标一样。
适合屏幕：适合模拟器屏幕。
缩放：使仿真器越来越大。
帮助：打开模拟器帮助。
打开设备门户：在仿真器中打开HoloLens OS的Windows设备门户。
工具：打开“ 其他工具 ”窗格。
第四部分：开发----Hello，HoloLens!
首先我们在unity中新建一个项目，接着添加一个简单的3D模型进行测试，比如：
![](https://img-blog.csdn.net/20170412174408818?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170412174417615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接着部署Windows Store
![](https://img-blog.csdn.net/20170412174445482?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170412174453538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接着，点击Build，生成VS项目：
![](https://img-blog.csdn.net/20170412174502100?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
启动VS：
![](https://img-blog.csdn.net/20170412174515460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170412174539624?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
一般默认情况下，从Unity导出的UWP应用程序在任何Windows 10设备上运行。由于HoloLens是不同的，应用程序应该利用仅在HoloLens上可用的功能。为此，您需要在Visual Studio TargetDeviceFamily中的Package.appxmanifest文件中设置为“Windows.Holographic” ，如下：
![](https://img-blog.csdn.net/20170412174613640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接下来，就可以运行啦：
![](https://img-blog.csdn.net/20170412174759104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170412174752119?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](http://mmbiz.qpic.cn/mmbiz_png/rh7Nru3hbNOJUf3pRah32I0cpQE3M9u39icakicdpaib7pHqXLSa1aNnW7pVeUk7TNZWVF3ia71BNNw1xc35icTUY2w/0?wx_fmt=png)
第五部分：输入事件总结
1.GAZE凝视操作
![](https://img-blog.csdn.net/20170412174823251?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](http://mmbiz.qpic.cn/mmbiz_png/rh7Nru3hbNOJUf3pRah32I0cpQE3M9u3kmI9dOCIicsXLvnc8OdiazyWKPPdozicIwSBxoia6l9b139EM6vy95cL9A/0?wx_fmt=png)
在Hololens中，使用的是用户的头部位置与方向来gaze，而不是眼睛。
示例代码(PS：核心在于RayCast)：
![](https://img-blog.csdn.net/20170412174839276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170412174927751?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.手势输入
![](https://img-blog.csdn.net/20170412175029404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170412175044986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170412175048236?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3.语音输入
![](https://img-blog.csdn.net/20170412175120190?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170412175125046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170412175130781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4.音频输入
![](https://img-blog.csdn.net/20170412175208752?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170412175212846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170412175216596?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDAyNTIwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
