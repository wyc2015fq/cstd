# 从零开始实现k线图走势图绘制(iOS实战篇) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月16日 16:20:20[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4492
[上篇文章中](http://www.jianshu.com/p/20c8eacd6934)，我们了解了走势图k线图绘制及的一些关键点，基于大家都懂期货股票这些东西的基础上去讲解的，说的也不够全面，这篇文章我们以实战为主，一步一步写出可以商业化的k线图。
# 一、什么是k线图？
1.k线图：有最新价、收盘价、开盘价、最高价、最低价、均价、卖量、买量等等。而我们需要展示给用户看的第一眼就是把它们全部绘制为k线图，直观。还有部分要转化为数值直接推动；
**蜡烛：需要用到收盘价、开盘价、最高价、最低价；红色是涨（国内），开盘价在上，收盘价在下；绿色是相反的；**
**这是一张没有背景图片的蜡烛图  蜡烛demo的实现！ 通过这篇文章，你可以绘制出商业用的各种组合走势图。。**
![](http://upload-images.jianshu.io/upload_images/3493037-59dafbf02f66d137.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**均线：需要用到均价；5均线：比如有100个数据，5均线就是在第4点的时候拿到0-4的平均数绘制在第4点的位置，类推； 还有10 20 30均线等；**
**分时图闪电图：现价；**
**买量卖量持仓量：下面的那些柱子。**
# **二：需求**
1.画一条分时图，具有拉动效果；十字光标，十字光标出现的时候数值的轴对应的分时图数据要显示在上方的详情框里（联动）；缩放效果；横屏效果；（闪电图 分时图 均线都是这样做）
2.画k线图，也就是蜡烛图；具有拉动效果；十字光标，十字光标出现的时候要把对应蜡烛的数据展现在详情款里面（联动）;缩放；横屏；（买量买量持仓量图可以参考这个）
# **三：分析需求**
**1.基本图；**
**2.拉动效果；**
**3.十字光标，联动数据(展示对应的蜡烛 分时)；**
**4.缩放效果；**
**5.横屏效果；**
**6.如果还有什么效果可以做的？；**
# **四：确定框架及API**
**1.我们的数据是以json数组形式推送过来的。所以我们要有一个数据类，数据转model类，然后用这个model去和绘图类做交互及展示; **
当然商业用的不单单那么简单，至少你还得加个统计类统计每分钟的变化趋势添加到这个model类里面转化成为可用model，也就是60s内的直接替换最后一个蜡烛图，60s过了，这个要把它加入这个蜡烛数据（也就是新增一个蜡烛）这里可以匹配每组数据推送过来的时间和数组的最后一个元素的时间差是否大于60s；后台推送过来的数据都是那边已经统计好的每分钟数据；
**我们这边还有一个1 3 5 10 15 30 60 90 120分钟的k线图 这些k线图就是要用这分钟数组去计算得出对应的数组model；**
**2.API: **
**数据类：**请求数据的类，缓存数据或者网络数据；用delegate，当model类被释放数据还没落地的时候也可以及时释放这个model类。
**model类：**填入一个合约名称提交事务，把合约名称交给数据类去获取数据，把获取完成的数据验证解析后转化为模型数组通过block递交对应的请求类（比如给统计类（相当于MVVM的VM））
![](http://upload-images.jianshu.io/upload_images/3493037-04899876e1559f81.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这些都是绘制k线图需要的数据，除此之外还有其它需要展示给外界的数据（买卖价买卖量时间等）
**绘图类：**
demo没有统计类，所以我们直接在vc里面把这些数据通过代理给绘图类；此时我们的k线图可以说是死k，不会有跳动效果；(我们的跳动效果在上文也有解析到，就是通过TCP推送的数据去替换或者新增蜡烛图,优化看后文)
**             到了这个绘图类；本文以k线图为主，不考虑分时图闪电图均线；**k线图我们需要外界提供数据，这时候我们需要用到代理；我们需要向外界获取显示的个数、还有目前显示的范围；也需要告诉外界这个视图即将刷新、以及这个十字光标滑动的时候这个y轴对应model是什么；所以现在我们需要给每一个绘图类做动作规范的protocol为：
![](http://upload-images.jianshu.io/upload_images/3493037-0be813e49d0efd8e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
index会在下文讲解到，indexpoint这个值是告诉外界现在手指到哪个地方了方便调整详情框位置
现在基础的和外界交互的API已经确定下来了；我们可以开始绘图了！
# 五、绘图
我们新建一个KLineView继承于UIView（这样做缩放滑动等效果可以直接操作所以不继承CAShapeLayer）；由上图我们可以知道我们这个绘图的搭配是**CAShapelayer**配合**UIBezierPath；原因、优化的原理和一些关键点看[上一篇文章](http://www.jianshu.com/p/20c8eacd6934);**
**我们定义这个类专属的和外界交互的动作(并不定义改变类变动的动作);**
![](http://upload-images.jianshu.io/upload_images/3493037-4891694f395b6969.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
如图
定义好这些对外操作后，我们开始实现逻辑。
**我们先定义好每一个蜡烛图的宽度 间隔 和每一次的偏移量**
![](http://upload-images.jianshu.io/upload_images/3493037-ed4e8a4b6a5ade41.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**2:建立私有属性(相对私有)记录好每一个数据 颜色 控件**
![](http://upload-images.jianshu.io/upload_images/3493037-2b128090f18aa71f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**3.初始化这些属性**
![](http://upload-images.jianshu.io/upload_images/3493037-550756a141eeb054.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
**- (void)reload:**
获取展示数据的最大值 刷新展示框的width、heighe
重新设置缩放比例x_scale
计算能容纳多少个蜡烛count
//计算现在是从哪一个下标开始取 总个数减去显示的个数
NSInteger index = self.ShowArrayMaxCount - _count;
self.OffsetIndex = index;//初始化偏移位置
**因为插入图片之后变得非常卡没法打字 所以不插入代码了 我会把代码放在GitHub里面; 大家可以提交优化版本!**
## 方法介绍（包含以上除了横屏外的所有效果实现）
> 
- (void)offset_xPoint方法:
如果point.x >0 减去偏移量 显示之前的数据 为0的时候显示self.OffsetIndex开始的数据 否则显示之后的数据。
> 
- (void)CalculationHeightAndLowerFromArray:(NSArray:) ;
//遍历获取显示数据的最高最低值
> 
- (void)CalculationH;
//计算self.h每个点代表的值是多少
> 
- (void)CalculationShowPointFromLastPrices:(NSArray <KlineModel *>*)array
//绘制所有蜡烛图通过代理获取到的model数据的蜡烛图
> 
- (void)replacementLastPoint:(KlineModel *)model
//替换最后一个点 逻辑是删除self.ShapeLayer的最后一个视图 再通过model生成这个位置对应的蜡烛图 添加到父layer
> 
- (CAShapeLayer *)GetShapeLayerFromModel:(KlineModel *)model Index:(NSInteger)idx
//通过model和对应下标生成蜡烛图 添加到父视图中 
> 
- (void)offset_xPoint:(CGPoint)point
//滑动效果 改变OffsetIndex的值 刷新数据
> 
- (void)pinchAction:(UIPinchGestureRecognizer *)pinch
//缩放效果 改变x_scale的值 刷新数据
> 
- (void)panGesture:(UIPanGestureRecognizer *)pan
//通过self.isShowTrackingCross判断是否显示十字光标  否则拉动新的数据
> 
- (void)TrackingCrossFromPoint:(CGPoint)point
//十字光标  通过point逆推得出index 现在的下标/(单元大小)*缩放量
//获取point.x坐标 和这个model的最新价或者其它价格对应的y  这里展示最新价
//把model通过@selector(TrackingCrossIndexModel:IndexPoint:)传递给外界 绘制当前路径
> 
- (void)setShowTrackingCross:(BOOL)ShowTrackingCross
重写set方法判断是否隐藏十字光标
## 横屏效果
在监控到屏幕转动的时候重新设置frame 调用reload就好了。 里面暂时没做任何处理.
[其它细节请参考理论篇](http://www.jianshu.com/p/20c8eacd6934) 请找茬 共同进步！！
demo[链接：https://github.com/454961172/KlineDemo](https://github.com/454961172/KlineDemo)
文／不留名的黄子嘉（简书作者）
原文链接：http://www.jianshu.com/p/f37ab1e73006
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
