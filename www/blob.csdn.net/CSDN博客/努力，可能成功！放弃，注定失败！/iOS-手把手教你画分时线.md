# iOS-手把手教你画分时线 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月08日 09:37:21[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：635
## 本文来自简书，原文地址:[http://www.jianshu.com/p/7e8d3b845331](http://www.jianshu.com/p/7e8d3b845331)
## 前言
### 最近项目和股票有关,自然而然想起了分时线,K线等东西.即使是出自自己手里的东西,放久了难觅看忘记,特别是当时的想法和逻辑思维过程.既然写了,总要留下点什么,于是有了这篇笔记.
### 先看看最后结果吧.此图确实不够美观,但是毕竟只是一个demo,一个骨架而已,具体的血肉需要大家自己根据需要去填充.
![](http://upload-images.jianshu.io/upload_images/2478673-bebe34e796346488.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1.png
## 画分时线,我们需要做些什么?
- 画框架(网格线);
- 横坐标上的文字(时间轴);
- 纵坐标上的文字(左边价格,右边涨跌率);
- 画分时线;
- 心跳效果;
- 添加手势(长按和点击);
- 画十字交叉线;
- 画十字交叉线与横纵坐标交点处的文字;
## 下面一步步来实现上面的效果吧
### 0.工欲善其事必先利其器,先来抽几个方法吧
- 心跳效果
```
/**心跳动画*/
- (void)heartAnimationWithLayer:(CALayer*)layer {
    CAAnimationGroup *group = [[CAAnimationGroup alloc]init];
    group.duration = 0.5;
    group.repeatCount = HUGE;
    CABasicAnimation *scaleAnim = [CABasicAnimation animationWithKeyPath:@"transform.scale"];
    scaleAnim.toValue = @1.2;
    CABasicAnimation *alphaAnim = [CABasicAnimation animationWithKeyPath:@"opacity"];
    alphaAnim.toValue = @0.3f;
    group.animations = @[scaleAnim,alphaAnim];
    [layer addAnimation:group forKey:nil];
}
```
- 坐标轴的转换
```
/**根据时间获取坐标点*/
-(CGFloat)miniteTimeWithTimeStr:(NSString*)timeStr {
    if (timeStr == nil || [timeStr isEqualToString:@""]) return 0.0;
    NSArray *temp = [timeStr componentsSeparatedByString:@":"];
    NSInteger minte = [temp[0] integerValue]*60 + [temp[1] integerValue];
    //每分钟代表的宽度
    CGFloat aveWidth = DPW/ 240.0;
    if (minte <= 11*60+30) {//上午
        return aveWidth*(minte - 9*60-30);
    }else {//下午
        return DPW *0.5 + aveWidth*(minte - 13*60);
    }
}
/**根据坐标点获取对应时间*/
-(NSString*)timeWithPoint:(CGPoint)point{
    NSString *str = @"";
    //单位距离代表的时间
    CGFloat aveWidth = 240.0/DPW;
    CGFloat totalTime = aveWidth * point.x + 9*60 + 30;//上午
    if (point.x >DPW *0.5) {
        totalTime = aveWidth * point.x + 11*60;//下午
    }
    int h = totalTime / 60;
    int m =  ((int)(totalTime + 0.5)) % 60;
    str = [NSString stringWithFormat:@"%d:%d",h,m];
    return str;
}
/**根据价格获取坐标位置*/
- (CGFloat)priceLabelLocationWithPrice:(CGFloat)price {
    CGFloat height = DPH - 60;
    CGFloat avrSpace = height/(self.max - self.min);
    return height - ((price - self.min)*avrSpace);
}
/**根据坐标位置获取价格*/
- (NSString*)pricWithPoint:(CGPoint)point {
    NSString *price = @"";
    //单位距离代表的价格
    CGFloat avePrice = (self.max - self.min)/(DPH - 60);
    //保留两位小数
    price = [NSString stringWithFormat:@"%.2f",(self.max - point.y*avePrice)+0.005];
    return price;
}
```
- 画文字
```
/**画文字,直接创建一个CATextLayer*/
- (void)drawLabelAtRect:(CGRect)rect textStr:(NSString*)textStr {
    CATextLayer *textLayer = [CATextLayer layer];
    textLayer.frame = rect;
    [self.layer addSublayer:textLayer];
    //set text attributes
    textLayer.foregroundColor = [UIColor blackColor].CGColor;
    textLayer.alignmentMode = kCAAlignmentJustified;
    textLayer.wrapped = YES;
    //choose a font
    UIFont *font = [UIFont systemFontOfSize:10];
    //set layer font
    CFStringRef fontName = (__bridge CFStringRef)font.fontName;
    CGFontRef fontRef = CGFontCreateWithFontName(fontName);
    textLayer.font = fontRef;
    textLayer.fontSize = font.pointSize;
    CGFontRelease(fontRef);
    textLayer.contentsScale = [UIScreen mainScreen].scale;
    //choose some text
    //set layer text
    textLayer.string = textStr;
}
/**画文字,指定CATextLayer*/
- (void)drawCrossLabelWithTextLayer:(CATextLayer*)textLayer AtRect:(CGRect)rect textStr:(NSString*)textStr {
    textLayer.frame = rect;
    [self.layer addSublayer:textLayer];
    //set text attributes
    textLayer.foregroundColor = [UIColor blackColor].CGColor;
    textLayer.alignmentMode = kCAAlignmentJustified;
    textLayer.wrapped = YES;
    //choose a font
    UIFont *font = [UIFont systemFontOfSize:10];
    //set layer font
    CFStringRef fontName = (__bridge CFStringRef)font.fontName;
    CGFontRef fontRef = CGFontCreateWithFontName(fontName);
    textLayer.font = fontRef;
    textLayer.fontSize = font.pointSize;
    CGFontRelease(fontRef);
    textLayer.contentsScale = [UIScreen mainScreen].scale;
    //choose some text
    //set layer text
    textLayer.string = textStr;
}
```
### 1.获取纵坐标的最大值,最小值和中间值
说明:
- 横线:分时线最中间的横线对应昨日收盘价,这是固定的.但是,其他横线是根据数据实时变化的,旨在包含所有的数据点.所以,max = mid + 最大偏差;min = mid - 最大偏差;最大偏差=所有数据中偏离mid最大的价格与mid之间差值的绝对值.这样max - min 为价格的波动范围,有了这个就可以根据需要,画出横线(这里画了3条横线);
- 竖线: 股票交易时间,上午9:30到11:30,下午13:00到15:00,总共4个小时;这里就可以算出单位距离代表的时间,从而确定每个时间节点在横坐标上的位置,画出横坐标.同理,也可算出每分钟代表的长度,这点在画十字交叉线与X轴的交点文字时用的到.对应的方法见上面已抽取的.
- 分时线: 可以看到在这个方法中已近开始划线了.划线无非就是连线,moveToPoint 和addLineToPoint.为了避免再次遍历所有数据,所以在确定最大和最小价格的循环中就开始添加线;
```
/**数据处理*/
- (void)preworkForData {
    //0 拿到纵坐标的3个点(max,mid,min),mid=昨日收盘价
    DPTimeLineItem *temp = self.dataArrM[0];
    self.mid = [temp.pre_close_px doubleValue];
    if ([temp.last_px doubleValue] -[temp.pre_close_px doubleValue] > 0) {
        self.max = [temp.last_px doubleValue];
        self.min = [temp.pre_close_px doubleValue] -([temp.last_px doubleValue] -[temp.pre_close_px doubleValue] );
    }else {
        self.min = [temp.last_px doubleValue];
        self.max =  [temp.pre_close_px doubleValue] + ([temp.pre_close_px doubleValue] -[temp.last_px doubleValue] );
    }
    for (int i = 0; i< self.dataArrM.count; i ++ ) {
        DPTimeLineItem *item = self.dataArrM[i];
        //获取纵坐标最大,最小,中间值
        if (fabs(item.last_px.doubleValue - self.mid) >(self.max - self.mid)) {
            self.max = self.mid +fabs(item.last_px.doubleValue - self.mid);
            self.min = self.mid - fabs(item.last_px.doubleValue - self.mid);
        }
        //画分时线
        CGFloat timeX = [self miniteTimeWithTimeStr:item.curr_time];
        CGFloat priceY = [self priceLabelLocationWithPrice:item.last_px.floatValue];
        if (i == 0) {
            if (self.isNeedBackGroundColor) {
                [self.timeLinePath moveToPoint:CGPointMake(0, DPH - 60)];
                [self.timeLinePath addLineToPoint:CGPointMake(timeX, priceY)];
            }else {
                [self.timeLinePath moveToPoint:CGPointMake(timeX, priceY)];
            }
        }
        if (i == self.dataArrM.count-1) {
            self.currentPoint = CGPointMake(timeX, priceY);
        }
        [self.timeLinePath addLineToPoint:CGPointMake(timeX,  priceY)];
    }
}
```
### 2.画网格线
说明:
- 利用for循环,画横竖线;
- 横纵坐标固定地方画文字
```
- (void)drawFramework {
    UIBezierPath *path = [[UIBezierPath alloc]init];
    //三条横线
    CGFloat rowSpace = (DPH -60.0)/2.0;
    NSString *tempStr = @"";
    for (int i = 0; i < 3; i ++) {
        [path moveToPoint:CGPointMake(0,rowSpace*i)];
        [path addLineToPoint:CGPointMake(DPW, rowSpace*i)];
        if (0 == i) {
            tempStr = [NSString stringWithFormat:@"%.2f",self.max];
        }else if (1==i) {
            tempStr = [NSString stringWithFormat:@"%.2f",self.mid];
        }else if (2 == i) {
            tempStr = [NSString stringWithFormat:@"%.2f",self.min];
        }
        [self drawLabelAtRect:CGRectMake(-25, rowSpace*i-10, 25, 20) textStr:tempStr];
    }
    //4条竖线
    CGFloat colSpace = DPW / 4.0;
    for (int i = 0; i < 5; i ++) {
        [path moveToPoint:CGPointMake(colSpace*i,0)];
        [path addLineToPoint:CGPointMake(colSpace*i, DPH-60)];
        if (0 == i) {
            tempStr = @"9:30";
            [self drawLabelAtRect:CGRectMake(colSpace*i, DPH-60 ,30,20) textStr:tempStr];
            continue;
        }else if (1==i) {
            tempStr = @"10:30";
        }else if (2 == i) {
            tempStr = @"11:30/13:00";
            [self drawLabelAtRect:CGRectMake(colSpace*i-30, DPH-60 ,70,20) textStr:tempStr];
            continue;
        }else if (3==i) {
            tempStr = @"14:00";
        }else if (4 == i) {
            tempStr = @"15:00";
            [self drawLabelAtRect:CGRectMake(colSpace*i - 30, DPH-60 ,30,20) textStr:tempStr];
            continue;
        }
        [self drawLabelAtRect:CGRectMake(colSpace*i-15, DPH-60 ,35,20) textStr:tempStr];
    }
    CAShapeLayer *shapeLayer = [[CAShapeLayer alloc]init];
    shapeLayer.strokeColor = [UIColor lightGrayColor].CGColor;
    shapeLayer.path = path.CGPath;
    [self.layer addSublayer:shapeLayer];
}
```
## 3.画坐标轴上的文字
见2.画网格线
## 4.画分时线
说明:这里通过isNeedBackGroundColor控制是否画背景
```
- (void)drawTimeLine  {
    if (self.isNeedBackGroundColor) {
        [self.timeLinePath addLineToPoint:CGPointMake(self.currentPoint.x , DPH - 60)];
        self.timeLineLayer.fillColor = [UIColor lightGrayColor].CGColor;
    }else {
        self.timeLineLayer.fillColor = [UIColor clearColor].CGColor;
    }
    self.timeLineLayer.path = self.timeLinePath.CGPath;
    [self.layer addSublayer:self.timeLineLayer];
}
```
## 5.心跳效果
```
/**添加心跳效果*/
- (void)setupheartLayer {
    self.heartLayer.frame = CGRectMake(0, 0, 4, 4);
    self.heartLayer.position = self.currentPoint;
    self.heartLayer.cornerRadius = _heartLayer.frame.size.width*0.5;
    self.heartLayer.masksToBounds = YES;
    [self.layer addSublayer:self.heartLayer];
    [self heartAnimationWithLayer:self.heartLayer];
}
```
## 6.添加手势
说明:
- 长按手势 :触发方法画十字交叉线及其与坐标轴的交叉点文字
- 点击手势: 影藏十字交叉线及对应文字
```
/**添加手势*/
- (void)setupGestureRecognize {
    UILongPressGestureRecognizer  *longPress = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(longPressAction:)];
    [self addGestureRecognizer:longPress];
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tap:)];
    [self addGestureRecognizer:tap];
}
/**长按*/
- (void)longPressAction:(UILongPressGestureRecognizer*)gesture {
    CGPoint tempPoint = [gesture locationInView:self];
    //越界控制
    if (tempPoint.x >= DPW) {
        tempPoint = CGPointMake(DPW, tempPoint.y);
    }
    if (tempPoint.x <= 0.0) {
        tempPoint = CGPointMake(0, tempPoint.y);
    }
    if (tempPoint.y >= DPH - 60) {
        tempPoint = CGPointMake(tempPoint.x, DPH-60);
    }
    if (tempPoint.y <= 0.0) {
        tempPoint = CGPointMake(tempPoint.x, 0);
    }
    if (gesture.state == UIGestureRecognizerStateBegan) {
        [self drawCrossLineWithPoint:tempPoint];
    }else if (gesture.state == UIGestureRecognizerStateChanged) {
        [self drawCrossLineWithPoint:tempPoint];
    }else if (gesture.state == UIGestureRecognizerStateEnded) {
    }
}
/**点击*/
- (void)tap:(UITapGestureRecognizer*) gesture {
    self.crossLayer.path = nil;
    [self.crossPriceLayer removeFromSuperlayer];
    [self.crossTimeLayer removeFromSuperlayer];
}
```
## 完整代码 拿走不谢
```
#import "DPTimeLineView.h"
#import "DPTimeLineItem.h"
#define DPW self.frame.size.width
#define DPH self.frame.size.height
@interface DPTimeLineView ()
/**纵坐标最大值*/
@property (nonatomic, assign) double max;
/**纵坐标中间值*/
@property (nonatomic, assign) double mid;
/**纵坐标最小值*/
@property (nonatomic, assign) double min;
/**分时线路径*/
@property (nonatomic, strong) UIBezierPath *timeLinePath;
/**最新点坐标*/
@property (nonatomic, assign) CGPoint currentPoint;
/**是否显示背景*/
@property (nonatomic, assign) BOOL isNeedBackGroundColor;
/**心跳点*/
@property (nonatomic, strong) CALayer *heartLayer;
/**分时线*/
@property (nonatomic, strong) CAShapeLayer *timeLineLayer;
/**十字光标*/
@property (nonatomic, strong) CAShapeLayer *crossLayer;
/**十字光标时间轴文字*/
@property (nonatomic, strong) CATextLayer *crossTimeLayer;
/**十字光标价格轴文字*/
@property (nonatomic, strong) CATextLayer *crossPriceLayer;
@end
@implementation DPTimeLineView
#pragma mark - 入口
//alloc方法调用
- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        self.backgroundColor = [UIColor whiteColor];
        [self setupGestureRecognize];
    }
    return self;
}
//load nib 调用
- (instancetype)initWithCoder:(NSCoder *)aDecoder {
    if (self = [super initWithCoder:aDecoder]) {
        self.backgroundColor = [UIColor whiteColor];
        [self setupGestureRecognize];
    }
    return self;
}
#pragma mark - 懒加载
- (UIBezierPath *)timeLinePath {
    if (!_timeLinePath) {
        _timeLinePath = [[UIBezierPath alloc]init];
    }
    return _timeLinePath;
}
- (CALayer *)heartLayer {
    if (!_heartLayer) {
        _heartLayer = [CALayer layer];
        _heartLayer.backgroundColor = [UIColor blueColor].CGColor;
    }
    return _heartLayer;
}
- (CAShapeLayer *)timeLineLayer {
    if (!_timeLineLayer) {
        _timeLineLayer = [[CAShapeLayer alloc]init];
        _timeLineLayer.strokeColor = [UIColor redColor].CGColor;
        _timeLineLayer.lineWidth = 0.5f;
    }
    return _timeLineLayer;
}
- (CAShapeLayer *)crossLayer {
    if (!_crossLayer) {
        _crossLayer =[[CAShapeLayer alloc]init];
        _crossLayer.lineDashPattern = @[@1, @2];//画虚线
    }
    return _crossLayer;
}
- (CATextLayer *)crossPriceLayer {
    if (!_crossPriceLayer) {
        _crossPriceLayer = [[CATextLayer alloc]init];
    }
    return _crossPriceLayer;
}
- (CATextLayer *)crossTimeLayer {
    if (!_crossTimeLayer) {
        _crossTimeLayer = [[CATextLayer alloc]init];
    }
    return _crossTimeLayer;
}
#pragma mark - GestureRecognize
/**添加手势*/
- (void)setupGestureRecognize {
    UILongPressGestureRecognizer  *longPress = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(longPressAction:)];
    [self addGestureRecognizer:longPress];
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tap:)];
    [self addGestureRecognizer:tap];
}
/**长按*/
- (void)longPressAction:(UILongPressGestureRecognizer*)gesture {
    CGPoint tempPoint = [gesture locationInView:self];
    //越界控制
    if (tempPoint.x >= DPW) {
        tempPoint = CGPointMake(DPW, tempPoint.y);
    }
    if (tempPoint.x <= 0.0) {
        tempPoint = CGPointMake(0, tempPoint.y);
    }
    if (tempPoint.y >= DPH - 60) {
        tempPoint = CGPointMake(tempPoint.x, DPH-60);
    }
    if (tempPoint.y <= 0.0) {
        tempPoint = CGPointMake(tempPoint.x, 0);
    }
    if (gesture.state == UIGestureRecognizerStateBegan) {
        [self drawCrossLineWithPoint:tempPoint];
    }else if (gesture.state == UIGestureRecognizerStateChanged) {
        [self drawCrossLineWithPoint:tempPoint];
    }else if (gesture.state == UIGestureRecognizerStateEnded) {
    }
}
/**点击*/
- (void)tap:(UITapGestureRecognizer*) gesture {
    self.crossLayer.path = nil;
    [self.crossPriceLayer removeFromSuperlayer];
    [self.crossTimeLayer removeFromSuperlayer];
}
#pragma mark - 数据入口
- (void)setDataArrM:(NSMutableArray<DPTimeLineItem *> *)dataArrM {
    _dataArrM = dataArrM;
    //0.数据处理,拿到纵坐标数据
    [self preworkForData];
    //1.画框架
    [self drawFramework];
    //2.画文字
    //3.画分时线
    [self drawTimeLine];
    //4.画末尾的心跳点
    [self setupheartLayer];
}
#pragma mark - 画图方法
- (void)drawTimeLine  {
    if (self.isNeedBackGroundColor) {
        [self.timeLinePath addLineToPoint:CGPointMake(self.currentPoint.x , DPH - 60)];
        self.timeLineLayer.fillColor = [UIColor lightGrayColor].CGColor;
    }else {
        self.timeLineLayer.fillColor = [UIColor clearColor].CGColor;
    }
    self.timeLineLayer.path = self.timeLinePath.CGPath;
    [self.layer addSublayer:self.timeLineLayer];
}
/**画框架*/
- (void)drawFramework {
    UIBezierPath *path = [[UIBezierPath alloc]init];
    //三条横线
    CGFloat rowSpace = (DPH -60.0)/2.0;
    NSString *tempStr = @"";
    for (int i = 0; i < 3; i ++) {
        [path moveToPoint:CGPointMake(0,rowSpace*i)];
        [path addLineToPoint:CGPointMake(DPW, rowSpace*i)];
        if (0 == i) {
            tempStr = [NSString stringWithFormat:@"%.2f",self.max];
        }else if (1==i) {
            tempStr = [NSString stringWithFormat:@"%.2f",self.mid];
        }else if (2 == i) {
            tempStr = [NSString stringWithFormat:@"%.2f",self.min];
        }
        [self drawLabelAtRect:CGRectMake(-25, rowSpace*i-10, 25, 20) textStr:tempStr];
    }
    //4条竖线
    CGFloat colSpace = DPW / 4.0;
    for (int i = 0; i < 5; i ++) {
        [path moveToPoint:CGPointMake(colSpace*i,0)];
        [path addLineToPoint:CGPointMake(colSpace*i, DPH-60)];
        if (0 == i) {
            tempStr = @"9:30";
            [self drawLabelAtRect:CGRectMake(colSpace*i, DPH-60 ,30,20) textStr:tempStr];
            continue;
        }else if (1==i) {
            tempStr = @"10:30";
        }else if (2 == i) {
            tempStr = @"11:30/13:00";
            [self drawLabelAtRect:CGRectMake(colSpace*i-30, DPH-60 ,70,20) textStr:tempStr];
            continue;
        }else if (3==i) {
            tempStr = @"14:00";
        }else if (4 == i) {
            tempStr = @"15:00";
            [self drawLabelAtRect:CGRectMake(colSpace*i - 30, DPH-60 ,30,20) textStr:tempStr];
            continue;
        }
        [self drawLabelAtRect:CGRectMake(colSpace*i-15, DPH-60 ,35,20) textStr:tempStr];
    }
    CAShapeLayer *shapeLayer = [[CAShapeLayer alloc]init];
    shapeLayer.strokeColor = [UIColor lightGrayColor].CGColor;
    shapeLayer.path = path.CGPath;
    [self.layer addSublayer:shapeLayer];
}
/**画文字,直接创建一个CATextLayer*/
- (void)drawLabelAtRect:(CGRect)rect textStr:(NSString*)textStr {
    CATextLayer *textLayer = [CATextLayer layer];
    textLayer.frame = rect;
    [self.layer addSublayer:textLayer];
    //set text attributes
    textLayer.foregroundColor = [UIColor blackColor].CGColor;
    textLayer.alignmentMode = kCAAlignmentJustified;
    textLayer.wrapped = YES;
    //choose a font
    UIFont *font = [UIFont systemFontOfSize:10];
    //set layer font
    CFStringRef fontName = (__bridge CFStringRef)font.fontName;
    CGFontRef fontRef = CGFontCreateWithFontName(fontName);
    textLayer.font = fontRef;
    textLayer.fontSize = font.pointSize;
    CGFontRelease(fontRef);
    textLayer.contentsScale = [UIScreen mainScreen].scale;
    //choose some text
    //set layer text
    textLayer.string = textStr;
}
/**画文字,指定CATextLayer*/
- (void)drawCrossLabelWithTextLayer:(CATextLayer*)textLayer AtRect:(CGRect)rect textStr:(NSString*)textStr {
    textLayer.frame = rect;
    [self.layer addSublayer:textLayer];
    //set text attributes
    textLayer.foregroundColor = [UIColor blackColor].CGColor;
    textLayer.alignmentMode = kCAAlignmentJustified;
    textLayer.wrapped = YES;
    //choose a font
    UIFont *font = [UIFont systemFontOfSize:10];
    //set layer font
    CFStringRef fontName = (__bridge CFStringRef)font.fontName;
    CGFontRef fontRef = CGFontCreateWithFontName(fontName);
    textLayer.font = fontRef;
    textLayer.fontSize = font.pointSize;
    CGFontRelease(fontRef);
    textLayer.contentsScale = [UIScreen mainScreen].scale;
    //choose some text
    //set layer text
    textLayer.string = textStr;
}
/**画十字光标线*/
- (void)drawCrossLineWithPoint:(CGPoint)point {
    UIBezierPath * path = [[UIBezierPath alloc]init];
    [path moveToPoint:CGPointMake(point.x, 0)];
    [path addLineToPoint:CGPointMake(point.x, DPH - 60)];
    [path moveToPoint:CGPointMake(0, point.y)];
    [path addLineToPoint:CGPointMake(DPH, point.y)];
    self.crossLayer.strokeColor = [UIColor blueColor].CGColor;
    self.crossLayer.path = path.CGPath;
    [self.layer addSublayer: self.crossLayer];
    //画坐标点对于文字
    NSString *price =  [self pricWithPoint:point];
    NSString *time = [self timeWithPoint:point];
    [self drawCrossLabelWithTextLayer:self.crossTimeLayer AtRect:CGRectMake(point.x, DPH-60, 30, 20) textStr:time];
    [self drawCrossLabelWithTextLayer:self.crossPriceLayer AtRect:CGRectMake(0, point.y, 30, 20) textStr:price];
}
/**添加心跳效果*/
- (void)setupheartLayer {
    self.heartLayer.frame = CGRectMake(0, 0, 4, 4);
    self.heartLayer.position = self.currentPoint;
    self.heartLayer.cornerRadius = _heartLayer.frame.size.width*0.5;
    self.heartLayer.masksToBounds = YES;
    [self.layer addSublayer:self.heartLayer];
    [self heartAnimationWithLayer:self.heartLayer];
}
#pragma mark - Others
/**数据处理*/
- (void)preworkForData {
    //0 拿到纵坐标的3个点(max,mid,min),mid=昨日收盘价
    DPTimeLineItem *temp = self.dataArrM[0];
    self.mid = [temp.pre_close_px doubleValue];
    if ([temp.last_px doubleValue] -[temp.pre_close_px doubleValue] > 0) {
        self.max = [temp.last_px doubleValue];
        self.min = [temp.pre_close_px doubleValue] -([temp.last_px doubleValue] -[temp.pre_close_px doubleValue] );
    }else {
        self.min = [temp.last_px doubleValue];
        self.max =  [temp.pre_close_px doubleValue] + ([temp.pre_close_px doubleValue] -[temp.last_px doubleValue] );
    }
    for (int i = 0; i< self.dataArrM.count; i ++ ) {
        DPTimeLineItem *item = self.dataArrM[i];
        //获取纵坐标最大,最小,中间值
        if (fabs(item.last_px.doubleValue - self.mid) >(self.max - self.mid)) {
            self.max = self.mid +fabs(item.last_px.doubleValue - self.mid);
            self.min = self.mid - fabs(item.last_px.doubleValue - self.mid);
        }
        //画分时线
        CGFloat timeX = [self miniteTimeWithTimeStr:item.curr_time];
        CGFloat priceY = [self priceLabelLocationWithPrice:item.last_px.floatValue];
        if (i == 0) {
            if (self.isNeedBackGroundColor) {
                [self.timeLinePath moveToPoint:CGPointMake(0, DPH - 60)];
                [self.timeLinePath addLineToPoint:CGPointMake(timeX, priceY)];
            }else {
                [self.timeLinePath moveToPoint:CGPointMake(timeX, priceY)];
            }
        }
        if (i == self.dataArrM.count-1) {
            self.currentPoint = CGPointMake(timeX, priceY);
        }
        [self.timeLinePath addLineToPoint:CGPointMake(timeX,  priceY)];
    }
}
/**根据时间获取坐标点*/
-(CGFloat)miniteTimeWithTimeStr:(NSString*)timeStr {
    if (timeStr == nil || [timeStr isEqualToString:@""]) return 0.0;
    NSArray *temp = [timeStr componentsSeparatedByString:@":"];
    NSInteger minte = [temp[0] integerValue]*60 + [temp[1] integerValue];
    //每分钟代表的宽度
    CGFloat aveWidth = DPW/ 240.0;
    if (minte <= 11*60+30) {//上午
        return aveWidth*(minte - 9*60-30);
    }else {//下午
        return DPW *0.5 + aveWidth*(minte - 13*60);
    }
}
/**根据坐标点获取对应时间*/
-(NSString*)timeWithPoint:(CGPoint)point{
    NSString *str = @"";
    //单位距离代表的时间
    CGFloat aveWidth = 240.0/DPW;
    CGFloat totalTime = aveWidth * point.x + 9*60 + 30;//上午
    if (point.x >DPW *0.5) {
        totalTime = aveWidth * point.x + 11*60;//下午
    }
    int h = totalTime / 60;
    int m =  ((int)(totalTime + 0.5)) % 60;
    str = [NSString stringWithFormat:@"%d:%d",h,m];
    return str;
}
/**根据价格获取坐标位置*/
- (CGFloat)priceLabelLocationWithPrice:(CGFloat)price {
    CGFloat height = DPH - 60;
    CGFloat avrSpace = height/(self.max - self.min);
    return height - ((price - self.min)*avrSpace);
}
/**根据坐标位置获取价格*/
- (NSString*)pricWithPoint:(CGPoint)point {
    NSString *price = @"";
    //单位距离代表的价格
    CGFloat avePrice = (self.max - self.min)/(DPH - 60);
    //保留两位小数
    price = [NSString stringWithFormat:@"%.2f",(self.max - point.y*avePrice)+0.005];
    return price;
}
/**心跳动画*/
- (void)heartAnimationWithLayer:(CALayer*)layer {
    CAAnimationGroup *group = [[CAAnimationGroup alloc]init];
    group.duration = 0.5;
    group.repeatCount = HUGE;
    CABasicAnimation *scaleAnim = [CABasicAnimation animationWithKeyPath:@"transform.scale"];
    scaleAnim.toValue = @1.2;
    CABasicAnimation *alphaAnim = [CABasicAnimation animationWithKeyPath:@"opacity"];
    alphaAnim.toValue = @0.3f;
    group.animations = @[scaleAnim,alphaAnim];
    [layer addAnimation:group forKey:nil];
}
```
## 提供一个数据入口,每次收到数据就重新划线
```
@class DPTimeLineItem;
@interface DPTimeLineView : UIView
/**分时数据列表*/
@property (nonatomic, strong)NSMutableArray<DPTimeLineItem*> *dataArrM;
@end
#pragma mark - 数据入口
- (void)setDataArrM:(NSMutableArray<DPTimeLineItem *> *)dataArrM {
    _dataArrM = dataArrM;
    //0.数据处理,拿到纵坐标数据
    [self preworkForData];
    //1.画框架
    [self drawFramework];
    //2.画文字
    //3.画分时线
    [self drawTimeLine];
    //4.画末尾的心跳点
    [self setupheartLayer];
}
```
## 备注:本文中之所以主要使用CAShapeLayer来画线,主要在于它的以下优势;
- 渲染快,采用硬件加速,比用Core Graphics快很多;
- 高效使用内存,由于不需要像普通layer那样创建寄宿图,所以无论改layer多大,都不要占用太多内存;
- 超出图层依然可以绘图,不会被图层边界裁剪掉;
- 不会出现像素化, CAShapeLayer在做3D变换时,不会像普通有寄宿图的CALayer那样变得像素画.
