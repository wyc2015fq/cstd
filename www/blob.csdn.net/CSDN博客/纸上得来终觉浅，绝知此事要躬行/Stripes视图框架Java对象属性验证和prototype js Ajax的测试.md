# Stripes视图框架Java对象属性验证和prototype.js Ajax的测试 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月06日 00:14:06[boonya](https://me.csdn.net/boonya)阅读数：2542








 Stripes视图框架Java对象属性验证，它允许对字段设置是否必须填写，对数字大小进行限制等。我用prototype.js Ajax 将验证后的数据及时地展示出来，下面来看程序。

**1、编写User实体类**

此用户共三个属性： name、email、age.



```java
package com.boonya.stripes.entity;

public class User {
	
	private String name;
	
	private String email;
	
	private int age;

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public int getAge() {
		return age;
	}

	public void setAge(int age) {
		this.age = age;
	}
	
	

}
```



**2、编写UserActionBean视图控制Action类**


 此类中包含了用户输入界面(userRegister.jsp)和用户数据提交完成后的数据展示界面(userInfo.jsp)，我们要验证的是用户的数据合法的时候才展示用户数据。



```java
package com.boonya.stripes.actions;

import com.boonya.stripes.entity.User;
import net.sourceforge.stripes.action.DefaultHandler;
import net.sourceforge.stripes.action.ForwardResolution;
import net.sourceforge.stripes.action.Resolution;
import net.sourceforge.stripes.action.SessionScope;
import net.sourceforge.stripes.validation.Validate;
import net.sourceforge.stripes.validation.ValidateNestedProperties;
@SessionScope
public class UserActionBean extends AbstractActionBean {

	private static final long serialVersionUID = 1L;
	
	protected static final String REGISTER="/WEB-INF/userRegister.jsp";
	
	protected static final String USER_INFO="/WEB-INF/userInfo.jsp";
	
	 @ValidateNestedProperties({
		    @Validate(field ="name",required=true,on = {"viewUserInfo"}),
		    @Validate(field ="email",required=true,on = {"viewUserInfo"}),
		    @Validate(field ="age", required = true, minvalue = 13, on = {"viewUserInfo"}) 
		  })
	private User user;

	@DefaultHandler
	public Resolution register() {
		return new ForwardResolution(REGISTER);
	}
	
	public Resolution viewUserInfo(){
		  return new ForwardResolution(USER_INFO);  
	}

	public void setUser(User user) {
		this.user = user;
	}

	public User getUser() {
		return user;
	}

}
```
 注：@ValidateNestedProperties  验证对象包含的特殊属性字段值的限制；@Validate  具体验证的字段。filed表示字段、required表示是否必需、on表示绑定到的事件方法。


**3、编写界面操作的Jsp文件**

**userRegister.jsp**



```
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<%@ taglib prefix="stripes"	uri="http://stripes.sourceforge.net/stripes.tld"%>
<html>
<head>
<title>User-Register</title>
<script src="../js/prototype.js" type="text/javascript"></script>

<script type="text/javascript">
function userHandleAjax() {
    var myAjax = new Ajax.Updater('user',

	'<stripes:url beanclass="com.boonya.stripes.actions.UserActionBean" event="viewUserInfo"/>',

	{
	   method: 'get',
	   parameters:Form.serialize('userForm')
	});
}
</script>
</head>
<body>
	<stripes:errors globalErrorsOnly="false"/> 
	<stripes:form beanclass="com.boonya.stripes.actions.UserActionBean" id="userForm">
Update user info: <br>

Name: <stripes:text name="user.name" />
		<br>
Email: <stripes:text name="user.email" />
		<br>
Age:<stripes:text name="user.age" />
		<br>
<stripes:button name="PrototypeAjax" value="Go to User View" onclick="userHandleAjax()" />

<div id="user"></div>
	</stripes:form>
</body>
</html>
```



**userInfo.jsp**



```
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="stripes"
	uri="http://stripes.sourceforge.net/stripes.tld"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<title>User-Info</title>
<body>
  <h4>User's info</h4><br/>
	Hello <font color="red">${actionBean.user.name}</font>
	<br />
	Email: <font color="blue">${actionBean.user.email}</font>
	<br />
	age: <font color="blue">${actionBean.user.age}</font>
	<br>
</body>
</html>
```
注：表单字段赋值：对象.属性 即user.name等等；而取值以$(actionBean.对象.属性)，即${actionBean.user.name}等等;


**4、测试表单提交数据异常处理**

故意少输入参数，页面显示请求不正常页面：

![](https://img-blog.csdn.net/20131106000457453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


产生的异常代码提示：



```java
Caused by: java.util.MissingResourceException: Could not find an error message with any of the following keys: 'com.boonya.stripes.actions.UserActionBean.user.age.valueNotPresent', 'com.boonya.stripes.actions.UserActionBean.user.age.errorMessage', '/actions/User.action.user.age.valueNotPresent', '/actions/User.action.user.age.errorMessage', 'user.age.valueNotPresent', 'user.age.errorMessage', 'com.boonya.stripes.actions.UserActionBean.valueNotPresent'.com.boonya.stripes.actions.UserActionBean.errorMessage', '/actions/User.action.valueNotPresent', '/actions/User.action.errorMessage', 'validation.required.valueNotPresent'.
	at net.sourceforge.stripes.validation.ScopedLocalizableError.getMessageTemplate(ScopedLocalizableError.java:164)
	at net.sourceforge.stripes.action.SimpleMessage.getMessage(SimpleMessage.java:91)
	at net.sourceforge.stripes.validation.SimpleError.getMessage(SimpleError.java:102)
	at net.sourceforge.stripes.tag.ErrorsTag.doEndTag(ErrorsTag.java:349)
	at org.apache.jsp.WEB_002dINF.userRegister_jsp._jspService(userRegister_jsp.java:109)
	at org.apache.jasper.runtime.HttpJspBase.service(HttpJspBase.java:70)
	at javax.servlet.http.HttpServlet.service(HttpServlet.java:722)
	at org.apache.jasper.servlet.JspServletWrapper.service(JspServletWrapper.java:432)
	... 46 more
```

当Stripes验证失败时，不知道怎样返回这些错误信息，解决方案也很简单：从 Stripes 包中复制一份 StripesResources.properties 到项目的源代码跟目录下（与WEB/classess同级）即可。加入StripesResources.properties文件后输入：name=boonya,email=boonyachengdu@gmail.com,age=12.;测试效果如下图所示：

![](https://img-blog.csdn.net/20131106002307984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


此时，年龄验证错误；但是Stripes的错误消息已经知道该怎么提示错误信息给用户了。

**5、测试正确的验证和Ajax效果**

在表单输入框中输入符合验证条件的数据如：name=boonya,email=boonyachengdu@gmail.com,age=23.;效果如下图所示：

![](https://img-blog.csdn.net/20131106001139718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


至此，整个示例程序测试完毕。



