# CSS display使用 - ggabcda的博客 - CSDN博客





2017年09月08日 11:06:01[ggabcda](https://me.csdn.net/ggabcda)阅读数：1049








 今天我们来分析一下display的一些用法，display样式在我们整个CSS设置中，非常重要，掌握好display，才能有效的解决CSS布局的问题，在理解display之前，我们先了解两个概念：块元素和行内元素。

**1、****基本概念 **

** 块级元素(block element) **

  其就是以块状呈现，具有块的基本特征，如高度、宽度等等，其主要有以下几个特点：
- 在新行上开始；所以它们都是独占一行度，行高以及外边距和内边距都可控制（使用float属性除外）。 
- 可以设置 width, height属性和margin,padding等属性。 
- 宽度缺省是它的容器的100%，除非设定一个宽度。 
- 它可以容纳内联元素和其他块元素 

比如我们常用的div,p, h1等，都是块级元素。

**内联元素（inline element） **

其是以长条型呈现，可以改变宽度，但是无法改变高度,其主要有以下几个特点：
- 和其他元素都在一行上；所以它们都是一个挨着一个，除非一行排不下，则会换行 
- 高，行高及外边距和内边距不可改变；只能改变宽度,Margin和padding只能改变左右，上下无效 
- 宽度就是它的文字或图片的宽度，不可改变 
- 内联元素只能容纳文本或者其他内联元素 

比如我们常用的span、a、input都是内联元素。

**2、****display样式 **

**   定义和用法 **

display 属性规定元素应该生成的框的类型。

**说明 **

这个属性用于定义建立布局时元素生成的显示框类型。对于 HTML 等文档类型，如果使用 display 不谨慎会很危险，因为可能违反 HTML 中已经定义的显示层次结构。对于 XML，由于 XML 没有内置的这种层次结构，所有 display 是绝对必要的。

**默认值**：inline

** 其他值： **

|**值**|**描述**|
|----|----|
|none|此元素不会被显示。|
|block|此元素将显示为块级元素，此元素前后会带有换行符。|
|inline|默认。此元素会被显示为内联元素，元素前后没有换行符。|
|inline-block|行内块元素。（CSS2.1 新增的值）|
|list-item|此元素会作为列表显示。|
|run-in|此元素会根据上下文作为块级元素或内联元素显示。|
|compact|CSS 中有值 compact，不过由于缺乏广泛支持，已经从 CSS2.1 中删除。|
|marker|CSS 中有值 marker，不过由于缺乏广泛支持，已经从 CSS2.1 中删除。|
|table|此元素会作为块级表格来显示（类似 <table>），表格前后带有换行符。|
|inline-table|此元素会作为内联表格来显示（类似 <table>），表格前后没有换行符。|
|table-row-group|此元素会作为一个或多个行的分组来显示（类似 <tbody>）。|
|table-header-group|此元素会作为一个或多个行的分组来显示（类似 <thead>）。|
|table-footer-group|此元素会作为一个或多个行的分组来显示（类似 <tfoot>）。|
|table-row|此元素会作为一个表格行显示（类似 <tr>）。|
|table-column-group|此元素会作为一个或多个列的分组来显示（类似 <colgroup>）。|
|table-column|此元素会作为一个单元格列显示（类似 <col>）|
|table-cell|此元素会作为一个表格单元格显示（类似 <td> 和 <th>）|
|table-caption|此元素会作为一个表格标题显示（类似 <caption>）|
|inherit|规定应该从父元素继承 display 属性的值。|




**3、分别介绍 **

**none：**

none这个值表示此元素将不被显示,设置为none的元素，即不显示在界面上，关键是设置为none，它将让出自己占有的空间，这个是和visibility的主要区别，visibility只是隐藏对象，对象在文档流中并没有被移除，display:none则元素会被从文档流中移除。 我们看看如下案例：

普通div

```
<div style="height:100px;background-color:#ffd800"></div>
<div style="height:100px;background-color:#ccd800"></div>
```

显示效果如下：

![](http://images2017.cnblogs.com/blog/24761/201709/24761-20170907165219226-635796346.png)

如设置上面div的display:none，代码如下：

```
<div style="height:100px;background-color:#ffd800;display:none"></div>
<div style="height:100px;background-color:#ccd800"></div>
```

其效果如下：

![](http://images2017.cnblogs.com/blog/24761/201709/24761-20170907165417819-1022843379.png)

发现上面的div不显示，且占有的区域让了出来。直接从文档流中剔除了，好像不存在一样。

同时，我们设置visibility看一下，代码如下：

```
<div style="height:100px;background-color:#ffd800;visibility:hidden"></div>
<div style="height:100px;background-color:#ccd800"></div>
```

其显示效果如下图所示：

![](http://images2017.cnblogs.com/blog/24761/201709/24761-20170907165814929-1607535216.png)

虽然上面的div隐藏不可见，但div所占的区域，还是被占用，并没有从文档流中剔除。

**  block：**

使用了display:block之后， 此元素将显示为块级元素，此元素前后会带有换行符。设置为block后，那么内联元素就具有了块级元素的特征，具有高度和margin-top和padding-top等信息，我们以案例说明，a标签原本为内联元素，我们进行如下的对比：

```
<div>
     <a href="#">1</a><a href="#">2</a><a href="#">3</a><a href="#">4</a><a href="#">1</a>
    </div>
    <style>
        #b a {
            display: block;
            height: 20px;
        }
    </style>
 <div id="b">
     <a href="#">1</a><a href="#">2</a><a href="#">3</a><a href="#">4</a><a href="#">1</a>
    </div>
```

将下面的div中的a标签设置为display: block;且指定了高度，其结果如下：

![](http://images2017.cnblogs.com/blog/24761/201709/24761-20170907170327757-1928805881.png)

下面div中的a元素，出现了块级元素的特征，换行，且对指定的高度有效。

**inline **

   使用了display:inline之后， 此元素将显示为内联元素,此元素会以内联元素的特征呈现，我们编写示例如下：

```
<style>
        div {
            display: inline;
        }
    </style>
    <div style="height:100px;background-color:#ffd800">111</div>
    <div style="height:100px;background-color:#ccd800">222</div>
```

本来div是块级元素，但是我们设置为内联元素以后，其显示效果如下：

![](http://images2017.cnblogs.com/blog/24761/201709/24761-20170907170701429-719328720.png)

其不仅没有换行，且设置的高度也无效。

**inline-block **

  inline-block将对象呈递为内联对象，但是对象的内容作为块对象呈递。旁边的内联对象会被呈递在同一行内。总结来说，就是可以设置高度和内联元素，内容是块级元素，但没有换行符。

  我们编写案例说明，其代码如下：

```
<style>
        div {
            width: 100%;
            height: 100%;
        }

        a {
            display: inline-block;
            height: 60px;
            width: 100px;
            color: #ffffff;
            background-color: #000000;
            border-left: 1px solid #ffffff;
        }
    </style>
    <div><a href="#">1</a><a href="#">2</a><a href="#">3</a><a href="#">4</a></div>
```

以上的代码，呈现的效果如下：

![](http://images2017.cnblogs.com/blog/24761/201709/24761-20170907171302866-643523074.png)

我们可以看到，元素呈现了块级元素的特征，除了没有换行，其最终呈现的是没有换行符的块级元素。

**list-item **

    list-item为元素内容生成一个块级元素，随后再生成一个列表型的行内盒。其效果就和ul中出现项目列表符号一样。通俗地说就是会在内容前面自动加上黑点，而display:block则不会出现黑点。

```
<style>
        a {
            display: list-item;
        }
    </style>
    <div><a href="#">1</a><a href="#">2</a><a href="#">3</a><a href="#">4</a></div>
```

这个比较简单，就是按照列表的方式显示，这个用得不是很多，可以通过UL实现，其效果如下：

![](http://images2017.cnblogs.com/blog/24761/201709/24761-20170907171754022-1803982258.png)

**run-in **

有些浏览器已经不支持了，所以这个属性现在也很少有人使用，我使用IE进行的测试。run-in也是一个有意思的行内块级元素，不过它与inline-block的表现是完全不同的。它的表现与它的后一个直接元素直接挂钩。如果它的后一个直接元素是块级元素的话，它会表现为行内元素插入到该块级元素中；如果它的后一个元素是行内元素的话，它就会表现为块级元素，坦白的讲就是跟后一个元素的表现是完全相反的,最后，它跟前一个元素的表现形式是无关的。

我们看第一种情况：它后面的元素是块级元素，代码如下：

```
<style>
        div {
            height: 100px;
            background-color: #f6cfcf;
        }
        a {
            display:run-in;
            height:100px;
        }
    </style>

        <a href="#">1</a>
        <div></div>
```

我们设置a的display为run-in,则其表现为：

![](http://images2017.cnblogs.com/blog/24761/201709/24761-20170907172759835-1654964171.png)

可以看到，其下一个元素是块状元素，则他会变成内联元素，并且插入到下一个元素中。我们可以把a改成p试试，也会出现相同的结果。

情况2：其后面元素是内联元素，代码如下：

```
<style>
        div {
            height: 100px;
            background-color: #f6cfcf;
        }
        a {
            display:run-in;
            height:100px;
        }
    </style>

        <a href="#">1</a>
        <span></span>
```

其显示效果如下：

![](http://images2017.cnblogs.com/blog/24761/201709/24761-20170907173155366-710162557.png)

a表现了块级元素的特征，所以说run-in的对象，其受下一个元素影响，下一个是块，他就是内联，下一个是内联，它表现为块。

**table **

  table元素和后面所有和table相关的元素，都是让元素按照表格方式显示，设置为table的元素，其具有<table>标签默认的特性。我们可以通过table样式，把不同的元素定义为表格，满足表格化的需求。
- table    此元素会作为块级表格来显示（类似 <table>），表格前后带有换行符。 
- table-row-group    此元素会作为一个或多个行的分组来显示（类似 <tbody>）。 
- table-header-group    此元素会作为一个或多个行的分组来显示（类似 <thead>）。 
- table-footer-group    此元素会作为一个或多个行的分组来显示（类似 <tfoot>）。 
- table-row    此元素会作为一个表格行显示（类似 <tr>）。 
- table-column-group    此元素会作为一个或多个列的分组来显示（类似 <colgroup>）。 
- table-column    此元素会作为一个单元格列显示（类似 <col>） 
- table-cell    此元素会作为一个表格单元格显示（类似 <td> 和 <th>） 
- table-caption    此元素会作为一个表格标题显示（类似 <caption>） 

下面以示例说明，以样式的方式定义了一个表格：

```
<style type="text/css">
        .table, .table * {
            margin: 0 auto;
            padding: 0;
        }

        .table {
            display: table;
            width: 80%;
            border-collapse: collapse;
        }
        .table-tr {
            display: table-row;
        }
        .table-th {
            display: table-cell;
            border: 1px solid #808080;
        }
        .table-td {
            display: table-cell;
            border:1px solid #808080;
        }
    </style>
        <div class="table">
            <div class="table-tr">
                <div class="table-th">编号</div>
                <div class="table-th">名字</div>
                <div class="table-th">10</div>
            </div>
            <div class="table-tr">
                <div class="table-td">1</div>
                <div class="table-td">张三</div>
                <div class="table-td">20</div>
            </div>
            <div class="table-tr">
                <div class="table-td">2</div>
                <div class="table-td">李四</div>
                <div class="table-td">30</div>
            </div>
            <div class="table-tr">
                <div class="table-td">3</div>
                <div class="table-td">王五</div>
                <div class="table-td">40</div>
            </div>
        </div>
```

其显示效果如下图所示：

![](http://images2017.cnblogs.com/blog/24761/201709/24761-20170907173810851-269652761.png)

我们可以通过自由的方式设置表格。



在css3.0以后，添加另外几个布局方式，由于现在浏览器兼容性的问题，暂时使用还不是很普遍，这几个会在后面的介绍。

inline-grid 

grid

inline-flex





