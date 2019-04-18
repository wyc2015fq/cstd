# CSS清除浮动_清除float浮动 - z69183787的专栏 - CSDN博客
2014年01月27日 15:31:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：723

**CSS清除浮动方法集合**
### 一、浮动产生原因   -   [TOP](http://www.divcss5.com/jiqiao/j406.shtml#top)
一般浮动是什么情况呢？一般是一个盒子里使用了[CSS float](http://www.divcss5.com/rumen/r93.shtml)浮动属性，导致父级对象盒子不能被撑开，这样[CSS](http://www.divcss5.com/)[float浮动](http://www.divcss5.com/rumen/r93.shtml)就产生了。
![CSS浮动产生截图](http://www.divcss5.com/uploads/allimg/130214/1_130214135727_1.png)
浮动产生样式效果截图
本来两个黑色对象盒子是在红色盒子内，因为对两个黑色盒子使用了[float](http://www.divcss5.com/rumen/r93.shtml)浮动，所以两个黑色盒子产生了[浮动](http://www.divcss5.com/rumen/r93.shtml)，导致红色盒子不能撑开，这样浮动就产生了。
简单地说，浮动是因为使用了[float:left](http://www.divcss5.com/rumen/r93.shtml)或[float:right](http://www.divcss5.com/rumen/r93.shtml)或两者都是有了而产生的浮动。
### 二、浮动产生负作用   -   [TOP](http://www.divcss5.com/jiqiao/j406.shtml#top)
**1、背景不能显示**
由于浮动产生，如果对父级设置了（[CSS background背景](http://www.divcss5.com/rumen/r125.shtml)）[CSS背景颜色](http://www.divcss5.com/jiqiao/j369.shtml)或[CSS背景图片](http://www.divcss5.com/jiqiao/j369.shtml)，而父级不能被撑开，所以导致[CSS背景](http://www.divcss5.com/rumen/r125.shtml)不能显示。
**2、边框不能撑开**
如上图中，如果父级设置了[CSS边框](http://www.divcss5.com/rumen/r120.shtml)属性（[css
 border](http://www.divcss5.com/rumen/r120.shtml)），由于子级里使用了float属性，产生浮动，父级不能被撑开，导致边框不能随内容而被撑开。
**3、margin padding设置值不能正确显示**
由于浮动导致[父级子级](http://www.divcss5.com/rumen/r239.shtml)之间设置了css padding、[css
 margin](http://www.divcss5.com/rumen/r128.shtml)属性的值不能正确表达。特别是上下边的padding和[margin](http://www.divcss5.com/rumen/r128.shtml)不能正确显示。
### 三、css解决浮动，清除浮动方法   -   [TOP](http://www.divcss5.com/jiqiao/j406.shtml#top)
这里[DIVCSS5](http://www.divcss5.com/)为了统一讲解浮动解决方法，假设了有三个盒子对象，一个父级里包含了两个子级，子级一个使用了float:left属性，另外一个子级使用float:right属性。同时设置[div
 css](http://www.divcss5.com/)[border](http://www.divcss5.com/rumen/r120.shtml)，父级css边框颜色为红色，两个子级边框颜色为蓝色；[父级](http://www.divcss5.com/rumen/r239.shtml)CSS背景样式为黄色，两个子级背景为白色；父级[css
 width](http://www.divcss5.com/rumen/r119.shtml)宽度为400px，两个子级[css宽度](http://www.divcss5.com/rumen/r119.shtml)均为180px，两个[子级](http://www.divcss5.com/rumen/r239.shtml)再设置相同[高度](http://www.divcss5.com/rumen/r123.shtml)100px，父级[css
 height](http://www.divcss5.com/rumen/r123.shtml)高度暂不设置（通常为实际[css布局](http://www.divcss5.com/)时候这样父级都不设置高度，而高度是随内容增加自适应高度）。
父级[CSS命名](http://www.divcss5.com/jiqiao/j4.shtml)为“.divcss5”对应[html标签](http://www.divcss5.com/html/h323.shtml)使用“<[div](http://www.divcss5.com/) class="divcss5">”
两个子级CSS命名分别为“.divcss5-left”“.divcss5-right”
根据以上描述DIVCSS5给出对应[CSS代码](http://www.divcss5.com/rumen/r95.shtml)和[HTML代码](http://www.divcss5.com/html/h393.shtml)片段
**CSS代码：**
- .divcss5{ width:400px; border:1px solid #F00; background:#FF0} 
- .divcss5-left,.divcss5-right{ width:180px; height:100px; 
-  border:1px solid #00F; background:#FFF} 
- .divcss5-left{ float:left} 
- .divcss5-right{ float:right} 
**对应[html源代码](http://www.divcss5.com/html/h26.shtml)片段：**
- <divclass="divcss5">
- <divclass="divcss5-left">left浮动</div>
- <divclass="divcss5-right">right浮动</div>
- </div>
![清除浮动前截图](http://www.divcss5.com/uploads/allimg/130214/1_130214135824_1.png)
清除浮动前案例截图父级需要清除浮动
**以下DIVCSS5总结了几点用于清除浮动的经验教程**
**1、对父级设置适合CSS高度**
对父级设置适合高度样式清除浮动，这里对“.divcss5”设置一定高度即可，一般设置高度需要能确定内容高度才能设置。这里我们知道内容高度是100PX+上下边框为2px，这样具体父级高度为102px
**CSS代码：**
- .divcss5{ width:400px;border:1px solid #F00;background:#FF0; height:102px} 
- .divcss5-left,.divcss5-right{width:180px;height:100px; 
- border:1px solid #00F;background:#FFF} 
- .divcss5-left{ float:left} 
- .divcss5-right{ float:right} 
[Html](http://www.divcss5.com/html/)代码不变。得到截图
![CSS高度设置清除浮动法](http://www.divcss5.com/uploads/allimg/130214/1_130214135937_1.png)
使用height高度清除浮动
小结，使用设置高度样式，清除浮动产生，前提是对象内容高度要能确定并能计算好。
**2、[clear:both](http://www.divcss5.com/rumen/r424.shtml)清除浮动**
为了统一样式，我们新建一个样式选择器CSS命名为“.clear”，并且对应选择器样式为“clear:both”，然后我们在父级“</div>”结束前加此div引入“class="[clear](http://www.divcss5.com/rumen/r424.shtml)"”样式。这样即可清除浮动。
具体CSS代码：
- .divcss5{ width:400px;border:1px solid #F00;background:#FF0} 
- .divcss5-left,.divcss5-right{width:180px;height:100px; 
- border:1px solid #00F;background:#FFF} 
- .divcss5-left{ float:left} 
- .divcss5-right{ float:right} 
- .clear{ clear:both} 
Html代码：
- <divclass="divcss5">
- <divclass="divcss5-left">left浮动</div>
- <divclass="divcss5-right">right浮动</div>
- **<divclass="clear"></div>**
- </div>
clear清除浮动截图
![clear清除浮动截图](http://www.divcss5.com/uploads/allimg/130214/1_130214140203_1.png)
clear清除浮动截图 使用CSS clear清除浮动
这个[css clear](http://www.divcss5.com/rumen/r424.shtml)清除float产生浮动，可以不用对父级设置高度 也无需技术父级高度，方便适用，但会多加CSS和HTML标签。
**3、父级div定义 overflow:hidden**
对父级CSS选择器加overflow:hidden样式，可以清除父级内使用float产生浮动。优点是可以很少CSS代码即可解决浮动产生。
overflow:hidden解决CSS代码：
- .divcss5{ width:400px;border:1px solid #F00;background:#FF0; overflow:hidden} 
- .divcss5-left,.divcss5-right{width:180px;height:100px;
border:1px solid #00F;background:#FFF} 
- .divcss5-left{ float:left} 
- .divcss5-right{ float:right} 
HTML代码不变。
解决清除浮动后截图
![css overflow清除浮动截图](http://www.divcss5.com/uploads/allimg/130214/1_130214140301_1.png)
overflow清除float产生浮动截图 overflow:hidden清除浮动截图
为什么加入overflow:hidden即可清除浮动呢？那是因为overflow:hidden属性相当于是让父级紧贴内容，这样即可紧贴其对象内内容（包括使用float的div盒子），从而实现了清除浮动。Css overflow:hidden清除浮动方法[DIVCSS5](http://www.divcss5.com/)推荐使用。
以上三点即是兼容各大浏览器清除浮动的方法，其它有的浏览器不兼容有的不兼容的方法就没必要介绍了，大家记住以上三点解决float浮动清除浮动方法。但这里推荐第三点和第二点解决清除浮动方法。
如需转载，请注明文章出处和来源网址：[http://www.divcss5.com/jiqiao/j406.shtml](http://www.divcss5.com/jiqiao/j406.shtml)
