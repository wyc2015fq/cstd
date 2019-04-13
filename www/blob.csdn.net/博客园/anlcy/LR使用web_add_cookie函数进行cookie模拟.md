
# LR使用web_add_cookie函数进行cookie模拟 - anlcy - 博客园






# [LR使用web_add_cookie函数进行cookie模拟](https://www.cnblogs.com/camilla/p/10283717.html)
1    为什么要使用cookie模拟
从日常项目测试过程中的问题说起。
比如要进行论坛中的文件下载功能的测试。我们都知道只有登录用户才能进行下载操作，这样我们的测试过程可能就变成了先登录系统，然后再进行下载操作。在使用loadrunner设计脚本时就要先在初始化部分写登录脚本，然后再action中写下载操作，当单个用户运行时我们可能看不到什么问题。当用户并发量较多时，问题马上暴露出来了。大部分用户可能很长时间也没有下载成功，大量的初始化登录操作已经给系统造成了很大的压力，而在日常访问中，这种大量短时间登录的压力是不容易出现的。这样的测试结果可能无法达到我们的测试目标。
的确，不能处理较大的登录吞吐可能已经是系统的瓶颈了，我们可以通过这种全流程的测试方式发现登录的问题。但是后续的测试我们可能就无法进行下去了。当项目组解决了登录的吞吐问题时，可能项目工期已经十分紧张了，再进行重要的下载功能测试可能已经太晚了。
这种情况在测试过程中经常会出现，我们要如何绕过登录尽早的进行后续的下载操作呢？如果网站使用了cookie的机制，则我们可以尝试使用下面的方法。
2    怎么使用loadrunner模拟
Loadrunner的web函数中提供了几个关于cookie的函数。
web_remove_cookie()
web_add_cookie()
web_cleanup_cookies();
web_reg_add_cookie();
这里我们为了模拟cookie绕过登录使用web_add_cookie();
首先获取网站留在客户端上的cookie都存储了什么。通常主要内容包括名字，值，过期时间，作用域和路径，分为会话cookie和文件cookie。会话cookie存储在内存里，关闭浏览器后就没有了，这种很难模拟。文件cookie是将cookie信息写到硬盘上，关闭浏览器再次打开后仍可以使用。我们要模拟的就是这种存储在硬盘文件里的cookie。我们观察cookie文件的内容，如果使用ie的话，在“C:\Documents and Settings\用户名\Local Settings\Temporary Internet Files”中的用户名@域名的文件里，cookie存储都是加密的，所以我们直接看并看不出什么特别的东西，也搞不清楚具体是什么。这时我们借助IE webdeveloper 工具获取cookie值如下所示：
cdb_cookietime=2592000;cdb_smile=1D1; cdb_sid=71WKOd; cdb_auth=TzsVl16XRFahjqkfuWyoF5OzI%2BrekHgiy0YXJd8m1y9vq6aV4bg3GGMsa9s; cdb_visitedfid=20; cdb_onlineusernum=28
获得了cookie信息后，我们进行编辑loadrunner脚本。将cookie每个分号分行，使用UltraEdit进行列模式操作，在每句后面加上域名。（域名在webdeveloper的Attributes下也可以找到domain对应值），修改完成后如下。
web_add_cookie("cdb_cookietime=2592000;domain=172.16.1.3");
web_add_cookie("cdb_smile=1D1;domain=172.16.1.3");
web_add_cookie("cdb_sid=71WKOd;domain=172.16.1.3");
web_add_cookie("cdb_auth=TzsVl16XRFahjqkfuWyoF5OzI%2BrekHgiy0YXJd8m1y9vq6aV4bg3GGMsa9s;domain=172.16.1.3");
web_add_cookie("cdb_visitedfid=20;domain=172.16.1.3");
web_add_cookie("cdb_onlineusernum=28;domain=172.16.1.3");
把这些值放在要访问的url的web函数之前，你就可以绕过登录进行后续操作了。
如:
Action()
{
web_add_cookie("cdb_cookietime=2592000;DOMAIN=172.16.1.3");
web_add_cookie("cdb_smile=1D1;DOMAIN=172.16.1.3");
web_add_cookie("cdb_oldtopics=D280D39523D39418D;DOMAIN=172.16.1.3");
web_add_cookie("cdb_sid=eSuWgw;DOMAIN=172.16.1.3");
web_add_cookie("cdb_auth=TzsVl16XRFahjqkfuWyoF5OzI%2BrekHgiy0YXJd8m1y9vq6aV4bg3GGMsa9s;DOMAIN=172.16.1.3");
web_add_cookie("cdb_visitedfid=20;DOMAIN=172.16.1.3");
web_url("cookies",
"URL=http://172.16.1.3:8080/bbs/index.php",
"referer=http://172.16.1.3/newiweb/index/dealIndex.do?action=Index",
"Mode=HTML",
"RecContentType=text/html",
LAST
);
return 0;
}
最后注明一下：loadrunner的web录制功能是自动的增加cookie的，只要在cookie不过期的情况下，我们不需要手工的增加cookie，当出现录制不上或录制的值发生变化时可以手工进行添加。
在loadrunner场景设计使用其他负载机器时，同样可以使用你本机的这个cookie值进行，不会因为客户端机器发生变化而产生影响。
本文转自：http://www.cnblogs.com/Lemon_s/articles/1759696.html





