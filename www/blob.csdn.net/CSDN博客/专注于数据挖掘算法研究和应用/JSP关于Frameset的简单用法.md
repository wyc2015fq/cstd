# JSP关于Frameset的简单用法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年04月13日 15:36:19[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6564








1、main.jsp设定页面框架为上下（左右）格局，代码如下：



```java
<frameset rows="100,*" cols="*" frameborder="yes" border="1" framespacing="1">  
	<frame  src="top.jsp"  name="topFrame" scrolling="No" noresize="noresize" 
	        id="topFrame" title="topFrame" />  
	<frameset cols="160,*" frameborder="No" border="0" framespacing="1">  
    	<frame src="left.html" name="leftFrame" scrolling="No" noresize="noresize" 
    	    id="leftFrame" title="leftFrame" />  
        <frame src="logView.jsp"  name="mainFrame" noresize="noresize" 
            id="mainFrame" title="mainFrame" />  
    </frameset>  
</frameset>
```


2.左边left.html为菜单栏，菜单链接的路径属性描述为：





```java
<a href="about.jsp" target="mainFrame">关于系统</a>
```



指定about.jsp的target是mainFrame显示在左边框架内，如果要跳出框架可设置target为_top或_parent值。



