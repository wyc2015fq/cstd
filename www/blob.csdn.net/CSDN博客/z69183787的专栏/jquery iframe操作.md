# jquery iframe操作 - z69183787的专栏 - CSDN博客
2012年10月26日 09:52:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：896
个人分类：[Javascript-Jquery](https://blog.csdn.net/z69183787/article/category/2175111)
使用jquery操作iframe
1、 内容里有两个ifame
<iframe id="leftiframe"...</iframe> 
<iframe id="mainiframe..</iframe>
leftiframe中jQuery改变mainiframe的src代码： 
$("#mainframe",parent.document.body).attr("src","[http://www.radys.cn](http://www.radys.cn/)")
2、 如果内容里面有一个ID为mainiframe的ifame 
  <iframe id="mainifame"...></ifame> 
ifame包含一个someID 
<div id="someID">you want to get this content</div> 
得到someID的内容
  $("#mainiframe").contents().find("someID").html() html 或者 $("#mainiframe").contains().find("someID").text()值
3、在父窗口中操作 选中IFRAME中的所有单选钮
$(window.frames["iframe1"].document).find("input[@type='radio']").attr("checked","true");
那选择id自然就是依然使用find方法
$(window.frames["iframe1"].document).find("#id")
4、 如上面所示 
   leftiframe中的jQuery操作mainiframe的内容someID的内容 
   $("#mainframe",parent.document.body).contents().find("someID").html()或者 $("#mainframe",parent.document.body).contents().find("someID").val()
使用JavaScript操纵iframe
框架间的互相引用
一个页面中的所有框架以集合的形式作为window对象的属性提供，例如：window.frames就表示该页面内所有框架的集合，这和表单对象、链接对象、图片对象等是类似的，不同的是，这些集合是document的属性。因此，要引用一个子框架，可以使用如下语法：
window.frames["frameName"];
window.frames.frameName
window.frames[index]
其中，window字样也可以用self代替或省略，假设frameName为页面中第一个框架，则以下的写法是等价的：
self.frames["frameName"]
self.frames[0]
frames[0]
frameName
每个框架都对应一个HTML页面，所以这个框架也是一个独立的浏览器窗口，它具有窗口的所有性质，所谓对框架的引用也就是对window对象的引用。有了这个window对象，就可以很方便地对其中的页面进行操作，例如使用window.document对象向页面写入数据、使用window.location属性来改变框架内的页面等。
下面分别介绍不同层次框架间的互相引用：
1．父框架到子框架的引用
知道了上述原理，从父框架引用子框架变的非常容易，即：
window.frames["frameName"];
这样就引用了页面内名为frameName的子框架。如果要引用子框架内的子框架，根据引用的框架实际就是window对象的性质，可以这样实现：
window.frames["frameName"].frames["frameName2"];
这样就引用到了二级子框架，以此类推，可以实现多层框架的引用。
2．子框架到父框架的引用
每个window对象都有一个parent属性，表示它的父框架。如果该框架已经是顶层框架，则window.parent还表示该框架本身。
3．兄弟框架间的引用
如果两个框架同为一个框架的子框架，它们称为兄弟框架，可以通过父框架来实现互相引用，例如一个页面包括2个子框架：
<frameset rows="50%,50%">
<frame src="1.html" name="frame1" />
<frame src="2.html" name="frame2" />
</frameset>
在frame1中可以使用如下语句来引用frame2：
self.parent.frames["frame2"];
4．不同层次框架间的互相引用
框架的层次是针对顶层框架而言的。当层次不同时，只要知道自己所在的层次以及另一个框架所在的层次和名字，利用框架引用的window对象性质，可以很容易地实现互相访问，例如：
self.parent.frames["childName"].frames["targetFrameName"];
5．对顶层框架的引用
和parent属性类似，window对象还有一个top属性。它表示对顶层框架的引用，这可以用来判断一个框架自身是否为顶层框架，例如：
//判断本框架是否为顶层框架
if(self==top){
//dosomething
}
