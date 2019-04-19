# Java中的cookie - LC900730的博客 - CSDN博客
2017年07月25日 12:05:47[lc900730](https://me.csdn.net/LC900730)阅读数：112标签：[java																[cookie](https://so.csdn.net/so/search/s.do?q=cookie&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## Cookie类
Cookie类：用于存储会话数据 
原理： 
1.服务器创建Cookie对象，把会话数据存储到cookie对象中 
2.服务器发送cookie信息到浏览器 
3.浏览器得到服务器发送的cookie，然后保存到浏览器端 
4.在浏览器下次访问服务器时，会带着cookie信息 
5.服务器接收到浏览器带来的cookie信息
|1.构造Cookie对象|Cookie(String name,String value)|
|----|----|
|2.设置Cookie|setPath(uri)：设置cookie的有效路径|
|setMaxAge(int expire)：设置cookie有效时间| |
|setValue(newValue):设置值| |
|3.发送cookie到浏览器保存|response.addCookie(Cookie cookie):发送cookie|
|4.Cookie [] cookies=request.getCookies()|拿到请求头里面的cookie|
### Cookie的使用
##### Cookie的设置
```java
public class CookieDemo1 extends HttpServlet{
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        // TODO Auto-generated method stub
//      super.doGet(req, resp);
        //1.创建cookie
        Cookie cookie=new Cookie("name", "eric");
        Cookie cookie1=new Cookie("eamil", "eric@qq.com");
        //2.发送到浏览器,通过响应头发送
//      resp.setHeader("set-cookie","name=eric,email=eric@qq.com");
        resp.addCookie(cookie); 
        resp.addCookie(cookie1);
        //可以追加多个cookie,内部自动帮我们拼接
    }
}
```
##### Cookie的获取
```
Cookie [] cookies=request.getCookies();
    if(cookies!=null){
        for(Cookie c:cookies){
            System.out.println(c.getName());
            System.out.println(c.getValue());
    }
    }
```
|setMaxage(-1):保存在浏览器内存中|setMaxage(0):清除cookie|
