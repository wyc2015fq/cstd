
# CSS中div的外边距margin - 李昆鹏的博客 - CSDN博客


2018年05月11日 11:30:29[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：210


---------------------------------------------CSS中div的外边距margin-----------------------------------------
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<|style|type|=|"text/css"|>
|span|{
|font-size|:|12px|;
|font-family|:|Arail|;
|padding|:|10px|;
|background-color|:|\#fffcd3|;
|border|:|1px solid black|;
|}
|.left|{
|background-color|:|\#a9d6ff|;
|margin-right|:|30px|;
|}
|.right|{
|margin-left|:|30px|;
|}
|</|style|>
|</|head|>
|<|body|>
|<|span|class|=|"left"|>|元素|1|</|span|>
|<|span|class|=|"right"|>|元素|2|</|span|>
|</|body|>
|</|html|>
|
---------------------------------------------------
![](https://img-blog.csdn.net/20180511112958469?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-----------------------------------------------------
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<|style|type|=|"text/css"|>
|div|{
|font-size|:|12px|;
|font-family|:|Arail|;
|padding|:|10px|;
|background-color|:|\#fffcd3|;
|border|:|1px solid black|;
|width|:|50%|;
|}
|.inside|{
|background-color|:|\#a9d6ff|;
|margin|:|10px 30px 50px 100px|;|/*|设置|div|的外边距，顺时针由上开始赋值|*/
|}
|</|style|>
|</|head|>
|<|body|>
|<|div|class|=|"outside"|>
|<|div|class|=|"inside"|>|内部|div|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
----------------------------------------------------
![](https://img-blog.csdn.net/20180511113013143?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


