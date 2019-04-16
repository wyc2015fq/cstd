# jQuery 图表插件 jqChart 使用 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月12日 13:01:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：30
个人分类：[HTML和Javascript](https://blog.csdn.net/leixiaohua1020/article/category/1678189)










jQuery 图表插件 jqChart显示效果效果非常好。支持以下几种图表：

> 
Area

Bar

Bubble

Column

Financial Chart - Candlestick

Financial Chart - Stock

Line

Pie

Radar Area

Radar Line

Radar Spline Area

Radar Spline

Scatter

Spline Area

Spline

Stacked Column

Stacked Bar


可见支持的种类非常之多。在这里我们使用一下Radar Area Chart（雷达图）。使用方式如下：

引入必要的文件，注意jQuery的js文件要放在最前面，版本不一定是1.5.1，比这个版本高也行。实际中根据示例代码修改相应的地方就行。



```java
<link rel="stylesheet" type="text/css" href="../css/jquery.jqChart.css" />
    <link rel="stylesheet" type="text/css" href="../css/jquery.jqRangeSlider.css" />
    <link rel="stylesheet" type="text/css" href="../themes/smoothness/jquery-ui-1.8.21.css" />
    <script src="../js/jquery-1.5.1.min.js" type="text/javascript"></script>
    <script src="../js/jquery.mousewheel.js" type="text/javascript"></script>
    <script src="../js/jquery.jqChart.min.js" type="text/javascript"></script>
    <script src="../js/jquery.jqRangeSlider.min.js" type="text/javascript"></script>
    <!--[if IE]><script lang="javascript" type="text/javascript" src="../js/excanvas.js"></script><![endif]-->
    
    <script lang="javascript" type="text/javascript">
        $(document).ready(function () {

            var background = {
                type: 'linearGradient',
                x0: 0,
                y0: 0,
                x1: 0,
                y1: 1,
                colorStops: [{ offset: 0, color: '#d2e6c9' },
                             { offset: 1, color: 'white'}]
            };

            $('#jqChart').jqChart({
                title: { text: 'Radar Area Chart' },
                border: { strokeStyle: '#6ba851' },
                background: background,
                axes: [
                        {
                            type: 'categoryAngle',
                            categories: ['France', 'Canada', 'Germany', 'USA', 'Italy', 'Spain', 'Russia', 'Sweden', 'Japan']
                        },
                        {
                            type: 'linearRadius',
                            renderStyle: 'polygon',
							lineWidth: '5',
							minimum : 0,
							maximum : 100,
							interval : 20,

							
                            majorTickMarks: { visible: true }
                        }
                      ],
                series: [
                            {
                                title : 'Series 1',
                                type: 'radarArea',
                                data: [65.62, 75.54, 60.45, 34.73, 85.42, 55.9, 63.6, 55.1, 77.2],
                                fillStyle: 'rgba(65,140,240,0.75)'
                            },
                            {
                                title: 'Series 2',
                                type: 'radarArea',
                                data: [76.45, 23.78, 86.45, 30.76, 23.79, 35.67, 89.56, 67.45, 38.98],
                                fillStyle: 'rgba(252,180,65,0.75)'
                            }
                        ]
            });
        });
    </script>
```


 需要显示图表的地方，只需使用一个<div>即可：





```
<div id="jqChart" style="width: 500px; height: 300px;">
        </div>
```


 显示结果如图所示：



![](https://img-blog.csdn.net/20131012125854890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

jqChart官方网站：[http://www.jqchart.com/Default.aspx](http://www.jqchart.com/Default.aspx)

jqChart下载：[http://download.csdn.net/detail/leixiaohua1020/6377505](http://download.csdn.net/detail/leixiaohua1020/6377505)








