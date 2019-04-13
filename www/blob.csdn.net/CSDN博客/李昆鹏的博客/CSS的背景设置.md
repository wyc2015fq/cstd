
# CSS的背景设置 - 李昆鹏的博客 - CSDN博客


2018年05月18日 15:40:09[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：47标签：[CSS																](https://so.csdn.net/so/search/s.do?q=CSS&t=blog)个人分类：[CSS																](https://blog.csdn.net/weixin_41547486/article/category/7656511)


-----------------------------------------CSS的背景设置-----------------------------------------
**背景图片设置**
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|图片设置|</|title|>
|<|style|type|=|"text/css"|>
|body|{
|/*|给|html|元素设置背景图片，一定要使用|url() */
|background-image|:|url(bg3.jpg)|;
|/*
|background-repeat:repeat-x;|在横向重复
|background-repeat:repeat-y;|纵向重复
|background-repeat:no-repeat;|不重复
|*/
|/*|默认是重复的横向和纵向|*/
|background-repeat|:|repeat|;
|}
|</|style|>
|</|head|>
|<|body|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/20180518153923419?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-------------------------------------------------------------------------------------------
**背景颜色设置**
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|字体设置|</|title|>
|<|style|type|=|"text/css"|>
|body|{
|margin|:|5px|;
|background-color|:|\#5B8A00|;
|font-size|:|15px|;
|}
|img|{
|width|:|200px|;
|margin|:|5px|;
|float|:|right|;
|}
|span|{
|font-family|:|黑体|;
|font-size|:|40px|;
|float|:|left|;
|margin|:|3px|;
|}
|</|style|>
|</|head|>
|<|body|>
|<|img|src|=|"mainroad.jpg"|/>
|<|span|>|春|</|span|>
|<|p|>|季，地球的北半球开始倾向太阳，受到越来越多的太阳光直射，因而气温开始升高。随着冰雪消融，河流水位上涨。春季植物开始发芽生长，许多鲜花开放。冬眠的动物苏醒，许多以卵过冬的动物孵化，鸟类开始迁徙，离开越冬地向繁殖地进发。许多动物在这段时间里发情，因此中国也将春季称为|“|万物复苏|”|的季节。春季气温和生物界的变化对人的心理和生理也有影响。|</|p|>
|<|p|>|对农民来说，春季是播种许多农作物的季节。在春季，地球的北半球开始倾向太阳，受到越来越多的太阳光直射，因而气温开始升高。随着冰雪消融，河流水位上涨。春季植物开始发芽生长，许多鲜花开放。冬眠的动物苏醒，许多以卵过冬的动物孵化，鸟类开始迁徙，离开越冬地向繁殖地进发。许多动物在这段时间里发情，因此中国也将春季称为|“|万物复苏|”|的季节。|</|p|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/20180518153938739?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


