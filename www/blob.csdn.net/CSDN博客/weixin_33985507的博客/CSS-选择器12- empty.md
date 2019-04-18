# CSS-选择器12-:empty - weixin_33985507的博客 - CSDN博客
2017年06月11日 17:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
[CSS选择器-系列文章](https://www.jianshu.com/nb/13119195)
### 1、:empty选择器
:empty 选择器匹配没有子元素（包括文本节点）的每个元素。
### 2、效果演示
源代码
```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>css-选择器-empty</title>
    <style type="text/css">
        p:empty{
            width: 100px;
            height: 20px;
            background: red;
        }
    </style>
</head>
<body>
    <p>p1</p>
    <p></p>
    <p>p2</p>
    <p>p3</p>
    <p>p4</p>
</body>
</html>
```
运行效果：
![6260202-e879bf5ae431d57e.png](https://upload-images.jianshu.io/upload_images/6260202-e879bf5ae431d57e.png)
image.png
[CSS选择器-系列文章](https://www.jianshu.com/nb/13119195)
[下一节 CSS-选择器13-:not](https://www.jianshu.com/p/17c0f33beff4)
