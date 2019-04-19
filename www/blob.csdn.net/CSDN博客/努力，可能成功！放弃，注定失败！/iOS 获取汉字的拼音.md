# iOS 获取汉字的拼音 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月10日 13:18:31[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：644
在ios开发中经常碰需要将汉字转成拼音的情况，以下就是我把汉字转成拼音的方法
## 代码实现
```
+ (NSString *)transform:(NSString *)chinese{
    //将NSString装换成NSMutableString
    NSMutableString *pinyin = [chinese mutableCopy];
    //将汉字转换为拼音(带音标)
    CFStringTransform((__bridge CFMutableStringRef)pinyin, NULL, kCFStringTransformMandarinLatin, NO);
    NSLog(@"%@", pinyin);
    //去掉拼音的音标
    CFStringTransform((__bridge CFMutableStringRef)pinyin, NULL, kCFStringTransformStripCombiningMarks, NO);
    NSLog(@"%@", pinyin);
    //返回最近结果
    return pinyin;
}
```
用`kCFStringTransformMandarinLatin`
方法转化出来的是带音标的拼音，如果需要去掉音标，则继续使用`kCFStringTransformStripCombiningMarks`
方法即可。
## 方法介绍
在iOS中实现上述功能，其实主要是`CFStringTransform`方法出了大力气，我们来重点介绍下这个方法
```
//函数原型
Boolean CFStringTransform(CFMutableStringRef string, CFRange *range, CFStringRef transform, Boolean reverse);
```
##### 参数具体介绍
- ***string***: 需要转换的字符串。由于这个参数是 CFMutableStringRef 类型，一个 NSMutableString 类型也可以通过自由桥接的方式传入。
- ***range***: 转换操作作用的范围。这个参数是 CFRange，而不是 NSRange。**当给定为NULL时，操作范围为全部**。
- ***transform***: 需要应用的变换。这个参数使用了包含下面将提到的字符串常量的 ICU transform string。
- ***reverse***: 如有需要，是否返回反转过的变换。
在上述参数中，重点也是难点的是 ***transform*** 参数，这个参数是一个CFStringRef类型的参数；来到头文件，我们会发现，这个参数的值有很多
![](http://upload-images.jianshu.io/upload_images/1196725-4fa80777c6149d5b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
transform
我们来粗略的介绍下上面这些值的作用，如下图：
![](http://upload-images.jianshu.io/upload_images/1196725-a15e22f4e8937bb4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
transform 参数作用
从上图我们可以看出，利用这些值，我们输入左边(Input)的语言，那么结果为右边(Output)的;
上面有些还没有介绍，就不再一一介绍了，这里我再重点介绍一个我们上面用到的`kCFStringTransformStripCombiningMarks`，他的功能就是去掉原有字符串中的音标或者类似于音标的符号。
本文主要参考：[http://nshipster.cn/cfstringtransform/](http://nshipster.cn/cfstringtransform/)
文／tanyufeng（简书作者）
原文链接：http://www.jianshu.com/p/a28be7b7f1d3
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
