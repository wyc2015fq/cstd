# Vue 全家桶 + Electron 开发的一个跨三端的应用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月22日 13:41:36[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5536
![](http://upload-images.jianshu.io/upload_images/1194012-212d2573a0b8f6ac.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
GitHub Repo：[vue-objccn](https://github.com/halfrost/vue-objccn)
Follow: [halfrost · GitHub](https://github.com/halfrost)
项目地址：[https://github.com/halfrost/vue-objccn](https://github.com/halfrost/vue-objccn)
[本文来自简书，原文地址:http://www.jianshu.com/p/bff94aa90836](http://www.jianshu.com/p/bff94aa90836)
利用 Vue.js 实现 [objc中国](https://objccn.io/) 的跨平台全栈应用
- ✅ 桌面应用，支持 Mac、Linux、Windows 三个平台
- ✅ Web 应用，支持 桌面浏览器 和 手机浏览器
- ✅ 手机 App，目前只支持了 Cordova 框架，支持 iOS 和 Android 两个平台
- ❌ 手机原生 App，打算用 Weex 框架，同样一起支持 iOS 和 Android 两个平台
> 
注：此项目纯属个人瞎搞，请大家支持 喵神(@onevcat)，支持 [Objc中国](https://objccn.io/)。
# 前言
## 一.关于我
我是一名全职的 iOS 开发者，非前端开发者。由于接触了 Weex 开发，从而接触到了 Vue.js。
## 二.为什么会写这个项目？
- 
最开始有这个想法的时候是来自一个网友，他在我的博客上问我，网上有没有写的比较好的 demo ？我说尤大写的那个 Hacker News 是最好的。后来网友就是，楼主能写一个么？我当时回答暂时不行。其实这事我一直记在心里。
- 
今年5月19号，GitHub 使用 Electron 重写了 macOS 和 Windows 的客户端，加上近些年跨端开发越来越火，对于一些公司来说，Web 和 app 应该都是需要的，app 还需要 iOS 和 Android 两个平台，再有甚者还要开发小程序，桌面级的应用虽然少，但是用 Electron 一样可以一起开发了。自己也萌生了想要跃跃欲试的念头。
- 
由于接触到了 Vue.js，当然不想停留在初级，想进阶，尤大给出了建议，就是多实践，多练。为了加快进阶的步伐，自己私下就找项目练。
- 
至于为何选择 Objc 中国，理由其实很简单，因为我是 iOS 开发者。在 iOS 开发者中，Objc 基本上人尽皆知（有不知道的？），喵神也基本上人尽皆知，我个人很崇拜喵神，所以就选了 Objc 中国来写。
- 
因为爱 ... ...
## 三.这次为何跨端开发没有weex？
这次在我写完项目以后，发现 Vue 的代码直接转换成 Weex 的项目，是无法实现的，好多报错。而且不是一下子能都修复好。我相信是我使用姿势的问题，不是 Weex 的问题。对了，Weex 又发布新版本了，接下来有时间的话就把 Weex 版的也做一遍开源。
好了，进入正题，说项目：
# 技术栈和主要框架
