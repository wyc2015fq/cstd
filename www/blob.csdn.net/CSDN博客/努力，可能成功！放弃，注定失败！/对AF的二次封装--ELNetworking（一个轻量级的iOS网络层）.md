# 对AF的二次封装--ELNetworking（一个轻量级的iOS网络层） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月08日 17:29:42[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1239
> 
在这篇文章的开头呢我想说一下适合看这篇文章的朋友，大概就是刚培训出来的or工作几月还用着培训时候老师封装的网络层。说起这个，现在北上广的it行业已经被各种机构搞得不能在乱了，苦了我们这些入坑晚的孩子。好了，小小吐槽一下，接下来就进入正题吧。
### 移动端所用到的HTTP请求
关于HTTP协议，我们初期学习只需要知道最基本的内容即可。就像http响应码，了解常见的几种200，404，502，503就可以了。具体内容网上一大堆，我就不在这啰嗦了。我们来看一下使用AFNetworking发起一次请求我们所需要做的事情：
- 创建HTTPSessionManager
- 设置baseURL（也就是所请求服务器的域名）
- 确定请求地址URL
- 构造请求参数（GET：拼接在url后 POST：封装到请求体中）
- 发起请求
![](http://upload-images.jianshu.io/upload_images/1770772-48335f7cad9d8a59.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1C446D67-3407-4013-9003-03E21DBAE1BA.png
就像上面这样，大多数初学者会对AF再进行一次封装，无非是写个工具类，然后传入请求方法、地址、请求体、完成回调，然后就没了。
如果单单论使用，这么封装确实勉强够用。但是考虑到业务层以及维护，这么封装的缺点就完全暴露了出来：
**1.url的维护**
这种封装通常需要一个.h文件里面放上N多个宏。 每个宏定义一个接口的请求地址，长此以往，.h里的宏越来越多，你自己看了都想吐，还谈什么维护。
**2.参数的维护**
如果接口全是get还好，如果有POST，构造请求体的代码就会散落各处，而且好像都得写控制器里，越写越乱。
**3.不方便调试**
这个自行感觉~
**4.请求参数的验证**
如果在某次请求前，接口所需要的某个参数不存在，那么这次请求必然会失败，就没必要去发起请求了，比如用户注册时所输入的账号密码等等。验证的逻辑写在哪里又是个问题。
**5.功能扩展受限**
需求是源源不断的，比如说现在需要给接口添加缓存功能，这种封装方式还真不知道该怎么下手。
### 设计一个好用易扩展的网络层
![](http://upload-images.jianshu.io/upload_images/1770772-65d5d63fd760eca9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
rua.png
OK，我来大致介绍一下上面这张图。
#### BaseAPI
作为网络请求API的基类，内部处理接口的url拼接（GET）、请求体的创建、参数验证、缓存的处理、请求数据完成后的回调等。每个接口创建一个类，都通过BaseAPI派生出来。
#### NetworkManager
对AF的一层抽象，内部在请求成功或失败后重新整合响应数据到Response对象中，同时将该对象以及请求到的原始数据回调给BaseAPI。
#### Response
重新整合后的数据，包括响应码、响应信息、响应数据、响应状态。目的：方便业务层调试以及缓存
#### NetworkProtocol
网络层协议，包括接口的基本配置（methodName）、域名配置（domain）、参数（params）、缓存配置、参数校验等等。 
为什么这么做？ 接口的配置选择度更高，接口想要什么功能就遵循什么协议。比如说实名认证的接口，需要传身份证参数，如果用户的输入不是身份证号码那么这次接口就没必要发起请求了，白白浪费服务器一次响应。这个接口类就可以遵循该类中的验证参数的协议，然后验证参数的逻辑就写在了相应的接口里，合情合理。
后续如果想加更多可选功能，可以通过增加协议来实现，扩展性就在这里体现出来了。
**说了这么多，先秀一哈，使用起来是这样的**
新建API继承自BaseAPI
.h
![](http://upload-images.jianshu.io/upload_images/1770772-3ce5ea1aaa4f6c28.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
API interface.png
.m
APIParams这个方法是@Optional的，因为并不是所有接口都需要参数。。。
![](http://upload-images.jianshu.io/upload_images/1770772-d25ddba853303e82.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
API implementation.png
控制器中的代码看起来是这样的
![](http://upload-images.jianshu.io/upload_images/1770772-afafff22d0d82ecc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Use of API.png
当然你还需要在控制器里遵循API的请求回调代理
![](http://upload-images.jianshu.io/upload_images/1770772-3ffbfd79880d8761.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
BaseAPIDelegate.png
忘了说，在这里我采用的是代理回调数据，(⊙v⊙)嗯其实我感觉代理还是block都可以，随大家喜欢。
#### Tip. 有的同学可能会问，你的回调函数就一个，如果一个页面多个接口怎么区分呢，你不就撒比了吗？
NoNoNo, 回调中可以获取到api对象，通过[api isMemberOfClass:[XXXAPI class]]来就可以区分不同的接口回调了，而且这么写回调代码统一集中，debug以及维护还很方便呢。
#### 我们试着进行一次请求，这是控制台的log，不需要打太多断点，方便调试。。
![](http://upload-images.jianshu.io/upload_images/1770772-700279cbb969349b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
API log.png
### 来看一下进行一次请求所经历的事情
![](http://upload-images.jianshu.io/upload_images/1770772-6fc9feec10c241bd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
http-steps.png
### 最后
关于文中提到的功能还有一部分未实现，部分实现也不是最好的方式，不过基本的使用是足够了，这篇文章也算是抛个砖，大家有什么好的想法以及建议可以在评论中指出，感激不尽。
喜欢的朋友欢迎到这里[ELNetworking](https://github.com/liyonghui16/ELNetworking.git)下载，顺便点个star资瓷我一下。如果有朋友需要，后续我会逐步实现未实现的功能。
文／HUswIft（简书作者）
原文链接：http://www.jianshu.com/p/4ae3ea16e0f7
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
