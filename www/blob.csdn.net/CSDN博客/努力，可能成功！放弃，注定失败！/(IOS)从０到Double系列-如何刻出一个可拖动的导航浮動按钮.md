# (IOS)从０到Double系列 - 如何刻出一个可拖动的导航浮動按钮 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年01月05日 10:10:50[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：317
本文转自简书：http://www.jianshu.com/p/7c8002b60a2e
本篇教程使用Swift 源碼:[https://github.com/jamesdouble/JDJellyButton](https://github.com/jamesdouble/JDJellyButton)
，以下稱JDJellyButton
# 0)何謂導航浮動按鈕
当你的应用开发到一定程度的规模时，必须要有个十分清晰明了的导航功能，才不会让使用者卡在某一页，不知道如何前往他们想去的页面。
常见的导航方式，不外乎最常用UITabBarController、UINavigationBar，另外有一种虽然常见，但是因为不是IOS原生就有的UIControl，所以还是比较少人使用，那就是 "floating navigation button"。
之所以会有'Floating'这个字眼，是大多这样的导航按钮会凌驾在所有视图控制器(UI...ViewController)上，不管底下的视图控制器如何切页他都会保持在同样的位置。
![](http://upload-images.jianshu.io/upload_images/3776017-66e2600fbb3fb27a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/250)
浮动导航按钮
#### 0.1)JDJellyButton特色：按鈕群組
源碼其中一個特色就是浮動按鈕附有群組的功能，能讓一個浮動按鈕能包含更多的子按鈕以處理更多不同的事件。
![](http://upload-images.jianshu.io/upload_images/3776017-fba56ac39907311a.gif?imageMogr2/auto-orient/strip)
jellybutton_delegate.gif
#### 0.2)UIView or UIButton?
大部分的按钮控件虽然都是‘按钮’，但是比起继承实作UIButton,还不如继承实作他的父类别UIView, 可做的事比较多，限制也比较少，本文的JDJellyButton继承自UIView。
#### 0.3)Gesturer or UIResponder
因为我们是自己实作继承UIView的类别，比起每个按钮都要加上手势，我比较偏好在类别下实作几个常见的UIResponder方法 - touchesBegan, touchesMoved。一来省去还要宣告selector这样拐个弯的做法。
# 1)代碼架構＆解析
以下是JDJelllyButton的元件，我將由底層的子元件往上講解。
```
var MainButton:JDJellyMainButton!
var Container:JelllyContainer!
var RootView:UIView?
var delegate:JellyButtonDelegate?
var _datasource:JDJellyButtonDataSource?
var jellybutton:JDJellyButtonView?
```
![](http://upload-images.jianshu.io/upload_images/3776017-f4e56d283ef3edcc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
架构图
##### 1.1)ButtonGroups
紀錄了多個JDJellyButtonView跟它們個別的位置，此為“一組”Button
```
struct ButtonGroups {
    var buttongroup:[JDJellyButtonView]!
    var groupPositionDiff:[CGPoint]?
}
```
##### 1.2)JDJellyButtonView:UIView
此一类别是实作每个按钮的基础样式与点击，一个圆配一张图片。
别忘了要处理点击的事件。我做的方法是通知委任（上层接口JDJellyButton）被点击的是第几的Group的第几个Button。
```
protocol JellyButtonDelegate {
    func JellyButtonHasBeenTap(touch:UITouch,image:UIImage,groupindex:Int,arrindex:Int)
}
```
```
override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?)
    {
       let image = self.imgView?.image
       let groupindex = dependingMainButton?.getGroupIndex()
       let arrindex = dependingMainButton?.getJellyButtonIndex(jelly: self)
       print("\(groupindex),\(arrindex)")
       tapdelegate?.JellyButtonHasBeenTap(touch: touches.first!,image: image!,groupindex: groupindex!,arrindex: arrindex!)
    }
```
#### 1.3)JDJellyMainButton:JDJellyButtonView
本控件最主要的类别，也是整个导航浮动按钮的主体。样式跟其他的按钮一样，差别是在点击后的事件以及它可以拖动，所以就直接继承
JDJellyButtonView并且覆写touchesBegan, touchesMoved，并且也由它来管理ButtonGroups。
![](http://upload-images.jianshu.io/upload_images/3776017-bfcdafda180e208b.gif?imageMogr2/auto-orient/strip)
JDJellyButtonDemo.gif
```
func appendButtonGroup(bgs:ButtonGroups)
    {
        var temp_bgs:ButtonGroups = bgs
        for jelly in temp_bgs.buttongroup
        {
            //让每个按钮知道自己依附的是谁
            //因為只有MainButton知道子Button位在第幾個Group
            jelly.dependingMainButton = self
        }
        temp_bgs.groupPositionDiff = [CGPoint]()
        for i in 0..<bgs.buttongroup.count
        {
            //计算位置
            let cgpoint:CGPoint = CGPoint(x: x[i] , y: y[i])
            temp_bgs.groupPositionDiff?.append(cgpoint)
        }
        buttongroups.append(temp_bgs)
    }
```
需要注意的是因为JDJellyButton有分群组，而触发的条件是“长按”，因此我们不再touchesBegan做立即展开，而是在touchesEnded处理。
```
override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?)
    {
        /*
          略
        */
        if(Expanding)
        {
            expandignMove = true
            closingButtonGroup(expandagain: false)
        }
        //紀錄點下去的時間
        LastTime = touches.first!.timestamp
         /*
          略
        */
    }
override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        /*
          略
        */
        //短按
        if(touches.first!.timestamp - LastTime! < 0.15){
            if(!Expanding) {
                expandButtonGroup()
            }
            else {
                closingButtonGroup(expandagain: false)
            }
        }
        else    //長按
        {
            if(!Moving)
            {
                switchButtonGroup()
            }
            if(expandignMove && Moving)
            {
                expandButtonGroup()
            }
        }
        Moving = false
        expandignMove = false
        /*
          略
        */
    }
```
##### 1.4)JelllyContainer:UIView
本来并没有打算制作这个类别，后来遇到了一个非常严重的问题：虽然按钮以外透明的地方看似可点击后方的其他View，但是其实会点到浮动导航按钮的整个背景，进而无法触发后方使用者原本的东西。上网爬了之后，发现需覆写point这个Function
```
override func point(inside point: CGPoint, with event: UIEvent?) -> Bool {
        for subview in subviews {
            if !subview.isHidden && subview.alpha > 0 && subview.isUserInteractionEnabled && subview.point(inside: convert(point, to: subview), with: event) {
                return true
            }
        }
        return false
    }
```
