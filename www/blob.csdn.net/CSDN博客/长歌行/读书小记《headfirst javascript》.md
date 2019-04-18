# 读书小记《headfirst javascript》 - 长歌行 - CSDN博客





2013年03月14日 22:53:29[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1591








第一天P1-P104

alert("")




onload = ""




onclick = ""




<script type = "test/javascript">

,</scipt>




var




parseInt("1")




document.getElementById("").value




<input type= ""  value = "" onclick = "function();">




isNaN()




indexOf("")




setTimeout(";", 60000);  / setInterval




document.body.clientHeight




第二天 P104-P145

onresize="";

cookie(今天是3.15关于cookie的话题比较热啊)

document.cookie = "name=XXXX;expiers="1213451";path=/"

prompt

navigator.cookieEnabled

与用户沟通......讲了总比没有讲好

客户端才是JavaScript运行的地方




第三天 P146-P292

var seats = new Array(new Array(9), new Array(9), new Array(9), new Array(9));

windows.onload = initSeats;   // 因为是对initSeat()的引用，所以不需要加括号




documnet.getElementById("seat6").onclick = function(evt){showSeateStatus(6);};




第四天 P293-P392

表单 域

onfocusonblur onchange

<span id = "" class = ""></span>

表单验证 正则表达式 

.

\s

\d

^

\w

$

*

{n}

+

?

()




/^\w{5,8}$

DOM

getElementById

getElementByTagName

<div></div>

决策树




第五天P293-P537

数据+行为=对象

构造函数

var invitation = new Invitation("","");

function Invitation(who, where)

{

this.who  = who;

this.where = where;

}




貌似在Javascript中，只有使用prototype才会创建类的特性，而非实例的特性。

C++表示毫无压力，类方法的地址都是单独存储的。即使删掉根据类所创建的实例的指针，也可以调用类的函数。

不使用prototype则为类的方法

第六天 The rest of all

Ajax

XMLHttpRequest

异步

DOM

PHP

好吧，本书最后一章，也是最神奇的一章。

对Javascript的回顾结束了，开始系统的练习了。






