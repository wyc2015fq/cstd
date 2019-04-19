# 是时候了解一下UILayoutGuide了 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年01月05日 10:28:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1366
本文来源于简书，原文地址：[http://www.jianshu.com/p/b5c3e0482f29](http://www.jianshu.com/p/b5c3e0482f29)
UILayoutGuide是 iOS 9 中增加的帮助开发者在使用auto layout布局时的一个虚拟占位对象。
先来看一个在布局时可能碰到的一个场景：有两个等宽的按钮，这两个按钮中间的空隙与它们到左边、右边边缘的距离相等。这样用auto layout要怎么做呢？
![](http://upload-images.jianshu.io/upload_images/225849-a7b119b8dbb396dc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
约束中有相等的约束，但是并没有margin相等的约束，显然我们无法直接通过配置约束达到需求。
一种很常见的思路就是把边距转换为一个占位的View，这样就可以设置相等的属性了。但是这个View要设置成hidden，不应该被用户看到。这样一来我们就可以设置这三个虚拟的View宽度相等，再依次设置他们的leading、trailing约束就可以达到所要的结果了。
也有一种情况：两个控件，需要两个成组后居中。这种情况下我们也会引入一个View，把这两个控件加入到这个容器View中，再设置容器View的居中。
但是这样为了辅助布局的View有一个让人难受的地方：它在View的层级里，在事件路由的时候还是会收到消息。这就是UILayoutGuide想要解决的问题。UILayoutGuide可以被添加到View中，和View一样可以设置各种约束，参与布局。但是它不会被渲染出来，不会响应事件路由。也可以像box一样直接包含一些View。
下面直接用代码演示一下如何把两个控件设置居中。使用的是Snapkit做约束，没有使用原生的蹩脚API。
首先我们先初始化两个View和一个LayoutGuide。
```
let label = UILabel()
        label.text = "UILabel"
        view.addSubview(label)
        let testView = UIView()
        testView.backgroundColor = UIColor.cyan
        view.addSubview(testView)
        let container = UILayoutGuide()
        view.addLayoutGuide(container)
```
LayoutGuide被添加进View使用的是` addLayoutGuide`方法。
接着我们设置label、testView和container的边缘相连接，并且它们之间有20的offset。
```
label.snp.makeConstraints { (make) in
            make.left.equalTo(container)
            make.centerY.equalTo(container)
        }
        testView.snp.makeConstraints { (make) in
            make.left.equalTo(label.snp.right).offset(20)
            make.height.equalTo(40)
            make.width.equalTo(120)
            make.right.equalTo(container)
            make.centerY.equalTo(container)
        }
```
这么一来，container的宽度就等于两个view的宽度加上20的offset。两个view的centerY和container一致。我们接着设置container的约束就可以了：
```
container.snp.makeConstraints { (make) in
            make.height.equalTo(5)
            make.center.equalTo(view)
        }
```
运行起来就能达到目的了：
![](http://upload-images.jianshu.io/upload_images/225849-8aad27861058dfea.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
所有需要一个虚拟View帮助的事情都可以交给UILayoutGuide来做。它更轻量、更快速、更高效。UILayoutGuide并没有真正的创建一个View，只是创建了一个矩形空间，只在进行auto layout时参与进来计算。
最后再提醒一遍，这是 iOS 9 的新特性。
愿上帝保佑那些还要支持 iOS 7 的孩子。
欢迎关注我的微博：[@没故事的卓同学](http://weibo.com/1926303682)
