
# 强悍的CSS工具组合：Blueprint, Sass, Compass - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月09日 20:25:30[Jlins](https://me.csdn.net/dyllove98)阅读数：1052


掌握CSS是每个Web开发者的基本要求，虽然CSS本身并不复杂，但怎样写出支持所有主流浏览器（特别是IE）的CSS，以及在大型网站中如何有序地组织好CSS结构却是一个相当棘手的问题。我更多的是一个开发者，而不是一个设计师，却不幸花了大量时间处理CSS这些棘手问题，如果能有一些工具能帮我处理这些问题，那将很有意义。经过一段时间的搜索和研究，还真发现了几个很棒的工具，感觉真是相见恨晚，与大家分享之。
Blueprint CSS Framework
刚才说过了，处理浏览噐的不一致性是很困难的事情。每开始一个新项目，我们都需要做一些重复的事情，比如需要将一些元素的padding和margin重置为0，设置某些元素的默认样式，定义一些通用的样式等，这些由于浏览器之间的不一致而变得复杂。有了[blueprint](http://blueprintcss.org/)，你就不用再担心这些啦，blueprint已经非常完美的为你做好这些事情了。这还只是blueprint做的一小部分事情，它还定义了一些form的样式，另外它带一些插件。blueprint还是一个基于网格(grid)布局的框架。在我看到blueprint之前还不知道网格布局已经这么流行了。网格布局就是把浏览器内容区域划分一些固定的大小的网格，网页中元素的定位都向网格靠齐。blueprint默认设置网页的宽度为950像素，并它分成24个网格，每个网格宽度为30像素，每个网格之间的距离为10像素。
![](http://marlonyao.iteye.com/upload/picture/pic/60427/ec89a394-67e5-3b35-89b4-dc40311197ae.jpg)
在blueprint下布局非常容易，我们来看如何使用blueprint来做非常常见的三列布局：
<html>
<head>
<linkrel="stylesheet"href="css/blueprint/screen.css"type="text/css"media="screen, projection">
<linkrel="stylesheet"href="css/blueprint/print.css"type="text/css"media="print">
<!--[if lt IE 8]><link rel="stylesheet" href="css/blueprint/ie.css" type="text/css" media="screen, projection"><![endif]-->
</head>
<body>
<divclass="container">
<divclass="span-24 last">
Header
</div>
<divclass="span-4">
Left sidebar
</div>
<divclass="span-16">
Main content
</div>
<divclass="span-4 last">
Right sidebar
</div>
<divclass="span-24 last">
Footer
</div>
</div>
</body>
</html>

span-x表示内容跨x个网格，同时右边留有10像素的margin，例如span-4的宽度为4*40-10=150像素。注意当内容跨了第24个风格，需要定义last样式，last样式的作用就是设置right-margin为0。这样上面的例子就很清楚了，header和footer跨所有24个网格，左右侧栏各跨4个网格，中间内容跨16个网格。[这里](http://nettuts.s3.amazonaws.com/158_blueprint/src/index.html)是个更酷的例子，它完全使用blueprint内置样式。
blueprint使得布局变得很容易，但是它也有问题，那就是它大量使用基于表现的class名称（如span-x)，这完全违背cass名称应该基于语义的规则。我们暂且搁置这个问题，来看另外一个非常非常强大的工具。

Sass
[Sass](http://sass-lang.com/)是一个输出CSS的编程语言，没错，CSS也有编程语言。Sass基于Ruby语言，并且是Haml的一部分，所以要安装Sass就得安装Haml，当然还得安装Ruby。我不喜欢Haml，却非常喜欢Sass。通过下面的命令来安装Haml(同时也安装了Sass)，基于你使用的操作系统，你可能需要在命令加上sudo:
gem install haml

Sass是基于缩进的语言，来看一个例子：
table.hl
margin: 2em0
td.ln
text-align: right
li
font:
family: serif
weight: bold
size:1.2em

假设上面的内容保存在style.sass文件，运行命令：
sass style.sass style.css

会输出style.css文件，它的内容为：
table.hl {
margin: 2em0;
}
table.hl td.ln {
text-align: right;
}
li {
font-family: serif;
font-weight: bold;
font-size:1.2em;
}

可以看出Sass语法的优势，由于基于缩进，共同的前缀只需要写一次，而在CSS文件则需要重复多次，这对维护是个问题。当然，Sass可以做得更多，它还支持变量和Mixin。Mixin是可重用的样式，它甚至还可以带参数。Mixin的定义以”=“开头，变量定义以"!"开头。
=table-scaffolding
th
text-align: center
font-weight: bold
td, th
padding: 2px
=left(!dist)
float: left
margin-left = !dist
\#data
+left(10px)
+table-scaffolding

上面的代码定义了两个Mixin，分别是table-scaffoding和left，其中left还接收一个dist参数。\#data样式则包含了left Mixin和table-scaffolding Mixin定义的样式。它会输出如下CSS：
\#data {
float: left;
margin-left: 10px;
}
\#data th {
text-align: center;
font-weight: bold;
}
\#data td, \#data th {
padding: 2px;
}

Mixin是Sass非常强大的一个特性，想想看你是不是在项目中看到很多重复的CSS定义，你不得不来回的拷贝复制。通过Sass你可以将这些公用的Sass定义成一个Mixin，然后在其它实际定义样式的地方包含它。你甚至还可以定义项目之间都通用的Mixin，把它们包含在一个单独的文件里，就像类库一样，需要时就引用。我们可以为Blueprint的样式转变成Sass的格式，并把它们做成Mixin，然后再定义基于语义的样式，它们包含这些Mixin，这样我们就可以在页面中使用语义样式名称了，而不需要使用Blueprint带的基于表现的样式。幸运的是，我们并不需要自己做这些事情，已经有另外的框架帮你做好了，那就我接下来要谈的Compass。Sass支持变量的运算，这有时非常方便，我们可以定义一些元素的宽度，定义为变量，其它元素的宽度由它们计算出来，当改变布局时，只需要修改几个变量的值就可以了。作为一门编程语言，Sass还支持控制条件和循环语句，虽然这在一般情况下很少用到。

Compass
Blueprint提供了一个非常健壮的CSS框架，但是却大量使用基于表现的class名称；Sass语言提供将基于表现的class名称转化成基于语义的class名称的基础设施，但本身不带任何样式；[Compass](http://wiki.github.com/chriseppstein/compass/getting-started)的作用就是将两者集成到一块，结合两者的优点。Compass还支持其它CSS框架的集成，这里就不说了。Compass也是基于Ruby语言的，使用下列命令来安装：
gem install compass

要开始一个新的Compass项目，使用：
compass -f blueprint project-name

选项-f表示和blueprint框架集成。进入到项目目录下，让我们基于Compass来实现一个三列布局。先写HTML文件：
<html>
<head>
<!-- Framework CSS -->
<linkhref="stylesheets/screen.css"media="screen, projection"rel="stylesheet"type="text/css">
<linkhref="stylesheets/print.css"media="print"rel="stylesheet"type="text/css">
<!--[if lt IE 8]>
<linkhref="stylesheets/ie.css"media="screen, projection"rel="stylesheet"type="text/css">
<![endif]-->
</head>
<body>
<divid="container">
<divid="header">
Header
</div>
<divid="left-sidebar">
Left sidebar
</div>
<divid="main-content">
Main content
</div>
<divid="right-sidebar">
Right sidebar
</div>
<divid="footer">
Footer
</div>
</div>
</body>
</html>

注意上面没有任何blueprint相关的样式名，我们为每个部分定义了一个基于语义的id，接下来就是它们定义样式，打开项目目录下src/screen.sass文件，将它的内容改成如下：
// This import applies a global reset to any page that imports this stylesheet.
@import blueprint/reset.sass
// To configure blueprint, edit the partials/base.sass file.
@import partials/base.sass
@import blueprint
@import blueprint/modules/scaffolding.sass
+blueprint-typography("body")
+blueprint-scaffolding("body")
+blueprint-utilities
+blueprint-debug
+blueprint-interaction
\#container
+container
\#header, \#footer
// true表示是最后一列
+column(!blueprint_grid_columns, true)
// sidebar占整个页面的1/4，也就是跨6列
!sidebar_columns = floor(!blueprint_grid_columns /4)
\#left-sidebar
+column(!sidebar_columns)
\#main-content
+column(!blueprint_grid_columns -2 * !sidebar_columns)
\#right-sidebar
+column(!sidebar_columns, true)

前几行导入Compass提供和Blue相关的Sass样式，里面包含了很多Mixin，可以拿来用。\#header, \#footer的样式直接包含了column Mixin，第一个参数是compass定义的bluepring_grid_columns变量，默认为24，第二个参数为true，表示元素会跨最后一列。left-sidebar和right-sidebar占据整个页面宽度的1/4，变量sidebar_columns表示sidebar占据的列宽，它根据bluepring_grid_columns计算得到。同样main-content的宽度也是计算得到的。它们都直接包含了column
 Mixin。将它转化成css，在工程目录下直接使用命令：
compass

就可将src目录下的sass文件转化成相应的css。现在打开刚才创建的HTML文件，应该可以看到正常布局的页面了。
在项目开发中，如果每次都需要手工调用compass命令去将sass文件转换成css文件，那就太麻烦了，compass提供命令
compass -w

它会自动监视src目录下sass文件的改动并自动将它转换成css文件。
欢迎大家访问我的个人小站[萌萌的it人](http://www.itmmd.com)www.itmmd.com

