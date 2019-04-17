# JavaScript Ajax快速学习 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年01月18日 11:58:08[boonya](https://me.csdn.net/boonya)阅读数：523








The Article is from:[https://dzone.com/refcardz/getting-started-ajax](https://dzone.com/refcardz/getting-started-ajax)




Written by  [Dave Crane](https://dzone.com/users/135039/davecrane.html) Snr Developer, Sunwheel《解锁不引人注目的JavaScript》

简介：介绍Ajax，一组用于客户端Web开发的相互关联的技术，用于创建异步Web应用程序。



建议先看英文再看翻译：翻译使用的是Google翻译。


# 第一部分

## 开始

执行Ajax的标准方法是使用XMLHttpRequest对象，友好点称之为XHR。直接使用XHR，或通过一个有用的Ajax库，如：Prototype或jQuery。我们如何“亲手”使用XHR？首先，我们需要得到一个引用：



```java
if (window.XMLHttpRequest) {

    xhr = new XMLHttpRequest();

  } else if (window.ActiveXObject) {

    xhr = new ActiveXObject("Microsoft.XMLHTTP");

  }
  //.. other codes!
}
```


开启一个URL连接：





```java
xhr.open(

    "GET",

    "my-dynamic-content.jsp?id="

    +encodeURI(myId),

    true

  );
```

指定回调函数以接收响应：





```java
xhr.onreadystatechange = function(){

    processReqChange(req);

  }
```
然后发送请求：





```java
xhr.send(null);
```

服务器可能正忙，或者网络可能很慢。我们不想坐在旁边什么都不做，我们不必直到响应到达，因为我们已经分配了回调函数。这是不耐烦的五分钟指南。对于那些想知道细节的人，我们列出了下面XHR对象的更全面的细节。



![](https://img-blog.csdn.net/20170118115505719)

# 第二部分

## 了解HTTP

为了充分利用XHR，我们建议您熟悉HTTP协议的工作原理。
使用Ajax，您比使用传统的Web应用程序开发对HTTP有更多的控制。

![](https://img-blog.csdn.net/20170118115704955)

HTTP是无状态请求 - 响应协议。


- 请求和响应都包含标题和可选的正文，它是自由文本。
- 只有POST请求包含正文。
- 请求定义动词或方法。
- Mime类型的请求和响应可以通过标题Content-type设置

## 常见HTTP动词

99％的时间，你只需要GET和POST。许多其他动词被WebDAV，Subversion over HTTP和其他利基应用程序使用，但不是所有的Web服务器都会理解它们。

![](https://img-blog.csdn.net/20170118130457283)


如果您使用日益流行的REST方法对Web服务，HTTP动词用于指示正在执行的操作的类型。REST中最常用的HTTP动词映射到CRUD（create, read, update, delete）方法：


![](https://img-blog.csdn.net/20170118130507315)

## 常见的MIME类型

为您的请求和响应设置正确的MIME类型是好的方式，它也是至关重要的，让应用程序正常行为！

![](https://img-blog.csdn.net/20170118130751847)


# 第三部分

## 使用XHR的提示
- 调用open（）时，始终将async设置为true。
同步Ajax请求阻止浏览器UI，给出我们试图摆脱的停止 - 开始行为！
- XHR对设置顺序可能很麻烦。在调用send（）之前始终设置回调处理程序
- 发送HTML形式的数据

                1.使用encodeURI（）对所有数据值进行编码
2.将请求的MIME类型设置为application / x-www- form-urlencoded

- 如果要使用responseXML属性，请将响应mime类型设置为application / xml或text / xml

## 处理响应

我们为我们的XHR对象分配了一个回调处理函数。
此函数将在响应发生时调用几次。通常，我们只想在响应完全到达时解析响应，即readyState完成。



```java
xhr.onreadystatechange=function(){

    if (xhr.readyState==4){

      if (xhr.status==200){

        parseResponse(xhr);

      }else{

        //handle the HTTP error...

      }

    };

  };
```




## XHR就绪状态（readyState）值

![](https://img-blog.csdn.net/20170118131507781)

那么，parseResponse（）方法可能是什么样子？
我们在我们发送的响应类型中有很多自由。让我们看看一些常见的。

## 处理一个HTML响应

服务器可以发送预先组装的HTML内容，我们只是拼接到网页。



```
<table class='item selected'>

<tr>

<td rowspan='3' valign='top'><div class='itemIcon'><img

src='../images/kmoon.png'></div></td>

<td class='itemTitle'>The Moon on a

Stick</td>

</tr>

<tr>

<td valign='top'>What every project

manager wants - and they want it yesterday!

<br/><br/><i>NB: Stick not included.</i></td>

</tr>

<tr>

<td><div class='price'>$365.00</div></td>

</tr>

</tr>

</table>
```


```java
function parseResponse(xhr){

  var div=document

    .getElementById("myDiv");

  div.innerHTML=xhr.responseText;

}
```




## 处理一个JSON响应

JSON是一个简单的文本标记，非常容易让JavaScript来消化！它不是那么自然地服务器端语言，但有大多数服务器的JSON库，这些天见[http://www.json.org](http://www.json.org)。大多数Ajax库现在支持JSON。




```java
{

  imgSrc: "kmoon.png",

  title: "The Moon on a Stick",

  description: "What every project manager wants 

        - and they want it yesterday!<br/><br/><i>

        NB:     Stick not included.</i>",

  price: "365.00"

}
```


```java
function parseResponse(xhr){

  var jsonObj=eval(

   "("

   +xhr.responseText

   +")"

  );

  setImgSrc(jsonObj.imgSrc);

  setTitle(jsonObj.title);

}
```




## 处理一个XML响应

XML是更自然地适合大多数服务器技术。XHR通过向我们提供responseXML属性来支持XML，但是使用DOM进行解析是很难的。




```
<item imgSrc="kmoon.png" price="365.00">

  <title>The Moon on a Stick</title>

  <description><![CDATA[What every project manager wants

         - and they want it yesterday!<br/><br/><i>NB: 

         Stick not included.</i>]]></description>

</item>
```


```java
function parseResponse(xhr){

  var xmlDoc=xhr.responseXML;

  var item=xmlDoc.getElementsByTagName('item')[0];

  var imgSrc=item.getAttribute('imgSrc');

  var title=item.getElementsByTagName('title')[0]

    .firstChild.data;

  setImgSrc(imgSrc);

  setTitle(title);

}
```

一些浏览器还支持XPath作为一种更愉快的方式来解析XML。
Sarissa和mozXPath.js都提供了跨浏览器的XPath支持。




```
<item imgSrc="kmoon.png" price="365.00">

  <title>The Moon on a Stick</title>

  <description><![CDATA[What every project manager wants 

        - and they want it yesterday!<br/><br/><i>NB: 

        Stick not included.</i>]]></description>

</item>
```


```java
function parseResponse(xhr){

  var xmlDoc=xhr.responseXML;

  var imgSrc=xmlDoc.selectSingleNode('/item/@imgSrc').value;

  var title=xmlDoc.selectSingleNode

          ('/item/title/text()').value;

  setImgSrc(imgSrc);

  setTitle(title);

}
```




## 处理一个JavaScript响应

Ajax的另一种方法是在服务器上生成脚本，并将它们发送到要评估的客户端。这里应注意在客户端上定义一个适当的高级API，生成的脚本将针对该客户端运行，否则可能导致服务器和客户端代码之间的紧密耦合。



```java
setImgSrc("kmoon.png");

setTitle("The Moon on a Stick");
```


```java
function parseResponse(xhr){

  eval(xhr.responseText);

}
```




## 处理混合响应

一些Javascript库允许在单个响应中混合Ajax的这些方言。例如，原型Ajax.Updater可以接受嵌入了<script>标签的HTML响应。将提取和评估脚本，而将其余内容嵌入到目标DOM元素中。

# 第四部分

## Ajax及其架构

Ajax是否只影响客户端？当然不是！特别是如果你的服务器响应数据而不是HTML片段，你会想在一定程度上重构。

![](https://img-blog.csdn.net/20170118132522469)




上面的dump客户端和thick客户端是极端。在其中，有一个更薄（但仍然智能）的客户端，这在很多情况下就足够了。没有单一模型适用于所有情况。试试这些经验法则：

-      要向现有应用程序添加小型Ajax功能，请遵循瘦客户端方法。胖客户端用于复杂的业务线应用程序替换。
- 您的客户端代码是可见的，并在其他人的计算机上运行。不要公开业务层的详细信息。保持粗糙。
- 某些功能必须保留在服务器上，例如数据验证。在客户端上简单，快速验证是一个额外的，而不是替换！
- 对待你的客户端代码好。使用您将在服务器上使用的模式和实践来保持代码的清洁和可维护性。
- 大多数项目都有遗留系统。如何引入Ajax，最小的中断？它会说XML还是从组件生成HTML？你可以重复使用吗？

# 第五部分

## Ajax工具包

工具包和框架将使您的生活更容易在几个方面：


- 为常见问题提供经过验证的解决方案
- 抽象出跨浏览器不兼容和烦恼
- 提供更高级的抽象，如现成的UI小部件和网络堆栈


然而，它是一个丛林在那里，与市场上许多不同类型的工具包。让我们把它们分成很多家庭。


## 客户端与服务器端

一些工具包仅是JavaScript，其他包括后端系统。
客户端工具包将提供更多的灵活性，但可能需要在服务器端进行更多的工作。


## 高级对低级

JavaScript是一种灵活的语言，一些工具包旨在以各种方式增强语言本身。其他人更关心更高级的问题，例如简化XHR，或提供插入小部件，如树，表和拖放。

## 一些热门的Ajax工具包

![](https://img-blog.csdn.net/20170118133343782)


我们没有时间告诉你如何使用所有这些工具包进行Ajax调用，但让我们选择最流行的两个：Prototype和jQuery。


## **基础：创建Ajax请求**

Prototype：



```java
new Ajax.Request(

  "my-dynamic-content.jsp",

  { method: "post",

    params: { id: myId },

    onComplete: function(response){

      parseResponse(response);

    }

  }

);
```


JQuery：





```java
$.post(

  "my-dynamic-content.jsp",

  { id: myId },

  function(xhr){

    parseResponse(xhr);

  }

);
```



-      无需创建自己的XHR对象
- 使用高级，有意义的回调，而不是onreadystatechange
- 为您不想指定自己的所有选项提供了明智的默认值

## 将HTML内容加载到DOM节点

Prototype：



```java
new Ajax.Updater(

  $("myDomNode"),

  "my-dynamic-content.jsp",

  { method: "post",

    params: { id: myId }

  }

);
```


JQuery：





```java
$("#myDomNode").load(

  "my-dynamic-content.jsp",

  { id: myId }

);
```



- 不需要提供回调函数

## 使用JSON响应

Prototype:



```java
new Ajax.Request(

  "my-dynamic-content.jsp",

  { method: "post",

    params: { id: myId },

    onComplete: function(response,json){

      alert(json.someProperty);

    }

  }

);
```


JQuery:





```java
$("#myDomNode").load(

  "my-dynamic-content.jsp",

  { id: myId }

);
```



- JSON响应返回到我们的回调已经解析

# 第六部分

## 一般JavaScript编程提示

JavaScript是一种宽松类型的脚本语言，支持面向对象和函数式编程风格。虽然它看起来像Java和C系列语言，它是完全不同的引擎盖下。这里有一些生存提示，让你通过你第一次严重遭遇这种语言：

- 对象可以在运行时使用新属性进行扩展。
将Javascript对象视为关联数组
- 函数是第一类对象，可以作为参数传递到其他函数（参见前面的众多回调函数）
- JavaScript函数支持闭包。
也就是说，定义函数时在范围内的变量仍然可以在函数内部被引用，即使稍后被调用

## Ajax用户界面




在Ajax之前，UI几乎总是作为声明性HTML提供，而文档对象模型（DOM）只能用于审核。使用Ajax特别是单页应用程序，DOM可以发挥更大的作用。

使用DOM是一个两个阶段的过程：

- 找到我们想要使用的元素
- 修改其内容或重新组织它们





## 查找DOM元素

DOM标准本身为我们提供了一些基本的工具。进取的JavaScript库开发人员已经构建在这些之上，以提供更丰富的功能集。

![](https://img-blog.csdn.net/20170118135605682)


id属性通常太具体，每个元素添加一个元素，我们可能需要定位变得乏味，并且杂乱的标记。另一方面，标签名称不够具体，不足以在许多情况下使用。最常见的解决方案是使用CSS类来定位元素。我们可以根据需要使它们具体或一般。


## 使用Prototype查找DOM元素

![](https://img-blog.csdn.net/20170118135917168)


## 使用JQuery查找DOM元素

![](https://img-blog.csdn.net/20170118135927483)


热心提示：DOM元素可以分配给多个CSS类。当使用选择器机制查找元素时，您可以使用相同的CSS类来确定页面的外观，或者可以分配单独的标记类，即在页面上没有视觉效果的CSS类。

## 修改DOM

同样，DOM标准为我们提供了一组基本的工具，浏览器供应商有效地标准化了几个。


## 使用Prototype修改DOM

Prototype喜欢使用innerHTML来修改DOM。
它使用插入命名空间，以及最近在DOM元素类本身上的插入方法增强了这一点。

![](https://img-blog.csdn.net/20170118140237341)


Prototype不支持以编程方式构建DOM元素，但Scriptaculous库会向混合中添加一个DOMBuilder对象。



## 使用JQuery修改DOM
jQuery基于选择DOM元素集，并且提供了批量处理DOM元素集的方法。（这些方法可以在一个元素的集合上使用！）这里的方法都对从选择器返回的一组DOM节点进行操作。



![](https://img-blog.csdn.net/20170118140356849)


# 第七部分

## 小部件与行为

jQuery和Prototype（及其姐妹Scriptaculous）都倾向于称为Unobtrusive Javascript的UI风格，其中页面的内容被声明为HTML，随后进行交互。选择器在这种方法中起重要作用，定位添加行为的元素。有一种替代方法来开发Ajax UI，更类似于桌面应用程序开发，其中DOM元素是由JavaScript组件以编程方式创建的，然后设计器使用布局和容器连接在一起。Qooxdoo和Ext2都是这种风格的UI开发的例子。

# 第八部分

## 交互工具

在理想的世界中，选择正确的框架会使开发变得轻而易举，但是在实践中，你需要不时地去看下发生了什么。我们建议使用以下工具来保持Ajax开发正常进行。


![](https://img-blog.csdn.net/20170118140708057)




