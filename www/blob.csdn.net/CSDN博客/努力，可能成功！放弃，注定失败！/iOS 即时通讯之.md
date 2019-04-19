# iOS:即时通讯之 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月08日 17:57:52[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4122
# 什么是环信?
**1.环信是一个第三平台，提供即时通信(IM–Instant Messaging )的服务**
**2.环信是在XMPP的基础上进行二次开发**
**3.环信在网络上传输的数据也是XML**
**4.使用环信，不用自己搭建服务器，节约成本**
**5.环信日活30万以下,永远免费**
# 为什么要选择环信?
数据来源艾瑞:[点击跳转](http://www.cctime.com/html/2015-6-8/2015681059556379.htm)
数据来源易观:[点击跳转](http://www.ithome.com/html/it/175235.htm)
![](http://upload-images.jianshu.io/upload_images/1829339-aa8e7b8b96773111.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1829339-538022623bd02428.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 集成环信前提准备:
1.注册成为环信开发者[点击进入环信](http://www.easemob.com/)
![](http://upload-images.jianshu.io/upload_images/1829339-2b5b8fb7de2bfaa7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2.在开发者后台创建APP获取Key
![](http://upload-images.jianshu.io/upload_images/1829339-2dcdd208f9b652c9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
3.下载官方SDK DEMO[点击查看](http://www.easemob.com/download)
![](http://upload-images.jianshu.io/upload_images/1829339-5ea9d52417e4c627.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 集成SDK步骤:
1.导入SDK
![](http://upload-images.jianshu.io/upload_images/1829339-0e80fa01d702db0c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2. 设置工程属性
![](http://upload-images.jianshu.io/upload_images/1829339-6f0403f8ebdb0df2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

2.2 SDK依赖库有
![](http://upload-images.jianshu.io/upload_images/1829339-1706d8e3bbaf4298.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**注意:(**如果使用的是xcode7，后缀为tbd**) <官网有提示>**
2.3 如果项目中使用-ObjC有冲突,可以添加-force_load来解决
![](http://upload-images.jianshu.io/upload_images/1829339-8e4caffd6c98dcfa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1829339-45011700e433b1ee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2.4 SDK不支持bitcode,向Build Settings → Linking → Enable Bitcode中设置NO。
# 编译工程[](http://docs.easemob.com/im/300iosclientintegration/20iossdkimport#%E7%BC%96%E8%AF%91%E5%B7%A5%E7%A8%8B)
以上步骤进行完后，编译工程，如果没有报错，恭喜你，集成sdk成功，可以进行下一步了。
# SDK同步/异步方法区分:
SDK中，大部分与网络有关的操作，提供的是同步方法(注:同步方法会阻塞主线程,需要用户自己创建异步线程执行;带有async的方法为异步方法)
# 初始化SDK
1.引入相关头文件 #import “EMSDK.h”
2.在工程的AppDelegate中的以下方法中，调用SDK对应方法:
![](http://upload-images.jianshu.io/upload_images/1829339-9b4f3d610ebb0839.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### 调用的SDK接口参数解释如下：
1.AppKey: 区别app的标识，[开发者注册及管理后台](http://docs.easemob.com/start/000quickstart/10register)
2.apnsCertName: iOS中推送证书名称。[制作与上传推送证书](http://docs.easemob.com/start/300iosclientintegration/10prepareforsdkimport)
3.环信为im部分提供了apns推送功能，如果您要使用，请跳转到[apns离线推送](http://docs.easemob.com/start/300iosclientintegration/80apns)
# 注册:
1.注册模式分两种，开放注册和授权注册。只有开放注册时，才可以客户端注册。
2.开放注册是为了测试使用，正式环境中不推荐使用该方式注册环信账号， 授权注册的流程应该是您服务器通过环信提供的rest api注册，之后保存到您的服务器或返回给客户端。
![](http://upload-images.jianshu.io/upload_images/1829339-d2d8fcb092608efe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 登陆:
登录：调用SDK的登录接口进行的操作；
![](http://upload-images.jianshu.io/upload_images/1829339-231d846d99e1dbb0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 自动登录:
自动登录：即首次登录成功后，不需要再次调用登录方法，在下次app启动时，SDK会自动为您登录。并且如果您自动登录失败，也可以读取到之前的会话信息。配置是否进行自动登录SDK中自动登录属性默认是关闭的，需要您在登录成功后设置，以便您在下次app启动时不需要再次调用环信登录，并且能在没有网的情况下得到会话列表。
![](http://upload-images.jianshu.io/upload_images/1829339-a7c2ce2fb5d81246.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**自动登录在以下几种情况下会被取消**
> 
1.用户调用了SDK的登出动作;
2.用户在别的设备上更改了密码, 导致此设备上自动登陆失败;
3.用户的账号被从服务器端删除;
4.用户从另一个设备登录，把当前设备上登陆的用户踢出.
所以，在您调用登录方法前，应该先判断是否设置了自动登录，如果设置了，则不需要您再调用
![](http://upload-images.jianshu.io/upload_images/1829339-a80b6ea4ad0ccbbc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
SDK中，如果发生自动登录，会有以下回调
![](http://upload-images.jianshu.io/upload_images/1829339-96057be20205a7c7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 重连:
当掉线时，IOS SDK会自动重连，只需要监听重连相关的回调，无需进行任何操作。
![](http://upload-images.jianshu.io/upload_images/1829339-c85c909c5d3dee18.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 退出登陆:
退出登录分两种类型：主动退出登录和被动退出登录。
> 
主动退出登录：调用SDK的退出接口；
被动退出登录： 1、 正在登陆的账号在另一台设备上登陆； 2、 正在登陆的账号被从服务器端删除。
logout:YES：是否解除device token的绑定，在被动退出时SDK内部处理,不需要调用退出方法
![](http://upload-images.jianshu.io/upload_images/1829339-91e5f9b17dc111d7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 被动退出登录:
回调方法监听:
![](http://upload-images.jianshu.io/upload_images/1829339-9a8e611de75b9d66.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
还有很多很多功能,而且[官网](http://docs.easemob.com/im/300iosclientintegration/40emmsg) 写的非常非常清楚!
# 好友管理[](http://docs.easemob.com/im/300iosclientintegration/50buddymgm#%E5%A5%BD%E5%8F%8B%E7%AE%A1%E7%90%86)
注：环信不是好友也可以聊天，不推荐使用环信的好友机制。如果你有自己的服务器或好友关系，请自己维护好友关系。
获取好友列表[](http://docs.easemob.com/im/300iosclientintegration/50buddymgm#%E8%8E%B7%E5%8F%96%E5%A5%BD%E5%8F%8B%E5%88%97%E8%A1%A8)
获取好友列表，环信提供了两种方法。
1. 从服务器获取所有的好友
![](http://upload-images.jianshu.io/upload_images/1829339-fb6165e3f7242bae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2. 从数据库获取所有的好友
![](http://upload-images.jianshu.io/upload_images/1829339-539429e5ae21c9e4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 好友申请[](http://docs.easemob.com/im/300iosclientintegration/50buddymgm#%E5%A5%BD%E5%8F%8B%E7%94%B3%E8%AF%B7)
# 发送加好友申请
环信iOS SDK提供了添加好友的方法
如果您已经发过，并且对方没有处理，您将不能再次发送
![](http://upload-images.jianshu.io/upload_images/1829339-f7c471185ac3d64a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 实时通话管理
### 客户端
1.发起实时通话
![](http://upload-images.jianshu.io/upload_images/1829339-cebac79d514645da.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2.被叫方同意实时通话
![](http://upload-images.jianshu.io/upload_images/1829339-4abb6bfcbc200cc9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
3.结束实时通话
![](http://upload-images.jianshu.io/upload_images/1829339-b5b8cc7e882cd0eb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
4.实时通话实例
![](http://upload-images.jianshu.io/upload_images/1829339-67f65ac08a131585.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### 等等等...   [点击查看详细](http://docs.easemob.com/im/300iosclientintegration/71call)
# EaseUI使用指南
EaseUI是一个UI库，封装了IM功能常用的控件、fragment等等，旨在帮助开发者快速集成环信sdkeaseui及demo的github下载地址为：[](https://github.com/easemob/easeui)
[https://github.com/easemob/easeui](https://github.com/easemob/easeui)；
[https://github.com/easemob/sdkdemoapp3.0_android](https://github.com/easemob/sdkdemoapp3.0_android)，3.0的代码需要切换到sdk3.0分支
**注意：**因为这是一个ui库，后续很可能还会继续改动，新旧版本在api的兼容上不会像im sdk那样绝对的兼容。(官网有提示)
# 代码导入
easeui库的代码是开源的，下载的sdk压缩包里面已经包含此库，解压后路径为/examples/easeui
**Eclipse中导入：**
1.先把easeui项目导入到eclipse中
2.在自己的项目中把easeui作为一个library引入。右键你的项目→Android→点击右下角的Add按钮→选中EaseUI→OK→OK
**Android studio中导入：**打开你的AS项目→File→New→Import Module→选择或输入easeui库路径→Next→Next→Finish
导入完可能会有如下错误：
![](http://upload-images.jianshu.io/upload_images/1829339-1b793425f17824da.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
找到相应报错的地方把swipeRefreshLayout.setColorSchemeResources改成swipeRefreshLayout.setColorScheme，ViewCompat.getX改成new ViewCompat().getX,然后重新build即可；**或者**把v4包的版本号加大，譬如compile 'com.android.support:support-v4:23.1.1'。
# 主要fragment
1.EaseConversationList - 聊天页面，最主要的fragment
2.EaseContactListFragment - 联系人页面
3.EaseConversationListFragment - 会话列表页面
![](http://upload-images.jianshu.io/upload_images/1829339-8673f66e277ad1d6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 主要控件[](http://docs.easemob.com/im/200androidcleintintegration/135easeuiuseguide#%E4%B8%BB%E8%A6%81%E6%8E%A7%E4%BB%B6)
1.EaseTitleBar - 标题栏
2.EaseChatMessageList - 聊天消息列表控件
3.EaseConversationList - 会话列表控件
4.EaseContactList - 联系人列表页面
5.EaseChatInputMenu - 聊天输入菜单栏
其他子控件，后文会做详细介绍
这里对聊天页面几个控件做简单图示：
![](http://upload-images.jianshu.io/upload_images/1829339-91a5e21fda6949aa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 类似这些功能:
![](http://upload-images.jianshu.io/upload_images/1829339-e1f5dc93a00f6ae0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1829339-7f1db8c340f35d4b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1829339-878ebf7bb47ceac9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这里都有介绍,只不过觉得这么好的东西放在官网没人看,太可惜了...[点击查看详细](http://docs.easemob.com/im/200androidcleintintegration/135easeuiuseguide)
# API大致变动介绍:
![](http://upload-images.jianshu.io/upload_images/1829339-4272dc14da14fcdf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
都是官网的东西,与君共勉!
