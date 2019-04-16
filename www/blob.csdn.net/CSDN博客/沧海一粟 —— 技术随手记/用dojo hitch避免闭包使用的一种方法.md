# 用dojo.hitch避免闭包使用的一种方法 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年01月04日 13:30:00[慢游](https://me.csdn.net/eengel)阅读数：2022








在使用Dojo的web应用中，经常会用dojo.connect绑定事件到DOM树上。



比如需要绑定一个事件到按钮"Show Text"上，当点击它时，在id为"content"的div中显示hello world.

<body>

<input type="button" id="confirm" value="Show Text"/>

<div id="content">content</div>

</body>



一种普遍的写法是：

<script type="text/javascript">

    function test(){

        var id = "content";

        dojo.connect(document.getElementById("confirm"), "onclick", function(){

            var text = "hello world";

            document.getElementById(id).innerHTML = text;

        });

    }


    test();

</script>



这儿click事件的处理形成了一个闭包。

由于多种原因，闭包在某些场合并不被推荐使用。而在很多情况下，也需要将处理的代码抽象成一个单独的函数以供重用。

函数抽象不难，麻烦的是怎样将函数用到的外部变量也能传进去。比如代码中的闭包调用了外部变量"id"，需要将其传入抽象出来的函数中。这儿dojo.hitch就可以被派上用场了：

<script type="text/javascript">

    function test(){

        var id = "content";

        dojo.connect(document.getElementById("confirm"), "onclick", dojo.hitch(this, "showContent", id));

    }


    function showContent(id){

        var text = "show something";

        document.getElementById(id).innerHTML = text;

    }


    test();

</script>



