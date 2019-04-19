# USB 2.0 A型、B型、Mini和Micro接口定义及封装 - xqhrs232的专栏 - CSDN博客
2014年04月24日 09:40:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1276
原文地址::[http://www.metsky.com/archives/474.html](http://www.metsky.com/archives/474.html)
相关文章
1、USB 2.0 A型、B型、Mini和Micro接口定义及封装----[http://www.360doc.com/content/11/0106/13/570477_84420676.shtml](http://www.360doc.com/content/11/0106/13/570477_84420676.shtml)
[USB](http://www.metsky.com/archives/229.html)全称Universal Serial Bus（通用串行总线），目前USB 2.0接口分为四种类型A型、B型、Mini型还有后来补充的Micro型接口，每种接口都分插头和插座两个部分，Micro还有比较特殊的AB兼容型，本文简要介绍这四类插头和插座的实物及结构尺寸图，如果是做设计用途，还需要参考官方最新补充或修正说明，尽管[USB
 3.0](http://www.metsky.com/archives/229.html)性能非常卓越，但由于USB 3.0规范变化较大，真正应用起来还需假以时日，不管怎样，都已经把火线逼到末路，苹果公司极其郁闷但也爱莫能助。
注意：
1、本文封装尺寸来源，USB 2.0 Specification Engineering Change Notice（Date:10/20/2000）
2、本文图片来源[USB官方协议](http://www.usb.org/)文档，由于USB 3.0在接口和线缆规范上变化较大，后面专门介绍。
3、本文未带插头封装尺寸，插头尺寸请参加官方文档ecn1-usb20-miniB-revd.pdf，下个版本USB 3.0在接口和封装上都有很大变化，本文属于USB 2.0协议内容，如果是USB 3.0设备，似乎只有A型头才能插到2.0插座中Receptacle。
### 1、A型USB插头（plug）和A型USB插座（receptacle）
![](http://www.metsky.com/upload/2010/07/201007131259579523.jpg)
引脚顺序（左侧为Plug，右侧为Receptacle）：
![](http://www.metsky.com/upload/2010/07/201007131432489240.jpg)
引脚定义：
|编号|定义|颜色识别|
|----|----|----|
|1|VBUS|Red（红色）|
|2|D-|White（白色）|
|3|D+|Green（绿色）|
|4|GND|Black（黑色）|
封装尺寸（单PIN Receptacle）：
![](http://www.metsky.com/upload/2010/07/201007131413195318.jpg)
### 2、B型USB插头（plug）和B型USB插座（receptacle）
![](http://www.metsky.com/upload/2010/07/201007131301149640.jpg)
引脚顺序（左侧为Plug，右侧为Receptacle，注意箭头所指斜口向上，USB端口朝向自己）：
![](http://www.metsky.com/upload/2010/07/201007131436178612.jpg)
引脚定义、封装尺寸均与A型[USB](http://www.metsky.com/archives/225.html)引脚说明相同。
[封装](http://www.metsky.com/archives/162.html)尺寸（单PIN Receptacle）：
![](http://www.metsky.com/upload/2010/07/201007131413335175.jpg)
### 3、Mini B型USB插头（plug）和Mini B型USB插座（receptacle）
![](http://www.metsky.com/upload/2010/07/201007131302325060.jpg)
引脚顺序（左侧为Plug，右侧为Receptacle，注意宽边在上，USB端口朝向自己）：
![](http://www.metsky.com/upload/2010/07/201007131455493184.jpg)
引脚定义：
|编号|定义|颜色识别|
|----|----|----|
|1|VBUS|Red（红色）|
|2|D-|White（白色）|
|3|D+|Green（绿色）|
|4|ID|Not connected（未连接）|
|5|GND|Black（黑色）|
[封装](http://www.metsky.com/archives/463.html)尺寸（Receptacle）：
![](http://www.metsky.com/upload/2010/07/201007131414155458.jpg)
以上部分为USB 2.0规范内容，下面的Micro USB实际上是在2006年才发布的补充规范，由于该接口定义无法后向支持USB 3.0协议，故仍然归于USB 2.0协议包。
### 4、Micro USB插头和插座
Micro USB补充定义用于蜂窝电话和便携设备的Micro USB接口，比Mini USB接口更小。其中标准A型和标准B型及Mini-B型都是在USB 2.0规范里定义，2006补充的Micro USB规范定义了，补充了以下定义：
Micro-B plug and receptacle
Micro-AB receptacle
Micro-A plug
由于该协议文档极不清晰，相关插图也是采用贴图形式，所以不再抓图介绍，只放两个实物照片上来看一下（图片来源:[USB MOBILE](http://www.usb-mobile-charger.com/)）：
![](http://www.metsky.com/upload/2010/07/201007131535145932.jpg)
引脚顺序：
Micro-USB（右图）从左到右：1、2、3、4、5，引脚定义同Mini-USB：1为V+、2为D-、3为D+、4为ID、5为GND。
详细了解，请参考官方USB 2.0规范文档，之Micro-USB_1_01.pdf一文，附USB官方网址：[http://www.usb.org/](http://www.usb.org/)。
