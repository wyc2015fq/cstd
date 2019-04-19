# web前端——div和span标签 - xqhrs232的专栏 - CSDN博客
2018年10月30日 14:01:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：59
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/xishaoguo/article/details/79479608](https://blog.csdn.net/xishaoguo/article/details/79479608)
相关文章
1、前端id属性和span标签----[https://blog.csdn.net/sdaujsj1/article/details/79173719?utm_source=blogxgwz0](https://blog.csdn.net/sdaujsj1/article/details/79173719?utm_source=blogxgwz0)
2、web前端——table标签----[https://blog.csdn.net/xishaoguo/article/details/79378729?utm_source=blogxgwz6](https://blog.csdn.net/xishaoguo/article/details/79378729?utm_source=blogxgwz6)
3、web前端——html其它常用标签----[https://blog.csdn.net/xishaoguo/article/details/79373445?utm_source=blogxgwz7](https://blog.csdn.net/xishaoguo/article/details/79373445?utm_source=blogxgwz7)
div和span标签都是用来帮助页面进行排版的，它们两个没有自己的语意。
div：块级元素，所谓的块级元素，就是可以通过css设置宽高等一些大小的样式，这样的元素标签称为块级元素。块级元素的特点是自己占一整行。如果再写其它标签，那么其它标签会显示在它的下面。
div的宽度默认是整个浏览器的宽度，这个宽度是从body标签继承过来的。
span：行内元素，不能通过css设置具体的宽高等样式的标签元素称为行内元素。行内元素的特点是标签内的内容是多大它就是多大，而且不会占一整行。
例子：
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>标签</title>
    <style>
        * {
            margin: 0;
            padding: 0;
        }
    </style>
</head>
    <body>
        <div>卡特琳娜</div>
        <div>盖伦</div>
        <span>暴走萝莉</span>
        <span>齐天大圣</span>
    </body>
</html>
