# 内部的XSS攻击的防范 - 零度的博客专栏 - CSDN博客
2016年08月05日 09:19:42[零度anngle](https://me.csdn.net/zmx729618)阅读数：452
       我们来专门探讨如何防范来自内部的XSS攻击，XSS攻击主要的攻击手段无外乎是发送了一段恶意脚本到受害者机器上去运行，所以我们的思路就是，如何让这段脚本失效。
       因为脚本的组成部分是<script>和</script>，而这其中最主要的是大于号和小于号字符，所以我们只要在请求中，把大于号字符，小于号字符处理下，让其没办法通过 Http发送到受害者机器上，当然就没办法在受害者机器上组成一段恶意脚本了。但是如果我们的内容中真的有大于号和小于号，并且作为内容的一部分而最终目的并不是入侵怎么办？我们只要吧大于号，小于号进行全角化处理就可以了，因为只有half-width的大于号和小于号能组成<script>,</script>，而full-width的大于号和小于号是不可以被对待成<script>,</script>的。
       读者可能又问，现在就算你不让我输入大于号和小于号，但是我可以以unicode字符的形式输入这些字符啊，比如大于号叫 > 小于号叫< 所以我只要在恶意脚本中所有用到大于小于的地方全部用这些替换，一样可以达到入侵的目的。 所以，我们必须对于这种&,#还有其他特殊字符也进行处理。
        综上所述，我们大概明白怎么做了，我们只要做一个过滤器，然后把这些特殊字符都过滤掉就可以了。
web.xml中定义一个过滤器：
**[html]**[view plain](http://blog.csdn.net/z69183787/article/details/34446229#)[copy](http://blog.csdn.net/z69183787/article/details/34446229#)[print](http://blog.csdn.net/z69183787/article/details/34446229#)[?](http://blog.csdn.net/z69183787/article/details/34446229#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/405828/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <web-appversion="2.5"xmlns="http://java.sun.com/xml/ns/javaee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
- <display-name>XSSDemo</display-name>
- <filter>
- <filter-name>XSS Filter</filter-name>
- <filter-class>com.charles.study.XSSFilter</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>XSS Filter</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- </web-app>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<span style="font-family:Arial;font-size:12px;"><?xml version="1.0" encoding="UTF-8"?>
<web-app version="2.5" xmlns="http://java.sun.com/xml/ns/javaee"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
    <display-name>XSSDemo</display-name>
 <filter>
        <filter-name>XSS Filter</filter-name>
        <filter-class>com.charles.study.XSSFilter</filter-class>
    </filter>
    <filter-mapping>
        <filter-name>XSS Filter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>           
</web-app></span>
```
[?](http://www.2cto.com/Article/201312/264747.html#)
        然后我们定义一个 XSSHttpServletRequestWrapper,它是HttpServletRequest的封装器，其中会吧一些特殊字符全部处理掉，被处理的特殊字符就是那些会造成script的字符，吧他们全部“全角化”，来避免植入恶意代码，这是通过覆写getParameter()和getHeader()方法来实现的：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/34446229#)[copy](http://blog.csdn.net/z69183787/article/details/34446229#)[print](http://blog.csdn.net/z69183787/article/details/34446229#)[?](http://blog.csdn.net/z69183787/article/details/34446229#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/405828/fork)
- /**   
- * @Title: XSSHttpServletRequestWrapper.java 
- * @Package com.wonders.xss 
- * @Description: TODO(用一句话描述该文件做什么) 
- * @author zhoushun   
- * @date 2014年7月2日 上午9:28:36 
- * @version V1.0   
- */
- package com.wonders.xss;  
- 
- import java.util.Iterator;  
- import java.util.Map;  
- import java.util.Set;  
- 
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletRequestWrapper;  
- 
- /** 
-  * @ClassName: XSSHttpServletRequestWrapper 
-  * @Description: TODO(这里用一句话描述这个类的作用) 
-  * @author zhoushun 
-  * @date 2014年7月2日 上午9:28:36 
-  *  
-  */
- publicclass XSSHttpServletRequestWrapper extends HttpServletRequestWrapper{  
- public XSSHttpServletRequestWrapper(HttpServletRequest request) {    
- super(request);                                                                      
-     }    
- 
- /** 
-      * Override the original getParameter() method , 
-      * so that it can filter all the parameter name and parameter value 
-      * then use replace the special character that may cause XSS attack 
-      */
- @Override
- public String getParameter(String name) {                                                              
-         String value = super.getParameter(encodeXSS(name));                                                               
- //the following sentences will be replaced by logging sentence in actual project     
-         System.out.println("The original value received from getParameter() is:"+value);                                                   if (value != null) {    
-             value = encodeXSS(value);    
-         }    
- 
- //the following sentences will be replaced by logging sentence in actual project     
-         System.out.println("After handling XSS ,the actual value is:"+value);    
-         System.out.println();                                                             
- return value;    
-     }    
- 
- 
- 
- @Override
- public Map<String, String[]> getParameterMap() {  
- // TODO Auto-generated method stub
-         Map<String, String[]> paramMap = super.getParameterMap();  
-         Set<String> keySet = paramMap.keySet();  
- for (Iterator iterator = keySet.iterator(); iterator.hasNext();) {  
-           String key = (String) iterator.next();  
-           String[] str = paramMap.get(key);  
- for(int i=0; i<str.length; i++) {  
-                     str[i] = encodeXSS(str[i]);  
- //这里可以对页面传入的所有值进行过滤了，你想怎么处理就怎么处理。比如对出入的值进行html危险字符过滤
-        }  
-         }  
- return paramMap ;  
-     }  
- 
- /** 
-      * Override the original getHeader() method , 
-      * so that it can filter all the parameter name and parameter value 
-      * then use replace the special character that may cause XSS attack 
-      */
- @Override
- public String getHeader(String name) {                                                             
-         String value = super.getHeader(encodeXSS(name));                                                                 
- //the following sentences will be replaced by logging sentence in actual project     
-         System.out.println("The original value received from getHeader() is:"+value);                                                                 
- if (value != null) {    
-             value = encodeXSS(value);    
-         }    
- 
- //the following sentences will be replaced by logging sentence in actual project     
-         System.out.println("After handling XSS ,the actual value is:"+value);    
-         System.out.println();    
- 
- return value;    
-     }    
- 
- /** 
-      * replace all the characters that may cause XSS attack from half-width character 
-      * to full-width character 
-      *  
-      * @param s 
-      * @return 
-      */
- private String encodeXSS(String s) {    
- if (s == null || "".equals(s)) {    
- return s;    
-         }    
-         StringBuilder sb = new StringBuilder(s.length() + 16);    
- for (int i = 0; i < s.length(); i++) {    
- char c = s.charAt(i);    
- switch (c) {    
- 
- //handle the '<' and '>' which can be used for constructing <script> and </script>  
- case'>':    
-                 sb.append('＞');    
- break;    
- case'<':    
-                 sb.append('＜');    
- break;    
- 
- //since the html can support the characters using $#number format  
- //so here also need to escape '#','&' and quote symbol  
- case'\'':    
-                 sb.append('‘');    
- break;    
- case'\"':    
-                 sb.append('“');    
- break;    
- case'&':    
-                 sb.append('＆');    
- break;    
- case'\\':    
-                 sb.append('＼');    
- break;    
- case'#':    
-                 sb.append('＃');    
- break;                                                                                                                                                                                                        
- //if not the special characters ,then output it directly     
- default:    
-                 sb.append(c);    
- break;    
-             }    
-         }    
- return sb.toString();    
-     }    
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
<span style="font-family:Arial;font-size:12px;">/**   
* @Title: XSSHttpServletRequestWrapper.java 
* @Package com.wonders.xss 
* @Description: TODO(用一句话描述该文件做什么) 
* @author zhoushun   
* @date 2014年7月2日 上午9:28:36 
* @version V1.0   
*/
package com.wonders.xss;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;
/** 
 * @ClassName: XSSHttpServletRequestWrapper 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2014年7月2日 上午9:28:36 
 *  
 */
public class XSSHttpServletRequestWrapper extends HttpServletRequestWrapper{
	public XSSHttpServletRequestWrapper(HttpServletRequest request) {  
        super(request);                                                                    
    }  
                                                                
     /** 
     * Override the original getParameter() method , 
     * so that it can filter all the parameter name and parameter value 
     * then use replace the special character that may cause XSS attack 
     */  
    @Override  
    public String getParameter(String name) {                                                            
        String value = super.getParameter(encodeXSS(name));                                                             
        //the following sentences will be replaced by logging sentence in actual project     
        System.out.println("The original value received from getParameter() is:"+value);                                                   if (value != null) {  
            value = encodeXSS(value);  
        }  
                                                               
        //the following sentences will be replaced by logging sentence in actual project     
        System.out.println("After handling XSS ,the actual value is:"+value);  
        System.out.println();                                                           
        return value;  
    }  
          
    
    
    @Override
	public Map<String, String[]> getParameterMap() {
		// TODO Auto-generated method stub
		Map<String, String[]> paramMap = super.getParameterMap();
	    Set<String> keySet = paramMap.keySet();
	    for (Iterator iterator = keySet.iterator(); iterator.hasNext();) {
	      String key = (String) iterator.next();
	      String[] str = paramMap.get(key);
				for(int i=0; i<str.length; i++) {
					str[i] = encodeXSS(str[i]);
	//这里可以对页面传入的所有值进行过滤了，你想怎么处理就怎么处理。比如对出入的值进行html危险字符过滤
	   }
	    }
	    return paramMap ;
	}
	/** 
     * Override the original getHeader() method , 
     * so that it can filter all the parameter name and parameter value 
     * then use replace the special character that may cause XSS attack 
     */  
    @Override  
    public String getHeader(String name) {                                                           
        String value = super.getHeader(encodeXSS(name));                                                               
        //the following sentences will be replaced by logging sentence in actual project     
        System.out.println("The original value received from getHeader() is:"+value);                                                               
        if (value != null) {  
            value = encodeXSS(value);  
        }  
                                                            
        //the following sentences will be replaced by logging sentence in actual project     
        System.out.println("After handling XSS ,the actual value is:"+value);  
        System.out.println();  
                                                           
        return value;  
    }  
                                                                                                                                                                                                 
    /** 
     * replace all the characters that may cause XSS attack from half-width character 
     * to full-width character 
     *  
     * @param s 
     * @return 
     */  
    private String encodeXSS(String s) {  
        if (s == null || "".equals(s)) {  
            return s;  
        }  
        StringBuilder sb = new StringBuilder(s.length() + 16);  
        for (int i = 0; i < s.length(); i++) {  
            char c = s.charAt(i);  
            switch (c) {  
                                                              
            //handle the '<' and '>' which can be used for constructing <script> and </script>  
            case '>':  
                sb.append('＞');  
                break;  
            case '<':  
                sb.append('＜');  
                break;  
                                                                 
            //since the html can support the characters using $#number format  
            //so here also need to escape '#','&' and quote symbol  
            case '\'':  
                sb.append('‘');  
                break;  
            case '\"':  
                sb.append('“');  
                break;  
            case '&':  
                sb.append('＆');  
                break;  
            case '\\':  
                sb.append('＼');  
                break;  
            case '#':  
                sb.append('＃');  
                break;                                                                                                                                                                                                      
            //if not the special characters ,then output it directly     
            default:  
                sb.append(c);  
                break;  
            }  
        }  
        return sb.toString();  
    }  
}
</span>
```
       最后，我们来定义过滤器的实现，我们在doFilter()方法体中会吧所有的Http请求包装为我们自定义的包装器，这样所有当请求相关内容时候，我们包装器中就会对有可能引起XSS攻击的特殊字符处理，从而预防XSS 攻击。
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/34446229#)[copy](http://blog.csdn.net/z69183787/article/details/34446229#)[print](http://blog.csdn.net/z69183787/article/details/34446229#)[?](http://blog.csdn.net/z69183787/article/details/34446229#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/405828/fork)
- package com.charles.study;  
- import java.io.IOException;  
- import javax.servlet.Filter;  
- import javax.servlet.FilterChain;  
- import javax.servlet.FilterConfig;  
- import javax.servlet.ServletException;  
- import javax.servlet.ServletRequest;  
- import javax.servlet.ServletResponse;  
- import javax.servlet.http.HttpServletRequest;  
- /**
-  * XSSFilter that can be used to filter invalid character which may cause XSS attack
-  * @author charles.wang
-  *
-  */
- publicclass XSSFilter implements Filter {  
- 
- @Override
- publicvoid destroy() {  
-     }  
- /**
-      * now the doFilter will filter the request ,using the Wrapper class to wrap the request
-      * and in the wrapper class, it will handle the XSS issue
-      */
- @Override
- publicvoid doFilter(ServletRequest request, ServletResponse response,  
-             FilterChain chain) throws IOException, ServletException {  
-          XSSHttpServletRequestWrapper xssRequest = new XSSHttpServletRequestWrapper(  
-                     (HttpServletRequest) request);  
-             chain.doFilter(xssRequest, response);  
-     }  
- @Override
- publicvoid init(FilterConfig arg0) throws ServletException {  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
<span style="font-family:Arial;font-size:12px;">package com.charles.study;
import java.io.IOException;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
/**
 * XSSFilter that can be used to filter invalid character which may cause XSS attack
 * @author charles.wang
 *
 */
public class XSSFilter implements Filter {
                                                                                                      
    @Override
    public void destroy() {
    }
    /**
     * now the doFilter will filter the request ,using the Wrapper class to wrap the request
     * and in the wrapper class, it will handle the XSS issue
     */
    @Override
    public void doFilter(ServletRequest request, ServletResponse response,
            FilterChain chain) throws IOException, ServletException {
         XSSHttpServletRequestWrapper xssRequest = new XSSHttpServletRequestWrapper(
                    (HttpServletRequest) request);
            chain.doFilter(xssRequest, response);
    }
    @Override
    public void init(FilterConfig arg0) throws ServletException {
    }
}</span>
```
       我们基于上面的实现来做例子，我们回忆以前文章，假如在恶意页面的输入框中输入<script>alert("Attack");</script>时候，在受害者页面会弹出一个alert对话框，也就是说这个恶意js在受害者的自己域上执行了从而达到内部XSS攻击的目的。
那么现在呢？假如我们在页面中输入同样的脚本：
![](http://www.2cto.com/uploadfile/2013/1213/20131213111909597.jpg)
因为我们启用了XSS的过滤器，所以他们会自动吧这些大于小于转为全角，从而破坏其形成一段脚本，我们来看下服务器日志：
![](http://www.2cto.com/uploadfile/2013/1213/20131213111910788.png)
因为做了半角到全角的转换，所以最终页面不会弹出那个alert了，而且正确的显示了：
![](http://www.2cto.com/uploadfile/2013/1213/20131213111910289.jpg)
总结：
       其实这种解决方案很common,其思路就是类似于海关检查，大家都知道爆炸物和有威胁的物品（恶意脚本）大多数都是硫磺，金属（'<','>')等等，那么我只要在过海关的时候(用户提交输入内容框）对这些危险物品进行清查，该扣留的扣留，该销毁的销毁（我们的encodeXSS方法，吧这些< ,>全部全角化）），这样就算这个人通过了检查，他因为没有了硫磺和金属物品，所以他没办法制作有威胁的炸弹了（到我们例子就是我们把特殊字符全部全角化了之后，它就不再能形成一个脚本了），于是达到防止内部XSS攻击的结果。
