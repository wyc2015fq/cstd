# JavaScript中的DOM和BOM - Big Smile - CSDN博客
2016年06月03日 16:42:55[王啸tr1912](https://me.csdn.net/tr1912)阅读数：757
        学习JavaScript，感觉里面的东西很多，很杂，尤其是DOM和BOM一直有在提，但是不知道是什么东西，买了一本书，是《JavaScript DOM编程艺术》，里面讲的DOM让我顿时明白了很多，借此和大家分享一下。
# 一、DOM(文档对象模型)分解
![](https://img-blog.csdn.net/20160603162106090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## （1）D：文档（document）
             文档的定义其实很是抽象的，因为，这个文档的生成是在网页的加载完成之后才会生成的。当一个网页被浏览器加载了之后，它在浏览器中就是以文档的形式而存在的，而这个DOM也就悄然而生了，就是它把你编写的网页转换为文档对象的。按照我的理解，这个文档描述了网页中的所有的对象，包括节点，定义的元素等等，这些文档用自己的理解就是以各种代码中的对象而存在的，所以包括了html文件中的一部分东西，还有css中的自建对象以及JS文件中自定义的对象。
## （2）O：对象（Object）
             从上面可知当文档被加载了之后就会生成文档的对象，依附于浏览器而生成的，而对象不仅仅只是此，还包括了浏览器提供的他自己的对象，包括了我们调用的弹窗等，还有浏览器中的计时器等。其实对象分为三类：
宿主对象、内建对象、用户自定义对象。其中宿主对象就是由浏览器提供的对象，而内建对象就是用户在JS中是使用的数组，函数，日期等，用户自定义对象是用户创建的对象，应该就是我们自建的匿名函数等对象。
## （3）M：模型（Model）
           模型，有着整个框架的意味，就好比是一张地图。浏览器提供的就是这张地图，JS的代码被浏览器解析的时候就是对应着这张地图来进行查找JS代码中写的有关于浏览器的对象等命令，以此来操作整个网页中的对象。那么网页中的对象具体指什么呢，其实就是节点，节点有分支，更像是一颗树一般。例如以下文档：
```
<html>
    <head>
        <meta charset="utf-8"/>
        <title>List</title>
    </head>
    <body>
        <h1>what to buy<h1>
        <p title=a gentle reminder">不要忘了购买这些</p>
        <ul>
            <li>一罐豆子</li>
            <li class="sale">奶酪</li>
            <li class="sale important">牛奶</li>
        </ul>
    </body>
</html>
```
节点（树形）图：
![](https://img-blog.csdn.net/20160603161023093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 二、BOM(浏览器对象模型)
        其实说到BOM，就是浏览器提供的对象的模型，也就是说他是和浏览器相关的，我们做JS处理的浏览器兼容的问题也大多来源于此。BOM 主要处理浏览器窗口和框架，不过通常浏览器特定的 JavaScript 扩展都被看做 BOM 的一部分。这些扩展包括：
     弹出新的浏览器窗口
     移动、关闭浏览器窗口以及调整窗口大小
     提供 Web 浏览器详细信息的定位对象
     提供用户屏幕分辨率详细信息的屏幕对象
     对 cookie 的支持
     IE 扩展了 BOM，加入了 ActiveXObject 类，可以通过 JavaScript 实例化 ActiveX 对象
      javacsript是通过访问BOM（Browser Object Model）对象来访问、控制、修改客户端(浏览器)，由于BOM的window包含了document，window对象的属性和方法是直接可以使用而且被感知的，因此可以直接使用window对象的document属性，通过document属性就可以访问、检索、修改XHTML文档内容与结构。因为document对象又是DOM（Document Object Model）模型的根节点。可以说，BOM包含了DOM(对象)，浏览器提供出来给予访问的是BOM对象，从BOM对象再访问到DOM对象，从而js可以操作浏览器以及浏览器读取到的文档。其中：
    DOM包含：window：
       Window对象包含属性：document、location、navigator、screen、history、frames
       Document根节点包含子节点：forms、location、anchors、images、links
从window.document已然可以看出，DOM的最根本的对象是BOM的window对象的子对象。
以上对比来自：[http://www.jb51.net/article/55851.htm](http://www.jb51.net/article/55851.htm)
区别：DOM描述了处理网页内容的方法和接口，BOM描述了与浏览器进行交互的方法和接口。
# 三、对象的获取和修改属性
        其实对象的获取也无非就是获得这个对象的ID，或者是class的名称，或者节点的名称TagName，对于他们的操作命令分别是：getElementById，getElementsByClassName，getElementsByTagName 这三种命令，都是W3C提供的，适用于绝大多数浏览器。
        关于使用属性方面，十分的简单，只需要用getAttribute和setAttribute两个命令即可，如下例子：
获得属性：
```java
var paras=document.getElementsByTagName("p");          //获得所有的p节点
for(var i=0;i<paras.length;i++){
    alert(paras[i].getAtrribute("title"));            //循环弹出p节点的Title属性
}
```
修改属性：
```java
<span style="font-size:14px;">var shopping=document.getElenmentById("purchases");    //获得id为</span><span style="font-family: Arial, Helvetica, sans-serif;"><span style="font-size:12px;">purchases的节点</span></span><span style="font-size:14px;">
shopping.setAtrribute("title","a list of goods");       //修改节点的title属性为 a list of goods</span>
```
由此可以看出document的主导地位，来获得浏览器中的节点。
# 四、总结
        在JS中的面向对象的思想和我们在编写一般C/S程序的时候有些不一样，因为它引入的是模型的对象，但是经过我们的分析和理解，他和一般的对象也是没有什么两样，只不过是嵌套在了文档中了，需要通过浏览器的中介和解释才能进行访问和修改，所以对于面向对象这里的理解以及文档的理解还是很重要的。
