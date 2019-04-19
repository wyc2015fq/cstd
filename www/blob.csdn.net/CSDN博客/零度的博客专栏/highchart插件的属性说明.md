# highchart插件的属性说明 - 零度的博客专栏 - CSDN博客
2016年04月06日 10:19:02[零度anngle](https://me.csdn.net/zmx729618)阅读数：640
一.chart的部分相关属性说明
renderTo:
 'container',  //图表的页面显示容器(也就是要显示到的div)
defaultSeriesType:
 'line',  //图表类型(line、spline、scatter、splinearea、bar、pie、area、column)
marginRight:
 50,  //上下左右空隙(图表跟图框之间)
marginBottom:
 60,  //下面空隙如果不够大，图例说明有可能看不到
plotBackgroundImage:
 '../graphics/skies.jpg', //(图表的)背景图片
plotBackgroundColor:  //背景颜色
width:
 1000,  //图框（最外层）宽(默认800)
height:
 500,  //图框高(默认500) 
backgroundColor:
 "red"  //图框的背景颜色
borderColor:
 "red"  //图框的边框颜色
borderRadius:
 5,  //图框的圆角大小
borderWidth:
 9,  //图框的边框大小
inverted:
 false,  //(使图)倒置
plotBorderColor:
 "red",  //图表的边框颜色
plotBorderWidth:
 0,  //图表的边框大小
plotShadow:
 false,  //图表是否使用阴影效果
reflow:
 false, 
shadow:true  //图框是否使用阴影
showAxes:
 false,  //是否最初显示轴
spacingTop:
 100,  //图表上方的空白
spacingRight:
 10,
spacingBottom:
 15,
spacingLeft:
 10,
colors:
 [...]  //图上线。。的颜色
二. credits的部分相关属性说明
credits:  //设置右下角的标记。highchart.com
 (这个也可以在highcharts.js里中修改)
{ 
//enabled:
 true,  //是否显示
position:
 {  //显示的位置 
 align:
 'left', 
 x:
 10 
},
text:
 "xoyo.com",  //显示的文字
style:{  //样式
 cursor:
 'pointer',
 color:
 'red',
 fontSize:
 '20px'
},
href:
 'http://www.xoyo.com', //路径
},
三. title的部分相关属性说明
title:
 //标题
{ 
text:
 '月份平均温度',
x:
 -20,  //center
 //水平偏移量 
y:
 100  //y:垂直偏移量
align:
 'right'  //水平方向（left,
 right, bottom, top）
floating:
 true,  //是否浮动显示
margin:
 15,
style:
 ,  //样式
verticalAlign:
 "left"  //垂直方向（left,
 right, bottom, top）
},
四. xAxis或者YAxis的部分相关属性说明
categories:
 ['一月', '二月'],
//allowDecimals:
 false
//alternateGridColor:
 'red'  //在图表中相隔出现纵向的颜色格栅
//dateTimeLabelFormats:
 ,
//endOnTick:
 false,  //是否显示控制轴末端的一个cagegories出来
//events:
 {
//setExtremes:
//},
//gridLineColor:
 "red",  //纵向格线的颜色
//gridLineDashStyle:
 Solid  //纵向格栅线条的类型
//gridLineWidth:
 5,  //纵向格线的的大小
//id:
 null,
//labels:
 {  //X轴的标签（下面的说明）
 //align:
 "center",  //位置
 //enabled:
 false,  //是否显示
 //formatter:
 ,
 //rotation:
 90,  //旋转,效果就是影响标签的显示方向
 //staggerLines:
 4,  //标签的交错显示（上、下）
 //step:
 2,  //标签的相隔显示的步长
 //style:{},
 //x:
 100,  //偏移量，默认两个都是0，
 //y:
 40
//},
//lineColor:
 "red",  //X轴的颜色
//lineWidth:
 5,  //X轴的宽度
//linkedTo:1,
//opposite:
 true  //是否把标签显示到对面
//max:
 12,  //显示的最大值
//maxPadding:
 6,
//maxZoom:
 1,
//min:
 10,  //显示的最小值
//minorGridLineColor:
 'red',  //副格线的颜色
//minorGridLineDashStyle:
 'blod', //副格线的的颜色
//minorGridLineWidth:
 50,  //副格线的宽度
//minorTickColor:
 #A0A0A0,  //???没有看出效果
minorTickInterval:3,  //副标记的间隔
//minorTickLength:
 10,  //副标记的长度
//minorTickPosition:
 'inside',  //副标记的位置
//minorTickWidth:
 5,  //副标记的宽
//minPadding:
 0.01,
//offset:
 0,  //坐标轴跟图的距离
//plotBands:
 //使某数据块显示不同的效果 
//plotLines:
 [{  //标线属性
 //value:
 0,  //值为0的标线
//}], 
//tickmarkPlacement:
 "on",  //标记(文字)显示的位置，on表示在正对位置上。
//reversed:
 true,  //是否倒置
//showFirstLabel:
 false,  //第一个标记的数值是否显示
//startOfWeek:
 2,
//tickColor:
 'blue',  //标记(坐标的记号)的颜色
//tickInterval:
 20,  //标记(坐标的记号)的步长
//tickLength:
 5,
//tickmarkPlacement:
 "on",
//tickPixelInterval:
 1000,  //两坐标之间的宽度
//tickPosition:
 "inside",  //坐标标记的方向
//title:
 {  //设置坐标标题的相关属性
 //margin:
 40,
 //rotation:
 90,
 //text:
 "Y-values",
 //align:
 "middle",
 //enabled:
 "middle",
 //style:
 {color: 'red'}
//},
//type:
 "linear"
五.tooltip的部分相关属性说明
tooltip:
 //提示框设置
{ 
formatter:
 function() {  //格式化提示框的内容样式
return
 '<b>'+ this.series.name +'</b><br/>'+
this.x
 +': '+ this.y +'°C';
}, 
backgroundColor:
 '#CCCCCC',  //背景颜色
//borderColor:
 '#FCFFC5'  //边框颜色
//borderRadius:
 2  //边框的圆角大小
borderWidth:
 3,  //边框宽度(大小)
//enabled:
 false,  //是否显示提示框
//shadow:
 false,  //提示框是否应用阴影  ?没有看出明显效果?????????
//shared:
 true,  //当打开这个属性，鼠标几个某一区域的时候，如果有多条线，所有的线上的据点都会有响应(ipad)
//snap:
 0,  //没有看出明显效果?????????
crosshairs:
 {  //交叉点是否显示的一条纵线
 width:
 2,
 color:
 'gray',
 dashStyle:
 'shortdot'
}
style:
 {  //提示框内容的样式
 color:
 'white',
 padding:
 '10px',  //内边距
 (这个会常用到)
 fontSize:
 '9pt', 
}
},
六.legend(图例说明)的部分相关属性说明
legend:
 //图例说明
{ 
//layout:
 'vertical',  //图例说明布局（垂直显示,默认横向显示）
align:
 'center',  //图例说明的显示位置
//verticalAlign:
 'top', //纵向的位置
//x:
 250,  //偏移量 
//y:
 0, 
borderWidth:
 1,  //边框宽度
//backgroundColor:
 'red'  //背景颜色
borderColor:
 'red',
//borderRadius  //边框圆角
//enabled:
 false  //是否显示图例说明
//floating:true  //是否浮动显示(效果就是会不会显示到图中)
//itemHiddenStyle:
 {color: 'red'},
//itemHoverStyle:
 {color: 'red'}  //鼠标放到某一图例说明上，文字颜色的变化颜色
//itemStyle:  {color:
 'red'}  //图例说明的样式
//itemWidth:  //图例说明的宽度
//labelFormatter:
 function() { return this.value}  //?????????????默认(return
 this.name)
//lineHeight:
 1000  //没看出明显效果
//margin:
 20
//reversed:true  //图例说明的顺序（是否反向）
//shadow:true  //阴影
//style:
 {color:'black'}
//symbolPadding:
 100  //标志（线）跟文字的距离
//symbolWidth:
 100  //标志的宽
//width:100
},
七. plotOptions的部分修改属性说明
plotOptions:
 (我这个是在柱形图上做的测试)
{
  
column:
 //柱形图
{
 //pointPadding:
 0.2,
 //borderWidth:
 1,  //柱子边框的大小
 //borderColor:
 "red",  //柱子边框的颜色
 //borderRadius:
 180,  //柱子两端的圆角的半径
 //colorByPoint:
 true,  //否应该接受每系列的一种颜色或每点一种颜色
 groupPadding:
 0,  //每一组柱子之间的间隔(会影响到柱子的大小)
 //minPointLength:
 0,  //最小数据值那一条柱子的长度(如果是0，可能看不到，可以设置出来)
 //pointPadding:
 0.1,  //柱子之间的间隔(会影响到柱子的大小)
 //pointWidth:
 2,  //柱子的大小(会影响到柱子的大小)
 //allowPointSelect:
 false, 
 //animation:
 true,  //图形出来时候的动画
 //color:
 'red',  //柱子的颜色
 //connectNulls:
 false,  //连接图表是否忽略零点(如线形图，数据为0是是否忽略)
 //cursor:
 '',  //?????????游标
 //dashStyle:
 null,
 dataLabels:
 { //图上是否显示数据标签
//enabled:
 true,
align:
 "center",
//color:
 'red',
formatter:
 function() 
{
 return
 this.y + 'mm'
},
rotation:
 270,
//staggerLines:
 0,
//step:
 ,
//style:
 ,
//x:
 0,
//y:
 -6
 },
 //enableMouseTracking: 
 events:
 {  //事件
click:
 function(event)
{
 alert(this.name);
},
//checkboxClick:
 ,
//hide:
 ,
//legendItemClick:
 ,
//mouseOver:
 ,
//mouseOut:
 ,
//show:
 },
 //id:
 null,
 //lineWidth:
 20,
 //marker:
 {  //图例说明上的标志
//enabled:
 false
 //},
 point:
 {  //图上的数据点(这个在线形图可能就直观)
events:
 {
 click:
 function() 
 {
alert(this.y);
 },
 //mouseOver:
 ,
 //mouseOut:
 ,
 //remove:
 ,
 //select:
 ,
 //unselect:
 ,
 //update:
 }
 },
 //pointStart:
 0,  //显示图数据点开始值
 //pointInterval:
 1,  //显示图数据点的间隔
 //selected:
 false,
 //shadow:
 true,
 //showCheckbox:
 true,  //是否显示(图例说明的)复选框
 //showInLegend:
 false, //是否显示图例说明
 //stacking:
 'percent', //是否堆积???
 states: 
 {
 hover: 
 {
 //brightness:
 0.1,
 enabled:
 true,  //图上的数据点标志是否显示
 //lineWidth:
 2,  //没看出效果
 marker: 
{
 //states:
 ,
 //enabled:
 true,  //数据点标志是否显示
 //fillColor:
 null,  //数据点标志填充的颜色
 //lineColor:
 "#FFFFFF",  //数据点标志线的颜色
 //lineWidth:
 0,  //数据点标志线的大小
 //radius:
 45,  //数据点标志半径
 //symbol:
 'triangle'//'url(http://highcharts.com/demo/gfx/sun.png)' //数据点标志形状(triangle三角形,或者用图片等等)
 }
 }
 },
 //stickyTracking:
 true,  //轨道粘性 (例如线图，如果这个设置为否定，那就必须点到数据点才有反应)
 //visible:
 true,  //设置为false就不显示图
 //zIndex:
 null  //没有看出效果？？？
},
