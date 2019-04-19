# JavaScript中的触发事件 - Big Smile - CSDN博客
2016年07月24日 13:44:47[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1502
        这两天在敲订餐管理系统的界面的时候，才感觉到自己在前端这里才疏学浅，什么都没有整理过，因为页面的效果想使用JavaScript来展示，却发现不知道JS的代码是如何被驱动的，因此才有了这篇博客，帮助大家了解都有什么事件能够触发JS的代码执行，这样对于我们的前台页面的构思也会有很大的帮助。
# 一、代码的书写位置
        代码的书写位置有很多，之一就是在整个页面前面写的代码，叫做内嵌式：
![](https://img-blog.csdn.net/20160724132758627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        还有在head中连接地址的叫做外联式：
![](https://img-blog.csdn.net/20160724132847041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        最后一种是内联模式，常见于html页面的控件中：
![](https://img-blog.csdn.net/20160724133018191?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
当然上图是在JS中写的正则表达式。
        有了位置的概念，那么触发的概念也就来了。
# 二、JavaScript代码的触发
①、默认事件触发：如上面的最后一个JS里面写的正则表达式的代码，其中onkeypress，onkeyup，onblur都是这个代码的触发事件，然后再触发事件的后面写要执行的代码。
**具体事件有：**
**1.单击事件___onclick**
      用户单击鼠标按键时产生的事件.同时onclick指定的事件处理程序或代码将被调用执行.
      如:<input type="button" value="打开页面"                    onlick="window.open('xxxx.html','newwindow','width=456,height=230,toolbar=no,menubar=no,scrollbars=yes');">
**2.改变事件___onchange**
      当text或textarea元素内的字符值改变或select表格选项状态改变里发生该事件.
      如;<textarea name="liuyan" rows=5 cols=70 value=" " onchange=alert("您在文本框中添加了新的内容")>
**3.选中事件____onselect**
      当text或textarea对象中的文字被选中里会引发该事件
      如:<ipnut type="text" value="默认信息" onselect=alert("您选中了文本框中的文字")>
**4.获得焦点事件____onfocus**
      用户单击text或textarea以及select对象时,即光标落在文本框或选择框时会产生该事件.
      如:<select name="zhengjian" onfocus="alert("我成为焦点")>
**5.失去焦点事件______onblur**
      失去焦点事件正好与获得焦点事件相对,当text对象,textarea对象或select对象不再拥有焦点而退出后台时,引发该事件.
**6.载入文件事件____onload**
      当页面文件载入时,产生该事件.onload的一个作用就是在首次载入一个页面文件时检测cookie的值,并用一个变量为其赋值,使它可以被源代码使用,本事件是window的事件,但是在HTML中指定事件处理程序时,一般把它写在<body>标记中.
      如:<body onload=alert("正在加载页面,请等待...")>
**7.卸载文件事件____onunload**
      与载入文件事件onload正好相反,当web页面退出时引发的事件,并可更新cookie的状态
      如:<body onunload="confirm("你确定要离开本页?")>
**8.鼠标覆盖事件 _____onmouseover**
      onmouseover是当鼠标位于元素上方时所引发的事件
      如:<input type="boutton" value="按钮" onmouseover="window.status='请您注意下面的状态栏'; return true">
**9.鼠标离开事件_____onmouseout**
      onmouseout是鼠标离开元素里引发的事件.如果和鼠标覆盖事件同时使用,可以创建动态按钮的效果.
**10.一般事件**
      ondbclick          鼠标双击事件
      鼠标上的按键被按下时激活的事件
      鼠标按下后,松开鼠标时触发的事件
      鼠标移动里触发的事件.
      onkeypress       当键盘上的某个键被按下并且释放时触发的事件,要求页面内必须有激活对象
      onkeydown       当键盘上某个键被按下时触发的事件,要求页面内必须有激活对象
      onkeyup            当键盘上某个按键被放开时触发的事件,要求页面内必须有激活对象
**11.页面相关事件**
      onabort   图片在下载时被用户中断
      onbeforeunload   当前页面的内容将要被改变时触发的事件
      onerror        捕捉当前页面因为某种原因而出现的错误,如脚本错误.
      onmove       浏览器的窗口被移动里触发的事件
      onresize     当浏览器的大小被改变时触发的事件
      onscroll         浏览器的滚动条位置发生变化时触发的事件
      onstop          浏览器的"停止"按钮被按下或者正在下载的文件被中断时触发的事件.
**12. 表单相关事件**
      onreset      当表单中reset属性被激活时触发的事件.
      onsubmit  一个表单被递交时触发的事件.
**13.滚动字幕事件**
      onbounce    当marquee内的内容移动至marquee显示范围之外时触发的事件.
      onfinish      当marquee元素完成需要显示的内容后触发的事件.
      onstart        当marquee元素开始显示内容时触发的事件.
**14.编辑事件.**
      onbeforecopy    当页面当前的被选择内容将要复制到浏览者系统的剪贴板前触发的事件.
      onbeforeupdate  当浏览者粘贴系统剪贴板中的内容时通知目标对象
      oncontextmenu   当按下鼠标右键出现菜单或通过键盘的按键触发页面菜单时触发的事件.
                                <body oncontentmenu="return false">禁止使用鼠标右键
      oncopy              当页面当前被选择内容被复制后触发的事件
      oncut                  当页面当前被选择内容被剪切时触发的事件
      onpaste               当内容被粘贴时触发的事件
      ondrag                当某个对象被拖动时触发的事件
      ondragend          当鼠标拖动结束时触发的事件,即鼠标的按键被释放时触发的事件.
**15.数据绑定**
      onafterupdate    当数据完成由数据源到对象的传送时触发的事件
      oncellchange      当数据来源发生变化时触发的事件
      ondataavailable  当数据接收完成时触发的事件
      ondatasetchanged   数据在数据源发生变化时触发的事件.
      ondatasetcomplete   当数据源的全部有效数据读取完毕时触发的事件.
      onerrorupdate          当使用onbeforeupdate事件触发取消了数据传送时,代替onafterupdate事件.
      onrowenter               当前数据源的数据发生变化并且有新的有效数据时触发的事件.
      onrowexit                 当前数据源的数据将要发生变化时触发的事件
      onrowsdelete            当前数据记录将被删除时触发的事件
      onrowsinserted         当前数据源将要插入新数据记录时触发的事件
**16.外部事件**
      onafterprint      当文档被打印后触发的事件
      onbeforeprint   当文档即将打印时触发的事件
      onhelp              当浏览者按下F1键或者单击浏览器中的"帮助"按钮时触发的事件
 ②、其他JS代码触发：比如一个function调用另一个function的情况。
