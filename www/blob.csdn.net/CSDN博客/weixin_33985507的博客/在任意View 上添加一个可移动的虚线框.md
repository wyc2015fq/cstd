# 在任意View 上添加一个可移动的虚线框 - weixin_33985507的博客 - CSDN博客
2016年04月07日 15:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
### 调用示例
在StoryBoard 上需要添加虚线框的视图上拖一个UIView 这样方便加 constrain
继承 DashedBoardView
在需要添加框框的view 的视图控制器里面
```
class aViewController: UIViewController {
  @IBOutlet weak var dashedBorderView: DashedBorderView!
  @IBOutlet weak var dashedViewX: NSLayoutConstraint! // 虚线框的x轴约束
  @IBOutlet weak var dashedViewY: NSLayoutConstraint! // 虚线框的y轴约束
  var touchPointCache: CGPoint?
  
  override func viewDidLoad() {
      super.viewDidLoad()
      // 添加拖动手势
      let panGesture = UIPanGestureRecognizer(target: self, action: #selector(self.handlePanGesture(_:)))
      dashedBorderView.addGestureRecognizer(panGesture)
    }
}
```
在其它控件的响应方法中改变 DashedBoardView 的属性
比如scrollView 的缩放结束响应方法中
```
func scrollViewDidEndZooming(scrollView: UIScrollView, withView view: UIView?, atScale scale: CGFloat) {
  // 博主这里做的是当UIScrollView 缩放到0.25 的比例的时候框框颜色发生改变
     dashedBorderView.textOnLabel = "当前缩放比例 \(scrollView.zoomScale) "
     if scrollView.zoomScale == 0.25 {
         dashedBorderView.border.strokeColor = UIColor.whiteColor().CGColor
     } else {
         dashedBorderView.border.strokeColor = UIColor.grayColor().CGColor
     }
  }
```
### 源码
#### DashedBoardView
```
//
//  DashedBoardView.swift
//  
//
//  Created by 王俊硕 on 16/3/22.
//  Copyright © 2016年 王俊硕. All rights reserved.
//
import UIKit
class DashedBorderView: UIView {
  var border:CAShapeLayer!
  var warnLabel: UILabel?
  var textOnLabel: String {
      get {
          return "please set a value"
      }
      set(currentPixel) {
          warnLabel?.attributedText = NSAttributedString(string: "\(currentPixel)*\(currentPixel)", attributes: [NSFontAttributeName: UIFont.systemFontOfSize((9))])
      }
  }
  required init?(coder aDecoder: NSCoder) {
      super.init(coder: aDecoder)
    
      warnLabel = UILabel(frame: CGRect(x: 10, y: 4, width: 100, height: 12))
      warnLabel?.textColor = UIColor.whiteColor()
      self.addSubview(warnLabel!)
    
      border = CAShapeLayer();
    
      border.strokeColor = UIColor.grayColor().CGColor;
      border.fillColor = nil;
      border.lineDashPattern = [4, 4];
      self.layer.addSublayer(border);
    
  }
  func addDashedFrame(inView view: UIView)  {
      border = CAShapeLayer();
    
      border.strokeColor = UIColor.whiteColor().CGColor;
      border.fillColor = nil;
      border.lineDashPattern = [4, 4];
      view.layer.addSublayer(border);
      border.path = UIBezierPath(roundedRect: self.bounds, cornerRadius:8).CGPath;
      border.frame = self.bounds;
    
  }
  func addLabel(currentPixel currentPixel: CGFloat) {
      let label = UILabel(frame: CGRect(x: 10, y: 10, width: 100, height: 100))
      label.text = "Current Pixel: \(currentPixel) * \(currentPixel)"
}
```
#### 拖动响应
```
func handlePanGesture(gesture: UIPanGestureRecognizer) {
    switch (gesture.state) {
    case UIGestureRecognizerState.Began:
        print("Moving bega")
        touchPointCache = gesture.locationInView(self.view)
    case UIGestureRecognizerState.Changed:
        print("Touch point changed")
        let touchPoint = gesture.locationInView(self.view)
        let deltaX = (touchPointCache?.x)! - touchPoint.x
        let deltaY = (touchPointCache?.y)! - touchPoint.y
        dashedViewX.constant -= deltaX // 刷新坐标
        dashedViewY.constant -= deltaY // 刷新坐标
        touchPointCache = touchPoint
        print(touchPoint)
    case UIGestureRecognizerState.Ended:
        print("User has ended the touch")
        print("x :  \(dashedViewX) y: \(dashedViewY)")
        touchPointCache = nil
    default:
        print("What's happend")
    }
    print("dashedBoard frmae: \(dashedBorderView)")
    
  }
```
