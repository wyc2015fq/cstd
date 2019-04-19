# Xcode8调试黑科技：Memory Graph实战解决闭包引用循环问题 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月17日 11:46:52[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：982
![](http://upload-images.jianshu.io/upload_images/225849-bae8c1f121559208.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Xcode8的调试技能又增加了一个黑科技：Memory Graph。简单的说就是可以在运行时将内存中的对象生成一张图。在现场的开发者听到了这个消息时响起了雷鸣般的掌声！我们来看看前方记者发回的现场照片：
![](http://upload-images.jianshu.io/upload_images/225849-329870fb58442793.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
妈妈说再也不用担心引用循环啦！除非你是个瞎子。
![](http://upload-images.jianshu.io/upload_images/225849-bcd3951be806cc9c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
那么通过一个实际项目来练习一下吧。
首先我们写了一个自定义UIView：MyView。初始化的时候接收一个没有参数也没有返回值的闭包作为参数，并存为自己的属性：
```
typealias Action = () -> Void
class MyView: UIView {
    var action: Action?
    init(action: @escaping Action) {
        self.action = action
        super.init(frame: CGRect.zero)
    }
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}
```
接着我们在一个ViewController中初始化MyView，并且也保存为属性：
```
class ViewController: UIViewController {
    @IBOutlet weak var label: UILabel!
    var myView: MyView?
    override func viewDidLoad() {
        super.viewDidLoad()
        myView = MyView(action: testMethod)
    }
    func testMethod() {
        label.text = "haha"
    }
}
```
这vc的view上有一个label控件，在viewDidLoad时初始化myView，并且将自身的一个**testMethod方法**当做参数传给了myView。
testMethod中设置了**自身**label的text。
**注意，划重点了！**
![](http://upload-images.jianshu.io/upload_images/225849-86a4f597c9236f2d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这里体现了swift函数式的特性：函数可以自由的当做一个变量传递，并且是值类型。
这个例子影射里开发中一个常见的场景：一个tableViewCell中有一个删除按钮，通过闭包将方法传进去，cell保存这个闭包；另一方面这个闭包被调起后，删除某条数据后刷新数据源。
那么这么写会产生引用循环吗？
```
func testMethod() {
        label.text = "haha"
    }
```
核心在这段代码上，一个类的方法里设置自身的属性，会捕捉这个属性吗？这个地方可以写self，但是捕捉策略是unowned还是strong呢？
这个闭包的实现是不能自己声明捕捉策略的：
![](http://upload-images.jianshu.io/upload_images/225849-2cea575679d5fa3b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
于是就来验证一下。运行起来后，push这个ViewController后pop出去（记得要进行两次，好像只有一次Xcode有时不会启动分析）。
接着点击这个按钮：
![](http://upload-images.jianshu.io/upload_images/225849-81486a8ba4f85ccd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这个时候就进入了断点模式，可以查看issue面板，注意选择右边Runtime：
![](http://upload-images.jianshu.io/upload_images/225849-5d7c2230bc3d4755.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
有很多叹号说明就有问题了。看内存中object的名字，有一条是Closure captures leaked。展开后点击就可以看到这个issue对应的内存图形展示在中间的面板中。
当然了，我们更多的时候是在debug页面下查看：
![](http://upload-images.jianshu.io/upload_images/225849-d6a25ca375131d85.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
注意到我们刚才的对象名：一个叫MyView，一个叫ViewController。我们pop了两次，按理说内存里不应该有这个两个对象，然而还是有两份实例。所以，这里面引用循环了。点击紫色的叹号会出现Xcode分析出来的内存引用图形：
![](http://upload-images.jianshu.io/upload_images/225849-2c8ba810774336fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
有了这个图就很容易看出来了：myView保持了action，action保持了testMethod，testMethod中因为设置了vc的label所以也保持了VC。所以我们可以确定：**方法中隐式的self的捕捉策略是strong**。这样直接把方法传入子view中会引起引用循环。
# 解决方案
### 1.将逻辑实现在一个匿名闭包里，不实现在类的方法上
这样就可以自己声明捕捉策略。这样的方式使用就和OC的block类似了：
```
myView = MyView(){ [unowned self] in
            self.label.text = "haha"
        }
```
### 2.在匿名闭包中调用方法
不是直接传入testMethod方法，而是在传入的闭包中调用自身的方法：
```
myView = MyView(){ [unowned self] in
            self.testMethod()
        }
```
欢迎关注我的微博：[@没故事的卓同学](http://weibo.com/1926303682)
相关链接：
[WWDC 2016 Session 410 Visual Debugging with Xcode](https://developer.apple.com/videos/play/wwdc2016/410/)
文／没故事的卓同学（简书作者）
原文链接：http://www.jianshu.com/p/f792f9aa2e45
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
