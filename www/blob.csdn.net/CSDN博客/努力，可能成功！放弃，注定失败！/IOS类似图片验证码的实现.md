# IOS类似图片验证码的实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月05日 09:28:53[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4857
1.初始化一个数组来存数字和字母，随机获取这个数组的4个元素组成一个验证码字符串（以4位验证码为例子 如“4kH6”）。
```
```
- (void)changeCodeStr{
    self.textArray = [[NSArray alloc] initWithObjects:@"0",@"1",@"2",@"3",@"4",@"5",@"6",@"7",@"8",@"9",@"A",@"B",@"C",@"D",@"E",@"F",@"G",@"H",@"I",@"J",@"K",@"L",@"M",@"N",@"O",@"P",@"Q",@"R",@"S",@"T",@"U",@"V",@"W",@"X",@"Y",@"Z",@"a",@"b",@"c",@"d",@"e",@"f",@"g",@"h",@"i",@"j",@"k",@"l",@"m",@"n",@"o",@"p",@"q",@"r",@"s",@"t",@"u",@"v",@"w",@"x",@"y",@"z",nil];
    for(NSInteger i = 0; i < 4; i++)
    {
        NSInteger index = arc4random() % ([self.textArray count] - 1);
        NSString *oneText = [self.textArray objectAtIndex:index];
        self.imageCodeStr = (i==0)?oneText:[self.imageCodeStr stringByAppendingString:oneText];
    }
    if (self.bolck) {
      //将验证码通过block的方式传出去
        self.bolck(self.imageCodeStr);
    }
}
```
```
2.将view等分成四份，每份add一个label，label的位置随机生成，取出验证码的字符串中每个字符，并将字符写在lable上。
```
-(void)initImageCodeView{
if (_bgView) {
        [_bgView removeFromSuperview];
    }    _bgView = [[UIView alloc]initWithFrame:self.bounds];
    [self addSubview:_bgView];
    [_bgView setBackgroundColor:[self getRandomBgColorWithAlpha:0.5]];
    CGSize textSize = [@"W" sizeWithAttributes:@{NSFontAttributeName:[UIFont systemFontOfSize:20]}];
    //每个label能随机产生的位置的point.x的最大范围
    int randWidth = (self.frame.size.width)/self.imageCodeStr.length - textSize.width;
  //每个label能随机产生的位置的point.y的最大范围
    int randHeight = self.frame.size.height - textSize.height;
    for (int i = 0; i<self.imageCodeStr.length; i++) {
        //随机生成每个label的位置CGPoint(x,y)
        CGFloat px = arc4random()%randWidth + i*(self.frame.size.width-3)/self.imageCodeStr.length;
        CGFloat py = arc4random()%randHeight;
        UILabel * label = [[UILabel alloc] initWithFrame: CGRectMake(px+3, py, textSize.width, textSize.height)];
        label.text = [NSString stringWithFormat:@"%C",[self.imageCodeStr characterAtIndex:i]];
        label.font = [UIFont systemFontOfSize:20];
        //label是否是可以是斜的，isRotation这个属性暴露在外面，可进行设置
        if (self.isRotation) {
            double r = (double)arc4random() / ARC4RAND_MAX * 2 - 1.0f;//随机生成-1到1的小数
            if (r>0.3) {
                r=0.3;
            }else if(r<-0.3){
                r=-0.3;
            }
            label.transform = CGAffineTransformMakeRotation(r);
        }
        [_bgView addSubview:label];
    }
```
3.随机获取view中的两个点画线，使用UIBezierPath画两点的线，根据需要随机画几条线。
```
for (int i = 0; i<10; i++) {
     UIBezierPath *path = [UIBezierPath bezierPath];
     CGFloat pX = arc4random() % (int)CGRectGetWidth(self.frame);
     CGFloat pY = arc4random() % (int)CGRectGetHeight(self.frame);
     [path moveToPoint:CGPointMake(pX, pY)];
     CGFloat ptX = arc4random() % (int)CGRectGetWidth(self.frame);
     CGFloat ptY = arc4random() % (int)CGRectGetHeight(self.frame);
     [path addLineToPoint:CGPointMake(ptX, ptY)];
     CAShapeLayer *layer = [CAShapeLayer layer];
     layer.strokeColor = [[self getRandomBgColorWithAlpha:0.2] CGColor];//layer的边框色
     layer.lineWidth = 1.0f;
     layer.strokeEnd = 1;
     layer.fillColor = [UIColor clearColor].CGColor;
     layer.path = path.CGPath;
     [_bgView.layer addSublayer:layer];
 }
```
4.随机获取背景颜色
```cpp
```cpp
-(UIColor *)getRandomBgColorWithAlpha:(CGFloat)alpha{
    float red = arc4random() % 100 / 100.0;
    float green = arc4random() % 100 / 100.0;
    float blue = arc4random() % 100 / 100.0;
    UIColor *color = [UIColor colorWithRed:red green:green blue:blue alpha:alpha];
    return color;
}
```
```
5.刷新验证码的操作方法，这个是暴露在外面的的方法，做刷新调用。
```
-(void)freshVerCode
{
    [self changeCodeStr];
    [self initImageCodeView];
}
```
6.随机生成-1到1的小数
```cpp
```cpp
#define ARC4RAND_MAX 0x100000000
double r = (double)arc4random() / ARC4RAND_MAX * 2 - 1.0f;
```
```
7.效果图如下
![](http://upload-images.jianshu.io/upload_images/1693185-4f7c86a57fbb06f6.gif?imageMogr2/auto-orient/strip)
verCodeView.gif
源码地址：[[https://github.com/meiqi1992/MQVerCodeView.git](https://github.com/meiqi1992/MQVerCodeView.git)]
## 注：第一次写东西，希望大家多多指导！！！
文／LinMeiQi（简书作者）
原文链接：http://www.jianshu.com/p/a52e375b20a9
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
