
# CSS中div的内边框padding - 李昆鹏的博客 - CSDN博客


2018年05月11日 11:28:06[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：937


----------------------------------CSS中div的内边框padding------------------------------------
**所有html****在布局上都遵守盒子模型**
![](https://img-blog.csdn.net/20180511112730634?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<|style|type|=|"text/css"|>
|.outside|{
|padding|:|10px 30px 50px 100px|;|/*|从上边距顺时针赋值宽度|*/
|/*padding:10px 10px 10px 10px;|从上边距顺时针赋值宽度|*/
|border|:|1px solid black|;|/*|给边框设置宽度，样式，颜色|*/
|width|:|800px|;
|background-color|:|\#fffcd3|;
|}
|.inside|{
|width|:|100%|;
|height|:|100%|;
|background-color|:|\#66b2ff|;
|border|:|1px solid black|;
|}
|</|style|>
|</|head|>
|<|body|>
|<|div|class|=|"outside"|>
|<|div|class|=|"inside"|>|内容|</|div|>
|</|div|>
|</|body|>
|</|html|>
|
--------------------------------------------
**内边框的宽高百分比大小由外边框的padding所设置的。**
![](https://img-blog.csdn.net/20180511112749779?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

