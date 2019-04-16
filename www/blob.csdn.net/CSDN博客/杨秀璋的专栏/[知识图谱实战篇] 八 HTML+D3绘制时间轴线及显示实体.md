# [知识图谱实战篇] 八.HTML+D3绘制时间轴线及显示实体 - 杨秀璋的专栏 - CSDN博客





2019年02月18日 17:04:21[Eastmount](https://me.csdn.net/Eastmount)阅读数：284标签：[知识图谱																[关系图谱																[D3																[HTML																[Python](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[知识图谱实战																[关系图谱																[知识图谱																[HTML网页知识](https://blog.csdn.net/Eastmount/article/category/6334624)
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)](https://blog.csdn.net/Eastmount/article/category/5613671)




前面作者讲解了很多知识图谱原理知识，包括知识图谱相关技术、Neo4j绘制关系图谱等，但仍缺少一个系统全面的实例。为了加深自己对知识图谱构建的认识，为后续创建贵州旅游知识图谱打下基础，作者深入学习了张宏伦老师的网易云课程（[星球系列电影](https://study.163.com/course/courseLearn.htm?courseId=1003528010)），并结合自己的理解和技术分享了该系列专栏，从数据采集、数据展示、数据分析到知识图谱构建，文章后续还会讲解中文数据的实体识别、关系抽取、知识计算等。

前面通过七篇文章基本构建了电影知识图谱，并且能显示选中节点相关联的边及属性，本文主要采用HTML和D3绘制时间轴线及显示实体名称。如下图所示：
![](https://img-blog.csdnimg.cn/2019021817034630.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
代码下载地址：

这是一系列基础性文章，希望对您有所帮助 ，尤其是对知识图谱感兴趣和编程刚入门的同学。同时也因为最近准备博士考试，做题做吐了，写点新专栏调节下心情，与君共勉，一起加油。

**前文：**
[[知识图谱实战篇] 一.数据抓取之Python3抓取JSON格式的电影实体](https://blog.csdn.net/eastmount/article/details/86714051)
[[知识图谱实战篇] 二.Json+Seaborn可视化展示电影实体](https://blog.csdn.net/Eastmount/article/details/86739154)
[[知识图谱实战篇] 三.Python提取JSON数据、HTML+D3构建基本可视化布局](https://blog.csdn.net/Eastmount/article/details/86755610)
[[知识图谱实战篇] 四.HTML+D3+CSS绘制关系图谱](https://blog.csdn.net/Eastmount/article/details/87090486)
[[知识图谱实战篇] 五.HTML+D3添加鼠标响应事件显示相关节点及边](https://blog.csdn.net/Eastmount/article/details/87116136)
[[知识图谱实战篇] 六.HTML+D3实现点击节点显示相关属性及属性值](https://blog.csdn.net/Eastmount/article/details/87193405)
[[知识图谱实战篇] 七.HTML+D3实现关系图谱搜索功能](https://blog.csdn.net/Eastmount/article/details/87270150)




### 文章目录
- [一.Python获取Json时间数据](#PythonJson_53)
- [二.D3载入数据](#D3_216)
- [三.绘制电影名称和实体名称](#_256)
- [四.绘制实体方框](#_345)
- [五.设置CSS样式增加悬浮功能](#CSS_448)
- [六.完整代码](#_498)
- [总结](#_1209)





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
# 一.Python获取Json时间数据

首先，通过Python3获取时间代码，如下所示：
**json_timeline.py**

```python
# coding:utf8
import time
import json

#定义六类实体
films = []
characters = []
planets = []
starships = []
vehicles = []
species = []

fr = open('films.csv', 'r')
for line in fr:
    tmp = json.loads(line.strip('\n'))
    films.append(tmp)
fr.close()

fr = open('film_characters.csv', 'r')
for line in fr:
    tmp = json.loads(line.strip('\n'))
    characters.append(tmp)
fr.close()

fr = open('film_planets.csv', 'r')
for line in fr:
    tmp = json.loads(line.strip('\n'))
    planets.append(tmp)
fr.close()

fr = open('film_starships.csv', 'r')
for line in fr:
    tmp = json.loads(line.strip('\n'))
    starships.append(tmp)
fr.close()

fr = open('film_vehicles.csv', 'r')
for line in fr:
    tmp = json.loads(line.strip('\n'))
    vehicles.append(tmp)
fr.close()

fr = open('film_species.csv', 'r')
for line in fr:
    tmp = json.loads(line.strip('\n'))
    species.append(tmp)
fr.close()

#输出各类实体数量
#7 87 21 37 39 37
print(len(films),len(characters), len(planets), len(starships), len(vehicles), len(species))

data = []

#遍历每个人物在电影里是否出现过
for item in characters:
    tmp = []
    for film in films:
            flag = False #标志变量 初始值未出现过
            for f in film['characters']:
                    #如果人的url在电影f里出现过跳出循环
                    if item['url'] == f:
                            flag = True
                            break
            if flag: #如果人没有则添加至变量tmp中
                    tmp.append(1)
            else:
                    tmp.append(0) #没出现在这部电影里
    #数据存储至data变量中
    #名字 类型 组(着色) tmp对应是否出现向量
    data.append({'name': item['name'], 'type': 'character', 'group': 0, 'vector': tmp})

#星球
for item in planets:
    tmp = []
    for film in films:
        flag = False
        for f in film['planets']:
            if item['url'] == f:
                flag = True
                break
        if flag:
            tmp.append(1)
        else:
            tmp.append(0)
    data.append({'name': item['name'], 'type': 'planet', 'group': 1, 'vector': tmp})

for item in starships:
    tmp = []
    for film in films:
        flag = False
        for f in film['starships']:
            if item['url'] == f:
                flag = True
                break
        if flag:
            tmp.append(1)
        else:
            tmp.append(0)
    data.append({'name': item['name'], 'type': 'starship', 'group': 2, 'vector': tmp})
    
for item in vehicles:
    tmp = []
    for film in films:
        flag = False
        for f in film['vehicles']:
            if item['url'] == f:
                flag = True
                break
        if flag:
            tmp.append(1)
        else:
            tmp.append(0)
    data.append({'name': item['name'], 'type': 'vehicle', 'group': 3, 'vector': tmp})

for item in species:
    tmp = []
    for film in films:
        flag = False
        for f in film['species']:
            if item['url'] == f:
                flag = True
                break
        if flag:
            tmp.append(1)
        else:
            tmp.append(0)
    data.append({'name': item['name'], 'type': 'species', 'group': 4, 'vector': tmp})

#将电影存储
#将电影和数据装载至Json文件中 7部电影 221个节点
films = [[films[x]['title'], films[x]['release_date']] for x in range(0, len(films))]
result = {'films': films, 'data': data}

fw = open('all_timeline.json', 'w')
fw.write(json.dumps(result))
fw.close()
```

源文件如下所示：
![](https://img-blog.csdnimg.cn/20190218171019965.png)
运行结果如下所示，将获取7部电影为films变量（7个），实体为data变量（221个）。
**实体数据包括：名字、类型、组(着色)、vector对应是否出现向量电影数据包括：电影名称、时间**![](https://img-blog.csdnimg.cn/20190218170927466.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 二.D3载入数据

数据获取成功之后，先通过HTML和D3加载数据并显示。代码置于服务器中，如下所示：
![](https://img-blog.csdnimg.cn/2019021817144555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
D3载入数据的核心代码如下所示：

```
//请求时间轴数据
d3.json("all_timeline.json", function(error, data) {
	if(error) throw error;

	//前面HTML设置第二个布局 高度为240px 宽度960px，减去40px间距
	var height2 = 240 - 40;
	var width2 = 960;
	
	//打印节点
	console.log(data);
});
```

通过 console.log(data) 将数据显示出来如下图所示：
![](https://img-blog.csdnimg.cn/20190218171856222.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
实体数据包括：名字、类型、组(着色)、vector对应是否出现向量

电影数据包括：电影名称、时间
![](https://img-blog.csdnimg.cn/20190218170946641.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 三.绘制电影名称和实体名称

接着绘制电影名称（显示左部）和实体名称（显示上部），如下所示：
![](https://img-blog.csdnimg.cn/20190218172246280.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**1.HTML设置#svg2 g布局**

```
<div style="text-align: center; position:relative;">
	<svg width="960" height="240" style="margin-right:60px;margin-bottom:-40px;" id="svg2">
		<g></g>
	</svg>
</div>
```

**2.选择#svg2 g布局绑定数据，设置最左边的电影名称**

核心代码如下：

```
<script type="text/javascript">

//请求时间轴数据
d3.json("all_timeline.json", function(error, data) {
	if(error) throw error;

	//前面HTML设置第二个布局 高度为240px 宽度960px，减去40px间距
	var height2 = 240 - 40;
	var width2 = 960;

	
	/* 第一步 设置最左边的电影名称*/

	//选择#svg2 g布局绑定数据
	//selectAll('text.film'):选择所有text并且存在class属性为film的
	//加载的data数据包括：films和data两个值
	d3.select('#svg2 g').selectAll('text.film').data(data['films']).enter().append('text').text(function(d) {
		//获取文本 d[0]文本 d[1]上映时间
	    return d[0]; 
	}).attr('transform', function(d, i) {
		//添加变化transform，其中translate表示平移 
		//x方向平移固定值150，y方向：平移初始值40、i表示第几条
		//height2/data['films'].length)表示高度除以7部电影，即每部显影平均下来的高度
	    return 'translate(150,' + (40 + (i + 0.5) * height2 / data['films'].length) + ')';
	}).attr('fill', '#fff').attr('font-size', 12)
	.attr('text-anchor', 'end').attr('class', 'film'); //填充色、字体大小、文本右对齐、class属性


	//打印节点
	console.log(data);
});

</script>
```

此时显示结果如下所示：
![](https://img-blog.csdnimg.cn/20190218172654858.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.接着设置X轴顶部显示实体名称，与上面相同的方法**

核心代码如下：

```
//获取#svg2 g中的text.title
//data(data['data'])获取具体的数据
d3.select('#svg2 g').selectAll('text.title').data(data['data']).enter().append('text').text(function(d) {
    return d['name']; //表示实体的名称
}).attr('name',function(d) {
    return d['name']; //取个名字name属性
}).attr('transform', function(d, i) {
	//横向平移 y轴方向均为25
    return 'translate(' + (165 + i * (width2 - 165) / data['data'].length) + ',25)';
}).attr('fill', '#fff').attr('font-size', 12).attr('text-anchor', 'middle')
.attr('class', 'title'); //透明度设置为0 .attr('fill-opacity', 0)
```

此时将所有实体名称均显示出来，效果非常差，密密麻麻的。
![](https://img-blog.csdnimg.cn/20190218173001426.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
接着我们需要实现的是鼠标移动到的显示，先设置透明度为0，增加如下代码：

.attr(‘class’, ‘title’)**.attr(‘fill-opacity’, 0);**

# 四.绘制实体方框

接着绘制方框矩阵，循环对每个节点#svg2 g再增加一个g，并定义一个id值；选中第二个g中所有的矩形rect，再绑定数据tmp[‘vector’]。

核心代码如下：
![](https://img-blog.csdnimg.cn/20190218173528368.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
此时该部分代码如下所示：

```
<script type="text/javascript">
//请求时间轴数据
d3.json("all_timeline.json", function(error, data) {
	if(error) throw error;

	//前面HTML设置第二个布局 高度为240px 宽度960px，减去40px间距
	var height2 = 240 - 40;
	var width2 = 960;

	
	/* 第一步 设置最左边的电影名称*/

	//选择#svg2 g布局绑定数据
	//selectAll('text.film'):选择所有text并且存在class属性为film的
	//加载的data数据包括：films和data两个值
	d3.select('#svg2 g').selectAll('text.film').data(data['films']).enter().append('text').text(function(d) {
		//获取文本 d[0]文本 d[1]上映时间
	    return d[0]; 
	}).attr('transform', function(d, i) {
		//添加变化transform，其中translate表示平移 
		//x方向平移固定值150，y方向：平移初始值40、i表示第几条
		//height2/data['films'].length)表示高度除以7部电影，即每部显影平均下来的高度
	    return 'translate(150,' + (40 + (i + 0.5) * height2 / data['films'].length) + ')';
	}).attr('fill', '#fff').attr('font-size', 12)
	.attr('text-anchor', 'end').attr('class', 'film'); //填充色、字体大小、文本右对齐、class属性


	/* 第二步 设置X轴顶部显示实体名称*/

	//获取#svg2 g中的text.title
	//data(data['data'])获取具体的数据
	d3.select('#svg2 g').selectAll('text.title').data(data['data']).enter().append('text').text(function(d) {
	    return d['name']; //表示实体的名称
	}).attr('name',function(d) {
	    return d['name']; //取个名字name属性
	}).attr('transform', function(d, i) {
		//横向平移 y轴方向均为25
	    return 'translate(' + (165 + i * (width2 - 165) / data['data'].length) + ',25)';
	}).attr('fill', '#fff').attr('font-size', 12).attr('text-anchor', 'middle')
	.attr('class', 'title').attr('fill-opacity', 0); 
	//透明度设置为0 .attr('fill-opacity', 0)


	/* 第三步 绘制方框矩阵 */

    //矩形颜色 没有电影五种颜色
    var colors2 = ['#4e88af', '#ca635f', '#d2907c', '#d6744d', '#ded295'];

    for (var i = 0; i < data['data'].length; i++) {

    	//获取第i个值
        var tmp = data['data'][i];

        //对每个节点#svg2 g再增加一个g，并定义一个id值
        //选中第二个g中所有的矩形rect，再绑定数据tmp['vector']
        //设置宽度、长度、填充色
        d3.select('#svg2 g').append('g').attr('id', tmp['name']).attr('class', 'row')
            .selectAll('rect').data(tmp['vector']).enter().append('rect')
            .attr('width', Math.floor((width2 - 165) / data['data'].length))
            .attr('height', (height2 / data['films'].length - 1))
            .attr('fill', function(d, j) {
                return colors2[tmp['group']];
            }).attr('transform', function(d, j) {
            	//矩形平移值 X轴表示实体i的宽度 Y轴表示高度
                return 'translate(' + (165 + i * (width2 - 165) / data['data'].length) + ',' 
                	+ (40 + j * height2 / data['films'].length) + ')';
            }).attr('fill-opacity', function(d) {
            	//设置透明度
                if (d == 1) {
                    return 1; //显示
                }
                else if (d == 0) {
                    return 0; //隐藏
                }
            });
    } //end for


	//打印节点
	console.log(data);
});
</script>
```

此时显示的结果如下所示：
![](https://img-blog.csdnimg.cn/20190218173658104.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 五.设置CSS样式增加悬浮功能

接着增加悬浮功能，选中的实体增加描边并显示其对应的名称。

**1.设置CSS样式**

之前通过JS为第二个g增加了一个属性row，现在设置它的样式 attr(‘class’, ‘row’)。代码如下：

```
#svg2 g.row:hover {
    stroke-width: 1px;
    stroke: #fff;
}
```

改代码为设置鼠标悬浮样式，增加描边长度和颜色。此时，鼠标悬浮上去显示了一个白色边框，接着增加相关实体的名称。
![](https://img-blog.csdnimg.cn/20190218174025611.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.设置鼠标选中节点显示其实体名称**

```
/* 第四步 选中节点显示对应的实体名称 */

//鼠标选中g.row则显示该名称
$('#svg2').on('mouseenter', 'g.row', function(event) {
    event.preventDefault();
    //#svg2 text.title透明度设置为0
    $('#svg2 text.title').attr('fill-opacity',0);
    console.log($(this).attr('id'));

    //设置名称
    $('#svg2 text.title[name="' + $(this).attr('id') + '"]').attr('fill-opacity', 1);
});

//鼠标移开名字隐藏
$('#svg2').on('mouseleave', 'g.row', function(event) {
    event.preventDefault();
    $('#svg2 text.title').attr('fill-opacity',0);
});
```

对应结果如下所示：
![](https://img-blog.csdnimg.cn/20190218174253353.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 六.完整代码

最终完整代码如下所示：

**完整代码**

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

.links line {
	stroke: rgb(240, 240, 240); 
	stroke-opactity: 0.2;
}

.links line.inactive {
    /*display: none !important;*/
    stroke-opacity: 0;
}

.nodes circle {
	stroke: #fff;
	stroke-width: 1.5px;
}

.nodes circle:hover {
    cursor: pointer;
}

.nodes circle.inactive {
	display: none !important;
}

.texts text {
    display: none;
}

.texts text:hover {
    cursor: pointer;
}

.texts text.inactive {
    display: none !important;
}



#mode {
    position: absolute;
    top: 160px;
    left: 60px;
}

#mode span {
    display: inline-block;
    border: 1px solid #fff;
    color: #fff;
    padding: 6px 10px;
    border-radius: 4px;
    font-size: 14px;
    transition: color, background-color .3s;
    -o-transition: color, background-color .3s;
    -ms-transition: color, background-color .3s;
    -moz-transition: color, background-color .3s;
    -webkit-transition: color, background-color .3s;
}

#mode span.active, #mode span:hover {
    background-color: #fff;
    color: #333;
    cursor: pointer;
}


#info {
    position: absolute;
    bottom: 40px;
    right: 30px;
    text-align: right;
    width: 270px;
}


#info p {
    color: #fff;
    font-size: 12px;
    margin-top: 0;
    margin-bottom: 5px;
}

#info p span {
    color: #888;
    margin-right: 10px;
}

#info h4 {
    color: #fff;
}


#search input {
    position: absolute;
    top: 220px;
    left: 60px;
    color: #fff;
    border: none;
    outline: none;
    box-shadow: none;
    width: 200px;
    background-color: #666;
}

#svg2 g.row:hover {
    stroke-width: 1px;
    stroke: #fff;
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
	    	<span class="active" style="border-top-right-radius:0;border-bottom-right-radius:0;">节点</span>
            <span style="border-top-left-radius:0;border-bottom-left-radius:0;position:relative;left:-5px;">文字</span>
	    </div>

	    <!-- 绘制搜索框 -->
	    <div id="search">
	    	<input type="text" class="form-control">
	    </div>

	    <!-- 绘制右边显示结果 -->
	    <div id="info">
	    	<h4></h4>
	    </div>
	</div>

	<!-- 第二个布局 下部分时间点 文本居中 相对定位-->
	<div style="text-align: center; position:relative;">
		<svg width="960" height="240" style="margin-right:60px;margin-bottom:-40px;" id="svg2">
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
	    	//前面定义var svg = d3.select("#svg1")
	    	var link = svg.append("g").attr("class","links").selectAll("line").data(graph.links)
	    	.enter().append("line").attr("stroke-width", function(d) {
	    		//return Math.sqrt(d.value);
	    		return 1; //所有线宽度均为1
	    	});

	    	//添加所有的点
	    	//selectAll("circle")选中所有的圆并绑定数据,圆的直径为d.size
	    	//再定义圆的填充色,同样数据驱动样式,圆没有描边,圆的名字为d.id
	    	//call()函数：拖动函数,当拖动开始绑定dragstarted函数，拖动进行和拖动结束也绑定函数
	    	var node = svg.append("g").attr("class", "nodes").selectAll("circle").data(graph.nodes)
	    	.enter().append("circle").attr("r", function(d) {
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
	    	//attr("text-anchor", "middle")文本居中
	    	var text = svg.append("g").attr("class", "texts").selectAll("text").data(graph.nodes)
	    	.enter().append("text").attr("font-size", function(d) {
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

	    	//simulation中ticked数据初始化，并生成图形
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


	    //该变量保证拖动鼠标时，不会影响图形变换，默认为false未选中鼠标
	    var dragging = false;

	    //开始拖动并更新相应的点
	    function dragstarted(d) {
	        if (!d3.event.active) simulation.alphaTarget(0.3).restart();
	        d.fx = d.x;
	        d.fy = d.y;
	        dragging = true;
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
	        dragging = false;
	    }


	    //span点击事件
	    $('#mode span').click(function(event) {
	    	//span都设置为不激活状态
	        $('#mode span').removeClass('active');

	        //点击的span被激活
	        $(this).addClass('active');

	        //text隐藏 nodes显示
	        if ($(this).text() == '节点') {
	            $('.texts text').hide();
	            $('.nodes circle').show();
	        } else {
	            $('.texts text').show();
	            $('.nodes circle').hide();
	        }
    	});

	    //为svg1父元素下的.nodes circle元素绑定鼠标进入事件
    	$('#svg1').on('mouseenter', '.nodes circle', function(event) {
    		//通过变量dragging保证拖动鼠标时，其状态不受影响，从而改变图形
    		//鼠标没有拖动才能处理事件
    		if(!dragging) {
    			//获取被选中元素的名字
	    		var name = $(this).attr("name");

	    		//设置#info h4样式的颜色为该节点的颜色，文本为该节点name
	    		//$(this).attr('fill')表示当前悬浮圆的填充色
	    		$('#info h4').css('color', $(this).attr('fill')).text(name);

	    		//每次点击添加属性前把上次显示的信息去除，否则会不断叠加
	    		$('#info p').remove();

	    		//打印悬浮的节点信息
	    		//console.log(info[name]);

	    		//遍历所有的
	    		for (var key in info[name]) {
	    			//类型复杂的不进行显示
	    			if (typeof(info[name][key]) == 'object') {
                    	continue;
                	}
                	//比较复杂的超链接字段不显示
	                if (key == 'url' || key == 'title' || key == 'name' || 
	                	key == 'edited' || key == 'created' || key == 'homeworld') {
	                    continue;
	                }
	                //显示值及其字段名字
	                $('#info').append('<p><span>' + key + '</span>' + info[name][key] + '</p>');
	    		}


	    		//选择#svg1 .nodes中所有的circle，再增加个class
	    		d3.select('#svg1 .nodes').selectAll('circle').attr('class', function(d) {
	    			//数据的id是否等于name,返回空
	    			if(d.id==name) {
	    				return '';
	    			} 
	    			//当前节点返回空，否则其他节点循环判断是否被隐藏起来(CSS设置隐藏)
	    			else {
	    				//links链接的起始节点进行判断,如果其id等于name则显示这类节点
	    				//注意: graph=data
	    				for (var i = 0; i < graph.links.length; i++) {
	    					//如果links的起点等于name，并且终点等于正在处理的则显示
		                    if (graph.links[i]['source'].id == name && graph.links[i]['target'].id == d.id) {
		                        return '';
		                    }
		                    if (graph.links[i]['target'].id == name && graph.links[i]['source'].id == d.id) {
		                        return '';
		                    }
		                }
	                
	    				return "inactive"; //前面CSS定义 .nodes circle.inactive
	    			}
	    		});

	    		//处理相邻的边line是否隐藏 注意 || 
	    		d3.select("#svg1 .links").selectAll('line').attr('class', function(d) {
	                if (d.source.id == name || d.target.id == name) {
	                    return '';
	                } else {
	                    return 'inactive';
	                }
	            });
    		}
   		});

    	//鼠标移开还原原图，显示所有隐藏的点及边
    	$('#svg1').on('mouseleave', '.nodes circle', function(event) {
    		//如果dragging为false才处理事件
    		if(!dragging) {
    			d3.select('#svg1 .nodes').selectAll('circle').attr('class', '');
           		d3.select('#svg1 .links').selectAll('line').attr('class', '');
    		} 
	    });


    	//鼠标进入文本显示相邻节点及边
    	$('#svg1').on('mouseenter', '.texts text', function(event) {
	        if (!dragging) {
	            var name = $(this).attr('name');

	            //同样的代码从选中圆中赋值过来
	            $('#info h4').css('color', $(this).attr('fill')).text(name);
	            $('#info p').remove();
	            for (var key in info[name]) {
	                if (typeof(info[name][key]) == 'object') {
	                    continue;
	                }
	                if (key == 'url' || key == 'title' || key == 'name' || key == 'edited' || key == 'created' || key == 'homeworld') {
	                    continue;
	                }
	                $('#info').append('<p><span>' + key + '</span>' + info[name][key] + '</p>');
	            }

	            d3.select('#svg1 .texts').selectAll('text').attr('class', function(d) {
	                if (d.id == name) {
	                    return '';
	                }

	                for (var i = 0; i < graph.links.length; i++) {
	                    if (graph.links[i]['source'].id == name && graph.links[i]['target'].id == d.id) {
	                        return '';
	                    }
	                    if (graph.links[i]['target'].id == name && graph.links[i]['source'].id == d.id) {
	                        return '';
	                    }
	                }
	                return 'inactive';
	            });
	            d3.select("#svg1 .links").selectAll('line').attr('class', function(d) {
	                if (d.source.id == name || d.target.id == name) {
	                    return '';
	                } else {
	                    return 'inactive';
	                }
	            });
	        }
	    });

    	//鼠标移除文本还原相应节点及边
	    $('#svg1').on('mouseleave', '.texts text', function(event) {
	        if (!dragging) {
	            d3.select('#svg1 .texts').selectAll('text').attr('class', '');
	            d3.select('#svg1 .links').selectAll('line').attr('class', '');
	        }
	    });


	    //搜索框中输入内容则响应该事件
	    //keyup按键敲击响应event
	    $('#search input').keyup(function(event) {
	    	//如果Input值是空的显示所有的圆和线(没有进行筛选)
	        if ($(this).val() == '') {
	            d3.select('#svg1 .texts').selectAll('text').attr('class', '');
	            d3.select('#svg1 .nodes').selectAll('circle').attr('class', '');
	            d3.select('#svg1 .links').selectAll('line').attr('class', '');
	        }
	        //否则判断判断三个元素是否等于name值，等于则显示该值
	        else {
	            var name = $(this).val();
	            //搜索所有的节点
	            d3.select('#svg1 .nodes').selectAll('circle').attr('class', function(d) {
	            	//输入节点id的小写等于name则显示，否则隐藏
	                if (d.id.toLowerCase().indexOf(name.toLowerCase()) >= 0) {
	                    return '';
	                } else {
	                	//优化：与该搜索节点相关联的节点均显示
	                	//links链接的起始节点进行判断,如果其id等于name则显示这类节点
	    				//注意: graph=data
	    				for (var i = 0; i < graph.links.length; i++) {
	    					//如果links的起点等于name，并且终点等于正在处理的则显示
		                    if ((graph.links[i]['source'].id.toLowerCase().indexOf(name.toLowerCase()) >= 0) && 
		                    	(graph.links[i]['target'].id == d.id)) {
		                        return '';
		                    }
		                    //如果links的终点等于name，并且起点等于正在处理的则显示
		                    if ((graph.links[i]['target'].id.toLowerCase().indexOf(name.toLowerCase()) >= 0) && 
		                    	(graph.links[i]['source'].id == d.id)) {
		                        return '';
		                    }
		                }  

		                return 'inactive'; //隐藏其他节点  
	                }

	            });


	            //搜索texts
	            d3.select('#svg1 .texts').selectAll('text').attr('class', function(d) {
	                if (d.id.toLowerCase().indexOf(name.toLowerCase()) >= 0) {
	                    return '';
	                } else {
	                	//优化：与该搜索节点相关联的节点均显示
	                	//links链接的起始节点进行判断,如果其id等于name则显示这类节点
	    				//注意: graph=data
	    				for (var i = 0; i < graph.links.length; i++) {
	    					//如果links的起点等于name，并且终点等于正在处理的则显示
		                    if ((graph.links[i]['source'].id.toLowerCase().indexOf(name.toLowerCase()) >= 0) && 
		                    	(graph.links[i]['target'].id == d.id)) {
		                        return '';
		                    }
		                    //如果links的终点等于name，并且起点等于正在处理的则显示
		                    if ((graph.links[i]['target'].id.toLowerCase().indexOf(name.toLowerCase()) >= 0) && 
		                    	(graph.links[i]['source'].id == d.id)) {
		                        return '';
		                    }
		                }  

	                    return 'inactive'; //隐藏其他节点
	                }
	            });


	            //搜索links 所有与搜索name相关联的边均显示
	            //显示相的邻边 注意 || 
		        //name=$(this).val()：名字为键盘输入的内容
	            d3.select("#svg1 .links").selectAll('line').attr('class', function(d) {
	            	if ((d.source.id.toLowerCase().indexOf(name.toLowerCase()) >= 0) || 
	            		(d.target.id.toLowerCase().indexOf(name.toLowerCase()) >= 0) 
	            		) {
	                    return '';
	                } else {
	                    return 'inactive'; //隐藏
	                }
	            });
	        }
	    }); //end input


	    //加载Python获取的Json信息：六类实体详细属性信息
	    var info;

	    //d3.json获取数据
	    d3.json("all_data.json", function(error, data) {
	    	if(error) throw error;
	    	info = data;
	    });

		//请求时间轴数据
		d3.json("all_timeline.json", function(error, data) {
			if(error) throw error;

			//前面HTML设置第二个布局 高度为240px 宽度960px，减去40px间距
			var height2 = 240 - 40;
			var width2 = 960;

			
			/* 第一步 设置最左边的电影名称*/

			//选择#svg2 g布局绑定数据
			//selectAll('text.film'):选择所有text并且存在class属性为film的
			//加载的data数据包括：films和data两个值
			d3.select('#svg2 g').selectAll('text.film').data(data['films']).enter().append('text').text(function(d) {
				//获取文本 d[0]文本 d[1]上映时间
			    return d[0]; 
			}).attr('transform', function(d, i) {
				//添加变化transform，其中translate表示平移 
				//x方向平移固定值150，y方向：平移初始值40、i表示第几条
				//height2/data['films'].length)表示高度除以7部电影，即每部显影平均下来的高度
			    return 'translate(150,' + (40 + (i + 0.5) * height2 / data['films'].length) + ')';
			}).attr('fill', '#fff').attr('font-size', 12)
			.attr('text-anchor', 'end').attr('class', 'film'); //填充色、字体大小、文本右对齐、class属性


			/* 第二步 设置X轴顶部显示实体名称*/

			//获取#svg2 g中的text.title
			//data(data['data'])获取具体的数据
			d3.select('#svg2 g').selectAll('text.title').data(data['data']).enter().append('text').text(function(d) {
			    return d['name']; //表示实体的名称
			}).attr('name',function(d) {
			    return d['name']; //取个名字name属性
			}).attr('transform', function(d, i) {
				//横向平移 y轴方向均为25
			    return 'translate(' + (165 + i * (width2 - 165) / data['data'].length) + ',25)';
			}).attr('fill', '#fff').attr('font-size', 12).attr('text-anchor', 'middle')
			.attr('class', 'title').attr('fill-opacity', 0); 
			//透明度设置为0 .attr('fill-opacity', 0)


			/* 第三步 绘制方框矩阵 */

		    //矩形颜色 没有电影五种颜色
		    var colors2 = ['#4e88af', '#ca635f', '#d2907c', '#d6744d', '#ded295'];

		    for (var i = 0; i < data['data'].length; i++) {

		    	//获取第i个值
		        var tmp = data['data'][i];

		        //对每个节点#svg2 g再增加一个g，并定义一个id值
		        //选中第二个g中所有的矩形rect，再绑定数据tmp['vector']
		        //设置宽度、长度、填充色
		        d3.select('#svg2 g').append('g').attr('id', tmp['name']).attr('class', 'row')
		            .selectAll('rect').data(tmp['vector']).enter().append('rect')
		            .attr('width', Math.floor((width2 - 165) / data['data'].length))
		            .attr('height', (height2 / data['films'].length - 1))
		            .attr('fill', function(d, j) {
		                return colors2[tmp['group']];
		            }).attr('transform', function(d, j) {
		            	//矩形平移值 X轴表示实体i的宽度 Y轴表示高度
		                return 'translate(' + (165 + i * (width2 - 165) / data['data'].length) + ',' 
		                	+ (40 + j * height2 / data['films'].length) + ')';
		            }).attr('fill-opacity', function(d) {
		            	//设置透明度
		                if (d == 1) {
		                    return 1; //显示
		                }
		                else if (d == 0) {
		                    return 0; //隐藏
		                }
		            });
		    } //end for


			//打印节点
			console.log(data);
		});

		/* 第四步 选中节点显示对应的实体名称 */

	    //鼠标选中g.row则显示该名称
	    $('#svg2').on('mouseenter', 'g.row', function(event) {
	        event.preventDefault();
	        //#svg2 text.title透明度设置为0
	        $('#svg2 text.title').attr('fill-opacity',0);
	        console.log($(this).attr('id'));

	        //设置名称
	        $('#svg2 text.title[name="' + $(this).attr('id') + '"]').attr('fill-opacity', 1);
	    });

	    //鼠标移开名字
	    $('#svg2').on('mouseleave', 'g.row', function(event) {
	        event.preventDefault();
	        $('#svg2 text.title').attr('fill-opacity',0);
	    });
	    
	});
</script>
</html>
```

显示结果如下所示：
![](https://img-blog.csdnimg.cn/20190218174414548.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 总结

写到这里，作者学习张老师的视频及扩展基本讲解结束，希望这八篇文章对您有所帮助，如果有侵权立即删除。后续作者将结合相关知识，讲解如何从百科知识中抓取数据，通过命名实体识别、实体对齐、关系抽取等，构建贵州旅游知识图谱，也希望读者能与作者探讨。再次推荐大家购买张宏伦老师的网易云课程（[星球系列电影](https://study.163.com/course/courseLearn.htm?courseId=1003528010)），真心不错~

还有不足一个月作者就要博士考试了，不论结果如何，我至少奋斗过，这个寒假每日每夜都忙碌着。同时也希望未来能有更多时间学习新知识，分享新知识。未来的路谁又知道，作为一名大学老师，希望自己能始终保持初心，教好自己的每一个学生；作为一名程序员，希望自己知道学无止境，写好自己的每一篇博文；作为一名主人公，希望自己能照顾好另一伴，爱她就像爱生命。我毫无阅历，毫无准备，我一头栽进我的命里，就像跌进一个深坑，从那一秒钟起，我的心里只有一个，就是你。

昨天没有给女神准备生日礼物，有些对不起，礼物明日才到，后面要多爱她一些。

（By:Eastmount 2019-02-18 下午6点 [http://blog.csdn.net/eastmount/）](http://blog.csdn.net/eastmount/%EF%BC%89)](https://blog.csdn.net/Eastmount/article/category/7944204)](https://blog.csdn.net/Eastmount/article/category/8653277)](https://so.csdn.net/so/search/s.do?q=HTML&t=blog)](https://so.csdn.net/so/search/s.do?q=D3&t=blog)](https://so.csdn.net/so/search/s.do?q=关系图谱&t=blog)](https://so.csdn.net/so/search/s.do?q=知识图谱&t=blog)




