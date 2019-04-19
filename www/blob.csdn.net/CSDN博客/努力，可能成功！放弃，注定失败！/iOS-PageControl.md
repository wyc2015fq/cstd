# iOS-PageControl - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月12日 10:13:59[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1776
# 4月11日更新效果:
![](http://upload-images.jianshu.io/upload_images/1298596-e0d2f6bdb88da632.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
navigation消失
![](http://upload-images.jianshu.io/upload_images/1298596-469d3925ed7abd67.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
navigation出现
更改了navigation之上的效果,主要牵扯到一些基数的计算,弄懂这几个数字就可以随意更改这个项目了.
还有隐藏Navigation和恢复Navigation的时候有很多坑,下一次更新讲解
之前那版比较像简书主页,这个更像掘金主页
github:[https://github.com/Lafree317/ZEPageControl](https://github.com/Lafree317/ZEPageControl)
# 第一版
之前公司做了一个类似于知乎小圆桌的页面,但是写完一直感觉有些地方不够好,所以就拿Swift重新写了一遍,如果有不足的地方欢迎大家指出
调用时只需要传入一个title数组就好了
```
override func viewDidLoad() {
        super.viewDidLoad()
        let zeVC = ZEPageViewController()
        zeVC.titlesArr = ["动态","问题","讨论"]
        self.addChildViewController(zeVC)
        self.view.addSubview(zeVC.view)
        // Do any additional setup after loading the view, typically from a nib.
    }
```
# 原理
这个项目的层级视图是这样的
![](http://upload-images.jianshu.io/upload_images/1298596-1a75086cb4e4c3c0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
先在底部创建一个和`viewController.view`一样大的`scrollView`
然后将`tableviewController`添加上去
最后将`tableviewController.tableView.contentInset`的`top`设置为你想要的高度(headerView+menuView)
然后通过代理将`tableviewController`的偏移量代理到主控制器上,用于计算顶部`headerView`和`menuView`的移动
然后主控制器的`scrollview`代理方法被调用时计算`tableviewcontroller.contentOffset`
难点主要在于计算三个`tableViewController`的`contentInset`和`contentOffset`
# 代码
#### 先看一眼全局属性
```cpp
```cpp
/** 屏幕宽度高度 */
let kZEScreenWidth = UIScreen.mainScreen().bounds.size.width
let kZEScreenHight = UIScreen.mainScreen().bounds.size.height
/** header和menu的高度 */
let kZEHeaderHight:CGFloat = 135
let kZEMenuHight:CGFloat = 50
let kScrollHorizY = kZEMenuHight+kZEHeaderHight
let kNavigationHight:CGFloat = 64
/** 偏移方法操作枚举 */
enum headerMenuShowType:UInt {
    case up = 1 // 固定在navigation上面
    case buttom = 2 // 固定在navigation下面
}
/** button两种状态的颜色 可以无视 */
let COLOR_BUTTON_DEFAULT = UIColor.init(red: 124/255.0, green: 129/255.0, blue: 138/255.0, alpha: 1)
let COLOR_BUTTON_SELECT = UIColor.init(red: 0/255.0, green: 127/255.0, blue: 255/255.0, alpha: 1)
```
```
#### ZEPageViewController
> 
创建scrollView
```
/** 创建底部scrollView,并将tableViewController添加到上面 */
    func layoutBackgroundScrollView(){
        self.backgroundScrollView = UIScrollView(frame: self.view.frame)
        self.backgroundScrollView?.pagingEnabled = true
        self.backgroundScrollView?.bounces = false
        self.backgroundScrollView?.delegate = self
        let floatArrCount = CGFloat(titlesArr.count)
        self.backgroundScrollView?.contentSize = CGSizeMake(floatArrCount*kZEScreenWidth,self.view.frame.size.height-64)
        // 给scrollY赋初值避免一上来滑动就乱
        scrollY = -kScrollHorizY // tableView自己持有的偏移量和赋值时给的偏移量符号是相反的
        for  i in 0 ..< titlesArr.count  {
            let floatI = CGFloat(i)
            let tableViewVC = ZETableViewController(style: UITableViewStyle.Plain)
            // tableView顶部流出HeaderView和MenuView的位置
            tableViewVC.tableView.contentInset = UIEdgeInsetsMake(kScrollHorizY, 0, 0, 0 )
            tableViewVC.delegate = self
            tableViewVC.view.frame = CGRectMake(floatI * kZEScreenWidth,0, self.view.frame.size.width, self.view.frame.size.height-64)
            tableViewVC.tags = titlesArr[i]
            // 将tableViewVC添加进数组方便管理
            tableViewArr.append(tableViewVC)
            self.addChildViewController(tableViewVC)
        }
        // 需要用到的时候再添加到view上,避免一上来就占用太多资源
        backgroundScrollView?.addSubview(tableViewArr[0].view)
        self.view.addSubview(backgroundScrollView!)
    }
```
> 
创建HeaderView MenuView
```
/** 创建HeaderView和MenuView */
    func layoutHeaderMenuView(){
        // 通过Xib导入headerView
        headerView = NSBundle.mainBundle().loadNibNamed("ZEHeaderView", owner: self, options: nil).first as! ZEHeaderView
        headerView.frame = CGRectMake(0, 64, kZEScreenWidth, kZEHeaderHight)
        self.view .addSubview(headerView)
        // MenuView
        menuView = ZEMenuView(frame:CGRectMake(0,CGRectGetMaxY(headerView.frame),kZEScreenWidth,kZEMenuHight))
        menuView.delegate = self
        menuView.setUIWithArr(titlesArr)
        self.view .addSubview(self.menuView)
    }
```
> 
然后在ViewDidLoad里面调用这两个方法
```
override func viewDidLoad() {
        super.viewDidLoad()
        self.automaticallyAdjustsScrollViewInsets = false
        layoutBackgroundScrollView()
        layoutHeaderMenuView()
    }
```
![](http://upload-images.jianshu.io/upload_images/1298596-e8dfcf5b4300a039.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
现在控件就创建完了
> 
重点来了,首先在`ZETableViewController`里面设置代理方法 回调tableView滚动时的偏移量
```
override func scrollViewDidScroll(scrollView: UIScrollView) {
        self.delegate?.tableViewDidScrollPassY(scrollView.contentOffset.y)
    }
```
> 
然后在ZEPageControl里进行操作
```
func tableViewDidScrollPassY(tableviewScrollY: CGFloat) {
        // 计算每次改变的值
        let seleoffSetY = tableviewScrollY - scrollY
        // 将scrollY的值同步
        scrollY = tableviewScrollY
        // 偏移量超出Navigation之上
        if scrollY >= -kZEMenuHight {
            headerMenuViewShowType(.up)
        }else if  scrollY <= -kScrollHorizY {
            // 偏移量超出Navigation之下
            headerMenuViewShowType(.buttom)
        }else{
            // 剩下的只有需要跟随的情况了
            // 将headerView的y值按照偏移量更改
            headerView.frame.origin.y -= seleoffSetY
            menuView.frame.origin.y = CGRectGetMaxY(headerView.frame)
        }
    }
    /** 因为频繁用到header和menu的固定,所以声明一个方法用于偷懒 */
    func headerMenuViewShowType(showType:headerMenuShowType){
        switch showType {
        case .up:
            menuView.frame.origin.y = kNavigationHight
            headerView.frame.origin.y = kNavigationHight-kZEHeaderHight
            break
        case .buttom:
            headerView.frame.origin.y = kNavigationHight
            menuView.frame.origin.y = CGRectGetMaxY(headerView.frame)
            break
        }
    }
```
> 
这两个方法写完之后上下滑动就可以有效果了
//不要看代码少,之前那个版本计算这里超级乱...整理之后发现代码还挺少的...
![](http://upload-images.jianshu.io/upload_images/1298596-2a5bfcb8f55b88a7.gif?imageMogr2/auto-orient/strip)
> 
然后是左右滑动时的方法
```
func scrollViewDidScroll(scrollView: UIScrollView) {
        // 判断是否有X变动,这里只处理横向滑动
        if scrollX == scrollView.contentOffset.x{
            return;
        }
        // 当tableview滑动到很靠上的时候,下一个tableview出现时只用在menuView之下
        if scrollY >= -kZEMenuHight {
            scrollY = -kZEMenuHight
        }
        for tableViewVC in tableViewArr {
            tableViewVC.tableView.contentOffset = CGPointMake(0, scrollY)
        }
        // 用于改变menuView的状态
        let rate = (scrollView.contentOffset.x/kZEScreenWidth)
        self.menuView.scrollToRate(rate)
        // +0.7的意思是 当滑动到30%的时候加载下一个tableView
        backgroundScrollView?.addSubview(tableViewArr[Int(rate+0.7)].view)
        // 记录x
        scrollX = scrollView.contentOffset.x
    }
```
> 
左右滑动还牵扯到MenuView的点击事件,所以把menuView里面选中代理回来
```
func menuViewSelectIndex(index: Int) {
        // 0.3秒的动画为了显得不太突兀
        UIView.animateWithDuration(0.3) {
            self.backgroundScrollView?.contentOffset = CGPointMake(kZEScreenWidth*CGFloat(index),-kNavigationHight)
        }
    }
```
> 
HeaderView和MenuView的具体代码在dem`里看就好了,因为这些都是根据不同需求自己定制的,如果想改造这个项目为己用的话,只需在具体View里改就好了,如果改变高度请在全局属性里更改
# 效果图
![](http://upload-images.jianshu.io/upload_images/1298596-58178431aea6216c.gif?imageMogr2/auto-orient/strip)
# 源码
##### github:[https://github.com/Lafree317/ZEPageControl](https://github.com/Lafree317/ZEPageControl)
喜欢的话请点个star 嘿嘿
顺便宣传一下我们公司的app 掘金 一个技术类的简书
