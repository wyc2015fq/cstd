# iOS 股票K线图绘制 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年07月10日 22:29:18[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2666
# 本文来自简书，原文地址:[http://www.jianshu.com/p/f7906f5b4f4c](http://www.jianshu.com/p/f7906f5b4f4c)
# 最近一段时间写了一个iOS的K线图。写这个纯属个人兴趣，正好提高一下自己绘图方面的能力。在写的时候，参考了[Cocoa-Charts](https://github.com/limccn/Cocoa-Charts) 的实现思路。 废话不多说，先上图:
![](http://upload-images.jianshu.io/upload_images/1331138-143f21a94d2a1a97.gif?imageMogr2/auto-orient/strip)
![](http://upload-images.jianshu.io/upload_images/1331138-1a3aa0592c6902d0.gif?imageMogr2/auto-orient/strip)
![](http://upload-images.jianshu.io/upload_images/1331138-5800b6181899ec95.gif?imageMogr2/auto-orient/strip)
## 特点:
- 采用 CAShapeLayer + UIBezierPath绘制，绘制效率高，占用内存低
- 底层视图是UIScrollView，ScrollView上面添加一个View，所有的绘制在这个View上完成。体验流畅丝滑，FPS平均在55帧以上
- 指标支持MACD WR KDJ，指标计算采用TALib，方便扩展
- 横竖屏切换
- 
右拉加载更多数据
## 代理方法
```
/**
取得当前屏幕内模型数组的开始下标以及个数
@param leftPostion 当前屏幕最右边的位置
@param index 下标
@param count 个数
*/
- (void)displayScreenleftPostion:(CGFloat)leftPostion startIndex:(NSInteger)index count:(NSInteger)count;
/**
长按手势获得当前k线下标以及模型
@param kLineModeIndex 当前k线在可视范围数组的位置下标
@param kLineModel   k线模型
*/
- (void)longPressCandleViewWithIndex:(NSInteger)kLineModeIndex kLineModel:(ZYWCandleModel *)kLineModel;
/**
返回当前屏幕最后一根k线模型
@param kLineModel k线模型
*/
- (void)displayLastModel:(ZYWCandleModel *)kLineModel;
/**
加载更多数据
*/
- (void)displayMoreData;`
```
## 基础属性方法
```
/**
 数据源数组 在调用绘制方法之前设置 。Demo中数据源个数是固定的，如需实现类似右拉加载更多效果(参考网易贵金属)，需要在每次添加数据的时候设置 然后调用绘制方法 (现在本地数据是重复的6组)
 */
@property (nonatomic,strong) NSMutableArray<__kindof ZYWCandleModel*> *dataArray;
/**
 当前屏幕范围内显示的k线模型数组
 */
@property (nonatomic,strong) NSMutableArray *currentDisplayArray;
/**
 当前屏幕范围内显示的k线位置数组
 */
@property (nonatomic,strong) NSMutableArray *currentPostionArray;
/**
 可视区域显示多少根k线 (如果数据源数组不足以占满屏幕，需要手动给定宽度)
 */
@property (nonatomic,assign) NSInteger displayCount;
/**
 k线之间的距离
 */
@property (nonatomic,assign) CGFloat candleSpace;
/**
 k线的宽度 根据每页k线的根数和k线之间的距离动态计算得出
 */
@property (nonatomic,assign) CGFloat candleWidth;
/**
 k线最小高度
 */
@property (nonatomic,assign) CGFloat minHeight;
/**
 当前屏幕范围内绘制起点位置
 */
@property (nonatomic,assign) CGFloat leftPostion;
/**
 当前绘制的起始下标
 */
@property (nonatomic,assign) NSInteger currentStartIndex;
/**
 滑到最右侧的偏移量
 */
@property (nonatomic,assign) CGFloat previousOffsetX;
/**
 当前偏移量
 */
@property (nonatomic,assign) CGFloat contentOffset;
@property (nonatomic,assign) BOOL kvoEnable;
/**
 长按手势返回对应model的相对位置
 @param xPostion 手指在屏幕的位置
 @return 距离手指位置最近的model位置
 */
- (CGPoint)getLongPressModelPostionWithXPostion:(CGFloat)xPostion;
- (void)stockFill;
- (void)calcuteCandleWidth;
- (void)updateWidth;
- (void)drawKLine;
```
## 绘图基本思路
> - 所有的绘制在UIView上面，UIView底层是一个ScrollView
- 设置K线之间的间距与可视区域想要显示的K线个数，动态计算出K线的宽度。之所以没有直接设置K线宽度，是为了保证每次滑动的时候，屏幕内总是占满整数倍的K线个数。
- 通过k线的个数，计算出当前View的与它的父视图ScrollView的宽度
- KVO监听ScrollView的contentOffset属性，计算每次滑动将要显示的K线数组(currentDisplayArray)
- 计算currentDisplayArray的最大值与最小值，然后得出每一根K线对应屏幕的坐标，绘图
## 链接
[ZYWStock][https://github.com/zyw113/ZYWStock.git](https://github.com/zyw113/ZYWStock.git)
- 在使用过程中如果遇到什么问题，欢迎给我简书留言，或者直接Pull request,欢迎star or fork。
- 更多功能，敬请期待~~~
