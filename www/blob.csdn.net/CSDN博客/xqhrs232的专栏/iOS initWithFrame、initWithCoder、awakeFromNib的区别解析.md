# iOS initWithFrame、initWithCoder、awakeFromNib的区别解析 - xqhrs232的专栏 - CSDN博客
2019年04月19日 15:23:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：9
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://www.cnblogs.com/yajunLi/p/6344023.html](https://www.cnblogs.com/yajunLi/p/6344023.html)
相关文章
1、[iOS中initWithFrame方法的理解](https://www.cnblogs.com/lv14/articles/5013262.html)----[https://www.cnblogs.com/lv14/articles/5013262.html](https://www.cnblogs.com/lv14/articles/5013262.html)
2、iOS开发小坑·不要initWithFrame和init两个方法都放----[https://www.jianshu.com/p/4439f15b852f](https://www.jianshu.com/p/4439f15b852f)
3、init 和 initWithFrame 重写 注意事项 建议----[https://blog.csdn.net/hccgk/article/details/79461282](https://blog.csdn.net/hccgk/article/details/79461282)
当我们需要自定义一个View控件时，会有 initWithFrame、initWithCoder、awakeFromNib 这三个系统方法，关于这三个方法何时调用，如何调用，有时候可能很多人会弄混淆。 
下面我会就理论结合代码举例来说明一下：
1、initWithFrame
- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        NSLog(@"initWithFrame");
        [self initViews];
    }
    return self;
}
这个方法是当控件不是从xib、storyboard中创建时，会调用这个方法，比如：
 XibShow *xib = [[XibShow alloc] initWithFrame:CGRectMake(0, 330, KScreenWidth, 100)];
 [self.view addSubview:xib];
这里我直接用  [XibShow alloc] initWithFrame  来创建控件，所以，这里自定义控件内，只会走  initWithFrame 方法，其余两个不会走。
2、initWithCoder
- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        NSLog(@"initWithCoder");
        [self initViews];
    }
    return self;
}
当控件是从xib、storyboard中创建时，就会调用这个方法。
概括的理解就是：你的view或cell在xib或storyboard上有体现，比如直接用xib创建的，或是拉控件关联已有类等。
而且，即使我们自定义的view是纯代码，没有xib，但我们在vc的xib里引用了，效果是一样的。
![](https://images2015.cnblogs.com/blog/511196/201701/511196-20170123150002394-1488802862.png)
比如，这两种方式创建的自定义view，只要我们有在xib或storyboard里引用过，就都会执行initWithCoder和awakeFromNib
3、awakeFromNib
-(void)awakeFromNib{
    NSLog(@"awakeFromNib");
    [super awakeFromNib];
    [self initViews];    
}
这个方法在initWithCoder:方法后调用 ，顺序是：initWithCoder  -> awakeFromNib （下面会有代码截图）
- 可以这么来理解：
一开始经过initWithCoder创建出来的控件是死的，然后通过awakeFromNib来唤醒，所以这会有一个先后的调用顺序。
下面会有一个具体的示例：
1、创建一个vc，在storyboard上关联界面，在界面上新增一个view，关联我们的自定义view:XibShow。
![](https://images2015.cnblogs.com/blog/511196/201701/511196-20170123141403737-1919792173.png)
2、在vc里，再额外用代码新增一个自定义view控件，add到另一个位置。
![](https://images2015.cnblogs.com/blog/511196/201701/511196-20170123141538816-1405557507.png)
这样，运行出来的结果如下：
![](https://images2015.cnblogs.com/blog/511196/201701/511196-20170123141657034-815769612.png)![](https://images2015.cnblogs.com/blog/511196/201701/511196-20170123141745909-547697509.png)
其中：打印顺序如上图所示。
可以发现：
1、在viewDidLoad之前，由于是storyboard上创建的类，会先执行好，且会顺序走 initWithCoder、awakeFromNib两个方法
2、到viewDidLoad之后，执行到用代码创建的view，只会走 initWithFrame 方法
PS：这里还有个小细节：我在storyboard上托控件创建的类，即使我给了背景色，但执行出来，还是以自定义view本身的背景色为准。
（可以看上图，开始设的灰色，但运行出来还是红色）
**最后总结：**
1、initWithFrame只适用纯代码创建时调用，不涉及xib或storyboard。
2、initWithCoder、awakeFromNib是从xib、storyboard中创建时会调用。
3、所以一般来说，为了安全起见，初始化控件的代码我们写在一个方法里面（类似我上面的initViews方法），然后我们分别在initWithFrame和initWithCoder中都调用一下，这样就保证了无论你是通过什么方式创建的控件，代码都能起到作用。
