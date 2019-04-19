# 自动布局 Auto Layout (原理篇) - 简书
# 自动布局 Auto Layout (原理篇)
# 目录
###### 0、前言
###### 一、Auto Layout前世今生
###### 二、Auto Layout基础知识
- 1.Auto Layout本质
- 2.Auto Layout基本原理
NSLayoutAttribute布局属性
约束关系
约束优先级
约束的安装与移除
###### 三、Auto Layout多种使用方式
- 1.NSLayoutConstraint
NSLayoutConstraint对象创建约束
NSLayoutConstraint优缺点讨论
示例代码
- 2.VFL
VFL创建约束
VFL优缺点讨论
示例代码
- 3.Interface Builder
- 4.NSLayoutAnchor
NSLayoutAnchor创建约束
NSLayoutAnchor优缺点讨论
示例代码
- 5.Masonry
示例代码
- 6.UIStackView
UIStackView布局方案特点
UIStackView特殊性
示例代码
###### 四、Auto Layout关键知识
- 1.Auto Layout布局原则
坚持一致的布局方式
创建充分的、可满足的约束
- 2.translatesAutoresizingMaskIntoConstraints
- 3.alignmentRect对齐矩形
alignmentRect简介
alignmentRect应用场景
alignmentRect可视化
- 4.内在内容大小IntrinsicContentSize
IntrinsicContentSize介绍
IntrinsicContentSize原理分析
IntrinsicContentSize应用
内在内容尺寸IntrinsicContentSize与自适应尺寸FittingSize
- 5.Auto Layout与国际化
布局方向适配
布局测试
###### 五、Auto Layout布局周期
- 1.Auto Layout布局机制
- 2.Auto Layout布局流程
Constraints Change
Deferred Layout Pass
- 3.Auto Layout布局流程总结
###### 六、References
# 0、前言
- 日常开发中，UI搭建、调试会占用我们大部分的时间，以至于移动端开发经常会被调侃为搭界面的。提高UI布局技术可以提高开发效率，把更多的时间放在优化、逻辑方面，而不是被界面业务绑死。本文是笔者近期学习的相关布局基础，没有很高深的技术，都是讲述原理性、基础性的东西。示例代码详见[DEMO](https://link.jianshu.com?t=https://github.com/Lin-Bing/SampleCode.git)，欢迎留言或者邮件（[mailtolinbing@163.com](https://link.jianshu.com?t=mailto:mailtolinbing@163.com)）勘误、交流。
# 一、Auto Layout前世今生
- Auto Layout是苹果公司在iOS6发布的界面布局技术，并随着iOS SDK的迭代逐步完善了各种布局API、提供多种使用Auto Layout的布局方式。实际上Auto Layout算法本身并非有Apple发明，Auto Layout源于[Cassary](https://link.jianshu.com?t=http://constraints.cs.washington.edu/cassowary/)约束解析工具包。该算法由Alan Borning、Kim Marriott、Peter Stuckey、Yi Xiao于1997年发布，而后被多门流行编程语言采用，Objective-C是其中之一。该算法的主要思想是：将基于约束系统的布局规则（本质上是表示视图布局关系的线性方程组）转化为表示规则的视图几何参数。
# 二、Auto Layout基础知识
### 1.Auto Layout本质
- Auto Layout本质就是一个线性方程解析Engine。基于Auto Layout的布局，不在需要像frame时代一样，关注视图尺寸、位置的常数，转而关注视图之间关系，描述一个表示视图间布局关系的约束集合，由Engine解析出最终数值。
- 一个约束对象NSLayoutConstraint，本质上是表示两个视图之间（当表示尺寸时只表示视图本身）布局关系的一个线性方程，该方程可以是线性等式、也可以是线性不等式。
- 多个约束对象组成是一个约束集合，本质上是表示某个界面上多个视图之间布局关系的线性方程组。方程组中的多个线性方程，以数字标识的优先级进行排序（UILayoutPriority，本质上是浮点型float）。
- Auto Layout Engine根据按照线性方程的优先级从高到底对线性方程组进行解析，求得方程组的解。
- 当设置的约束欠缺，即存在约束歧义，线性方程组有多个解，而不是唯一解。这便是约束错误的一种：约束不充分，可能导致视图丢失，视图错位。
- 当设置的约束过多，存在多个优先级相同的描述同一个关系的线性方程，并且约束产生的效果不同（例如 View1.left = View2.right + 10 ; View1.left = View2.right + 20，优先级都为1000），线程方程组无解。这是约束错误的另一种：约束不可满足，产生约束约束冲突，控制台会Log错误日志，同样可能造成布局错误。
### 2.Auto Layout基本原理
![](https://upload-images.jianshu.io/upload_images/696463-66769da1d41fafac.png)
图片出处 ：苹果官方文档 - Auto Layout Guide
- 正如上文提及，一个约束本质上就是一个表示视图布局关系的线性方程。一个完整的约束方程式如图所示。图示表示的布局关系是：RedView的左边距离BlueView的右边8个点。
- Item1、Item2：一般是UIView，表示该约束关系对应的两个视图，当约束等式表示尺寸时，其中一个Item为nil。
- Attribute1、Attribute2：NSLayoutAttribute类型，表示约束属性。当约束等式表示尺寸时，其中一个Attribute为NSLayoutAttributeNotAnAttribute，表示占位，无任何意义。
- Relationship：NSLayoutRelation类型，表示约束关系，可以是=、>=、<=。
- Multiplier：CGFloat类型，表示倍数关系，一般用于尺寸（eg：Item1的宽度为Item2的两倍，则Multiplier为2.0）
- Constant：CGFloat类型，表示常数。
##### NSLayoutAttribute布局属性
|布局属性|表示意义|
|----|----|
|NSLayoutAttributeWidth、NSLayoutAttributeHeight|表示视图的尺寸：宽、高|
|NSLayoutAttributeLeft、NSLayoutAttributeRight|表示视图的X轴方向的位置：左、右|
|NSLayoutAttributeLeading、NSLayoutAttributeTrailing|表示视图的X轴方向的位置：前、后|
|NSLayoutAttributeTop、 NSLayoutAttributeBottom|表示视图Y轴方向的位置：顶、底|
|NSLayoutAttributeBaseline|表示视图Y轴方向的位置：底部基准线|
|NSLayoutAttributeCenterX、NSLayoutAttributeCenterY|表示视图的中心点：视图在X轴的中心点、视图在Y轴的中心点|
> 
注意点
- 
只有同类型的约束才能互相做约束
表示尺寸的约束width/height只能与其他视图的width/height做约束,或者与非负常数做约束；
表示Y轴方向的约束属性（top、bottom、baseLine、CenterY）只能与Y轴方向的约束属性做约束；
表示X轴方向的约束属性只能与表示X轴的约束属性做约束，且leading/trailing不可以跟left/right做约束。
- 
leading表示前边、trailing表示后边，在阅读习惯从左到右的语言中，leading相当于left、trailing相当于right。在从右到左的语言中，leading相当于right、trailing相当于left。
- 
baseLine指视图的文本内容底部，该属性只对有文本的控件类型有效（UILabel、UIButton…），并且只有当控件赋值了文本，该约束才能正确布局。文本控件的文字顶部与底部与控件本身会有间隙，当要实现文本底部对齐，可使用该约束属性。
- 
Item1、Item2位置问题
- 从数学的角度，线性方程式两边的Item1、Item2是可以调换位置的。eg：View右边距离父视图superView右边10pt，可以表示为View.right = superView.right - 10 ； 也可以表示为superView.right = View.right + 10。可以保持视图顺序，使用负数；也可以保持数值为正数，调换视图顺序。
##### 约束关系
- Auto Layout提供三种约束关系：>=、=、<=，分别对应NSLayoutRelationLessThanOrEqual、NSLayoutRelationEqual、NSLayoutRelationGreaterThanOrEqual。即线性方程不一定是等式，也可以是不等式。
```
// View的高度<=100
NSLayoutConstraint *height = [NSLayoutConstraint constraintWithItem:greenView attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationLessThanOrEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1.0 constant:100];
```
##### 约束优先级
- 无论是我们创建的约束，还是系统创建的约束(IntrinsicContentSize相关的约束由系创建，下文会涉及)，都必须指定一个约束优先级UILayoutPriority。默认创建出来的约束优先级为UILayoutPriorityRequired（1000），称为必需约束；其他优先级小于1000的约束称为可选约束。Auto Layout Engine进行约束解析时，尝试着按优先级从高到低满足约束集合中的每一个约束，如果无法满足某个可选约束，则跳过；当优先级不同的两个约束描述的是同一个布局关系，Auto Layout会跳过优先级较低的约束。
> 
Apple官方文档表示：可选约束因无法满足被跳过时，仍旧可能影响布局。当约束冲突时，Auto Layout会选择相对接近的解，选择打破某些约束。在这个选择过程中，被跳过可选约束同样能影响选择最终结果。
##### 约束的安装与移除
- 1.使用NSLayoutConstraint创建一个约束对象，必须把约束添加到对应的位置，Apple规定约束必须添加到该约束相关的视图所在的视图树的第一个公共祖先(第一个公共superView)，以下通过几个图示说明
- 约束表示视图本身尺寸（width/height），则直接添加到该视图本身；约束表示两个视图的布局关系，则添加多着两个视图所在的视图树的第一个公共祖先；
- 约束移除，使用removeConstraint/removeConstraints移除约束；当一个视图调用removeFromSuperView，与该视图相关的全部约束都会自动移除。
```
// 约束安装示例
UIView *view1 = [[UIView alloc] init];
[self.view addSubview:view1];    
view1.translatesAutoresizingMaskIntoConstraints = NO;
NSLayoutConstraint *width = [NSLayoutConstraint constraintWithItem:view1 attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1.0 constant:100];
NSLayoutConstraint *height = [NSLayoutConstraint constraintWithItem:view1 attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1.0 constant:100];
[self.view addConstraints:@[left, top]];
```
![](https://upload-images.jianshu.io/upload_images/696463-9d36e1efbf50e3f9.png)
图片出处 ： WWDC2012 - Introduction to Auto Layout for iOS and OS X
![](https://upload-images.jianshu.io/upload_images/696463-578c5d74f63197ab.png)
图片出处 ： WWDC2012 - Introduction to Auto Layout for iOS and OS X
![](https://upload-images.jianshu.io/upload_images/696463-217a5e81c4b80b64.png)
图片出处 ： WWDC2012 - Introduction to Auto Layout for iOS and OS X
- 
2.使用Interface Builder方式进行布局，在xib、Storyboard中拖线实现布局的，约束会自动添加到对应的位置，无需考虑约束安装问题。
- 
3.iOS8+，Auto Layout推出新的接口。NSLayoutConstraint多了一个active属性，用于激活、失效一个约束。不需要再考虑约束安装位置。原本用于添加、移除约束的接口addConstraint/addConstraints、removeConstraint/removeConstraints，接口文档表示在后续的版本升级将会过期，建议避免使用，转而使用NSLayoutConstraint的active/activateConstraints、deactivateConstraints。
> 
笔者看了一下Masonry源码，作者在安装约束的install方法中做了版本适配，判断NSLayoutConstraint是否响应active方法。如果响应，则直接使用active方法安装、移除约束；否则使用传统方式，获取约束等式中两个view第一个公共祖先，找到约束应该添加的位置。
```
- (void)install {
        
    if (self.hasBeenInstalled) {
        return;
    }
    
    // 1.响应active方法，直接激活
    if ([self supportsActiveProperty] && self.layoutConstraint) {
        self.layoutConstraint.active = YES;
        [self.firstViewAttribute.view.mas_installedConstraints addObject:self];
        return;
    }
    
    // 2.否则，使用传统方式，把约束添加到对应位置
    // 利用各个元素，构造一个完整的约束对象 MASLayoutConstraint
    MAS_VIEW *firstLayoutItem = self.firstViewAttribute.item;
    NSLayoutAttribute firstLayoutAttribute = self.firstViewAttribute.layoutAttribute;
    MAS_VIEW *secondLayoutItem = self.secondViewAttribute.item;
    NSLayoutAttribute secondLayoutAttribute = self.secondViewAttribute.layoutAttribute;
    // 兼容省略写法，例如 make.left.equalTo(@10)，firstViewAttribute不是尺寸，却没有secondViewAttribute，则默认secondViewAttribute的secondLayoutItem是父view、secondLayoutAttribute与firstLayoutAttribute一致
    if (!self.firstViewAttribute.isSizeAttribute && !self.secondViewAttribute) {
        secondLayoutItem = self.firstViewAttribute.view.superview;
        secondLayoutAttribute = firstLayoutAttribute;
    }
    
    MASLayoutConstraint *layoutConstraint
        = [MASLayoutConstraint constraintWithItem:firstLayoutItem
                                        attribute:firstLayoutAttribute
                                        relatedBy:self.layoutRelation
                                           toItem:secondLayoutItem
                                        attribute:secondLayoutAttribute
                                       multiplier:self.layoutMultiplier
                                         constant:self.layoutConstant];
    
    layoutConstraint.priority = self.layoutPriority;
    layoutConstraint.mas_key = self.mas_key;
    
    // 获取约束安装位置installedView ：firstViewAttribute.view 和secondViewAttribute.view的最近公共祖先
    if (self.secondViewAttribute.view) {
        MAS_VIEW *closestCommonSuperview = [self.firstViewAttribute.view mas_closestCommonSuperview:self.secondViewAttribute.view];
        NSAssert(closestCommonSuperview,
                 @"couldn't find a common superview for %@ and %@",
                 self.firstViewAttribute.view, self.secondViewAttribute.view);
        self.installedView = closestCommonSuperview;
    } else if (self.firstViewAttribute.isSizeAttribute) {
        self.installedView = self.firstViewAttribute.view;
    } else {
        self.installedView = self.firstViewAttribute.view.superview;
    }
    
    //根据标识位，执行约束更新、安装；把安装完毕的约束存储到view的mas_installedConstraints集合中
    MASLayoutConstraint *existingConstraint = nil;
    if (self.updateExisting) {
        existingConstraint = [self layoutConstraintSimilarTo:layoutConstraint];
    }
    
    if (existingConstraint)      // 更新操作：更新常量即可
    {
        // just update the constant
        existingConstraint.constant = layoutConstraint.constant;
        self.layoutConstraint = existingConstraint;
    }
    else // 安装操作
    {
        [self.installedView addConstraint:layoutConstraint];
        self.layoutConstraint = layoutConstraint;
        [firstLayoutItem.mas_installedConstraints addObject:self];
    }
}
```
# 三、Auto Layout多种使用方式
### 1.NSLayoutConstraint
##### NSLayoutConstraint对象创建约束
- 原生的NSLayoutConstraint进行布局，使用NSLayoutConstraint提供的约束对象创建接口，传入对应的参数即可，一个约束对象对应一个布局关系。具体步骤如下
- 设置View的translatesAutoresizingMaskIntoConstraints属性为NO
- 根据约束方程式，创建约束对象；
- 把约束添加到对应位置（iOS8+直接通过active激活约束）；
- NSLayoutConstraint接口设计，只有constant常量是readwrite，其他都是readonly属性，约束对象在创建的时候传入约束等式的各个参数，之后就只能修改约束常量（做动画时经常这么用）。
##### NSLayoutConstraint优缺点讨论
- 使用该方式进行布局，最明显的特点是代码量冗余，欠优雅。
- 约束更新、删除时，执行起来也不方便，需要实现用指针记录约束对象、或者通过匹配找到对应的约束。
##### 示例代码
![](https://upload-images.jianshu.io/upload_images/696463-822c7606ed086f74.png)
Snip20170917_25.png
```
UIView *grayView = [[UIView alloc] init];
grayView.backgroundColor = [UIColor lightGrayColor];
[self.view addSubview:grayView];    
grayView.translatesAutoresizingMaskIntoConstraints = NO;
    
NSLayoutConstraint *left = [NSLayoutConstraint constraintWithItem:grayView attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeLeft multiplier:1.0 constant:50];
NSLayoutConstraint *top = [NSLayoutConstraint constraintWithItem:grayView attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeTop multiplier:1.0 constant:100];
NSLayoutConstraint *width = [NSLayoutConstraint constraintWithItem:grayView attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1.0 constant:100];
NSLayoutConstraint *height = [NSLayoutConstraint constraintWithItem:grayView attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1.0 constant:100];
        
[self.view addConstraints:@[left, top]];
[grayView addConstraints:@[width, height]];
//  iOS8+
//  left.active = YES;
//  top.active = YES;
//  width.active = YES;
//  height.active = YES;
```
### 2.VFL
##### VFL创建约束
- VFL，即Visual Format Language，可视化格式语言。这种布局方式，同样是使用NSLayoutConstraint类来创建约束。不同之处在于：上面演示的NSLayoutConstraint方式是基于线程方程式创建约束，一个约束对象表示一个约束关系。而VFL是使用字符串编码的方式创建约束，Auto Layout根据字符串创建对应的约束对象。VFL字符串中可以传入任意多个视图、可以表示任意多个布局关系，因此使用VFL创建的约束时一次创建一个约束集合，返回一个装着NSLayoutConstraint对象的数组。
##### VFL优缺点讨论
- VFL使用简短的字符串指定布局关系，对一个布局字符串中传入的视图个数、布局关系个数不做限制，约束代码简洁。
- 某些约束关系无法使用VFL约束规则来描述，例如尺寸比例（A的宽度是B的宽度的2倍）。
- 字符串编码的固有缺陷：Xcode无法在编译期间检查约束，只能在运行时生效，安全性低。Xcode中把字符串颜色设置为警告效果的红色，应该是表示这部分代码编译器无能为力，程序猿自求多福。
- 企业开发一般不会使用VFL，此处仅做简单介绍，详见 [Apple官方教程VFL](https://link.jianshu.com?t=https://developer.apple.com/library/content/documentation/UserExperience/Conceptual/AutolayoutPG/VisualFormatLanguage.html#//apple_ref/doc/uid/TP40010853-CH27-SW1)。Auto Layout 约束冲突时Log信息一般是以VFL语言展示，最好要能读懂。
##### 示例代码
![](https://upload-images.jianshu.io/upload_images/696463-194de82fb3d5f7ef.png)
Snip20170917_19.png
```
UIView *view1 = [[UIView alloc] init];
view1.translatesAutoresizingMaskIntoConstraints = NO;
view1.backgroundColor = [UIColor redColor];
[self.view addSubview:view1];
    
UIView *view2 = [[UIView alloc] init];
view2.translatesAutoresizingMaskIntoConstraints = NO;
view2.backgroundColor = [UIColor blueColor];
[self.view addSubview:view2];
/*
     水平方向约束：
     view1.left = superView.left+50；
     view1.width = 100；
     view1.right+50 = view2.right；
     view2.right+50 = superVew.right；
     
     view1.top = view2.top;
     view1.bottom = view2.bottom;
     
     竖直方向
     view1.top = superView.top+100；
     view1.height = 100;
*/
NSArray<NSLayoutConstraint *> *horizontalConstraint = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|-50-[view1(100)]-50-[view2]-50-|" options:(NSLayoutFormatAlignAllTop | NSLayoutFormatAlignAllBottom) metrics:nil views:NSDictionaryOfVariableBindings(view1, view2)];
[self.view addConstraints:horizontalConstraint];
    
NSArray<NSLayoutConstraint *> *verticalConstraint = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|-100-[view1(100)]" options:0 metrics:nil views:NSDictionaryOfVariableBindings(view1, view2)];
[self.view addConstraints:verticalConstraint];
```
### 3.Interface Builder
- Apple建议开发中使用Interface Builder进行布局，使用该方式进行布局，Xcode自带约束冲突、约束歧义检查。这种布局方式开发速度相对较快，遇到比较复杂的布局也可以结合代码进行。笔者日常开发中是纯代码流，就不班门弄斧。详见 [Apple官方教程Interface Builder](https://link.jianshu.com?t=https://developer.apple.com/library/content/documentation/UserExperience/Conceptual/AutolayoutPG/WorkingwithConstraintsinInterfaceBuidler.html#//apple_ref/doc/uid/TP40010853-CH10-SW1)
### 4.NSLayoutAnchor
##### NSLayoutAnchor创建约束
- [NSLayoutAnchor](https://link.jianshu.com?t=https://developer.apple.com/documentation/uikit/nslayoutanchor) 布局锚点，提供一种比NSLayoutConstraint更方便、安全方案。NSLayoutAnchor作为UIView的属性，它与NSLayoutConstraint的布局属性的枚举类型NSLayoutAttribute一一对应，并细分为三个子类，分别是：
|布局锚点类型|对应的子类|布局属性|
|----|----|----|
|X轴方向|NSLayoutXAxisAnchor|leadingAnchor、trailingAnchor、leftAnchor、rightAnchor、centerXAnchor|
|Y轴方向|NSLayoutYAxisAnchor|topAnchor、bottomAnchor、centerYAnchor、firstBaselineAnchor、lastBaselineAnchor|
|尺寸|NSLayoutDimension|widthAnchor、heightAnchor|
##### NSLayoutAnchor优缺点讨论
- 通过把NSLayoutAnchor子类化，布局类、布局属性被细化，Xcode在编译期间进行布局检查，只有相同类型的布局属性才能互相约束，否则编译警告，优化了NSLayoutConstraint运行时才检查的缺陷，及早更正错误。注意leadingAnchor/trailingAnchor不可以与leftAnchor/rightAnchor匹配，NSLayoutAnchor没有进行区分，还是需要在runtime执行检查。
- 创建约束接口更加安全，只有尺寸相关的布局锚点才需要设置multiplier参数（其他都是默认1.0），即NSLayoutDimension类型的约束创建接口才提供multiplier参数定制，避免错误使用。
- 约束接口也更加简洁，一行代码即可搞定一个约束；无需考虑约束需要添加到哪里，直接使用active属性激活。布局效率已经接近Masonry。
- NSLayoutAnchor不足之处是仅支持iOS9+，不兼容更早的版本。NSLayoutAnchor布局接口相对简洁，苹果爸爸帮我们多做了一些脏活累活，Masonry以后应该会是一个更轻量级的库。
##### 示例代码
![](https://upload-images.jianshu.io/upload_images/696463-385ebf0d6adf7ece.png)
Snip20170920_68.png
```
UIView *yellow = [[UIView alloc] init];
    yellow.translatesAutoresizingMaskIntoConstraints = NO;
    yellow.backgroundColor = [UIColor yellowColor];
    [self.view addSubview:yellow];
    
    UIView *green = [[UIView alloc] init];
    green.translatesAutoresizingMaskIntoConstraints = NO;
    green.backgroundColor = [UIColor greenColor];
    [yellow addSubview:green];
    
    UIView *red = [[UIView alloc] init];
    red.translatesAutoresizingMaskIntoConstraints = NO;
    red.backgroundColor = [UIColor redColor];
    [yellow addSubview:red];
    
    CGFloat margin = 20;
    [yellow.leadingAnchor constraintEqualToAnchor:self.view.leadingAnchor constant:margin].active = YES;
    [yellow.trailingAnchor constraintEqualToAnchor:self.view.trailingAnchor constant:-margin].active = YES;
    [yellow.topAnchor constraintEqualToAnchor:self.view.topAnchor constant:100].active = YES;
    [yellow.bottomAnchor constraintEqualToAnchor:self.view.bottomAnchor constant:-margin].active = YES;
    
    [green.leadingAnchor constraintEqualToAnchor:yellow.leadingAnchor constant:margin].active = YES;
    [green.trailingAnchor constraintEqualToAnchor:yellow.trailingAnchor constant:-margin].active = YES;
    [green.topAnchor constraintEqualToAnchor:yellow.topAnchor constant:margin].active = YES;
    [green.bottomAnchor constraintEqualToAnchor:red.topAnchor constant:-margin].active = YES;
    
    [red.leadingAnchor constraintEqualToAnchor:green.leadingAnchor].active = YES;
    [red.trailingAnchor constraintEqualToAnchor:green.trailingAnchor].active = YES;
    [red.bottomAnchor constraintEqualToAnchor:yellow.bottomAnchor constant:-margin].active = YES;
    [red.heightAnchor constraintEqualToAnchor:green.heightAnchor multiplier:2.0].active = YES;
```
### 5.Masonry
- [Masonry](https://link.jianshu.com?t=https://github.com/SnapKit/Masonry) ，对应的Swift版本是SnapKit，企业开发中绝大多数都会使用该库作为布局方案。该库接口设计优雅，只需要少量代码即可实现布局、布局更新。Masonry提供的Demo中对该库使用提供了详细的代码示例，上手相对容易。
##### 示例代码
```
// 实现NSLayoutAnchor相同的布局
    UIView *yellow = [[UIView alloc] init];
    yellow.backgroundColor = [UIColor yellowColor];
    [self.view addSubview:yellow];
    
    UIView *green = [[UIView alloc] init];
    green.backgroundColor = [UIColor greenColor];
    [yellow addSubview:green];
    
    UIView *red = [[UIView alloc] init];
    red.backgroundColor = [UIColor redColor];
    [yellow addSubview:red];
    
    CGFloat margin = 20;
    
    [yellow mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.equalTo(self.view).offset(margin);
        make.trailing.equalTo(self.view).offset(-margin);
        make.top.equalTo(self.view).offset(100);
        make.bottom.equalTo(self.view).offset(-margin);
    }];
    
    [green mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.equalTo(yellow).offset(margin);
        make.trailing.equalTo(yellow).offset(-margin);
        make.top.equalTo(yellow).offset(margin);
        make.bottom.equalTo(red.mas_top).offset(-margin);
    }];
    
    [red mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.equalTo(green);
        make.trailing.equalTo(green);
        make.bottom.equalTo(yellow).offset(-margin);
        make.height.equalTo(green).multipliedBy(2.0);
    }];
```
### 6.UIStackView
- [UIStackView](https://link.jianshu.com?t=https://developer.apple.com/documentation/uikit/uistackview)是iOS9+推出的一个专门用于布局的UI控件，尤其擅长多个控件成行、成列布局。UIStackView提供了一种脱离约束的布局方案，约束不需要手动创建。实际上底层也是通过添加约束实现布局，Xcode调试工具 Debug View Hierarchy可以看到，添加到UIStackView中的控件都自动设置了约束。
![](https://upload-images.jianshu.io/upload_images/696463-c26784c2d9dcd1f3.png)
Snip20170919_49.png
##### UIStackView布局方案特点
- 
UIStackView是iOS9推出的布局方案，理论上无法向下兼容iOS9以前的版本。百度团队维护的一个第三方库[FDStackView](https://link.jianshu.com?t=https://github.com/forkingdog/FDStackView)，可以做到无缝向下兼容iOS9，因此项目即使支持iOS8也可以直接使用UIStackView，等到放弃iOS8了再移除该库即可，没有任何代码侵入性。
- 
UIStackView不足之处是，复杂的布局方案UIStackView需要多层嵌套，还是需要结合手动设置约束的方式来实现复杂布局。UIStackView擅长多个控件成行、成列布局，Masonry也提供了专门用于布局一行、一列控件的接口。
##### UIStackView特殊性
- 
UIStackView继承自UIView，当时该控件只用于布局，不参与图层树的渲染。设置UIStackView的UI样式不起作用，例如圆角、背景色……
- 
添加到UIStackView的控件，设置hidden状态为YES的View依旧存在arrangedSubviews中，但是不会展示出来，也不影响其他控件的布局；Auto Layout中，View设置hidden为YES后不会展示，但约束依旧存在，影响其他控件的布局。
- 
UIView通过addArrangedSubview添加到arrangedSubviews中，也自动添加到视图树中；UIView通过removeArrangedSubview仅仅移出arrangedSubviews，不会从视图树中移除；UIView通过removeFromSuperview移出视图层级，则自动移出arrangedSubviews。
- 
UIView的hidden属性本来是不可动画属性（not animatable），在UIStackView中只要UIView添加到arrangedSubviews，它的hidden就变成了可动画属性，动画效果即：View加入/移出StackView的动画。
- 
使用UIStackView布局，通常需要设置具有内在尺寸Intrinsic Content Size视图的内容吸附、压缩阻力优先级（下文会涉及）。
##### 示例代码
- 使用UIStackView布局，步骤如下
- 最外层UIStackView与传统布局方案一样，手动设置约束；
- 创建UIStackView，设置布局参数（布局方向、间距、对齐方式…）；
- 把需要布局的控件添加到UIStackView中，由arrangedSubviews管理；
![](https://upload-images.jianshu.io/upload_images/696463-c6be6494ab847907.jpg)
15057846355533.jpg
```
UIStackView *stackView = [[UIStackView alloc] init];
    stackView.backgroundColor = [UIColor redColor];
    stackView.layer.cornerRadius = 50;
    stackView.layer.masksToBounds = YES;
    stackView.axis = UILayoutConstraintAxisVertical;
    stackView.distribution = UIStackViewDistributionFill;
    stackView.alignment = UIStackViewAlignmentFill;
    stackView.spacing = 8;
    
    self.stackView = stackView;
    [self.view addSubview:self.stackView];
    
    [stackView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(self.view).insets(UIEdgeInsetsMake(84, 10, 200, 10));
    }];
    
    UILabel *label = [[UILabel alloc] init];
    label.backgroundColor = [UIColor yellowColor];
    label.text = @"标题标题";
    label.textAlignment = NSTextAlignmentCenter;
    [self.stackView addArrangedSubview:label];
    
    UIImageView *imageView = [[UIImageView alloc] init];
    imageView.image = [UIImage imageNamed:@"Snip20170906_172"];
    [imageView setContentHuggingPriority:249 forAxis:UILayoutConstraintAxisVertical];
    [imageView setContentCompressionResistancePriority:749 forAxis:UILayoutConstraintAxisVertical];
    
    [self.stackView addArrangedSubview:imageView];
    
    UIButton *btn = [[UIButton alloc] init];
    btn.backgroundColor = [UIColor yellowColor];
    [btn setTitle:@"按钮按钮" forState:UIControlStateNormal];
    [btn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [self.stackView addArrangedSubview:btn];
```
- 关于UIStackView使用， [这篇博客](https://www.jianshu.com/p/213702004d0d) 给出了详细的纯代码方式布局教程，[苹果官方示例](https://link.jianshu.com?t=https://developer.apple.com/documentation/uikit/uistackview?language=objc) 也通俗易懂。
# 四、Auto Layout关键知识
### 1.Auto Layout布局原则
##### 坚持一致的布局方式
- 如上所示，Auto Layout布局方式有多种，不同开发者会采用不同的方式。纯代码布局一般会采用Masonry；也接触过不少同行采用Interface Builder方式。苹果的建议是选择一种合适的布局方式，并坚持一致性。
- 倍数关系multipliers，优先考虑整数，再考虑小数；
- 常数Constant，优先考虑正数，再考虑负数；
- 布局顺序一般是从左到右、从上到下。符合阅读习惯，代码可读性更强。
##### 创建充分的、可满足的约束
- 约束集合必须是充分的、可满足的。约束不充分（欠约束）、不可满足（约束冲突）会造成视图错位、视图丢失等。要创建充分的、可满足的约束需要遵循一定的布局原则。
- 每一个约束对象关联一至两个视图。当设置尺寸约束时，关联一个视图；当设置位置约束时，关联两个视图；
- 多个约束之间逻辑上必须互不冲突。（eg:一个视图不能同时满足width =100、width=200两个约束）当约束冲突时，Auto Layout会根据自身算法，选择打破某些约束直到约束集合逻辑上互不冲突，这就存在不确定性。
- 一个充分的布局在每个坐标轴上至少要有两个约束，即一个视图一共至少需要四个约束，否则视图的尺寸、位置确定。（eg：一个视图在X轴上要充分约束，可以是这几种情况其中之一：视图设置了left、right约束；视图设置了left、centerX约束；视图设置了left、width约束；视图设置了centerX、width约束……）。具有内在内容尺寸（IntrinsicContentSize）的视图表面上我们只需要设置两个约束，实际上它还是遵守了该原则，系统自动帮我们加另外两个约束（eg:UILabel只需设置位置约束，系统帮我们加上尺寸相关的两个约束）。
- 当多个约束对象描述的是同一个几何关系，不会造成约束冲突。eg:当A.width=B.width，视图C的添加了两个宽度约束，C.width = A.width、C.width = B.width，`两个约束产生的效果相同，即使都是最高优先级UILayoutPriorityRequired也不会有约束冲突。开发中避免这么使用，约束集合充分即可，过度了就存在约束冲突的隐患了`。
### 2.translatesAutoresizingMaskIntoConstraints
- [translatesAutoresizingMaskIntoConstraints](https://link.jianshu.com?t=https://developer.apple.com/documentation/uikit/uiview/1622572-translatesautoresizingmaskintoco)接口文档介绍：决定是否把UIView的autoresizing mask转化为Auto Layout约束集合的Bool值。autoresizing mask是Auto Layout之前的一种布局方案，Apple为了向后兼容，允许前者转换成后者。translatesAutoresizingMaskIntoConstraints默认设为YES，即autoresizing mask将会转换auto layout对应的约束集合（对应的约束类是NSAutoresizingMaskLayoutConstraint），且这些约束已经完全确定了UIView的位置和尺寸，如果额外添加约束就会造成约束冲突。
- 该属性默认为YES,使用Auto Layout布局时需要设置为NO。使用Masonry则不用手动设置，底层帮我们设置好了。
```
- (NSArray *)mas_makeConstraints:(void(^)(MASConstraintMaker *))block {
    
    // 禁用Autoresizing
    self.translatesAutoresizingMaskIntoConstraints = NO;
    
    // 初始化constraintMaker，执行设置约束的block
    MASConstraintMaker *constraintMaker = [[MASConstraintMaker alloc] initWithView:self];
    block(constraintMaker);
    
    // 安装约束
    return [constraintMaker install];
}
```
### 3.alignmentRect对齐矩形
##### alignmentRect简介
- 
Apple接口文档对alignmentRect的解释大致如下，位于UIView.h中分类UIView (UIConstraintBasedLayoutLayering)。Auto Layout并不是根据frame来确定视图的大小、尺寸，而是根据对齐矩形alignmentRect。默认情况下alignmentRect与frame是一致的，除非子类重写了alignmentRectInsets方法。对齐矩形是视图的边界，它忽略视图的装饰元素，例如阴影、徽章……即视图布局时确定尺寸和位置的对齐矩形，默认是忽略阴影、徽章……
- 
示例：绿色UIView设置约束如下，最终视图的尺寸、位置是忽略阴影的，仅仅是根据视图alignmentRect
![](https://upload-images.jianshu.io/upload_images/696463-f12bafafa536d360.png)
Snip20171008_166.png
![Snip20171008_166]
```
UIView *greenView = [[UIView alloc] init];
greenView.backgroundColor = [UIColor greenColor];
[self.view addSubview:greenView];
    
[greenView mas_makeConstraints:^(MASConstraintMaker *make) {
    make.left.equalTo(self.view).offset(50);
    make.top.equalTo(self.view).offset(100);            
    make.width.height.equalTo(@100);
}];
    
// 添加阴影
CALayer *greenViewLayer = greenView.layer;
greenViewLayer.shadowColor = [UIColor lightGrayColor].CGColor;
greenViewLayer.shadowOpacity = 1.0;  // 此参数默认为0，即阴影不显示
greenViewLayer.shadowRadius = 5.0;
greenViewLayer.shadowOffset = CGSizeMake(20, 20);
```
##### alignmentRect应用场景
- 
当UI提供的图片、或者代码绘制的图片包含阴影、徽章等装饰元素时，一般情况下是要忽略装饰元素，根据核心区域做约束，此时需要调整alignmentRect来达到目的。
- 
如果视图是UIImageView，则可以通过UIImage的方法imageWithAlignmentRectInsets来调整对齐矩形，插入内边距；如果是UIView，则可以通过重写UIView的方法alignmentRectInsets，调整对齐矩形。
- 
示例：UI提供的图片包含阴影效果，上、左、右分别有12pt的阴影，alignmentRect要实现与系统默认做法一致（忽略阴影），需要调整alignmentRect内边距。
- 此处要实现核心区域（不包括阴影）尺寸大小为60pt*120pt。图一不做任何处理，阴影效果作为alignmentRect的一部分参与布局，不满足需求；图二通过imageWithAlignmentRectInsets调整image，满足需求；图三通过重写alignmentRectInsets，调整alignmentRect，满足需求。代码详见DEMO
- 当然了，也可以把视图宽度增大为60pt+12pt*2，高度增大为120pt+12pt，x、y减小12pt，布局时包含阴影，也实现同样的效果。
![](https://upload-images.jianshu.io/upload_images/696463-1b5ca8f62d62bd26.jpg)
UI切图上、左、右包含12pt阴影效果
![](https://upload-images.jianshu.io/upload_images/696463-e71adf2c668659f9.png)
实现效果
##### alignmentRect可视化
- Xcode8下（笔者在Xcode9下配置出现奔溃，原因未知），可以配置启动参数，开启对齐矩形可视化，具体配置过程如图，图中的黄色框框就是对齐矩形的区域，也就是布局最终产生的效果（位置、尺寸）。
![](https://upload-images.jianshu.io/upload_images/696463-6075f8110af9d4e8.png)
alignmentRect可视化 开启
![](https://upload-images.jianshu.io/upload_images/696463-cbd34fdd475eb545.png)
alignmentRect可视化 效果
### 4.内在内容大小IntrinsicContentSize
##### IntrinsicContentSize介绍
- 
一般情况下，视图要设置位置、尺寸约束才能正确布局，拥有内在内容大小的视图，只需要设置位置约束，不需要设置尺寸约束。Auto Layout会根据视图的自然尺寸，自动帮我们设置尺寸约束，这就是Intrinsic Content Size，它描述的是视图内容（文字、图片等）在不压缩不拉伸情况下展示出来的自然尺寸。
- 
只有部分视图具有IntrinsicContentSize，有IntrinsicContentSize的视图，一部分是只有width/height，一部分两者兼具，具体如图所示
- UIView没有IntrinsicContentSize；
- UISlider在iOS下只定义了width；
- UILabel、UIButton、UISwitch、UITextField的IntrinsicContentSize同时存在width、height；
- UITextView、UIImageView的IntrinsicContentSize是动态变化的；
![](https://upload-images.jianshu.io/upload_images/696463-30b1ab3d49da787e.png)
图片出处 ：苹果官方文档 - Auto Layout Guide
- 
IntrinsicContentSize是基于控件的当前内容的。
- UILabel、UIButton的IntrinsicContentSize与视图文字数量、字体大小相关；没有设置内容之前，也有IntrinsicContentSize；
- UIImageView是IntrinsicContentSize是动态变化的，当没有设置image没有IntrinsicContentSize（（-1，-1）），当设置了image，则IntrinsicContentSize就是设置的image对应的Size;
- UITextView的IntrinsicContentSize也是动态变化的，它相对复杂，与内容、是否可滚动、约束相关。
- 
没有IntrinsicContentSize的视图，例如UIView，默认IntrinsicContentSize是返回（UIViewNoIntrinsicMetric, UIViewNoIntrinsicMetric），UIViewNoIntrinsicMetric是UIView中定义的一个常量，值为-1，表示没有内在内容大小。当然我们也可以自定义UIView，重写intrinsicContentSize方法，返回一个固定的CGSize，让自定义视图具备intrinsicContentSize。
##### IntrinsicContentSize原理分析
- 视图的IntrinsicContentSize本质上还是通过约束来实现的，AutoLayout在每个坐标轴方向设置两个约束，分别是：contentHugging（内容吸附）、compressionResistance（压缩阻力），简称`CHCR`。图示显示的是X轴方向的IntrinsicContentSize约束，伪代码如下
- 内容吸附向内挤压视图，使得视图匹配视图内容的自然大小，防止视图被拉伸、填充空白；
- 压缩阻力向外拉伸视图，使得视图匹配视图内容的自然大小，防止视图被挤压、剪切；
![](https://upload-images.jianshu.io/upload_images/696463-035e5de3187da1aa.png)
图片出处： 苹果官方文档 - Auto Layout Guide
```
// Compression Resistance
View.height >= 0.0 * NotAnAttribute + IntrinsicHeight
View.width >= 0.0 * NotAnAttribute + IntrinsicWidth
 
// Content Hugging
View.height <= 0.0 * NotAnAttribute + IntrinsicHeight
View.width <= 0.0 * NotAnAttribute + IntrinsicWidth
```
- IntrinsicContentSize对应的约束对象是私有类[NSContentSizeLayoutConstraint](https://link.jianshu.com?t=https://github.com/nst/iOS-Runtime-Headers/blob/master/Frameworks/Foundation.framework/NSContentSizeLayoutConstraint.h)。通过Log打印，可以验证视图的IntrinsicContentSize对应的约束。
- 设置了UIButton的位置约束，不设置尺寸约束，打印UIButton的约束集合，结果如下所示。
- UIButtonX轴、Y轴方向分别有对应的NSContentSizeLayoutConstraint类型的约束对象
- contentHugging（内容吸附）默认优先级为250、compressionResistance（压缩阻力）默认优先级为750。
```
UIButton *button = [[UIButton alloc] init];
[self.view addSubview:button];
[button setTitle:@"按钮按钮按钮" forState:UIControlStateNormal];
    
[button mas_makeConstraints:^(MASConstraintMaker *make) {
        make.bottom.equalTo(self.view).offset(-10);
        make.left.equalTo(self.view).offset(10);
}];
[self.view layoutIfNeeded];
    
for (id constrain in button.constraints) 
{
NSLog(@"%@;  %f  %f",constrain, (CGFloat)([[constrain valueForKey:@"compressionResistancePriority"] floatValue]), [[constrain valueForKey:@"huggingPriority"] floatValue]);
}
NSLog(@"UIButton %@", NSStringFromCGSize(button.intrinsicContentSize));
```
```
2017-09-25 09:33:00.640 AutoLayoutTheory[25066:6102718] <NSContentSizeLayoutConstraint:0x6000000b3ce0 UIButton:0x7ff52f113060.width == 111>;  750.000000  250.000000
2017-09-25 09:33:00.640 AutoLayoutTheory[25066:6102718] <NSContentSizeLayoutConstraint:0x6000000b3d40 UIButton:0x7ff52f113060.height == 34>;  750.000000  250.000000
2017-09-25 09:33:00.641 AutoLayoutTheory[25066:6102718] UIButton {111, 34}
```
##### IntrinsicContentSize应用
- 
具有IntrinsicContentSize的视图进行约束时只需设置位置 ，尺寸自动匹配内容，减少工作量。
- 
当存在多个具有IntrinsicContentSize视图，某些视图要保持原尺寸，只拉伸、压缩特定视图时，通过设置CHCR优先级来实现，它包括X、Y轴两个方向。
- 当使用Interface Builder布局时，默认会把UIImageView 、UILabel的content-hugging优先级调整为251。Apple这么做原因应该是：大部分情况下，当与UITextField等共存，当需要拉伸时，一般是希望拉伸后者。
- 通过代码设置CHCR优先级，可以把CHCR优先级调整为0~1000之间。不希望视图被拉伸则把content-hugging优先级调高；不希望视图被压缩则把compression-resistance优先级调高。`Apple建议避免设置CHCR优先级为UILayoutPriorityRequired。大部分情况下，设置CHCR优先级目的是保护视图不被拉伸、压缩，只需要把CHCR优先级设置为解决UILayoutPriorityRequired（eg:999）即可实现。设置CHCR优先级为1000时，当存在约束冲突时，就无法通过拉伸、压缩具有IntrinsicContentSize的视图来避免`。Apple设置content-hugging默认优先级为250，compression-resistance默认优先级为750的初衷，应该是：拉伸、压缩视图好过约束冲突造成的不确定性；拉伸视图好过压缩视图。建议项目中抽取自定义优先级常量。（当添加了视图尺寸约束，优先级设置为1000，并且把CHCR优先级调整为1000，最终结果不会产生约束冲突，而是显示视图尺寸约束对应的尺寸，笔者猜测是Apple底层做了处理，当约束优先级相同是，生效的时视图尺寸对应的约束，而不是IntrinsicContentSize对应的约束。）
- 代码演示
```
UILabel *label1 = [[UILabel alloc] init];
    label1.text = @"label1";
    label1.textColor = [UIColor blackColor];
    label1.backgroundColor = [UIColor greenColor];
    [self.view addSubview:label1];
    
    UITextField *textFeild = [[UITextField alloc] init];
    textFeild.placeholder = @"请输入文本";
    textFeild.textColor = [UIColor blackColor];
    textFeild.backgroundColor = [UIColor yellowColor];
    [self.view addSubview:textFeild];
    
    [label1 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(self.view).offset(10);
        make.bottom.equalTo(self.view).offset(-10);
    }];
    
    [textFeild mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(label1.mas_right).offset(10);
        make.right.equalTo(self.view).offset(-10);
        make.bottom.equalTo(label1);
    }];
    
    // 水平方向 label保持原尺寸，textFeild允许拉伸/压缩
    [label1 setContentCompressionResistancePriority:UILayoutPriorityRequired-1 forAxis:UILayoutConstraintAxisHorizontal];
    [label1 setContentHuggingPriority:UILayoutPriorityRequired-1 forAxis:UILayoutConstraintAxisHorizontal];
```
##### 内在内容尺寸IntrinsicContentSize与自适应尺寸FittingSize
- `IntrinsicContentSize是Auto Layout的输入源。而FittingSize是Auto Layout的输出结果`。上文提及：视图的IntrinsicContentSize本质上就是约束NSContentSizeLayoutConstraint，即IntrinsicContentSize是转化为约束集合的一部分，参与Auto Layout的布局计算。而一个视图的FittingSize是基于子视图的约束集合、内容，来计算出视图本身的尺寸，即FittingSize是Auto Layout的计算结果。自动计算高度的API `- (CGSize)systemLayoutSizeFittingSize:(CGSize)targetSize NS_AVAILABLE_IOS(6_0);`就是视图自上向下设置好子视图约束，视图即可计算出本身尺寸，实现自动撑开。
- UIStackView看起来好像是具有IntrinsicContentSize，实际上并不是，他是基于FittingSize。当使用UIStackView进行布局时，通常需要频繁设置具有IntrinsicContentSize的子视图CHCR优先级来调整视图拉伸、压缩，但是调整UIStackView的CHCR优先级是无效的，因为它就不具有IntrinsicContentSize。UIStackView添加子视图、设置好布局参数之后，就能自动撑开。UIStackView是通过设置约束实现布局的，只是这部分约束是系统自动转化，无需手动设置，即UIStackView本质上就是根据子视图的约束集合、内容来计算出本身的尺寸。
### 5.Auto Layout与国际化
- app国际化，即适配不同国家的语言，主要包括语种本地化、布局适配，此处主要讨论后者。
##### 布局方向适配
- 上文介绍NSLayoutAttribute布局属性提及：leading表示前边、trailing表示后边，在left-to-right language，leading=left、trailing=right。在right-to-left language，leading相当于right、trailing相当于left。苹果建议开发中避免使用left/right，而是使用leading/trailing，当设备语言切换到right-to-left languages时，所有视图都会自动实现镜面对称，从右到左布局。iOS9+可以通过UIView的属性semanticContentAttribute自定义是否需要实现镜面对称。苹果的官方文档、demo都是用leading/trailing。
![](https://upload-images.jianshu.io/upload_images/696463-25ac63f7a0c30d13.png)
图片出处：  苹果官方文档 - Auto Layout Guide
- 话说回来，Apple Developer提及的right-to-left language，也就Arabic、Hebrew两门语言。而支持Arabic（阿拉伯语）的Apple官网也是在2016年中才上线，App Store目前也还没支持right-to-left language，也就是说苹果爸爸家的APP都不支持。花大量的技术成本去适配，个人认为没有必要，毕竟也只有做梦能梦见阿拉伯人在使用自家的APP。当然了，如果是项目刚启动，可以规定使用leading/trailing，避免使用left/right。
##### 布局测试
- APP支持国际化需要注意：描述同一个内容，不同语种的字符长度不一致。对本地化文本进行测试时可以开启启动参数NSDoubleLocalizedStrings 设置为YES，配置字符串长度加倍，方便进行极端情况测试。具体配置如图
![](https://upload-images.jianshu.io/upload_images/696463-09d453fd5a89ed51.jpg)
15061736124730.jpg
- 适配right-to-left language，测试界面做镜面对称的翻转时，需要在Setting中配置iPhone语言为对应语种。iOS10下配置路径是：设置-通用-语言与地区-iPhone语言-阿拉伯文……
# 五、Auto Layout布局周期
### 1.Auto Layout布局机制
![](https://upload-images.jianshu.io/upload_images/696463-22995f9148fa634e.jpg)
图片出处： WWDC2015 - Mysteries of Auto Layout, Part 2
- 大多数人能够熟练使用Auto Layout进行布局，但不一定知晓它的布局过程。我们创建视图树、描述视图之间的约束、设置优先级、设置视图内容，Layout Engine计算出视图位置、尺寸，绘制出对应的图层。Auto Layout是个Black Box，当出现问题时，调试起来比较困难。
- Auto Layout布局过程涉及延迟机制，并非一有约束更新就马上进行布局重绘，当有约束更改时，系统的默认做法是延迟更新，目的是实现批量更改约束、绘制视图，避免频繁遍历视图层级，优化性能。当更新约束太慢影响到后序代码逻辑，也可强制马上更新。
### 2.Auto Layout布局流程
![](https://upload-images.jianshu.io/upload_images/696463-13cb1bc293287cf9.jpg)
图片出处： WWDC2015 - Mysteries of Auto Layout, Part 2
- 关于Auto Layout的布局流程，Apple给出图示如上：即Layout Cycle是一个在App运行循环RunLoop下循环执行的一个过程。
- App启动后开启RunLoop，循环检测图层树中是否存在约束变化；
- 当发生Constrints Change（直接or间接设置、更新、移除约束）,RunLoop检测到约束变化；
- RunLoop发现约束变化后，就会进入Deferred Layout阶段，视图的位置、尺寸值会在这个过程计算，设置到对应视图上，并绘制出来；
- 执行完一轮布局，RunLoop会继续检查视图树的约束更新情况，当再次发现约束更新，则执行新一轮布局……
##### Constraints Change
- Constraints Change过程包括两个步骤：约束更新；Layout Engine重新计算布局。
![](https://upload-images.jianshu.io/upload_images/696463-0aa59f43bb20cae3.jpg)
图片出处： WWDC2015 - Mysteries of Auto Layout, Part 2
- 
1.约束更新
- 约束作为输入，以线性方程集合的形式存放在Layout Engines中，当涉及到约束方程集合的更改，就属于Constraints Change。例如ctivate/deactivate约束对象（激活/失效，对应旧版本的安装/移除约束对象）；修改约束常量constant；修改约束优先级priority（改变约束方程集合的排序关系）；修改视图树的结构（eg：移除视图会自动移除与之相应的约束）
- 
2.Layout Engine重新计算布局
- 约束表达式是由表示视图的位置、尺寸的变量构成，当约束更新后，Layout Engine重新计算布局，这些变量可能被更新。重新计算布局后，那些由于约束更新导致位置、尺寸发生改变的视图的父视图会被打上needing layout的标记，即调用视图的setNeedsLayout进行标记。此时视图新的frame已经存在Layout Engine中，但是视图还没有更新位置、尺寸。接下来Deferred Layout Pass将会被安排执行。
##### Deferred Layout Pass
![](https://upload-images.jianshu.io/upload_images/696463-d76fe3eccc0d629a.jpg)
图片出处： WWDC2015 - Mysteries of Auto Layout, Part 2
- Deferred Layout Pass过程包括两个步骤 Update Constraints ；Reassign View Frames。
![](https://upload-images.jianshu.io/upload_images/696463-1d35fcc91683b81e.jpg)
图片出处： WWDC2015 - Mysteries of Auto Layout, Part 2
- Update Constraints约束更新
- 这里是笔者学习过程中比较迷惑的地方，前面的Constraints Change过程已经更新约束，到了Deferred Layout Pass阶段，又需要更新约束。该步骤确保将要发生改变的视图能够在此时更新，在遍历视图树重新摆放视图之前及时更新。
- 该步骤`从下到上（即：从子视图到父视图，表面上子视图是添加到最顶部，是"上面"，但是从数据结构角度是从视图树子点到父节点，因此是从下到上）遍历视图层级`，调用UIView的updateConstraints（UIViewController对应updateViewConstraints，该方法的默认实现是调用[self.view updateConstraints]），更新视图。可以重写该方法监听这个过程，也可以调用setNeedsUpdateConstraints手动触发。`自定义View重写updateConstraints，可以发现在调用之前，视图的needsUpdateConstraints属性为YES，调用完毕needsUpdateConstraints被标记为NO`
- 实际上这个阶段目的是让视图有机会`在下一轮layout pass前及时更新约束`。通常这个步骤并不是必须的。一般情况下我们在初始化时设置约束（eg:UIView的init方法、UIViewController的viewDidLoad方法……），在获取到数据、触发交互事件时更新约束，比较少在updateConstraints中更新约束。WWDC视频中Apple给了两个应用场景：当约束更新过慢时可以在updateConstraints方法中更新约束，得益于批处理，在此更新约束会比较快（实际上我们更常用layoutIfNeed强制立即布局）；另一个场景是当约束需要更具多个参数进行配置，导致约束需要频繁更新，此时可以同一在updateConstraints方法中更新，避免多次冗余更新。
![](https://upload-images.jianshu.io/upload_images/696463-4c6f6cdaa30ddb04.jpg)
图片出处： WWDC2015 - Mysteries of Auto Layout, Part 2
- Reassign View Frames重新赋值视图frames，更新视图的位置、尺寸
- 该步骤`从上到下遍历视图层级，调用更新约束时被标记为needing layout的视图的layoutSubviews方法（UIViewController是对应viewWillLayoutSubviews），让方法调用者重新布局它的子视图（注意不是本身）`。可以重写layoutSubviews进行监听。
- 实际上这个阶段是从Layout Engine中把视图的位置、尺寸的值读取出来设置到对应的视图上（在Mac OS中是通过setFrame赋值，在iOS中是setBounds、setCenter）。`重写layoutSubviews可以发现，视图本身frame在该方法调用前已经有值，调用后该值不变；子视图frame在该方法调用前是旧值，该方法调用完毕会赋上新值。`
![](https://upload-images.jianshu.io/upload_images/696463-38651a2d2f445931.jpg)
图片出处： WWDC2012：Introduction to Auto Layout for iOS and OS X
### 3.Auto Layout布局流程总结
- 学习完整个布局流程，笔者自我感觉没有理解透彻这个过程，只能算是了解基本流程、布局机制，欢迎大家发表见解。总而言是，Auto Layout布局是存在延迟的，视图的frame不会立即更新。
![](https://upload-images.jianshu.io/upload_images/696463-c22be4970e51f82e.jpg)
图片出处： WWDC2015 - Mysteries of Auto Layout, Part 2
- 对应重写layoutSubviews，Apple的建议是谨慎使用，除非使用Auto Layout无法搞定的布局需求，才考虑使用。这个一个微妙的时刻，有些视图layout完毕，有些即将layout。重写layoutSubviews需要记住几点
- 必须调用super的实现 （[super layoutSubviews]）；
- 如果想要invalidate子视图的布局，需要在调用super的实现之前；
- 不要在此调用setNeedsUpdateConstraints，因为update constraints pass已经过了，在此调用为时已晚；
- 不要invalidate视图对应是子视图树以外的视图，该方法只应该对子视图树负责，操作子视图树以外的视图可能会造成循环布局；
# References
- [苹果官方文档：Auto Layout Guide](https://link.jianshu.com?t=https://developer.apple.com/library/content/documentation/UserExperience/Conceptual/AutolayoutPG/index.html#//apple_ref/doc/uid/TP40010853-CH7-SW1)
- [objc：Advanced Auto Layout Toolbox](https://link.jianshu.com?t=https://www.objc.io/issues/3-views/advanced-auto-layout-toolbox/)
- [WWDC2012：Introduction to Auto Layout for iOS and OS X](https://link.jianshu.com?t=https://developer.apple.com/videos/play/wwdc2012/202/)
- [WWDC2015：Mysteries of Auto Layout, Part 2](https://link.jianshu.com?t=https://developer.apple.com/videos/play/wwdc2015/219/)
- [Autolayout的第一次亲密接触](https://www.jianshu.com/p/052e8c7e8e7f)
- 书籍《iOS Auto Layout 开发秘籍(第2版)》

