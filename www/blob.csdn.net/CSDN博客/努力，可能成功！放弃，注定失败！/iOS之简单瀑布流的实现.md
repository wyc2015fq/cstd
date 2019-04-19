# iOS之简单瀑布流的实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月30日 11:09:25[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：338标签：[iOS学习资料																[iOS开发																[iOS																[iOS之简单瀑布流的实现](https://so.csdn.net/so/search/s.do?q=iOS之简单瀑布流的实现&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS开发&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
# 前言
> 
超简单的瀑布流实现，这里说一下笔者的思路，[详细代码在这里](https://github.com/codingZero/XRWaterfallLayout)
# 效果演示
![](http://upload-images.jianshu.io/upload_images/1429074-1957143332caad52.gif?imageMogr2/auto-orient/strip)
# 实现思路
> 
collectionView能实现各中吊炸天的布局，其精髓就在于UICollectionViewLayout，因此我们要自定义一个layout来继承系统的UICollectionViewLayout，所有工作都在这个类中进行
#### 1.定义所需属性
瀑布流的思路就是，从上往下，那一列最短，就把下一个item放在哪一列，因此我们需要定义一个字典来记录每一列的最大y值
每一个item都有一个attributes，因此定义一个数组来保存每一个item的attributes
我们还必须知道有多少列以及列间距、行间距、section到collectionView的边距
![](http://upload-images.jianshu.io/upload_images/1429074-a9e7dc0bd2e41b7e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
```
//总列数
@property (nonatomic, assign) NSInteger columnCount;
//列间距
@property (nonatomic, assign) NSInteger columnSpacing;
//行间距
@property (nonatomic, assign) NSInteger rowSpacing;
//section到collectionView的边距
@property (nonatomic, assign) UIEdgeInsets sectionInset;
//保存每一列最大y值的数组
@property (nonatomic, strong) NSMutableDictionary *maxYDic;
//保存每一个item的attributes的数组
@property (nonatomic, strong) NSMutableArray *attributesArray;
```
#### 2.重写系统方法
> 
我们一共需要重写4个方法
a.- (void)prepareLayout
b.- (CGSize)collectionViewContentSize
c.- (UICollectionViewLayoutAttributes *)layoutAttributesForItemAtIndexPath:(NSIndexPath *)indexPath
d.- (NSArray *)layoutAttributesForElementsInRect:(CGRect)rect
###### - (void)prepareLayout 方法
布局前的一些准备工作都在这里进行
初始化字典，有几列就有几个键值对，key为第几列，value为列的最大y值，初始值为上内边距
```
for (int i = 0; i < self.columnCount; i++) {
    self.maxYDic[@(i)] = @(self.sectionInset.top);
}
```
创建每个item的attributes，并存入数组
```
//根据collectionView获取总共有多少个item
NSInteger itemCount = [self.collectionView numberOfItemsInSection:0];
//为每一个item创建一个attributes并存入数组
for (int i = 0; i < itemCount; i++) {
    UICollectionViewLayoutAttributes *attributes = [self layoutAttributesForItemAtIndexPath:[NSIndexPath indexPathForItem:i inSection:0]];
    [self.attributesArray addObject:attributes];
}
```
###### - (CGSize)collectionViewContentSize 方法
用来计算collectionView的contentSize
一般瀑布流只能垂直滚动，不能水平滚动，因此contentSize.width = 0，我们只需要计算contentSize.height即可
从字典中找出最长列的最大y值，再加上下面的内边距，即为contentSize.height
```
- (CGSize)collectionViewContentSize {
    //假设第0列是最长的那列
    __block NSNumber *maxIndex = @0;
    //遍历字典，找出最长的那一列
    [self.maxYDic enumerateKeysAndObjectsUsingBlock:^(NSNumber *key, NSNumber *obj, BOOL *stop) {
        //如果maxColumn列的最大y值小于obj，则让maxColumn等于obj所属的列
        if ([self.maxYDic[maxIndex] floatValue] < obj.floatValue) {
            maxIndex = key;
        }
    }];
    //collectionView的contentSize.height就等于最长列的最大y值+下内边距
    return CGSizeMake(0, [self.maxYDic[maxIndex] floatValue] + self.sectionInset.bottom);
}
```
###### - (UICollectionViewLayoutAttributes *)layoutAttributesForItemAtIndexPath:(NSIndexPath *)indexPath 方法
该方法则用来设置每个item的attributes，在这里，我们只需要简单的设置每个item的attributes.frame即可
首先我们必须得知collectionView的尺寸，然后我们根据collectionView的宽度，以及列数、各个间距来计算每个item的宽度
item的宽度 = (collectionView的宽度 - 内边距及列边距) / 列数
![](http://upload-images.jianshu.io/upload_images/1429074-3af52d8a3037c966.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
```
CGFloat collectionViewWidth = self.collectionView.frame.size.width;
//self.sectionInset.left：左边距    self.sectionInset.right：右边距
//(self.columnCount - 1) * columnSpacing：一行中所有的列边距
CGFloat itemWidth = (collectionViewWidth - self.sectionInset.left - self.sectionInset.right - (self.columnCount - 1) * self.columnSpacing) / self.columnCount;
```
接下来计算item的坐标，要想计算坐标，那就必须知道最短的那一列，先遍历字典，找出最短列是哪一列（minColumn）以及其最大y值
item的y值就等于最短列的最大y值再加上行间距，x值就等于左边距 + (item宽度 + 列间距) * minColumn
![](http://upload-images.jianshu.io/upload_images/1429074-f4143d3948e1a169.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
```
//找出最短的那一列
__block NSNumber *minIndex = @0;
[self.maxYDic enumerateKeysAndObjectsUsingBlock:^(NSNumber *key, NSNumber *obj, BOOL *stop) {
    if ([self.maxYDic[minIndex] floatValue] > obj.floatValue) {
        minIndex = key;
    }
}];
//根据最短列的列数计算item的x值
CGFloat itemX = self.sectionInset.left + (self.columnSpacing + itemWidth) * minIndex.integerValue;
//item的y值 = 最短列的最大y值 + 行间距
CGFloat itemY = [self.maxYDic[minIndex] floatValue] + self.rowSpacing;
```
接下来便是item的高度，我们应该根据图片的原始尺寸以及计算出来的宽度，等比例缩放来计算高度，但是在layout类中，我们是拿不到图片的，因此我们可以定义一个block属性，或者代理，让外界来计算并返回给我们，我们需要将item的宽度以及indexPath传递给外界
`@property (nonatomic, strong) CGFloat(^itemHeightBlock)(CGFloat itemHeight,NSIndexPath *indexPath);`
根据返回值来设置item的高度
`if (self.itemHeightBlock) itemHeight = self.itemHeightBlock(itemWidth, indexPath);`
最后设置attributes的frame并更新字典
```
//设置attributes的frame
attributes.frame = CGRectMake(itemX, itemY, itemWidth, itemHeight);
//更新字典中的最短列的最大y值
self.maxYDic[minIndex] = @(CGRectGetMaxY(attributes.frame));
```
###### - (NSArray *)layoutAttributesForElementsInRect:(CGRect)rect 方法
该方法用来返回rect范围内，item的attributes
直接返回attributesArray即可
```
- (NSArray *)layoutAttributesForElementsInRect:(CGRect)rect {
    return self.attributesArray;
}
```
# 使用
布局类写完了，接下来就可以直接使用了
```cpp
```cpp
//创建布局对象
XRWaterfallLayout *waterfall = [[XRWaterfallLayout alloc] init];
//设置相关属性
waterfall.columnCount = 3;//共多少列
waterfall.columnSpacing = 10;//列间距
waterfall.rowSpacing = 10;//行间距
waterfall.sectionInset = UIEdgeInsetsMake(10, 10 , 10, 10);//内边距
[waterfall setItemHeightBlock:^CGFloat(CGFloat itemWidth, NSIndexPath *indexPath) {
    //根据图片的原始尺寸，及显示宽度，等比例缩放来计算显示高度
    XRImage *image = self.images[indexPath.item];
    return image.imageH / image.imageW * itemWidth;
}];
collectionView.collectionViewLayout = waterfall;
......
......
```
```
具体代码请到这里下载[https://github.com/codingZero/XRWaterfallLayout](https://github.com/codingZero/XRWaterfallLayout)，觉得不错的，请献上你的star
