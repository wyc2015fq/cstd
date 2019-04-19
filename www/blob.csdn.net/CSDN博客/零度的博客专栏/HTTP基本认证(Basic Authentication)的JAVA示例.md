# HTTP基本认证(Basic Authentication)的JAVA示例 - 零度的博客专栏 - CSDN博客
2016年05月11日 16:15:14[零度anngle](https://me.csdn.net/zmx729618)阅读数：5933
大家在登录网站的时候，大部分时候是通过一个表单提交登录信息。
但是有时候浏览器会弹出一个登录验证的对话框，如下图，这就是使用HTTP基本认证。
![](https://img-blog.csdn.net/20140604141818218)
下面来看看一看这个认证的工作过程:
第一步:  客户端发送http request 给服务器,服务器验证该用户是否已经登录验证过了，如果没有的话，
服务器会返回一个401 Unauthozied给客户端，并且在Response 的 header "WWW-Authenticate" 中添加信息。
如下图。
![](https://img-blog.csdn.net/20140604141847593)
第二步:浏览器在接受到401 Unauthozied后，会弹出登录验证的对话框。用户输入用户名和密码后，
浏览器用BASE64编码后，放在Authorization header中发送给服务器。如下图:
![](https://img-blog.csdn.net/20140604141926218)
第三步: 服务器将Authorization header中的用户名密码取出，进行验证， 如果验证通过，将根据请求，发送资源给客户端。
下面来看一个JAVA的示例代码:
**[java]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/28419625#)[copy](http://blog.csdn.net/kkdelta/article/details/28419625#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/375955/fork)
- import java.io.IOException;  
- import java.io.PrintWriter;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- import sun.misc.BASE64Decoder;  
- 
- publicclass HTTPAuthServlet extends HttpServlet {  
- 
- publicvoid doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {  
-         String sessionAuth = (String) request.getSession().getAttribute("auth");  
- 
- if (sessionAuth != null) {  
-             System.out.println("this is next step");  
-             nextStep(request, response);  
- 
-         } else {  
- 
- if(!checkHeaderAuth(request, response)){  
-                 response.setStatus(401);  
-                 response.setHeader("Cache-Control", "no-store");  
-                 response.setDateHeader("Expires", 0);  
-                 response.setHeader("WWW-authenticate", "Basic Realm=\"test\"");  
-             }             
- 
-         }  
- 
-     }  
- 
- privateboolean checkHeaderAuth(HttpServletRequest request, HttpServletResponse response) throws IOException {  
- 
-         String auth = request.getHeader("Authorization");  
-         System.out.println("auth encoded in base64 is " + getFromBASE64(auth));  
- 
- if ((auth != null) && (auth.length() > 6)) {  
-             auth = auth.substring(6, auth.length());  
- 
-             String decodedAuth = getFromBASE64(auth);  
-             System.out.println("auth decoded from base64 is " + decodedAuth);  
- 
-             request.getSession().setAttribute("auth", decodedAuth);  
- returntrue;  
-         }else{  
- returnfalse;  
-         }  
- 
-     }  
- 
- private String getFromBASE64(String s) {  
- if (s == null)  
- returnnull;  
-         BASE64Decoder decoder = new BASE64Decoder();  
- try {  
- byte[] b = decoder.decodeBuffer(s);  
- returnnew String(b);  
-         } catch (Exception e) {  
- returnnull;  
-         }  
-     }  
- 
- publicvoid nextStep(HttpServletRequest request, HttpServletResponse response) throws IOException {  
-         PrintWriter pw = response.getWriter();  
-         pw.println("<html> next step, authentication is : " + request.getSession().getAttribute("auth") + "<br>");  
-         pw.println("<br></html>");  
-     }  
- 
- publicvoid doPost(HttpServletRequest request, HttpServletResponse response) throws IOException {  
-         doGet(request, response);  
-     }  
- 
- }  
当request第一次到达服务器时，服务器没有认证的信息，服务器会返回一个401 Unauthozied给客户端。
认证之后将认证信息放在session，以后在session有效期内就不用再认证了。
