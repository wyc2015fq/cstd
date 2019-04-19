# iOS使用Charts框架绘制—饼状图 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月16日 19:15:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：9278
首先先看一下效果:
![](http://upload-images.jianshu.io/upload_images/1803339-af5190b3fd0b1d1e.gif?imageMogr2/auto-orient/strip)
饼状图
#### 一、创建饼状图对象
创建饼状图对象用到类是`PieChartView.h`, 代码如下:
```
self.pieChartView = [[PieChartView alloc] init];
self.pieChartView.backgroundColor = BgColor;
[self.view addSubview:self.pieChartView];
[self.pieChartView mas_makeConstraints:^(MASConstraintMaker *make) {
      make.size.mas_equalTo(CGSizeMake(300, 300));
      make.center.mas_equalTo(self.view);
}];
```
#### 二、设置饼状图外观样式
###### 1. 基本样式
```
[self.pieChartView setExtraOffsetsWithLeft:30 top:0 right:30 bottom:0];//饼状图距离边缘的间隙
self.pieChartView.usePercentValuesEnabled = YES;//是否根据所提供的数据, 将显示数据转换为百分比格式
self.pieChartView.dragDecelerationEnabled = YES;//拖拽饼状图后是否有惯性效果
self.pieChartView.drawSliceTextEnabled = YES;//是否显示区块文本
```
###### 2. 设置饼状图中间的空心样式
空心有两个圆组成, 一个是hole, 一个是transparentCircle, transparentCircle里面是hole, 所以饼状图中间的空心也就是一个同心圆. 代码如下:
```cpp
```cpp
self.pieChartView.drawHoleEnabled = YES;//饼状图是否是空心
self.pieChartView.holeRadiusPercent = 0.5;//空心半径占比
self.pieChartView.holeColor = [UIColor clearColor];//空心颜色
self.pieChartView.transparentCircleRadiusPercent = 0.52;//半透明空心半径占比
self.pieChartView.transparentCircleColor = [UIColor colorWithRed:210/255.0 green:145/255.0 blue:165/255.0 alpha:0.3];//半透明空心的颜色
```
```
###### 3. 设置饼状图中心的文本
当饼状图是空心样式时, 可以在饼状图中心添加文本, 添加文本有两种方法. 一种方法是使用`centerText` 属性添加, 这种方法不能设置字体颜色、大小等. 另一种方法是使用`centerAttributedText`属性添加, 这种方法添加的富文本, 因此就可以对字体进行进一步美化了. 代码如下:
```
if (self.pieChartView.isDrawHoleEnabled == YES) {
        self.pieChartView.drawCenterTextEnabled = YES;//是否显示中间文字
        //普通文本
//        self.pieChartView.centerText = @"饼状图";//中间文字
        //富文本
        NSMutableAttributedString *centerText = [[NSMutableAttributedString alloc] initWithString:@"饼状图"];
        [centerText setAttributes:@{NSFontAttributeName: [UIFont boldSystemFontOfSize:16],
                                    NSForegroundColorAttributeName: [UIColor orangeColor]}
                            range:NSMakeRange(0, centerText.length)];
        self.pieChartView.centerAttributedText = centerText;
}
```
###### 4. 设置饼状图描述
```
self.pieChartView.descriptionText = @"饼状图示例";
self.pieChartView.descriptionFont = [UIFont systemFontOfSize:10];
self.pieChartView.descriptionTextColor = [UIColor grayColor];
```
###### 6. 设置饼状图图例样式
```
self.pieChartView.legend.maxSizePercent = 1;//图例在饼状图中的大小占比, 这会影响图例的宽高
self.pieChartView.legend.formToTextSpace = 5;//文本间隔
self.pieChartView.legend.font = [UIFont systemFontOfSize:10];//字体大小
self.pieChartView.legend.textColor = [UIColor grayColor];//字体颜色
self.pieChartView.legend.position = ChartLegendPositionBelowChartCenter;//图例在饼状图中的位置
self.pieChartView.legend.form = ChartLegendFormCircle;//图示样式: 方形、线条、圆形
self.pieChartView.legend.formSize = 12;//图示大小
```
#### 三、为饼状图提供数据
为饼状图提供数据, 首先需要创建两个数组`yVals`和`xVals`, `yVals`数组存放饼状图每个区块的数据,
`xVals`存放的是每个区块的名称或者描述.
接着需要用`PieChartDataSet.h`类创建`dataSet`对象, 创建时将`yVals`放进去.
然后需要用`PieChartData.h`类创建`data`对象, 创建时将`xVals`和`dataSet`对象放进去.
最后直接把`data`对象赋值给饼状图的`data`属性即可. 创建`data`对象代码如下:
```cpp
```cpp
- (PieChartData *)setData{
    double mult = 100;
    int count = 5;//饼状图总共有几块组成
    //每个区块的数据
    NSMutableArray *yVals = [[NSMutableArray alloc] init];
    for (int i = 0; i < count; i++) {
        double randomVal = arc4random_uniform(mult + 1);
        BarChartDataEntry *entry = [[BarChartDataEntry alloc] initWithValue:randomVal xIndex:i];
        [yVals addObject:entry];
    }
    //每个区块的名称或描述
    NSMutableArray *xVals = [[NSMutableArray alloc] init];
    for (int i = 0; i < count; i++) {
        NSString *title = [NSString stringWithFormat:@"part%d", i+1];
        [xVals addObject:title];
    }
    //dataSet
    PieChartDataSet *dataSet = [[PieChartDataSet alloc] initWithYVals:yVals label:@""];
    dataSet.drawValuesEnabled = YES;//是否绘制显示数据
    NSMutableArray *colors = [[NSMutableArray alloc] init];
    [colors addObjectsFromArray:ChartColorTemplates.vordiplom];
    [colors addObjectsFromArray:ChartColorTemplates.joyful];
    [colors addObjectsFromArray:ChartColorTemplates.colorful];
    [colors addObjectsFromArray:ChartColorTemplates.liberty];
    [colors addObjectsFromArray:ChartColorTemplates.pastel];
    [colors addObject:[UIColor colorWithRed:51/255.f green:181/255.f blue:229/255.f alpha:1.f]];
    dataSet.colors = colors;//区块颜色
    dataSet.sliceSpace = 0;//相邻区块之间的间距
    dataSet.selectionShift = 8;//选中区块时, 放大的半径
    dataSet.xValuePosition = PieChartValuePositionInsideSlice;//名称位置
    dataSet.yValuePosition = PieChartValuePositionOutsideSlice;//数据位置
    //数据与区块之间的用于指示的折线样式
    dataSet.valueLinePart1OffsetPercentage = 0.85;//折线中第一段起始位置相对于区块的偏移量, 数值越大, 折线距离区块越远
    dataSet.valueLinePart1Length = 0.5;//折线中第一段长度占比
    dataSet.valueLinePart2Length = 0.4;//折线中第二段长度最大占比
    dataSet.valueLineWidth = 1;//折线的粗细
    dataSet.valueLineColor = [UIColor brownColor];//折线颜色
    //data
    PieChartData *data = [[PieChartData alloc] initWithXVals:xVals dataSet:dataSet];
    NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
    formatter.numberStyle = NSNumberFormatterPercentStyle;
    formatter.maximumFractionDigits = 0;//小数位数
    formatter.multiplier = @1.f;
    [data setValueFormatter:formatter];//设置显示数据格式
    [data setValueTextColor:[UIColor brownColor]];
    [data setValueFont:[UIFont systemFontOfSize:10]];
    return data;
}
```
```
运行结果如下:
![](http://upload-images.jianshu.io/upload_images/1803339-be08307c9af3ec96.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
运行结果
如果不需要空心样式的饼状图, 可以将饼状图的`drawHoleEnabled`赋值为`NO`, 将中间的文本去掉即可, 代码如下:
`self.pieChartView.drawHoleEnabled = NO;`
效果如下:
![](http://upload-images.jianshu.io/upload_images/1803339-fcbd03ade1b10ec3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
实心饼状图
每个区块之间如果需要间距, 可以通过`dataSet`对象的`sliceSpace`属性设置, 代码如下:
```cpp
```cpp
dataSet.sliceSpace = 3;
```
```
效果如下:
![](http://upload-images.jianshu.io/upload_images/1803339-bdd6d689c258d1b4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
有区块间距的饼状图
*由于属性较多, 其它效果可以自行尝试.*
Demo 地址: [PieChartDemo](https://github.com/GitHubWanglei/PieChartDemo).
文／jianshu_wl（简书作者）
原文链接：http://www.jianshu.com/p/45194d861b21
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
