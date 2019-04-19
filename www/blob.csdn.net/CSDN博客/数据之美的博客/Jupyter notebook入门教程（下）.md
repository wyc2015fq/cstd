# Jupyter notebook入门教程（下） - 数据之美的博客 - CSDN博客
2017年12月19日 09:03:23[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：385
Jupyter notebook的入门教程第二部分的英文原文出处： 
[Getting started with the Jupyter notebook (part 2)](https://www.packtpub.com/books/content/getting-started-jupyter-notebook-part-2)
从之前介绍的教程第一部分中，我们发现使用Jupyter notebook的基本功能就可以做很多事情了，但是它还有更多的功能和选择，允许用户创造更漂亮的交互式文档。
#### 单元操作
当你在编辑notebook时，你希望使用更多高级的cell操作。幸运的是，notebook允许你使用非常丰富的cell操作。你可以删除一个cell，先选中cell，点击Edit->Delete cell。你也可以移动一个cell的位置，点击Edit-> 
Move cell [up | down]。你也可以剪切、粘贴cell，点击Edit->Cut Cell then Edit -> Paste Cell …，选择需要的粘贴形式。如果有许多cells，而你只想执行一次，或者你想一次性执行大量代码，你就可以合并多个cells，点击Edit->Merge cell [above|below]。编辑notebook时，记住这些操作，会节约你很多时间。
#### Markdown单元高级用法
让我们再深入地探讨下markdown单元类型，即便它的类型是markdown，它同时也支持HTML代码。你可以在你的cell中创建更高级的样式，比如添加图片等等。举个例子来说，如果你想在notebook中添加Jupyter的图标，尺寸为100x100，并且放置在cell左侧，可以这样编写：
```xml
<img src="http://blog.jupyter.org/content/images/2015/02/jupyter-sq-text.png"
style="width:100px;height:100px;float:left">
```
- 1
- 2
运行该单元，效果如下：
![这里写图片描述](https://img-blog.csdn.net/20170605082511967?)
除此之外，markdown还支持LaTex语法。你可以在markdown cell中按照LaTex语法规则写下方程式，然后直接运行，就可以看到结果。例如运行下面方程式：
`$$\int_0^{+\infty} x^2 dx$$`- 1
运行后就得到了LaTex方程式：
![这里写图片描述](https://img-blog.csdn.net/20170605083006316?)
#### 导出功能
notebook另一个强大的功能就是导出功能。你可以把你的notebook（例如是个图解代码课程）导出为如下多种形式：
- 
HTML
- 
Markdown
- 
ReST
- 
PDF(Through LaTex)
- 
Raw Python
如果导出成PDF格式，你甚至可以不使用LaTex就创建了一个漂亮的文档。或者，你可以将你的notebook保存为HTML格式，发布到个人网站上。你还可以导出成ReST格式，作为软件库的文档。
#### Matplotlib集成
如果你用Python画过图，应该知道matplotlib。Matplotlib是用来画图的Python库。与Jupyter notebook结合使用时，效果更好。下面，让我们来看看如何在Jupyter notebook中使用matplotlib。
为了在Jupyter notebook中使用matplotlib，你需要告诉Jupyter获取所有matplotlib生成的图形，并把它们全部嵌入到notebook中。为此，只需输入以下命令：
`%matplotlib inline`- 1
这条语句执行可能耗费几秒钟，但是只需要你打开notebook时执行一次就好。让我们作个图，看看是怎么集成的：
```
import matplotlib.pyplot as plt
import numpy as np
x = np.arange(20)
y = x**2
plt.plot(x, y)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
这段简单代码将绘出对应的二次曲线。运行这个cell，结果如下所示：
![这里写图片描述](https://img-blog.csdn.net/20170605085947890?)
我们可以看到，图直接嵌入到notebook中，就在代码下面。修改代码，重新运行，图形将自动同步更新。对于每个想要把代码和图形放在同一个文件中的数据科学家来说，这是一个很好的特性，这样可以清楚知道每段代码究竟干了什么。同时，在文档中添加一些文字型描述也有很大的作用。
#### 非本地内核
Jupyter notebook非常容易从本地电脑上启动，也允许多个人通过网络连接到同一个Jupyter实例。你是否注意到，在上一部分的教程中，启动Jupyter时出现了下面这条语句：
IPython Notebook运行在：[http://localhost:8888/](http://localhost:8888/)
这条语句表示你的notebook是本地运行，可以在浏览器中输入地址[http://localhost:8888/](http://localhost:8888/)，打开你的notebook。通过修改配置，可以让notebook面向公开访问。这样，任何人如果知道这个notebook地址，就通过浏览器可以远程访问并修改notebook。
#### 结语
通过这两个部分的入门教程，我们知道了Jupyter notebook是一个非常强大的工具，允许使用者为数据分析、教育、文件等任何你可以想到的内容创建漂亮的交互式文档。不要犹豫，探索它更多的功能和可能性。如果你有任何问题或者想感谢开发者们，请给他们反馈。
[](http://blog.csdn.net/red_stone1/article/details/72863749#)[](http://blog.csdn.net/red_stone1/article/details/72863749#)[](http://blog.csdn.net/red_stone1/article/details/72863749#)[](http://blog.csdn.net/red_stone1/article/details/72863749#)[](http://blog.csdn.net/red_stone1/article/details/72863749#)[](http://blog.csdn.net/red_stone1/article/details/72863749#)
