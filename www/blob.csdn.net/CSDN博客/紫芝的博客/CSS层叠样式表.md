# CSS层叠样式表 - 紫芝的博客 - CSDN博客





2019年03月05日 10:40:49[紫芝](https://me.csdn.net/qq_40507857)阅读数：1172








# CSS层叠样式表

## 1.使用CSS样式的方式

### 1.1 内链样式表

```
<body style="background-color: brown;color: #fff;margin:0;padding:0;">
```

### 1.2 嵌入式样式表

```
<head>
    <meta charset="UTF-8">
    <title>CSS样式表</title>
    <style type="text/css">
        body{
            background-color: brown;color: #fff;margin:0;padding:0;
        }
        p{
            background-color: pink;color: black;margin:0;padding:0;
        }
    </style>
</head>
```

### 1.3 引入式样式表

```
<link  rel="StyleSheet" type="text/css" href="../CSS/style.css">
```

## 2.定义样式表

### 2.1 HTML标记定义

p可以叫做选择器，定义哪个标记中的内容执行其中的样式。

一个选择器可以控制若干个样式属性，属性之间用";"隔开

```
<style type="text/css">
    p{
        color:red;
        font-size: 55px;
        font-face:微软雅黑;
    }
    </style>
<p>....</p>
```

### 2.2 class定义

class定义以"."开始

```
<style type="text/css">
        .p{
            color:green;
            font-size: 30px;
        }
    </style>
<h1 class="p">麦子学院</h1>
```

### 2.3 ID定义

ID定义以"#"开始

```
<style type="text/css">
        #p{
            color: coral;
            font-size: 70px;
        }
    </style>
<p id="p">麦子学院麦子学院麦子学院</p>
```

### 2.4 优先级

**ID>Class>HTML,同级选择距离元素最近的样式**

### 2.5 组合选择器

**同时控制多个元素，用","隔开**

```
h1,h2,h3,h4{
    color: yellow;
}
<h1>麦子学</h1>
<h2>麦子学</h2>
<h3>麦子学</h3>
<h4>麦子学</h4>
```

### 2.6 伪元素选择器
- a:link 正常链接的样式
- a:hover 鼠标放上去的样式
- a:active 点击链接时的样式
- a:visited 已经访问过的链接的样式

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>定义CSS样式（CSS选择器）</title>
    <style type="text/css">
        /*HTML选择器*/
        p{
            color: red;
            font-size: 15px;
        }
        /*class选择器*/
        .p{
            color:green;
            font-size: 30px;
        }
        /*ID选择器*/
        #p{
            color: coral;
            font-size: 70px;
        }
        /*组合选择器*/
        h1,h2,h3,h4{
            color: yellow;
        }
        /*伪元素选择器*/
        a:link{
            color: red;/*正常显示*/
            font-size: 30px;
        }
        a:hover{
            color: yellow;/*鼠标放上的样式*/
            font-size: 40px;
        }
        a:active{
            color: coral;/*点击时的样式*/
            font-size: 50px;
        }
        a:visited{
            color: blueviolet;/*访问过的样式*/
            font-size: 60px;
        }
    </style>
</head>
<body>
<p>麦子学院</p>
<p class="p">麦子学院 麦子学院</p>
<p id="p">麦子学院 麦子学院 麦子学院</p>
<h1>麦子学</h1>
<h2>麦子学</h2>
<h3>麦子学</h3>
<h4>麦子学</h4>
<a href="https://www.baidu.com"><font size="6">百度</font> </a>
</body>
</html>
```

## 3.常见属性

### 3.1 颜色属性

**color属性定义文本的颜色**
- color:green
- color:#ff6600
- color:#f60(简写)
- color:rgb(红色0255,绿色0255,蓝色0~255)
- color:rgba(红色0255,绿色0255,蓝色0255,透明度01)

### 3.2 字体属性

#### 3.2.1 font-size字体大小
- px：设置固定值
- %：父元素百分比
- smaller：比父元素小
- larger：比父元素大
- inherit：继承父元素

#### 3.2.2 font-family定义字体

**font-family: 微软雅黑,宋体,黑体;**

> 
显示字体优先级：微软雅黑>宋体>黑体;


#### 3.2.3 font-weight字体加粗
- normal：默认
- bold：粗
- bolder：更粗
- lighter：细
- 100，200，300，400…800，900九个等级

#### 3.2.4 font-style字体样式
- normal:正常
- italic:斜体
- oblique:倾斜
- inherit:继承

#### 3.2.5 font-variant小型大写字母显示文本
- nomal 标准
- small-caps 小型大写字母显示
- inherit 继承

### 3.3 背景属性

#### 3.3.1 background-color 背景颜色

#### 3.3.2 background-image 背景图片

**background-image:url(图片路径);**

#### 3.3.3 background-repeat 背景重复
- background-repeat:repeat;/*重复铺满*/
- background-repeat:repeat-x;/*向x轴重复*/
- background-repeat:repeat-y;/*向y轴重复*/
- background-repeat:no-repeat;/*不重复*/

#### 3.3.4 background-position 背景位置
- background-position:(x,y);/*左上角的坐标*/

#### 3.3.5 background 简写方式
- background:背景颜色 url(图像) 重复 位置

### 3.4 文本属性

#### 3.4.1 text-align 横向排列
- left
- center
- right

#### 3.4.2 line-height 文本行高

#### 3.4.3 text-indent 首行缩进

#### 3.4.4 letter-spacing 字符间距

#### 3.4.5 word-spacing 单词间距

#### 3.4.6 direction 文本方向
- rtl 从左到右
- ltr 从右到左

#### 3.4.7 text-transform 文本大小写
- capitalize 单词第一个字母大写，其余小写
- uppercase 大写
- lowercase 小写

### 3.5 边框属性

#### 3.5.1 border-style 边框风格

> 
border-style 统一风格

- solid 直线
- dashed 虚线
- dotted 点状
- double 双线

> 
单独定义某一方向风格

- border-top-style
- border-bottom-style
- border-left-style
- border-right-style

#### 3.5.2 border-width 边框宽度

> 
border-width 统一风格

- thin 细边框
- medium 中等
- thick 粗
- px 固定值

> 
单独定义某一方向风格

- border-top-width
- border-bottom-width
- border-left-width
- border-right-width

#### 3.5.3 border-color 边框宽度

#### 3.5.4 border 简写方式

### 3.6 列表属性

#### 3.6.1 list-style-type 标记的类型

#### 3.6.2 list-style-position 标记的位置

#### 3.6.3 list-style-image 设置图像列表标记

#### 3.6.4 list-style 简写方式

## 4.DIV+CSS布局

### 4.1 DIV+SPAN

> 
div是块级元素，span是内联元素，目的是应用CSS样式


### 4.2 盒模型
- margin 盒子外边距
- padding 盒子内边距
- border 盒子边框宽度
- width 盒子长度
- height 盒子高度

### 4.3 布局的相关属性

#### 4.3.1 position定位方式
- relative 正常定位
- absolute 根据父元素进行定位
- fixed 根据浏览器窗口进行定位
- static 没有定位

#### 4.3.2 定位
- top
- bottom
- left
- right

#### 4.3.3 z-index层覆盖优先级

#### 4.3.4 display显示属性
- display:none 层不显示
- display:block 块状显示，在元素后面换行，显示下一个块元素
- display:inline

#### 4.3.5 float属性
- left 左浮动
- right 右浮动

#### 4.3.6 clear清除浮动
- clear:both;

#### 4.3.7 overflow溢出处理
- hidden 隐藏超出层大小的内容
- scroll 无论内容是否超出层大小，都添加滚动条
- auto 内容超出层大小时，添加滚动条

### 4.4 兼容问题

#### 4.4.1 测试工具
- IE Tester
- Multibrower

#### 4.4.2 判断 IE  浏览器的方法



