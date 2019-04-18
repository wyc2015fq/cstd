# 懒加载（Lazy-Load）- iOS - weixin_33985507的博客 - CSDN博客
2018年04月06日 15:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
[http://www.cocoachina.com/ios/20170720/19967.html](https://link.jianshu.com?t=http%3A%2F%2Fwww.cocoachina.com%2Fios%2F20170720%2F19967.html)
这一篇，被....
我认为博主说的没有错。
`懒加载的使用需要看具体的场景，比如一个很可能不会被使用的属性，使用懒加载确实可以避免无所谓的性能损耗；`
`还有就是`null_resettable`修饰的属性，该属性意为：setter nullable，但是 getter nonnull，典型的就是控制器的`view`属性：“你可以不要我，把我置空；但只要你需要我，我就是在的”。诸如此类都可以使用懒加载。`
懒加载的初衷，就是为了让自己在需要被用到的时候才初始化，避免性能的浪费。
而我们现在写代码的时候，几乎是把所有的属性都定义了出来，并使用懒加载。
- 你定义的属性会很多
- 有些控件或属性，是100%会用到的，又何必用懒加载
- 前两者告诉我们，你既用不到懒加载的特性（占用内存小），也会加大代码量。
但是！懒加载能让阅读代码的人，快速的找到这个组件的初始化，也能让代码的可读性更好，但是在定义属性的地方会很乱。
有利有弊，我们该怎么做。
方法一：
```
规范代码习惯，区分代码块
// 初始化
[self setupInit];
// 布局
[self setupLayout];
// 设置值
[self setupData];
```
方法二：
继续使用懒加载，不用去考虑什么这个属性要不要用懒加载，管他呢。
```
@property (nonatomic , strong) UIView *firstView;
@property (nonatomic , strong) UIView *secondView;
@property (nonatomic , strong) UIView *thirdView;
// .. NView
@property (nonatomic , strong) NSString *firstPro;
@property (nonatomic , strong) NSArray *secondPro;
@property (nonatomic , strong) NSDictionary *thirdPro;
// .. NProperty
```
`然后在用一个 Xcode 插件，快速生成懒加载代码。`
