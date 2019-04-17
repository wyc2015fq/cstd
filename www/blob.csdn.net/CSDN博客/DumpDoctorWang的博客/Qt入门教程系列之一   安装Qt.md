# Qt入门教程系列之一      安装Qt - DumpDoctorWang的博客 - CSDN博客





2018年08月10日 21:19:22[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：2104








# 一、下载Qt

官网给出的都是在线安装包，个人觉得还是离线安装包好用，给出两个离线安装包下载地址

[http://download.qt.io/official_releases/qt/](http://download.qt.io/official_releases/qt/)

[http://download.qt.io/archive/qt/](http://download.qt.io/archive/qt/)

最新的Qt版本的是5.11.1，下载地址是

[http://download.qt.io/official_releases/qt/5.11/5.11.1/](http://download.qt.io/official_releases/qt/5.11/5.11.1/)

[http://download.qt.io/archive/qt/5.11/5.11.1/](http://download.qt.io/archive/qt/5.11/5.11.1/)

下载页面如下

![](https://img-blog.csdn.net/20180809212907248?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 Windows系统就下载[qt-opensource-windows-x86-5.11.1.exe](http://download.qt.io/official_releases/qt/5.11/5.11.1/qt-opensource-windows-x86-5.11.1.exe)，Linux系统下载[qt-opensource-linux-x64-5.11.1.run](http://download.qt.io/official_releases/qt/5.11/5.11.1/qt-opensource-linux-x64-5.11.1.run)，MacOS系统下载[qt-opensource-mac-x64-5.11.1.dmg](http://download.qt.io/official_releases/qt/5.11/5.11.1/qt-opensource-mac-x64-5.11.1.dmg)。

# 二、安装

只贴出Windows和Ubuntu的安装教程，因为我只有这两个系统。

1、Windows

双击安装包，依次点击“Next”→“Skip“→”下一步(N)“，进入如下页面

![](https://img-blog.csdn.net/2018081020554617?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 红框内的是编译器组件。因为教程中只涉及到桌面应用，我的电脑装了VS2015，所以我选了“MSVC 2015 32-bit”和“MSVC 2015 54-bit”；“MinGW 5.3.0 32 bit”是一个跨平台的编译器，这个要选上。"Sources"是Qt的源码，除了源码之外，还有“Sample”，也就是例子，可勾选上。至于剩下的，就按照图中的勾选就可以了。"Qt Script"也要勾选上，我忘记勾选了。

然后点击“下一步”，进入协议页，同意协议之后点击“下一步”→“下一步”→“安装”。等待一段时间，安装完成之后“Next“，然后载点击”Finish“，就会启动Qt Creator。

**2、Ubuntu**
- 安装依赖项

```bash
sudo apt-get install build-essential libgl1-mesa-dev gcc g++ make cmake git
```
- 安装Qt

进入Qt安装包所在目录,打开终端，给Qt安装包加上运行权限

```bash
chmod +x qt-opensource-linux-x64-5.11.1.run
```

运行Qt安装包

```bash
./qt-opensource-linux-x64-5.11.1.run
```

直接选择“下一步(Next)“

![](https://img-blog.csdn.net/20180809214322958?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 直接选择“跳过(Skip)“

![](https://img-blog.csdn.net/20180809214529836?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

选择“下一步(Next)“，然后进入下面的页面，选择好Qt的安装路径，点击“下一步(Next)“。**还是要说一下，安装路径最好不要有空格。**

![](https://img-blog.csdn.net/20180809214748242?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

因为教程中不涉及Android的开发，所以可以按照下面的来勾选，然后点击“下一步(Next)“。”Desktop gcc 64-bit“是编译器组件；”Sources“是源码，可以不勾选，不过建议选上，有时候可能需要看源码；其他项的话就看看介绍，英文不懂就自行百度。

![](https://img-blog.csdnimg.cn/20190303111642405.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)

然后同意协议 ，点击“下一步(Next)“

![](https://img-blog.csdn.net/20180809220058157?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后点击“安装(Install)“

![](https://img-blog.csdn.net/20180809220128178?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 等待一段时间，安装完成之后“下一步(Next)“，然后载点击”完成(Finish)“，就会启动Qt Creator。

![](https://img-blog.csdn.net/20180809220847561?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

解决Ubuntu下Qt不能输入中文：[解决UbuntuQT5.10无法输入中文](https://blog.csdn.net/hanshihao1336295654/article/details/82885777)



