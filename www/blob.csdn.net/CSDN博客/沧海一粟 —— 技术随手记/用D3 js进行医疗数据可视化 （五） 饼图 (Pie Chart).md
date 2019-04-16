# 用D3.js进行医疗数据可视化 （五） 饼图 (Pie Chart) - 沧海一粟 —— 技术随手记 - CSDN博客





2015年08月03日 17:05:10[慢游](https://me.csdn.net/eengel)阅读数：4752








# 介绍

经过一系列对折线图的试验，这儿就来尝试一下用饼图（Pie Chart）来做医疗数据的可视化。在《用D3.js进行医疗数据可视化 （四） 堆积区图(Stacked Area Chart)》的图2中，我们见识了每种医疗机构每月院均诊疗人次数（人次）的比较。其实对于一组数据间的比较，用饼图是非常适合的：它可以直观地看到每种数据占总体的比例，以及相对于其他组数据孰大孰小。



# 代码



```java
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Pie Chart</title>
    <link rel="stylesheet" type="text/css" href="css/styles.css"/>
    <script type="text/javascript" src="js/d3.min.js"></script>
    <script type="text/javascript" src="js/jquery-1.11.0.min.js"></script>
</head>

<body>

<script type="text/javascript">
    function pieChart() {
        var _chart = {};

        var _width = 650, _height = 500,
                _data = [],
                _colors, // = d3.scale.category10(),
                _names = [],    //keep name list for the chart
                _svg,
                _bodyG,
                _pieG,
                _radius = 200,
                _innerRadius = 100;

        _chart.render = function () {
            if (!_svg) {
                _svg = d3.select("body").append("svg")
                        .attr("height", _height)
                        .attr("width", _width);
            }

            renderBody(_svg);
        };

        function renderBody(svg) {
            if (!_bodyG)
                _bodyG = svg.append("g")
                        .attr("class", "body");

            renderPie();
        }

        function renderPie() {
            var pie = d3.layout.pie()
                    .value(function (d) {
                        return d.value;
                    });

            var arc = d3.svg.arc()
                    .outerRadius(_radius)
                    .innerRadius(_innerRadius);

            if (!_pieG)
                _pieG = _bodyG.append("g")
                        .attr("class", "pie")
                        .attr("transform", "translate(" 
                            + _radius 
                            + "," 
                            + _radius + ")");

            renderSlices(pie, arc);

            renderLegend();
        }

        function renderSlices(pie, arc) {
            var slices = _pieG.selectAll("path.arc")
                    .data(pie(_data));

            slices.enter()
                    .append("path")
                    .attr("class", "arc")
                    .attr("fill", function (d, i) {
                        return _colors(_names[i]);
                    });

            slices.transition()
                    .attrTween("d", function (d) {
                        var currentArc = this.__current__;

                        if (!currentArc)
                            currentArc = {startAngle: 0, 
                                            endAngle: 0};

                        var interpolate = d3.interpolate(
                                            currentArc, d);
                                            
                        this.__current__ = interpolate(1);
                        
                        return function (t) {
                            return arc(interpolate(t));
                        };
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
    				var legendX = 450;   //set position for each legend element
    				var legendY = 30 * i + 120;
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

        _chart.radius = function (r) {
            if (!arguments.length) return _radius;
            _radius = r;
            return _chart;
        };

        _chart.innerRadius = function (r) {
            if (!arguments.length) return _innerRadius;
            _innerRadius = r;
            return _chart;
        };

        _chart.data = function (d) {
            if (!arguments.length) return _data;
            _data = d;
            return _chart;
        };

        return _chart;
    }

    function randomData() {
        return Math.random() * 9 + 1;
    }

    function update() {
        for (var j = 0; j < data.length; ++j)
            data[j].value = randomData();

        chart.render();
    }
    
    d3.text("data/health-service-quantity-2.json", function(rawdatastr){
    	//transform data to proper format
    	var rawdata = JSON.parse(rawdatastr);
    	var names = d3.keys(rawdata);
    	var sublists = [];
    	names.forEach(function(name){
    		var sublist = rawdata[name].map(function(d){
    			return {
    				name: name,
    				value: Math.round((d.value * 1000)/d.number) 
    			}
    		});
    		sublists.push(sublist);
    	});
    	var list = sublists[0].map(function(d, j){
    		var l = [];
    		for(var i = 0; i < sublists.length; i++){
    			l.push(sublists[i][j]);
    		}
    		return l;
    	});
    	
    	var timeformat = d3.time.format("%Y-%m");
    	var dates = rawdata[names[0]].map(function(d){
    		return timeformat.parse(d.date);
    	});
    	
    	//periodly render the pie chart
    	var len = list.length;
    	var n = 0;
    	var data = list[n];
    	var chart = pieChart()
	        .radius(200)
	        .innerRadius(0)
	        .data(data);
    	
    	var date = dates[n];
    	$("#year").html(date.getFullYear());
    	$("#month").html(date.getMonth()+1);
    	
    	chart.names(names);
    	chart.colors(d3.scale.category20().domain(names));
    	chart.render();
    	
    	var handle = setInterval(function(){
    		++n;
    		
    		var date = dates[n];
        	$("#year").html(date.getFullYear());
        	$("#month").html(date.getMonth()+1);
    		
		    chart.data(list[n]);
		    chart.render();
    		  
			if(n >= (len - 1)){
				clearInterval(handle);
			}
    	}, 1000);
	
    });
</script>

<div class="control-group">
    <span id="year"></span> 年
    <span id="month"></span> 月
</div>

</body>

</html>
```




# 可视化效果

![](https://img-blog.csdn.net/20150803170426489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 讨论

这儿基本饼图的实现参考了Nick Qi Zhu的[源码](https://raw.githubusercontent.com/NickQiZhu/d3-cookbook/master/src/chapter9/pie-chart.html)，主要的改动，首先是对原数据进行了变形。原数据以每种机构作为key，所有日期的数据是一个list。但在饼图中，需要把同一个时间点上不同机构的value作为一个list。其次，采用了setInterval对每个时间点的数据进行顺序播放，直到全部可视化完毕。

图中显而易见，三级医院的每月院均诊疗人次数是最多的。而基层医疗机构相比而言几乎可以忽略不计。



