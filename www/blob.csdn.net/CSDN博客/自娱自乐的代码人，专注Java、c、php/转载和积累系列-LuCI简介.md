# 转载和积累系列 - LuCI简介 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年01月02日 16:53:26[initphp](https://me.csdn.net/initphp)阅读数：1259








# 

LuCI作为“FFLuCI”诞生于2008年3月份，目的是为OpenWrt[固件](http://baike.baidu.com/view/33738.htm)从 Whiterussian 到 Kamikaze实现快速配置接口。Lua是一个小巧的[脚本语言](http://baike.baidu.com/view/76320.htm)，很容易嵌入其它语言。轻量级
 LUA语言的官方版本只包括一个精简的核心和最基本的库。这使得LUA体积小、启动速度快，从而适合嵌入在别的程序里。UCI是OpenWrt中为实现所有系统配置的一个统一接口，英文名Unified Configuration Interface，即统一配置接口。LuCI,即是这两个项目的合体，可以实现[路由](http://baike.baidu.com/view/18655.htm)的网页配置界面。

最初开发这个项目的原因是没有一个应用于嵌入式的免费，干净，可扩展以及维护简单的网页用户界面接口。大部分相似的配置接口太依赖于大量的Shell[脚本语言](http://baike.baidu.com/view/76320.htm)的应用，但是LuCi使用的是Lua编程语言，并将接口分为逻辑部分，如模板和视图。LuCI使用的是面向对象的库和模板，确保了高效的执行，轻量的安装体积，更快的执行速度以及最重要的一个特性————更好的可维护性。

与此同时，LuCI从MVC-Webframework衍生出一个包含了很多库、程序以及Lua程序[用户接口](http://baike.baidu.com/view/1309799.htm)的集合，但是LuCI仍然专注于实现网页用户界面并成为OpenWrt Kamikaze官方的一份子。

LuCI是一个开放源码的独立项目，欢迎任何人的加入。

**安装LuCI**

openwrt源：

1.转到OpenWrt根目录。

2.输入 ./scripts/feeds update

3.输入 ./scripts/feeds install -a -p luci

4.输入 make menuconfig

5.在”LuCI”菜单下你将找到所有的组件。

OpenWrt 安装包版本库：

1.添加一行文字到你的/etc/opkg.conf中，即将LuCI添加到版本库中:

src luci http://downloads.openwrt/kamikaze/8.09.2/YOUR_ARCHITECTURE/packages

2.输入 [opkg](http://baike.baidu.com/view/4232809.htm) update

3.LuCI 简版，输入: [opkg](http://baike.baidu.com/view/4232809.htm) install luci-light

LuCI 普通版: [opkg](http://baike.baidu.com/view/4232809.htm) install luci

自定义模块的安装: [opkg](http://baike.baidu.com/view/4232809.htm) install luci-app-*

4.为了实现HTTPS支持，需要安装luci-ssl meta安装包

5.由于opkg-installed服务是默认关闭的，你需要手动开启使它能够开机启动：

root@OpenWrt:~# /etc/init.d/uhttpd enable

root@OpenWrt:~# /etc/init.d/uhttpd start




