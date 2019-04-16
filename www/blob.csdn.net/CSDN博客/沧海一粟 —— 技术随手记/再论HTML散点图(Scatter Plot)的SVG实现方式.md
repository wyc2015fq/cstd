# 再论HTML散点图(Scatter Plot)的SVG实现方式 - 沧海一粟 —— 技术随手记 - CSDN博客





2017年06月13日 17:04:06[慢游](https://me.csdn.net/eengel)阅读数：855








# 介绍

在上一篇《[HTML散点图(Scatter Plot)的三种不同实现方式性能比较](http://blog.csdn.net/eengel/article/details/72967170)》中，重点比较了散点图的SVG实现方式和HTML5
 Canvas实现方式的不同。这儿再详细看看使用SVG，预先生成节点和实时加载数据之间的区别。

这儿为了在性能上有所比较，因此将散点数目增加到250,000，并且用于绘制散点的csv文件由data-generator.py统一生成。源代码可在[此处](https://github.com/slowlyswimming/scatter-charts-comparison)下载。

data-generator.py




```python
import pandas as pd
import numpy as np

df = pd.DataFrame(np.random.randint(0,500,size=(250000, 2)), columns=list('xy'))
df.to_csv('./data.csv',index=False)
```



# 实现方式

## 1. 预先用后端代码在HTML页面中生成250000个<circle>节点

pre-render-with-csv.js




```java
var el = window.document.querySelector('#dataviz-container')
			, body = window.document.querySelector('body')
			, circleId = 'a2324'  // say, this value was dynamically retrieved from some database

		
		//load data from csv file
		//d3.csv('data.csv', function(data){
		fs.readFile('data.csv', 'utf8', function (err, data) {
			data = d3.csvParse(data);
			
			data.forEach(function(d){
			    d.x = +d.x;
			    d.y = +d.y;
			});
			console.log(data.length);
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
					.attr('cx', function(d){return d.x;})
	    			.attr('cy', function(d){return d.y;});

	    	// save result in an html file, we could also keep it in memory, or export the interesting fragment into a database for later use
			var svgsrc = window.document.documentElement.innerHTML;
			fs.writeFile('index-svg-d3-csv-pre-render.html', svgsrc, function(err) {
				if(err) {
					console.log('error saving document', err);
				} else {
					console.log('The file was saved!');
				}
			});
		});
```



## 2. 预先用后端代码在HTML页面中生成100000个<circle>节点。剩余的节点用d3.js代码在页面加载时实时生成

index-svg-d3-csv-partial-render.html


代码除了有预先存在的100000个<circle>节点之外，其他代码与index-svg-d3-csv.html相同。




## 3. 所有250000个<circle>节点用d3.js代码在页面加载时实时生成



index-svg-d3-csv.html



```java
//load data from csv file
		d3.csv('data.csv', function(data){
			data.forEach(function(d){
			    d.x = +d.x;
			    d.y = +d.y;
			});

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
					.attr('cx', function(d){return d.x;})
	    			.attr('cy', function(d){return d.y;});
			});
```


# 结果比较



由于三种实现的最后呈现效果一样，主要就是看一下性能，即页面的加载时间，见图1。

如果从最终加载完毕的时间来看，第3种实现用时最短，第1种用时最长。与上一篇博客里的观察一致。由于服务器就在本地，因此加载csv文件的时间可以忽略不计。

如果从一开始出现散点来看，则是第1种用时最短，第3种用时最长。

从总体性能来看，如果将散点的数量增加到1,000,000，则加载用时将会达到40秒以上。显然，从用户体验来看，几十万的散点图性能将会超过个人所能忍受的程度。但如果能像第1种实现和第2种实现那样提前显示一部分散点，将会有效改善用户体验。

第2种实现和第1种相比，牺牲了一部分提前显示散点的时间，但降低了HTML页面的大小。

![](https://img-blog.csdn.net/20170613165618871?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


图1






