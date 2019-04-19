# iOS用Sketch制作APP下拉刷新的GIF动画 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年02月01日 14:49:09[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：110
前言
中秋三天就这么睡过去了~~这也不能怪我，听说深圳连飞机都吹跑了，吓死宝宝了。
节假日一过总会留下后遗症，但是为了钱途，咱们是要挺直腰杆，接着敲代码。。
一、用Sketch结合MJRefresh来自定义刷新动画
今天分享一下iOS关于自定义下拉刷新动画的实现。我们平时看到很多APP的下拉刷新都是菊花在转的样式，因为很多APP都是用`MJRefresh`默认下拉刷新来实现的。但是，我们也会看到不少APP的下拉刷新很有特色，像**美团、分期乐、喵播**等等。
他们的刷新样式同样是用`MJRefresh`来实现的，`MJRefresh`有一个类`MJRefreshGifHeader`，自定义这个类可以将GIF图片集合制作成下拉刷新的动画。但是他的前提是得制作出一个GIF图片集合。
在我的上一篇文章[iOS工程师Mac上的必备软件](https://www.jianshu.com/p/fbd19f724cf1)中我介绍了一款UI软件Sketch。这个软件相信大多数人都接触过（如果你还不会用，那么可以看我的上一篇文章，里面有教程链接）。这个软件不仅能够做静态的UI图，还能做动态的GIF图。前段时间我尝试用Sketch做一个GIF动画，无意间被我发现了一个很好用的插件[AnimateMate](https://link.jianshu.com?t=https://github.com/Creatide/AnimateMate)，这个插件使用javaScript写的，但这不影响我们的使用，关于这个插件的使用可以去这个网站看视频[AnimateMate视频教程](https://link.jianshu.com?t=%5Bhttps://www.youtube.com/watch?v=bDm_c4wPBkc%5D(https://www.youtube.com/watch?v=bDm_c4wPBkc))。我们用Sketch ToolBox安装完插件之后就可以使用了。
这里我以分期乐的刷新动画为例进行讲解。这个是最后集成到项目中的效果：
![](https://upload-images.jianshu.io/upload_images/1248713-06549405e304c2c4.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/368/format/webp)
最终效果图.gif
下面我就分两个大的步骤来给大家讲解一下如何让自己的APP有一个专属的下拉刷新动画 :
```
1. 用Sketch制作GIF动画
2. 用MJRefreshGifHeader来集成下拉刷新动画
```
二、用Sketch制作GIF动画
1. Sketch安装插件[AnimateMate](https://link.jianshu.com?t=https://github.com/Creatide/AnimateMate)
2. 打开Sketch新建一个画板，命名为refresh，把你要画的图形放到画布上
![](https://upload-images.jianshu.io/upload_images/1248713-60d4adfb0d47cebc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
新建画布.png
3. 接下来是按照图形把分期乐的猫头鹰画出来
先分析一下这个猫头鹰：
```
1.先用钢笔工具把头和脸的形状画出来，再进行贝兹曲线变形
2.用铅笔工具把头发画出来
3.鼻子就是两个三角形
4.眼睛就是三个圆
5.翅膀就是一个矩形弄成圆角
6.身体就是三个圆
7.脚是三个矩形组成的
```
- 
选择钢笔工具
![](https://upload-images.jianshu.io/upload_images/1248713-73d858eec4176750.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/542/format/webp)
选择贝兹曲线.png
- 
用钢笔工具沿着头部描7个点，最后重合第1个点和第7个点重合
![](https://upload-images.jianshu.io/upload_images/1248713-7db02af0a6005585.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/406/format/webp)
用钢笔工具描点.png
- 
对线条进行编辑
![](https://upload-images.jianshu.io/upload_images/1248713-7e7951154df2f8d1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/653/format/webp)
选中线条点击Edit.png
- 
分别选中第3个点和第6个点进行贝兹曲线变形
![](https://upload-images.jianshu.io/upload_images/1248713-5c445106345f330b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/721/format/webp)
头部进行贝兹曲线变形.png
- 
接下来再用同样的方式把脸部的关键点描出来
![](https://upload-images.jianshu.io/upload_images/1248713-4bf9f949d7a7e88e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/391/format/webp)
用钢笔工具把脸部的关键点描出来.png
- 
对脸部的第2、5、7这三个点进行贝兹曲线变形
![](https://upload-images.jianshu.io/upload_images/1248713-395396705a4302d8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/398/format/webp)
对脸部进行贝兹曲线变形.png
- 
把眼睛和鼻子画出来
![](https://upload-images.jianshu.io/upload_images/1248713-99e496b908914dc2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/403/format/webp)
画鼻子和眼睛.png
- 
用一个矩形把翅膀画出来
![](https://upload-images.jianshu.io/upload_images/1248713-e2b2a98ec46851f2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/374/format/webp)
画翅膀.png
- 
改变翅膀的圆角为100
![](https://upload-images.jianshu.io/upload_images/1248713-a66048d64fbe5873.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/805/format/webp)
修改翅膀圆角.png
- 
用三个圆把身体画出来
![](https://upload-images.jianshu.io/upload_images/1248713-17db374de40474f4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/371/format/webp)
.画身体png
- 
用6个矩形把脚画出来
![](https://upload-images.jianshu.io/upload_images/1248713-51d56f01ab9f0fdb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/378/format/webp)
画脚.png
- 
接下来就是头发了，头发我们用铅笔工具直接画出来，铅笔工具不好把握，有一点偏差也没事
![](https://upload-images.jianshu.io/upload_images/1248713-4574c4b2548f0f2e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/375/format/webp)
用铅笔画头发.png
![](https://upload-images.jianshu.io/upload_images/1248713-ee852ed7b50c7b13.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/357/format/webp)
画出头发.png
- 
最后把下面的图形移到旁边，用取色的方式对各个部位填充颜色，然后把边界都去掉（注意图层的位置，如果遮挡住了，通过移动图层来使其摆放在正确的图层）
![](https://upload-images.jianshu.io/upload_images/1248713-d39c06ad21c19868.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/600/format/webp)
我下面图片移开.png
- 
这样就把猫头鹰给画出来，接下来就是制作动画。
![](https://upload-images.jianshu.io/upload_images/1248713-b397de165ea4b063.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/177/format/webp)
最终画出来的图形.png
4. 用AnimationMate来制作动画
- 
先把整个猫头鹰按照比例调整到宽为56的大小，画板调整到宽高都为60的大小
![](https://upload-images.jianshu.io/upload_images/1248713-6dbc41af8d2825d9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/762/format/webp)
修改猫头鹰大小和画布大小.png
- 
将眼睛的黑色部分和大白色的圆建成一个组
![](https://upload-images.jianshu.io/upload_images/1248713-121de1946cfb9a14.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/944/format/webp)
合并成组.png
- 
选中所有图层->plugins->AnimateMate->Creat Animateion
![](https://upload-images.jianshu.io/upload_images/1248713-5e7b36a39e60a375.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/690/format/webp)
创建动画1.png
- 
创建第0帧的动画。KeyframeNumber为0，EasingType动画类型为线性动画linearEase。
![](https://upload-images.jianshu.io/upload_images/1248713-5a95f00a0b9cd68a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/658/format/webp)
创建动画2.png
- 
改变eye_right和eye_left的Transform为359°
![](https://upload-images.jianshu.io/upload_images/1248713-c868fc7d36fa22c9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/695/format/webp)
创建动画3.png
- 
选中所有图层->plugins->AnimateMate->Creat Animateion，我们让这个猫头鹰的眼睛从0°旋转到359°分成20个关键帧。KeyframeNumber为20，EasyingType为线性动画linearEase，点击OK。
![](https://upload-images.jianshu.io/upload_images/1248713-8e1cdea390c5ebdd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/557/format/webp)
创建动画4.png
- 
选中refresh图层->plugins->ExportAnimation导出动画
![](https://upload-images.jianshu.io/upload_images/1248713-ff88c6676b7ff0ca.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/968/format/webp)
导出动画1.png
- 
可以看到，这里我们可以导出GIF Aniamtion动图，也可以直接导出20个PNG图片。我们都勾选上，然后给导出的GIF图和PNG图填一个前缀名字fenqile_refresh。选择导出出到fenqile_refresh文件夹
![](https://upload-images.jianshu.io/upload_images/1248713-58c8db75aa3f74c1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/582/format/webp)
导出动画2.png
- 
导出之后的结果为下图所示。有20张png图片和一个gif动图。
![](https://upload-images.jianshu.io/upload_images/1248713-803052d8f68f7117.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/558/format/webp)
最后导出的结果.png
- 
用浏览器打开gif动图可以看到最终实现的效果。
![](https://upload-images.jianshu.io/upload_images/1248713-10fd027cd7ee7623.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/346/format/webp)
最终实现的效果.gif
**至此用Sketch就把这个GIF动图给制作完成了，接下来就是要用`MJRefresh`来自定义下拉刷新样式了**
三、用MJRefreshGifHeader来自定义下拉刷新样式
- 
将20个png图片导入到Xcode工程
![](https://upload-images.jianshu.io/upload_images/1248713-1f5c6e209108d414.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/661/format/webp)
将图片导入工程.png
- 
创建一个类**`CustomRefreshGifHeader`**继承自**`MJRefreshGifHeader`**。
```
import UIKit
import MJRefresh
class CustomRefreshGifHeader: MJRefreshGifHeader {
    // MARK: 重写父类方法
    
    /**
     在这里做一些初始化配置（比如添加子控件）
     */
    override func prepare() {
        super.prepare()
        var images = [UIImage]()
        for i in 0...20 {
            if i < 10 {
                images.append(UIImage.init(named: "fenqile_refresh_0\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\(i)")!)
            }else {
                images.append(UIImage.init(named: "fenqile_refresh_\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\(i)")!)
            }
        }
        
        self.setImages(images, duration: 1.2, forState: MJRefreshState.Refreshing)
        self.setImages([UIImage.init(named: "fenqile_refresh_00")!], forState: MJRefreshState.Pulling)
        self.setImages([UIImage.init(named: "fenqile_refresh_00")!], forState: MJRefreshState.Idle)
        self.lastUpdatedTimeLabel.hidden = true
        
        self.stateLabel.font = UIFont.systemFontOfSize(10)
        self.setTitle("释放即可刷新", forState: MJRefreshState.Pulling)
        self.setTitle("下拉刷新", forState: MJRefreshState.Idle)
        self.setTitle("刷新中", forState: MJRefreshState.Refreshing)
    }
    
    /**
     在这里设置子控件的位置和尺寸
     */
    override func placeSubviews() {
        super.placeSubviews()
        
        self.gifView.contentMode = UIViewContentMode.Center
        self.gifView.frame = CGRect.init(x: 0, y: 4, width: self.mj_w, height: 34)
        self.stateLabel.frame = CGRect.init(x: 0, y: 40, width: self.mj_w, height: 14)
    }
}
```
3.给TableView添加下拉刷新
```
override func viewDidLoad() {
        super.viewDidLoad()
        tableView = UITableView(frame: CGRect(x: 0, y: 0, width: SCREEN_WIDTH, height: SCREEN_HEIGHT), style: UITableViewStyle.Grouped)
        view.addSubview(tableView)
        tableView.mj_header = CustomRefreshGifHeader.init { 
            // loading data
        }
}
```
> 
**总结**
Sketch不仅能够制作UI图，也能制作GIF动画。我这里只是举了个例子，使用AnimationMate这个插件来做GIF动画真的很方便，但也有很多局限性。目前这款插件只能做一些线性的动画效果，像 `Position`、`Size`、`Transform`、`Opacity`等。如果要做一些比较复杂的动画那用AnimationMate就不太合适了。
这篇文章没有重点讲解AnimationMate的其他使用。接下来我会出一篇关于这个插件的详细使用教程。
马上来为你的APP制作炫酷的刷新动画吧，说不定你们老板会给你涨工资呢~~。
喜欢的话可以点个赞。
作者：peaktan
链接：https://www.jianshu.com/p/45022427cf96
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
