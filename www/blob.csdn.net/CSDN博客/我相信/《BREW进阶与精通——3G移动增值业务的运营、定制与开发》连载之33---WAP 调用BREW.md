# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之33---WAP 调用BREW - 我相信...... - CSDN博客





2009年12月09日 14:40:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1698










### WAP调用BREW业务为用户访问BREW应用提供了更为广泛地接入方式。BREW开发商可以通过该业务推广自己的应用服务，WAP服务商可以为其所提供的内容增加特色服务，从而促进整个增值业务产业链的发展。

WAP调用BREW是采用WAP页面嵌入BREW超级链接方式实现的。为了支持WAP调用BREW功能，需要手机厂商在OEM层做相应的修改与支持工作。当用户点击该某个指定的Hyperlink时，WAP浏览器将把该链接直接传递到手机OEM层。手机OEM层将根据链接内容启动BREW的AEE层，并执行相应的功能。同时，WAP浏览器将被挂起。当完成BREW功能操作后，WAP浏览器被恢复到进入页面。

WAP调用BREW的主要功能有：

1.直接启动MobileShop连接到下载中心首页。

2.直接启动MobileShop连接到推荐应用的购买页面。

3.直接启动MobileShop对应用进行升级检查。

4.直接启动MobileShop的应用查询功能。

5.直接启动BREW应用，并传递内容参数。



利用WAP调用BREW业务，通过运营商或者CP/SP的WAP平台来展示或推广BREW应用是一种重要的技术推广手段。运营商或者CP/SP为了利用WAP调用BREW业务的功能来展示应用，就需要获取应用的ItemID与机型对照列表，并明确进行WAP展示的业务名称、目的。

通过WAP调用BREW业务，可以增加运营商移动增值业务品牌的访问量。在WAP浏览器的页面中提供相应的连接到基于BREW的业务, 可以引导更多的用户使用该业务,
从而为运营商增加BREW的ARPU。同时，可以增强BREW分发系统目录的表现力，利用WAP对图片等多媒体的支持,丰富BREW分发系统目录中应用的表现和描述,
从而增强用户的使用体验和满意度.

另一方面，通过WAP调用BREW业务，运营商或者CP/SP为用户提供了一致的界面,能够激发CP/SP开发移动增值业务的积极性。比如在LBS应用的栏目中可以同时提供WAP和BREW的定位应用，使开发商在同一WAP页中同时提供WAP和BREW的应用。

一个WAP调用BREW业务的WML页面文件如下：

<?xml version="1.0"
encoding="gb2312"?>

<!DOCTYPE wml PUBLIC "-//WAPFORUM//DTD WML
1.1//EN"

"http://www.wapforum.org/DTD/wml_1.1.xml">

<wml>

<head>

<meta
http-equiv="Cache-Control" content="max-age=0"/>

</head>



<template>

<do
type="option" label="返回">

<go
href="http://wap.uni-info.com.cn"/>

</do>

</template>

<card title="大富翁游戏">

<p>

<img src="./pict/log.png" alt="图片下载中..."/><br/>

大富翁游戏的广告，描述等等<br/>

<a href="cmshop:ItemID=762368"
title="下载">下载大富翁游戏</a><br/>

<a href="wulininfo.wml" title="详细描述">游戏的具体信息</a><br/>

<a href="http://wap.uni-info.com.cn"
title="返回¨">返回上一级</a>

</p>

</card>

</wml>



