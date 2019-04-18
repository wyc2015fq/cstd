# Servlet中进行context属性的同步 - weixin_33985507的博客 - CSDN博客
2015年09月29日 16:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Servlet中进行context属性的同步；
必须所有使用context的servlet都进行synchronized才可以实现同步；
servlet：
```
package com.stono.servlet.synchronize;
import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
public class TestServletA extends HttpServlet {
    private static final long serialVersionUID = 1L;
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ServletContext context = req.getServletContext();
        Object attribute;
        synchronized (context) {// 为了防止在sleep过程中其他线程(servlet)修改context中的属性a；
            context.setAttribute("a", "a");
            try {
                Thread.sleep(3000);
            } catch (Exception e) {
                e.printStackTrace();
            }
            attribute = context.getAttribute("a");
        }
        PrintWriter writer = resp.getWriter();
        writer.println(attribute);
    }
}
```
```
package com.stono.servlet.synchronize;
import java.io.IOException;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
public class TestServletB extends HttpServlet {
    private static final long serialVersionUID = 1L;
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ServletContext context = req.getServletContext();
        synchronized (context) {// 必须这个类也进行synchronized才可以实现同步；这种情况下，这个servlet也会等带3秒的；
            context.setAttribute("a", "change to b");
        }
    }
}
```
 servlet实现SingleThreadModel是不行的，该接口已经被废弃，而且只是对某一个servlet实现单线程处理；
不能保证线程之间的安全性；
