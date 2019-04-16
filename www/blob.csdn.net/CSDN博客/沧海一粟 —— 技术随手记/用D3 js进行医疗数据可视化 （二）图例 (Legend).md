# 用D3.js进行医疗数据可视化 （二）图例 (Legend) - 沧海一粟 —— 技术随手记 - CSDN博客





2015年07月22日 14:42:20[慢游](https://me.csdn.net/eengel)阅读数：6042








# 介绍

在上一篇文章的结尾，对生成的可视化图，我们提到了许多待改进的地方。这儿就先来讨论下图例的显示。

图例的主要目的是说明图表中各种符号和颜色所代表的内容及指标的说明。对于我们之前生成的line chart，需要用图例说明每条曲线所代表的具体含义是什么。总结两点基本原则如下：

1.     图例中要对应曲线的颜色，并指出对应的医疗机构名称；

2.     图例要放在适当的位置，以便起到恰如其分的指示作用。

本文下面会实现两种不同的实现方法，并讨论其不同之处。



# 代码示例





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
            _margins = {top: 30, left: 50, right: 130, bottom: 60},  //expand margin for legend rendor
            _legendbottom = 30,   //area for legend in Method 2
            _legendright = 100,  //area for legend in Method 1
            _x, _y,
            _names = [],    //keep name list for the chart
            _data = [],
            //_colors = d3.scale.category10(),
            _colors,    //define colors according to name list
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
                .attr("width", quadrantWidth() + 2 * padding + _legendright)  //expand clip path for legend
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
        
        //Method 2: put legend at the bottom of the chart
        renderLegend();
    }

    function renderLines() {
        _line = d3.svg.line()
                        .x(function (d) { return _x(d.date); })
                        .y(function (d) { return _y(d.value); });
                        
        _bodyG.selectAll("g.linegroup path.line")
                    .data(_data)
                .enter()
                .append("g")
                .attr("class", "linegroup")
                .append("path")                
                .style("stroke", function (d, i) { 
                    return _colors(_names[i]);
                })
                .attr("class", "line");
        
        _bodyG.selectAll("g.linegroup path.line")
        			.data(_data)
        			.exit()
        			.remove();

        _bodyG.selectAll("g.linegroup path.line")
                .transition()
                .duration(1000)
                .attr("d", function (d) { return _line(d); });
        
        //Method 1: add legend along with the line
        _bodyG.selectAll("g.linegroup")
        	.append("text")
	        .datum(function(d, i) { 
	        	return {name: _names[i], date: d[d.length - 1].date, value: d[d.length - 1].value};   //get the last point of the line
	        })
	        .attr("transform", function(d) { 
	        	return "translate(" + (_x(d.date) + 5) + "," + _y(d.value) + ")";  //set the legend beside the last point
	        })
	        .attr("x", 3)
	        .attr("dy", ".35em")
	        .classed("legendtext", true)
	        .text(function(d) { 
	        	return d.name; 
	        });
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
                        return _colors(_names[i]);
                    })
                    .transition()
                    .duration(1000)
                    .attr("cx", function (d) { return _x(d.date); })
                    .attr("cy", function (d) { return _y(d.value); })
                    .attr("r", 3.5);
        });
    }
    
    //render legend at the bottom of the chart
    function renderLegend(){
    	var legend = _svg.selectAll(".legend")
			.data(_colors.domain())
			.enter()
			.append("g")
			.attr("class", "legend")
			.attr("transform", function(d, i) {
				var legendX = i * 120 + _margins.left;   //set position for each legend element
				var legendY = _height - _legendbottom;
				return "translate(" + legendX + ", " + legendY + ")";
			});
		
		legend.append("rect")
			.attr("x", 0)
			.attr("y", 1)
			.attr("width", 16)
			.attr("height", 8)
			.style("fill", _colors);
		
		legend.append("text")
			.attr("x", 20)
			.attr("y", 9)
			.classed("legendtext", true)
			.text(function(d) {
				return d;
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
    
    //set name list function
    _chart.names = function (n) {
        if (!arguments.length) return _names;
        _names = n;
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
        
//load data
var names = [];
var data = [];
var minvalue = 0, maxvalue = 0;
var mindate = 0, maxdate = 0;

var timeformat = d3.time.format("%Y-%m");

var chart;

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
	
	//set name list to the chart
	chart.names(names);
	//define colors according to name list
	chart.colors(d3.scale.category10().domain(names));
	
	chart.render();
});

</script>

</body>

</html>
```






# 可视化结果

可以看到，其中一种实现方法（方法一）把图例直接放到曲线的后面（原理可以参考[1]），而另一种方法（方法二）则把图例集中放在了图表的下方。



![](https://img-blog.csdn.net/20150722144128187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


# 讨论

先看看实现方法。在上面图例的实现中，有几个基本概念。

首先，无论哪种实现方法，都需要把names传入chart，以在图例中显示医疗机构的名称。其次，这个名称列表和colors的对应关系需要明确地定义，以便在图例中也能保持和曲线一致的关联关系。

除了上面的一些通用的地方， 方法一关键的两点，一是要将图例的文字放到曲线最后一个点的位置旁边，二是要为图例的文字留够右边的空白。本例中由于使用了clipPath，还需要让clipPath的范围也能覆盖到图例的范围。

方法二也需要为图例留够图表下方的空白。另外还要设置恰当的图例排版。现在的实现由于宽度足够，因此是均匀排列成一行。也可以设置恰当的计算方式进行其他样子的排版，比如排成多行或纵向排列等。

加入两种方法产生的图例后，上面整个chart的布局如下图所示。

![](https://img-blog.csdn.net/20150722175836974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


从目前的实现效果来看， 第一种方法占用的是横向的空间，优点是图例跟着曲线走，让用户对每根曲线代表的数据一目了然。但它的缺陷也是显而易见的， 不同的图例名字可能会重叠到一起而看不清楚。第二种方法是一种比较标准的图例放置办法，占用的是纵向的空间，没有特别明显的缺陷。对于当前这个线图，个人感觉第二种方法更合适一些。当然对于每个不同的图表，需要根据其实际情况选择其图例呈现的方式。



# 参考文献

1.     Multi-Series Line Chart. [http://bl.ocks.org/mbostock/3884955](http://bl.ocks.org/mbostock/3884955)



