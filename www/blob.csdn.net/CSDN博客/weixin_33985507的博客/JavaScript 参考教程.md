# JavaScript 参考教程 - weixin_33985507的博客 - CSDN博客
2008年03月28日 00:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2

## 事件处理
### 事件处理概述
　　 事件处理是对象化编程的一个很重要的环节，没有了事件处理，程序就会变得很死，缺乏灵活性。事件处理的过程可以这样表示：发生事件 - 启动事件处理程序 - 事件处理程序作出反应。其中，要使事件处理程序能够启动，必须先告诉对象，如果发生了什么事情，要启动什么处理程序，否则这个流程就不能进行下去。事件的处理程序可以是任意 JavaScript 语句，但是我们一般用特定的自定义函数（function）来处理事情。
### 指定事件处理程序
　　指定事件处理程序有三种方法：
**方法一** 直接在 HTML 标记中指定。这种方法是用得最普遍的。方法是：
> 
<标记 ... ... 事件="事件处理程序" [事件="事件处理程序" ...]>
让我们来看看例子：
> 
<body ... onload="alert('网页读取完成，请慢慢欣赏！')" onunload="alert('再见！')">
这样的定义<body>标记，能使文档读取完毕的时候弹出一个对话框，写着“网页读取完成，请慢慢欣赏”；在用户退出文档（或者关闭窗口，或者到另一个页面去）的时候弹出“再见”。
**方法二** 编写特定对象特定事件的 JavaScript。这种方法用得比较少，但是在某些场合还是很好用的。方法是：
> 
<script language="JavaScript" for="对象" event="事件">
...
(事件处理程序代码)
...
</script>
例：
> 
<script language="JavaScript" for="window" event="onload">
  alert('网页读取完成，请慢慢欣赏！');
</script>
**方法三** 在 JavaScript 中说明。方法：
<事件主角 - 对象>.<事件> = <事件处理程序>;
用这种方法要注意的是，“事件处理程序”是真正的代码，而不是字符串形式的代码。如果事件处理程序是一个自定义函数，如无使用参数的需要，就不要加“()”。例：
> 
...
function ignoreError() {
  return true;
}
...
window.onerror = ignoreError; // 没有使用“()”
这个例子将 ignoreError() 函数定义为 window 对象的 onerror 事件的处理程序。它的效果是忽略该 window 对象下任何错误（由引用不允许访问的 location 对象产生的“没有权限”错误是不能忽略的）。
### 事件详解
**onblur 事件** 发生在窗口失去焦点的时候。
**　应用于：[window 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#window%20%E7%AA%97%E5%8F%A3%E5%AF%B9%E8%B1%A1)**
**onchange 事件** 发生在文本输入区的内容被更改，然后焦点从文本输入区移走之后。捕捉此事件主要用于实时检测输入的有效性，或者立刻改变文档内容。
**　应用于：[Password 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#Password%20%E5%AF%86%E7%A0%81%E8%BE%93%E5%85%A5%E5%8C%BA%E5%AF%B9%E8%B1%A1)；[Select 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#Select%20%E9%80%89%E6%8B%A9%E5%8C%BA%EF%BC%88%E4%B8%8B%E6%8B%89%E8%8F%9C%E5%8D%95%E3%80%81%E5%88%97%E8%A1%A8%EF%BC%89%E5%AF%B9%E8%B1%A1)；[Text 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#Text%20%E6%96%87%E6%9C%AC%E6%A1%86%E5%AF%B9%E8%B1%A1)；[Textarea 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#Textarea%20%E5%A4%9A%E8%A1%8C%E6%96%87%E6%9C%AC%E8%BE%93%E5%85%A5%E5%8C%BA%E5%AF%B9%E8%B1%A1)**
**onclick 事件** 发生在对象被单击的时候。单击是指鼠标停留在对象上，按下鼠标键，没有移动鼠标而放开鼠标键这一个完整的过程。
　　一个普通按钮对象（Button）通常会有 onclick 事件处理程序，因为这种对象根本不能从用户那里得到任何信息，没有 onclick 事件处理程序就等于废柴。按钮上添加 onclick 事件处理程序，可以模拟“另一个提交按钮”，方法是：在事件处理程序中更改表单的 action, target, encoding, method 等一个或几个属性，然后调用表单的 submit() 方法。
　　在 Link 对象的 onclick 事件处理程序中返回 false 值（return false），能阻止浏览器打开此连接。即，如果有一个这样的连接：<a href="http://www.a.com" onclick="return false">Go!</a>，那么无论用户怎样点击，都不会去到 www.a.com 网站，除非用户禁止浏览器运行 JavaScript。
**　应用于：[Button 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#Button%20%E6%8C%89%E9%92%AE%E5%AF%B9%E8%B1%A1)；[Checkbox 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#Checkbox%20%E5%A4%8D%E9%80%89%E6%A1%86%E5%AF%B9%E8%B1%A1)；[Image 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#images%5B%5D;%20Image%20%E5%9B%BE%E7%89%87%E5%AF%B9%E8%B1%A1)；[Link 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#anchors%5B%5D;%20links%5B%5D;%20Link%20%E8%BF%9E%E6%8E%A5%E5%AF%B9%E8%B1%A1)；[Radio 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#Radio%20%E5%8D%95%E9%80%89%E5%9F%9F%E5%AF%B9%E8%B1%A1)；[Reset 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#Reset%20%E9%87%8D%E7%BD%AE%E6%8C%89%E9%92%AE%E5%AF%B9%E8%B1%A1)；[Submit 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#Submit%20%E6%8F%90%E4%BA%A4%E6%8C%89%E9%92%AE%E5%AF%B9%E8%B1%A1)**
**onerror 事件** 发生在错误发生的时候。它的事件处理程序通常就叫做“错误处理程序”(Error Handler)，用来处理错误。上边已经介绍过，要忽略一切错误，就使用：
> 
function ignoreError() {
  return true;
}
window.onerror = ignoreError;
**　应用于：[window 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#window%20%E7%AA%97%E5%8F%A3%E5%AF%B9%E8%B1%A1)**
**onfocus 事件** 发生在窗口得到焦点的时候。
**　应用于：[window 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#window%20%E7%AA%97%E5%8F%A3%E5%AF%B9%E8%B1%A1)**
**onload 事件** 发生在文档全部下载完毕的时候。全部下载完毕意味着不但 HTML 文件，而且包含的图片，插件，控件，小程序等全部内容都下载完毕。本事件是 window 的事件，但是在 HTML 中指定事件处理程序的时候，我们是把它写在<body>标记中的。
**　应用于：[window 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#window%20%E7%AA%97%E5%8F%A3%E5%AF%B9%E8%B1%A1)**
**onmousedown 事件** 发生在用户把鼠标放在对象上按下鼠标键的时候。参考 onmouseup 事件。
**　应用于：[Button 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#Button%20%E6%8C%89%E9%92%AE%E5%AF%B9%E8%B1%A1)；[Link 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#anchors%5B%5D;%20links%5B%5D;%20Link%20%E8%BF%9E%E6%8E%A5%E5%AF%B9%E8%B1%A1)**
**onmouseout 事件** 发生在鼠标离开对象的时候。参考 onmouseover 事件。
**　应用于：[Link 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#anchors%5B%5D;%20links%5B%5D;%20Link%20%E8%BF%9E%E6%8E%A5%E5%AF%B9%E8%B1%A1)**
**onmouseover 事件** 发生在鼠标进入对象范围的时候。这个事件和 onmouseout 事件，再加上图片的预读，就可以做到当鼠标移到图像连接上，图像更改的效果了。有时我们看到，在指向一个连接时，状态栏上不显示地址，而显示其它的资料，看起来这些资料是可以随时更改的。它们是这样做出来的：
> 
<a href="..."
   onmouseover="window.status='Click Me Please!'; return true;"
   onmouseout="window.status=''; return true;">
**　应用于：[Link 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#anchors%5B%5D;%20links%5B%5D;%20Link%20%E8%BF%9E%E6%8E%A5%E5%AF%B9%E8%B1%A1)**
**onmouseup 事件** 发生在用户把鼠标放在对象上鼠标键被按下的情况下，放开鼠标键的时候。如果按下鼠标键的时候，鼠标并不在放开鼠标的对象上，则本事件不会发生。
**　应用于：****[Button 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#Button%20%E6%8C%89%E9%92%AE%E5%AF%B9%E8%B1%A1)；****[Link 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#anchors%5B%5D;%20links%5B%5D;%20Link%20%E8%BF%9E%E6%8E%A5%E5%AF%B9%E8%B1%A1)**
**onreset 事件** 发生在表单的“重置”按钮被单击（按下并放开）的时候。通过在事件处理程序中返回 false 值（return false）可以阻止表单重置。
**　应用于：[Form 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#forms%5B%5D;%20Form%20%E8%A1%A8%E5%8D%95%E5%AF%B9%E8%B1%A1)**
**onresize 事件** 发生在窗口被调整大小的时候。
**　应用于：[window 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#window%20%E7%AA%97%E5%8F%A3%E5%AF%B9%E8%B1%A1)**
**onsubmit 事件** 发生在表单的“提交”按钮被单击（按下并放开）的时候。可以使用该事件来验证表单的有效性。通过在事件处理程序中返回 false 值（return false）可以阻止表单提交。
**　应用于：[Form 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#forms%5B%5D;%20Form%20%E8%A1%A8%E5%8D%95%E5%AF%B9%E8%B1%A1)**
**onunload 事件** 发生在用户退出文档（或者关闭窗口，或者到另一个页面去）的时候。与 onload 一样，要写在 HTML 中就写到<body>标记里。
　　有的 Web Masters 用这个方法来弹出“调查表单”，以“强迫”来者填写；有的就弹出广告窗口，唆使来者点击连接。我觉得这种“onunload="open..."”的方法很不好，有时甚至会因为弹出太多窗口而导致资源缺乏。有什么对来者说就应该在网页上说完，不对吗？
**　应用于：[window 对象](http://www.iselong.com/online/ebooks/javascript/js_3.htm#window%20%E7%AA%97%E5%8F%A3%E5%AF%B9%E8%B1%A1)**
### 关于对象化编程的语句
　　现在我们有实力学习以下关于对象化编程，但其实属于上一章的内容了。
**with 语句** 为一个或一组语句指定默认对象。
用法：with (<对象>) <语句>;
with 语句通常用来缩短特定情形下必须写的代码量。在下面的例子中，请注意 Math 的重复使用： 
> 
x = Math.cos(3 * Math.PI) + Math.sin(Math.LN10);
y = Math.tan(14 * Math.E);
当使用 with 语句时，代码变得更短且更易读： 
> 
with (Math) {
  x = cos(3 * PI) + sin(LN10);
  y = tan(14 * E);
}
**this 对象** 返回“当前”对象。在不同的地方，this 代表不同的对象。如果在 JavaScript 的“主程序”中（不在任何 function 中，不在任何事件处理程序中）使用 this，它就代表 window 对象；如果在 with 语句块中使用 this，它就代表 with 所指定的对象；如果在事件处理程序中使用 this，它就代表发生事件的对象。
一个常用的 this 用法：
> 
<script>
...
function check(formObj) {
  ...
}
...
</script>
<body ...>
...
<form ...>
...
<input type="text" ... onchange="check(**this**.form)">
...
</form>
...
</body>
这个用法常用于立刻检测表单输入的有效性。
**自定义构造函数**我们已经知道，Array()，Image()等构造函数能让我们构造一个变量。其实我们自己也可以写自己的构造函数。自定义构造函数也是用 function。在 function 里边用 this 来定义属性。
> 
function <构造函数名> [(<参数>)] {
  ...
  this.<属性名> = <初始值>;
  ...
}
然后，用 new 构造函数关键字来构造变量：
> 
var <变量名> = new <构造函数名>[(<参数>)];
构造变量以后，<变量名>成为一个对象，它有它自己的属性——用 this 在 function 里设定的属性。
以下是一个从网上找到的搜集浏览器详细资料的自定义构造函数的例子：
> 
function Is() {
  var agent = navigator.userAgent.toLowerCase();
  this.major = parseInt(navigator.appVersion);  //主版本号
  this.minor = parseFloat(navigator.appVersion);//全版本号
  this.ns = ((agent.indexOf('mozilla')!=-1) &&
             ((agent.indexOf('spoofer')==-1) && //是否 Netscape
              (agent.indexOf('compatible') == -1)));
  this.ns2 = (this.ns && (this.major == 3));    //是否 Netscape 2
  this.ns3 = (this.ns && (this.major == 3));    //是否 Netscape 3
  this.ns4b = (this.ns && (this.minor < 4.04)); //是否 Netscape 4 低版本
  this.ns4 = (this.ns && (this.major >= 4));    //是否 Netscape 4 高版本
  this.ie = (agent.indexOf("msie") != -1);      //是否 IE
  this.ie3 = (this.ie && (this.major == 2));    //是否 IE 3
  this.ie4 = (this.ie && (this.major >= 4));    //是否 IE 4
  this.op3 = (agent.indexOf("opera") != -1);    //是否 Opera 3
  this.win = (agent.indexOf("win")!=-1);        //是否 Windows 版本
  this.mac = (agent.indexOf("mac")!=-1);        //是否 Macintosh 版本
  this.unix = (agent.indexOf("x11")!=-1);       //是否 Unix 版本
}
var is = new Is();
这个构造函数非常完整的搜集了浏览器的信息。我们看到它为对象定义了很多个属性：major, minor, ns, ie, win, mac 等等。它们的意思见上面的注释。把 is 变量定义为 Is() 对象后，用 if (is.ns) 这种格式就可以很方便的知道浏览器的信息了。我们也可以从这个构造函数中看到，它也可以使用一般的 JavaScript 语句（上例中为 var 语句）。
让我们再来看一个使用参数的构造函数：
> 
function myFriend(theName, gender, theAge, birthOn, theJob) {
  this.name = theName;
  this.isMale = (gender.toLowerCase == 'male');
  this.age = theAge;
  this.birthday = new Date(birthOn);
  this.job = theJob
}
var Stephen = new myFriend('Stephen', 'Male', 18, 'Dec 22, 1982', 'Student');
从这个构造函数我们不但看到了参数的用法，还看到了不同的属性用不同的数据型是可以的（上例五个属性分别为：字符串，布尔值，数字，日期，字符串），还看到了构造函数里也可以用构造函数来“构造”属性。如果用了足够的“保护措施”来避免无限循环，更可以用构造函数自身来构造自己的属性。
## 使用框架和Cookies
### 使用框架
　　在讲述 window 对象的时候，我们提到过，一个框架内的网页也是 window 对象，也就是说，Frame 对象也是 window 对象。用最容易理解的话说，每一个 HTML 文件占用一个 window 对象，包括定义框架的网页（“框架网页”）。在 IE 里用“<iframe>”标记在文档中插入的框架也是 window 对象，但是用“包含网页”的方法（在 HTML 中显示为“<!--webbot bot="include" ...-->”）读取的 HTML 就不占用独自的 window 对象。每一个框架都是包含它的页的 window 对象的一个子对象（不知道应该叫“属性”不该），要引用它，可以用以下几种方法之一：
> 
window.frames[x]
window.frames['frameName']
window.frameName
　　其中，x 指的是该 window 对象中指定的第几个框架，与其它数组一样，x 也是从零开始的。frameName 指的是该框架的名字，跟<frame>里的“name”属性一样。
　　如果使用 window.frameName 指定的 window 对象又是一个框架网页，那么引用它的框架的方法：window.frameName.subFrameName。以此类推。
　　要注意的时，无论在何处，引用“window”对象所返回的，都是“当前”window 对象。如果要访问其它 window 对象，就要用到 parent 和 top 属性。parent 指的是“父级”window 对象，也就是包含当前 window 对象的框架网页；top 指的是窗口最顶端的 window 对象。
　　使用框架还要密切留意你的 JavaScript 中定义的全局变量和自定义函数。它们都有它们的所属——所在的 window 对象。要引用其它框架中的全局变量或自定义函数，都要用“窗口对象.框架对象[.框架对象…].全局变量或自定义函数”这种很烦的方法。
　　以上这个问题在建立连接时经常会被忽略：如果在<head>中定义了一个默认目标窗口（<base target="...">），在<a href="javascript:...">中，要知道输入的 JavaScript 语句是在默认目标窗口中运行的，必要时加一些“parent”“top”属性。
### 使用 Cookies
　　我们已经知道，在 document 对象中有一个 cookie 属性。但是 Cookie 又是什么？“某些 Web 站点在您的硬盘上用很小的文本文件存储了一些信息，这些文件就称为 Cookie。”—— MSIE 帮助。一般来说，Cookies 是 CGI 或类似，比 HTML 高级的文件、程序等创建的，但是 JavaScript 也提供了对 Cookies 的很全面的访问权利。
　　在继续之前，我们先要学一学 **Cookie 的基本知识**。
　　每个 Cookie 都是这样的：<cookie名>=<值>
　　<cookie名>的限制与 JavaScript 的命名限制大同小异，少了“不能用 JavaScript 关键字”，多了“只能用可以用在 URL 编码中的字符”。后者比较难懂，但是只要你只用字母和数字命名，就完全没有问题了。<值>的要求也是“只能用可以用在 URL 编码中的字符”。
　　每个 Cookie 都有失效日期，一旦电脑的时钟过了失效日期，这个 Cookie 就会被删掉。我们不能直接删掉一个 Cookie，但是可以用设定失效日期早于现在时刻的方法来间接删掉它。
　　每个网页，或者说每个站点，都有它自己的 Cookies，这些 Cookies 只能由这个站点下的网页来访问，来自其他站点或同一站点下未经授权的区域的网页，是不能访问的。每一“组”Cookies 有规定的总大小（大约 2KB 每“组”），一超过最大总大小，则最早失效的 Cookie 先被删除，来让新的 Cookie“安家”。
　　现在我们来学习**使用 document.cookie 属性**。
　　如果直接使用 document.cookie 属性，或者说，用某种方法，例如给变量赋值，来获得 document.cookie 的值，我们就可以知道在现在的文档中有多少个 Cookies，每个 Cookies 的名字，和它的值。例如，在某文档中添加“document.write(document.cookie)”，结果显示：
> 
name=kevin; email=kevin@kevin.com; lastvisited=index.html
这意味着，文档包含 3 个 Cookies：name, email 和 lastvisited，它们的值分别是 kevin, kevin@kevin.com 和 index.html。可以看到，两个 Cookies 之间是用分号和空格隔开的，于是我们可以用 cookieString.split('; ') 方法得到每个 Cookie 分开的一个数组（先用 var cookieString = document.cookie）。
　　设定一个 Cookie 的方法是对 document.cookie 赋值。与其它情况下的赋值不同，向 document.cookie 赋值不会删除掉原有的 Cookies，而只会增添 Cookies 或更改原有 Cookie。赋值的格式：
> 
document.cookie = '**cookieName**=' + escape('**cookieValue**')
     + ';expires=' + **expirationDateObj**.toGMTString();
是不是看到头晕了呢？以上不是粗体字的地方是要照抄不误的，粗体字是要按实际情况做出改动的。cookieName 表示 Cookie 的名称，cookieValue 表示 Cookie 的值，expirationDateObj 表示储存着失效日期的日期对象名，如果不需要指定失效日期，则不需要第二行。不指定失效日期，则浏览器默认是在关闭浏览器（也就是关闭所有窗口）之后过期。
　　看到了上面的一些下划线了么？这些是应该注意的地方。
　　首先 escape() 方法：为什么一定要用？因为 Cookie 的值的要求是“只能用可以用在 URL 编码中的字符”。我们知道“escape()”方法是把字符串按 URL 编码方法来编码的，那我们只需要用一个“escape()”方法来处理输出到 Cookie 的值，用“unescape()”来处理从 Cookie 接收过来的值就万无一失了。而且这两个方法的最常用途就是处理 Cookies。其实设定一个 Cookie 只是“document.cookie = 'cookieName=cookieValue'”这么简单，但是为了避免在 cookieValue 中出现 URL 里不准出现的字符，还是用一个 escape() 好。
　　然后“expires”前面的分号：注意到就行了。是分号而不是其他。
　　最后 toGMTString() 方法：设定 Cookie 的时效日期都是用 GMT 格式的时间的，其它格式的时间是没有作用的。
　　现在我们来实战一下。设定一个“name=rose”的 Cookie，在 3 个月后过期。
> 
var expires = new Date();
expires.setTime(expires.getTime() + 3 * 30 * 24 * 60 * 60 * 1000);
/*   三个月 x 一个月当作 30 天 x 一天 24 小时
   x 一小时 60 分 x 一分 60 秒 x 一秒 1000 毫秒 */
document.cookie = 'name=rose;expires=' + expires.toGMTString();
为什么没有用 escape() 方法？这是因为我们知道 rose 是一个合法的 URL 编码字符串，也就是说，'rose' == escape('rose')。一般来说，如果设定 Cookie 时不用 escape()，那获取 Cookie 时也不用 unescape()。
　　再来一次：编写一个函数，作用是查找指定 Cookie 的值。
> 
function getCookie(cookieName) {
  var cookieString = document.cookie;
  var start = cookieString.indexOf(cookieName + '=');
  // 加上等号的原因是避免在某些 Cookie 的值里有
  // 与 cookieName 一样的字符串。
  if (start == -1) // 找不到
    return null;
  start += cookieName.length + 1;
  var end = cookieString.indexOf(';', start);
  if (end == -1) return unescape(cookieString.substring(start));
  return unescape(cookieString.substring(start, end));
}
这个函数用到了字符串对象的一些方法，如果你不记得了（你是不是这般没记性啊），请快去查查。这个函数所有的 if 语句都没有带上 else，这是因为如果条件成立，程序运行的都是 return 语句，在函数里碰上 return，就会终止运行，所以不加 else 也没问题。该函数在找到 Cookie 时，就会返回 Cookie 的值，否则返回“null”。
　　现在我们要删除刚才设定的 name=rose Cookie。
> 
var expires = new Date();
expires.setTime(expires.getTime() - 1);
document.cookie = 'name=rose;expires=' + expires.toGMTString();
可以看到，只需要把失效日期改成比现在日期早一点（这里是早 1 毫秒），再用同样的方法设定 Cookie，就可以删掉 Cookie 了。
