# 在asp.net2中针对不支持cookie的APP自动添加sessionid到URL中 - weixin_33985507的博客 - CSDN博客
2007年04月14日 18:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
在web.config中的设置如下:
<sessionState mode="InProc"
                    cookieless="true"
                    timeout="20"/>
在default.aspx中输入如下代码测试:
Response.Write(Response.ApplyAppPathModifier("default.aspx"));
在我的机器上输出为:
/testweb/(S(dp4y0e45j5yoeq45beh1sk55))/default.aspx 
使用此Response.ApplyAppPathModifier函数即可自动转换URL,如果有HTTPS的站点需要切换,到是个不错的方法.
