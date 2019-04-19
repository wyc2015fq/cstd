# Server-Side Swift系列 「Intro Vapor2」 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月27日 08:58:06[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：854
![](http://upload-images.jianshu.io/upload_images/2751003-1b0b2c7e23262ec7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Get started with Vapor 2 at [docs.vapor.codes](https://docs.vapor.codes/)
本文来自简书，原文地址:[http://www.jianshu.com/p/24ade96e61e7](http://www.jianshu.com/p/24ade96e61e7)
近日，在Medium上看到Vapor出了2.0版本，并且出了新的文档，新的网站。于是忍不住试了一下，并且这次打算不在本地测试，想在Ubuntu系统上尝试。
### Intro
通过学习这篇文章，你将学习到：
- 在阿里云和腾讯云上搭载Ubuntu云服务器
- 在Ubuntu上配置Vapor2.0以及Swift3.1运行环境
- 了解编译Vapor2.0工程的速度
- 掌握如何编写简单的JSON数据返回API
### Get Start
##### 阿里云、腾讯云搭载Ubuntu服务器端系统
因为绝大部分公司是不可能会用苹果系统来作服务器的，考虑到实用性，这次使用云服务器来运行我们的服务端Swift代码。刚好作为学生能够以廉价买阿里云和腾讯云服务器，所以下手测试了两家云端服务器。
[腾讯云服务器CVM](https://www.qcloud.com/product/cvm)
[阿里云服务器ECS](https://cn.aliyun.com/product/ecs?spm=5176.8142029.388261.28.w5XgKY)
由于Swift暂时只支持Ubuntu运行环境，所以云服务器系统我们选择安装Ubuntu16.04 64位版本，请确定安装正确的系统。
##### 性能比较
对比两种服务器，阿里云的速度慢腾讯云可不止一两倍，而且，搞了很久，阿里云打开不了80窗口，这样我们的服务端代码就起不了作用了，亲测腾讯云成功运行。
#### 配置Vapor2.0以及Swift3.1运行环境
###### 通过ssh来登录我们的服务器
`ssh [name]@公网iP`然后输入密码即可登录。`name`为管理员名，或者你也可以在腾讯云管理中心远程连接服务器。
###### 运行脚本
`eval "$(curl -sL https://apt.vapor.sh)"`
###### 安装Vapor
`sudo apt-get install swift vapor`安装Vapor2.0。
需要注意的是，这两行指令已经默认给我们配置了Swift3.1的环境，我们不需要手动下载Swift3.1，非常方便。
###### 最后
通过一行指令再次确认Vapor2.0是否安装成功
`eval "$(curl -sL check.vapor.sh)"`
###### 如果安装成功，你会看到以下的效果
![](http://upload-images.jianshu.io/upload_images/2751003-b89aa0508b2f5fce.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Success
#### 新建一个Vapor工程
通过一行命令新建一个工程
`vapor new HelloWorld`
第一次新建的时候，你可能会看到
![](http://upload-images.jianshu.io/upload_images/2751003-e81e1ec311c40bd0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
是因为我们还没有设置我们的Git邮箱和名称，通过执行以下两行命令即可修复错误。
`git config --global user.email "you@example.com"`
`git config --global user.name "Your Name"`
这时候我们可以重新新建一个工程`vapor new [name]`
`cd`到我们的工程文件中，运行
```
vapor
 build
```
![](http://upload-images.jianshu.io/upload_images/2751003-f68476f54b94d77e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
可怕的等待
然后就看云服务器的造诣了，在前面的文章里面，我写过一篇在苹果系统本地测试我们的Vapor，当我运行`vapor build`大约都要等个5到10分钟。在阿里云测试我们的Vapor时，我大约也要等10来分钟，但是腾讯云我测试大约要3分钟，速度明显差异。
##### 成功编译
![](http://upload-images.jianshu.io/upload_images/2751003-ca4f76d05ffd107d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
成功编译
Vapor目录结构
```
Hello
├── Config
│   ├── app.json
│   ├── crypto.json
│   ├── droplet.json
│   ├── fluent.json
│   └── server.json
├── Package.pins
├── Package.swift
├── Public
├── README.md
├── Sources
│   ├── App
│   │   ├── Config+Setup.swift
│   │   ├── Controllers
│   │   │   └── PostController.swift
│   │   ├── Droplet+Setup.swift
│   │   ├── Models
│   │   │   └── Post.swift
│   │   └── Routes.swift
│   └── Run
│       └── main.swift
├── Tests
│   ├── AppTests
│   │   ├── PostControllerTests.swift
│   │   ├── RouteTests.swift
│   │   └── Utilities.swift
│   └── LinuxMain.swift
├── circle.yml
└── license
```
需要注意的是，Vapor2.0的文件结构和Vapor1稍有差别，确实是Less Code，More Power.
`cd Sources/App`cd 到App目录下
运行`vim Routes.swift`查看该文件代码，我们可以发现
```
import Vapor
final class Routes: RouteCollection {
    func build(_ builder: RouteBuilder) throws {
        builder.get("hello") { req in
            var json = JSON()
            try json.set("hello", "world")
            return json
        }
        builder.get("plaintext") { req in
            return "Hello, world!"
        }
        // response to requests to /info domain
        // with a description of the request
        builder.get("info") { req in
            return req.description
        }
       builder.get("*") { req in return req.description }
        try builder.resource("posts", PostController.self)
    }
}
/// Since Routes doesn't depend on anything
/// to be initialized, we can conform it to EmptyInitializable
///
/// This will allow it to be passed by type.
extension Routes: EmptyInitializable { }
```
对比上个版本，确实是简洁了很多。
补充：退出`vim Routes.swift`命令可以按`Shift健+Q`再输入`wq!`退出来。
##### 最后，Run Server
先cd回工程根目录下，并执行
`vapor run serve`
![](http://upload-images.jianshu.io/upload_images/2751003-6a1a5cfa8454491b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
vapor run serve
##### 测试API数据返回
在浏览器上输入
`[你的云服务器公网IP]:8080/hello`
![](http://upload-images.jianshu.io/upload_images/2751003-2261179ac565c134.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
哇哈哈，成功啦
### Vapor 2：Less code, more power.
Vapor2添加了很多新的库
![](http://upload-images.jianshu.io/upload_images/2751003-6fa6b342cafc3da0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Some of the new features in Vapor 2.
相比Vapor1.5，快速大概3倍，而且比PHP，Ruby快近乎100倍
![](http://upload-images.jianshu.io/upload_images/2751003-3197f922dba35321.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Vapor 2 is 3x faster than Vapor 1 for plaintext responses.
### 写在最后
本次是Vapor2.0的开篇，大家可以上[Vapor2.0新文档](https://docs.vapor.codes/2.0/)查看新版介绍。往后会详细讲Vapor2的功能介绍和应用，喜欢请给红心，也是对我的鼓励。
