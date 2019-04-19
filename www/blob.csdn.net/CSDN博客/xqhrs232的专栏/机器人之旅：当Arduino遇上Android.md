# 机器人之旅：当Arduino遇上Android - xqhrs232的专栏 - CSDN博客
2014年05月02日 10:32:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：913
原文地址::[http://blog.knownsec.com/2012/02/%E6%9C%BA%E5%99%A8%E4%BA%BA%E4%B9%8B%E6%97%85%EF%BC%9A%E5%BD%93arduino%E9%81%87%E4%B8%8Aandroid/](http://blog.knownsec.com/2012/02/%E6%9C%BA%E5%99%A8%E4%BA%BA%E4%B9%8B%E6%97%85%EF%BC%9A%E5%BD%93arduino%E9%81%87%E4%B8%8Aandroid/)
相信很多同学都看过android手机那个很酷的宣传片（android手机+乐高搭建的解魔方机器人）。打小就一直梦想着能拥有一个自己亲手做的机器人，尤其是看完那部经典的机器管家（[http://movie.douban.com/subject/1292529/](http://movie.douban.com/subject/1292529/)，不过估计宅男们更喜欢《我的机器人女友》）。
貌似这个理想是渐行渐远了，直到老杨强烈给推荐arduino，让我们这些电路基础一般的爱好者也能有机会动手搭建自己的机器人，只要搞搞结构设计再coding一把，而不用困扰于具体的电路实现、传感器控制。鉴于现在的android手机功能都那么强劲，当然得充分利用一把了。上周在公司的showcase上给大家秀了一下基于arduino和android的蓝牙机器人小车，下一个目标就是整个遥控灰机。
**什么是Arudino（百度百科）：**
> 
Arduino，是一块基于开放源代码的[USB](http://baike.baidu.com/view/854.htm)接口Simple i/o接口板（包括12通道数字GPIO，4[通道](http://baike.baidu.com/view/157980.htm)PWM输出，6-8通道10bit
 ADC输入通道），并且具有使用类似Java,C语言的IDE[集成开发环境](http://baike.baidu.com/view/14867.htm)。
让您可以快速使用Arduino语言与Flash或Processing…等[软件](http://baike.baidu.com/view/37.htm)，作出互动作品。
Arduino可以使用开发完成的电子元件例如Switch或sensors或其他控制器、LED、步进马达或其他输出装置。Arduino也可以独立运作成为一个可以跟软件沟通的接口，例如说：[flash](http://baike.baidu.com/view/7641.htm)、processing、Max/MSP、VVVV
 或其他互动软件…。Arduino开发IDE接口基于开放源代码原，可以让您免费下载使用开发出更多令人惊艳的互动作品。
![](http://blog.knownsec.com/wp-content/uploads/2012/02/arduino%E6%8E%A7%E5%88%B6%E6%9D%BF.jpg)
**arduino的特点：**
- 开源架构。我们也可以很低的价格从网上购买到现成的控制板；
- 统一接口的传感器，应有尽有的传感器组件。通过统一的插口，很方便就能连接不同扩展设备，淘宝上都有的卖。相比之下，乐高机器人的传感器就贵的很离谱。
- 编程语言：基于c语言，类似java和processing。IDE环境使用java开发。只要有简单的编程经验，上手就超级简单。
**android手机的优势：**
- cpu：杠杠的，远远高于普通单片机；
- 丰富的传感器：自带摄像头，重力加速计，麦克风，喇叭，等等；
- 编程接口：java。关键是，还支持python！
- 自带蓝牙和wifi模块，很方便用来进行远程数据传输；
**Android+Arduino示意图：**
![](http://blog.knownsec.com/wp-content/uploads/2012/02/android+arduino2.png)
千里之行，始于足下。虽然很想现在就搞个四轴飞行器耍一把，还是先整个简单的机器人小车比较靠谱。android系统提供了蓝牙串口功能，并且支持python开发哦。在熟悉了arduino基本接口之后，就开始着手了。
## 前期准备
**开发软件**
软件下载 [http://arduino.cc/en/Main/Software](http://arduino.cc/en/Main/Software)，下载回来解压就能用了。
提供以下平台的支持：
·   Mac OS X
·   Windows平台
·   Linux平台
**所需的全套设备：**
arduino控制板
二轮机器人小车套件（包含车底板，电池盒，马达2个）
马达驱动板
蓝牙扩展接口板
BluetoothV3蓝牙串口模块（提供透明TTL串口通讯）
组装过程实在是有点简单，线路连接都没有操作错误的机会。
需要注意的是，传感器方面：马达驱动板占用了4个数字IO口 ：4,5,6,7，分别用做两个马达的方向、转速。
而蓝牙模块就比较省事了，不需要任何coding，直接与串口连接了。使用时只要进行简单的蓝牙配对（蓝牙适配器pin码默认是1234）。附上核心代码：
void loop(){char val =Serial.read();Serial.write(val);if(-1!= val){if('W'== val)
    go();elseif('A'==val)
    left();elseif('D'== val)
    right();elseif('S'== val)
    back();else
      stop();
    delay(500);}else{//stop();
    delay(500);}}
**Android手机：**
为了图省事，网上直接找了个蓝牙串口通信助手，用来与arduino进行基于蓝牙的串口操作。
一切搞定后，打开蓝牙串口通信助手，连接到arduino，接下来输入口令就行了。WASD空格，很标准的操作:p
这张是搞到一半时的照片：
![](http://ww1.sinaimg.cn/bmiddle/65c2aa0fjw1dp18gcgrdrj.jpg)
**PS:**
**组件列表：**
arduino 控制板:  淘宝上一大堆，挑便宜的arduino就阔以了。
二轮机器人小车套件：建议用机器人基地的这个：[http://item.taobao.com/item.htm?spm=11028vrd.1-1lRm*.3-2aNwt0&id=3311355487](http://item.taobao.com/item.htm?spm=11028vrd.1-1lRm*.3-2aNwt0&id=3311355487)（我自己买的内个小车实在有点丑）
马达驱动板：采用的是L293 1A 双路电机驱动板。用来控制两个马达输入输出。
扩展接口板：挑选了Arduino Xbee传感器扩展板V5 含RS485无线数传蓝牙接口
BluetoothV3蓝牙串口模块： 与Arduino Xbee扩展版共同工作。
顺便show一下放在公司的乐高机器人套件，哈哈用这个做机器人更酷更方便，编程也跟搭积木一样，但是动手的乐趣就少了点。
**参考：**
arduino介绍：
[http://zh.wikipedia.org/wiki/Arduino](http://zh.wikipedia.org/wiki/Arduino)
[http://baike.baidu.com/view/1268436.htm](http://baike.baidu.com/view/1268436.htm)
Arduino机器人应用1-2WD小车基本功能测试
[http://blog.sina.com.cn/s/blog_5e4725590100gcu2.html](http://blog.sina.com.cn/s/blog_5e4725590100gcu2.html)
