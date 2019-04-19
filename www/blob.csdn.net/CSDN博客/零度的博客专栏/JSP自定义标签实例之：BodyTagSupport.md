# JSP自定义标签实例之：BodyTagSupport - 零度的博客专栏 - CSDN博客
2016年05月16日 14:47:16[零度anngle](https://me.csdn.net/zmx729618)阅读数：441
个人分类：[Servlet/JSP](https://blog.csdn.net/zmx729618/article/category/6227151)
当需要处理标签体时，标签处理类应继承BodyTagSupport: 
一、需求分析： 
数据展示是常有长字符串会影响页面显示效果，需要对其按一定和长度和形式进行省略。来控制显示效果。 
二、标签处理类： 
Java代码  ![收藏代码](http://ll-feng.iteye.com/images/icon_star.png)
- publicclass StringOmitTag extends BodyTagSupport{  
- privatestaticfinallong serialVersionUID = -1385010302157701800L;  
- /**
-      * 
-      */
- privateint maxLength;  //限定的最长字数
- private String suffix;  //后缀
- private String value;   //要处理的值
- 
- 
- publicint getMaxLength() {  
- return maxLength;  
-     }  
- 
- 
- publicvoid setMaxLength(int maxLength) {  
- this.maxLength = maxLength;  
-     }  
- 
- 
- public String getSuffix() {  
- return suffix;  
-     }  
- 
- 
- publicvoid setSuffix(String suffix) {  
- this.suffix = suffix;  
-     }  
- 
- 
- public String getValue() {  
- return value;  
-     }  
- 
- 
- publicvoid setValue(String value) {  
- this.value = value;  
-     }  
- 
- 
- @Override
- publicint doAfterBody() throws JspException {  
- if(null==value){  
-             value=bodyContent.getString();  
-         }  
- 
- return SKIP_BODY;  
-     }  
- 
- 
- @Override
- publicint doEndTag() throws JspException {  
- if(null==value){  
-             value="";  
-         }  
- //JspWriter out = super.getBodyContent().getEnclosingWriter();
-         JspWriter out = pageContext.getOut();  
- try {  
- if(value.length()>maxLength){  
-                 value=value.substring(0, maxLength);  
- if(suffix!=null && !"".equals(suffix)){  
-                     value+=suffix;        
-                 }  
-             }  
-             out.print(value);  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
- return EVAL_PAGE;  
-     }  
- }  
三、标签描述文件： 
将下面的内容加入tld文件中。以增加web应用对上面的标签类的支持 
Java代码  ![收藏代码](http://ll-feng.iteye.com/images/icon_star.png)
- <tag>  
-     <name>StringOmit</name>  
-     <tag-class>cn.lvlf.framework.tags.StringOmitTag</tag-class>  
-     <body-content>jsp</body-content>  
-     <attribute>  
-         <name>maxLength</name>  
-         <required>true</required>  
-         <rtexprvalue>true</rtexprvalue>  
-         <type>int</type>  
-     </attribute>  
-     <attribute>  
-         <name>suffix</name>  
-         <required>false</required>  
-         <rtexprvalue>true</rtexprvalue>  
-     </attribute>  
-     <attribute>  
-         <name>value</name>  
-         <required>false</required>  
-         <rtexprvalue>true</rtexprvalue>  
-     </attribute>  
- </tag>  
四、标签应用： 
在jsp头加上： 
Java代码  ![收藏代码](http://ll-feng.iteye.com/images/icon_star.png)
- <%@ taglib uri="/WEB-INF/tld/mytags.tld" prefix="my" %>  
应用代码： 
Java代码  ![收藏代码](http://ll-feng.iteye.com/images/icon_star.png)
- <my:StringOmit maxLength="5" suffix="..." value="abcdefghijklmnopqrstuvwxyz"/>  
当不指定value值，标签将处理标签体内的内容： 
Java代码  ![收藏代码](http://ll-feng.iteye.com/images/icon_star.png)
- <my:StringOmit maxLength="5" suffix="..."/>  
- <my:StringOmit maxLength="5" suffix="..."></my:StringOmit>  
- <my:StringOmit maxLength="5" suffix="..." value="abcdefgasdfadsf"/><br>  
- <my:StringOmit maxLength="5" suffix="..." value="41234123412341"></my:StringOmit><br>  
- <my:StringOmit maxLength="5" suffix="...">fasdfasdfasdfasdfa</my:StringOmit>  
五、备注 
1、在这里只需实现BodyTagSupport类的实现BodyTagSupport类的doAfferBody()和doEndTag()方法即可。 
2、value属性和标签体内容都不是必须的。所以在jsp中可能灵活运用。当设置value属性时。取value输出。没有value时。取标签体内容输出。二者都没有也不会报错 
3、当没有标签体时。doAfterBody()是不会被执行的。所以最终要在doEndTag()方法中对最终输出作统一处理
