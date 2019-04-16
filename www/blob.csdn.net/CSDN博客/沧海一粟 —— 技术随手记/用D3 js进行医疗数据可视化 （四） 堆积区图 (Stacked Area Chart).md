# 用D3.js进行医疗数据可视化 （四） 堆积区图 (Stacked Area Chart) - 沧海一粟 —— 技术随手记 - CSDN博客





2015年07月23日 21:56:03[慢游](https://me.csdn.net/eengel)阅读数：4092








# 介绍

在之前的文章《[用D3.js进行医疗数据可视化 （一）折线图](http://blog.csdn.net/eengel/article/details/46958643)》中，我们可以看到对于所可视化的医疗卫生机构诊疗人次数线图，最下部的几条线几乎重合在一起，而且几种机构类型相互包含，关系比较混乱。因此在这篇文章中，我们尝试一下用堆积区图（stackedarea chart）来强调它们之间的对比。



# 代码



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
            _margins = {top: 30, left: 50, right: 130, bottom: 70},  //expand margin for legend rendor
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
                .orient("bottom")
                .tickFormat(d3.time.format("%y年%m月"))  //set tick name in Chinese
                .ticks(d3.time.months, 1);  //set ticks for x axis as monthly

        axesG.append("g")
                .attr("class", "x axis")
                .attr("transform", function () {
                    return "translate(" + xStart() + "," + yStart() + ")";
                })
                .call(xAxis);
                
        /* d3.selectAll("g.x g.tick")
            .append("line")
                .classed("grid-line", true)
                .attr("x1", 0)
                .attr("y1", 0)
                .attr("x2", 0)
                .attr("y2", - quadrantHeight()); */
        
        d3.select("g.x.axis")   //add label for x axis
        	.append("text")
            .attr("class", "axislabel")
            .attr("text-anchor", "end")
            .attr("x", quadrantWidth)
            .attr("y", 40)
            .text("日期");
        
        d3.selectAll("g.x g.tick text")
        	.attr("x", -24)
        	.attr("transform", "rotate(-30)"); 
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
                
         /* d3.selectAll("g.y g.tick")
            .append("line")
                .classed("grid-line", true)
                .attr("x1", 0)
                .attr("y1", 0)
                .attr("x2", quadrantWidth())
                .attr("y2", 0); */
         
         d3.select("g.y.axis")   //add label for y axis
	     	.append("text")
	        .attr("class", "axislabel")
	        .attr("text-anchor", "start")
	        .attr("x", -_margins.left)
	        .attr("y", -6)
	        .text("诊疗人次数（万人次）");
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

         var stack = d3.layout.stack()
		        .offset('zero')
		        .order('default');
		stack(_data);

        renderLines();

        //renderDots(); 
        
        renderAreas();
        
        //Method 2: put legend at the bottom of the chart
        renderLegend(); 
    }

    function renderLines() {
        _line = d3.svg.line()
                        .x(function (d) { return _x(d.x); })
                        .y(function (d) {
		                    return _y(d.y + d.y0);
		                });
                        
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
                    .attr("cx", function (d) { return _x(d.x); })
                    .attr("cy", function (d) { return _y(d.y); })
                    .attr("r", 3.5);
        });
    }
    
    function renderAreas() {
        var area = d3.svg.area()
                .x(function (d) {
                    return _x(d.x);
                })
                .y0(function(d){return _y(d.y0);})
                .y1(function (d) {
                    return _y(d.y + d.y0);
                });
        _bodyG.selectAll("g.areagroup path.area")
					.data(_data)
                .enter()
                .append("g")
                .attr("class", "areagroup")
                .append("path")
                .style("fill", function (d, i) {
                    return _colors(_names[i]);
                })
                .attr("class", "area");
        _bodyG.selectAll("g.areagroup path.area")
					.data(_data)
                .transition()
                .attr("d", function (d) {
                    return area(d);
                });

        //Method 1: add legend along with the line
        _bodyG.selectAll("g.areagroup")
        	.append("text")
	        .datum(function(d, i) { 
	        	return {name: _names[i], x: d[d.length - 1].x, y: (d[d.length - 1].y0 + d[d.length - 1].y)};   //get the last point of the line
	        })
	        .attr("transform", function(d) { 
	        	return "translate(" + (_x(d.x) - 5) + "," + _y(d.y) + ")";  //set the legend beside the last point
	        })
	        .attr("x", 3)
	        .attr("dy", "1em")
	        .attr("text-anchor", "end")
	        .attr("font-size", "10px")
	        .text(function(d) { 
	        	return d.name; 
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

//load data
var timeformat = d3.time.format("%Y-%m");

var chart;

d3.text("data/health-service-quantity-2.json", function(rawdatastr){
	var rawdata = JSON.parse(rawdatastr);
	
	var names = d3.keys(rawdata);
	//transform data string to Date format
	names.forEach(function(name){
		rawdata[name].forEach(function(d){
			d.date = timeformat.parse(d.date);
		});
	});
	
	//get date range
	var dateRange = d3.extent(rawdata[names[0]], function(d){
		return d.date;
	})
	
	//get value range
	var valueSum = [];
	names.forEach(function(name){
		valueSum.push(d3.extent(rawdata[name], function(d){
			return d.value;
		})[1]);
	});
	var maxValue = d3.sum(valueSum);	
	
	chart = lineChart()
			.x(d3.time.scale().domain(dateRange))
			.y(d3.scale.linear().domain([0, maxValue]));   //adjust domain for y axis
	
	names.forEach(function (name) {
	    chart.addSeries(rawdata[name].map(function(d){
	    	return {
	    		x: d.date, 
	    		y: d.value
	    	};
	    }));
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




# 可视化效果

![](https://img-blog.csdn.net/20150723214622551?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图1 每种医疗机构每月总诊疗人次数（万人次）

![](https://img-blog.csdn.net/20150725221652458?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图2 每种医疗机构每月院均诊疗人次数（人次）


# 讨论

先看图1。直观来看，堆积区图比起折线图，能更好地反映机构之间诊疗人次数的对比。可以从图中看到，基层医疗卫生机构、三级医院和二级医院，占了诊疗数的大头。而一级医院和未定级医院相比而言，诊疗数很少。而二级医院和三级医院相比，差距不明显。除了这些结论，我也比较好奇如果平均到每所机构，其诊疗数的差异是否还是这样。比如在这张图上，二级和三级医院的诊疗数差异不明显，但如果分别考虑这两种医院的数量，也许就会有不同的结论。图2就是对Y轴代表的含义进行了调整，通过用每种医疗机构每月总诊疗人次数除以当月每种机构总数得到了院均诊疗人次数。从图上看到，原本诊疗数量很大的基层机构平均到每个机构，每月只有区区几十的数量，在图中几乎看不出来。而三级医院一平均到每院，立马就占了诊疗数的大头。

下面是一些技术上面的讨论。


## 1 改变Y轴的domain

在之前的折线图中，我们设置Y轴的domain是从value的最小值到最大值。而堆积区图需要显示的是从0到几个机构每个纵向的value堆积起来的最大值。参考maxValue的计算。对X轴domain的计算也用了一种新的方法。

## 2 调整数据

对于原始的JSON文件，考虑到堆积区图会自动累积每种机构的value，因此我们把JSON中是由别的机构value合计出来的机构类型都去掉了，并按照有意义的顺序排列了机构。

## 3 产生堆积区域

由于d3.layout.stack()的特殊要求，我们对_data进行了变换，把相应的属性名字变成了x和y。再用d3.svg.area()渲染区块。而区块之间的折线位置也根据堆积的特性进行了调整。

## 4 去掉点和网格

为了图的简洁起见，这儿去掉了折线图中的点和网格。这个也是见仁见智吧。

## 5 调整右侧图例

通过调整右侧图例text节点的text-anchor和纵向位置，我们把图例放到了区域内部。从目前的效果来看，除非让整个图的高度变大，不然这种图例还是会出现重叠的可能。而放在下方的图例则可以不改变实现方法。

# 参考文献

1.    StackedArea Chart. [https://github.com/NickQiZhu/d3-cookbook/blob/master/src/chapter9/stacked-area-chart.html](https://github.com/NickQiZhu/d3-cookbook/blob/master/src/chapter9/stacked-area-chart.html)



