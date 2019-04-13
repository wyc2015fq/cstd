
# CSS的div和span的区别 - 李昆鹏的博客 - CSDN博客


2018年05月11日 11:23:36[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：216


----------------------------------------CSS的div和span的区别-----------------------------------------
**div和span**
**div****和****span****是****html****中块级元素，是层叠样式表中的定位技术。**
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<|style|type|=|"text/css"|>
|div|{
|font-family|:|宋体|;
|font-size|:|20px|;
|font-weight|:|bold|;
|background-color|:|\#FFFF00|;
|width|:|200px|;
|height|:|200px|;
|text-align|:|center|;
|}
|</|style|>
|</|head|>
|<|body|>
|<|div|>|www.rlteach.com|</|div|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/20180511112227130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
--------------------------------------------------Div和span的区别--------------------------------
**div****自动的换行，span****不换行**
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<|style|type|=|"text/css"|>
|div|{
|font-family|:|宋体|;
|font-size|:|20px|;
|font-weight|:|bold|;
|background-color|:|\#FFFF00|;
|width|:|200px|;
|height|:|200px|;
|text-align|:|center|;
|}
|</|style|>
|</|head|>
|<|body|>
|<|h1|>|div|自动换行|</|h1|>
|<|div|><|img|src|=|"fj.jpg"|></|div|>
|<|div|><|img|src|=|"fj.jpg"|></|div|>
|<|div|><|img|src|=|"fj.jpg"|></|div|>
|<|h1|>|span|不自动换行|</|h1|>
|<|span|><|img|src|=|"fj.jpg"|></|span|>
|<|span|><|img|src|=|"fj.jpg"|></|span|>
|<|span|><|img|src|=|"fj.jpg"|></|span|>
|</|body|>
|</|html|>
|
--------------------------------------------------
![](https://img-blog.csdn.net/2018051111230389?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

