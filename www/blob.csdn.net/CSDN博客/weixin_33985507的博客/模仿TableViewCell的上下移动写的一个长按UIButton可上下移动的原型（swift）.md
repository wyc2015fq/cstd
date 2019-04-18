# 模仿TableViewCell的上下移动写的一个长按UIButton可上下移动的原型（swift） - weixin_33985507的博客 - CSDN博客
2016年09月02日 14:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
有时候并没有用到TableView，但也想实现长按上下移动，比如多个按钮实现长按移动。所以就写了一个简单的方法来实现这个效果。此效果的基础是所有按钮全部代码创建，是基于更改frame来实现的
## 效果图
![1870198-ed8b8e94e6cfa8a2.gif](https://upload-images.jianshu.io/upload_images/1870198-ed8b8e94e6cfa8a2.gif)
效果图.gif
## 原理
- 每个按钮要都添加一个`tag`
- 给每个按钮添加一个长按手势
```
let longPressGes = UILongPressGestureRecognizer.init(target:   self, action: #selector(longPressButton))
  button.addGestureRecognizer(longPressGes)
```
- 在`longPressButton`方法里添加实现
## 讲解
- 
属性
```
lazy var buttonArray = NSMutableArray() //按钮数组
  var tag              = 0                //按钮tag
  var endButtonTag     = 0                //用来保存移动的按钮最后停止时的tag
  var upY: CGFloat?                //移动的按钮之上的按钮的中心y轴
  var downY: CGFloat?             //移动的按钮之下的按钮的中心y轴
  var beginY: CGFloat?           //用以保存移动的按钮的y轴
  var oldFrame: CGRect?         //用以保存移动的按钮的最后停止的位置frame
```
- 
创建按钮样例
- 循环调用
```
for _ in 0..<10 {
          let button =  self.addButton(CGRectMake(0, 69 + CGFloat(self.buttonArray.count) * 49, self.view.frame.width, 44), title: "\(tag)")
          self.buttonArray.addObject(button)
}
```
- 
方法
``
// MARK: 添加按钮
func addButton(rect: CGRect, title: String) -> UIButton {
let button = UIButton.init(type: UIButtonType.System)
button.backgroundColor = UIColor(red: 0.94, green: 0.94, blue: 0.94, alpha: 1.00)
button.setTitle(title, forState: UIControlState.Normal)
button.frame = rect
button.tag = tag
tag += 1
let longPressGes = UILongPressGestureRecognizer.init(target: self, action: #selector(longPressButton))
button.addGestureRecognizer(longPressGes)
self.view.addSubview(button)
return button
}
```
- 
实现上下移动
```
// MARK: 长按按钮移动
  func longPressButton(sender: UIGestureRecognizer) {
      let view = sender.view as! UIButton //获取手势的视图，既移动的按钮
      let maxY = CGRectGetMaxY(view.frame)//获取按钮的最大y轴
      view.center = sender.locationInView(self.view)
      if sender.state == UIGestureRecognizerState.Began {   
            //Func:1
      }
      if sender.state == UIGestureRecognizerState.Changed {  
           //Func:2
      }
      if sender.state == UIGestureRecognizerState.Ended {
          //Func:3
      }   
  }
```
- 
在`Fun:1`里面，也就是开始手势的时候,将一些属性赋值
```
endButtonTag = view.tag  //用来保存移动的按钮最后停止时的tag
 oldFrame = view.frame //用以保存移动的按钮的最后停止的位置frame
 upY = view.frame.origin.y - 27 //移动的按钮之上的按钮的中心y轴
 downY = maxY + 27  //移动的按钮之下的按钮的中心y轴
 beginY = view.frame.origin.y //用以保存移动的按钮的y轴
```
- 
在`Fun:2`手势改变的时候来处理上下移动的逻辑
- 
向上移动
```
if view.frame.origin.y < upY   {
         if view.tag != 0 {
             let frontButton = self.buttonArray[view.tag-1] as! UIButton
             oldFrame = CGRectMake(oldFrame!.origin.x, frontButton.frame.origin.y, oldFrame!.width, oldFrame!.height)
             upY = frontButton.frame.origin.y - 27
             var frame = frontButton.frame
             frame.origin.y = frame.origin.y + 49
             downY = frame.origin.y + 22
             endButtonTag = frontButton.tag
             self.buttonArray[frontButton.tag + 1] = frontButton
             frontButton.tag = frontButton.tag + 1
             UIView.animateWithDuration(0.1, animations: {
                 frontButton.frame = frame
             })
             view.tag -= 1
        }
    }
  ```
  - 首先第一个按钮`tag = 0`不要做向上移动的处理，
  - 然后具体实现的逻辑是：每向上移动一格，就将原来上面的按钮的`frame`保存起来，将按钮数组中的对应的两个按钮互相交换，同时上面的按钮和移动中的按钮交换`tag`，再将上面的按钮移动到移动的按钮的下面，这样就完成了向上移动的实现。
-  向下移动
  ```
  if maxY > downY  {
              if view.tag != self.buttonArray.count-1 {
                  let afterButton = self.buttonArray[view.tag + 1] as! UIButton
                  oldFrame = CGRectMake(oldFrame!.origin.x, afterButton.frame.origin.y, oldFrame!.width, oldFrame!.height)
                  downY = CGRectGetMaxY(afterButton.frame) + 27
                  var midFrame = afterButton.frame
                  midFrame.origin.y = midFrame.origin.y - 49
                  upY = midFrame.origin.y + 22
                  endButtonTag = afterButton.tag
                  self.buttonArray[afterButton.tag - 1] = afterButton
                  afterButton.tag -= 1
                  UIView.animateWithDuration(0.1, animations: {
                      afterButton.frame = midFrame
                  })
                  view.tag += 1
              }
          }
  ```
  - 最后一个按钮不做向下移动
  - 具体实现逻辑与向上移动类似：每向下移动一格，就将原来下面的按钮的`frame`保存起来，将按钮数组中的对应的两个按钮互相交换，同时下面的按钮和移动中的按钮交换`tag`，再将下面的按钮移动到移动中的按钮的上面，完成向下移动的实现。
- 在`Fun:3`结束手势时
```
self.buttonArray[endButtonTag] = view
if view.frame.origin.x <= self.view.frame.width {
UIView.animateWithDuration(0.1, animations: {
view.frame = self.oldFrame!
})
}
```
逻辑：将移动中的按钮替换掉最后停止的位置的`tag`对应的数组中的值，再将移动中的按钮移动到最后的位置即可。
---
## 最后
这个方法实现起来并不是很难，代码量也不多。可能还有更简单的方法来实现，如果有人了解可以告知，在这先提前感谢。
```
