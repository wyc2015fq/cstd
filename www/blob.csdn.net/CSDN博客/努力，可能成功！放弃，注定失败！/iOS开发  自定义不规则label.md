# iOS开发 | 自定义不规则label - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年04月04日 13:58:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：120标签：[自定义不规则label																[UILabel](https://so.csdn.net/so/search/s.do?q=UILabel&t=blog)](https://so.csdn.net/so/search/s.do?q=自定义不规则label&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
### 场景
最近app改版，以下是截取的部分UI图：
![](https://upload-images.jianshu.io/upload_images/1692043-b3cd8f2f333b38fc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/552)
顶部的这一部分我是把它看做`UICollectionView`的组头：
![](https://upload-images.jianshu.io/upload_images/1692043-3dcaada81f906545.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
其中有一个不太规则的`label`：
![](https://upload-images.jianshu.io/upload_images/1692043-bbe5117784178b56.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/540)
这个`label`顶部的两个角是圆角，底部的两个角是直角，底部还有一个小三角。
### 思路
`CAShapeLayer`联合`UIBezierPath`画一个不规则的`layer`作为`label.layer`的`mask`。
### 具体实现
#### 1.自定义一个继承自`UILabel`的`IrregularLabel`
```
#import "IrregularLabel.h"
@interface IrregularLabel ()
/** 遮罩 */
@property (nonatomic, strong) CAShapeLayer *maskLayer;
/** 路径 */
@property (nonatomic, strong) UIBezierPath *borderPath;
@end
```
#### 2.在初始化方法中进行相应初始化和设置
```
- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // 初始化遮罩
        self.maskLayer = [CAShapeLayer layer];
        // 设置遮罩
        [self.layer setMask:self.maskLayer];
        // 初始化路径
        self.borderPath = [UIBezierPath bezierPath];
    }
    return self;
}
```
#### 3.在`layoutSubviews`方法中进行路径的设置
```
- (void)layoutSubviews {
    [super layoutSubviews];
    
    // 遮罩层frame
    self.maskLayer.frame = self.bounds;
    
    // 设置path起点
    [self.borderPath moveToPoint:CGPointMake(0, 10)];
    // 左上角的圆角
    [self.borderPath addQuadCurveToPoint:CGPointMake(10, 0) controlPoint:CGPointMake(0, 0)];
    // 直线，到右上角
    [self.borderPath addLineToPoint:CGPointMake(self.bounds.size.width - 10, 0)];
    // 右上角的圆角
    [self.borderPath addQuadCurveToPoint:CGPointMake(self.bounds.size.width, 10) controlPoint:CGPointMake(self.bounds.size.width, 0)];
    // 直线，到右下角
    [self.borderPath addLineToPoint:CGPointMake(self.bounds.size.width, self.bounds.size.height)];
    // 底部的小三角形
    [self.borderPath addLineToPoint:CGPointMake(self.bounds.size.width/2+5, self.bounds.size.height)];
    [self.borderPath addLineToPoint:CGPointMake(self.bounds.size.width/2, self.bounds.size.height-5)];
    [self.borderPath addLineToPoint:CGPointMake(self.bounds.size.width/2 - 5, self.bounds.size.height)];
    // 直线，到左下角
    [self.borderPath addLineToPoint:CGPointMake(0, self.bounds.size.height)];
    // 直线，回到起点
    [self.borderPath addLineToPoint:CGPointMake(0, 10)];
    
    // 将这个path赋值给maskLayer的path
    self.maskLayer.path = self.borderPath.CGPath;
}
```
**左上角的坐标是(0,0)右下角的坐标是(maxX,maxY)**
圆角是用的二次贝塞尔曲线，关于二次贝塞尔曲线，我在网上看到一张比较形象的图：
![](https://upload-images.jianshu.io/upload_images/1692043-490b54925cdedf56.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/600)
图片来自CSDN
### 最终效果
```
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.label = [[IrregularLabel alloc] initWithFrame:CGRectMake(90, 200, 200, 40)];
    [self.view addSubview:self.label];
    self.label.text = @"这是一个不规则label";
    self.label.textAlignment = NSTextAlignmentCenter;
    self.label.backgroundColor = [UIColor redColor];
    self.label.textColor = [UIColor whiteColor];
    self.label.font = [UIFont boldSystemFontOfSize:16];
}
```
效果图：
![](https://upload-images.jianshu.io/upload_images/1692043-6c1a687ebe252d99.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/648)
### demo
[https://github.com/CaiWanFeng/IrregularLabel](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FCaiWanFeng%2FIrregularLabel)
### 总结
![](https://upload-images.jianshu.io/upload_images/1692043-b925fe1cfb5beb46.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/148)
作者：无夜之星辰
链接：https://www.jianshu.com/p/fade8341c316
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。            
