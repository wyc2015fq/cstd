
# CSS的文字处理 - 李昆鹏的博客 - CSDN博客


2018年05月17日 23:42:10[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：120


--------------------------------------CSS的文字处理--------------------------------------
**字体**
**CSS****字体属性定义文本的字体系列、大小、加粗、风格（如斜体）和变形（如小型大写字母）。**
**CSS****字体属性**
|属性
|描述
|
|font
|简写属性。作用是把所有针对字体的属性设置在一个声明中。
|
|font-family
|设置字体系列。
|
|font-size
|设置字体的尺寸。
|
|font-size-adjust
|当首选字体不可用时，对替换字体进行智能缩放。（|CSS2.1|已删除该属性。）
|
|font-stretch
|对字体进行水平拉伸。（|CSS2.1|已删除该属性。）
|
|font-style
|设置字体风格。
|
|font-variant
|以小型大写字体或者正常字体显示文本。
|
|font-weight
|设置字体的粗细。
|
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|字体设置|</|title|>
|<|style|type|=|"text/css"|>
|h1|{
|font-family|:|黑体|;
|}
|.content|{
|font-family|:|仿宋|;
|}
|.author|{
|font-family|:|方正兰亭超细黑简体|;
|}
|</|style|>
|</|head|>
|<|body|>
|<|h1|>|立春|</|h1|>
|<|p|class|=|"content"|>|自秦代以来，我国就一直以立春作为春季的开始。立春是从天文上来划分的，
|而在自然界、在人们的心目中，春是温暖，鸟语花香；春是生长，耕耘播种。
|在气候学中，春季是指候（|5|天为一候）平均气温|10|℃|至|22|℃|的时段。
|</|p|>
|<|p|class|=|"author"|>|作者：李昆鹏|</|p|>
|</|body|>
|</|html|>
|
-----------------------------------------------------------------------------------------------------------------------------
**字体的大小**
**大小单位**
**px**
相对长度单位。像素（Pixel）。
像素是相对于显示器屏幕分辨率而言的。譬如，WONDOWS的用户所使用的分辨率一般是96像素/英寸。而MAC的用户所使用的分辨率一般是72像素/英寸。
**em**
相对长度单位。相对于当前对象内文本的字体尺寸。
如当前行内文本的字体尺寸未被人为设置，则相对于浏览器的默认字体尺寸。
**ex**
相对长度单位。相对于字符“x”的高度。此高度通常为字体尺寸的一半。
如当前对行内文本的字体尺寸未被人为设置，则相对于浏览器的默认字体尺寸。
**pt**
绝对长度单位。点（Point）。
1in = 2.54cm = 25.4 mm = 72pt = 6pc
**pc**
绝对长度单位。派卡（Pica）。相当于我国新四号铅字的尺寸。
1in = 2.54cm = 25.4 mm = 72pt = 6pc
**in**
绝对长度单位。英寸（Inch）。
1in = 2.54cm = 25.4 mm = 72pt = 6pc
**mm**
绝对长度单位。毫米（Millimeter）。
1in = 2.54cm = 25.4 mm = 72pt = 6pc
**cm**
绝对长度单位。厘米（Centimeter）。
1in = 2.54cm = 25.4 mm = 72pt = 6pc
代码示例：
|<|html|>
|<|head|>
|<|title|>|文字大小|</|title|>
|<|style|>
|<!--
|p|.inch|{|font-size|:|0.5in|; }
|p|.cm|{|font-size|:|0.5cm|; }
|p|.mm|{|font-size|:|4mm|; }
|p|.pt|{|font-size|:|12pt|; }
|p|.pc|{|font-size|:|2pc|; }
|p|.px|{|font-size|:|12px|;}
|-->
|</|style|>
|</|head|>
|<|body|>
|<|p|class|=|"inch"|>|文字大小，|0.5in|</|p|>
|<|p|class|=|"cm"|>|文字大小，|0.5cm|</|p|>
|<|p|class|=|"mm"|>|文字大小，|4mm|</|p|>
|<|p|class|=|"pt"|>|文字大小，|12pt|</|p|>
|<|p|class|=|"pc"|>|文字大小，|2pc|</|p|>
|<|p|class|=|"px"|>|文字大小|</|p|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/20180517234057724?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-----------------------------------------------------------------------
**文字加粗、斜体、装饰线**
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|字体加粗|</|title|>
|<|style|type|=|"text/css"|>
|p|{
|font-size|:|30px|;
|}
|.p1|{|font-weight|:|normal|; }
|.p2|{|font-weight|:|lighter|;}
|.p3|{|font-weight|:|bold|;   }
|.p4|{|font-weight|:|bolder|; }
|.p5|{|font-style|:|italic|;  }
|.p6|{|font-style|:|oblique|; }
|.p7|{|text-decoration|:|none|; }
|.p8|{|text-decoration|:|underline|; }
|.p9|{|text-decoration|:|overline|; }
|.p10|{|text-decoration|:|line-through|; }
|.p11|{|text-decoration|:|blink|; }
|.p12|{|text-decoration|:|inherit|; }
|</|style|>
|</|head|>
|<|body|>
|<|h1|>|字体加粗|</|h1|>
|<|p|class|=|"p1"|>|我是正常体|</|p|>
|<|p|class|=|"p2"|>|我是小加粗|</|p|>
|<|p|class|=|"p3"|>|我是大加粗|</|p|>
|<|p|class|=|"p4"|>|我是最加粗|</|p|>
|<|p|class|=|"p5"|>|我是斜体|</|p|>
|<|p|class|=|"p6"|>|我是斜体|</|p|>
|<|p|class|=|"p7"|>|我没有线|</|p|>
|<|p|class|=|"p8"|>|我是下划线|</|p|>
|<|p|class|=|"p9"|>|我是上划线|</|p|>
|<|p|class|=|"p10"|>|我是删除线|</|p|>
|<|p|class|=|"p11"|>|我是闪烁|</|p|>
|<|p|class|=|"p12"|>|我是继承|</|p|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/20180517234113938?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
----------------------------------------------------------
**文字对齐方式**
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|对齐方式|</|title|>
|<|style|type|=|"text/css"|>
|/*|元素内部的内部的对齐方式|*/
|.left|{|text-align|:|left|;  }
|.center|{|text-align|:|center|;}
|.right|{|text-align|:|right|; }
|</|style|>
|</|head|>
|<|body|>
|<|p|class|=|"left"|>|作者：李昆鹏|</|p|>
|<|p|class|=|"center"|>|作者：李昆鹏|</|p|>
|<|p|class|=|"right"|>|作者：李昆鹏|</|p|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/2018051723413024?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-------------------------------------------------------------
**实现首字放大效果**
代码示例：
|<|html|>
|<|head|>
|<|title|>|首字放大|</|title|>
|<|style|>
|<!--
|body|{
|background-color|:|black|;|/*|背景色|*/
|}
|p|{
|font-size|:|15px|;|/*|文字大小|*/
|color|:|white|;|/*|文字颜色|*/
|}
|p|span|{
|font-size|:|60px|;|/*|首字大小|*/
|float|:|left|;|/*|首字下沉|*/
|padding-right|:|5px|;|/*|与右边的间隔|*/
|font-weight|:|bold|;|/*|粗体字|*/
|font-family|:|黑体|;|/*|黑体字|*/
|color|:|yellow|;|/*|字体颜色|*/
|}
|</|style|>
|</|head|>
|<|body|>
|<|p|><|span|>|中|</|span|>|秋节是远古天象崇拜|——|敬月习俗的遗痕。据《周礼|¡¤|春官》记载，周代已有|“|中秋夜迎寒|”|、|“|中秋献良裘|”|、|“|秋分夕月（拜月）|”|的活动；汉代，又在中秋或立秋之日敬老、养老，赐以雄粗饼。晋时亦有中秋赏月之举，不过不太普遍；直到唐代将中秋与储娥奔月、吴刚伐桂、玉兔捣药、杨贵妃变月神、唐明皇游月宫等神话故事结合起，使之充满浪漫色彩，玩月之风方才大兴。|</|p|>
|<|p|>|北宋，正式定八月十五为中秋节，并出现|“|小饼如嚼月，中有酥和饴|”|的节令食品。孟元老《东京梦华录》说：|“|中秋夜，贵家结饰台榭，民间争占酒楼玩月|”|；而且|“|弦重鼎沸，近内延居民，深夜逢闻笙芋之声，宛如云外。间里儿童，连宵婚戏；夜市骈阗，至于通晓。|”|吴自牧《梦梁录》说：|“|此际金凤荐爽，玉露生凉，丹桂香飘，银蟾光满。王孙公子，富家巨室，莫不登危楼，临轩玩月，或开广榭，玳筵罗列，琴瑟铿锵，酌酒高歌，以卜竟夕之欢。|</|p|>
|</|body|>
|</|html|>
|
![](https://img-blog.csdn.net/20180517234143743?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

