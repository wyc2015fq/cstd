# [知识图谱实战篇] 四.HTML+D3+CSS绘制关系图谱 - 杨秀璋的专栏 - CSDN博客





2019年02月12日 17:40:52[Eastmount](https://me.csdn.net/Eastmount)阅读数：574
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)









前面作者讲解了很多知识图谱原理知识，包括知识图谱相关技术、Neo4j绘制关系图谱等，但仍缺少一个系统全面的实例。为了加深自己对知识图谱构建的认识，为后续创建贵州旅游知识图谱打下基础，作者深入学习了张宏伦老师的网易云课程（[星球系列电影](https://study.163.com/course/courseLearn.htm?courseId=1003528010)），并结合自己的理解和技术分享了该系列专栏，从数据采集、数据展示、数据分析到知识图谱构建，文章后续还会讲解中文数据的实体识别、关系抽取、知识计算等。

前文介绍了Python3抓取电影实体知识，Seaborn可视化展示电影信息，D3可视化布局。本文着重构建知识图谱，通过D3显示已获取的节点和关系图谱。

代码下载地址：[https://download.csdn.net/download/eastmount/10954282](https://download.csdn.net/download/eastmount/10954282)

这是一系列基础性文章，希望对您有所帮助 ，尤其是对知识图谱感兴趣和编程刚入门的同学。同时也因为最近准备博士考试，做题做吐了，写点新东西调节下心情，与君共勉，一起加油。

**前文：**
[[知识图谱实战篇] 一.数据抓取之Python3抓取JSON格式的电影实体](https://blog.csdn.net/eastmount/article/details/86714051)
[[知识图谱实战篇] 二.Json+Seaborn可视化展示电影实体](https://blog.csdn.net/Eastmount/article/details/86739154)
[[知识图谱实战篇] 三.Python提取JSON数据、HTML+D3构建基本可视化布局](https://blog.csdn.net/Eastmount/article/details/86755610)




### 文章目录
- [一.前文回顾](#_43)
- [二.D3设置节点和关系](#D3_72)
- [三.CSS设置节点样式](#CSS_433)





**推荐作者的知识图谱前文：**
[知识图谱相关会议之观后感分享与学习总结](https://blog.csdn.net/Eastmount/article/details/46672701)
[中文知识图谱研讨会的学习总结 (上) 图谱引入、百度知心、搜狗知立方](https://blog.csdn.net/Eastmount/article/details/49785639)
[搜索引擎和知识图谱那些事 (上).基础篇](https://blog.csdn.net/Eastmount/article/details/46874155)
[基于VSM的命名实体识别、歧义消解和指代消解](https://blog.csdn.net/Eastmount/article/details/48566671)
[CSDN下载-第一届全国中文知识图谱研讨会演讲PPT 清华大学](https://download.csdn.net/download/eastmount/9255871)
[CSDN下载-知识图谱PDF资料 清华大学知识图谱研讨会汇报PPT](https://download.csdn.net/download/eastmount/9159689)

[[知识图谱构建] 一.Neo4j图数据库安装初识及药材供应图谱实例](https://blog.csdn.net/Eastmount/article/details/83268410)
[[知识图谱构建] 二.《Neo4j基础入门》基础学习之创建图数据库节点及关系](https://blog.csdn.net/Eastmount/article/details/85093828)
[[关系图谱] 一.Gephi通过共线矩阵构建知网作者关系图谱](https://blog.csdn.net/Eastmount/article/details/81746650)
[[关系图谱] 二.Gephi导入共线矩阵构建作者关系图谱](https://blog.csdn.net/Eastmount/article/details/85046305)
再次强烈推荐大家阅读张宏伦老师的网易云课程及Github源码，受益匪浅。
[https://github.com/Honlan/starwar-visualization/tree/master/star_war](https://github.com/Honlan/starwar-visualization/tree/master/star_war)
[https://study.163.com/course/courseLearn.htm?courseId=1003528010](https://study.163.com/course/courseLearn.htm?courseId=1003528010)
# 一.前文回顾

前文已经讲解D3获取JSON数据，包括节点和边，如下所示。注意，需要将JSON和HTML文件放置服务器中，作者本地服务器使用的是Apache（WAMP）。
![](https://img-blog.csdnimg.cn/20190212173631847.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190212173651427.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
本文主要讲解如何生成关系图谱，其最终生成的效果图如下所示：
![](https://img-blog.csdnimg.cn/20190212170153248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
D3是基于数据的文档操作javascript库，D3能够把数据和HTML、SVG、CSS结合起来，创造出可交互的数据图表。D3 （Data-Driven Documents）是基于数据的文档操作javascript库，D3能够把数据和HTML、SVG、CSS结合起来，创造出可交互的数据图表。
![](https://img-blog.csdnimg.cn/20190203005739151.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 二.D3设置节点和关系

核心代码如下：
**1.定义函数获取JSON数据**

定义d3.json请求python处理好的节点及边 请求成功返回数据，否则报错，利用console.log(graph)输出获取的JSON数据结果。
```
d3.json("starwar_alldata.json", function(error, data) {
	    	if(error) throw error;
	    	graph = data;
	    	console.log(graph);
}
```

**2.添加节点和关系**

通过D3映射数据至HTML中，svg.append(“g”)定义g用于绘制所有边和节点，selectAll()选中所有的line，并绑定数据data(graph.links)，enter().append(“line”)添加元素，再设置各属性。如attr(“r”, function(d){})设置节点直径，.attr(“fill”, function(d) { return colors[d.group];})填充颜色，.call(d3.drag(){})设置拖动开始、拖动进行和拖动结束的函数。

```
<script type="text/javascript">
        //定义d3.json请求python处理好的节点及边 请求成功返回数据，否则报错
	    d3.json("starwar_alldata.json", function(error, data) {
	    	if(error) throw error;
	    	graph = data;
	    	console.log(graph);

	    	//绘制所有边
	    	var link = svg.append("g").attr("class","links").selectAll("line").data(graph.links).enter()
	    	.append("line").attr("stroke-width", function(d) {
	    		//return Math.sqrt(d.value);
	    		return 1; //所有线宽度均为1
	    	});

	    	//添加所有的点
	    	var node = svg.append("g").attr("class", "nodes").selectAll("circle").data(graph.nodes).enter()
	    	.append("circle").attr("r", function(d) {
	    		return d.size;
	    	}).attr("fill", function(d) {
	    		return colors[d.group];
	    	}).attr("stroke", "none").attr("name", function(d) {
	    		return d.id;
	    	}).call(d3.drag()
	    		.on("start", dragstarted)
	    		.on("drag", dragged)
	    		.on("end", dragended)
	    	);

	    	//显示所有的文本 
	    	var text = svg.append("g").attr("class", "texts").selectAll("text").data(graph.nodes).enter()
	    	.append("text").attr("font-size", function(d) {
                return d.size;
            }).attr("fill", function(d) {
                return colors[d.group];
            }).attr('name', function(d) {
                return d.id;
            }).text(function(d) {
                return d.id;
            }).attr('text-anchor', 'middle').call(d3.drag()
                .on("start", dragstarted)
                .on("drag", dragged)
                .on("end", dragended)
            );
	    });
</script>
```

**3.在d3.json()中定义simulation()和ticked()函数确定link线的起始点x、y坐标，node中心点，文本通过translate平移变化。**

```
<script type="text/javascript">
            //simulation中ticked数据初始化并生成图形
            simulation
	            .nodes(graph.nodes)
	            .on("tick", ticked);

	        simulation.force("link")
	            .links(graph.links);

	        //ticked()函数确定link线的起始点x、y坐标 node确定中心点 文本通过translate平移变化
	        function ticked() {
	            link
	                .attr("x1", function(d) {
	                    return d.source.x;
	                })
	                .attr("y1", function(d) {
	                    return d.source.y;
	                })
	                .attr("x2", function(d) {
	                    return d.target.x;
	                })
	                .attr("y2", function(d) {
	                    return d.target.y;
	                });

	            node
	                .attr("cx", function(d) {
	                    return d.x;
	                })
	                .attr("cy", function(d) {
	                    return d.y;
	                });

	            text.
	            attr('transform', function(d) {
	                return 'translate(' + d.x + ',' + (d.y + d.size / 2) + ')';
	            });
</script>
```

**4.最后定义拖动函数dragstarted(d)、function dragged(d)、function dragended(d)。**


**完整代码如下所示：**

```
<!DOCTYPE html>
<html>

<head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>知识图谱</title>
    <meta name="description" content="" />
    <meta name="keywords" content="" />
    <meta name="author" content="" />
    <link rel="shortcut icon" href="">
    <script src="http://cdn.bootcss.com/jquery/2.1.4/jquery.min.js"></script>
    <link href="http://cdn.bootcss.com/bootstrap/3.3.4/css/bootstrap.min.css" rel="stylesheet">
    <script src="http://cdn.bootcss.com/bootstrap/3.3.4/js/bootstrap.min.js"></script>
</head>
<style type="text/css">
body {
	background-color: #272b30;
	padding: 30px 40px;
	text-align: center;
    font-family: OpenSans-Light, PingFang SC, Hiragino Sans GB, Microsoft Yahei, Microsoft Jhenghei, sans-serif;
}

#indicator {
	position: absolute; 
	left: 60px;
	bottom: 120px;
    text-align: left;
    color: #f2f2f2;
    font-size: 12px;
}

#indicator>div {
    margin-bottom: 4px;
}

#indicator span {
    display: inline-block;
    width: 30px;
    height: 14px;
    position: relative;
    top: 2px;
    margin-right: 8px;
}

</style>
<body>

	<!-- 绘制标题样式 -->
	<h1 style="color:#fff;font-size:32px;margin-bottom:0px;text-align:center;margin-left:40px;">Star Wars</h1>

	<!-- 第一个布局 绘制知识图谱主图 -->
	<div style="text-align: center; position:relative;">
		<svg width="800" height="560" style="margin-right:80px;margin-bottom:-40px;" id="svg1">
    	</svg>

    	<!-- 绘制图例 -->
		<div id="indicator">
	    </div>

	    <!-- 绘制模式选择 -->
	    <div id="mode">
	    </div>

	    <!-- 绘制搜索框 -->
	    <div id="search">
	    </div>

	    <!-- 绘制右边显示结果 -->
	    <div id="info">
	    </div>
	</div>

	<!-- 第二个布局 下部分时间点 文本居中 相对定位-->
	<div style="text-align: center; position:relative;">
		<svg width="960" height="240" style="margin-right:60px;margin-bottom:-40px;" id="svg1">
			<g></g>
    	</svg>
	</div>

</body>

<!-- 增加D3元素库 -->
<script src="https://d3js.org/d3.v4.min.js"></script>
<!-- 补充JS代码 -->
<script type="text/javascript">
	$(document).ready(function() {
		//定义svg变量将布局svg1选出来 
		var svg = d3.select("#svg1"), 
			width = svg.attr("width"), 
			height = svg.attr("height");
		
		//定义name变量制作图标
		var names = ['Films', 'Characters', 'Planets', 'Starships', 'Vehicles', 'Species'];
		var colors = ['#6ca46c', '#4e88af', '#ca635f', '#d2907c', '#d6744d', '#ded295'];

		//背景颜色设置 补充CSS样式设置字体布局
		for (var i=0; i < names.length; i++) {
			$('#indicator').append("<div><span style='background-color:" + colors[i] + "'></span>" + names[i] + "</div>");
		}

		//利用d3.forceSimulation()定义关系图 包括设置边link、排斥电荷charge、关系图中心点
		var simulation = d3.forceSimulation()
	        .force("link", d3.forceLink().id(function(d) {
	            return d.id;
	        }))
	        .force("charge", d3.forceManyBody())
	        .force("center", d3.forceCenter(width / 2, height / 2));

	    //存储关系图的数据
	    var graph;

	    //定义d3.json请求python处理好的节点及边 请求成功返回数据，否则报错
	    d3.json("starwar_alldata.json", function(error, data) {
	    	if(error) throw error;
	    	graph = data;
	    	console.log(graph);

	    	//D3映射数据至HTML中
	    	//g用于绘制所有边,selectALL选中所有的line,并绑定数据data(graph.links),enter().append("line")添加元素
	    	//数据驱动文档,设置边的粗细
	    	var link = svg.append("g").attr("class","links").selectAll("line").data(graph.links).enter()
	    	.append("line").attr("stroke-width", function(d) {
	    		//return Math.sqrt(d.value);
	    		return 1; //所有线宽度均为1
	    	});

	    	//添加所有的点
	    	//selectAll("circle")选中所有的圆并绑定数据,圆的直径为d.size
	    	//再定义圆的填充色,同样数据驱动样式,圆没有描边,圆的名字为d.id
	    	//call()函数：拖动函数,当拖动开始绑定dragstarted函数，拖动进行和拖动结束也绑定函数
	    	var node = svg.append("g").attr("class", "nodes").selectAll("circle").data(graph.nodes).enter()
	    	.append("circle").attr("r", function(d) {
	    		return d.size;
	    	}).attr("fill", function(d) {
	    		return colors[d.group];
	    	}).attr("stroke", "none").attr("name", function(d) {
	    		return d.id;
	    	}).call(d3.drag()
	    		.on("start", dragstarted)
	    		.on("drag", dragged)
	    		.on("end", dragended)
	    	);

	    	//显示所有的文本 
	    	//设置大小、填充颜色、名字、text()设置文本
	    	//attr("text-anchor", "middle")设置文本居中
	    	var text = svg.append("g").attr("class", "texts").selectAll("text").data(graph.nodes).enter()
	    	.append("text").attr("font-size", function(d) {
                return d.size;
            }).attr("fill", function(d) {
                return colors[d.group];
            }).attr('name', function(d) {
                return d.id;
            }).text(function(d) {
                return d.id;
            }).attr('text-anchor', 'middle').call(d3.drag()
                .on("start", dragstarted)
                .on("drag", dragged)
                .on("end", dragended)
            );

	    	//圆增加title
	    	node.append("title").text(function(d) {
	    		return d.id;
	    	})
	    	
	    	//simulation中ticked数据初始化并生成图形
	    	simulation
	            .nodes(graph.nodes)
	            .on("tick", ticked);

	        simulation.force("link")
	            .links(graph.links);

	        //ticked()函数确定link线的起始点x、y坐标 node确定中心点 文本通过translate平移变化
	        function ticked() {
	            link
	                .attr("x1", function(d) {
	                    return d.source.x;
	                })
	                .attr("y1", function(d) {
	                    return d.source.y;
	                })
	                .attr("x2", function(d) {
	                    return d.target.x;
	                })
	                .attr("y2", function(d) {
	                    return d.target.y;
	                });

	            node
	                .attr("cx", function(d) {
	                    return d.x;
	                })
	                .attr("cy", function(d) {
	                    return d.y;
	                });

	            text.
	            attr('transform', function(d) {
	                return 'translate(' + d.x + ',' + (d.y + d.size / 2) + ')';
	            });
	        }
	    });


	    // Cross origin requests are only supported for protocol schemes: http, data, chrome, chrome-extension
	    // 本地json数据需要放置服务器中请求 XAMPP


	    //拖动函数代码
	    //var dragging = false;

	    //开始拖动并更新相应的点
	    function dragstarted(d) {
	        if (!d3.event.active) simulation.alphaTarget(0.3).restart();
	        d.fx = d.x;
	        d.fy = d.y;
	        //dragging = true;
	    }

	    //拖动进行中
	    function dragged(d) {
	        d.fx = d3.event.x;
	        d.fy = d3.event.y;
	    }

	    //拖动结束
	    function dragended(d) {
	        if (!d3.event.active) simulation.alphaTarget(0);
	        d.fx = null;
	        d.fy = null;
	        //dragging = false;
	    }

	});
</script>
</html>
```

此时的运行结果如下图所示，无法看见边的颜色和节点，接下来需要简单设置节点和边的样式。
![](https://img-blog.csdnimg.cn/20190212173026367.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 三.CSS设置节点样式

CSS代码如下：

1.设置线条的颜色和透明度。

2.设置节点的描边和圆描边的宽度。

3.文字设置为不显示。
```
<style type="text/css">
.links line {
	stroke: rgb(240, 240, 240); 
	stroke-opactity: 0.2;
}

.nodes circle {
	stroke: #fff;
	stroke-width: 1.5px;
}

.texts text {
    display: none;
}
</style>
```

最终输出结果如下：
![](https://img-blog.csdnimg.cn/20190212170153248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
下载地址如下：[https://download.csdn.net/download/eastmount/10954282](https://download.csdn.net/download/eastmount/10954282)


（By:Eastmount 2019-02-12 下午6点 [http://blog.csdn.net/eastmount/）](http://blog.csdn.net/eastmount/%EF%BC%89)









