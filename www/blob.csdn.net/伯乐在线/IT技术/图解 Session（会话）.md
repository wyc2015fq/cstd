# 图解 Session（会话） - 文章 - 伯乐在线
原文出处： [BYSocket （@BYSocket）](http://www.bysocket.com/?p=384)
相继 [图解Http协议](http://blog.jobbole.com/108188/) 和 [图解Cookie](http://blog.jobbole.com/108191/) 之后，中间迷茫期哈，没写了！可是又要告诉你自己明明喜欢写为啥不写了！那就写吧，学到老学到老~ 然后写到老！本系列皆**以图为主**，力求**简单易懂**，娓娓道来`
# 一、Session由来
HTTP的**无状态**，也就是说，每次请求都是独立的线程。举个例子吧：购物中，你选择了A商品，加入购物车，这就是A线程。然后在选择B商品就是B线程。可是每次线程独立（对容器而言，A、B成了不同的用户），线程A不知道有B，B也不知道A。如何一起付款呢？
简答来说：**怎么保存同个用户多个请求会话状态呢**？自然**HTTPS**保证连接是安全的，可以使它与一个会话关联。
问题就在于如何跟踪同一个用户，选择自然很多：
> 
1、**EJB**(有状态会话bean保存会话状态) 环境苛刻需要带EJB的J2EE服务器，而不是Tomcat这种Web容器。
2、**数据库**（这貌似万能的。针对数据）
3、就是我们要讲的**HttpSession**，**保存跨一个特定用户多个请求的会话状态**。
4、上面说的**HTTPS**,条件太苛刻了。
如图：![session](http://jbcdn2.b0.upaiyun.com/2016/11/1b7163bb51505f0e6858f2f0849e82be.png)
# 二、Session机制
机制，什么用词有点高大上。其实就是把它内在的一点东西说出来。主要两个W：What？How？
**What is Session?**
Session代表着**服务器**和**客户端**一次**会话**的过程。直到**session失效**（服务端关闭），或者**客户端关闭**时结束。
**How does session works？**
Session 是**存储**在**服务端**的，并针对每个客户端（客户），通过SessionID来区别不同用户的。Session是以Cookie技术或URL重写实现。默认以Cookie技术实现，服务端会给这次会话创造一个JSESSIONID的Cookie值。
**补充**：
> 
其实还有一种技术：**表单隐藏字段**。它也可以实现session机制。这里只是作为补充，服务器响应前，会修改form表单，添加一个sessionID类似的隐藏域，以便传回服务端的时候可以标示出此会话。
这技术，也可以使用在Web安全上，可以有效地控制**CSRF跨站请求伪造**。
# 三、详细介绍Session机制过程
![session2](http://jbcdn2.b0.upaiyun.com/2016/11/3c9d1dbca23569f0685bff0e9b43ff54.png)
图中这是session**第一次请求**的详细图。以Cookie技术实现，我也写了个HttpSessionByCookieServletT.java 的Servlet小demo，模拟下Session的一生。代码如下：

Java
```
package org.servlet.sessionMngmt;
 
import java.io.IOException;
import java.io.PrintWriter;
 
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
/*
 * Copyright [2015] [Jeff Lee]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
/**
 * @author Jeff Lee
 * <a href="http://www.jobbole.com/members/chchxinxinjun">@since</a> 2015-7-12 10:58:28
 *  HttpSession的默认Cookie实现案例
 */
@WebServlet(urlPatterns = "/sessionByCookie")
public class HttpSessionByCookieServletT extends HttpServlet {
 
    private static final long serialVersionUID = 1L;
 
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
         
        // 获取session
        // 如果是第一次请求的话，会创建一个HttpSession，等同于 req.getSession(true);
        // 如果已存在session，则会获取session。
        HttpSession session = req.getSession();
         
        if (session.isNew()) {
            // 设置session属性值
            session.setAttribute("name", "Jeff");
        }
        // 获取SessionId
        String sessionId = session.getId();
         
        PrintWriter out = resp.getWriter();
        // 如果HttpSession是新建的话
        if (session.isNew()) {
            out.println("Hello,HttpSession! <br>The first response - SessionId="
                    + sessionId + " <br>");
        } else {
            out.println("Hello,HttpSession! <br>The second response - SessionId="
                    + sessionId + " <br>");
            // 从Session获取属性值
            out.println("The second-response - name: "
                    + session.getAttribute("name"));
        }
         
    }
     
}
```
**① 客户端向服务端发送第一次请求**
此时，客户端想让服务端把自己的名字设置到会话中。
**② 服务端的容器产生该用户唯一sessionID的session对象，并设置值**
可以从代码中看出通过从请求中req.getSession()，新生成了一个session对象。并设置了setAttribute(“name”, “Jeff”)，key为string，value是对象皆可。
这时候，我们不用再把session通过cookie技术处理，**容器**帮我们**处理**了。
**③ 容器响应 Set-Cookie：JSESSIONID= …**
我们可以F12,查看此次响应。
![image](http://jbcdn2.b0.upaiyun.com/2016/11/e192af9ef36540fa80ad79254e8cfcb6.png)
从图中可得到，每个Cookie的set，都有一个对应Set-Cookie的头。HttpOnly可是此Cookie只读模式。只不过**session唯一标识**是：**JSESSIONID**
**④ 浏览器解析Cookie，保存至浏览器文件。**
![image1](http://jbcdn2.b0.upaiyun.com/2016/11/609345650a8966b3901d80142c8dbb0e.png)
如图，找到了对应的session存储的cookie文件。该文件被保护不能打开。[图解Cookie](http://blog.jobbole.com/108191/) 教你怎么找到该文件。
**第二次请求会发什么变化呢？**
![3](http://jbcdn2.b0.upaiyun.com/2016/11/dd6c7bce85aaf29023b2627c69ac52d8.png)
下面，泥瓦匠重新访问了这个地址：
① 再次请求
![image2](http://jbcdn2.b0.upaiyun.com/2016/11/b153a90d975081f8f58cdb2d96834faf.png)
此时，请求会有Cookie值：**JSESSIONID=… 该值传给服务端**
**② 容器获取SessionId，关联HttpSession**
**③ 此时响应无SetCookie**
如图：
![image3](http://jbcdn2.b0.upaiyun.com/2016/11/fc4cbff112a9bb7124642aa2a3e2d88b.png)
但是这次请求，我们响应出上一次请求set的值。Jeff 就打印出来了！
关于服务端获取session，也就是从请求中获取session对象，容器会帮你根据Cookie找到唯一的session对象。
**泥瓦匠记忆小抄：Session机制，记住两次请求图即可。**
# 四、补充
点到为止哈~ 以后详细写。此图来自网络
![wkiom1l4oh2s5jwhaaa5g2i22fe912](http://jbcdn2.b0.upaiyun.com/2016/11/3a8103fce225a47420c6804e276f760e.png)
上图Bad guy，就是攻击者。跨站请求伪造，伪造用户请求来对服务器数据或者是用户等造成威胁。web安全也就是从这些基础中慢慢提升。
# 五、总结
> 
1、大概地描述了session的工作**机制**，和一些**安全**相关。记住Session是什么，怎么用，在服务端客户端之间怎么传输即可。
2、泥瓦匠学习的**代码**都在**github**上（同步osc git），欢迎大家点star，提意见，一起进步。***地址：******[https://github.com/JeffLi1993](https://github.com/JeffLi1993)***
