# 同一页面多个ajax请求后台堵塞问题 - 左直拳的马桶_日用桶 - CSDN博客
2018年09月21日 18:35:27[左直拳](https://me.csdn.net/leftfist)阅读数：1178
有个小伙子告诉我，一个页面上同时开启多个ajax，向后台请求数据，会被堵塞。
“因为是同一个会话。但如果在控制器上加上这个特性就可以了”，小伙子说。
```
//session只读，避免同一会话中session锁导致请求阻塞
    [SessionState(System.Web.SessionState.SessionStateBehavior.ReadOnly)]
    public class CxController : BaseLT.Web.UserControllerBase
    {
    。。。。
    }
```
经测试，还真是。如果没有这个特性，多个ajax请求到了后台，原本是并发的，结果变成了队列，一个接一个地完成。如果加上，则是并行完成。
个中原因，可以看以下参考资料：
[ASP.net Session阻塞、Session锁、MVC Action请求阻塞问题](https://www.cnblogs.com/fanqie-liuxiao/p/5702633.html)
“*HttpSessionState来自于HttpModule的SessionStateModule。在每次请求处理过程中，HttpApplication的请求的处理管道中会检查当前请求的处理程序是否实现了接口IRequiresSessionState，如果实现的话，那么SessionStateModule将为这个请求分配HttpSessionState。同时SessionStateModule还负责SessionID的生成、Cookieless会话管理、从外部状态提供程序中检索会话数据以及将数据绑定到请求的调用上下文。如果页面请求设置一个读取器锁定，同一会话中同时处理的其他请求将无法更新会话状态，但是至少可以进行读取。如果页面请求为会话状态设置一个写入锁，那么所有其他页面都被阻止，无论他们是否要读取或写入内容。例如，如果同时有两段程序视图在同一个Session中写入内容，一段程序必须等到另一段程序完成后才能写入。在AJAX程序设计中，必须注意这种情况的发生。*”
我只想说，后生可畏。
