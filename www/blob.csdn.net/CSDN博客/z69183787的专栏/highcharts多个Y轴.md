# highcharts多个Y轴 - z69183787的专栏 - CSDN博客
2013年12月10日 09:20:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4382
多个Y轴的实现方法在于把yAxis设置成一个数组，里面的一个对象代表一条Y轴，利用opposite:true来表示是否跟默认位置相反，默认Y轴的位置在图形左边，series中通过对每个series设置yAxis来表示使用哪个Y轴，0表示第一个，以此类推。
<script src="http://code.highcharts.com/highcharts.js"></script>
<div id="container" style="height: 400px"></div>
画图：
$(function () {
    $('#container').highcharts({
        xAxis: {
            categories: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
        },
        yAxis:[{
            lineWidth : 1,
            title:{
                text :'y1'
            }
        },{
         title:{
                text :'y2'
            },
            lineWidth : 1,
            opposite:true
        },{
         title:{
                text :'y3'
            },
                  lineWidth : 1,
            opposite:true
        }],
        series: [{
            data: [1,2,3,4,5,6,7,8,9],
            name: 'Right',
            yAxis:0
        }, {
            data: [4,1,5,8,7,10,13,11,11],
            yAxis:1,
            name: 'Center'
        }, {
            data: [9,10,11,12,13,14,15,16,17],
            step: 'left',
            yAxis:2,
            name: 'Left'
        }]
    });
});
图形：![highcharts多个Y轴](http://s16.sinaimg.cn/mw690/6a535991td9fbf37d4e4f&690)
