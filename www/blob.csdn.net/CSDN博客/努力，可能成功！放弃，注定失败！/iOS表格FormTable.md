# iOS表格FormTable - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年11月07日 13:21:26[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：124
在项目开发中，我们偶尔会用到表格来展示一些数据，但是在iOS的现有控件中，对于表格支持的不好，应该说并没有支持，所以，造了一个表格的轮子，根据数据驱动实现不同的表格样式及交互事件，先来看看效果
- 
图一：
![](https://upload-images.jianshu.io/upload_images/1044445-8a6fa2c68f6f75eb?imageMogr2/auto-orient/strip%7CimageView2/2/w/746/format/webp)
image
- 
图二：
![](https://upload-images.jianshu.io/upload_images/1044445-267bb2676692d9bf?imageMogr2/auto-orient/strip%7CimageView2/2/w/746/format/webp)
image
- 
图三：
![](https://upload-images.jianshu.io/upload_images/1044445-f9f37cd35b7b73a8?imageMogr2/auto-orient/strip%7CimageView2/2/w/746/format/webp)
image
还算比较简单，由于项目中还存在抬头数量不同的情况(图二与图三)，以及对特定的item具有点击事件，所以如果没有一个通用的轮子，在做UI的时候，还是比较麻烦的。
## 处理方案：
```
1.整个外部采用的是一个tableView，我们称为外部tableView
2.cell中的每行也还是一个tableview，我们称为内部tableView
3.内部tableView的cell就是collectionview了，每一个item，都是uicollectionviewCell
4.然后再针对item的样式，事件进行定制化开发。
```
## 样式说明
```
可以调整每个item的占比，文字的大小，颜色，对齐方式，是否存在边框线，以及是否有点击事件
```
## 数据模型
```
内层样式
FTFormItemModel
/** 显示内容 */
@property (strong, nonatomic) NSString *title;
/** 字体颜色 */
@property (strong, nonatomic) UIColor *textColor;
/** 字体大小 （默认12）*/
@property (assign, nonatomic) CGFloat fontSize;
/** 位置：居中，靠左，靠右等（默认居中） */
@property (assign, nonatomic) NSTextAlignment textAlignment;
/** 宽度比率，默认为：1 （假设比率为1，一行总共4个数据，则宽度为 ） 0.25 * SCREEN_WIDTH */
@property (assign, nonatomic) CGFloat widthRate;
/** 背景色 */
@property (strong, nonatomic) UIColor *backgroundColor;
/** 文字或图片 */
@property (assign, nonatomic) FTFormItemType formItemType;
#pragma mark - ################内容的展开################
/** 次级内容 */
@property (strong, nonatomic) NSString *subTitle;
/** 次级字体颜色 */
@property (strong, nonatomic) UIColor *subTextColor;
/** 次级字体大小 （默认12）*/
@property (assign, nonatomic) CGFloat subFontSize;
/** 是否允许点击 */
@property (assign, nonatomic, getter=isAllowTap) BOOL allowTap;
@property (assign, nonatomic, getter=isNextPage) BOOL nextPage;
```
```
外层数据
FTFormModel
/** 行高:默认44 */
@property (assign, nonatomic) CGFloat rowHeight;
/** 宽度比率总和 */
@property (assign, nonatomic) CGFloat widthRateTotal;
/** 背景色 */
@property (strong, nonatomic) UIColor *backgroundColor;
/** 边距 */
@property (nonatomic) UIEdgeInsets edgeInsets;
/** 分隔线长度：为0时则达到隐藏分隔线的效果，默认占满整行 */
@property (assign, nonatomic, ) CGFloat separateLineWidth;
/** item数据 */
@property (strong, nonatomic) NSArray <FTFormItemModel *> *formItemArray;
/** 属于第几条数据 */
@property (assign, nonatomic) NSInteger index;
#pragma mark - ################内容的展开################
/** sub宽度比率总和 */
@property (assign, nonatomic) CGFloat subWidthRateTotal;
/** 内边距 */
@property (nonatomic) UIEdgeInsets subEdgeInsets;
/** 子项集合 */
@property (strong, nonatomic) NSArray <FTFormItemModel *> *subItemArray;
/** 选中状态 */
@property (assign, nonatomic, getter=isSelectedStatus) BOOL selectedStatus;
```
完整Demo：[GitHub](https://github.com/hmx101607/FormTable)
作者：MasonCoder
链接：https://www.jianshu.com/p/9df66049f530
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
