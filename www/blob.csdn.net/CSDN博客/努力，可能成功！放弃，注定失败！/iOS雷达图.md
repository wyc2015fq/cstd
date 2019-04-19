# iOS雷达图 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月22日 19:03:04[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2239
> 
最近公司要做一个医学考试类的App，里面有一个能力分析的雷达图，leader让我来封装这个雷达图。这个模块是在做测心率模块期间完成的，也是使用的`CoreGraphics`来实现的。
![](http://upload-images.jianshu.io/upload_images/1907868-c23360b7eb85618c.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
之前并不知道雷达图长什么样，这个是我在百度上下载的。大概就长这个样子，我们要用的和这个不太一样，没有文字和介绍。
### 思路
> 
写这个的时候，没有查阅什么资料，所以不太清楚别人是怎么做的。
我是从角度出发的，例如上面的图中，有5个能力模块，5个能力模块的分支（后面就叫它`主干`了，我也不知道叫什么）两两的夹角都是相同的。
- 首先把一个主干放在`y轴`上，这个主干绕`原点 O`一周是`2π`,被5个`主干`5等分，那么每一个夹角都是`2π/5`。
- 主干的长是相同的，可以通过`正弦`和`余弦`来求出每个主干落点的坐标,再从`原点O`到`落点`画线。
- 之后的网格和能力分布的画法也是这个思路，同样是从`y轴`上的主干取点，不同的是`斜边`的长度会根据`网格间距`和`能力百分比`变化，如下图：
![](http://upload-images.jianshu.io/upload_images/1907868-96743e47c8199317.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
不知道大家还记不记得正弦和余弦是啥了，我是已经还给数学老师了(虽然初中数学也是年组第一的)，刚好写这个的前一天在看高数。。
```
正弦： sin∂ = 对边/斜边
  余弦： cos∂ = 临边/斜边
```
## 3实现
###### 1. 初始化，定义几个属性后面使用
```
// 元素数组
@property (strong, nonatomic) NSArray <Element *>*elements;
```
这是一个放Model的数组，用来储存能力模块信息
![](http://upload-images.jianshu.io/upload_images/1907868-68456dcd951ed13c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
```cpp
```cpp
// 每一个主干的长度
static float element_w = 0;
// 主干到View预留的空白部分宽度
static float border_w  = 5;
// 中心点的横坐标
static float center_w  = 0;
// 中心点的纵坐标
static float center_h  = 0;
```
```
```
- (instancetype)initWithFrame:(CGRect)frame Elements:(NSArray <Element *>*)elements {
    if (self = [super initWithFrame:frame]) {
        self.backgroundColor = [UIColor whiteColor];
        self.elements = elements;
        if (self.frame.size.width>self.frame.size.height)
            element_w = (self.frame.size.height-border_w*2)/2;
        else
            element_w = (self.frame.size.width-border_w*2)/2;
        center_w = self.frame.size.width/2;
        center_h = self.frame.size.height/2;
    }
    return self;
}
```
###### 2.画主干
```
- (void)buildRadarMap {
    if (self.elements.count<3) return;
    // 获取画布
    CGContextRef context = UIGraphicsGetCurrentContext();
    // 划线颜色
    if (self.trunkLineColor)
        CGContextSetStrokeColorWithColor(context, self.trunkLineColor.CGColor);
    else
        CGContextSetStrokeColorWithColor(context, [UIColor blackColor].CGColor);
    // 划线宽度
    if (self.lineWidth)
        CGContextSetLineWidth(context, self.lineWidth);
    else
        CGContextSetLineWidth(context, 1);
    // 起点坐标
    CGContextMoveToPoint(context, center_w, center_h);
    // 第一条线
    CGContextAddLineToPoint(context, center_w, center_h - element_w);
    //画元素主干
    for (int i = 1; i <self.elements.count; i++) {
        float x   = 0;
        float y   = 0;
        double pi = (M_PI*2.0/(self.elements.count))*i;
        // 计算主干落点坐标
        Coordinate(pi, element_w, center_w, center_h,&x, &y);
        // 设置每次的初始点坐标
        CGContextMoveToPoint(context, center_w, center_h);
        // 设置终点坐标
        CGContextAddLineToPoint(context, x, y);
    }
    CGContextStrokePath(context);
}
```
**注：**`self.trunkLineColor`是我设置的一个外部可控主干的颜色，如果尾部没有设置就给默认值。`self.lineWidth`就是先的宽度了。
```
/**
 *  雷达图分成几等份 默认1等份（最外面一圈的框框）
 */
@property (assign, nonatomic) float part;
/**
 *  雷达线的宽度 默认1
 */
@property (assign, nonatomic) float lineWidth;
/**
 *  主干线颜色 默认黑色
 */
@property (strong, nonatomic) UIColor *trunkLineColor;
/**
 *  分等份线颜色 默认黑色
 */
@property (strong, nonatomic) UIColor *partLineColor;
/**
 *  比例线颜色 默认绿色
 */
@property (strong, nonatomic) UIColor *percentLineColor;
- (void)setTrunkLineColor:(UIColor *)trunkLineColor {
    _trunkLineColor =trunkLineColor;
    [self setNeedsDisplay];
}
- (void)setPartLineColor:(UIColor *)partLineColor {
    _partLineColor = partLineColor;
    [self setNeedsDisplay];
}
- (void)setPercentLineColor:(UIColor *)percentLineColor {
    _percentLineColor = percentLineColor;
    [self setNeedsDisplay];
}
```
*设置set方法调用`setNeedsDisplay`外部设置颜色的时候会调用`drawRect:`*
###### 3.计算落点的方法
```cpp
```cpp
#pragma mark - 算落点坐标
void Coordinate (double pi, float l, float c_w , float c_h, float *x, float *y) {
    if (pi < M_PI_2) {
        *x = c_w + sin(pi)*l;
        *y = c_h - cos(pi)*l;
    } else if (pi == M_PI_2) {
        *x = c_w + sin(pi)*l;
        *y = c_h + cos(pi)*l;
    } else if (pi < M_PI) {
        *x = c_w + sin(pi)*l;
        *y = c_h - cos(pi)*l;
    } else if (pi == M_PI) {
        *x = c_w + sin(pi)*l;
        *y = c_h + cos(pi)*l;
    } else if (pi < M_PI_2*3) {
        *x = c_w + sin(pi)*l;
        *y = c_h - cos(pi)*l;
    } else if (pi == M_PI_2*3) {
        *x = c_w + sin(pi)*l;
        *y = c_h + cos(pi)*l;
    } else {
        *x = c_w + sin(pi)*l;
        *y = c_h - cos(pi)*l;
    }
}
```
```
|变量|用途|
|----|----|
|`pi`|角度|
|`l`|主干长度|
|`c_w`|原点横坐标|
|`c_h`|原点纵坐标|
|`x`|落点横坐标|
|`y`|落点纵坐标|
*坐标系和View的Rect还不太一样，比如在坐标系第二、三象限的`y`是负值，而我们要的坐标的`y`是越往下越大，所以我们要`c_h+(-y)`，`x`同理。*
###### 4.画网格，也就是要把主干分成几等分，为了更灵活我也设置了可以从外部设置。
```
/**
 *  雷达图分成几等份 默认1等份（最外面一圈的框框）
 */
@property (assign, nonatomic) float part;
- (void)setPart:(float)part {
    _part = part;
    [self setNeedsDisplay];
}
```
```
#pragma mark - 画雷达分等分图
- (void)buildPart {
    float r = self.part<=1?1:self.part;
    // 获取画布
    CGContextRef context = UIGraphicsGetCurrentContext();
    // 划线颜色
    if (self.partLineColor)
        CGContextSetStrokeColorWithColor(context, self.partLineColor.CGColor);
    else
        CGContextSetStrokeColorWithColor(context, [UIColor blackColor].CGColor);
    // 划线宽度
    if (self.lineWidth)
        CGContextSetLineWidth(context, self.lineWidth);
    else
        CGContextSetLineWidth(context, 1);
    // 话分割线
    for (int j = 0; j<r; j++) {
        // 设置每次的初始点坐标
        CGContextMoveToPoint(context, center_w, border_w);
        // 画百分比分部
        for (int i = 1; i<=self.elements.count; i++) {
            float x   = 0;
            float y   = 0;
            double pi = (M_PI*2.0/(self.elements.count))*i;
            Coordinate(pi,element_w*(r-j)/r, center_w, center_h,&x, &y);
            if (i == 1) {
                CGContextMoveToPoint(context, center_w, border_w + element_w*j/r);
            }
            if (i == self.elements.count) {
                CGContextAddLineToPoint(context, center_w, border_w + element_w*j/r);
            } else {
                CGContextAddLineToPoint(context, x, y);
            }
        }
    }
    CGContextStrokePath(context);
}
```
*原理和画主干是一样的，只不过是每次的`起点`变成了上一个点的`落点`。把`主干`的长度进行了分割*
###### 5.画能力占比线
```
#pragma mark - 画百分比占比线
- (void)buildPercent {
    // 获取画布
    CGContextRef context = UIGraphicsGetCurrentContext();
    // 划线颜色
    if (self.percentLineColor)
        CGContextSetStrokeColorWithColor(context, self.percentLineColor.CGColor);
    else
        CGContextSetStrokeColorWithColor(context, [UIColor greenColor].CGColor);
    // 划线宽度
    if (self.lineWidth)
        CGContextSetLineWidth(context, self.lineWidth);
    else
        CGContextSetLineWidth(context, 1);
    Element *ele = self.elements[0];
    CGContextMoveToPoint(context, center_w, border_w +element_w*(1-ele.percent));
    for (int i = 1; i<=self.elements.count; i++) {
        float x   = 0;
        float y   = 0;
        if (i == self.elements.count) {
            //终点，最终回到开始点坐标
            Element *ele = self.elements[0];
            CGContextAddLineToPoint(context, center_w, border_w +element_w*(1-ele.percent));
        } else {
            Element *ele = self.elements[i];
            double pi = (M_PI*2.0/(self.elements.count))*i;
            Coordinate(pi,element_w*ele.percent, center_w, center_h,&x, &y);
            CGContextAddLineToPoint(context, x, y);
        }
    }
    CGContextStrokePath(context);
}
```
*这里和画网格不一样的就是每次的把主干的长度按照这个`能力值`的`百分比`分割*
### 6.总结
因为有之前的心率那得基础，这里完成的很快，然后做了一个相对比较灵活的封装，还没有加能力描述和百分比值，后续加上。
代码地址：[https://github.com/YvanLiu/RadarMap.git](https://github.com/YvanLiu/RadarMap.git)
欢迎吐槽，欢迎指正，欢迎批评，欢迎提问。
文／YvanLiu（简书作者）
原文链接：http://www.jianshu.com/p/e6d65460a6c2
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
