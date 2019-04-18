# 最常用的20个ASP代码片段 上 - weixin_33985507的博客 - CSDN博客
2007年05月31日 09:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
1.如何用Asp判断你的网站的虚拟物理路径 
答：使用Mappath方法 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< p align="center">< font size="4" face="Arial">< b >
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)The Physical path to this virtual website is: 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< /b >< /font >
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< font color="#FF0000" size="6" face="Arial">
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< %= Server.MapPath("\")% >
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< /font >< /p >
2.我如何知道使用者所用的浏览器？ 
答：使用the Request object方法 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)strBrowser=Request.ServerVariables("HTTP_USER_AGENT") 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)If Instr(strBrowser,"MSIE") <> 0 Then 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)　　Response.redirect("ForMSIEOnly.htm") 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)Else 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)　　Response.redirect("ForAll.htm") 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)End If
3.如何计算每天的平均反复访问人数 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< % startdate=DateDiff("d",Now,"01/01/1990") 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)if strdate< 0 then startdate=startdate*-1 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)avgvpd=Int((usercnt)/startdate) % >
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)显示结果 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< % response.write(avgvpd) % >
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)that is it.this page have been viewed since November 10,1998
4.如何显示随机图象 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< % dim p,ppic,dpic 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)ppic=12 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)randomize 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)p=Int((ppic*rnd)+1) 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)dpic="graphix/randompics/"&p&".gif" 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)% >
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)显示 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< img src="< %=dpic% >">
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
5.如何回到先前的页面 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)：< a href="< %=request.serverVariables("Http_REFERER")% >" >preivous page< /a >
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)或用图片如：< img src="arrowback.gif" alt="< %=request.serverVariables("HTTP_REFERER")% >" >
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
6.如何确定对方的IP地址 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< %=Request.serverVariables("REMOTE_ADDR)% >
7.如何链结到一副图片上 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< % @Languages=vbs cript % >
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< % response.expires=0 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)strimagename="graphix/errors/erroriamge.gif"
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)response.redirect(strimagename) 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)% >
8.强迫输入密码对话框 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)把这句话放载页面的开头 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< % response.status="401 not Authorized"
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)response.end 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)% >
9.如何传递变量从一页到另一页 
答：用 HIDDEN 类型来传递变量 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< % form method="post" action="mynextpage.asp">
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< % for each item in request.form % >
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< input namee="< %=item% >" type="HIDDEN"
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)value="< %=server.HTMLEncode(Request.form(item)) % >">
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< % next % >
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)< /form >
10.为何我在 asp 程序内使用 msgbox，程序出错说没有权限 
答：由于 asp 是服务器运行的，如果可以在服务器显示一个对话框，那么你只好等有人按了确定之后，你的程序才能继续执行，而一般服务器不会有人守着，所以微软不得不禁止这个函数，并胡乱告诉你 (:) 呵呵) 没有权限。但是ASP和客户端脚本结合倒可以显示一个对话框，as follows:
来源：51js.com
