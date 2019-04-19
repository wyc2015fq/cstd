# Java后台实时输出数据到页面 - 零度的博客专栏 - CSDN博客
2016年07月15日 17:17:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：12750
当然，这个专业的有保持长链接，服务器推送等实现，这里需求较简单，不讨论。
做了个小功能，希望前台可以看到后台的实时处理进度，首先想到的就是能够在页面像后台打印日志一样的输出，
如下代码：
```
PrintWriter out = response.getWriter();
while(true){
    try {
        out.print("输出内容");
        out.flush();
        Thread.sleep(1000);
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```
        在我的想象中，response会每隔一秒就向前端输出一条内容，前端像打印日志一样实时显示。可实际情况是这些内容在请求结束后是同时被输出到页面的。
       网上查了一下，大致是说这样输出没给浏览器响应的机会，只有等方法结束后浏览器才能响应读取到数据。貌似tomcat7之后的可以支持，行不通啊。后来发现，如果向页面输出js脚本的话，浏览器是可以实时读取到的，见下面代码：
```
response.setContentType("text/html; charset=UTF-8");
		PrintWriter out = response.getWriter();
		while(true){
			    try {
			    	out.println("<script>");
			    	out.println("document.write('<div>输出内容</div>');");
			    	out.println("</script>");
			        out.flush();
			        Thread.sleep(1000);
			    } catch (Exception e) {
		        e.printStackTrace();
			    }
		}
```
       这样做，如果输出内容很多的话貌似会使页面内容过多而消耗一些资源，不过后台的系统没什么并发，内容也不是很多，将就了。
