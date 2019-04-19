# iOS-无限循环轮播器(注释详细到没有之一) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月27日 17:01:59[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5119
### Bg:
1)有一段时间没有写文章了,最近事儿比较多,今天有人在技术群里面问我使用UIScrollview实现无限循环轮播的思想(3个UIImageView实现),我当时给了他一篇博客,不过好像这位朋友看的不是很懂,所以我写了一个小Demo打算写这篇文章去讲解下,帮助有需要的朋友们,所以我尽量把能写的注释都写详细了,把思想写全面了,让大家看一遍基本就明白了,里面关于如何好的封装一个控件的细节这里就不实现了,以讲解实现为主哦
2)另外这种循环利用的思想也是面试可能会问到的,说不定还是加分项哦
3)另外这个文章说的是3个UIImageView,其实2个imageview完全可以实现([点击这里看2个imageview实现](http://www.cocoachina.com/ios/20160328/15808.html)),实时判断向左向右,然后改变imageview的frame的x/y即可,当然还可以完全使用collectionview去实现,这个也比较流行([点击这里看collectionview实现](http://www.cnblogs.com/wendingding/p/3890953.html))
- 先看下效果图
![](http://upload-images.jianshu.io/upload_images/313229-ea8e6c676ba98238.gif?imageMogr2/auto-orient/strip)
kobe.gif
- 这个效果图也没什么特别,大家都看到过无数次了,包括这个无线轮播,大家想必也都了解过,所以这次我们不实现什么特别的效果,主要是通过这个小功能,给有需要的朋友讲解下无线轮播思想
### 使用3个imageview实现无线轮播的大致原理
- 将3个imageview添加到scrollview上面,scrollview的`contensize`是3个`imageview的宽度`,设置scrollview一开始初始的偏移量为一个`imageview宽度`,因为里面有3个UIImageView,所以scrollview默认显示的就是中间的那个imageview,并且关键就是让屏幕显示的始终就是中间的这个imagview
- 使用3个`imageview`来回更换图片,并在每一次更换图片后立即再设置scrollview偏移量还为一个imagview的宽度,也就是让scrollview滚动后再滚回原来默认的位置,这样就可以达到始终显示中间那个imageview的效果
- 看到过其他博客里面有这样描述过这个原理
```cpp
```cpp
ps:例如要使用三个UIImageView循环显示5张图片
1)由于中间的imageview是显示在屏幕上的,它需要在启动时默认显示第1张图片,那么左边的imagview
自然就需要显示最后一张图片,右边的imagview自然要显示第二张图片了.所以一开始肯定默认放图片5、
图片1、图片2，当前显示中间的UIImageView，也就是图片1
2)如果用户手指向左滑动,那么就会显示图片2，当图片2显示完整后迅速重新设置左中右三个UIImageView
的内容为图片1、图片2、图片3，然后马上设置contentOffset再次为一开始默认的一个imageview宽度,
让它滚回默认一开始的位置,以此来达到一直显示的是中间的UIImageView的效果，此刻中间那个imagview
显示的也就是图片2
3)继续向左滑动看到图片3，当图片3滚动完成迅速重新设置3个UIImageView的内容为图片2、图片3、图片
4，然后通过设置contentOffset依然显示中间的那个UIImageView，此刻也就是图片3
5)当然，向右滑动原理完全一样，如此操作就给用户一种循环的错觉，而且图片多的话不占用过多内存
```
```
- 为此我做了一个动态图,以此来动态描述下这个原理
![](http://upload-images.jianshu.io/upload_images/313229-7c78188b3624b7df.gif?imageMogr2/auto-orient/strip)
scroll.gif
### 部分代码实现:
- 大致原理就是上述那些,文字比较多,但是是核心思想,可以借助代码再去理解一下,代理里面注释也是非常详细的
```
- (void)creatUI
{
    //初始化scrollview
    _scrollView = [[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, self.frame.size.width, self.frame.size.height)];
    self.scrollView.contentSize = CGSizeMake(view_WIDTH * 3, view_HEIGHT);
    self.scrollView.showsHorizontalScrollIndicator = NO;
    self.scrollView.showsVerticalScrollIndicator = NO;
    self.scrollView.pagingEnabled = YES;
    self.scrollView.bounces = NO;
    //设置scrollview一开始的偏移量为一个宽度,因为里面有3个UIImageView,所以scrollview默认显示的就是中间的那个imageview
    self.scrollView.contentOffset = CGPointMake(view_WIDTH, 0);
    self.scrollView.delegate = self;
    [self addSubview:self.scrollView];
    //初始化imageview
    _imageViews = [NSMutableArray array];
    //创建三个imageView作为循环复用的载体，图片将循环加载在这三个imageView上面
    for (NSInteger i = 0; i < 3; i++) {
        UIImageView *imageView = [[UIImageView alloc] init];
        imageView.frame = CGRectMake(view_WIDTH * i, 0, view_WIDTH,view_HEIGHT);
        //(self.dataArray.count - 1 + i)%self.dataArray.count也可以达到让一开始3个imageview分别显示最后一张<-->第一张<-->第二张图片,但是让大家理解起来会有一定难度,所以采用下面最简单的方法直接设置
        //imageView.tag = (self.dataArray.count - 1 + i)%self.dataArray.count;
        //3个imageview一开始需要的图片分别对应图片数组的图片索引应该是imageview[0].index-->images.count-1,imageview[1].index-->0,imageview[2].index-->1
        NSInteger index = 0;
        if (i == 0) index = _imagesArray.count - 1;
        if (i == 1) index = 0;
        if (i == 2) index = 1;
        //把index赋值给imageview的tag值,这样方便在后面方法中通过imageview的tag值直接拿到index,我们就可以轻松从图片数组获取对应的图片然后显示到imageview上面,有媒介的作用
        imageView.tag = index;
        imageView.userInteractionEnabled = YES;
       //这里给imageview添加了一个单击手势,通过block回调处理了imageview点击的监听事件
        UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(imageViewClicked:)];
        [imageView addGestureRecognizer:tap];
        //设置imageView上的image图片,2个方法使用哪一个设置都可以,关于2个方法的选择可以看下面详细注释
        [self setImageWithImageView:imageView];
        [self setImageView:imageView atIndex:index];
        //将imageView加入数组中，方便随后取用
        [_imageViews addObject:imageView];
        [self.scrollView addSubview:imageView];
    }
    //初始化pageControl,最后添加,这样它会显示在最前面,不会被遮挡
    self.pageControl = [[UIPageControl alloc] initWithFrame:CGRectMake(0, CGRectGetMaxY(self.scrollView.frame) - 30, view_WIDTH, 30)];
    self.pageControl.numberOfPages = _imagesArray.count;
    self.pageControl.currentPage = 0;
    [self addSubview:self.pageControl];
}
```
```
/*
 这里给imageview设置图片有2个选择,第一可以使用这个方法,传递一个需要设置的imageview以及对应的index
 好处:一眼就可以让大家明白这个方法内部的实现功能,易与阅读和理解
 相对于下面那个方法的坏处:其实我们完全可以不传递index这个参数,我们完全可以把index赋值给imageview的tag,这样我们只用传递一个imageview过来,就可以既拿到imageview,又可以通过imageview的tag拿到index
 总结:2个方法都可以,看大家喜欢哪一种,哪一种顺手好理解就使用哪一种
 */
- (void)setImageView:(UIImageView *)imageView atIndex:(NSInteger)index
{
    //根据实时计算得出的index,从图片数组里面取值,然后赋值给对应左中右3个imageview
    UIImage *image = (UIImage *)_imagesArray[index];
    imageView.image = image;
}
- (void)setImageWithImageView:(UIImageView *)imageView{
    //根据imageView的tag值给imageView设置image
    // UIImage *image = (UIImage *)self.dataArray[imageView.tag];
    // imageView.image = image;
}
```
```
//定时器调用的方法
- (void)nextPage
{
    //NSLog(@"定时器的%f",_scrollView.contentOffset.x);
    //定时器方法都是相当于向左滑动,偏移量是增大的,原本偏移量是一倍的宽度,定时器方法执行一次,偏移量就要增大一个宽度,这样也就是setContentOffset:CGPointMake(VIEW_WIDTH * 2, 0),相当于设置偏移量是2倍宽度
    //执行了setContentOffset:方法,系统会自动调用scrollViewDidEndScrollingAnimation:方法,在这个方法里面再设置回偏移量等于一倍的宽度,同时更换各个imageview的图片,那么还是相当于中间的那个imageview显示在屏幕上
    [self.scrollView setContentOffset:CGPointMake(view_WIDTH * 2, 0) animated:YES];
}
```
```
#pragma mark - 更新图片和分页控件的当前页
- (void)updateImageViewsAndPageControl {
    //先判断出scrollview的操作行为是向左向右还是不动
    //定义一个flag,目前是让scrollview向左向右滑动的时候索引对应的+1或者-1
    int flag = 0;
    if (self.scrollView.contentOffset.x > view_WIDTH)
    {//手指向左滑动
        flag = 1;
    }
    else if (self.scrollView.contentOffset.x == 0)//原本偏移量是一个宽度,现在==0了,那么就是手指向右滑动了
    {//手指向右滑动
        flag = -1;
    }
    else
    {//除了向左向右之外就是没有移动,那么不需要任何操作，直接返回
        return;
    }
    //    NSInteger index = 0;
    //修改imageViews中的imageView的tag值，从而修改imageView上显示的image，pageControl的页码
    for (UIImageView *imageView in _imageViews) {
        /*
         （1）当屏幕中间那个imageview显示最后一张图片时，右边的ImageView,也即下一张图片应该是显示最开始的那一张图片(第0张)；
         （2）当屏幕中间显示最开始的那一张图片(第0张)时，左边的ImageView,也即上一张图片应该是最后一张图片。
         */
        NSInteger index = imageView.tag + flag ;
        if (index < 0) {
            index = self.pageControl.numberOfPages - 1;
        } else if (index >= self.pageControl.numberOfPages) {
            index = 0;
        }
        imageView.tag = index;
        //更新每一页上的image
        [self setImageWithImageView:imageView];
        [self setImageView:imageView atIndex:index];
    }
    //更新pageControl显示的页码,也就是中间那个imageview的tag值
    self.pageControl.currentPage = [_imageViews[1] tag];
    //使用无动画的效果快速切换,也就是把scrollview的偏移量还设置成一个imageview的宽度
    //这里是通过设置scrollview的偏移量让其来回滑动,时刻更换imageview的图片,每换一次,就立即让scrollview以无动画的方式再回到偏移量为一个imageview宽度的偏移量位置,即还是显示的中间那个imageview,以此给用户产生一种来回切换的错觉,实质一直是在显示中间那个imageview
    self.scrollView.contentOffset = CGPointMake(view_WIDTH, 0);
}
```
- 其他的就是在scrollview的几个代理方法里面要么开启定时器,要么关闭定时器,要么就是调用`updateImageViewsAndPageControl`方法更新图片以及分页控件的状态了,这里就不再描述了,详细的建议可以看代码完整参考理解,另外这些是个人理解,不足之处欢迎指正,感谢支持
- 代码可以[点击这里](https://github.com/LBCoderLee/LBBannerView)查看
推荐拓展阅读
[举报文章](#)- [广告及垃圾信息](/notes/4535392/abuse_reports?type=ad)
- [抄袭或未授权转载](/notes/4535392/abuse_reports?type=plagiarism)
- [其他](#report-modal)
著作权归作者所有
如果觉得我的文章对您有用，请随意打赏。您的支持将鼓励我继续创作！
文／小小小阿博er（简书作者）
原文链接：http://www.jianshu.com/p/3444ef49dc2a
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
