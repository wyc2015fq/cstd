# CSS的垂直居中和水平居中总结 - 菜鸟路上的小白 - CSDN博客





2016年11月16日 17:33:42[somnus_小凯](https://me.csdn.net/u012486840)阅读数：243








## 前言

> 

CSS居中一直是一个比较敏感的话题，为了以后开发的方便，楼主觉得确实需要总结一下了，总的来说，居中问题分为垂直居中和水平居中，实际上水平居中是很简单的，但垂直居中的方式和方法就千奇百怪了。


## 内联元素居中方案

水平居中设置：
- 
行内元素 设置 text-align:center；

- 
Flex布局 设置display:flex;justify-content:center;(灵活运用)


垂直居中设置：
- 父元素高度确定的单行文本（内联元素） 设置 height = line-height；
- 父元素高度确定的多行文本（内联元素） a:插入 table （插入方法和水平居中一样），然后设置 vertical-align:middle； b:先设置 display:table-cell 再设置 vertical-align:middle；

## 块级元素居中方案

水平居中设置：
- 
定宽块状元素 设置 左右 margin 值为 auto；

- 
不定宽块状元素 a:在元素外加入 table 标签（完整的，包括 table、tbody、tr、td），该元素写在 td 内，然后设置 margin 的值为 auto； b:给该元素设置 display:inine 方法； c:父元素设置 position:relative 和 left:50%，子元素设置 position:relative 和 left:50%；


垂直居中设置：
- 
1.使用position:absolute（fixed）,设置left、top、margin-left、margin-top的属性;

```java
.box{
position:absolute;/*或fixed*/
top:50%;
left:50%;
margin-top:-100px;
margin-left:-200px;
}
```
- 2.利用position:fixed（absolute）属性，margin:auto这个必须不要忘记了;


```java
.box{
    position: absolute;或fixed
    top:0;
    right:0;
    bottom:0;
    left:0;
    margin: auto;
}
```
- 3.利用display:table-cell属性使内容垂直居中;

```java
.box{
    display:table-cell;
    vertical-align:middle;
    text-align:center;
    width:120px;
    height:120px;
    background:purple;
}
```
- 4.使用css3的新属性transform:translate(x,y)属性;

```java
.box{
    position: absolute;
    transform: translate(50%,50%);
    -webkit-transform:translate(50%,50%);
    -moz-transform:translate(50%,50%);
    -ms-transform:translate(50%,50%);
}
```
- 5.最高大上的一种，使用:before元素;

```java
.box{
    position:fixed;
    display:block;
    background:rgba(0,0,0,.5);
}
.box:before{
    content:'';
    display:inline-block;
    vertical-align:middle;
    height:100%;
}
.box.content{
    width:60px;
    height:60px;
    line-height:60px;
    color:red;
```
- 6.Flex布局;

```java
.box{
    display: -webkit-box;
    display: -webkit-flex;
    display: -moz-box;
    display: -moz-flex;
    display: -ms-flexbox;
    display: flex;
    水平居中
    -webkit-box-align: center;
    -moz-box-align: center;
    -ms-flex-pack:center;
    -webkit-justify-content: center;
    -moz-justify-content: center;
    justify-content: center;
     垂直居中
    -webkit-box-pack: center;
    -moz-box-pack: center;
    -ms-flex-align:center;
    -webkit-align-items: center;
    -moz-align-items: center;
    align-items: center;
}
```



