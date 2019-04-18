# iOS 给 UIView 某几个角添加圆角 - weixin_33985507的博客 - CSDN博客
2016年03月26日 12:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
Objective-C
```
#import <Foundation/Foundation.h>
@interface OTSCorner : NSObject
/**
 *   给 UIView 某几个角添加圆角
 *
 *  @param aView    输入view
 *  @param aCorners 要添加圆角的角（方向）
 *  @param aSize    圆角size
 */
+ (void)addCornerWithView:(UIView *)aView type:(UIRectCorner)aCorners size:(CGSize)aSize;
@end
```
```
#import "OTSCorner.h"
@implementation OTSCorner
+ (void)addCornerWithView:(UIView *)aView type:(UIRectCorner)aCorners size:(CGSize)aSize
{
    // 根据矩形画带圆角的曲线
    UIBezierPath *maskPath = [UIBezierPath bezierPathWithRoundedRect:aView.bounds byRoundingCorners:aCorners cornerRadii:aSize];
    CAShapeLayer *maskLayer = [[CAShapeLayer alloc] init];
    maskLayer.frame = aView.bounds;
    maskLayer.path = maskPath.CGPath;
    aView.layer.mask = maskLayer;
}
@end
```
Swift
```
import UIKit
class OTSCorner: NSObject {
    
    /**
     *   给 UIView 某几个角添加圆角
     *
     *  @param aView    输入view
     *  @param aCorners 要添加圆角的角（方向）
     *  @param aSize    圆角size
     */
    class func addCornerWithView(aView : UIView, aCorners : UIRectCorner, aSize : CGSize) ->Void
    {
        // 根据矩形画带圆角的曲线
        let maskPath = UIBezierPath.init(roundedRect: aView.bounds, byRoundingCorners: aCorners, cornerRadii: aSize)
        let maskLayer = CAShapeLayer()
        maskLayer.frame = aView.bounds
        maskLayer.path = maskPath.CGPath
        aView.layer.mask = maskLayer
    }
}
```
Swift 测试
```
let view = UIView()
        view.frame = CGRectMake(100, 100, 100, 100)
        view.backgroundColor = UIColor.redColor()
        self.view.addSubview(view)
        OTSCorner.addCornerWithView(view, aCorners:[.TopLeft, .TopRight, .BottomLeft, .BottomRight], aSize: CGSizeMake(20, 20))
```
测试效果
![1708486-18d0b2b817350d95.png](https://upload-images.jianshu.io/upload_images/1708486-18d0b2b817350d95.png)
D8592B7C-84C2-4BA5-ACAF-BDE3E08A2584.png
