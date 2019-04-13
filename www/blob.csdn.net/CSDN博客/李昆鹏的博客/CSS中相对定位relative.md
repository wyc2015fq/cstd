
# CSS中相对定位relative - 李昆鹏的博客 - CSDN博客


2018年05月17日 12:01:38[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：349


-------------------------------------------CSS中相对定位relative-----------------------------------
**相对定位**
相对定位是一个非常容易理解的概念，如果对一个元素先对定位，可以设置其水平位置和垂直位置，这个元素相对于元素的起点开始移动。
注意：在使用相对定位的时候无论是否进行移动，元素仍然会占据原有的空间，因此移动元素可能覆盖其他元素。
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
|border|:|1px dashed black|;
|}
|.inside|{
|padding|:|10px|;
|background-color|:|\#fffcd3|;
|border|:|1px dashed black|;
|margin|:|10px|;
|}
|.inside1|{
|margin|:|10px|;
|padding|:|10px|;
|background-color|:|\#fffcd3|;
|border|:|1px dashed black|;
|/*|设置相对定位|,|相对点是当前|div|的原始位置的左上角|*/
|position|:|relative|;
|/*|距离|div|的原始位置的左边框|*/
|left|:|20px|;
|/*|距离|div|的原始位置的上边框|*/
|top|:|30px|;
|/*
|right|距离|div|的原始位置的右边框
|bottom|距离|div|的原始位置的下边框
|*/
|}
|</|style|>
|</|head|>
|<|body|>
|<|div|class|=|"outside"|>
|<|div|class|=|"inside"|>|div1|</|div|>
|<|div|class|=|"inside1"|>|div2|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/2018051712003640?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-----------------------------------------------------------------
**相对定位对文档流的影响**
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
|border|:|1px dashed black|;
|}
|.inside|{
|padding|:|10px|;
|background-color|:|\#fffcd3|;
|border|:|1px dashed black|;
|margin|:|10px|;
|position|:|relative|;
|left|:|30px|;
|top|:|30px|;
|/* div1|相对定位脱离了文档流，
|但是后续的|div|还会认为|div1|是在没有相对定位之前的状态
|所有后续的|div|不会填补|div1|的空缺位置，而是继续按照文档流来排序
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
|<|div|class|=|"inside"|>|div1|</|div|>
|<|div|class|=|"inside1"|>|div2|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/2018051712005170?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

