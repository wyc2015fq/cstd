# iOS: 玩转UICollectionViewLayout - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月08日 10:58:39[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：15291
#### 前言：
> 
实现垂直方向的单列表来说，使用UITableView足以；若是需要构建横向滑动列表、gridView等直线型布局，则使用UICollectionView+UICollectionViewFlowLayout搭建最合适；更复杂的布局，则可以使用UICollectionView+自定义Layout来实现。
文章的最后，笔者将会贴出使用自定义的UICollectionViewLayout实现的比较炫酷的动画，代码并非笔者原创，而是来自[珲少](http://my.oschina.net/u/2340880),实现的思路，读者可参考原文。
demo已上传到[github](https://github.com/Flying-Einstein/CollectionViewTest),方便读者下载；
#### 译文：
UICollectionViewLayout是一个应该子类化的抽象基类，用来生成collection view的布局信息。 布局对象决定`cell`,
`追加视图`(supplementary views), 和 `装饰视图`(decoration views) 在collection view内部的位置，并在 collection view 获取的时候提供这些布局信息。collection view将把这些布局信息应用到相应的视图上以便在屏幕上进行展示。
如果想使用UICollectionViewLayout，你必须把它子类化。在子类化UICollectionViewLayout之前，应该先了解[UICollectionViewFlowLayout](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UICollectionViewFlowLayout_class/)，看看它是否已经能够满足你的布局需求。(注：[UICollectionViewFlowLayout](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UICollectionViewFlowLayout_class/)是官方提供的子类化的UICollectionViewLayout，能满足绝大部分需求)
**子类化注意事项:**
布局对象(layout object)主要用来为collection view的元素提供位置信息和状态信息(比如：选中状态，未选中状态等)。布局对象并不负责创建视图，这些视图是collection view的数据源(data source)创建的。布局对象只不过定义了这些视图的位置和大小。
Collection view有三种需要布局的视图元素。
- Cells：
cell是布局中的主要元素，在collection中每一个cell代表了一个单独的数据项。一个collection view 既可能只有唯一的一组cell，也可能把这一组cell分到多个 段(section)。布局对象的主要功能就是在 collection view的内容区对这些cell进行布局
- supplementary views(追加视图)
追加视图代表和cell完全不同的数据。补充视图不支持选中非选中状态，可以通过追加视图添加某个段的段头和段尾，或者整个collection view的段头和段尾。追加视图不是必选项，即实不实现都可以。追加视图的使用和布局也是由布局对象决定的。
- Decoration views(装饰视图)
装饰视图是用来装饰的视图，不支持选中非选中状态，和数据也没有内在的关联(不像cell和section需要返回个数)。装饰视图可以看作另一种类型的追加视图，和追加视图一样，它们的使用和布局也是由布局对象决定的。
Collection view在多个不同的时刻都会从它的布局对象获取这些元素的布局信。每一个呈现在屏幕上的cell和view，布局信息都来自布局对象。同样，在collection view插入或者删除item的时候,就会有相应的布局对象被加入或者删除。需要注意的是，collection view进行的布局限制在屏幕可见的范围之内。(注：即，即便创建了所有元素的布局对象，但是真正的布局只是在可见的范围内，超出屏幕的部分没有布局)
**需要重写的方法 :**
每一个布局对象需要实现接下来的几个方法
`- (CGSize)collectionViewContentSize;`
返回collectionView内容区的宽度和高度，子类必须重载该方法，返回值代表了所有内容的宽度和高度，而不仅仅是可见范围的，collectionView通过该信息配置它的滚动范围，默认返回 CGSizeZero。
`- (NSArray<__kindofUICollectionViewLayoutAttributes *> *)layoutAttributesForElementsInRect:(CGRect)rect;`
返回UICollectionViewLayoutAttributes 类型的数组，UICollectionViewLayoutAttributes 对象包含cell或view的布局信息。子类必须重载该方法，并返回该区域内所有元素的布局信息，包括cell,追加视图和装饰视图。
在创建 layout attributes的时候，创建的是相应元素类型(cell, supplementary, decoration)的 attributes对象,比如：
```
```
+ (instancetype)layoutAttributesForCellWithIndexPath:(NSIndexPath *)indexPath;
+ (instancetype)layoutAttributesForSupplementaryViewOfKind:(NSString *)elementKind withIndexPath:(NSIndexPath *)indexPath;
+ (instancetype)layoutAttributesForDecorationViewOfKind:(NSString *)decorationViewKind withIndexPath:(NSIndexPath *)indexPath;
```
```
collection view 根据不同的类型区分属性，并根据这些信息决定创建怎样的视图及如何进行管理。
`-(UICollectionViewLayoutAttributes *)layoutAttributesForItemAtIndexPath:(NSIndexPath *)indexPath`
返回指定indexPath的item的布局信息。子类必须重载该方法,该方法只能为cell提供布局信息，不能为补充视图和装饰视图提供。
`- (UICollectionViewLayoutAttributes *)layoutAttributesForSupplementaryViewOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath;`
如果你的布局支持追加视图的话，必须重载该方法，该方法返回的是追加视图的布局信息，kind这个参数区分段头还是段尾的，在collectionview注册的时候回用到该参数。
`- (UICollectionViewLayoutAttributes *)layoutAttributesForDecorationViewOfKind:(NSString *)decorationViewKind atIndexPath:(NSIndexPath *)indexPath;`
如果你的布局支持装饰视图的话，必须重载该方法，该方法返回的是装饰视图的布局信息，ecorationViewKind这个参数在collectionview注册的时候回用到
`- (BOOL)shouldInvalidateLayoutForBoundsChange:(CGRect)newBounds`
该方法用来决定是否需要更新布局。如果collection view需要重新布局返回YES,否则返回NO,默认返回值为NO。子类重载该方法的时候，基于是否collection view的bounds的改变会引发cell和view布局的改变，给出正确的返回值。
如果collection view的bounds改变,该方法返回YES，collection view通过调用
invalidateLayoutWithContext方法使原来的layout失效
这些方法为collection view 在屏幕上布局提供了最基础的布局信息，如果你不想为追加视图和装饰视图布局，可以不去重载相应的方法。
当collection view的数据发生改变的时候，比如插入或者删除 item的时候，collection view将会要求布局对象更新相应的布局信息。移动、添加、删除 items时都必须更新相应的布局信息以便反映元素最新的位置。对于移动的元素， collection view提供了标准的方法获取更新后的布局信息。而collection view删除或者添加元素的时候，将会调用一些不同的方法，你应该重载以便提供正确的布局信息：
`- (UICollectionViewLayoutAttributes *)initialLayoutAttributesForAppearingItemAtIndexPath:(NSIndexPath *)itemIndexPath`
在一个 item被插入到collection view 的时候，返回开始的布局信息。这个方法在 prepareForCollectionViewUpdates:之后和finalizeCollectionViewUpdates 之前调用。collection view将会使用该布局信息作为动画的起点(结束点是该item在collection view 的最新的位置)。如果返回为nil，布局对象将用item的最终的attributes 作为动画的起点和终点。
```
//返回值是追加视图插入collection view时的布局信息。该方法使用同initialLayoutAttributesForAppearingItemAtIndexPath:
- (UICollectionViewLayoutAttributes *)initialLayoutAttributesForAppearingSupplementaryElementOfKind:(NSString *)elementKind atIndexPath:(NSIndexPath *)elementIndexPath
//返回值是装饰视图插入collection view时的布局信息。该方法使用同initialLayoutAttributesForAppearingItemAtIndexPath:
- (UICollectionViewLayoutAttributes *)initialLayoutAttributesForAppearingDecorationElementOfKind:(NSString *)elementKind atIndexPath:(NSIndexPath *)elementIndexPath
```
`- (UICollectionViewLayoutAttributes *)finalLayoutAttributesForDisappearingItemAtIndexPath:(NSIndexPath *)itemIndexPath`
返回值是item即将从collection view移除时候的布局信息，对即将删除的item来讲，该方法在 prepareForCollectionViewUpdates: 之后和finalizeCollectionViewUpdates 之前调用。在该方法中返回的布局信息描包含 item的状态信息和位置信息。 collection view将会把该信息作为动画的终点(起点是item当前的位置)。如果返回为nil的话，布局对象将会把当前的attribute，作为动画的起点和终点。
```
//....不用解释了吧 .....
- (UICollectionViewLayoutAttributes *)finalLayoutAttributesForDisappearingSupplementaryElementOfKind:(NSString *)elementKind atIndexPath:(NSIndexPath *)elementIndexPath
//....不用解释了吧 .....
- (UICollectionViewLayoutAttributes *)finalLayoutAttributesForDisappearingDecorationElementOfKind:(NSString *)elementKind atIndexPath:(NSIndexPath *)elementIndexPath
```
除了这些方法之外，你也可以重载`- (void)prepareForCollectionViewUpdates:(NSArray<UICollectionViewUpdateItem *> *)updateItems;`做一些和布局相关的准备工作。也可以重载`- (void)finalizeCollectionViewUpdates;`通过该方法添加一些动画到block，或者做一些和最终布局相关的工作。
**通过使上下文无效( Invalidation Contexts )来优化布局:**
在自定义布局的时候，应该通过使那些布局真正发生改变的部分无效来提升布局性能。在你改变item的时候，通过调用invalidateLayout方法强制collection view重新计算全部的布局信息，并应用该布局信息。一个更好的方式是只重新计算发生改变的布局信息，这恰恰是Invalidation Contexts允许你去做的。你可以通过Invalidation Context指明布局发生改变的部分，布局对象就可以根据该信息 减小重新计算的数据量。
通过子类化 `UICollectionViewLayoutInvalidationContext`,为你的布局定义一个 自定义的 invalidation context。在子类中定义一些属性，这些属性代表布局中可以单独重新计算的数据。当你需要 invalidate 你的布局时，创建一个 invalidation context 子类的实例，配置自定义的属性，并把该实例传给invalidateLayoutWithContext: 方法。你自定义的方法可以根据invalidation
 context 中的信息重新计算布局改变的部分。
如果你定义了一个自定义的 invalidation context 类，你也应该重载invalidationContextClass方法，返回自定义的类。 collection view 在需要invalidation context时，总是会创建一个指明的类实例。返回你自定义的子类，确保了自定义的对象拥有正确的 invalidation context。
**获取 Collection View 的信息:**
```
//当一个新的布局对象分配给collection view时，collection view将设置该属性的值
@property(nonatomic, readonly) UICollectionView *collectionView
//上面已经介绍过，不再介绍
- collectionViewContentSize；
```
`+ (Class)layoutAttributesClass`
返回创建布局信息时用到的类，如果你创建了继承自 UICollectionViewLayoutAttributes的子类，你也应该重载该方法，返回该子类。该方法主要是为了子类化，无需在代码中调用。
`- (void)prepareLayout`
该方法将通知布局对象更新当前的布局。布局更新发生在 collection view 第一次展示它的内容的时候，以及由于view的改变导致布局 invalidated 的时候。在布局更新期间， collection view都会首先调用该方法，允许布局对象对此次的更新做一些准备操作。
默认情况下，该方法不会做任何操作。子类可以重载该方法，在方法内部做一些和布局相关的数据创建或计算操作。
`- (UICollectionViewLayoutAttributes *)layoutAttributesForInteractivelyMovingItemAtIndexPath:(NSIndexPath *)indexPath withTargetPosition:(CGPoint)position`
当item在手势交互下移动时，通过该方法返回这个item布局的attributes 。默认实现是，复制已存在的attributes，改变attributes两个值，一个是中心点center；另一个是z轴的坐标值，设置成最大值。所以该item在collection view的最上层。子类重载该方法，可以按照自己的需求更改attributes，首先需要调用super类获取attributes,然后自定义返回的数据结构。
`- (CGPoint)targetContentOffsetForProposedContentOffset:(CGPoint)proposedContentOffset`
在进行动画式布局的时候，该方法返回内容区的偏移量。在布局更新或者布局转场的时候，collection view 调用该方法改变内容区的偏移量，该偏移量作为动画的结束点。如果动画或者转场造成item位置的改变并不是以最优的方式进行，可以重载该方法进行优化。 collection view在调用prepareLayout 和 collectionViewContentSize 之后调用该方法
`- (CGPoint)targetContentOffsetForProposedContentOffset:(CGPoint)proposedContentOffset withScrollingVelocity:(CGPoint)velocity`
该方法返回值为滑动停止的点。如果你希望内容区快速滑动到指定的区域，可以重载该方法。比如，你可以通过该方法让滑动停止在两个item中间的区域，而不是某个item的中间。
**和collection view更新相关的方法:**
`- (void)prepareForCollectionViewUpdates:(NSArray<UICollectionViewUpdateItem *> *)updateItems`
当插入或者删除 item的时候，collection view将会通知布局对象它将调整布局，第一步就是调用该方法告知布局对象发生了什么改变。除此之外，该方法也可以用来获取插入、删除、移动item的布局信息。
```
//该方法上文已介绍
- (void)finalizeCollectionViewUpdates
```
`- (NSArray<NSIndexPath *> *)indexPathsToInsertForSupplementaryViewOfKind:(NSString *)kind`
返回一个NSIndexPath 类型的数组，该数组存放的是将要添加到collection view中的追加视图的 NSIndexPath 。往collection view添加cell或者section的时候，就会调用该方法。collection view将会在prepareForCollectionViewUpdates: 和finalizeCollectionViewUpdates之间调用该方法。
`- (NSArray<NSIndexPath *> *)indexPathsToInsertForDecorationViewOfKind:(NSString *)kind`
```
//类比上个方法，不再过多解释；
- (NSArray<NSIndexPath *> *)indexPathsToDeleteForSupplementaryViewOfKind:(NSString *)kind
```
`- (NSArray<NSIndexPath *> *)indexPathsToDeleteForDecorationViewOfKind:(NSString *)kind;`
返回一个NSIndexPath 类型的数组，该数组存放的是将要从collection view中删除的追加视图的 NSIndexPath 。从collection view删除cell或者section的时候，就会调用该方法。collection view将会在prepareForCollectionViewUpdates: 和finalizeCollectionViewUpdates之间调用该方法。
```
类比)indexPathsToDeleteForDecorationViewOfKind:
- (NSIndexPath *)targetIndexPathForInteractivelyMovingItem:(NSIndexPath *)previousIndexPath withPosition:(CGPoint)position
```
根据item在collection view中的位置获取该item的index path。第一个参数该item原来的index path，第二个参数是item在collection view中的位置。在item移动的过程中，该方法将collection view中的location映射成相应 index paths。该方法的默认是现实，查找指定位置的已经存在的cell，返回该cell的 index path 。如果在相同的位置有多个cell，该方法默认返回最上层的cell。
你可以通过重载该方法来改变 index path的决定方式。比如，你可以返回z坐标轴最底层cell的index path.当你重载该方法的时候，没有必要去调用super类该方法。
**使布局失效:**
`- (void)invalidateLayout`
使当前的布局失效，同时触发布局更新，可以在任何时间调用该方法更新布局信息。该方法使collection view的布局立即失效，你可以在不触发多次更新的情况下，在同一个block中多次调用该方法。重载该方法的时候，必须调用super类的该方法。
`- (void)invalidateLayoutWithContext:(UICollectionViewLayoutInvalidationContext *)context`
该方法使布局的部分区域失效，而不是全局失效。是对invalidateLayout的一种优化，这种优化基于UICollectionViewLayoutInvalidationContext 类型的属性。如果你自定义了UICollectionViewLayoutInvalidationContext ，就应该重载该方法，并使用自定义的类型。重载该方法的时候，必须调用super类。
`+ (Class)invalidationContextClass;`
当你子类化UICollectionViewLayout，并且使用自定义的 invalidation context对象优化布局更新，重载该方法，返回UICollectionViewLayoutInvalidationContext的子类。当 collection view 需要invalidate你的布局的时候，将会用该方法返回值创建正确的invalidation context 对象。
``- (BOOL)shouldInvalidateLayoutForBoundsChange:(CGRect)newBounds;``
该方法决定了collection view是否能够进行布局更新，默认为NO。子类在重载该方法的时候，根据cell或者追加视图是否发生布局改变，返回正确的值。
``- (UICollectionViewLayoutInvalidationContext *)invalidationContextForBoundsChange:(CGRect)newbounds;``
collection view 的bounds发生改变的时候返回的`无效上下文`，该`无效上下文`描述了bounds变化后需要做出改变的部分。
该方法默认实现是，通过invalidationContextClass方法返回的类创建一个实例，并作为返回值。如果你想获得一个自定义的`无效上下文`对象，就要重载invalidationContextClass方法。
你可以通过重载该方法去创建和配置自定义的`无效上下文`。如果你重载该方法，第一步应该调用super类获取`无效上下文`,在获得该`无效上下文`后，为它设置自定义的属性，并返回。
`- (BOOL)shouldInvalidateLayoutForPreferredLayoutAttributes:(UICollectionViewLayoutAttributes *)preferredAttributes withOriginalAttributes:(UICollectionViewLayoutAttributes *)originalAttributes`
当collection view包含[self-sizing](http://yulingtianxia.com/blog/2014/08/17/New-in-Table-and-Collection-Views/?utm_source=tuicool&utm_medium=referral)(自排列)的cell时，这些cell可以在布局attributes 应用到它之前更改这些attributes。一个自排列的cell指明一个不同于布局对象给出的size的时候，就会这么做。当cell设置一系列不同的attributes时，collection view将会调用该方法判断是否需要更新布局，默认返回为NO。
`- (UICollectionViewLayoutInvalidationContext *)invalidationContextForPreferredLayoutAttributes:(UICollectionViewLayoutAttributes *)preferredAttributes withOriginalAttributes:(UICollectionViewLayoutAttributes *)originalAttributes`
该方法返回值是一个上下文，上下文包含布局中需要改变的信息。默认的实现是，使用invalidationContextClass 方法返回的类创建一个实例，并返回。
你可以通过重载该方法去创建和配置自定义的`无效上下文`。如果你重载该方法，第一步应该调用super类获取`无效上下文`,在获得该`无效上下文`后，为它设置自定义的属性，并返回。
```
- (UICollectionViewLayoutInvalidationContext *)invalidationContextForInteractivelyMovingItems:(NSArray<NSIndexPath *> *)targetIndexPaths withTargetPosition:(CGPoint)targetPosition previousIndexPaths:(NSArray<NSIndexPath *> *)previousIndexPaths previousPosition:(CGPoint)previousPosition;
- (UICollectionViewLayoutInvalidationContext *)invalidationContextForEndingInteractiveMovementOfItemsToFinalIndexPaths:(NSArray<NSIndexPath *> *)indexPaths previousIndexPaths:(NSArray<NSIndexPath *> *)previousIndexPaths movementCancelled:(BOOL)movementCancelled;
```
**调整动态的变化:**
`- (void)prepareForAnimatedBoundsChange:(CGRect)oldBounds；`
collection view在bounds动态改变或者插入、删除items之前，调用该方法。可以在该方法进行一些相关的计算，比如可以在该方法内部计算插入和删除的item初始和最终的位置。你也可用通过该方法添加动画，这些动画被用于处理item插入、删除和bounds改变。
`- (void)finalizeAnimatedBoundsChange;`
该方法在item插入、删除和bounds改变动画完成之后，清空相关的操作。
**在布局之间转换:**
``- (void)prepareForTransitionFromLayout:(UICollectionViewLayout *)oldLayout;``
告知布局对象将会作为新的布局被导入到 collection view，该方法先于转场之前执行，可以在该方法做一些初始化的操作，生成布局attributes;
``- (void)prepareForTransitionToLayout:(UICollectionViewLayout *)newLayout;``
告知布局对象作为布局即将从collection view移除，该方法先于转场之前执行，可以在该方法做一些初始化的操作，生成布局attributes;
`- (void)finalizeLayoutTransition；`
collection view在获取从一个布局向另一个布局转场的时候所有的布局attributes 后，调用该方法。你可以用该方法清空prepareForTransitionFromLayout: 和prepareForTransitionToLayout:生成的数据和缓存。
**注册装饰视图:**
```
```
- (void)registerClass:(Class)viewClass forDecorationViewOfKind:(NSString *)decorationViewKind;
- (void)registerNib:(UINib *)nib forDecorationViewOfKind:(NSString *)decorationViewKind;
```
```
**demo**
效果图：
![](http://upload-images.jianshu.io/upload_images/1595096-cf338b45bdadb18c.gif?imageMogr2/auto-orient/strip)
效果图:
![](http://upload-images.jianshu.io/upload_images/1595096-0e5dc842ab3b1f5b.gif?imageMogr2/auto-orient/strip)
#### 结语：
> 
本文并未对UICollectionView多做介绍，只是对官方UICollectionViewLayout文档进行翻译，笔者没有逐字逐句的进行翻译，在翻译的过程中也加入了自己的理解。如果读者觉得有什么不妥的地方，请予以指正，笔者会加以改正并及时更新。
#### 参考：
[http://my.oschina.net/u/2340880](http://my.oschina.net/u/2340880)
[http://yulingtianxia.com/blog/2014/08/17/New-in-Table-and-Collection-Views/?utm_source=tuicool&utm_medium=referral](http://yulingtianxia.com/blog/2014/08/17/New-in-Table-and-Collection-Views/?utm_source=tuicool&utm_medium=referral)
文／Flying_Einstein（简书作者）
原文链接：http://www.jianshu.com/p/97e930658671
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
