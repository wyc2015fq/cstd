# iOS UIButton之UIEdgeInsets详解 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月21日 13:42:35[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：33标签：[iOS UIButton之UIEdgeInsets详解](https://so.csdn.net/so/search/s.do?q=iOS UIButton之UIEdgeInsets详解&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
> 
级别：★★☆☆☆
标签：「UIButton内偏移量」「titleEdgeInsets」「imageEdgeInsets」
作者： [MrLiuQ](https://www.jianshu.com/u/6663b66c3df3)
审校： [QiShare团队](https://www.jianshu.com/c/b3bd94559163)
我们先看一下苹果官方对UIEdgeInsets说明：
```
typedef struct UIEdgeInsets {
    CGFloat top, left, bottom, right;  
// specify amount to inset (positive) for each of the edges. values can be negative to 'outset'
} UIEdgeInsets;
```
- 官方：`specify amount to inset (positive) for each of the edges. values can be negative to 'outset' .`
- 解释：对每条边向**内方向**的偏移量，可以为正值（向内偏移）也可以为负值（向外偏移）。
![](https://upload-images.jianshu.io/upload_images/3407530-dcdf89af112aebc4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/866/format/webp)
基本使用：
```
xxx.edgeInsets = UIEdgeInsetsMake(.0, .0, .0, .0);
//例如我们设置UICollectionView的edgeInset会使collectionView产生内偏移
UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
layout.sectionInset = UIEdgeInsetsMake(20.0, .0, .0, .0);
```
> 
引入正题：
然而，`UIButton`的内偏移量与其他控件有些区别，
因为UIButton内部默认有两个子控件：`UILabel`和`UIImageView`
所以UIButton在内偏移量的计算上会有差异。
为什么？先卖个关子，下文解释。
![](https://upload-images.jianshu.io/upload_images/3407530-d4405eb6278ee5ca.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/535/format/webp)
UIButton默认子控件位置
# UIButtonEdgeInsets:
需求：通过修改`edgeInsets`，改变Button内部的imageView和titleLabel的相对位置。
思路：通过修改button的两个属性：`titleEdgeInsets`和`imageEdgeInsets`，从而达到最终的具体需求。
这里列出了三个比较常见的需求：
- image左 title右
- image右 title左
- image上 title下
小编做了一个demo，效果如下图：
![](https://upload-images.jianshu.io/upload_images/3407530-69767caedaf7e4a5.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/267/format/webp)
Demo效果图
## 场景一：左图右字
```
button.titleEdgeInsets = UIEdgeInsetsMake(0, 10.0, 0, 0);
```
语法解释：设置了title的左边线的内偏移量为10.0，
但经测试，
**注意：实际上title和Image只有5.0的距离**
**注意：实际上title和Image只有5.0的距离**
**注意：实际上title和Image只有5.0的距离**
图解如下：
![](https://upload-images.jianshu.io/upload_images/1977357-0b5d01587b056456.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/638/format/webp)
在这种场景下：
- title的 **上边线、右边线、下边线 内偏移** 是相对于contentView的
- image的 **上边线、左边线、下边线 内偏移** 是相对于contentView的
- title的 **左边线 内偏移** 相对于image
- image的 **右边线 内偏移** 相对于title
## 场景二：左字右图
```
button.titleEdgeInsets = UIEdgeInsetsMake(.0, - button.imageView.bounds.size.width - 10.0, .0, button.imageView.bounds.size.width);
button.imageEdgeInsets = UIEdgeInsetsMake(.0, button.titleLabel.bounds.size.width, .0, - button.titleLabel.bounds.size.width);
```
语法解释：
- title的 **左边线** 内偏移 - (imageView.width +10）<=> 等价于 title的左边线 向 **内偏移的反方向** 移动 (image.width +10)
- title的 **右边线** 内偏移 imageView.width <=> 等价于 title的右边线 向 **内偏移的正方向** 移动 imageView.width
- image的 **左边线** 内偏移 titleLabel.width <=> 等价于 image的左边线 向 **内偏移的正方向** 移动 titleLabel.width
- image的 **右边线** 内偏移 - titleLabel.width <=> 等价于 title的左边线 向 **内偏移的反方向** 移动 titleLabel.width
是不是有点绕人？哈哈，不要慌，请看图解
![](https://upload-images.jianshu.io/upload_images/3407530-873682a4a867fc9a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/653/format/webp)
## 场景三：上图下字
```
button.titleEdgeInsets = UIEdgeInsetsMake(button.imageView.frame.size.height + 10.0, - button.imageView.bounds.size.width, .0, .0);
button.imageEdgeInsets = UIEdgeInsetsMake(.0, button.titleLabel.bounds.size.width / 2, button.titleLabel.frame.size.height + 10.0, - button.titleLabel.bounds.size.width / 2);
```
语法解释：
- 标题的**上边线**内偏移（imageView.height 10）<=>等价于标题上的边线向**内偏移的正方向**移动（image.height 10）
- 标题的**左边线**内偏移- imageView.width <=>等价于标题左边的线向**内偏移的反方向**移动image.width
- 图像的**左边线**内偏移titleLabel.width * 0.5 <=>等价于图像左边的线向**内偏移的正方向**移动titleLabel.width的一半
- 图像的**下边线**内偏移titleLabel.height + 10 <=>等价于图像下的边线向**内偏移的正方向**移动titleLabel.height + 10
- 图像的**右边线**内偏移- titleLabel.width * 0.5 <=>等价于图像右边的线向**内偏移的反方向**移动titleLabel.width的一半
请看图解：
![](https://upload-images.jianshu.io/upload_images/3407530-90591ad31804299e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/724/format/webp)
最后，本文Demo链接：[GitHub](https://github.com/QiShare/QiButton_UIEdgeInsets)
> 
关注我们的途径有：
[QiShare（简书）](https://www.jianshu.com/u/3db23baa08c7)
[QiShare（掘金）](https://juejin.im/user/5b542b76e51d4533d2042b62)
[QiShare（知乎）](https://www.zhihu.com/people/edit)
[QiShare（GitHub）](https://github.com/QiShare)
[QiShare（CocoaChina）](http://www.cocoachina.com/bbs/u.php?tid=658244)
[QiShare（StackOverflow）](https://stackoverflow.com/users/10118400/qishare)
QiShare（微信公众号）
> 
推荐文章：[iOS UISlider数值与滑块联动](https://www.jianshu.com/p/8dac60bf640f)
作者：QiShare 
链接：HTTPS：//www.jianshu.com/p/b4cb35c41bf0 
来源：书繁简
繁简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
