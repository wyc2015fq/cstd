# HighCharts 非规律日期 多条曲线的 绘画 - z69183787的专栏 - CSDN博客
2013年03月08日 16:25:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7671
项目中需要为A，B 2个元素 绘出统计值的曲线，但A与B 的 时间点 并不一致，查找HighCharts API后 ，发现了解决方法
```java
$(function () {
    var chart = new Highcharts.Chart({
    
        chart: {
            renderTo: 'container'
        },
    
        xAxis: {
            type: 'datetime',
             labels: {
                formatter: function() {
                       
                               return  Highcharts.dateFormat('%Y-%m-%d', this.value);
                },
                
                }
        },
         tooltip: {  
             shared : true,
            xDateFormat: '%Y-%m-%d'//鼠标移动到趋势线上时显示的日期格式  
        },  
        series: [{
            name: 'A',
            data: [
                [Date.UTC(2010, 0, 1), 29.9],
                [Date.UTC(2010, 0, 2), 71.5],
                [Date.UTC(2010, 0, 3), 106.4],
                [Date.UTC(2010, 0, 6), 129.2],
                [Date.UTC(2010, 0, 7), 144.0],
                [Date.UTC(2010, 0, 8), 176.0]
             ]
        },
                 {     name: 'B',
            data: [
                [Date.UTC(2010, 0, 1), 29.9],
                [Date.UTC(2010, 0, 3), 71.5],
                [Date.UTC(2010, 0, 7), 106.4],
                [Date.UTC(2010, 0, 10), 129.2],
                [Date.UTC(2010, 0, 24), 144.0],
                [Date.UTC(2010, 0, 30), 176.0]
             ]
        }]
    
    });
});
```
![](https://img-my.csdn.net/uploads/201303/08/1362731393_5537.jpg)
[http://jsfiddle.net/GxYM3/](http://jsfiddle.net/GxYM3/)
可以看到X轴上的时间点与 A，B 两条折现的 数据点并不对应。
