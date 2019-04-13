
# CSS的绝对定位absolute - 李昆鹏的博客 - CSDN博客


2018年05月11日 15:29:11[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：60标签：[CSS																](https://so.csdn.net/so/search/s.do?q=CSS&t=blog)个人分类：[CSS																](https://blog.csdn.net/weixin_41547486/article/category/7656511)


-----------------------------CSS的绝对定位absolute---------------------------
**绝对定位**
**设置绝对定位的元素从文档流中完全删除，以浏览器的左上角为相对位置，设置水平位置和垂直的位置。**
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<|style|type|=|"text/css"|>
|body|{
|margin|:|10px|;
|font-size|:|12px|;
|font-family|:|Arial|;
|}
|.outside|{
|width|:|1000px|;
|height|:|600px|;
|background-color|:|\#a9d6ff|;
|border|:|1qx dashed black|;
|}
|.inside|{
|padding|:|10px|;
|border|:|1px dashed black|;
|background-color|:|\#fffcd3|;
|width|:|100px|;
|/*|设置绝对定位，当前|div|脱离文档流|*/
|position|:|absolute|;
|/*|距离浏览器的左边框|*/
|left|:|20px|;
|/*|距离浏览器的上边框|*/
|top|:|30px|;
|/*
|right|距离浏览器的右边框
|bottom|距离浏览器的下边框
|*/
|}
|</|style|>
|</|head|>
|<|body|>
|<|div|class|=|"outside"|>
|<|div|class|=|"inside"|>|绝对定位|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/20180511152642382?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-----------------------------------------------------------
**CSS绝对定位对宽度的影响**
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<|style|type|=|"text/css"|>
|body|{
|margin|:|10px|;
|font-size|:|12px|;
|font-family|:|Arial|;
|}
|.outside|{
|width|:|1000px|;
|height|:|600px|;
|background-color|:|\#a9d6ff|;
|border|:|1qx dashed black|;
|}
|.inside|{
|padding|:|10px|;
|border|:|1px dashed black|;
|background-color|:|\#fffcd3|;
|/*|设置绝对定位，当前|div|脱离文档流
|如果不设置宽度，则宽段变为|0
|*/
|position|:|absolute|;
|/*|距离浏览器的左边框|*/
|left|:|10%|;
|/*|距离浏览器的上边框|*/
|top|:|20%|;
|/*
|right|距离浏览器的右边框
|bottom|距离浏览器的下边框
|*/
|right|:|10%|;
|bottom|:|20%|;
|/*|如果四个边距都设置，就会改变|div|的宽度和高度|*/
|}
|</|style|>
|</|head|>
|<|body|>
|<|div|class|=|"outside"|>
|<|div|class|=|"inside"|>|绝对定位|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/20180511152709486?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-------------------------------------------------------------
**CSS的absolute定位脱离文档流**
代码示例1：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<|style|type|=|"text/css"|>
|body|{
|margin|:|10px|;
|font-size|:|12px|;
|font-family|:|Arial|;
|}
|.outside|{
|width|:|1000px|;
|height|:|600px|;
|background-color|:|\#a9d6ff|;
|border|:|1px dashed black|;
|}
|.inside|{
|padding|:|10px|;
|background-color|:|\#fffcd3|;
|border|:|1px dashed black|;
|/*|设置绝对定位，当前|div|脱离文档流
|如果不设置宽度，则宽段变为|0
|*/
|position|:|absolute|;
|/*|距离浏览器的左边框|*/
|left|:|20px|;
|/*|距离浏览器的上边框|*/
|top|:|30px|;
|/*
|right|距离浏览器的右边框
|bottom|距离浏览器的下边框
|*/
|}
|.inside1|{
|margin|:|10px|;
|padding|:|10px|;
|background-color|:|\#fffcd3|;
|border|:|1px dashed black|;
|}
|</|style|>
|</|head|>
|<|body|>
|<|div|class|=|"outside"|>
|<|div|class|=|"inside"|>|绝对定位|</|div|>
|<|div|class|=|"inside1"|>|文档流中|div|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/20180511152739311?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例2：
|<|html|>
|<|head|>
|<|title|>|position|属性|</|title|>
|<|style|type|=|"text/css"|>
|<!--
|body|{
|margin|:|10px|;
|font-family|:|Arial|;
|font-size|:|13px|;
|}
|\#father|{
|background-color|:|\#a0c8ff|;
|border|:|1px dashed \#000000|;
|width|:|100%|;
|height|:|100%|;
|padding|:|5px|;
|}
|\#block1|{
|background-color|:|\#fff0ac|;
|border|:|1px dashed \#000000|;
|padding|:|10px|;
|position|:|absolute|;|/* absolute|绝对定位|*/
|left|:|30px|;
|top|:|35px|;
|}
|\#block2|{
|background-color|:|\#ffbd76|;
|border|:|1px dashed \#000000|;
|padding|:|10px|;
|position|:|absolute|;|/* absolute|绝对定位|*/
|left|:|50px|;
|top|:|60px|;
|}
|-->
|</|style|>
|</|head|>
|<|body|>
|<|div|id|=|"father"|>
|<|div|id|=|"block1"|>|block1|</|div|>
|<|div|id|=|"block2"|>|block2|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/20180511152839386?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


