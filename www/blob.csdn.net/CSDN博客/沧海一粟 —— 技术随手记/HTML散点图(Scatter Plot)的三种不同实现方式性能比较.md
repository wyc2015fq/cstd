# HTML散点图(Scatter Plot)的三种不同实现方式性能比较 - 沧海一粟 —— 技术随手记 - CSDN博客





2017年06月09日 17:48:49[慢游](https://me.csdn.net/eengel)阅读数：6046








# 介绍

相对于其他图表类型而言，散点图往往会接收大数据做为输入，而需要同时显示上万甚至上百万的数据点在图上。对于基于HTML的散点图来说，其性能无疑是一个非常重要的考虑因素。

下面就对比一下3种不同的散点图实现，来看看其性能如何。实现的源码可在[这儿](https://github.com/slowlyswimming/scatter-charts-comparison)下载。为了有可比性，这儿的散点图都是在一个500*500的区域内，绘制10,0000个半径为1的绿色点。其位置完全随机。



# 散点图实现

1. 基于SVG的散点图实现

SVG被广泛用于 基于HTML的图表实现。而用SVG实现散点图也很简单。在HTML中建一个<svg>节点，再生成相应数量的<circle>节点就可以。

因为要生成的节点比较多，因此这儿借用了node.js，先在服务器端用d3+jsdom生成了HTML页面[1]，再在浏览器上直接加载。

pre-render.js



```java
var el = window.document.querySelector('#dataviz-container')
			, body = window.document.querySelector('body')
			, circleId = 'a2324'  // say, this value was dynamically retrieved from some database

		
		// set sample data
		data = [];
		for(var i=0; i<100000;  i++){
			data.push(1);
		}

		// generate the dataviz
		var svg = d3.select(el)
			.append('svg:svg')
				.attr('width', 500)
				.attr('height', 500);

		svg.selectAll('.circle')
			.data( data )
			.enter()
				.append('circle')
				.attr('r', 1)
				.attr('fill', '#26963c')
				.attr('cx', function(){ return Math.floor(Math.random() * 1000); })
    			.attr('cy', function(){ return Math.floor(Math.random() * 1000); });
```


index1.html




`<div id="dataviz-container"><svg width="500" height="500"><circle r="1" fill="#26963c" cx="406" cy="619">more circle nodes...</circle></svg></div>`


2. 基于SVG+d3.js的散点图实现

这个实现与上一个实现的不同之处在于，这儿不直接在HTML中罗列所有的<circle>节点，而是通过d3.js代码在浏览器加载时实时生成。

index2.html



```
<div id="dataviz-container"></div>
	<script src="d3.v3.min.js"></script>
	<script>
		// set sample data
		data = [];
		for(var i=0; i<100000;  i++){
			data.push(1);
		}

		// generate the dataviz
		var svg = d3.select('#dataviz-container')
			.append('svg:svg')
				.attr('width', 500)
				.attr('height', 500);

		svg.selectAll('.circle')
			.data( data )
			.enter()
				.append('circle')
				.attr('r', 1)
				.attr('fill', '#26963c')
				.attr('cx', function(){ return Math.floor(Math.random() * 1000); })
    			.attr('cy', function(){ return Math.floor(Math.random() * 1000); });
	</script>
```




3. 基于HTML5 Canvas的散点图实现

HTML5 Canvas支持在<canvas>节点中绘制各种图形，因此自然也支持散点图的绘制。同样的，这儿通过JavaScript代码在<canvas>节点中生成10,0000个圆形，设置其半径为1，填色绿色。

index3.html



```
<canvas id="myCanvas" width="500" height="500"></canvas>
	<script>
		var canvas=document.getElementById("myCanvas");
		var ctx=canvas.getContext("2d");

		ctx.beginPath();
		for(var i=0; i<100000; i++){
		  var cx = Math.floor(Math.random() * canvas.width);
		  var cy = Math.floor(Math.random() * canvas.height);
		  ctx.moveTo(cx,cy);
		  ctx.arc(cx,cy,1,0,Math.PI*2);
		}
		ctx.fillStyle='#26963c';
		ctx.fill();
	</script>
```





# 结果对比

首先来看看渲染后的效果。

从图1和图2可以看出，SVG绘制的散点图边缘更加清晰，颜色比较锐利，且放大后保持了原有的清晰度。而Canvas的散点图边缘更加模糊，虽然点的数目与SVG中一样，但看起来更加密集，点与点的区分也不清楚，放大后也不能区分点与点之间的距离。这样看起来，SVG对图形的渲染更胜一筹。

![](https://img-blog.csdn.net/20170609175250189?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


图1

![](https://img-blog.csdn.net/20170609175418094?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


图2

再看看图3中页面从一开始加载到完成散点图显示的用时。这个时候Canvas显示出了巨大的优势，用了大约300ms就完成了渲染。而SVG普遍需要约3000ms。单单从这个例子来看，SVG用时是Canvas的十倍。有意思的是两个SVG实现。与想象中不同，虽然第二个SVG实现还需要通过d3.js生成<circle>节点后再绑到<svg>上去，但其用时却比第一个直接有<circle>节点的实现要少。值得注意的是，如果只是算开始渲染的时间，也就是出现散点的时间，那么第一个实现其实是发生在600ms左右的时候。如果那个时候能先渲染关键形状，那么对于用户体验来说也是一个改进。而第二种实现，目前采用的是在JavaScript中生成散点数据。如果数据来源于后端，那么从后端加载数据也需要花一定的时候，要算在页面加载的总时间里。

![](https://img-blog.csdn.net/20170609175010001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


图3

# 总结

从上面的结果来看，如果需要比较清晰的、高分辨率的渲染，那么推荐用基于SVG的实现。如果比较注重性能，则HTML5Canvas更胜一筹。

当然，这次试验用的是比较原始的实现方式。而目前很多画图的框架在SVG或者Canvas的基础架构上，想必也做了非常多的优化。还是需要根据自身的需要，选择合适自己的。

# 参考文献

[1] Pre-render d3.js charts at server side. [https://mango-is.com/blog/engineering/pre-render-d3-js-charts-at-server-side/](https://mango-is.com/blog/engineering/pre-render-d3-js-charts-at-server-side/)



