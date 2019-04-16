# 用锚点进行HTML页面内容的跳转 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年12月04日 15:30:33[慢游](https://me.csdn.net/eengel)阅读数：18513








# 介绍

我们知道，JavaScript可以实现HTML页面上非常丰富的事件和动作，比如说点击工具栏某个按钮时，显示相应特定的内容并隐藏其他。但在有些Web应用部署的实际情况中，会禁用除了HTML之外的其他代码，比如JavaScript和CSS。那如何实现类似的效果呢？

经过尝试，本人发现用锚点也可以实现页面内容的跳转。具体来说，就是当点击某个按钮时，跳转到特定的内容处。当工具栏有若干按钮时，每个按钮都实现类似功能，看起来就和“点击工具栏某个按钮时，显示相应特定的内容并隐藏其他”效果类似。

下面来看看具体是如何实现的。



# 例子

这个例子中的网页显示了每个星期一、二、三分别包含哪些日期。比如点击星期一时，就会跳转并显示相应的日期。

![](https://img-blog.csdn.net/20131204152614531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






# 代码

test.html

```
<div style="width: 100%; height: 50px; font-family: arial; font-size: 36px; margin-bottom: 10px; background-color: rgb(160, 176, 192);">
	Day Checker
</div>
<a name="day_1"></a> 
<div style="margin-bottom: -5px;">	
	<a href="#day_1"
		style="background-color: #BeBeBe;color: #000000;display: inline-block;height: 30px;text-align: center;text-decoration: none;width: 100px;font-weight: bold;">Monday</a>
	<a href="#day_2"
		style="background-color: #BeBeBe;color: #000000;display: inline-block;height: 30px;text-align: center;text-decoration: none;width: 100px;">Tuesday</a>
	<a href="#day_3"
		style="background-color: #BeBeBe;color: #000000;display: inline-block;height: 30px;text-align: center;text-decoration: none;width: 100px;">Wednesday</a>
</div>
<div>
	<p>2012-9-2</p>
	<p>2012-9-9</p>
	<p>2012-9-16</p>
	<p>2012-9-23</p>
	<p>2012-9-30</p>
	<p>2012-10-7</p>
	<p>2012-10-14</p>
	<p>2012-10-21</p>
	<p>2012-10-28</p>
	<p>2012-11-4</p>
	<p>2012-11-11</p>
	<p>2012-11-18</p>
	<p>2012-11-25</p>
	<p>2012-12-2</p>
	<p>2012-12-9</p>
	<p>2012-12-16</p>
	<p>2012-12-23</p>
	<p>2012-12-30</p>
	<p style="margin-bottom:100px;">2014...</p>
</div>
<a name="day_2"></a> 
<div style="margin-bottom: -5px;">	
	<a href="#day_1"
		style="background-color: #BeBeBe;color: #000000;display: inline-block;height: 30px;text-align: center;text-decoration: none;width: 100px;">Monday</a>
	<a href="#day_2"
		style="background-color: #BeBeBe;color: #000000;display: inline-block;height: 30px;text-align: center;text-decoration: none;width: 100px;font-weight: bold;">Tuesday</a>
	<a href="#day_3"
		style="background-color: #BeBeBe;color: #000000;display: inline-block;height: 30px;text-align: center;text-decoration: none;width: 100px;">Wednesday</a>
</div>
<div>
	<p>2012-9-3</p>
	<p>2012-9-10</p>
	<p>2012-9-17</p>
	<p>2012-9-24</p>
	<p>2012-10-8</p>
	<p>2012-10-15</p>
	<p>2012-10-22</p>
	<p>2012-10-29</p>
	<p>2012-11-5</p>
	<p>2012-11-12</p>
	<p>2012-11-19</p>
	<p>2012-11-26</p>
	<p>2012-12-3</p>
	<p>2012-12-10</p>
	<p>2012-12-17</p>
	<p>2012-12-24</p>
	<p>2012-12-31</p>
	<p style="margin-bottom:100px;">2014...</p>
</div>
<a name="day_3"></a> 
<div style="margin-bottom: -5px;">	
	<a href="#day_1"
		style="background-color: #BeBeBe;color: #000000;display: inline-block;height: 30px;text-align: center;text-decoration: none;width: 100px;">Monday</a>
	<a href="#day_2"
		style="background-color: #BeBeBe;color: #000000;display: inline-block;height: 30px;text-align: center;text-decoration: none;width: 100px;">Tuesday</a>
	<a href="#day_3"
		style="background-color: #BeBeBe;color: #000000;display: inline-block;height: 30px;text-align: center;text-decoration: none;width: 100px;font-weight: bold;">Wednesday</a>
</div>
<div>
	<p>2012-9-4</p>
	<p>2012-9-11</p>
	<p>2012-9-18</p>
	<p>2012-9-25</p>
	<p>2012-10-9</p>
	<p>2012-10-16</p>
	<p>2012-10-23</p>
	<p>2012-10-30</p>
	<p>2012-11-6</p>
	<p>2012-11-13</p>
	<p>2012-11-20</p>
	<p>2012-11-27</p>
	<p>2012-12-4</p>
	<p>2012-12-11</p>
	<p>2012-12-18</p>
	<p>2012-12-25</p>
	<p style="margin-bottom:100px;">2014...</p>
</div>
```


# 技术点

1.      在每块要显示的内容开头，加入标签<a>，并设定它的 “name”属性。比如“<aname="day_1"></a>”。这个标签只是为了跳转控制，因此不包含内容。这样在页面中就看不到。

2.      按钮也用标签<a>做。设定其“href”属性为对应要显示内容的“name”属性值，并用“#”约束。“#”表示在本页内寻找“name”为相应值的标签。

3.      为了让标签<a>长的像按钮，可以设定其“style”属性。比如“<ahref="#day_1"            style="background-color:#BeBeBe;color: #000000;display: inline-block;height: 30px;text-align:center;text-decoration: none;width: 100px;">Monday</a>”。

4.      另外，如果要表示当前所在的是哪个按钮对应的内容，可以加入特殊风格。比如在上面的例子中，当前按钮都会加粗：“font-weight: bold;”。




