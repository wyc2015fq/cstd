# How To Develop Login Form In Struts - weixin_33985507的博客 - CSDN博客
2008年07月11日 15:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
Apache Struts  is an open-source framework for developing J2EE web applications. It uses and extends the Java Servlet API to encourage developers to adopt an MVC architecture. It was originally created by Craig McClanahan and donated to the Apache Foundation in May, 2000. 
This article  will explain how to develop login form in struts. Struts adopts an MVC  architecture. 
**Model Part of Login form Example:**
Model is basically data of you application. A collection of Java beans and other helper classes that you use for  your application
In Login Form Example ,the following classes will be used--  
- Action class  
- ActionForm class  
**Action Class:**The action class  is the link between the Struts framework and your business application logic. The purpose of Action Class is to translate the *HttpServletRequest* to the business logic.
In this example our Action class is LoginAction.java.  this Action class only forwards the login.jsp. Our Action class returns the *ActionForward*  called "loginAction", which is defined in the struts-config.xml file . Here is code of our Action Class: 
**LoginAction.java**
**package** roseindia.web.struts.action;
/**
* @Author Sushil Pal
* @Web http://www.roseindia.net
* @Email [pal_sushil@hotmail.com](mailto:pal_sushil@hotmail.com;deepak@roseindia.net)
* @**Copyright RoseIndia.net**
*/ 
import javax.servlet.http.HttpServletRequest;
**import** javax.servlet.http.HttpServletResponse;
**import** org.apache.struts.action.Action;
**import** org.apache.struts.action.ActionForm;
**import** org.apache.struts.action.ActionForward;
**import** org.apache.struts.action.ActionMapping;
**public****class** LoginAction **extends** Action
{
**public** ActionForward execute(
ActionMapping mapping,
ActionForm form,
HttpServletRequest request,
HttpServletResponse response) throws Exception{
LoginForm loginform = (LoginForm) form;
if (loginform.getUsername().equalsIgnoreCase("username") &&                        loginform.getPassword().equals("password")) {
// we are in
     return mapping.findForward("success");
} else {
// not allowed
     return mapping.findForward("failure");
}
}
} 
Action Class process the specified HTTP request, and create the corresponding HTTP response and Return an `ActionForward` instance describing where and how control should be forwarded. In our login action class if username field value will be 'username' and password will be 'password' then the mapping will forward to success.jsp otherwise to failure.jsp. 
**ActionForm Class:**
An ActionForm is a JavaBean that extends `org.apache.struts.action.ActionForm`. ActionForm maintains the session state for web application and the ActionForm object is automatically populated on the server side with data entered from a form on the client side. It must have setters and getters for all the form fields.The `ActionForm` class is pre-populated with form data when the form is submitted.
In this example our ActionForm class is LoginForm.java. Here is code of our ActionForm Class: 
**LoginForm.java **
package roseindia.web.struts.action;
/**
* @Author Sushil Pal
* @Web http://www.roseindia.net
* @Email [pal_sushil@hotmail.com](mailto:pal_sushil@hotmail.com;deepak@roseindia.net)
* @**Copyright RoseIndia.net**
*/
```
/**
 * Form bean for the Login Entry Screen.
 *
*/
```
import javax.servlet.http.HttpServletRequest;
import org.apache.struts.action.*;
public class LoginForm extends ActionForm
{
    private String action="add";
    private String username=null; 
    private String password=null;
   private String usertype=null;
public void setAction(String action){
    this.action=action;
}
public String getAction(){
   return this.action;
} 
public void setUsername(String username){
    this.username=username;
}
public String getUsername(){
    return this.username;
}
public void setPassword(String password){
   this.password=password;
}
public String getPassword(){
   return this.password;
}
public void setUsertype(String usertype){
   this.usertype=usertype;
}
public String getUsertype(){
   return this.usertype;
}
public void reset(ActionMapping mapping,HttpServletRequest request){
   this.username=null;
   this.password=null;
   this.usertype=null;
   this.action="add";
}
}  
**View Part of Login form Example:**The View portion of a web application is most often constructed using JavaServer Pages (JSP) technology. JSP pages can contain static HTML (or XML),plus the ability to insert dynamic content based on the interpretation of special action tags.
In this example our JSP pages are login.jsp, success.jsp, failure.jsp. Here is code of our JSP pages:
**login.jsp **
<%@ taglib uri="/tags/struts-bean" prefix="bean" %>
<%@ taglib uri="/tags/struts-html" prefix="html" %>
<html:html locale="true">
<head>
<title><bean:message key="welcome.title"/></title>
<html:base/>
<script language="javascript">
function validate(objForm){
if(objForm.username.value.length==0){
alert("Please enter UserID!");
objForm.username.focus();
return false;
}
if(objForm.password.value.length==0){
alert("Please enter Password!");
objForm.password.focus();
return false;
}
if(objForm.usertype.selectedIndex == 0 ){
alert("Please Select User Type!");
objForm.usertype.focus();
return false;
}
return true;
}
</script>
</head>
<body bgcolor="white">
<html:form action="/login" method="post" onsubmit="return validate(this);">
<center>
<table width="400" border="1" align="center" cellpadding="0" cellspacing="0">
<tr>
<td>
<table border="0" cellspacing="2" cellpadding="1" width="100%" >
<tr bgcolor="#eaeac8">
<td align="left" colspan="2"><font size="5">User Login</font></td>
</tr> 
<tr><td colspan="2"><p> </p></td></tr>
<tr align="center">
<td align="right">User ID:</td> 
<td><html:text property="username" size="30" maxlength="30"/></td>
</tr> 
<tr align="center">
<td align="right">Password:</td> 
<td><html:password property="password" size="30" maxlength="30"/></td>
</tr> 
<tr align="center">
<td align="right">Login As:</td> 
<td> 
<html:select property="usertype">
<option>---------------Login As---------------</option>
<option value="Buyer">Buyer</option>
<option value="Seller">Seller</option>
<option value="Guest">Guest</option>
</html:select>
</td>
</tr> 
<tr><td colspan="2"><p> </p></td></tr>
<tr>
<td align="center" colspan="2"><html:submit>Login Now !</html:submit></td>
</tr> 
</table>
</td>
</tr>
</table>
</center>
</html:form>
<body>
</html:html> 
**success.jsp **
<%@page import="java.io.*"%>
<%@page import="java.sql.*"%>
<%@page import="javax.sql.*"%>
<%@page import="java.util.*"%>
<%@page import="javax.naming.*"%>
<%@page import="javax.servlet.*"%>
<%
//getting value
String username=request.getParameter("username");
String password=request.getParameter("password");
String usertype=request.getParameter("usertype");
%>
<html>
<head>
<title>Success</title>
</head>
<body>
<p> Hi , <%=username%><p>
<p align="center"><font size="3" color="#000080">You Are Successfully Loged in as <font color="red"><%=usertype%></font></font></p>
</body>
</html> 
**failure.jsp**
<%@ taglib uri="/tags/struts-bean" prefix="bean" %>
<%@ taglib uri="/tags/struts-html" prefix="html" %>
<html:html locale="true">
<head>
<title><bean:message key="welcome.title"/></title>
<html:base/>
</head>
<body bgcolor="white">
<p align="center"><font size="3" color="red">Sorry your UserId/Password is incurrect</font></p>
<center><p><html:link page="/pages/login.jsp"><font size="3" color="blue">Retry click here!</font></html:link></p></center>
</body>
</html:html> 
**Controller Part of Login form Example:**The Controller part is focused on receiving requests from a  web browser, deciding what business logic function is to be performed, and then delegating responsibility for producing the next phase of the user interface to an appropriate View component. The main component of the Controller in the framework is a servlet of class `ActionServlet.` This servlet is configured by defining a set of 
```
ActionMappings.Defining the mapping for our example. Put the following code in **struts-config.xml**.
Put the following codes within the <form-beans> --------- </form-beans> section.
```
<form-bean
            name="LoginForm"
            type="roseindia.web.struts.action.LoginForm"/>
</form-beans> 
`Put the following codes within the <action-mappings> --------- </action-mappings> section.`
<action
        path="/login"
        type="roseindia.web.struts.action.LoginAction"
        name="LoginForm"
        scope="request"
        validate="true"
        input="/pages/login.jsp">
        <forward name="success" path="/pages/success.jsp"/>
        <forward name="failure" path="/pages/failure.jsp"/>
</action> 
**How to run this Example:Installing Struts Application:**
Download latest version of Struts from the site of Struts [http://jakarta.apache.org/struts](http://jakarta.apache.org/struts). Copy struts-blank.war to tomcat webapps directory. Start tomcat, Tomcat automatically extracts the file and loads the application. Download the code from [here](http://www.roseindia.net/struts/strutsloginapp.zip) and then integrate it with the struts-blank application. Make necessary changes in the struts-config.xml, and compile application using ant. The restart the tomcat. Now your login application is ready. Now open Internet Explorer and type http://localhost:8080/struts-blank/ and press Enter. You browser window should look like:  
![how_to2.gif](http://www.roseindia.net/struts/how_to2.gif)
Now click on the link  Login Page Example the following window will open...
![how_to1.gif](http://www.roseindia.net/struts/how_to1.gif)
Now type User Id as 'username' and password as 'password' and select a login option then press login button. The success page will display.
Hi , username 
                                     You Are Successfully Loged in as Buyer 
 If you enter incorrect user id and password the failure page will display. 
                                     Sorry your UserId/Password is incorrect 
Retry click here!
In this tutorial you have learned how to develop simple login application with struts.
