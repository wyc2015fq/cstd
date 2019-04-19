# Swift4 新特性一览 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月22日 13:22:13[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：547标签：[swift4新特性](https://so.csdn.net/so/search/s.do?q=swift4新特性&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
![](http://upload-images.jianshu.io/upload_images/2340489-d90e1c85189b14ec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
本篇为 2017 WWDC Session 402 笔记，原视频在[这里](https://developer.apple.com/videos/play/wwdc2017/402/)
原文地址：[http://www.jianshu.com/p/169a34de7532](http://www.jianshu.com/p/169a34de7532)
## 语法特性
- 
Swift3中，如果将主体函数的变量定义为private，则其extension无法读取此变量，必须将其改为filePrivate才可以，但是权限又被扩大了。在Swift4中，private变量也可以被其extension读取。
- 
类和接口的组合语法支持
例如在代码中，定义了一个接口名为Shakeabe，并让UIButton和UISlide来实现此接口。但是在之后的变量定义时，如果无法确定是UIButton类型还是Slide类型，又不想写多行条件语句来判断，可以在Swift4中用如下方法来搞定：
`var targetControl: [UIControl & Shakeable]`
再例如，在OC中，我们会有如下代码：
`@property (weak) NSView<NSTextInputClient> *client;`
在Swift4中，直接可以用以下语句来实现
`var client: [NSView & NSTextInputClient]?`
## 工具附加特性
- XCode提供了refactor功能
## 编译相关
- 加快了编译速度
- Swift3.2可以和Swift4进行共存，并在XCode中提供版本选择
- 提前编译Swift-OC Bridge，加快速度
- 不再单独进行Indexing，每次编译时会自动index
- 在Size方面自动过滤掉不用的代码，减少体积
- 可以选择 "Strip Swift Symbol"来达到减少编译后的体积
## String相关
- String的很多characters的简介操作直接升级为对String的直接操作,String本身变成了Characters Collector，例如：
```
Swift3:
let values = "One, Tow,Three...."
var i = values.characters.startIndex
```
在Swift4中，变成了
```
Swift4:
let values = "One, Tow,Three...."
var i = values.startIndex
```
- 在遍历String的过程中，如果从下标i的string末尾，通常写成：
`values[i ..< values.endIndex]`
在Swift4中，直接改成了：
`value[i...]`
利用i...这种类型，会自动匹配到string结尾
- 
在String的基础上，增加了SubString类型，为了共享存储提高代码执行效率。但是一般情况下不需要手动来进行创建，会根据String的一些extract相关方法自动返回，其方法列表也和String基本保持一致。
- 
Swift4支持了分段长字符的表达方式，例如以前为：
`var str = "Q: How does the project \(projectName) do in this process? A: It followed these steps: \(step1), \(step2),\(step3)... Q: That's very helpful."`
在Swift4中可以写成：
```
var str = """
                  Q: How does the project \(projectName) do in this process? 
                  A: It followed these steps: \(step1), \(step2),\(step3)... 
                  Q: That's very helpful.
          """
```
看起来更加的结构化。
## 其余特性
- 强制要求对内存变量的访问具有互斥性
例如，在遍历一个数组的过程中如果同时在循环内对数组进行数据操作，会在编译时报错。
## 个人感想
当然，由于Session的时间限制，肯定没有把所有Swift4的特性说全。仅就列出来的这几个，我个人比较喜欢的有 XCode支持Swift的refactor，Swift编译速度加快，以及String本身包含了Characters Collector这几个。
由于Swift的开源属性，也使得Swif借由社区的力量越来越好用以及越来越强大。这个对iOS开发者来说是一个天大的好事。
再加上最近WWDC上release出来的几个强大的SDK，例如CoreML，ARKit等，个人觉得iOS开发者的好日子来了。
## 其它推荐
如果你是Swift新手，可以参考下我之前写的[30天30个Swift小App](http://www.jianshu.com/p/c6ae28964ad5)
相关Github地址为：
[https://github.com/nimomeng/30-swift-projects-in-30-days](https://github.com/nimomeng/30-swift-projects-in-30-days)
