# IE里Iframe的Cookie问题解决办法总结(转) - z69183787的专栏 - CSDN博客
2012年11月22日 15:23:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1099
最近在做一个流量统计的东西的时候，偶然发现IE在对iframe里面的页面写Cookie的时候有一些安全限制，导致读取Cookie不成功，找了好长时间的解决办法，重要找到如下的办法：
1.页面里的COOKIE不能是浏览器进程的COOKIE(包括验证票和不设置超时时间的COOKIE),否则跨域会取不到.这点做跨域COOKIE的人比较少提到.不过实际上留意下几家大学做的方案,有细微的提到他们的验证模块里的COOKIE是有设置超时时间的.
2.当利用IFRAME时,记得要在相应的动态页的页头添加一下P3P的信息,否则IE会自觉的把IFRAME框里的COOKIE给阻止掉,产生问题.本身不保存自然就取不到了.这个其实是FRAMESET和COOKIE的问题,用FRAME或者IFRAME都会遇到.
3.测试时输出TRACE,会减少很多测试的工作量.
**只需要设置 P3P HTTP Header**，在隐含 iframe 里面跨域设置 cookie 就可以成功。他们所用的内容是：
P3P: CP='CURa ADMa DEVa PSAo PSDo OUR BUS UNI PUR INT DEM STA PRE COM NAV OTC NOI DSP COR'
ASP直接在头部加了头部申明，测试有效。
<%Response.AddHeader "P3P", "CP=CAO PSA OUR"%>
php的话，应该是如下写法：
header('P3P: CP=CAO PSA OUR');
ASP.NET的话
通过在代码上加Response.AddHeader("P3P", "CP=CAO PSA OUR")或者在Window服务中将ASP.NET State Service 启动。
JSP:
response.setHeader("P3P","CP=CAO PSA OUR")
