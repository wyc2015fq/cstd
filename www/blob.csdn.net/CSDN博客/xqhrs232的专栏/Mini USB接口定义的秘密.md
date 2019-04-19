# Mini USB接口定义的秘密 - xqhrs232的专栏 - CSDN博客
2016年12月17日 11:18:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3999
个人分类：[PS2/USB/HID技术](https://blog.csdn.net/xqhrs232/article/category/7431349)
原文地址::[http://www.eepw.com.cn/article/269206.htm](http://www.eepw.com.cn/article/269206.htm)
相关文章
1、Mini USB-百度百科----[http://baike.baidu.com/link?url=9TvrNZ4NO9g2JG35j6kvWKy_XN2_ZgsCgXva-5TXd3xmVNWyKhkJ3t2OD7Ip4nh75fBu7L3BpxSTXRYpr8PPR_S4Rp0kHQRiy1wD1Cp761i](http://baike.baidu.com/link?url=9TvrNZ4NO9g2JG35j6kvWKy_XN2_ZgsCgXva-5TXd3xmVNWyKhkJ3t2OD7Ip4nh75fBu7L3BpxSTXRYpr8PPR_S4Rp0kHQRiy1wD1Cp761i)
2、Micro_USB和Mini_USB接口的区别----[http://wenku.baidu.com/link?url=EI9yRcm4mKjzJnR9ia0Yglcl_YL7WE5UjZtWSSkL2gIErKO2cpQexUC1IhTlFBiPkwnGDOn1t1Pox20YjYHAEDWSKx4VoY4Q0PiMxtoo6qK](http://wenku.baidu.com/link?url=EI9yRcm4mKjzJnR9ia0Yglcl_YL7WE5UjZtWSSkL2gIErKO2cpQexUC1IhTlFBiPkwnGDOn1t1Pox20YjYHAEDWSKx4VoY4Q0PiMxtoo6qK)
3、USBA型、B型、Mini和Micro接口定义及封装----[http://wenku.baidu.com/link?url=Dc-iYfzwcj7eWA3gIVCiaQ0oCpJ5iKbn9XJ8Vu4lj2eWYOdEnYRZKdg3f7rMC4jx7G25qM3_Ltzw6wKpND6nIinE78aSpCxazHz_xEvOOyS](http://wenku.baidu.com/link?url=Dc-iYfzwcj7eWA3gIVCiaQ0oCpJ5iKbn9XJ8Vu4lj2eWYOdEnYRZKdg3f7rMC4jx7G25qM3_Ltzw6wKpND6nIinE78aSpCxazHz_xEvOOyS)
**导读**：想必大家对USB都不陌生吧，那么[*Mini USB*](http://www.eepw.com.cn/news/listbylabel/label/Mini%20USB)是什么呢?[*Mini USB*](http://www.eepw.com.cn/news/listbylabel/label/Mini%20USB)的接口又是什么样的呢?本文将会为您解答~~
本文引用地址：[http://www.eepw.com.cn/article/269206.htm](http://www.eepw.com.cn/article/269206.htm)
## 一、[*Mini USB*](http://www.eepw.com.cn/news/listbylabel/label/Mini%20USB)接口定义- -简介
　　USB(Universal Serial Bus)，译为通用串行总线，用于电脑与数码设备间的数据传输。考虑到体积因素，USB设计了数种接口，目前广泛使用的有三种，即标准USB、Mini USB和[*Micro USB*](http://www.eepw.com.cn/news/listbylabel/label/Micro%20USB)。本文将为您详细介绍Mini USB接口。
　　Mini USB，即迷你USB，是一种USB接口标准，与USB的功能和技术指标一致，但比标准USB更小，适用于手机等小型电子设备。现已广泛应用于个人电脑和移动设备等信息通讯产品、摄影器材、数字电视等领域中。
## 二、[*Mini USB接口定义*](http://www.eepw.com.cn/news/listbylabel/label/Mini%20USB%E6%8E%A5%E5%8F%A3%E5%AE%9A%E4%B9%89)- -引脚
　　Mini USB又可以分为A型、B型和AB型。其触点及其功能如下：
![](http://editerupload.eepw.com.cn/201501/fa2bcbbd3b9d75ee3b6c8d979601c5d5.jpg)
　　1脚：V Bus(电源正5V)，为红色;
　　2脚：Data-(数据-)，为白色;
　　3脚：Data+(数据+)，为绿色;
　　4脚：ID(A型Mini USB：与地相连);
　　5脚：GND(地)，为黑色。
　　除4脚外其他引脚均与标准USB相同。A型Mini USB的4脚连接到5脚上，B型Mini USB的4脚可空置，也可连接到5脚上。
　　4脚仅在OTG(On-The-Go，在没有Host情况下的设备间的数据传送)功能中使用，系统控制器通过判断ID脚的电平来判断插入设备，若为高电平，则是B接头插入，若为低电平，则是A接口插入，进而系统使用HNP对话协议来判断哪个做master，哪个做slave。如果系统仅仅用作slave，则使用B型Mini USB接口。
![](http://editerupload.eepw.com.cn/201501/8d73e8a1f87f3699e55e4b4a7593761e.jpg)
## 三、[*Mini USB接口定义*](http://www.eepw.com.cn/news/listbylabel/label/Mini%20USB%E6%8E%A5%E5%8F%A3%E5%AE%9A%E4%B9%89)- -应用
　　在三种Mini USB中，应用范围最广的就是B型Mini USB接口了，这种接口的防误插性能很出众，体积也很小巧，现已广泛应用于MP3、读卡器、数码相机、移动硬盘和智能手机上。
![](http://editerupload.eepw.com.cn/201501/284d89ccc40e89e8e627b43732c5dd1c.jpg)
**　扩展阅读：**
**标准USB**
　　标准USB是一种常见的PC接口，只有4根线，两根电源线两根信号线，一定要注意其正负极不要弄反，否则的话会烧掉USB设备或电脑的南桥芯片。标准USB的触点及其功能如下：
![](http://editerupload.eepw.com.cn/201501/36a6442d16a6d7484ffa5e058befaafe.jpg)
　　1脚：V Bus(电源正5V)，为红色;
　　2脚：Data-(数据-)，为白色;
　　3脚：Data+(数据+)，为绿色;
　　4脚：GND(地)，为黑色。
![](http://editerupload.eepw.com.cn/201501/ae713ae2ec7f37a250010536526083cf.jpg)
**[*Micro USB*](http://www.eepw.com.cn/news/listbylabel/label/Micro%20USB)**
[*Micro USB*](http://www.eepw.com.cn/news/listbylabel/label/Micro%20USB)是USB2.0的一个便携版本，它比部分手机使用的Mini USB接口更小，其接口定义与Mini USB相同。
![](http://editerupload.eepw.com.cn/201501/1bf62c5811994fd875bb084da88268f2.jpg)
**标准USB与Mini USB**
　　图中从左往右依次是：A型Mini USB接口(公口)、B型Mini USB接口(公口)、B型USB 接口(公口)、A型USB接口 (母口)、A型USB接口 (公口)
![](http://editerupload.eepw.com.cn/201501/06e1e5b7b2be459566beb70165841d41.jpg)
**Micro USB与Mini USB**
![](http://editerupload.eepw.com.cn/201501/eedf0a85c838cffe91259f1b62ba6b2e.jpg)
　　了解了[*Mini USB接口定义*](http://www.eepw.com.cn/news/listbylabel/label/Mini%20USB%E6%8E%A5%E5%8F%A3%E5%AE%9A%E4%B9%89)后，亲们可以试着自己制作一个Mini
 USB接口，具体方法请戳链接~~~
　　1、**[自制Mini USB图解](http://www.eepw.com.cn/article/167537_2.htm)**
　　2、**[自制Mini USB的方法](http://www.eepw.com.cn/article/169334.htm)**
　　3、**[USB3.0接口到底改变了我们什么?](http://www.eepw.com.cn/article/245805.htm)**
