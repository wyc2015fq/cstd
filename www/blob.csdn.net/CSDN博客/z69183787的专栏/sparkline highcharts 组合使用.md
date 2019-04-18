# sparkline highcharts 组合使用 - z69183787的专栏 - CSDN博客
2015年01月27日 13:22:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3670
highcharts插件js：
```java
/**
 * Created by yaung on 2014/12/20.
 */
(function($){
    $.extend({
        getStartAngle : function (value,control) {
            if(value == control){
                return 90;
            }else if(value < control){
                return 360 - ($.getPercent(value,control)*2.7).toFixed(0);
            }else{
                return (90 - ($.getPercent(value-control,control) * 2.7 ).toFixed(0))<0?0:
                    90 - ($.getPercent(value-control,control) * 2.7 ).toFixed(0);
            }
        },
        getEndAngle : function (value,control) {
            return 360;
        },
        getPercent : function (value,control){
            if(value == control){
                return 100;
            }else{
                return control != 0 ? (value/control*100).toFixed(1):0
            }
        }
    });
    $.fn.extend({
        highchart_pie : function(options,callback) {
            var defaults = {
                pieArray: [], control: 0,
                width: '', height: '' ,unit:''
            };
            var opts = $.extend({}, defaults, options);
            var seriesArray = [];
            $.each(opts.pieArray, function (i,o) {
                seriesArray.push(
                    {
                        type: 'pie',
                        data: [
                            {y: o.y, name: o.name, color: o.color}
                        ],
                        borderColor: 'white',
                        borderWidth: 0,
                        innerSize: o.innerSize,
                        size: o.size,
                        startAngle: $.getStartAngle(o.y, opts.control),
                        endAngle: $.getEndAngle(o.y, opts.control),
                        dataLabels: {
                            enable : false,
                            //x: o.xPos,
                           // y: o.yPos,
                            useHTML: true,
                            formatter: function () {
                                return '';
                            },
                            connectorWidth: 0,
                            color: o.color,
                            style: {"fontSize": "13px", "fontWeight": "bold"}
                        }
                    }
                )
            });
            this.highcharts({
                chart: {
                    plotBackgroundColor: null,
                    backgroundColor: null,
                    plotBorderWidth: 0,
                    plotShadow: false,
                    width: opts.width,
                    height: opts.height,
                    marginLeft: -35
                },
                credits: {
                    enabled: false
                },
                title: {
                    text: null
                },
                tooltip: {
                    //pointFormat: '{point.name}: <b>{point.y}</b>' + opts.unit
                    pointFormat: '<b>{point.y}</b>' + opts.unit
                },
                series: seriesArray
            }, function (chart) { // on complete
//'<img src="smile.png" ' 'style="display:inline";/><br>
                $.each(opts.pieArray, function (i,o) {
                    var value = o.y/10000>=1 ? (o.y/10000).toFixed(2)+o.unit.replace('万','亿'): o.y.toFixed(0) + o.unit;
                    var temp = (o.overPlan?($.getPercent(o.y, opts.control)>100?'超计划'+($.getPercent(o.y, opts.control)-100).toFixed(1)+'%<br>':''):'');
                    chart.renderer.text(
                        temp+o.phtml+value,
                        o.xPos, o.yPos)
                        .css({
                            color: o.color,
                            fontSize: '12px',
                            fontWeight: 'bold',
                            zIndex:9999
                        })
                        .add();
                    if(temp.length > 0){
                        chart.renderer.image('smile.png', 115, 95, 20, 20)
                            .add();
                    }
                });
            })
        }
    });
})(jQuery);
```
调用实例：
```java
$("#metroDistanceTotalChart").highchart_pie({
                    width : 290, height : 300,control : obj.metroDistanceControlYear, unit:'万车公里',
                    pieArray:[
                        new pieObject(obj.metroDistanceLastYear,lastYear+'年<br>年度客流量','#1874CD','85%','80%',205,235,'万车公里',false,
                                lastYear+'年<br>年度客流量<br>'),
                        new pieObject(obj.metroDistanceControlYear,thisYear+'年<br>年度管控值','#87CEFF','80%','60%',205,40,'万车公里',false,
                                thisYear+'年<br>年度管控值<br>'),
                        new pieObject(obj.metroDistancePlanMonth,thisYear+'年<br>月度计划累计值','#FFD700','80%','60%',20,290,'万车公里',false,
                                thisYear+'年月度计划累计值'),
                        new pieObject(obj.metroDistanceYear,thisYear+'年<br>年累计值','#FF4040','80%','60%',90,130,'万车公里',true,
                                thisYear+'年<br>已完成'+ $.getPercent(obj.metroDistanceYear,obj.metroDistanceControlYear)+'%<br>年累计值<br>')
                    ]
                },null)
```
pie对象：
```java
//构造饼图
function pieObject(y,name,color,size,innerSize,xPos,yPos,unit,overPlan,phtml){
    this.y = y;
    this.name = name;
    this.color = color;
    this.size = size;
    this.innerSize = innerSize;
    this.xPos = xPos;
    this.yPos = yPos;
    this.unit = unit;
    this.overPlan = overPlan;
    this.phtml = phtml;
}
```
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
sparkline插件
```java
/**
 * Created by yaung on 2014/12/20.
 */
(function ($) {
    Array.prototype.clone=function(){ return this.slice(0); }
    $.extend({
        getMax : function(arr1,arr2){
            var copy = arr1.clone();
            Array.prototype.push.apply(copy,arr2);
            return Math.max.apply(null,copy);
        },
        getMin : function(arr1,arr2){
            var copy = arr1.clone();
            Array.prototype.push.apply(copy,arr2);
            return Math.min.apply(null,copy);
        }
    });
    $.fn.extend({
        //管控线形图
        control_line : function (opts) {
            this.sparkline(opts.control,
                {type: 'line',
                    barWidth: 7,
                    barSpacing: 3,
                    width: opts.width, height: opts.height,
                    tooltipSuffix:opts.suffix,
                    composite: true,
                    tooltipFormat: '<span style="display:inline;color: {{color}}">●</span> '+opts.controlName+'： {{prefix}}{{y}}{{suffix}}',
                    fillColor: false,
                    lineColor: '#FFA042',
                    highlightLineColor: 'red',
                    chartRangeMin: $.getMin(opts.data,opts.control) - 0.1,
                    chartRangeMax: $.getMax(opts.data,opts.control) - 0.1
                });
        },
        //线形图
        spark_line: function (options, callback) {
            var defaults = {
                data : [0],control : [0],
                width : '',height : '',title: '',
                suffix : '',composite : '',controlName:'管控值'
            };
            var opts = $.extend({}, defaults, options);
            this.html(opts.data.join(","));
            this.sparkline('html',
                { type: 'line',
                    width: opts.width, height: opts.height,
                    lineWidth: 1,
                    spotRadius: 2,
                    spotColor: false,
                    tooltipChartTitle: '<span style="display:inline;">   </span>' + opts.title,
                    tooltipSuffix: opts.suffix,
                    tooltipFormat: '<span style="display:inline;color: {{color}}">●</span> 实际值： {{prefix}}{{y}}{{suffix}}',
                    fillColor: false,
                    lineColor: '#84C1FF',
                    chartRangeMin: $.getMin(opts.data,opts.control) - 0.1,
                    chartRangeMax: $.getMax(opts.data,opts.control) + 0.1
                });
            //代表有2张图
            if(opts.composite){
                this.control_line(opts);
            }
        },
        //柱状图
        spark_bar : function(options, callback){
            var defaults = {
                data : [0],control : [0],
                width : '',height : '',title: '',
                suffix : '',composite : '',controlName:'管控值'
            };
            var opts = $.extend({}, defaults, options);
            this.html(opts.data.join(","));
            this.sparkline('html',
                { type: 'bar',
                    width: opts.width, height: opts.height,
                    barColor: '#0072E3',
                    barWidth: 7,
                    barSpacing: 3,
                    tooltipChartTitle: '<span style="display:inline;">   </span>' + opts.title,
                    tooltipSuffix: opts.suffix,
                    tooltipFormat: '<span style="display:inline;color: {{color}}">●</span> 实际值： {{prefix}}{{value}}{{suffix}}',
                    chartRangeMin: $.getMin(opts.data,opts.control) - 0.1,
                    chartRangeMax: $.getMax(opts.data,opts.control) + 0.1
                });
            //代表有2张图
            if(opts.composite){
                this.control_line(opts);
            }
        },
        //饼图
        spark_pie : function (options,callback) {
            var defaults = {
                data : [0],control : [0],
                width : '',height : '',title: '',
                suffix : '',offset : ''
            };
            var opts = $.extend({}, defaults, options);
            this.html(opts.data);
            this.sparkline('html',
                {type:'pie',
                    height: opts.height,
                    offset:'-90',
                    tooltipFormat: '  <span style="display:inline;color: {{color}}">●</span> 编组数：{{value}}',
                    tooltipChartTitle:'<span style="display:inline;">   </span>'+opts.title
                });
        }
    })
})(jQuery);
```
调用实例：
```java
$("#onTimeChart").spark_line(
                    {
                        data : obj.onTimeList,
                        control : obj.onTimeControlList,
                        suffix : '%',
                        title : '正点率',
                        composite : true,
                        width : '150',
                        height : '40'
                    },null)
```
```java
$("#useMetroChart").spark_bar(
                {
                    data : obj.useMetroList,
                    suffix : '列',
                    title : '运用列车数',
                    composite : false,
                    width : '150',
                    height : '40'
                },null)
```
效果图：
![](https://img-blog.csdn.net/20150127132227562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20150127132235765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
