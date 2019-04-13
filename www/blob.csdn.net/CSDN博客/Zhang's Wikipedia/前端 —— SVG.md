
# 前端 —— SVG - Zhang's Wikipedia - CSDN博客


2018年04月03日 22:14:56[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：684



## 0. 简介
SVG：可缩放矢量图形；
SVG 是代码，通过浏览器的解析而渲染成一种图形；
可缩放矢量图形是基于可扩展标记语言（XML），以描述二维矢量图形的一种图形格式，由万维网联盟（
World Wide Web Consortium，W3C）指定，是一个开放标准；
矢量图与像素图的不同就在于，矢量图是由数学表达式定义的，矢量图不适合表现色彩丰富的照片，更加适合展示简单的几何图案；
1. 简单绘制
svg 标记：
<svgxmlns="http://www.w3.org/2000/svg"width="800"height="600"version="1.1"></svg>
简单几何图案：
<rect width="200"height="100"x="100"y="20"style="fill:red;stroke:blue;stroke-width:4"/>\#宽两百，高100，x="100"表示横向偏移，y="20"，则为纵向偏移；

