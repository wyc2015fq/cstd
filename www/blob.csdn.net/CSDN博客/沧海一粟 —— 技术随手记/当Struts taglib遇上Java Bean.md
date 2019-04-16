# 当Struts taglib遇上Java Bean - 沧海一粟 —— 技术随手记 - CSDN博客





2011年07月25日 21:24:04[慢游](https://me.csdn.net/eengel)阅读数：2758标签：[struts																[java																[bean																[getter																[string																[django](https://so.csdn.net/so/search/s.do?q=django&t=blog)
个人分类：[Web Application Framework](https://blog.csdn.net/eengel/article/category/784428)





习惯了强大的Django template系统，再回归到Struts Taglib，难免会觉得束手束脚。Struts taglib提供了4种tag：Bean，HTML，Logic和Nested，具体介绍见[1]。用法很简单，网上有大量的例子。这儿说说当用Struts Taglib引用Java Bean生成的数据结构时，碰到的一个有趣的问题。废话少说，上例子。



**TestBean.java**：一个Java Bean，内含若干变量，比如可用于存储数据库表的数据。

```java
public class TestBean {

	private String name;
	private boolean boo1; //boolean类型
	private boolean isBoo2; //boolean类型，请注意其命名
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public boolean isBoo1() {//由于boo1是boolean类型，其getter为“is”开头
		return boo1;
	}
	public void setBoo1(boolean boo1) {
		this.boo1 = boo1;
	}
	public boolean isBoo2() {{//同样是boolean类型 。但变量本身叫isBoo2，其getter不叫“isisBoo2”，而直接叫“isBoo2”。这就是问题所在。

		return isBoo2;
	}
	public void setBoo2(boolean isBoo2) {
		this.isBoo2 = isBoo2;
	}
}
```


**SimpleAction.java**：调用上面JavaBean的servlet。

```java
public class SimpleAction extends Action {
	public ActionForward execute(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		
		TestBean bea = new TestBean();
		bea.setName("I'm a bea.");
		bea.setBoo1(true);
		bea.setBoo2(true);
		
		request.setAttribute("bea", bea);
		return mapping.findForward("success");
	}
}
```


**simple.jsp**：用StrutsTaglib显示bean中的内容。

```
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="logic" uri="http://struts.apache.org/tags-logic"%>
<%@ taglib prefix="bean" uri="http://struts.apache.org/tags-bean"%>
<div>
	<p><bean:write name="bea" property="name"/></p>
	<p><logic:equal name="bea" property="boo1" value="true">My boo1 is true.</logic:equal></p>
	<p><logic:equal name="bea" property="isBoo2" value="true">My isBoo2 is true.</logic:equal></p>
</div>
```


当请求这个页面的时候，出错了。显示找不到属性“isBoo2”。若在TestBean中将起名字改为“boo2”，则可用Struts Taglib取其值。

不太懂Struts Taglib的内部机制， 猜测是由于“isBoo2”的getter名字不规范引起的。



**结论：**

当Struts taglib遇上Java Bean时，若要在bean中定义boolean类型的变量，请注意其命名，最好不要以“is”开头。



**参考文献：**

[1] Struts Taglib, [http://struts.apache.org/1.x/struts-taglib/index.html](http://struts.apache.org/1.x/struts-taglib/index.html)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=getter&t=blog)](https://so.csdn.net/so/search/s.do?q=bean&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




