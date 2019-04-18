# Highcharts绘制饼图 - z69183787的专栏 - CSDN博客
2014年02月12日 15:37:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2997
1、使用Highcharts制作饼图的效果图如下：
![](https://img-my.csdn.net/uploads/201209/20/1348129572_3775.png)
2、对应的JavaScript代码---pie-chart.js如下：
**[javascript]**[view
 plain](http://blog.csdn.net/wl_ldy/article/details/8000707#)[copy](http://blog.csdn.net/wl_ldy/article/details/8000707#)
- $(function () {  
- var chart;  
- var totalMoney=50000  
-     $(document).ready(function() {  
-         chart = new Highcharts.Chart({  
-             chart: {  
-                 renderTo: 'pie_chart',  
-                 plotBackgroundColor: 'white',//背景颜色
-                 plotBorderWidth: 0,  
-                 plotShadow: false
-             },  
-             title: {  
-                 text: 'Total:$'+totalMoney,  
-                 verticalAlign:'bottom',  
-                 y:-60  
-             },  
-             tooltip: {//鼠标移动到每个饼图显示的内容
-                 pointFormat: '{point.name}: <b>{point.percentage}%</b>',  
-                 percentageDecimals: 1,  
-                 formatter: function() {  
- returnthis.point.name+':$'+totalMoney*this.point.percentage/100;  
-                 }  
-             },  
-             plotOptions: {  
-                 pie: {  
-                     size:'60%',  
-                     borderWidth: 0,  
-                     allowPointSelect: true,  
-                     cursor: 'pointer',  
-                     dataLabels: {  
-                     enabled: true,  
-                     color: '#000',                        
-                     distance: -50,//通过设置这个属性，将每个小饼图的显示名称和每个饼图重叠
-                     style: {                              
-                         fontSize: '10px',  
-                         lineHeight: '10px'
-                     },  
-                     formatter: function(index) {      
- return'<span style="color:#00008B;font-weight:bold">' + this.point.name + '</span>';  
-                        }  
-                   },  
-                  padding:20  
-                 }  
-             },  
-             series: [{//设置每小个饼图的颜色、名称、百分比
-                 type: 'pie',  
-                 name: null,  
-                 data: [  
-                     {name:'Base salary',color:'#3DA9FF',y:65},  
-                     {name:'Health insurance',color:'#008FE0',y:20},  
-                     {name:'Company matched',color:'#00639B',y:10},  
-                     {name:'Others',color:'#CBECFF',y:5}  
-                 ]  
-             }]  
-         });  
-     });  
- 
- });  
- 
