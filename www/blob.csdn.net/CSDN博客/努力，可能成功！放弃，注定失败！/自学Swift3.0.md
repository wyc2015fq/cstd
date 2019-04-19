# 自学Swift3.0 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月15日 13:37:29[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：749
本文来自简书，原文地址:[http://www.jianshu.com/p/a09ce22ffc46#](http://www.jianshu.com/p/a09ce22ffc46#)
我的经历可能和大部分专职做开发的程序员不一样，所以这份学习经过，也和网络上大部分基础教程不同，如果你刚好非科班出生，也有这个业余爱好，对于iOS编程也没有任何基本知识的了解，这份资料可能对你很有意思。
我自学过的唯一一门语言是Python，主要原因是：你去搜索0基础，学一门编程语言，很多地方都会向你推荐它，入门简单，整个写代码过程很容易理解，没有长长的关键词，也没有复杂的IDE，而且一处编写，各种电脑都能运行，最最关键的是拥有各种第三方库。基本掌握了Python的语法后，学了一点Django的框架和各种第三方库，了解RESTfull的概念，运用Python最多的就是一些数据处理，网络爬虫，大多都是没有界面的“程序”，但是也能解决工作中遇到的一些需求，
所以学习Swift基本是从0开始，今年上半年Swift推出2.0的时候，就很想业余时间学学iOS开发，感觉能做出一个有界面的程序会更有意思，所有的学习过程基本也就是通过网络搜索开始，2016年初，粗略看过 [Swift
 Language Guide 中文版](http://wiki.jikexueyuan.com/project/swift-language-guide/)但是因为基础语法看起来比较枯燥，而且总体感觉和Python没啥很大区别，加上工作也忙，最后得知Swift3.0将和2.0不兼容，所以就暂停了，准备在Swift3.0的时候，再尝试一下。
待Swift3.0发布，目前开始学习，所有学习经过会一直在这里更新。这个学习路径也是我目前认为最适合0基础，学习iOS的一个路径。
#### 了解Xcode，对Swift有点兴趣
兴趣是最好的老师，但是学习的第一步往往很艰难，大部分Swift的教程都比较枯燥，但是下面这个例子，却真的做到了深入浅出。虽然教程只有10个视频，作者不知道是不是换了女朋友，最后教程没有更新下去。
学习内容：
[给女朋友的 iOS 开发教程](http://space.bilibili.com/3517995)
学习时间：2天
学习难度：★★☆☆☆
#### 了解闭包
我在前期看了Swift Language Guide，发现其中最难理解的就是闭包和各种闭包的缩略写法，但是Guide也没有把这个事情讲的很清楚，所以理解这概念就显得尤为突出。
学习内容：
[Swift 闭包趣事谈](http://swiftcafe.io/2016/07/19/funny-closure/)
[SwiftCafe 快报 - 了解闭包](http://swiftcafe.io/2015/10/10/swift-daily-closure/)
[Swift 中的利刃函数和闭包](http://swiftcafe.io/2015/02/14/swift-tips-func-closure/)
学习时间：1天
学习难度：★★★☆☆
因为这些博客已经有些光景了，部分代码已经不能在3.0环境运行，例如下面这段写法已经不能运行了，在Xcode运行的时候可以自行修改，例如：
```
let sum: (left:Int, right:Int) -> Int = { (left:int, right:Int) -> Int in
  return left + right
}
```
修改为：
```
let sum: (Int, Int) -> Int = { (left:int, right:Int) -> Int in
  return left + right
}
```
#### 了解Swift语法
可以按照自己的理解和随意看一部分，不用全部看完，了解这个手册的目录，以后遇到问题知道在这里查找就好了，不需要全部记住。
[Swift Language Guide](http://wiki.jikexueyuan.com/project/swift-language-guide/)
学习时间：2天
学习难度：★★☆☆☆
#### 了解CocoaPods
了解包管理工具的安装，会使用就可以了。
[用CocoaPods做iOS程序的依赖管理](http://blog.devtang.com/2014/05/25/use-cocoapod-to-manage-ios-lib-dependency/)
学习时间：1天
学习难度：★☆☆☆☆
#### 了解第三方库
因为我新学Swift，所以只推荐纯的Swift库。
[Swift 有哪些优秀的第三方库？](https://www.zhihu.com/question/28816900)
[GitHub第三方资源库整理(Swift篇)](http://www.jianshu.com/p/68e12b966d86)
学习时间：1天
学习难度：★★☆☆☆
#### 了解RxSwift
响应式编程，理解响应式编程的概念，这个概念和AngularJS，Vue很像。为以后写出简洁的代码做准备。
[移动开发响应式编程初探 - RxSwift](http://www.jianshu.com/p/83acb603af5e)
学习时间：1天
学习难度：★★★☆☆
