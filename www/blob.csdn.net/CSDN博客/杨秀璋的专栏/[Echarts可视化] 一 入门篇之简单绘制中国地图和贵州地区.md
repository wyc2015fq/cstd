# [Echarts可视化] 一.入门篇之简单绘制中国地图和贵州地区 - 杨秀璋的专栏 - CSDN博客





2017年04月09日 23:53:21[Eastmount](https://me.csdn.net/Eastmount)阅读数：11042
所属专栏：[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)









        最近发生了很多事情，去到了一个新环境学习。但是不论在哪里，我都需要不忘初心，坚持做自己喜欢的事情，在CSDN写博客、教学、爱娜、生活等等。
        这篇文章主要是通过Echarts可视化介绍入门知识、中国地图和贵州地区各省份的数据分析。其中贵州地图才是这篇文章的核心内容。这是一篇入门文章，希望对您有所帮助，如果文章中存在不足之处，还请海涵~
        官网地址：[http://echarts.baidu.com/index.html](http://echarts.baidu.com/index.html)






## 一. 入门介绍-第一张图

        强烈推荐大家阅读官网的教程进行学习，这里的入门就参考官网内容了。
        官网地址：[http://echarts.baidu.com/tutorial.html](http://echarts.baidu.com/tutorial.html)

**ECharts 特性介绍**
        ECharts，一个纯 Javascript 的图表库，可以流畅的运行在 PC 和移动设备上，兼容当前绝大部分浏览器（IE8/9/10/11，Chrome，Firefox，Safari等），底层依赖轻量级的 Canvas 类库 ZRender，提供直观，生动，可交互，可高度个性化定制的数据可视化图表。

        ECharts 提供了常规的折线图，柱状图，散点图，饼图，K线图，用于统计的盒形图，用于地理数据可视化的地图，热力图，线图，用于关系数据可视化的关系图，treemap，多维数据可视化的平行坐标，还有用于 BI 的漏斗图，仪表盘，并且支持图与图之间的混搭。

        ECharts 3 中更是加入了更多丰富的交互功能以及更多的可视化效果，并且对移动端做了深度的优化。ECharts 3 开始独立出了“坐标系”的概念，支持了直角坐标系（catesian，同 grid）、极坐标系（polar）、地理坐标系（geo）。图表可以跨坐标系存在，例如折、柱、散点等图可以放在直角坐标系上，也可以放在极坐标系上，甚至可以放在地理坐标系中。

**Echarts入门代码**ECharts 3 开始不再强制使用 AMD 的方式按需引入，代码里也不再内置 AMD 加载器。因此引入方式简单了很多，只需要像普通的 JavaScript 库一样用 script 标签引入。
        1.首先下载最新的 echarts.min.js 文件
        下载地址：[http://echarts.baidu.com/download.html](http://echarts.baidu.com/download.html)



![](https://img-blog.csdn.net/20170409230458171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



        2.在Script中调用该文件，核心代码格式如下：


```
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <!-- 引入 ECharts 文件 -->
    <script src="echarts.min.js"></script>
</head>
</html>
```
        3.绘制图形
        下面代码参考我的文章：[Echarts字体和线条颜色设置操作笔记](http://blog.csdn.net/eastmount/article/details/52823548)

```
<!DOCTYPE html>  
<html>  
<head>  
    <meta charset="utf-8">  
    <title>ECharts</title>  
    <!-- 引入 echarts.js -->  
    <script src="echarts.min.js"></script>  
</head>  
  
<body>  
    <!-- 为ECharts准备一个具备大小的Dom -->  
    <div align="left" id="main" style="width: 900px;height:500px;"></div>  
    <script type="text/javascript">  
        //基于准备好的dom，初始化echarts实例  
        var myChart = echarts.init(document.getElementById('main'));  
     
        option = {  
            title: {  
                text: '2016年上半年全国规模以上工业企业用水情况（单位：亿立方米）',   
                subtext: '数据来源：国家统计局',  
                x: 'center',  
            },  
            tooltip : {  
                trigger: 'axis',  
                axisPointer : {         
                    //坐标轴指示器，坐标轴触发有效
                    //默认为直线，可选为：'line' | 'shadow'
                    type : 'shadow'           
                }  
            },  
            legend: {  
                orient: 'vertical',  
                x: 'left',  
                y:"top",  
                padding:50,     
                data: ['用水量', '减少量',]  
            },  
            grid: {  
                left: '10',  
                right: '60',  
                bottom: '3%',  
                height: '30%',    
                top: '20%',  
                containLabel: true  
            },  
            xAxis:  {  
                type: 'value',  
                //设置坐标轴字体颜色和宽度  
                axisLine:{  
                    lineStyle:{  
                        color:'yellow',  
                        width:2  
                    }  
                },  
            },  
            yAxis: {  
                type: 'category',    
                axisLine:{  
                    lineStyle:{  
                        color:'yellow',  
                        width:2  
                    }  
                },  
                data: ['东部地区','中部地区','西部地区',]  
            },  
            series: [  
            {  
                name: '用水量',  
                type: 'bar',  
                stack: '总量',  
                label: {  
                    normal: {  
                        show: true,  
                        position: 'insidelift'  
                    }  
                },  
                data: [109.2, 48.2, 41 ]  
            },  
            {  
                name: '减少量',  
                type: 'bar',  
                stack: '总量',  
                label: {  
                    normal: {  
                        show: true,  
                        position: 'insidelift'  
                    }  
                },  
                data: [1.638, 1.0122, 1.025]  
            },  
        ]  
    };  
    myChart.setOption(option);  
    </script>  
</body>  
</html>
```
        运行结果如下图所示：


![](https://img-blog.csdn.net/20170409231241097?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








## 二. 绘制中国地图

        下面是绘制中国地图的代码，网上非常常见，下面简单介绍。
        官方模块地址：[http://echarts.baidu.com/download-map.html](http://echarts.baidu.com/download-map.html)



![](https://img-blog.csdn.net/20170409231619099?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        然后下载中国地图的JS或JSON文件，文件名为china.js和china.json。
        代码如下，核心代码是获取id为main的div模块：
        var chart = echarts.init(document.getElementById('main'));

```
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>ECharts</title>
   
</head>

<body>
    <!-- 为ECharts准备一个具备大小（宽高）的Dom -->
    <div id="main" style="width: 600px;height:400px;"></div>
    <!-- 引入 echarts.js -->
    <script src="echarts.min.js"></script>
    <script src="china.js"></script>
    <script>
    var chart = echarts.init(document.getElementById('main'));
    chart.setOption({
        series: [{
            type: 'map',
            map: 'china'
        }]
    });
    </script>
	</div>
</body>
</html>
```
        运行结果如下：

![](https://img-blog.csdn.net/20170409232020305)







## 三. 绘制贵州地图

        这是这篇文章的重点是绘制贵州相关的地图，具体步骤如下。
        从官方下载贵州省的guizhou.js和guizhou.json文件
        官方模块地址：[http://echarts.baidu.com/download-map.html](http://echarts.baidu.com/download-map.html)
        代码如下：


```
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>ECharts</title>
   
</head>

<body>
    <!-- 为ECharts准备一个具备大小的Dom -->
    <div id="main" style="width: 600px;height:400px;"></div>
    <!-- 引入 echarts.js -->
    <script src="echarts.min.js"></script>
	  <script src="guizhou.js"></script>
    <script>
     var myChart = echarts.init(document.getElementById('main'));
     var option = {
      title: {
       text : '贵州地图',
       subtext : '各市区显示'
      },
      series: [
        {
            name: '数据名称',
        type: 'map',
        mapType: '贵州',
        selectedMode : 'single',
        itemStyle:{
             normal:{label:{show:true}},
             emphasis:{label:{show:true}}
            },
            data:[
             {name: '贵阳市',value: Math.round(Math.random()*1000)},
             {name: '遵义市',value: Math.round(Math.random()*1000)},
             {name: '六盘水市',value: Math.round(Math.random()*1000)},
             {name: '毕节市',value: Math.round(Math.random()*1000)},
             {name: '铜仁市',value: Math.round(Math.random()*1000)},
             {name: '安顺市',value: Math.round(Math.random()*1000)},
             {name: '黔西南布依族苗族自治州',value: Math.round(Math.random()*1000)},
             {name: '黔东南苗族侗族自治州',value: Math.round(Math.random()*1000)},
             {name: '黔南布依族苗族自治州',value: Math.round(Math.random()*1000)},  
            ]
        }]
      };
     myChart.setOption(option);    
    </script>
	</div>
</body>
</html>
```
        运行结果如下所示，其中每个省份对应的市州可以在json文件中查找。


![](https://img-blog.csdn.net/20170409233237693)
核心代码如下，它即可显示贵州省地图。
**<script>    var myChart = echarts.init(document.getElementById('main'));    var option = {      title: {           text : '贵州地图',           subtext : '各市区显示'      },      series: [        {            name: '数据名称',            type: 'map',            mapType: '贵州',            selectedMode : 'single',            itemStyle:{                 normal:{label:{show:true}},                 emphasis:{label:{show:true}}            }        }]      };     myChart.setOption(option);  </script>**




## 四. 推荐地图博文



        最后推荐一些文章作为在线笔记，全是一些地图操作，希望对你有所帮助。
        1.官网地图浏览：[http://echarts.baidu.com/demo.html#map-province](http://echarts.baidu.com/demo.html#map-province)



![](https://img-blog.csdn.net/20170409233530556)



        2.枯鸦专栏：[百度Echarts 全国省市地图 动态数据图形报表](http://blog.csdn.net/will_awoke/article/details/19119915)
           程序媛专栏： [ECHARTS-MAP的使用: 中国地图及省份边界的查看](http://blog.csdn.net/u010367582/article/details/53305866)



![](https://img-blog.csdn.net/20170409233656797)


        3.hcy12321专栏：[一个让echarts中国地图包含省市轮廓的技巧](http://www.cnblogs.com/yaoh/p/6183170.html)


![](https://img-blog.csdn.net/20170409233903187)





4.liuchonge专栏：[Echarts使用二：全国地图与各省市地图联动](http://blog.csdn.net/liuchonge/article/details/52199100)



![](https://img-blog.csdn.net/20170409234101283)



        5.dhforsite专栏：[echarts 某省下钻某市地图](http://www.cnblogs.com/dhforsite/p/6198579.html)



![](https://img-blog.csdn.net/20170409234226972)




        6.xianwen专栏：[ECharts（中国地图篇）的使用](http://www.cnblogs.com/xianwen/p/6045454.html)




![](https://img-blog.csdn.net/20170409234446864)





        7.little2专栏：[echarts3 使用总结](http://www.cnblogs.com/hhw3/p/6252677.html)




![](https://img-blog.csdn.net/20170409234831085)





        风雪交加雨婆娑，
        琴瑟和鸣泪斑驳。
        不管在哪，依然做自己，
        娜美陪伴，爱还在，且更爱。谢谢，晚安~
        (By:Eastmount 2017-04-09 晚上12点  [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))





