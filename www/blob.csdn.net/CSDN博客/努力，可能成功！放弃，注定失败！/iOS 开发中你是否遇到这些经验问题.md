# iOS 开发中你是否遇到这些经验问题 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月08日 08:39:34[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：449
## 前言
> 
小伙伴们在开发中难免会遇到问题, 你是如何解决问题的？不妨也分享给大家！如果此文章其中的任何一条问题对大家有帮助，那么它的存在是有意义的！ 反正不管怎样遇到问题就要去解决问题, 在解决问题的同时也是提高开发经验的渠道!
##### 1.既然有问题我们该怎样解决 ?
首先大部分人都会去百度搜索来解决问题, 谁都不例外, 可是百度这东西会有很多误解, 甚至误人子弟, 同时解决问题的效率也不是很高, 如果是技术问题可以去:
- `Google` 简单来说比百度搜索的答案相对要多,要更准确效率更高, 建议使用英文搜索
- `Stack Overflow` 一个与程序相关的IT技术问答网站, 简单来说工作中的`70%`问题在这里都能解决
搜索是解决问题的一种办法, 那么剩余`30%`的问题还是需要自己独立去解决, 那么有人会问有没有能提高独立解决问题的办法呢 ? 比如: 积累所遇到的崩溃信息, 学会查看崩溃信息, 查看官方文档的技巧等等) 本文暂时不会详解, 有时间的话需要单独写一篇文章来探究如何高效的解决崩溃问题!
##### 2.用断点调试崩溃问题的小技巧
- (1)如果当前断点指向内存, 那么就是初始化时分配内存时有问题
- (2)添加全局断点的问题:
![](http://upload-images.jianshu.io/upload_images/2353624-6e6f508188bfe97a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
添加全局断点.png
全局断点的目的就是当产生`异常`或`崩溃`能够停留在`异常`或者`崩溃`发生的地方而不是跳转到主函数
![](http://upload-images.jianshu.io/upload_images/2353624-9ea1209e52aaee04.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
崩溃断点.png
但是添加全局断点后会跳转到主函数, 会遇到如上图的情况, 那么`80%`的问题就是`storyboard`或者`xib`拖线没有及时清理掉`坏线`如下图:
![](http://upload-images.jianshu.io/upload_images/2353624-5c4a15bb16ed6d32.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
拖线没有清除.png
这条`Button`的连线在代码中已经删除了不用了, 但是没有及时清理掉,所以会造成`崩溃`!
##### 3.NSString使用stringWithFormat拼接的相关知识
- 保留`2`位小数点
```
//.2代表小数点后面保留2位(2代表保留的数量)
NSString *string = [NSString stringWithFormat:@"%.2f",M_PI];
//输出结果是: 3.14
NSLog(@"%@", string);
```
- 用`0`补全的方法
```
NSInteger count = 5;
//02代表:如果count不足2位 用0在最前面补全(2代表总输出的个数)
NSString *string = [NSString stringWithFormat:@"%02zd",count];
//输出结果是: 05
NSLog(@"%@", string);
```
- 字符串中有特殊符号`%`怎么办
```
NSInteger count = 50;
//%是一个特殊符号 如果在NSString中用到%需要如下写法
NSString *string = [NSString stringWithFormat:@"%zd%%",count];
//输出结果是: 50%
NSLog(@"%@", string);
```
- 字符串中有特殊符号`"`怎么办
```
NSInteger count = 50;
//"是一个特殊符号, 如果在NSString中用到"需要用\进行转义
NSString *string = [NSString stringWithFormat:@"%zd\"",count];
//输出结果是: 50"
NSLog(@"%@", string);
```
##### 4.判断是否为gif/png图片的正确姿势
首先我们先想想如果是你, 你会怎么去判断一个从网络获取的图片`URL`是否为`gif`图片, 是否是这样呢 ? 如下:
```
//假设这是一个网络获取的URL
   NSString *path = @"http://pic3.nipic.com/20090709/2893198_075124038_2.gif";
   // 判断是否为gif
   NSString *extensionName = path.pathExtension;
    if ([extensionName.lowercaseString isEqualToString:@"gif"]) {
        //是gif图片
    } else {
        //不是gif图片
    }
```
什么难道你不是这么判断的么 ? 哦~ 我懂了, 难道你使用字符串截取来判断的吗 ? 哈哈开个玩笑!
以上判断看似是可以的,但是这不严谨的, 在不知道图片扩展名的情况下, 如何知道图片的真实类型 ? 其实就是`取出图片数据的第一个字节, 就可以判断出图片的真实类型`那该怎么做呢如下:
```cpp
```cpp
//通过图片Data数据第一个字节 来获取图片扩展名
- (NSString *)contentTypeForImageData:(NSData *)data {
    uint8_t c;
    [data getBytes:&c length:1];
    switch (c) {
        case 0xFF:
            return @"jpeg";
        case 0x89:
            return @"png";     
        case 0x47:
            return @"gif";        
        case 0x49:   
        case 0x4D:
            return @"tiff";        
        case 0x52:  
            if ([data length] < 12) {
                return nil;
            }
            NSString *testString = [[NSString alloc] initWithData:[data subdataWithRange:NSMakeRange(0, 12)] encoding:NSASCIIStringEncoding];
            if ([testString hasPrefix:@"RIFF"] && [testString hasSuffix:@"WEBP"]) {
                return @"webp";
            }
            return nil;
    }
    return nil;
}
```
```
其实图片数据的第一个字节是固定的,一种类型的图片第一个字节就是它的标识, 我们来调用一下这个方法:
```
//假设这是一个网络获取的URL
    NSString *path = @"http://pic.rpgsky.net/images/2016/07/26/3508cde5f0d29243c7d2ecbd6b9a30f1.png";
    NSData *data = [NSData dataWithContentsOfURL:[NSURL URLWithString:path]];
    //调用获取图片扩展名
    NSString *string = [self contentTypeForImageData:data];
    //输出结果为 png
    NSLog(@"%@",string);
```
##### 5.Button禁止触摸事件的2种方式
大家应该知道, 有很多需求是在规定内不允许点击Button, 并且让用户知道这个按钮是不可以点击的，那我们应该这样设置:
```
//会改变按钮的状态，颜色会变灰
button.enabled = NO;
```
但是又有一个需求是既不能点击也不要改变Button颜色:
```
//保持按钮原来的状态，颜色不会变
button.userInteractionEnabled = NO;
```
##### 6.跟xib一起走过的坑
(1) 如果在`xib`中有一个控件, 已经明确设置尺寸了,输出的`frame`也是对的, 但是显示出来的效果不一样(比如尺寸变大了), 如果是这种情况一般就是`autoresizingMask`自动伸缩属性在搞鬼! 解决办法如下:
```
//xib的awakeFromNib方法中设置UIViewAutoresizingNone进行清空
  - (void)awakeFromNib {
    self.autoresizingMask = UIViewAutoresizingNone;
}
```
(2)如果你的控制器的`view`是用`xib`创建的, 当你拿到`view`的尺寸是不准确的, 在这里我们就需要通过`[UIScreen mainScreen].bounds`拿到尺寸, 但是`storyboard`的尺寸是准确的!
##### 7.你是用什么方法设置图片圆角?
首先你是否是这么设置的:
```
//cornerRadius 设置为self.iconImage图片宽度的一半(圆形图片)
    self.iconImage.layer.cornerRadius = 20;
    self.iconImage.layer.masksToBounds = YES;
```
或者是在`xib`&`storyboard`中点击要设置圆角的图片:
![](http://upload-images.jianshu.io/upload_images/2353624-cce526062c2d783a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
xib或者storyboard设置圆角.png
在此之后建议大家尽量不要这么设置, 因为使用`图层`过量会有卡顿现象, 特别是弄圆角或者阴影会很卡, 如果设置图片圆角我们一般用`绘图`来做:
```
/** 设置圆形图片(放到分类中使用) */
- (UIImage *)cutCircleImage {
    UIGraphicsBeginImageContextWithOptions(self.size, NO, 0.0);
    // 获取上下文
    CGContextRef ctr = UIGraphicsGetCurrentContext();
    // 设置圆形
    CGRect rect = CGRectMake(0, 0, self.size.width, self.size.height);
    CGContextAddEllipseInRect(ctr, rect);
    // 裁剪
    CGContextClip(ctr);
    // 将图片画上去
    [self drawInRect:rect];
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return image;
}
```
这个方法就是设置圆角图片, 效率很高, 不会造成卡顿现象, 大家要把这个方法单独放到分类中使用
##### 8. ## 与 @# 在宏里面该怎样使用
- 
`##`的使用, 首先我们添加一个宏
`#define LRWeakSelf(type)  __weak typeof(type) weak##type = type;`
`##`是连接的作用, 即当使用上面的宏会把`weak`与输入的`type`值连接起来如下图:
![](http://upload-images.jianshu.io/upload_images/2353624-51d711f56b8df62b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 的使用.png
- 
`#`的意思是紧跟着它的后面的标识符添加一个双引号`""`
- `@#`的使用, 我们添加一个普通的宏:
```
//随便写一个宏
#define LRToast(str) [NSString stringWithFormat:@"%@",str]
//这个宏需要这样写
LRToast(@"温馨提示");
NSLog(@"%@",LRToast(@"温馨提示"));
```
强调下我只是随便定义一个宏来做示例, 以上代码是正常的使用,我们在来看看添加`@#`是怎么使用的:
```
//随便写一个宏
  #define LRToast(str) [NSString stringWithFormat:@"%@",@#str]
  //这个宏需要这样写
  LRToast(温馨提示);
  //正常运行, 打印不会报错
  NSLog(@"%@",LRToast(温馨提示));
```
我们可以看出来 `LRToast(温馨提示);`与`LRToast(@"温馨提示");`区别, 也就是说`@#`可以代替`@""` 那么我们以后开发就省事了, 不用再添加`@""`了!
##### 9.自动布局Autolayout口诀
在`storyboard`或者`xib`使用自动布局, 如果控件比较多而且布局复杂, 一不小心就会报一大堆错误警告, 那么这个口诀是必备良药, 跟着这个口诀走再也不用害怕`Autolayout`恐惧症了 !
![](http://upload-images.jianshu.io/upload_images/2353624-78f21c084c43ee8a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Autolayout.png
按照如上图从上到下顺序读就是当前这条约束的状态
`First Item`(登录按钮的顶部)`Relation`(等于) `Second Item`(父类View的顶部)
`Constant`(加上 10) `Multiplier`(乘以 1)
##### 10.App迭代开发版本号的规则
在iOS中简单的版本号是怎样管理的呢? 首先我们的`App`第一版本首次上线, 比如以`1.0.0`为首次上线的版本号:
1.上线后突然发现一个严重的`Bug`那我们就要修复更新版本, 此时我们的版本号为`1.0.1`
所以说如果修复`Bug`或者优化功能, 我们只修改叠加第三位数字, 其他不变
2.如果有了新的需求, 在原来的基础上增加了一个新功能, 那么我们的版本号变为`1.1.0`, 需要清空第三位数字为`0`, 来叠加修改第二位数字
3.如果`App`需求功能大改, 更新量非常大, 那我们的版本号变为`2.0.0`, 需要叠加修改第一位数字, 清空其他数字为`0`
> 
喜欢的小伙伴请点赞一下吧！如果有不足的地方，请大家及时帮忙纠正与补充，顺便谈谈你的建议！
文／判若两人丶（简书作者）
原文链接：http://www.jianshu.com/p/8207621ddcaa
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
