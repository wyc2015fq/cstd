# JSP Debug日志 - =朝晖= - 博客园
# [JSP Debug日志](https://www.cnblogs.com/dhcn/p/7116169.html)
2006年末一篇blog，那时候好生涩啊：
 先谈两个关于reponse.sendRedirect()函数的问题 :
      1、在Servlet的处理流或JSP的页面流中，sendRedirect()的出现并不意味着跳出处理流，或言之：处理流的中断，也就是说，即便页面跳转了，但后续处理如果没有用判断语句加以跳出，则后续语句仍然会运行，查了一下资料，发现其原因是sendRedirect()引入了额外的响应/请求对，所以原请求/响应对仍然存在，所以原Servlet仍然会运行。
     2、第二个是sendRedirect()的作用域问题，经过bug情况和试验发现，sendRedirect()在<jsp:include>和tile引用的子页面中出现时，根本不会影响页面转向。对于这一点，我查了一下资料发现：sendRedirect()控制的转移通过向客户发送302状态代码和Location响应报头来完成。转移需要另外的网络传送，而作为服务器端子页面包含的这种情况下网络传送就缺乏充当客户端角色的元素,根本无法完成网络传送
    3、Tomcat出现springcontext.xml定义的datasource缺乏找到数据源的问题，这种问题把webapp中的所有的用户app全部删除了，然后重新部署哪一个webapp就没了，以前都没有找到问题之所在，今天我调试了一下另外一个webapp，又转回这个[spring](http://lib.csdn.net/base/javaee)的这个webapp就发生这种问题，才找到问题的解决办法。数据源是在服务器的server.xml中定义的,不知为什么，为什么多app的时候就找不到了。
    4、表单form在method为“get”时，action属性中手工添加的表单参数就无效，只有method在post时，这种情况下的查询参数才有效。
     5、在用getPrarameter查询post参数以后，就不能再读request的inputstream了，负责会导致死循环。
      6、对于频繁更新的页面，如ajax请求的页面，为了防止缓存，可以设置首部 Cache-control:no-cache
      7、发现一个郁闷的问题：在IE6中document.getElementById这个方法，是name 和id属性一起取，也就是说当你想取一个id为"idA"的元素时，最好不要有其他元素的name是"idA"，否则会发生混淆。
      8、说一个情况:那种多页面组合互包含组合形成的页面，head头等前面的内容最好放在主主体包含页面上，特别是是这个头的某些表述会对某些代码属性有影响的时候。

