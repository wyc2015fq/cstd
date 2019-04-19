# iOS 枚举的巧用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月26日 10:34:38[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1347
## 前言
> 
在之前的一篇文章中简单的提到了这个问题, 但是自己写的不详细, 并且自己深入了解的也不是特别多, 在开发中也没怎么用到,所以经过阅读者的反馈对这个问题很是疑惑! 本篇文章会分析之前的不足之处, 如果有地方不对还请帮忙纠正!
相关文章: [iOS开发中你是否遇到这些经验问题(二)](http://www.jianshu.com/p/a924b4a17686)
## 1.使用层面的理解
在这里首先讲如何简单的使用, 仅仅是使用层面(有理解错误的地方帮忙纠正), 然后我们在去理解位运算符! 在下面的图中我们可以看见枚举值中有`<<`(位运算符:左移):
![](http://upload-images.jianshu.io/upload_images/2353624-476df8d2cb3f2524.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
如果我们在枚举值中看见`<<`那我们就可以通过`|`(位运算符:或)进行`组合使用`如下代码为例:
```
//随便添加一个UITextField
 UITextField *field = [UITextField new];
 //Begin,Changed,DidEnd都能触发UITextField的事件
 [field addTarget:self action:@selector(textFieldDidChanged) forControlEvents: UIControlEventEditingDidBegin |
                  UIControlEventValueChanged |
                  UIControlEventEditingDidEnd
     ];
 [self.view addSubview:field];
```
如下图枚举值中没有`<<`,这就是普通的`NSInteger`类型的枚举, 所以不能组合使用:
![](http://upload-images.jianshu.io/upload_images/2353624-87cc026229c4ccc0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
那苹果官方是怎么知道我们多个条件组合使用了呢? 答案是通过`&`(位运算符:与)进行判断的:
```
//controlEvents是组合使用后的一个值
 NSUInteger controlEvents = UIControlEventEditingDidBegin | UIControlEventValueChanged | UIControlEventEditingDidEnd;
    /**
    //通过 & 来判断是否包含:
    UIControlEventEditingDidBegin,
    UIControlEventValueChanged,
    UIControlEventEditingDidEnd
     */
    if (controlEvents & UIControlEventEditingDidBegin) {
        NSLog(@"UIControlEventEditingDidBegin");
    }else if (controlEvents & UIControlEventValueChanged) {
        NSLog(@"UIControlEventValueChanged");
    }else if (controlEvents & UIControlEventEditingDidEnd) {
        NSLog(@"UIControlEventEditingDidEnd");
    }
```
那么我们接下来看看使用过程中牵扯到的位运算符, 我们会在下面举个例子!
## 2.理解位运算符
首先我们有一个枚举, 下面代码2种写法我们暂时先不用管,等位运算符讲完我们会讨论`枚举的宏使用`:
```cpp
```cpp
//typedef NS_OPTIONS(NSInteger, myTests) {
//    nameA = 1 << 0,
//    nameB = 1 << 1,
//    nameC = 1 << 2,
//    nameD = 1 << 3,
//};
typedef enum {
    nameA = 1 << 0,
    nameB = 1 << 1,
    nameC = 1 << 2,
    nameD = 1 << 3,
}myTests;
/**
 nameA = 1 << 0 :值为1(2的0次方)
 nameB = 1 << 1 :值为2(2的1次方)
 nameC = 1 << 2 :值为4(2的2次方)
 nameD = 1 << 3 :值为8(2的3次方)
 */
```
```
通过`&`进行判断我们来看看输出结果如下图:
![](http://upload-images.jianshu.io/upload_images/2353624-227e20bc09059900.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
我们得到`NSInteger value = nameA | nameB;`的组合的值, 判断结果是:`1`是`nameA`的值,
`2`是`nameB`的值, `nameC`与`nameD`没有组合使用所以值为`0`,最后我们知道如果`value & nameC`为`0`说明`value`不包含`nameC` 相反则包含!
还有一点就是`value & nameA`就是`nameA`的值为`1`, `value & nameB`就是`nameB`的值为`2`
- `<<`(左移):`a << b`就表示把`a`转为二进制后左移`b`位（在后面添`b`个`0`）
- `|`(或):只要有一个为`1`, 结果就是`1`
- `&`(或):只要有二个为`1`, 结果就是`1`
我们已经知道`nameA = 1`, `nameB = 2`, `nameC = 4`, 
```
nameD = 8
```
下面来通过二进制来解释:
```cpp
```cpp
NSInteger value = nameA | nameB | nameC | nameD;
     转成二进制:
     nameA: 0 0 0 1
       |
     nameB: 0 0 1 0
       |
     nameC: 0 1 0 0
       |
     nameD: 1 0 0 0
    ----------------
     value: 1 1 1 1
     上面是使用 | 得出value的值为1111(|的意思是有一个为1结果就为1)
     下面是使用 & 判断输出的值(&的意思就是有二个为1结果才为1)
      value: 1 1 1 1         value: 1 1 1 1
        &                      &
      nameA: 0 0 0 1         nameB: 0 0 1 0
     ----------------       ----------------
      结果值: 0 0 0 1         结果值: 0 0 1 0
      我就写2个例子:0001就是nameA的值, 0010就是nameB的值
```
```
相信大家已经明白其中的道理了, 接下来我们来看看枚举的宏, 为了更好阅读也可以看下面的截图:
![](http://upload-images.jianshu.io/upload_images/2353624-d269cf4285dd884a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 3.枚举的宏(`NS_ENUM`与`NS_OPTIONS`)
`NS_ENUM`和`NS_OPTIONS`宏提供了一个简洁、定义枚举和C语言选项的简单方法。
The `NS_ENUM` and `NS_OPTIONS` macros provide a concise, simple way of defining enumerations and options in C-based languages. These macros improve code completion in Xcode and explicitly specify the type and size of your enumerations
 and options. Additionally, this syntax declares enums in a way that is evaluated correctly by older compilers, and by newer ones that can interpret the underlying type information.
这是最初的使用方法:
```
enum {
        UITableViewCellStyleDefault,
        UITableViewCellStyleValue1,
        UITableViewCellStyleValue2,
        UITableViewCellStyleSubtitle
};
typedef NSInteger UITableViewCellStyle;
--------------------------------------------------
enum {
        UIViewAutoresizingNone                 = 0,
        UIViewAutoresizingFlexibleLeftMargin   = 1 << 0,
        UIViewAutoresizingFlexibleWidth        = 1 << 1,
        UIViewAutoresizingFlexibleRightMargin  = 1 << 2,
        UIViewAutoresizingFlexibleTopMargin    = 1 << 3,
        UIViewAutoresizingFlexibleHeight       = 1 << 4,
        UIViewAutoresizingFlexibleBottomMargin = 1 << 5
};
typedef NSUInteger UIViewAutoresizing;
```
通过使用枚举的宏:
`NS_ENUM`:是用来声明一般的`NSInteger`(下面代码使用`NSInteger`)类型的枚举
Use the NS_ENUM macro to define enumerations, a set of values that are mutually exclusive.
`NS_OPTIONS`:是用来声明位掩码(bitmasked)
Use the NS_OPTIONS macro to define options, a set of bitmasked values that may be combined together.
```
//NS_ENUM
typedef NS_ENUM(NSInteger, UITableViewCellStyle) {
        UITableViewCellStyleDefault,
        UITableViewCellStyleValue1,
        UITableViewCellStyleValue2,
        UITableViewCellStyleSubtitle
};
--------------------------------------------------
//NS_OPTIONS
typedef NS_OPTIONS(NSUInteger, UIViewAutoresizing) {
        UIViewAutoresizingNone                 = 0,
        UIViewAutoresizingFlexibleLeftMargin   = 1 << 0,
        UIViewAutoresizingFlexibleWidth        = 1 << 1,
        UIViewAutoresizingFlexibleRightMargin  = 1 << 2,
        UIViewAutoresizingFlexibleTopMargin    = 1 << 3,
        UIViewAutoresizingFlexibleHeight       = 1 << 4,
        UIViewAutoresizingFlexibleBottomMargin = 1 << 5
};
```
`NS_OPTIONS` 与 `NS_ENUM` 和 `enum` 是有什么区别呢? 
1.通过上面介绍我们可以看出`enum`可以声明一般类型和位掩码(bitmasked)类型
2.`NS_ENUM`声明一般类型, `NS_OPTIONS`声明掩码(bitmasked)类型
3.那么问题又来了, 直接用`enum`不就可以了? 答案不是这样的, 苹果建议我们在OC中使用`NS_ENUM`与`NS_OPTIONS`, 为什么呢? 因为他们除了推断出不同类型的枚举,再就是当编译`Objective-C++`模式，它们产生的代码是不同的, 就是因为不同所以混编的时候使用`enum`会报错!
大家可以看看[stackoverflow中的问题解答!](http://stackoverflow.com/questions/18962925/what-is-the-difference-between-ns-enum-and-ns-options)不知道我的理解是否有错误, 如果有错误还希望帮忙纠正!
文／判若两人丶（简书作者）
原文链接：http://www.jianshu.com/p/97e582fe89f3
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
