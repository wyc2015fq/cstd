# css div自适应高度height - z69183787的专栏 - CSDN博客
2014年02月08日 09:46:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2311
DIVCSS5为大家介绍两种情况下自适应高度样式设置与解决。
**第一种：**一定高度内容加多时自适应高度，而内容少时[DIV](http://www.divcss5.com/)有一定最小高度；
**第二种：**没有最小高度，DIV完全自适应高度。
以下为大家介绍这两种情况与解决方法
### 一、一定最小高度，内容加多时自适应高度，而内容少时DIV有一定最小高度   -   [TOP](http://www.divcss5.com/jiqiao/j499.shtml#top)
有一DIV盒子,默认高度为200px,当DIV里内容多余并超出限定高度时,DIV自适应[高度](http://www.divcss5.com/rumen/r123.shtml), 要求使用纯[CSS+DIV](http://www.divcss5.com/),不需要JS,兼容火狐浏览器
**1、回答与解释：**
- _height:200px; /* [css 注解](http://www.divcss5.com/rumen/r32.shtml)： 仅IE6设别此属性，假定最低高度是200px ，设置高度200px，内容超出后IE6会自动撑高设定高度 */ 
- min-height:200px; /* [css注释](http://www.divcss5.com/rumen/r32.shtml)： css最小高度为200px支持所有浏览器，IE6浏览器除外 */ 
这个设置，ie6,7,8 ,9,10,火狐等都能兼容
**2、具体设置代码：**
- div{ 
- _height:200px; 
- min-height:200px 
- /* [css 注释](http://www.divcss5.com/rumen/r32.shtml)：两个放置不分前后顺序，兼容所有浏览器 */ 
- } 
**3、CSS最小高度与自适应高度并存案例**
我们设置2个DIV盒子，最小高度均为200px，当内容较少时DIV盒子的最小高度为200px,当内容比较多超出高度能装下时，DIV盒子自适应高度，为了便于观察与参考分析，我们统一设置[宽度](http://www.divcss5.com/rumen/r119.shtml)为100px,一个黑色1px [CSS边框](http://www.divcss5.com/rumen/r120.shtml)。
**[CSS代码](http://www.divcss5.com/rumen/r95.shtml)如下：**
- div{_height:200px; min-height:200px; border:1px solid #000; width:100px} 
- /* css注释：设置最小高度，[border边框](http://www.divcss5.com/rumen/r120.shtml)，宽度 */ 
**[HTML代码](http://www.divcss5.com/html/h393.shtml)片段：**
- <div>200高度能装下这点内容，设置最小高度200px</div>
- <div>设置最小高度200px<br/>
- 而内容多，超出200px高度限制，DIV自适应高度<br/>
- <br/>
- 
- 案例占位<br/>
- 案例占位<br/>
- 案例占位<br/>
- 案例占位<br/>
- 案例占位<br/>
- 案例占位<br/>
- 案例占位<br/>
- 案例占位<br/>
- 案例占位<br/>
- 案例占位<br/>
- 案例占位<br/>
- 案例占位<br/>
- 案例占位 
- </div>
**4、在浏览器案例截图**
![css自适应高度运用截图](http://www.divcss5.com/uploads/allimg/130513/1_130513093307_1.png)
css自适应高度案例截图
**在线演示：**[查看案例](http://www.divcss5.com/yanshi/2013051301.html)
### 二、CSS默认自适应高度   -   [TOP](http://www.divcss5.com/jiqiao/j499.shtml#top)
当我们不对DIV设置固定高度或不设置[CSS高度](http://www.divcss5.com/rumen/r123.shtml)样式，其DIV盒子默认是自适应高度。
### 三、总结   -   [TOP](http://www.divcss5.com/jiqiao/j499.shtml#top)
我们不对DIV设置高度样式，默认情况下是自适应高度，当我们需要设置一个[CSS最小高度](http://www.divcss5.com/rumen/r422.shtml)时，所有浏览器都支持DIV盒子最小高度演示，但IE6不支持min-height样式，我们就需要设置针对IE6的[css
 hack](http://www.divcss5.com/css-hack/)实现，或去了解[ie6支持min-width max-width](http://www.divcss5.com/jiqiao/j298.shtml)，[ie6支持min-height
 max-height](http://www.divcss5.com/jiqiao/j299.shtml)。
如需转载，请注明文章出处和来源网址：[http://www.divcss5.com/jiqiao/j499.shtml](http://www.divcss5.com/jiqiao/j499.shtml)
