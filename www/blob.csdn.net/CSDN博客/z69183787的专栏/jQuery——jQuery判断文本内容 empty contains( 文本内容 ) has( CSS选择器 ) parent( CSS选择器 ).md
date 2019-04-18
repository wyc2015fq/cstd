# jQuery——jQuery判断文本内容:empty/contains("文本内容")/has("CSS选择器")/parent("CSS选择器") - z69183787的专栏 - CSDN博客
2014年01月23日 10:47:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8851
      注：从这篇文章开始，渐渐意识到毕竟这些知识只是**选择器**的一些常用方法，与jQuery框架提供的方法毕竟还是有不少区别的。而且jQuery框架没有为此处理到一一对应，因此在使用时，还是需要分清并且分开这两种应用。
        希望每位看到这些文章的哥们姐们都能够理解，欢迎拍砖，但不是喷泡沫。
<body>
    <!--
** jQuery——jQuery判断文本内容:empty/contains("文本内容")/has("CSS选择器")/parent("CSS选择器")**
        理解：jQuery判断文本内容函数主要是判断所获取的jQuery对象中进行条件的获取
        1、:empty——表示获取的jQuery对象中文本内容为空的所有的匹配元素集合
        (注：这里的是伪类选择器，不是empty()方法；两个意义完全不一样)
        2、:contains("文本内容")——表示获取的jQuery对象中包含着文本内容的所有匹配元素的集合
        (注：这里也是一个在伪类中使用的，并不是从jQuery对象中进行再次判断，jQuery框架没有提供contains()方法)
        3、has("CSS选择器")——表示获取的jQuery对象中包含有参数“CSS选择器”的所有匹配的元素集合
        (注：这里has()方法中包含的CSS选择器只限于标签选择器，而不包含ID选择器和Class选择器)
        4、parent("CSS选择器")——表示获取的jQuery对象汇总含有参数“CSS选择器”的所有匹配的父元素集合
    -->
    <table>
        <tr><td>afs</td><td>adf</td></tr>
        <tr><td>Value 2</td><td></td></tr>
    </table>
    <div id="" class="">
        hello
    </div>
    <div id="ada" class="ada"><p><span id="" class="" >和咯哦</span></p>
        kobicc!
    </div>
</body>
<script language="javascript">
<!--
    alert($("td:contains('2')").html());//显示获取到包含文本内容为2的第一个匹配元素的文本内容
    //alert($("td").contains('2').html())  //ERROR这里没有这个方法，jQuery框架没有提供对应的同名方法
    alert($("div").has('p').html());//这里has()方法的参数只限于标签选择器，不包括ID选择器和Class选择器
    alert($("td:empty").length);//这里使用td:empty表示匹配td选择器中文本内容为空的所有元素
    alert($("td").empty().length);//这里使用$().empty()则是表示清空了td选择器中的所有的子节点
    alert($("div:parent").html()); //这里显示出来的是获取到第一个DIV后显示出来的innerHTML;而不是包含元素自身的outerHTML
    alert($("div").parent().html());// 这个的parent()方法则获取的又是另一个意思了。
//-->
</script>
