# ios开发之UI基础 - LC900730的博客 - CSDN博客
2017年06月27日 23:18:22[lc900730](https://me.csdn.net/LC900730)阅读数：184
## 1.初始阶段：
1.core data某些数据可以放在本地数据库； 
2.界面控制器相当于是界面管家；(管理view和view里面的东西) 
3.
## 2.启动过程
plist中设置了的话，加载Main.storyboard,加载箭头指向的控制器，加载控制器中的view，显示给用户
is intial View Controller
每个控制器都会有个类，也就是我们的ViewController已经和Mainstoryboard绑定；
IBAction： 
从返回值来看，是IBAction，类似与void； 
只有返回是IBAction,才能与storyboard中控件进行连线。
一个控制器管理一个界面 
IBAction:方法和storyboard中控件进行连线，相当于void 
@property(nonatomic,weak) IBOutlet UILabel *label; 
IBOutlet：修饰属性 
IB:interface builder,Xcode4开始，将IB整合到Xcode中
self.redButton.backgroundColor=[UIColor redColor];
判断一个对象能否连线给方法： 
1.判断对象所在的类是否继承自UIController； 
2.看对象是否有setevents 
UIButton继承自UIController，UILabel继承自UIView
## 连线中的错误：
错误一：找不到键值对； 
reason：[MainViewController 0xxxx setValue：forUndefinedKey:] 
原因：有多余连线，注释掉了，但是连线还存在； 
错误二：找不到方法 
reason：[MainViewController clickBtn:]:unrecognized selector sent to instance 0xxxx];
## 控件
屏幕上所有的UI元素就是控件，也有人叫做视图、组件； 
按钮(UIButton)、文本(UILabel)都是控件； 
常见属性： 
控件的共同属性有哪些？ 
尺寸 
颜色 
背景色 
苹果将控件的共同属性都抽取到父类UIView中
每个控件其实都是一个容器，可以将其他控件放到该控件的内部，比较常见的还是将UIView作为容器； 
每一个控制器都有一个UIView，控制器本身是不可见的，能够看到的是控制器的View，每一个控制器的都有UIView的属性，控制器中管理的所有子控件都是该控件的子控件；
@property(nonatomic,readonly)UIView *superview: 获取自己的父控件对象；
@property(nonatomic,readonly,copy)NSArray *subviews: 获取自己的父控件对象；
ViewController中的ViewDidLoad方法和didReceiveMemoryWarning时候； 
加载控制器先调用loadview方法 
然后是 
//1.系统调用;2.控制权的view加载完毕的时候调用；3.控件初始化，数据初始化(懒加载)； 
viewDidLoad
然后viewWillappear:(Bool)animated{ 
}
//1.系统调用；2.当控制器收到内存警告；3.去除一些不必要的内存，去掉耗时内存 
viewDidreceiveMemory
控制器view的父控件是UIWindow 
@property(nonatomic)NSInteger tag; 
控件的id，父控件通过tag来找到对应的子控件
@property(nonatomic)CGAffineTranform transform; 
控件的形变属性(可以设置旋转角度、比例缩放、平移等属性)
## UIView常见方法
addSubview: 
remove
只有控制器里面能放属性；
控制器的属性关联着对象，通过控制器属性拿到这个对象做一些事情；
选项卡对象： 
[UISegmentedControl *sg]=[UISegmentedControl alloc] initWithItems:@[@”哈哈”,@”呵呵”,@”嘻嘻”]; 
//从父控件中移除 
[sg removeFromSuperView];
//根据一个tag找到对应的控件(一般都是子控件) 
[UIView *]viewWithTag:(NSInteger)tag; 
UIView中坐标： 
frame： 
bounds：一般是改变宽度的; 
center：中心点相对于左上角的点；
