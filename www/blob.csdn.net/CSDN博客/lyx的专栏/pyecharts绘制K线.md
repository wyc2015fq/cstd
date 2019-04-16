# pyecharts绘制K线 - lyx的专栏 - CSDN博客





2018年12月23日 20:07:15[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1186
个人分类：[量化投资																[Python](https://blog.csdn.net/qtlyx/article/category/5969093)](https://blog.csdn.net/qtlyx/article/category/6276672)








        最近想扩展一下vnpy，优化一些功能和代码的性能。在看backtesting部分代码的时候，发现，vnpy其实回测功能挺弱的，可以自己扩展一下。随之而来的就是一个回测结果可视化的问题。vnpy原生的回测结果没有绘制k线，所以也就没有指标的可视化和开仓平仓的可视化，只有随后交易结果的可视化。笔者自己其实有点点不习惯，没有看到策略的可视化回测结果，有点点不开心，所以打算自己做一下。首先就是选择可视化的工具，pyecharts应该是一个首选了，而且现在发展的越来越好了。

        那么，首先来尝试一下k线部分pyechats官方的代码吧。

        下面的代码来自官网哦

[http://pyecharts.org](http://pyecharts.org)

```python
from pyecharts import Kline

v1 = [[2320.26, 2320.26, 2287.3, 2362.94], [2300, 2291.3, 2288.26, 2308.38],
      [2295.35, 2346.5, 2295.35, 2345.92], [2347.22, 2358.98, 2337.35, 2363.8],
      [2360.75, 2382.48, 2347.89, 2383.76], [2383.43, 2385.42, 2371.23, 2391.82],
      [2377.41, 2419.02, 2369.57, 2421.15], [2425.92, 2428.15, 2417.58, 2440.38],
      [2411, 2433.13, 2403.3, 2437.42], [2432.68, 2334.48, 2427.7, 2441.73],
      [2430.69, 2418.53, 2394.22, 2433.89], [2416.62, 2432.4, 2414.4, 2443.03],
      [2441.91, 2421.56, 2418.43, 2444.8], [2420.26, 2382.91, 2373.53, 2427.07],
      [2383.49, 2397.18, 2370.61, 2397.94], [2378.82, 2325.95, 2309.17, 2378.82],
      [2322.94, 2314.16, 2308.76, 2330.88], [2320.62, 2325.82, 2315.01, 2338.78],
      [2313.74, 2293.34, 2289.89, 2340.71], [2297.77, 2313.22, 2292.03, 2324.63],
      [2322.32, 2365.59, 2308.92, 2366.16], [2364.54, 2359.51, 2330.86, 2369.65],
      [2332.08, 2273.4, 2259.25, 2333.54], [2274.81, 2326.31, 2270.1, 2328.14],
      [2333.61, 2347.18, 2321.6, 2351.44], [2340.44, 2324.29, 2304.27, 2352.02],
      [2326.42, 2318.61, 2314.59, 2333.67], [2314.68, 2310.59, 2296.58, 2320.96],
      [2309.16, 2286.6, 2264.83, 2333.29], [2282.17, 2263.97, 2253.25, 2286.33],
      [2255.77, 2270.28, 2253.31, 2276.22]]
kline = Kline("K 线图示例")
kline.add("日K", ["2017/7/{}".format(i + 1) for i in range(31)], v1)
kline.render()
```

        首先，k线在pyechats里面定义为Kline类。这里先是写好了k线的数据，v1.我们注意到，数据的结构是一个列表的列表，里面的每一个列表是open close low high，也就是oclh格式。非常符合我们做量化的风格。

        有了数据之后是实例化k线对象，传个图像的名称就可以了。然后就是关键的add方法，其实就是往图像里面插入数据，我们可以看一下add的实现：

```python
def __add(self, name, x_axis, y_axis, **kwargs):
        """

        :param name:
            系列名称，用于 tooltip 的显示，legend 的图例筛选。
        :param x_axis:
            x 坐标轴数据。
        :param y_axis:
            y 坐标轴数据。数据中，每一行是一个『数据项』，每一列属于一个『维度』。
            数据项具体为 [open, close, lowest, highest] （即：[开盘值, 收盘值,
             最低值, 最高值]）。
        :param kwargs:
        """
```

        在Kline类里面，add其实调用了__add，其中，name参数其实就是一个图例，然后是x坐标的数据，y坐标的数据。

        然后就是render方法就可以获得一个html格式的结果，用浏览器打开就可以了。

大概是下面这样：

![](https://img-blog.csdnimg.cn/20181223120424144.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4,size_16,color_FFFFFF,t_70)

图片来自pyecharts官网。

        此外还可以增加一些别的设置，这些设置都是通过add方法中设置一些关键字来完成，比如：

```python
kline.add(
    "日K",
    ["2017/7/{}".format(i + 1) for i in range(31)],
    v1,
    mark_point=["max"],
    is_datazoom_show=True,
)
```

        这里，我们发现多了mark_point和is_datazoom_show的一个设置，其中，mark_point是用来标记处最大值，而is_datazoom_show是用来标记出是否具有伸缩坐标轴功能的。

![](https://img-blog.csdnimg.cn/20181223121134181.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4,size_16,color_FFFFFF,t_70)

        同样的，坐标轴伸缩方向可以通过datazoom_orient来设置：

```python
kline.add(
    "日K",
    ["2017/7/{}".format(i + 1) for i in range(31)],
    v1,
    mark_point=["max"],
    is_datazoom_show=True,
    datazoom_orient="vertical",
)
```

        此外，还可以在图上画一些别的线,比如close价的最大值。

```python
kline.add(
    "日K",
    ["2017/7/{}".format(i + 1) for i in range(31)],
    v1,
    mark_line=["max"],
    mark_line_symbolsize=0,
    datazoom_orient="vertical",
    mark_line_valuedim="close",
)
```

![](https://img-blog.csdnimg.cn/2018122315330242.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4,size_16,color_FFFFFF,t_70)

        那么，接下来我们来看一下如何改进这个k先的绘制方法吧，我们从一个pandas开始。后面的代码就是笔者自己写的哦。

        我们现在有一个pandas, 里面的数据如下：

![](https://img-blog.csdnimg.cn/20181223193701505.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4,size_16,color_FFFFFF,t_70)

        还有一列ma10没有放上去，其实就是5日均线和10日均线。

```python
def backtesting_plot(table_name, indicator_name_list):
    # data preparation
    da = pd.DataFrame(data=table_name)
    da['volume'] = da['volume'].apply(lambda vol: vol if vol > 0 else 0)
    date = da["datetime"].apply(lambda x: str(x)).tolist()
    k_plot_value = da.apply(lambda record: [record['open'], record['close'], record['low'], record['high']], axis=1).tolist()
    
    # K chart
    kline = Kline()
    kline.add("Backtesting Result", date, k_plot_value)
    
    indicator_lines = Line()
    for indicator_name in indicator_name_list:
        indicator_lines.add(indicator_name, date, da[indicator_name].tolist())
    # trading volume bar chart
    bar = Bar()
    bar.add("volume", date, da["volume"],
            tooltip_tragger="axis", is_legend_show=False, is_yaxis_show=False, yaxis_max=5*max(da["volume"]))
    # buy and sell
    v1 = date[10]
    v2 = da['high'].iloc[10]
    es = EffectScatter("buy")
    es.add("buy", [v1], [v2])
    v1 = date[18]
    v2 = da['high'].iloc[18]
    es.add( "sell",  [v1],  [v2], symbol="pin",)
    
    overlap = Overlap()
    overlap.add(kline)
    overlap.add(indicator_lines,)
    overlap.add(bar,yaxis_index=1, is_add_yaxis=True)
    overlap.add(es)
    overlap.render(path='tt.html')
```

        我们看一下上面这个函数，首先我们从pandas中拿出数据，转换成pyecharts能接受的list格式。要提醒大家的是，这里的datetime也要转化成字符串格式。

        然后就是实例化Kline和技术指标的Line

```python
# K chart
    kline = Kline()
    kline.add("Backtesting Result", date, k_plot_value)
    
    indicator_lines = Line()
    for indicator_name in indicator_name_list:
        indicator_lines.add(indicator_name, date, da[indicator_name].tolist())
```

        然后用bar来制作成交量。这样的话基本就形成了。

        但是我们进一步希望能够在k线图上绘制出买卖信号发生的信息，也就是交易发出的时间点，那么我们用es来添加，这里随便使用了10和18天作为一个买卖时间点。

        我们做了这么多的图怎么让他们一起显示出来呢？这里就要用到overlap了，也就是叠加的类。

```python
overlap = Overlap()
    overlap.add(kline)
    overlap.add(indicator_lines,)
    overlap.add(bar,yaxis_index=1, is_add_yaxis=True)
    overlap.add(es)
    overlap.render(path='tt.html')
```

        我们最后来看一下结果怎么样：

![](https://img-blog.csdnimg.cn/20181223200500717.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4,size_16,color_FFFFFF,t_70)



        可以说相当漂亮了。笔者圈起来的就是我们绘制的时候设置的buy和sell两个点。




