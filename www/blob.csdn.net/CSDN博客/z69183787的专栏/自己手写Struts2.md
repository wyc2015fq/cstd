# 自己手写Struts2 - z69183787的专栏 - CSDN博客
2015年05月21日 22:13:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1105
记得以前教Struts的时候，自己动手写过一个类似Struts的MVC框架。在培训的过程中，我发现这种过渡非常平滑：学习MVC设计模式时，自己动手写一个Struts框架，再使用自己写的小框架完成一个项目。这样既让学员深刻地理解了MVC的重要思想，又为后面学习Struts框架做好了铺垫，以至于后面真正培训Struts只需要短短5天时间。所以要想灵活掌握一个框架，最好的方式还是自己模仿着去实现它，然后不停地发现问题，解决问题，参考原框架的源代码，从而深刻体会框架设计的精妙之处。
现在Struts已经从1.x升级为2.x,它的架构体系也发生了很大变化。实际上稍有了解的人都知道，Struts2就是Struts和WebWork的强强联姻。至于这些框架的恩恩怨怨，大家可以参考我前面Struts2系列的文章，这里我就不赘述了。我们今天的主题是：模仿Struts2，手写自己的MVC框架。
       手写自己的框架？提到这里，有人犯嘀咕了。这有点难度也。我们知道，大凡比较著名的框架，都有着精巧的设计：OOP的设计思想，随处可见的设计模式，JAVA反射机制的灵活应用，XML配置文件的解析,元数据注解甚至IOC和AOP的强大渗透。这里面没一个是好惹的主！但大家要注意，我们模仿着手写框架的目的，只是为了更好的学习和理解原框架，并不是重复发明轮子，所以我们只需要关注原框架的核心流程和重点实现，并不需要面面俱到。先来看一下Struts2的架构流程图，感受一下它的魅力。
      呵呵，眼看百遍还不如手写一遍。好啦，洪哥，我们动手吧！
       首先我们看一下我们小框架（暂时取名为mvc1）的核心配置文件mvc1.xml：
<?xml version="1.0" encoding="UTF-8"?>
<mvc1>
<package>
<action name="login" method="logon" 
class="org.leno.mvc1.app.action.LoginAction">
<result name="success">/index.jsp</result>
<result name="login">/login.jsp</result>
</action>
</package>
</mvc1>
    将它放在/src底下，是不是很熟悉？这就是struts.xml的简化版。呵呵，我们这里先做一个初级版本，弄清楚核心原理后再慢慢升级。
       框架代码的核心部分是一个Filter，因为Struts2所有工作都是通过一个Filter来完成的(struts1.*是通过一个Servlet实现的)。我们先来看代码：
**package** org.leno.mvc1.core;
**import** java.io.IOException;
**import** java.lang.reflect.Method;
**import** java.util.HashMap;
**import** java.util.Map;
**import** javax.servlet.Filter;
**import** javax.servlet.FilterChain;
**import** javax.servlet.FilterConfig;
**import** javax.servlet.ServletException;
**import** javax.servlet.ServletRequest;
**import** javax.servlet.ServletResponse;
**import** javax.servlet.http.HttpServletRequest;
**import** javax.servlet.http.HttpServletResponse;
**publicclass** FilterDispatcher **implements** Filter {
**private** Map<String, ActionConfig> **actionMap** = **new** HashMap<String, ActionConfig>();
**publicvoid init**(FilterConfigfilterConfig) **throws** ServletException{
// 得到框架核心配置文件mvc1.xml在web服务器上的物理路径，然后解析XML
       StringwebPath = getClass().getClassLoader().getResource("mvc1.xml")
              .getPath();
       ConfigUtil.*parseConfigFile*(webPath, actionMap);
    }
@SuppressWarnings("static-access")
**publicvoid doFilter**(ServletRequest req,ServletResponse res,
           FilterChainchain) **throws** IOException, ServletException {
// 针对HTTP协议,将请求和响应对象还原为HTTP类型
       HttpServletRequestrequest = (HttpServletRequest) req;
       HttpServletResponseresponse = (HttpServletResponse) res;
// 设置请求和响应的编码方式，避免乱码
       request.setCharacterEncoding("utf-8");
       response.setCharacterEncoding("utf-8");
// 得到request的请求路径
       Stringuri = request.getServletPath();
// 如果后缀不为.action,不需要该核心Filter控制，直接过关
**if** (!uri.endsWith(".action")) {
           chain.doFilter(request,response);
**return**;
       }
// 解析Request的路径
**int** start = uri.indexOf("/");
**int** end = uri.lastIndexOf(".");
       Stringpath = uri.substring(start + 1, end);
//由path找到相对应的ActionConfig类，它里面包含了所有Action的相关信息
       ActionConfigconfig = actionMap.get(path);
// 匹配不成功就返回找不到页面错误信息
**if** (config == **null**) {
           response.setStatus(response.*SC_NOT_FOUND*);
**return**;
       }
//由ActionConfig得到Action类的完整名字
       StringclzActionName = config.getClzName();
// 实例化Action对象, 不存在给出错误提示
       Objectaction = getAction(clzActionName);
**if** (action == **null**) {
           response.setStatus(response.*SC_NOT_FOUND*);
**return**;
       }
// 前置拦截,取得request里面的参数，调用Action的set方法给属性设置值
       BeanUtil.*requestToAction*(request,action);
// 执行Action的无参方法，例如execute()
       Stringresult = executeAction(config, action);
// 后置拦截,调用Action的get方法,将所有Action的属性放置到request区域
       BeanUtil.*actionToRequest*(request,action);
// 返回相应的JSP页面
**if** (result == **null**) {
           response.setStatus(response.*SC_NOT_FOUND*);
**return**;
       }
       request.getRequestDispatcher(result).forward(request,response);
    }
**private** String executeAction(ActionConfigconfig, Object action) {
       Stringmethod = config.getMethod();
       Stringresult = **null**;
**try** {
           MethodcallMethod = action.getClass().getMethod(method,
**new** Class[] {});
           result= (String) callMethod.invoke(action, **new** Object[] {});
       } **catch** (Exception e) {
// **TODO** Auto-generated catch block
           e.printStackTrace();
       }
**return** config.getResultMap().get(result);
    }
**private** Object getAction(String clzActionName){
       Objectaction = **null**;
**try** {
           action= Class.*forName*(clzActionName).newInstance();
       } **catch** (Exception e) {
// **TODO** Auto-generated catch block
           e.printStackTrace();
       }
**return** action;
    }
**publicvoid** destroy() {
// **TODO** Auto-generated method stub
    }
}
里面都做好了注释，好好把这个类品一下。FilterDispatcher过滤器里有两个重要的方法，一个init(),一个doFilter().我们分别来看看吧！
init()方法就是做初始化的工作，我们在里面主要是解析配置文件mvc1.xml，然后将配置信息存放到actionMap属性里。我这里使用dom4j解析XML配置文件，里面涉及到一个很简单的帮助类，贴出来供大家参考：
**package** org.leno.mvc1.core;
**import** java.io.File;
**import** java.util.Iterator;
**import** java.util.List;
**import** java.util.Map;
**import**org.dom4j.Document;
**import** org.dom4j.DocumentException;
**import** org.dom4j.Element;
**import** org.dom4j.io.SAXReader;
**publicclass** ConfigUtil {
@SuppressWarnings("unchecked")
**publicstaticvoid** parseConfigFile(String fileName,Map<String, ActionConfig> map) {
       SAXReaderreader = **new** SAXReader() ;
**try** {
Document doc = reader.read(**new** File(fileName));
           Elementroot = doc.getRootElement();
           List<Element>list = root.selectNodes("package/action");
           System.*out*.println(list.size());
**for**(Element actionNode : list)
           {
//封装成ActionConfig对象，保存到map中。
              ActionConfigconfig = **new** ActionConfig();
//将action节点的path, type, name属性值获取到。
              Stringname = actionNode.attributeValue("name");
              StringclzName = actionNode.attributeValue("class");
              Stringmethod = actionNode.attributeValue("method");
              config.setName(name);
              config.setClzName(clzName);
**if**(method == **null** || "".equals(method)){
                  method="execute";
              }
              config.setMethod(method);
//遍历action节点下的forward子节点
              IteratoritrNodes = actionNode.selectNodes("result").iterator();
**while**(itrNodes.hasNext()){
                  ElementforwardNode = (Element)itrNodes.next();
                  StringforwardName = forwardNode.attributeValue("name");
                  StringforwardPath = forwardNode.getTextTrim(); 
**if**(forwardName == **null** || "".equals(forwardName)){
                     forwardName="success";
                  }
                  config.getResultMap().put(forwardName,forwardPath);
              }  
              map.put(name,config);
           }
       } **catch** (DocumentException e) {
// **TODO** Auto-generated catch block
           e.printStackTrace();
       }
    }
}
       根据XML的配置,我们将action的节点抽象成一个类，它具有name,clzName,method等属性，是一个普通的Javabean，代码如下：
**package** org.leno.mvc1.core;
**import** java.util.HashMap;
**import** java.util.Map;
**publicclass** ActionConfig {
**private** String name;
**private** String clzName;
**private** String method;
**private** Map<String, String> resultMap = **new** HashMap<String, String>();
**public** ActionConfig() {
**super**();
    }
**public** ActionConfig(String name, StringclzName, String method,
           Map<String,String> resultMap) {
**super**();
**this**.name = name;
**this**.clzName = clzName;
**this**.method = method;
**this**.resultMap = resultMap;
    }
**public** String getName() {
**return**name;
    }
**publicvoid** setName(String name) {
**this**.name = name;
    }
**public** String getClzName() {
**return**clzName;
    }
**publicvoid** setClzName(String clzName) {
**this**.clzName = clzName;
    }
**public** String getMethod() {
**return**method;
    }
**publicvoid** setMethod(String method) {
**this**.method = method;
    }
**public** Map<String, String>getResultMap() {
**return**resultMap;
    }
**publicvoid** setResultMap(Map<String, String>resultMap) {
**this**.resultMap = resultMap;
    }
}
doFilter方法是整个过滤器的核心，它的大概流程是：
1. 针对HTTP协议,将请求和响应对象还原为HTTP类型，譬如ServletRequest要转化为HttpServletRequest。并设置请求和响应的编码方式为UTF-8，避免中文乱码。
2. 解析客户端请求的URL路径,从而由**actionMap**得到相应的在mvc1.xml配置文件中所配置的Action完整类名以及执行方法名。如果不是合法的.action后缀，直接chain.doFilter(request, response)放行。
3. 将request里面的参数取出，在调用Action执行方法之前，给具有set器的Action的属性赋值。这里利用了JAVA的反射机制。
4. 继续利用JAVA的反射机制，动态执行Action的方法。
5. 然后将具有get器的Action的属性存放在request区域中，供页面使用。大家记得在每次执行Action的方法时都会返回一个String字符串，比如”success”,”login”,”input”等，我们可以将这个值与mvc1.xml的<result name="**success**"....>/index.jsp</result>　中的name属性值进行匹配，取出需要返回的页面资源/index.jsp
6.最后，调用request.getRequestDispatcher().forward()方法一次跳转到相应的页面上去。
       这里大家要对JAVA的反射机制比较熟悉，里面涉及到的比较晦涩的代码都跟反射有关，BeanUtil.*requestToAction*(request,action);
BeanUtil.*actionToRequest*(request,action);
前者将request里面的参数设置到action属性中去，后者将action属性的值保存在request区域中。我们来看看BeanUtil类的代码：
**package** org.leno.mvc1.core;
**import** java.lang.reflect.Field;
**import** java.lang.reflect.Method;
**import** java.util.Enumeration;
**import** javax.servlet.http.HttpServletRequest;
**publicclass**BeanUtil {
@SuppressWarnings("unchecked")
**publicstaticvoid** requestToAction(HttpServletRequestrequest, Object action) {
       ClassclzAction = action.getClass();
       Enumeration<String>names = request.getParameterNames();
**while** (names.hasMoreElements()){        
           Stringname=  names.nextElement();        //获得每一个参数名      eg:username
           String[]value = request.getParameterValues(name);   //获取这个参数对应的值 eg:leno
**if** (value != **null**) {
**try** {
// 看Action里是否有name这个属性，并获取在Action里的数据类型
                  ClassfieldType = clzAction.getDeclaredField(name).getType(); 
                  StringsetName = "set"
                         +name.substring(0, 1).toUpperCase()
                         +name.substring(1);
                  Methodmethod = clzAction.getMethod(setName,
**new** Class[] { fieldType });
                  Object[]o = *transfer*(fieldType, value);
//判断是否为数组属性
**if** (fieldType.isArray()) {
                     method.invoke(action, **new** Object[]{o});
                  } **else** {
                     method.invoke(action, **new** Object[]{o[0]} );
                  }
              } **catch** (NoSuchFieldException e) {
//e.printStackTrace();不用处理,因为有些表单字段在Action中没有对应的属性。
              } **catch** (Exception e) {
                  e.printStackTrace();
              }
           }
       }
    }
**publicstaticvoid** actionToRequest(HttpServletRequestrequest, Object action) {
       Field[]fields = action.getClass().getDeclaredFields();
**for** (**int** i = 0; i < fields.length; i++) {
           StringfieldName = fields[i].getName();
           StringgetMethodName = "get"
              +fieldName.substring(0, 1).toUpperCase()
              +fieldName.substring(1);
**try** {
              MethodgetMethod = action.getClass().getMethod(getMethodName, **new**Class[]{});
              Objectvalue = getMethod.invoke(action, **new** Object[]{});
**if**(value!=**null**){
                  request.setAttribute(fieldName,value);
              }
           } **catch** (Exception e) {
// **TODO** Auto-generated catch block
              e.printStackTrace();
           }
    }
    }
@SuppressWarnings("unchecked")
**privatestatic** Object[] transfer(Class fieldType,String[] value){
       Object[]os = **null**;
       StringtypeString = fieldType.getSimpleName().replace("[]", "");
**if**("String".equalsIgnoreCase(typeString)){
           os= value;
       }
**elseif**("int".equals(typeString) || "Integer".equals(typeString)){
           os= **new** Integer[value.length];
**for**(**int** i = 0; i < os.length; i++){
              os[i] = Integer.*parseInt*(value[i]);
           }
       }
**elseif**("float".equals(typeString) || "Float".equals(typeString)){
           os= **new** Float[value.length];
**for**(**int** i = 0; i < os.length; i++){
              os[i]= Float.*parseFloat*(value[i]);
           }
       }
**elseif**("double".equals(typeString) || "Double".equals(typeString)){
           os= **new** Double[value.length];
**for**(**int** i = 0; i < os.length; i++){
              os[i]= Double.*parseDouble*(value[i]);
           }
       }
**return** os;
    }
}
    打完收工，框架编码工作暂时告一段落！
别忘记了在web.xml中配置这个Filter：
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="2.5"
xmlns="http://java.sun.com/xml/ns/javaee"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://java.sun.com/xml/ns/javaee
    http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
<filter>
<filter-name>coreFilter</filter-name>
<filter-class>org.leno.mvc1.core.FilterDispatcher</filter-class>
</filter>
<filter-mapping>
<filter-name>coreFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
<welcome-file-list>
<welcome-file>index.jsp</welcome-file>
</welcome-file-list>
</web-app>
最后我们做个登陆的demo来测试一下吧，就按照本文最开始的那个mvc1.xml配置编写LoginAction.java(是不是觉得这个Action比Struts1的要简单的多？)，这里我们不需要实现特殊的接口，也不用掺杂很多Servlet的API，是一个很容易测试的Action。
**package** org.leno.mvc1.app.action;
**publicclass** LoginAction {
**private** String username;
**private** String password;
**public**String getUsername(){
**return**username;
    }
**publicvoid** setUsername(String username) {
**this**.username = username;
    }
**public** String getPassword() {
**return**password;
    }
**publicvoid** setPassword(String password) {
**this**.password = password;
    }
**public** String logon() **throws** Exception {
       System.*out*.println("Login:username:"+username+"password:"+password);
**if**("leno".equals(username)){
**return**"success";
       }**else**{
**return**"login";
       }
    }
}
下面是我们测试用到的页面login.jsp:
<%@ page language="java" contentType="text/html;charset=UTF-8"
pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML4.01 Transitional//EN""http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html;
 charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
<form method="post" action="login.action" name="loginForm">
<table width="422" border="1" bgcolor="#0080c0" height="184">
<caption>
<h1>
                     用户登陆
</h1>
</caption>
<tbody>
<tr>
<td>
姓名：
</td>
<td>
<input type="text" name="username">
</td>
</tr>
<tr>
<td>
密码：
</td>
<td>
<input type="password" name="password">
</td>
</tr>
<tr align="center">
<td colspan="2">
<input type="submit" value="登陆" name="submit">
<input type="reset" value="重置" name="reset">
</td>
</tr>
</tbody>
</table>
</form>
</body>
</html>
这样我们自己的简易版Struts2就算完成了。怎么样，写框架也不是想象中那么困难吧。虽然我们这里完成的功能还非常简单，远远够不上一个真正框架的级别，但核心部分已经完成了。所以只要我们有一颗不断学习和创新的心，加上持续不断的努力，何愁写不出好的框架来呢？好啦，以后我们有时间再慢慢丰富我们的功能吧。今天就写到这里，大家有什么问题或者不明白的地方可以给我留言，祝愿大家学习愉快。哦，对了，别忘了导入dom4j.jar和jaxen.jar包啊。呵呵，常识常识！
