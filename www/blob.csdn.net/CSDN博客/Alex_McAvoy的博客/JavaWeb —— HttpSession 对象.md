# JavaWeb —— HttpSession 对象 - Alex_McAvoy的博客 - CSDN博客





2019年01月11日 22:23:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：30








# 【概述】

由于 HTTP 协议是一种无状态协议，即当客户向服务器发送请求，服务器接受请求并作出回应后，该连接就结束了，而服务器不保存相关的信息，为了弥补这一缺点，HTTP 提供了 session 机制。

session 是一类用于在客户端与服务端保持状态的解决方案，有时其也指这种解决方案的存储结构，在 JavaWeb 中，session 指的是 HttpSession 对象。

服务器使用一种类似于散列表的结构来保存信息，当程序需要为某客户端的请求创建一个 session 时，服务器首先检查这个客户端请求中是否包含一个 session 标识，即：sessionID，若想在整个交互过程中始终保存状态，就必须在每个客户端请求的路径后都保持这个 sessionID
- 若请求不包含 sessionID，则为客户端创建一个 session 并生成一个与此 session 相关的 sessionID，这个 sessionID 将在本次响应中返回给客户端保存
- 若已包含 sessionID，则说明以前已经为此客户端创建过 session，服务器就按照 sessionID 把这个 session 检索出来。若检索不到，则会新建一个，这种情况可能出现在服务器已经删除了该用户对应的 session 对象，但用户人为地在请求的 URL 后附加一个 JSESSION 参数。

保存 sessionID 的可以使用 cookie，这样在交互过程中，浏览器可以自动的按照规则，把这个标识发送给服务器，但由于 cookie 可被人为禁用，因此需要在 cookie 被禁用时仍能够将 sessionID 传回服务器，除使用 cookie 外，还有两种方式：
- 一种是 URL 重写，即将 sessionID 附加在 URL 路径后
- 一种是将 sessionID 作为查询字符串附加在 URL 后

# 【session 创建条件】

对 JSP 而言，当服务器访问任何一个资源时，只有以下两种情况不会创建一个新的 HttpSession 对象：
- 当前 JSP 是客户端访问当前 WEB 应用的第一个资源，且 JSP 的 page 指令的 session 属性值为 false（不能使用 JSP 的 session 隐含对象，但可以显式的来使用其他的 HttpSession 对象）
- 当前 JSP 不是客户端访问当前 WEB 应用的第一个资源，且其他页面已经创建了一个 HttpSession 对象，则会将与当前会话相关联的 HttpSession 对象返回给当前的 JSP 页面

对 Servlet 而言，当服务器访问任何一个资源时，只有以下两种情况不会创建一个新的 HttpSession 对象：
- 若 Servlet 是客户端访问当前 WEB 应用的第一个资源，则只有调用了 request.getSessoin() 或 request.getSession(true) 才会创建 HttpSession 对象，否则不会创建
- 若 Servlet 不是客户端访问当前 WEB 应用的第一个资源，则其他页面已经创建了一个 HttpSession 对象，则会将与当前会话相关联的 HttpSession 对象返回给当前的 JSP 页面

# 【生命周期】

## 1.创建

创建 HttpSession 的方法位于 HttpServletRequest 接口中，其有两个方法：
- request.getSession(boolean create)：

	若 create 为 false，若没有与当前 JSP 关联的 HttpSession 对象，返回 null，若有返回 true

	若 create 为 true，则一定返回一个 HttpSession 对象，若没有与当前 JSP 关联的 HttpSession 对象，则服务器创建一个新的 HttpSession 对象并返回，若有，直接返回关联的 HttpSession 对象
- request.getSession()：等同于 request.getSession(true)
`<% request.getSession(true); %>`
## 2.销毁

长时间不向客户端发送请求，session 对象会自动销毁，但在某些情况下需要手动销毁，此时可使用：**session.invalidate()** 来使当前的 session 失效。

当 session 被销毁后，就不能使用该 session 对象，如果再调用相关方法，将会抛出异常。
`<% session.invalidate(); %>`
需要注意的是，不是关闭了浏览器就销毁了 session

## 3.过期时间

若一个会话中，客户端长时间不向服务器发送请求，session 对象就会自动消失，这个时间称为过期时间，其取决于服务器，一般来说，其生存周期在 20~30 分钟左右，例如：Tomcat 服务器默认为 30 分钟。

当用户首次访问时将产生一个新的 session，服务器就会记住这个会话状态，当会话超过过期时间或手动销毁后，这个 session 就不能被使用了，为避免用户登录网站后在 session 有效期外进行相应操作从而看到一错误画面，在开发时需要考虑 session 的有效性判断。

对 session 过期时间的设置有两种方法，一种是在 WEB 服务器的 web.xml 文件以分钟为单位来进行设置，一种是使用 HttpSession 对象的相关方法。

```
<!-- 设置服务器对session的默认销毁时间为30分钟 -->
<servlet-config>
    <session-timeout>30</session-timeout>
<servlet-config/>
```

HttpSession 对象与过期时间相关的方法：
- getLastAccessedTime()：返回客户端最后一次与会话相关联的请求时间
- getMaxInactiveInterval()：以秒为单位返回一个会话内两个请求的最大间隔
- setMaxInactiveInterval(int number)：以秒为单位设置 session 的有效时间

# 【常用方法】
- getID()：获取 sessionID
- isNew()：判断 session 是否为新建
- invalidate()：销毁当前 session
- getMaxInactiveInterval()：获取 session 的最大时效
- getCreationTime()：获取 session 的创建时间
- getLastAccessedTime()：获取 session 的上次访问时间
- setAttribute(String name,Object obj)：用于创建客户端会话，将 obj 对象保存在 session 作用域内，name 为 obj 对象在 session 中的名字
- getAttribute(String name)：用于获取客户端会话，从 session 作用域中获取名为 name 的对象，其返回类型为 Object 类，若想将获取到的信息赋给 String 类型，需要使用强制转换或者 toString() 方法
- removeAttribute(String name)：移除指定对象，从 session 作用域中移除名为 name 的对象，使用时需保证移除的变量在 session 范围内有效，否则将抛出异常

# 【URL 重写实现 session 跟踪】

URL 重写即为将会话标号以参数的形式附加在超链接的 URL 地址后面 ，其使得当在浏览器不支持 Cookie 或关闭了 Cookie 的情况下，WEB 服务器仍可与浏览器实现现有状态的会话，这种补充机制要求在响应消息的实体内容中必须包含下一次请求的超链接，并将会话标号作为超链接的 URL 地址的一个特殊参数，即需要对所有可能被客户端访问的请求路径（超链接、frform 表单的 action 属性、重定向的 URL）进行URL重写。

HttpServletResponse 接口定义了两个用于重写 URL 的方法：
- encodeURL()：对给定 URL，通过加上 sessionID 的方式进行编码，若不需编码，则直接返回未经修改的 URL
- encodeRedirectURL()：对给定 URL 进行编码，以用于 sendRedirect() 方法，若不需编码，则直接返回未经修改的 URL

例如：

```
<form action="<%= response.encodeURL("test.jsp")>" method="post">
    ...
</form>
```

两方法的异同：

共同点：均对 URL 附加上 jsessionid 参数进行了处理
- 若需要，则在 URL 的 path 后附加上 jsessionid=xxx
- 若不需要则直接返回传入的 URL

不同点：encodeURL() 在附加 jsessionid 之前对 URL 做了判断处理，而 encodeRedirectURL() 不对 URL 本身进行处理，只专注于是否需要添加 jsessionid参数，简单来说，若 encodeURL() 方法进行了编码，则返回的 URL 一定是完整 URL 而不是相对路径
- 如果 URL 为空字符串，则 encodeURL() 将 URL 转换为以 http 或 https 开头的完整的 URL
- 如果 URL 是完整的 URL，但不含任何路径（即只包含协议、主机名、端口，例：http://127.0.0.1），则 encodeURL() 将在末尾加上根路径符号 " / "



