# 零基础HTML学习历程 （一）  初识HTML - HJ - CSDN博客
2016年10月27日 08:59:42[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：2236
首先我是拿着这本书开始我的HTML入门之旅的（如下图）
![](https://img-blog.csdn.net/20161027085008636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对这本书的评价在网上都有，我就不多说了哈。
先给HTML下个含义：HTML是超文本标记语言（HyperText Markup Language）的缩写，用来建立网页的结构。与它形影不离的一个好兄弟是css（这个同时在另外的文章里面专门会讲），他们两个好兄弟一起工作，是我么用来创建网页的语言。
初次见面，不好意思，还需要介绍一下什么Web（这里只做简单的了解，详细的会另外有专门讲到）。不用怕，你可以直接理解为浏览器：像搜狗、IE、360、火狐之类的。现在正要学的HTML就是我们俗称的Web静态页面技术。也就是说，你学会了HTML语言，在记事本里面敲好代码，然后记事本格式txt改成html，就像如下，你就可以打开它，在浏览器上面看到你预计的内容了。
![](https://img-blog.csdn.net/20161027084630760?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)把后面的后缀改动一下![](https://img-blog.csdn.net/20161027084654428?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，然后双击打开，如下图
等等，如果你的记事本是这个样子![](https://img-blog.csdn.net/20161027084724416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)改不了后缀名怎么办？其实这是电脑隐藏了文件的后缀名。操作如下：
a、打开控制面板
![](https://img-blog.csdn.net/20161027084737869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
b、选中外观和个性化，打开文件夹选项
![](https://img-blog.csdn.net/20161027084754057?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
c、点击查看，找到“隐藏已知文件类型的扩展名”
![](https://img-blog.csdn.net/20161027084746916?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
d、点击取消选项，然后点击“应用”就好了
![](https://img-blog.csdn.net/20161027084742397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
以上就是对HTML的简单介绍，下面开始正式学习。
首先，学习HTML，要了解它实际上是一种框架，就像建房子要用的钢筋和房梁。只不过跟建房子有一些不同的是，建房子是立体框架，而HTML是[数据结构](http://lib.csdn.net/base/datastructure)里面的树形框架，用父节点和孩子节点的关系来联通维持。
而这一次我们就首先来了解了解这个框架的规则。
（1）通过HTML，我们利用标记来表示内容提供结构。我们把匹配标记以及它们包围的内容称为元素。元素又由三部分组成：
        a、一个开始标记；
        b、内容；
        c、一个结束标记；
      （注意：有些元素有所例外，比如<img>）
（2）所有的页面都要有一个<html>元素，其中要有<head>元素和<body>元素，在<body>元素里的内容，就是你将在浏览器里看到的东西；而在<head>元素里，你需要放网页的信息。
（3）类似[Java](http://lib.csdn.net/base/javaee)等语言的程序代码，大多数空白符（制表符、回车、空格）都会被浏览器忽略，不过可以利用空白符让你的HTML（对你）更有可读性。
接着我们来了解常见的HTML元素。
（1）<title>元素
        <title>元素放在HTML的首部里，顾名思义，就是题目的意思。也就是说，在<title>元素中，你写什么内容，网页打开那上面的标题就是什么。如下图。假如你的部分代码是这样![](https://img-blog.csdn.net/20161027085035089?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，那么打开网页，你得到的标题就是这样![](https://img-blog.csdn.net/20161027085047137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。这就是<title>元素的作用。
（2）<h1>、<h2>、<h3>、<h4>、<h5>、<h6>
      你可以叫他们为分类标题元素，总共是1到6六个层次。请注意，需要放在<body>元素中，每个标题元素代表的字体大小不一样，效果大家可以去试试，这里就不在赘述。
（3）<p>
      分段元素，需要放在<body>元素中，主要的目的是一段结束，另起一段。
（4）<style>元素
      定义页面样式元素，需要放在<head>里。<style>标记还有一个（可选的）属性，名为type，它能告诉浏览器你在使用什么类型的样式。由于要使用css，所以可以指定“text/css”类型。
关于属性：通过属性，可以提供一个元素的附加信息。比如说，如果有一个<style>元素，属性允许你准确地指定是什么类型的样式。以后还会看到不同元素的更多属性：你只要记住一点，属性能提供元素的一些额外的信息。
关于结束标记：大部分元素都是由开始标记、内容和结束标记三部分组成。结束标记在左尖括号后面、标记名前面有一个“/”，以明确这是一个结束标记。
好了，本节结束之前，附上hello world简单代码。
![](https://img-blog.csdn.net/20161027085109027?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
希望大家学习愉快，每天进步一点点。
            
