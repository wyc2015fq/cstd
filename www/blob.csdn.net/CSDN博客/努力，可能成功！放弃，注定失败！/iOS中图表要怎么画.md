# iOS中图表要怎么画 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月01日 09:25:54[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2902
因为公司是做移动医疗APP的，里面会牵扯到用户的数据问题，关于用户每天记录的数据要怎么显示，这时就用到了图表了，有了图表它可以明了的显示用户的数据，提高用户的体验。
那下面就要说这个图标到底该怎么画，思路在那里，要往哪方面想。
先看下效果图吧：
![](http://upload-images.jianshu.io/upload_images/1830264-dd02bef9be0917e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-06-03 11.53.04.png
## 1.思路
该图表是用上下文，也就是CGContextRef进行绘制，大家可能对它不是很陌生，它可以画一条线，画个正方形，长方形，圆等，有很多作用，该文章就是在于怎么通过这点，线，圆，画出你需要的表格。
首先你要计算好，你要画多大的图表，要分成几个间隔，有哪些变量等。
## 2.主要代码
先创建一个类.h里面
```
typedef enum DrowLineType{
    PointType = 0,
    BrokenType ,
    BezierType,
}DrowLineType;
@interface ZYJ_Draw : UIView
// 数据
-(void)setData:(NSMutableArray*)LevelDataArray;
// 获取绘制类型
@property(assign,nonatomic) DrowLineType * LineType;
//点数据的日期
-(void)drawData:(NSMutableArray *)drawDataArray;
//画点的数据
-(void)drawDatacount:(NSMutableArray *)drawDataCountArry;
```
.m里面，在这里面，我要分开说，把这个图表分成部分来写。
声明
```
#define POINT_CIRCLE  6.0f
@interface ZYJ_Draw()
{
@private
    NSArray * KLevelDataArray;//横坐标数据
    NSMutableArray *KdrawDataCountArry;
    NSMutableArray *kdrawDataArray;
    float _width;
    float _height;
    NSMutableArray *SeconArry; //计算时间的数据
}
@property(nonatomic)float width;
@property(nonatomic)float height;
@end
```
### 1.横线
上下文
```
// 获取上下文,进行绘制
    CGContextRef ContextRef = UIGraphicsGetCurrentContext();
```
画横线，横线是12条
```
// 线的颜色 横线
    CGContextSetStrokeColorWithColor(ContextRef, [UIColor lightGrayColor].CGColor);
    for (int i =0 ; i<12; i++) {
        if (i!=0||i!=11) {
             CGContextSetLineWidth(ContextRef, 0.9);
        }
        CGContextMoveToPoint(ContextRef, 0,i*self.frame.size.height/11);
        CGContextAddLineToPoint(ContextRef,self.frame.size.width,(self.frame.size.height/11)*i);
        CGContextStrokePath(ContextRef);
    }
```
### 2.竖线
```cpp
```cpp
// 设置线的宽度 竖线
    for (int i= 0; i< 7; i++) {
        if (i==0||i==6) {
            CGFloat lengths[] = {1,1};
            CGContextSetLineDash(ContextRef, 0, lengths, 0);
        }
        else
        {
            CGFloat lengths[] = {3,3};
            CGContextSetLineDash(ContextRef, 0, lengths, 2);
        }
        CGContextMoveToPoint(ContextRef, i*self.frame.size.width/6, self.frame.size.height);
        CGContextAddLineToPoint(ContextRef, i*self.frame.size.width/6,0);
        CGContextStrokePath(ContextRef);
    }
```
```
到这的话，图表的大致就好了，剩下的x，y轴，和折线的形成了
### 3.x轴
```
// 水平坐标的数据
    for (int i = 0 ; i<KLevelDataArray.count; i++) {
        float x=self.bounds.size.width/(KLevelDataArray.count-1);
        UILabel * LevelLabel= [[UILabel alloc]initWithFrame:CGRectMake(i*x-20, self.bounds.size.height+5, 40, 10)];
        LevelLabel.textAlignment=NSTextAlignmentCenter;
        LevelLabel.text = KLevelDataArray[i];
        //LevelLabel.backgroundColor=[UIColor redColor];
        LevelLabel.textColor = UIColorFromRGB(0x333333);
        LevelLabel.font = [UIFont systemFontOfSize:12];
        [ self addSubview:LevelLabel];
    }
```
### 4.y轴
y轴没有写到这个类里面，写到了Controller里面
```cpp
```cpp
//画竖直的坐标
    for (int i =(int)self.VerticalDataArray.count -1 ; i>=0; i--) {
        UILabel * VerticalDataArrayLabel = [[UILabel alloc]init];
        [drwaBG addSubview:VerticalDataArrayLabel];
        float y=i*(430/(self.VerticalDataArray.count+1));
        VerticalDataArrayLabel.textAlignment=NSTextAlignmentCenter;
        [VerticalDataArrayLabel mas_makeConstraints:^(MASConstraintMaker *make) {
            make.top.equalTo(titleLable.mas_bottom).offset(XYZScreenRateIphoneHeight*(20+y));
            make.left.equalTo(drwaBG).offset(XYZScreenRateIphoneWidth*0);
            make.height.equalTo(@(XYZScreenRateIphoneWidth*10));
            make.width.equalTo(@(XYZScreenRateIphoneWidth*30));
        }];
        VerticalDataArrayLabel.font = [UIFont systemFontOfSize:12];
        VerticalDataArrayLabel.text = self.VerticalDataArray[self.VerticalDataArray.count-i-1];
    }
```
```
### 5.折线
在画折线时，要确定一个点x,y坐标才能画点，我用的是用测量时间确定x坐标，也就是这个点距离最左边的的距离，测量的值确定y轴。
确定x坐标
如上图可以看到，我在画竖线时，每个间隔是有一定的宽度，我把它转化分钟，下面是该点距离最左边的宽度，也就是x的位移
```cpp
```cpp
NSString *strtime=SeconArry[i];
        int  j=[strtime intValue];
        float width=j*self.frame.size.width/6/4/60+1.5;
```
```
确定y坐标
因为y坐标是变化，它是变化的，下面是一个点的y坐标
```cpp
```cpp
if (count>=4.0&&count<=9.0) {
            float height=self.frame.size.height-star-2*star-(count-4)*star-3;
            self.height=height;
            CGContextSetFillColor(ContextRef,  CGColorGetComponents(UIColorFromRGB(0x24b3fa).CGColor));
        }
```
```
折线代码
```cpp
```cpp
float star=self.frame.size.height/11;
    //画折线
    CGContextSetStrokeColorWithColor(ContextRef, [UIColor blackColor].CGColor);
    CGContextSetLineWidth(ContextRef, 0.6f);
    NSLog(@"%@",KdrawDataCountArry);
    //CGContextMoveToPoint(ContextRef, 20, 200);
    for (NSInteger i=0; i<KdrawDataCountArry.count; i++) {
        CGContextSetLineDash(ContextRef, 0, 0, 0);
        NSString *strtime=SeconArry[i];
        int  j=[strtime intValue];
        float width=j*self.frame.size.width/6/4/60+1.5;
        NSString *str=KdrawDataCountArry[i];
        float count=[str floatValue];
        if (count>=4.0&&count<=9.0) {
            float height=self.frame.size.height-star-2*star-(count-4)*star;
            self.height=height;
        }
        else if (count<4)
        {
            float height=self.frame.size.height-star-count*star/2;
            self.height=height;
        }
        else if(count>9.0&&count<=11.0)
        {
            float height=self.frame.size.height-star-7*star-(count-9)*star/2;
            self.height=height;
        }
        else if (count>11.0&&count<15.0)
        {
            float height=self.frame.size.height-star-8*star-(count-11.0)*star/4;
            self.height=height;
        }
        else
        {
            float height=self.frame.size.height-star-9*star-(count-15.0)*star/18;
            self.height=height;
        }
        //开始画线
        if (i==0) {
            CGContextMoveToPoint(ContextRef, width, self.height);
        }
        else
        {
            CGContextAddLineToPoint(ContextRef, width, self.height);
        }
    }
    CGContextStrokePath(ContextRef);
```
```
### 6.画点
画点和画折线几本上是一样的，下面就是画点的程序
```
// 进行绘图 画点
    for (NSInteger i=0; i<KdrawDataCountArry.count; i++) {
        NSString *strtime=SeconArry[i];
        int  j=[strtime intValue];
        float width=j*self.frame.size.width/6/4/60-1.5;
        NSString *str=KdrawDataCountArry[i];
        float count=[str floatValue];
        if (count>=4.0&&count<=9.0) {
            float height=self.frame.size.height-star-2*star-(count-4)*star-3;
            self.height=height;
            CGContextSetFillColor(ContextRef,  CGColorGetComponents(UIColorFromRGB(0x24b3fa).CGColor));
        }
        else if (count>9.0&&count<=11.0)
        {
            float height=self.frame.size.height-star-7*star-(count-9)*star/2;
            self.height=height;
            CGContextSetFillColor(ContextRef,  CGColorGetComponents([UIColor redColor].CGColor));
        }
        else if (count>11.0&&count<=15.0)
        {
            float height=self.frame.size.height-star-8*star-(count-11.0)*star/4;
            self.height=height;
            CGContextSetFillColor(ContextRef,  CGColorGetComponents([UIColor redColor].CGColor));
        }
        else if (count>15.0&&count<=33.0)
        {
            float height=self.frame.size.height-star-9*star-(count-15.0)*star/18;
            self.height=height;
            CGContextSetFillColor(ContextRef,  CGColorGetComponents([UIColor redColor].CGColor));
        }
        else
        {
            float height=self.frame.size.height-star-count*star/2;
            self.height=height;
            CGContextSetFillColor(ContextRef,  CGColorGetComponents([UIColor redColor].CGColor));
        }
        CGContextFillEllipseInRect(ContextRef, CGRectMake(width, self.height, 6.0, 6.0));
    }
```
以上就是用上下文画表格的主要代码，有没有感觉很简单，其实一点点的来的话，也没有很难，下面是完整的效果图
![](http://upload-images.jianshu.io/upload_images/1830264-8f5a795a08bd1115.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-06-03 15.12.47.png
如果有什么疑问可以给我留言，我会及时改正。
代码下载地址[https://github.com/zhaoyanjie/ZYJ_DrawViewController.git](https://github.com/zhaoyanjie/ZYJ_DrawViewController.git)
网盘 下载地址 [http://pan.baidu.com/s/1nvvUgS5](http://pan.baidu.com/s/1nvvUgS5)
文／敲代码的小矮人（简书作者）
原文链接：http://www.jianshu.com/p/c9a07b97d3ee
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
