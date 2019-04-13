
# CSS中div的浮动float - 李昆鹏的博客 - CSDN博客


2018年05月11日 11:33:44[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：146


------------------------------CSS中div的浮动float-----------------------------------
**HTML****页面的标准文档流(****默认布局)是：从上到下，从左到右，遇块(块级元素)换行。浮动层：给元素的float属性赋值后，就是脱离文档流，进行左右浮动，紧贴着父元素(默认为body文本区域)的左右边框。而此浮动元素在文档流空出的位置，由后续的(非浮动)元素填充上去：块级元素直接填充上去，若跟浮动元素的范围发生重叠，浮动元素覆盖块级元素。**
**Float****有4****个值：**
**left****：元素向左浮动。**
**right****：元素向右浮动。**
**none****：默认值。**
**inherit****：从父元素继承float属性。**
**浮动后的div****宽度会变成0，但是其内边框可能撑起它的宽和高。**
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<|style|type|=|"text/css"|>
|.outside|{
|font-size|:|12px|;
|font-family|:|Arail|;
|padding|:|10px|;
|background-color|:|\#fffcd3|;
|border|:|1px solid black|;
|width|:|50%|;
|}
|.inside1|{
|padding|:|10px|;
|border|:|1px dashed black|;
|background|:|\#a9d6ff|;
|margin|:|5px|;
|float|:|left|;
|/* 1.div1|向左漂浮之后，宽度为|0
|2.div1|移到父元素的左边
|3.div1|的原始位置被空出来，由后面的|html|元素来补上。
|*/
|}
|.inside2|{
|padding|:|10px|;
|border|:|1px dashed black|;
|background|:|\#ffacd3|;
|}
|</|style|>
|</|head|>
|<|body|>
|<|div|class|=|"outside"|>
|<|div|class|=|"inside1"|>|内部|div|</|div|>
|<|div|class|=|"inside2"|>|内部|div|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
-----------------------------------------------
![](https://img-blog.csdn.net/20180511113250595?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-------------------------------------------------
**div浮动对html元素的影响**
**如果浮动的div****前面有同级别html元素，该浮动的div会排在html元素后面浮动，不会覆盖html元素**
**总结：div****的浮动对前面的html元素没影响，对后面的html元素有影响。**
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|div|浮动|</|title|>
|<|style|type|=|"text/css"|>
|.outside|{
|font-size|:|12px|;
|font-family|:|Arail|;
|padding|:|10px|;
|background-color|:|\#fffcd3|;
|border|:|1px solid black|;
|width|:|50%|;
|}
|.inside1|{
|padding|:|10px|;
|border|:|1px dashed black|;
|background|:|\#a9d6ff|;
|margin-bottom|:|5px|;
|}
|.inside2|{
|padding|:|10px|;
|border|:|1px dashed black|;
|background|:|\#ffacd3|;
|margin|:|5px|;
|float|:|left|;
|/*
|如果浮动的|div|在前面有相同的|div|，直接排在同级的|div|的下面浮动，
|不会超越上面同级的|div|。
|总结：
|div|的浮动对前面的|html|元素每影响，对后面的|html|元素有影响。
|*/
|}
|</|style|>
|</|head|>
|<|body|>
|<|h1|>|div|浮动|</|h1|>
|<|div|class|=|"outside"|>
|<|div|class|=|"inside1"|>|内部|div|</|div|>
|<|div|class|=|"inside2"|>|内部|div|</|div|>
|<|div|class|=|"inside1"|>|内部|div|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
-------------------------------------------------
![](https://img-blog.csdn.net/20180511113312359?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-----------------------------------------------------------------------------------
**CSS中多个div浮动**
多个同级块元素同时在一个方向浮动，则从该方向上水平依次排列
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|div|浮动|</|title|>
|<|style|type|=|"text/css"|>
|.outside|{
|font-size|:|12px|;
|font-family|:|Arail|;
|padding|:|10px|;
|background-color|:|\#fffcd3|;
|border|:|1px solid black|;
|width|:|50%|;
|}
|.inside1|{
|padding|:|10px|;
|border|:|1px dashed black|;
|background|:|\#a9d6ff|;
|margin-bottom|:|5px|;
|}
|.inside2|{
|padding|:|10px|;
|border|:|1px dashed black|;
|background|:|\#ffacd3|;
|margin|:|5px|;
|float|:|left|;
|}
|</|style|>
|</|head|>
|<|body|>
|<|h1|>|div|浮动|</|h1|>
|<|div|class|=|"outside"|>
|<|div|class|=|"inside1"|>|内部|div1|</|div|>
|<!--
|如果多个同级|html|元素同时浮动，这几个|html|元素就会从左到右浮动排列
|脱离了|div|的自动换行
|-->
|<|div|class|=|"inside2"|>|内部|div2|</|div|>
|<|div|class|=|"inside2"|>|内部|div3|</|div|>
|<|div|class|=|"inside2"|>|内部|div4|</|div|>
|<|div|class|=|"inside2"|>|内部|div5|</|div|>
|<|div|class|=|"inside1"|>|内部|div6|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
-------------------------------------------------------------
![](https://img-blog.csdn.net/20180511135803637?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-------------------------------------------------------------
**CSS消除div漂浮的影响**
前面div的浮动会影响后面的div的布局，如果想消除该影响可以使用clear:left  |   right  |  both
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<|style|type|=|"text/css"|>
|.outside|{
|font-size|:|12px|;
|font-family|:|Arail|;
|padding|:|10px|;
|background-color|:|\#fffcd3|;
|border|:|1px solid black|;
|width|:|50%|;
|}
|.inside1|{
|padding|:|10px|;
|border|:|1px dashed black|;
|background|:|\#a9d6ff|;
|margin|:|5px|;
|float|:|left|;
|/* 1.div1|向左漂浮之后，宽度为|0
|2.div1|移到父元素的左边
|3.div1|的原始位置被空出来，由后面的|html|元素来补上。
|*/
|}
|.inside2|{
|padding|:|10px|;
|border|:|1px dashed black|;
|background|:|\#a9d6ff|;
|margin|:|5px|;
|float|:|right|;
|}
|.inside3|{
|padding|:|10px|;
|border|:|1px dashed black|;
|background|:|\#ffacd3|;
|clear|:|both|;
|/* left|消除左边漂浮的影响
|right|消除右边漂浮的影响
|both|消除两边漂浮的影响
|*/
|}
|</|style|>
|</|head|>
|<|body|>
|<|div|class|=|"outside"|>
|<|div|class|=|"inside1"|>|内部|div1|</|div|>
|<|div|class|=|"inside2"|>|内部|div2|</|div|>
|<|div|class|=|"inside3"|>|内部|div3|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/2018051113581935?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

