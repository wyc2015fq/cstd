# jQuery 中 remove() 与 detach() 的区别 - weixin_33985507的博客 - CSDN博客
2018年03月30日 01:17:29[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
jQuery中有两种方法功能类似：删除dom元素（剪切下来）。这两种方法分别是remove()和detach()。
我们来写一个小demo用一下这两种方法：
```java
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Document</title>
    <style>
        .wrapper {
            width: 100px;
            height: 200px;
            border: 1px solid #ccc;
        }        
        
        div {
            width: 100px;
            height: 100px;
        }
        .item1 {
            background: blueviolet
        }
        .item2 {
            background: cadetblue;
        }
    </style>
</head>
<body>
    <div class="wrapper">
        <div class="item1"></div>
        <div class="item2"></div>
    </div>
    <script src="https://code.jquery.com/jquery-3.3.1.js"></script>
    <script>
        $('.item1').click(function () {
            $(this).remove();
        })
        $('.item2').click(function () {
            $(this).detach();
        })
    </script>
</body>
</html>
复制代码
```
实现的效果是：点击哪个方块，哪个方块删除。只不过点击item1使用remove()方法删除元素，点击item2使用detach()方法删除元素。
初始状态如下：
先点击item1，也就是紫色方块，发现紫色方块消失了。
再点击item2，也就是绿色方块，发现绿色方块也消失了。
到目前为止，这两种方法没什么区别。
我们继续，增加一个按钮，作用是点击之后，重新插入被删除的这两个元素。
```java
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Document</title>
    <style>
        .wrapper {
            width: 100px;
            height: 200px;
            border: 1px solid #ccc;
        }        
        
        div {
            width: 100px;
            height: 100px;
        }
        .item1 {
            background: blueviolet
        }
        .item2 {
            background: cadetblue;
        }
    </style>
</head>
<body>
    <div class="wrapper">
        <div class="item1"></div>
        <div class="item2"></div>
    </div>
    <button>插入div</button>
    <script src="https://code.jquery.com/jquery-3.3.1.js"></script>
    <script>
        $('.item1').click(function () {
            item1 = $(this).remove();
        })
        $('.item2').click(function () {
            item2 = $(this).detach();
        })
        $('button').click(function () {
            $('.wrapper').append(item1).append(item2);
        })
    </script>
</body>
</html>
复制代码
```
首先依次点击item1和item2这两个方块，删除这两个元素
点击按钮，发现被删除的两个元素重新被插入进来
再次点击两个item，发现item2能被删除，而item1删除不掉了。
也就是说，用detach()方法删除的元素，插入回来之后，可以再次通过点击事件进行删除；而用remove()删除的元素，插入回来之后，点击事件失效了。
这就是这两种方法的区别：
- remove()方法不仅可以删除dom元素，连同该元素绑定的事件也一并删除。
- detach()方法删除dom元素，不会影响该元素绑定的事件。
谢谢浏览，期待你的反馈！
