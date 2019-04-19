# Swift-Charts 多种样式图表-详细 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月22日 16:50:32[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：9447
> 
##### 我是前言: 大约几个月前我在简书写了一篇文章, 文中简单地介绍了Charts两种图表的样式的使用, 不过有种意犹未尽的感觉, 利用周末的空闲时间再次看了看, 有了新的收获, 今天发出来,分享给大家, 来稍加详细的再次走进Charts的世界. 完全纯代码.
说在前面的话： 这篇的篇幅确实是有些长, 不过你可以会一些图表的tips, 还能熟悉Swift语言的语法. #感觉不错请点个赞# 摸摸哒(冷漠脸.png), 如果还想看更多关于Swift和iOS开发相关的知识, 请关注我.
###### 经过测试:
1.1使用`cocoapods`集成(只能在`iOS9.3`运行), 有知道原因的请在下方评论
1.2 这篇文章的例子我是使用手动加入的framework进行测试的（`iOS8.0`之后）. 如果你不清楚如何加入三方框架请点击看看这篇文章[http://www.jianshu.com/p/fd91c10c9f55](http://www.jianshu.com/p/fd91c10c9f55)
###### (注: 如果模拟器能运行, 真机不能运行, 一定看这篇)
这篇你可以看到下面这些
- LineChartView ----- 折线
- BarChartView ------ 柱状
- CandleStickChartView-----K线
- PieChartView -----饼状
- RadarChartView ----- 雷达
还有这些你看不到0.-, 用法大同小异.
![](http://upload-images.jianshu.io/upload_images/1506501-b69b0ad81fc95c59.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这样
![](http://upload-images.jianshu.io/upload_images/1506501-10598a75f8a9caba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这样
![](http://upload-images.jianshu.io/upload_images/1506501-8571059cbb76cd96.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这样
##### 折线图 - `LineChartView`
![](http://upload-images.jianshu.io/upload_images/1506501-36433aa0fd57d610.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
折线
```
// 创建-
func createLineChartView()
    {
        chartView = LineChartView.init(frame: CGRectMake(0, 64, s_w, s_h - 64))
        chartView.lineData
        // 签协议
        chartView.delegate = self
        chartView.backgroundColor = UIColor.whiteColor()
        self.view.addSubview(chartView)
    }
```
```
// 加上模拟数据
func setChart(dataPoints: [String], values: [Double]) {
        var dataEntries: [ChartDataEntry] = []
        for i in 0..<dataPoints.count {
            let dataEntry = ChartDataEntry(value: values[i], xIndex: i)
            dataEntries.append(dataEntry)
        }
        let chartDataSet = LineChartDataSet(yVals: dataEntries, label: "Units Sold")
        let chartData = LineChartData(xVals: months, dataSet: chartDataSet)
        // 加上一个界限, 演示图中红色的线
        let jx = ChartLimitLine(limit: 12.0, label: "I am LimitLine")
        chartView.rightAxis.addLimitLine(jx)
        chartView.data = chartData
        // 自定义颜色
        // colors 是一个数组, 可以给相应的颜色
        chartDataSet.colors = [UIColor.redColor()]
        chartDataSet.colors = ChartColorTemplates.colorful()
        chartView.animate(yAxisDuration: 1.0, easingOption: .EaseInBounce)
    }
```
##### 柱状图- `BarChartView`
![](http://upload-images.jianshu.io/upload_images/1506501-095d70c213550e84.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
柱状
```
func createLineChartView()
    {
        chartView = BarChartView.init(frame: CGRectMake(0, 64, s_w, s_h - 64))
        chartView.barData
        // 签协议
        chartView.delegate = self
        chartView.backgroundColor = UIColor.whiteColor()
        self.view.addSubview(chartView)
    }
```
```
func setChart(dataPoints: [String], values: [Double]) {
        var dataEntries: [BarChartDataEntry] = []
        for i in 0..<dataPoints.count {
            let dataEntry = BarChartDataEntry(value: values[i], xIndex: i)
            dataEntries.append(dataEntry)
        }
        let chartDataSet = BarChartDataSet(yVals: dataEntries, label: "Units Sold")
        let chartData = BarChartData(xVals: months, dataSet: chartDataSet)
        // 加上一个界限, 演示图中红色的线
        let jx = ChartLimitLine(limit: 12.0, label: "I am LimitLine")
        chartView.rightAxis.addLimitLine(jx)
        chartView.data = chartData
        // 自定义颜色
        // 例子中有十二个柱状图
        // colors 是一个数组, 可以给相应的颜色
        chartDataSet.colors = [UIColor.blueColor(), UIColor.redColor(), UIColor.cyanColor(), UIColor.greenColor(), UIColor.brownColor(), UIColor.purpleColor()]
        // API 自带颜色模板
//        chartDataSet.colors = ChartColorTemplates.liberty()
        chartView.animate(yAxisDuration: 1.0, easingOption: .EaseInBounce)
    }
```
##### K线图- `CandleStickChartView`
![](http://upload-images.jianshu.io/upload_images/1506501-b59a5788b7d6862d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
K线
```cpp
```cpp
chartView = CandleStickChartView.init(frame: CGRectMake(0, 64, s_w, s_h - 64))
            // 签协议
            chartView.delegate = self
            chartView.backgroundColor = UIColor.whiteColor()
            // 画板颜色
            chartView.gridBackgroundColor = UIColor.clearColor()
            //
            chartView.borderColor = UIColor.whiteColor()
            self.view.addSubview(chartView)
            // 横轴数据
            var xValues = [String]()
            for i in 0...25 {
                xValues.append(NSString(format: "%d", i + 1993) as String)
            }
            // 初始化CandleChartDataEntry数组
            var yValues = [CandleChartDataEntry]()
            // 产生20个随机立柱数据
            for j in 0...24 {
                let val = (Double)(arc4random_uniform(40))
                let high = (Double)(arc4random_uniform(9)) + 8.0
                let low = (Double)(arc4random_uniform(9)) + 8.0
                let open = (Double)(arc4random_uniform(6)) + 1.0
                let close = (Double)(arc4random_uniform(6)) + 1.0
                let even = j % 2 == 0
                yValues.append(CandleChartDataEntry.init(xIndex: j, shadowH: val + high, shadowL: val - low, open: even ? val + open : val - open, close: even ? val - close : val + close))
            }
            let set1 = CandleChartDataSet.init(yVals: yValues, label: "data set")
            // defult left
            // set1.axisDependency
            // data set color
            set1.setColor(UIColor.blueColor())
            set1.shadowColor = UIColor ( red: 0.5536, green: 0.5528, blue: 0.0016, alpha: 1.0 )
            // 立线的宽度
            set1.shadowWidth = 0.7
            // close >= open
            set1.decreasingColor = UIColor.redColor()
            // 内部是否充满颜色
            set1.decreasingFilled = true
            // open > close
            set1.increasingColor = UIColor ( red: 0.0006, green: 0.2288, blue: 0.001, alpha: 1.0 )
            // 内部是否充满颜色
            set1.increasingFilled = true
            // 赋值数据
            let data = CandleChartData(xVals: xValues, dataSet: set1)
            chartView.data = data
```
```
##### 饼状图- `PieChartView`
![](http://upload-images.jianshu.io/upload_images/1506501-2f070e448178bbb3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
饼状
```cpp
```cpp
pieChartView = PieChartView.init(frame: CGRectMake(0, 0, s_w, s_h))
        pieChartView.delegate = self;
        // 可以调整大小, 位置
//        pieChartView.setExtraOffsets(left: 20.0, top: 0.0, right: 20.0, bottom: 0.0)
        self.view.addSubview(pieChartView)
        var yValues = [BarChartDataEntry]()
        // 最好从0 开始. 否则第一个将失去点击效果, 并出现bug...
        for i in 0...5 {
            // 占比数据
            yValues.append(BarChartDataEntry.init(value: (Double)(arc4random_uniform(5)) + 2.0, xIndex: i))
        }
        var xValues = [String]()
        for j in 0...5 {
            // 描述文字
            xValues.append(NSString(format: "%d\\\\("夏天然后")", j + 5) as String)
        }
        //
        let dataSet: PieChartDataSet = PieChartDataSet.init(yVals: yValues, label: "data Set");
        // 空隙
        dataSet.sliceSpace = 5.0
        var colors = [UIColor]()
        colors.append(UIColor ( red: 0.8185, green: 0.8172, blue: 0.0023, alpha: 1.0 ))
        colors.append(UIColor ( red: 0.0, green: 0.81, blue: 0.81, alpha: 1.0 ))
        colors.append(UIColor.greenColor())
        colors.append(UIColor.grayColor())
        colors.append(UIColor.purpleColor())
        colors.append(UIColor.blueColor())
        dataSet.colors = colors
        // 如果你需要指示文字在外部标注百分比, 你需要这样.
        dataSet.valueLinePart1OffsetPercentage = 0.8;
        dataSet.valueLinePart1Length = 0.2;
        dataSet.valueLinePart2Length = 0.4;
        dataSet.yValuePosition = .OutsideSlice
        let data = PieChartData(xVals: xValues, dataSet: dataSet)
        let formatter = NSNumberFormatter.init()
        formatter.maximumFractionDigits = 1
        formatter.numberStyle = NSNumberFormatterStyle.PercentStyle
        formatter.multiplier = 1.0
        formatter.percentSymbol = " %"
        data.setValueFormatter(formatter)
        data.setValueTextColor(UIColor.blackColor())
        pieChartView.data = data
```
```
##### 雷达图- `RadarChartView`
![](http://upload-images.jianshu.io/upload_images/1506501-c8d21cfb33187bfd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
雷达
```cpp
```cpp
radarBarChartView = RadarChartView.init(frame: CGRectMake(0, 0, s_w, s_h))
        // 描述文字
        radarBarChartView.descriptionText = "I am destext"
        // 指向定点线的宽度
        radarBarChartView.webLineWidth = 2.0
        radarBarChartView.innerWebColor = UIColor.blackColor()
        radarBarChartView.innerWebLineWidth = 0.75
        radarBarChartView.webAlpha = 1.0
        self.view.backgroundColor = UIColor.whiteColor()
        self.view.addSubview(radarBarChartView)
        var yValues1 = [ChartDataEntry]()
        var yValues2 = [ChartDataEntry]()
        var xValues = [String]()
        for i in 0...8 {
            //
            yValues1.append(ChartDataEntry(value: (Double)(arc4random_uniform(5)), xIndex: i))
            yValues2.append(ChartDataEntry(value: (Double)(arc4random_uniform(5)), xIndex: i))
            //
            xValues.append(NSString(format: "%d\\\\("夏天然后")", i + 5) as String)
        }
        let dataSet = RadarChartDataSet.init(yVals: yValues1, label: "data Set")
        dataSet.drawFilledEnabled = true
        dataSet.setColor(UIColor ( red: 0.7913, green: 0.9949, blue: 0.4941, alpha: 1.0 ))
        dataSet.fillColor = UIColor ( red: 0.7913, green: 0.9949, blue: 0.4941, alpha: 1.0 )
        dataSet.lineWidth = 1.0
        let dataSet1 = RadarChartDataSet.init(yVals: yValues2, label: "data Set2")
        dataSet1.drawFilledEnabled = true
        dataSet1.setColor(UIColor ( red: 0.4702, green: 1.0, blue: 0.9962, alpha: 1.0 ))
        dataSet1.fillColor = UIColor ( red: 0.4702, green: 1.0, blue: 0.9962, alpha: 1.0 )
        dataSet1.lineWidth = 1.5
        let data = RadarChartData.init(xVals: xValues, dataSets: [dataSet, dataSet1])
        radarBarChartView.data = data
```
```
##### 相对图表进行操作, 例如点击 请使用`ChartViewDelegate`
```
optional public func chartValueSelected(chartView: Charts.ChartViewBase, entry: Charts.ChartDataEntry, dataSetIndex: Int, highlight: Charts.ChartHighlight)
optional public func chartValueNothingSelected(chartView: Charts.ChartViewBase)
optional public func chartScaled(chartView: Charts.ChartViewBase, scaleX: CGFloat, scaleY: CGFloat)
optional public func chartTranslated(chartView: Charts.ChartViewBase, dX: CGFloat, dY: CGFloat)
```
毕竟是将近10000Star的库, 介绍的有限, 如果你还感兴趣, 请仔细阅读API吧.
[iOS 实现第三方登录(图文)](http://www.jianshu.com/p/b8b0e085b074)
[iOS 走进Facebook POP的世界 -- 简书](http://www.jianshu.com/p/0bc2127692e5)
[Swift版本仿网易云音乐播放音乐动画效果-- 简书](http://www.jianshu.com/p/23b0fa948bcf)
[三分钟教你把代码托管到Github -- 简书](http://www.jianshu.com/p/1dab64e7c9ba)
[Swift 很强大的图表库-Charts使用 -- 简书](http://www.jianshu.com/p/0e025931a49a)
[Swift版仿简书App淘宝App很友好弹出view效果 -- 简书](http://www.jianshu.com/p/01a420681ca9)
[iOS UICollectionView高级用法“长按自由移动cell重排 -- 简书](http://www.jianshu.com/p/31d07bf32d62)
[iOS仿微信App添加带箭头的view -- 简书](http://www.jianshu.com/p/260ca284fda0)
[iOS仿美团外卖饿了吗App点餐动画 -- 简书](http://www.jianshu.com/p/d0ae176bc40b)
[你要的Swift资料 -- 简书](http://www.jianshu.com/p/4a1978845eac)
#### 微博@夏天是个大人了欢迎你关注我你还可以加入我创建技术交流群: 498143780 与我交流. 欢迎你加入
文／夏天然后（简书作者）
原文链接：http://www.jianshu.com/p/7ea8f2b99abe
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
