# MVVM架构 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月18日 21:12:39[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：10094
### 概要
MVC架构，Model-View-Controller，如图一所示为一个典型的MVC设置。
![](http://upload-images.jianshu.io/upload_images/1322408-449b1cf134054eda.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图一：mvc
- Model呈现数据
- View呈现用户界面
- Controller调节两者之间的交互。从Model取数据，显示在View中。
典型的MVC应用里，许多逻辑被放在View Controller中，他们中一些确实属于View Controller，但更多的是表现逻辑，即将Model中数据转换为View可以呈现的内容的事情。例如将JSON包里的某个NSDate转换为特定格式的NSString。这也导致了MVC被人称作Massive-View-Controller（重量级视图控制器）。
通常Controller中应该只放如下代码：
- 初始化时构造相应的View和Model
- 监听Model层的事件，将Model层的数据传递到View层
- 监听View层的时间，将View层的事件传递到Model层
仅此而已，除此之外的任何逻辑都不应该放到Controller中。因此这也就有了MVVM
### MVVM
图二所示为MVVM设置：MVVM其实就是MVC的增强版。我们正式连接了View 和View Controller，并将表示逻辑从Controller中移出，放到了一个新的对象里，即View Model中。
![](http://upload-images.jianshu.io/upload_images/1322408-f7071f2b570e640c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图二：MVVM.png
这样做可带来如下的益处：
- 减少View Controller的复杂性，使得表示逻辑易于测试。
- 兼容MVC模式
- MVVM 配合一个绑定机制效果最好
### 举例
下面是一个看一个简单的 Person Model 以及相应的 View Controller。
```
@interface Person : NSObject
- (instancetype)initwithSalutation:(NSString *)salutation firstName:(NSString *)firstName lastName:(NSString *)lastName birthdate:(NSDate *)birthdate;
@property (nonatomic, readonly) NSString *salutation;
@property (nonatomic, readonly) NSString *firstName;
@property (nonatomic, readonly) NSString *lastName;
@property (nonatomic, readonly) NSDate *birthdate;
@end
```
现在假设有一个 PersonViewController，在 `viewDidLoad`里，只需要基于它的 model属性设置一些Label即可。
```
- (void)viewDidLoad { 
    [super viewDidLoad]; 
    if (self.model.salutation.length > 0) { 
        self.nameLabel.text = [NSString stringWithFormat:@"%@ %@ %@",self.model.salutation,self.model.firstName, self.model.lastName];
    } else { 
        self.nameLabel.text = [NSString stringWithFormat:@"%@ %@", self.model.firstName,self.model.lastName];
    } 
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init]; 
    [dateFormatter setDateFormat:@"EEEE MMMM d, yyyy"];
    self.birthdateLabel.text = [dateFormatter stringFromDate:model.birthdate];
}
```
现在来看怎样通过一个ViewModel来增强它。
```
//ViewModel.h
@interface PersonViewModel : NSObject
- (instancetype)initWithPerson:(Person *)person;
@property (nonatomic, readonly) Person *person;
@property (nonatomic, readonly) NSString *nameText;
@property (nonatomic, readonly) NSString *birthdateText;
@end
```
ViewModel实现如下
```
//ViewModel.m
@implementation PersonViewModel
- (instancetype)initWithPerson:(Person *)person { 
    self = [super init]; 
    if (!self) return nil; 
    _person = person; 
    if (person.salutation.length > 0) { 
    _nameText = [NSString stringWithFormat:@"%@ %@ %@", self.person.salutation, self.person.firstName, self.person.lastName]; 
    } else { 
        _nameText = [NSString stringWithFormat:@"%@ %@", self.person.firstName, self.person.lastName];
    }
     NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"EEEE MMMM d, yyyy"]; 
    _birthdateText = [dateFormatter stringFromDate:person.birthdate]; 
    return self;
}
@end
```
现在我们已将`viewDidLoad`中的表示逻辑放入我们的 View Model 里了。此时，我们新的 viewDidLoad
就会非常轻量：
```
- (void)viewDidLoad { [super viewDidLoad]; 
    self.nameLabel.text = self.viewModel.nameText; 
    self.birthdateLabel.text = self.viewModel.birthdateText;
}
```
### MVVM作用与问题
MVVM在实际使用时也有一定的问题，主要体现在两点：
- 数据绑定使得 Bug 很难被调试。你看到界面异常了，有可能是你 View 的代码有 Bug，也可能是 Model 的代码有问题。数据绑定使得一个位置的 Bug 被快速传递到别的位置，要定位原始出问题的地方就变得不那么容易了。
- 对于过大的项目，数据绑定需要花费更多的内存。
### ReactiveCocoa
MVVM引出了一个ReactiveCocoa，ReactiveCocoa作用如图三所示：
![](http://upload-images.jianshu.io/upload_images/1322408-1604d5274f4bec49.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图三：MVVM ReactiveCocoa.png
如果想要深入了解ReactiveCocoa，可以看下[ReactiveCocoa](http://southpeak.github.io/blog/2014/08/08/mvvmzhi-nan-yi-:flickrsou-suo-shi-li/)这篇文章。
*备注：*本文为读书笔记，主要参考了如下几篇文章：
- [MVVM 介绍](https://objccn.io/issue-13-1/)
- [被误解的 MVC 和被神化的 MVVM](http://blog.devtang.com/2015/11/02/mvc-and-mvvm/)
文／NapoleonY（简书作者）
原文链接：http://www.jianshu.com/p/6e5dda137c7e
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
