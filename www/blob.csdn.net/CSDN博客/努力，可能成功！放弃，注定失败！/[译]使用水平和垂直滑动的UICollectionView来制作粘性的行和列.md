# [译]使用水平和垂直滑动的UICollectionView来制作粘性的行和列 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月05日 11:22:53[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：574标签：[iOS学习资料																[iOS 资源大全																[iOS																[UICollectionView](https://so.csdn.net/so/search/s.do?q=UICollectionView&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
#### 一个tableView可以被水平和垂直滑动吗?
应用程序日常开发工作的现实之一就是克服日常的挑战.[原文](http://www.brightec.co.uk/ideas/uicollectionview-using-horizontal-and-vertical-scrolling-sticky-rows-and-columns?destination=taxonomy/term/1)
远非是我们要抱怨它。我们喜欢每一个新的障碍并且爱解决各种各样的困惑和难题为我们的客户服务。
我们最近的挑战就是用一个`tableView`来实现水平和垂直的滑动。条件是：当水平滑动时第一列和垂直滑动时的第一行要保持静止.
最后,我们需要给这个tableView包含一个巨大的数据,因此它重要性就是能够平稳的滑动。
![](http://upload-images.jianshu.io/upload_images/318204-d52b7b752f79a9e0.gif?imageMogr2/auto-orient/strip)
## 挑战
首先,我们简单说明下挑战--有时在开发的第一步就是要明确实际的问题是什么:
- 创建一个水平和垂直滑动的tableView.
- 水平滑动第一列和垂直滑动第一行的粘性.
- 使用大数据的情况下,必须要保持平滑的滚动.
## 第一次尝试-UITableView
我们开始考虑在UIScrollView中包含一个`UITableView`,但经过一些尝试,我们意识到:定制它的滚动和粘性是一个棘手的任务。
使用UITableView来做出必要的定制工作,真的很痛苦.因此,我们决定使用可定制的UICollectionView.
## 第二次尝试-UICollectionView
`UICollectionView`提供了强大的定制化服务,允许你设置一个自定义的`UICollectionFlowLayout`.不幸的是,网格布局的滚动只能沿着一个轴滑动:要么是水平要么是垂直.
我们希望我们能嵌入一个UIScrollView的集合,允许我们能够两个方向都能滑动,因此多方考虑我们决定使用`UICollectionFlowLayout`的父类`UICollectionViewLayout`的子类。
## UICollectionViewLayout子类
以下是创建`collectionView` 的步骤:
- 创建一个`UICollectionViewLayout`的子类`CustomCollectionViewLayout`.
- 在`storyboard`中拖动一个`UICollectionView`.我们也可以使用代码创建，这取决于你自己.
- 
在`storyboard`中的`Attributes Inspector`设置一些属性.我们设置`Layout`为`Custom`,设置`Class`为`CustomCollectionViewLayout`.`Scrolling Enabled`为选中状态,如果我们想要每次只滑动一个方向,选中
```
Direction
 Lock Enabled
```
.否则,会同一时刻滑动两个方向.
![](http://upload-images.jianshu.io/upload_images/318204-c6003caab22472bd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 最后,将`UICollectionView`连接到`UIViewController`.不要忘记连接`dataSource`和`delegate`.
- 在我们的`CollectionView`中,这个视图表示`row`和`列所在的row`,因此记住`sections`的索引下标和`item`的索引下标.这取决我们设置`collectionView flow layout`的方向.
![](http://upload-images.jianshu.io/upload_images/318204-0eb5fb59fee1e257.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
一旦完成了设置`collectionView`的方式,我们就需要设置我们自己所要创建的`custom collection view layout`.首先,我们要设置3个管理所有数据的属性.
```
@property (nonatomic,strong) NSMutableArray *itemAttributes;
  @property (nonatomic,strong) NSMutableArray *itemsSize;
  @property (nonatomic, assign) CGSize contentSize;
```
- 
如果你要自定义`item`的大小,你需要定义以下方法:
`` - (CGSize)sizeForItemWithColumnIndex:(NSUInteger)columnIndex;``
我强烈推荐你计算item大小存储到`itemsSize`中,是为了每一个列只计算一次.提高性能.
- 
写如下方法并返回`YES`,是为了调用当你每次滑动`CollectionView`的时候都能够调用`prepareLayout`方法.你必须设置这个。
```
-(BOOL)shouldInvalidateLayoutForBoundsChange:(CGRect)newBounds
  {  
  return YES; 
  }
```
- 
你需要调用以下方法布局显示的items
```
/**
   *  CGRectOffset: 返回一个原点在源矩形基础上进行了偏移的矩    形。
   *  CGRectInset: 返回一个与源矩形共中心点的，或大些或小些的新    矩形。
   *  CGRectIntegral: 返回包围源矩形的最小整数矩形。
   *  const CGRect CGRectZero: 一个原点在(0, 0)，且长宽均为     0 的常数矩形。这个零矩形与 CGRectMake(0.0f, 0.0f, 0.0f, 0.0f) 是等    价的。
   *  const CGRect CGRectNull: 空矩形。这个会在，比如说，求两    个不相交的矩形的相交部分时返回。注意，空矩形不是零矩形。
   *  const CGRect CGRectInfinite: 无穷大矩形。
   *  CGRectDivide: 将源矩形分为两个子矩形。
   */
- (NSArray *)layoutAttributesForElementsInRect:(CGRect)rect
        {
       NSMutableArray *attributes = [@[] mutableCopy];
        for (NSArray *section in self.itemAttributes) {
               [attributes addObjectsFromArray:[section filteredArrayUsingPredicate:[NSPredicate predicateWithBlock:^BOOL(UICollectionViewLayoutAttributes *evaluatedObject, NSDictionary *bindings) {
           return CGRectIntersectsRect(rect, [evaluatedObject frame]);
        }]]];
      }
            return attributes;
      }
```
- 
你需要根据`indexPath`返回`layout attributes`,如果才能返回出正确的`item attribute`.
```
- (UICollectionViewLayoutAttributes *)layoutAttributesForItemAtIndexPath:(NSIndexPath *)indexPath
{
    return      self.itemAttributes[indexPath.section]  [indexPath.row];
}
```
- 
你还需要返回`collectionView`内容视图的滑动大小.
```
- (CGSize)collectionViewContentSize
  {
          return self.contentSize;
  }
```
- 
最后,需要设`prepareLayout`方法.我们重画`collectionView`这将要被每次调用.
`  - (void)prepareLayout;`
- 
最后,我们要确定要展示的数据,否则返回`void`
```
if ([self.collectionView numberOfSections] == 0) 
{
       return;
}
```
- 
现在必须检查我们是否已经计算了`item`的attribute.在确定的情况下,我们将要使用这个attributes来粘性头部(第15步).否则,我们要每一次都计算,并且保存他们到`itemAttributes`数组中.
基本上,我们为了保证它们课件,是通过遍历所有的`items`然后计算`item`的`size`、`frame`和`zIndex`.
最后,我们使用`last item`来计算`contentSize`为了计算出content的全部高度.
```
// 以下代码只在prepareLayout中执行一次
  self.itemAttributes = [@[] mutableCopy];
  self.itemsSize = [@[] mutableCopy];
 // Tip:我们可以使用NSUInteger来代替numberOfItemsInSection
 // NSUInteger numberOfItems = [self.collectionView numberOfItemsInSection:section];
 //计算每一列中item 的大小
 if (self.itemsSize.count != NUMBEROFCOLUMNS) {
   [self calculateItemsSize];
 }
 // 通过循环计算items
   for (int section = 0; section < [self.collectionView numberOfSections]; section++) {
   NSMutableArray *sectionAttributes = [@[] mutableCopy];
   for (NSUInteger index = 0; index < NUMBEROFCOLUMNS; index++) {
       CGSize itemSize = [self.itemsSize[index] CGSizeValue];
       // 每一次都创建一个UICollectionViewLayoutAttributes,并且保存到数组中
       NSIndexPath *indexPath = [NSIndexPath indexPathForItem:index inSection:section];
       UICollectionViewLayoutAttributes *attributes = [UICollectionViewLayoutAttributes layoutAttributesForCellWithIndexPath:indexPath];
       attributes.frame = CGRectIntegral(CGRectMake(xOffset, yOffset, itemSize.width, itemSize.height));
           if (section == 0 && index == 0) {
           attributes.zIndex = 1024; //设置(Sec0Row0)的第一项,使其能在第一列和第一行之上
       } else if (section == 0 || index == 0) {
           attributes.zIndex = 1023; //设置第一列和第一行在其余为设置的item之上
       }
       if (section == 0) {
           CGRect frame = attributes.frame;
           frame.origin.y = self.collectionView.contentOffset.y;
           attributes.frame = frame; // 顶部(第一行)黏性
       }
       if (index == 0) {
           CGRect frame = attributes.frame;
           frame.origin.x = self.collectionView.contentOffset.x;
           attributes.frame = frame; // 左部(第一列)黏性
       }
       [sectionAttributes addObject:attributes];
       xOffset = xOffset+itemSize.width;
       column++;
       // 如果最后的列,定义最终的宽度
       if (column == NUMBEROFCOLUMNS) {
           if (xOffset > contentWidth) {
               contentWidth = xOffset;
           }  
           // Reset values
           column = 0;
           xOffset = 0;
           yOffset += itemSize.height;
       }
   }
   [self.itemAttributes addObject:sectionAttributes];
 }
     //获取最后的一个item 来计算 content的全部高度
 UICollectionViewLayoutAttributes *attributes = [[self.itemAttributes lastObject] lastObject];
 contentHeight = attributes.frame.origin.y+attributes.frame.size.height;
 self.contentSize = CGSizeMake(contentWidth, contentHeight);
```
- 
事实上我们已经在之前计算过`items attributes`,我们只需要(垂直滑动的时候)通过设置`first row`的`y-offset`和(水平滑动)设置`first columns`的`x-offset`即可.
```
//如果不进入以下if语句,就会执行计算方法
if (self.itemAttributes.count > 0) {
     for (int section = 0; section < [self.collectionView numberOfSections]; section++) {
        NSUInteger numberOfItems = [self.collectionView numberOfItemsInSection:section];
        for (NSUInteger index = 0; index < numberOfItems; index++) {
             if (section != 0 && index != 0)      {         //这是我们不需要黏性的内容
              continue;
           }
        UICollectionViewLayoutAttributes *attributes = [self layoutAttributesForItemAtIndexPath:[NSIndexPath indexPathForItem:index inSection:section]];
        if (section == 0) { // 顶部(第一行)黏性
            CGRect frame = attributes.frame;
            frame.origin.y = self.collectionView.contentOffset.y;
            attributes.frame = frame;
        }
        if (index == 0) { // 左部(第一列)黏性
            CGRect frame = attributes.frame;
            frame.origin.x = self.collectionView.contentOffset.x;
            attributes.frame = frame;
        }
    }
}
return;
}
```
如若喜欢请点赞或关注,不定期的发布文章或译文.
[源代码下载](https://github.com/twenty-zp/CustomCollectionViewLayout/archive/master.zip)
[ps: 转载注明出处](http://blog.img421.com/shi-yong-shui-ping-he-chui-zhi-hua-dong-de-uicollectionviewlai-zhi-zuo-nian-xing-de-xing-he-lie/)
文／Springer（简书作者）
原文链接：http://www.jianshu.com/p/a0089f2db49e
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
