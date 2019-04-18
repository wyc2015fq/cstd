# Ios Android  与 H5 交互 - weixin_33985507的博客 - CSDN博客
2017年11月30日 15:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
一、基本信息介绍
Ios 和 Android两种终端与Javascript的通信机制有些不同：
1. Ios 机制：
（1）Native可直接调用Js，并可直接获取Js的返回值（这点与Android不同）
（2）Js不能直接调用Native，但可以通过反向调用，流程就是先将消息临时存储在Js端，然后通知Native反向调用获取消息内容
（实现原理是利用 shouldStartLoadWithRequest，就是Web的所有URL请求都会触发Native的行为）
2. Android 机制：  
（1）Native可直接调用Js，但不可直接获取Js的返回值（这点与Ios不同）
（2）Native注册addJavascriptInterface 后，Js 可直接调用Native的接口，并获取Native的返回值
（3）通过 shouldOverrideUrlLoading，也还是拦截Web的所有URL请求来达到通信的目的
3.新的进展：
在之前一种叫JSBridge方式的方式解决两种系统与H5交互是一种不错的选择，但是现在随着移动系统的更新出现了一些改变：
Ios在ios7 js能直接调用Native ，ios 已经出到11了，现在完全可以放弃ios7 以下的版本了；
Android4.2以下,addJavascriptInterface方式有安全漏掉，但是现在android  也出到8了，4.2以下的版本也可以放弃了；
4 注意：
Android 4.4之后,通过evaluateJavascript异步调用JS方法,并且能在onReceiveValue中拿到返回值，要是放弃4.4之前的版本有点可惜；
Android JS能调用到已经暴露的api,并且能得到相应返回值
参考：https://www.cnblogs.com/liubei/p/Hybrid.html  http://blog.csdn.net/jacin1/article/details/39993935
