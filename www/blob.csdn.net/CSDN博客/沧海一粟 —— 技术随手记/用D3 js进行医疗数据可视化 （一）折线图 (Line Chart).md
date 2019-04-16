# 用D3.js进行医疗数据可视化 （一）折线图 (Line Chart) - 沧海一粟 —— 技术随手记 - CSDN博客





2015年07月19日 22:01:08[慢游](https://me.csdn.net/eengel)阅读数：10185








# 介绍

大概说说这个事情的前因后果吧。



去年下半年，我有机会参加了一个大型项目，在里面参与了前端可视化组建的开发，用的library是D3.js [2]。虽然3个月后我就结束了在这个项目里的工作，但后面断断续续也用D3做了一些别的实现。

而从几年前开始，我一直在做和healthcare相关的项目，主要就是做医疗数据的信息化，从而优化医疗领域的信息管理、决策支持。从项目的角度出发，我们的着眼点一般都是某个具体的问题，比如如何优化病人诊疗的流程。但从宏观上出发，如果能了解某个地区乃至全国的医疗服务情况是怎么样的，也是 非常有意义的 。

结合这两者，我就想做一件事情，将一些宏观的医疗服务数据通过可视化技术，更直观地表达出来。



# 原则

对于做这个事情，个人认为也有一些原则要遵守。首先，最基本的，医疗数据的来源要公开、权威。其次，最重要的，这样的可视化要体现医疗服务的特点，使得其在医疗领域能体现价值。当然，这不是一蹴而就的，而需要不断地在探索和实践中摸索。实践本身也是一个学习的过程。除了D3.js官网的API文档，主要的参考书就是Nick Qi Zhu的《D3.js数据可视化实战手册》[3]。由于本人较懒，后面很多实践也是在Nick的源码基础上直接修改所得。



# 数据来源

本着公开、权威的原则，找到了国家卫生和计划生育委员会统计信息中心[1]的统计数据。由于在这个阶段我的目标是验证医疗数据可视化的一个实现的流程，因此并没有花太多时间在数据的抓取上。通过简单的调研，发现其中全国医疗服务情况的数据还比较全面。而由于精力所限，最终选择了以下几类医疗卫生机构医疗服务量中的诊疗人次数，输出的是JSON格式文档。
- 机构合计
- 医院
- 三级医院
- 二级医院
- 一级医院
- 未定级医院

- 基层医疗卫生机构




```
{
	"医疗卫生机构合计": [
		{
			"date": "2013-05",
			"value":	60005.2	
		},{
			"date": "2013-06",
			"value":	58861.9	
		},{
			"date": "2013-07",
			"value":	60264.2	
		},{
			"date": "2013-08",
			"value":	59519.3	
		},...
        ], ...
}
```




# 可视化选型

通过对数据的解读并结合之前的原则，我一开始的想法是想用D3图表表现全国不同医疗机构月均诊疗人次数随着时间的变化。也就是说，图表的横轴是时间，纵轴是诊疗人次数，而不同的医疗机构的数据能够出现在同一个图表中以便做对比。因此，我自然而然将线图(linechart)作为了第一选择。



# D3实现

对于基本线图的实现，《D3.js数据可视化实战手册》中有很好的例子，也提供了源码，因此在整个coding过程中，比较多的时间花在了数据的处理上。本人使用的D3版本是3.5.5。源码如下。



```java
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>全国医疗卫生机构医疗服务量</title>
    <link rel="stylesheet" type="text/css" href="css/styles.css"/>
    <script type="text/javascript" src="js/d3.min.js"></script>
</head>

<body>

<script type="text/javascript">
function lineChart() {
    var _chart = {};

    var _width = screen.width * 0.9, _height = 350,
            _margins = {top: 30, left: 50, right: 30, bottom: 30},
            _x, _y,
            _data = [],
            _colors = d3.scale.category10(),
            _svg,
            _bodyG,
            _line;

    _chart.render = function () {
        if (!_svg) {
            _svg = d3.select("body").append("div")
            		.style("text-align", "center")
            		.append("svg")
                    .attr("height", _height)
                    .attr("width", _width);

            renderAxes(_svg);

            defineBodyClip(_svg);
        }

        renderBody(_svg);
    };

    function renderAxes(svg) {
        var axesG = svg.append("g")
                .attr("class", "axes");

        renderXAxis(axesG);

        renderYAxis(axesG);
    }
    
    function renderXAxis(axesG){
        var xAxis = d3.svg.axis()
                .scale(_x.range([0, quadrantWidth()]))
                .orient("bottom");        

        axesG.append("g")
                .attr("class", "x axis")
                .attr("transform", function () {
                    return "translate(" + xStart() + "," + yStart() + ")";
                })
                .call(xAxis);
                
        d3.selectAll("g.x g.tick")
            .append("line")
                .classed("grid-line", true)
                .attr("x1", 0)
                .attr("y1", 0)
                .attr("x2", 0)
                .attr("y2", - quadrantHeight());
    }
    
    function renderYAxis(axesG){
        var yAxis = d3.svg.axis()
                .scale(_y.range([quadrantHeight(), 0]))
                .orient("left");
                
        axesG.append("g")
                .attr("class", "y axis")
                .attr("transform", function () {
                    return "translate(" + xStart() + "," + yEnd() + ")";
                })
                .call(yAxis);
                
         d3.selectAll("g.y g.tick")
            .append("line")
                .classed("grid-line", true)
                .attr("x1", 0)
                .attr("y1", 0)
                .attr("x2", quadrantWidth())
                .attr("y2", 0);
    }

    function defineBodyClip(svg) {
        var padding = 5;

        svg.append("defs")
                .append("clipPath")
                .attr("id", "body-clip")
                .append("rect")
                .attr("x", 0 - padding)
                .attr("y", 0)
                .attr("width", quadrantWidth() + 2 * padding)
                .attr("height", quadrantHeight());
    }

    function renderBody(svg) {
        if (!_bodyG)
            _bodyG = svg.append("g")
                    .attr("class", "body")
                    .attr("transform", "translate(" 
                        + xStart() + "," 
                        + yEnd() + ")")
                    .attr("clip-path", "url(#body-clip)");        

        renderLines();

        renderDots();
    }

    function renderLines() {
        _line = d3.svg.line()
                        .x(function (d) { return _x(d.date); })
                        .y(function (d) { return _y(d.value); });
                        
        _bodyG.selectAll("path.line")
                    .data(_data)
                .enter()
                .append("path")                
                .style("stroke", function (d, i) { 
                    return _colors(i);
                })
                .attr("class", "line");
        
        _bodyG.selectAll("path.line")
        			.data(_data)
        			.exit()
        			.remove();

        _bodyG.selectAll("path.line")
                .transition()
                .duration(1000)
                .attr("d", function (d) { return _line(d); });
    }

    function renderDots() {
        _data.forEach(function (list, i) {
            _bodyG.selectAll("circle._" + i)
                        .data(list)
                    .enter()
                    .append("circle")
                    .attr("class", "dot _" + i);
            
            _bodyG.selectAll("circle._" + i)
            		.data(list) 
            		.exit()
            		.remove();

            _bodyG.selectAll("circle._" + i)
                    .data(list)                    
                    .style("stroke", function (d) { 
                        return _colors(i);
                    })
                    .transition()
                    .duration(1000)
                    .attr("cx", function (d) { return _x(d.date); })
                    .attr("cy", function (d) { return _y(d.value); })
                    .attr("r", 3.5);
        });
    }

    function xStart() {
        return _margins.left;
    }

    function yStart() {
        return _height - _margins.bottom;
    }

    function xEnd() {
        return _width - _margins.right;
    }

    function yEnd() {
        return _margins.top;
    }

    function quadrantWidth() {
        return _width - _margins.left - _margins.right;
    }

    function quadrantHeight() {
        return _height - _margins.top - _margins.bottom;
    }

    _chart.width = function (w) {
        if (!arguments.length) return _width;
        _width = w;
        return _chart;
    };

    _chart.height = function (h) {
        if (!arguments.length) return _height;
        _height = h;
        return _chart;
    };

    _chart.margins = function (m) {
        if (!arguments.length) return _margins;
        _margins = m;
        return _chart;
    };

    _chart.colors = function (c) {
        if (!arguments.length) return _colors;
        _colors = c;
        return _chart;
    };

    _chart.x = function (x) {
        if (!arguments.length) return _x;
        _x = x;
        return _chart;
    };

    _chart.y = function (y) {
        if (!arguments.length) return _y;
        _y = y;
        return _chart;
    };

    _chart.addSeries = function (series) {
        _data.push(series);
        return _chart;
    };

    return _chart;
}

function randomData() {
    return Math.random() * 9;
}

function update() {
    for (var i = 0; i < data.length; ++i) {
        var series = data[i];
        series.length = 0;
        for (var j = 0; j < numberOfDataPoint; ++j)
            series.push({x: j, y: randomData()});
    }

    chart.render();
}
        
var names = [];
var data = [];
var minvalue = 0, maxvalue = 0;
var mindate = 0, maxdate = 0;

var timeformat = d3.time.format("%Y-%m");

var chart;

<pre name="code" class="javascript">//load data
d3.text("data/health-service-quantity.json", function(rawdatastr){
	var rawdata = JSON.parse(rawdatastr);
	names = d3.keys(rawdata);
	names.forEach(function(name){
		var list = rawdata[name].map(function(item){
			return {
				"date": timeformat.parse(item.date), 
				"value": item.value
			}
		});
		
		//get min and max value
		if(minvalue == 0){
			minvalue = d3.min(list, function(d){return d.value;});			
		}else{
			minvalue = d3.min([minvalue, d3.min(list, function(d){return d.value;})]);
		}
		maxvalue = d3.max([maxvalue, d3.max(list, function(d){return d.value;})]);
		
		//get min and max date
		if(mindate == 0){
			mindate = d3.min(list, function(d){return d.date;});
		}
		if(maxdate == 0){
			maxdate = d3.max(list, function(d){return d.date;});
		}
		
		data.push(list);
	});
	
	chart = lineChart()
			.x(d3.time.scale().domain([mindate, maxdate]))
			.y(d3.scale.linear().domain([minvalue * 0.5, maxvalue * 1.02]));
	
	data.forEach(function (series) {
	    chart.addSeries(series);
	});
	
	chart.render();
});

</script>

</body>

</html>
```






分析一下几个技术点。

## 1. 从JSON文件加载数据

D3提供了d3.json，但很不幸地，此API只能加载list式的JSON（例如[1,2, 3]）而不能加载key-value式的（例如{“id”:1}）。而我之前构造的数据是以不同机构的名字作为key，其下面的数据列表作为value。因此我选择了用d3.text加载JSON文件成String，再通过JSON.parse将String变换成JSON Object。

接下来，考虑到每条线对应的是list式的数据，因此还需要对原始JSONObject做一些变换，比如将每个value中的list放到list变量data中，并将原来以字符串形式存在的date通过d3.time.format变换成Date Object。



## 2. 得到数据分布的最大最小值

要生成x轴和y轴，需要设置其合理的domain。这儿就要计算数据的最大最小值。对x轴而言，对应的是数据中的日期。而对不同的医疗机构而言，这儿我们假设其数据的日期范围是一样的，因此只要对一个机构的数据进行计算即可。用到的就是d3.min和d3.max。并且由于每条数据是一个JSON Object，设置d3.min和d3.max的accessor为一个返回date的function。对于y轴而言，需要计算所有机构的数据的最大最小值，因此使用了嵌套的d3.min和d3.max。



# 可视化结果

![](https://img-blog.csdn.net/20150719215754839?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



# 讨论

从呈现的结果来看，达到了初步的预期：每种机构的日均诊疗人次数各有大小；总体呈缓慢上升趋势；过年期间会小降；等等。

但总体而言，这个可视化呈现还有很多待改进的地方。

1. 需要显示图例，说明每条曲线对应的机构

2. x轴最好能显示每个月的中文名字，中间区域内的网格也是每个月一格。

3. 由于其中的机构种类有包含关系，是否对其改用堆积区图 (stackedarea chart) 比较好？

4. 从美观考虑，线的出现最好有动画效果。

后面的系列会陆续探讨这几点。



# 参考文献

1.    国家卫生和计划生育委员会统计信息中心.[http://www.moh.gov.cn/mohwsbwstjxxzx/index.shtml](http://www.moh.gov.cn/mohwsbwstjxxzx/index.shtml)

2.    D3.jsAPI Reference. [https://github.com/mbostock/d3/wiki/API-Reference](https://github.com/mbostock/d3/wiki/API-Reference)

3.    Nick Qi Zhu. D3.js数据可视化实战手册. [http://book.douban.com/subject/26256865/](http://book.douban.com/subject/26256865/)





