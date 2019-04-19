# 【zz】免费的午餐——编程利用Google日历API发短信、Email - 三少GG - CSDN博客
2012年06月02日 13:16:07[三少GG](https://me.csdn.net/scut1135)阅读数：3760
[免费的午餐——编程利用Google日历API发短信、Email](http://www.cnblogs.com/SkyD/archive/2009/07/23/1529037.html)
![image](http://images.cnblogs.com/cnblogs_com/SkyD/WindowsLiveWriter/GoogleAPIEmail_3B7/image_105a4278-b4bd-4915-8827-e3c7fb8f90b3.png) Google日历是相当优秀的日程管理工具，可以方便的组织、分享各类日程，Google还提供了贴心而免费的手机短信、Email提示功能，真是非常的赞。
此外Google还公开了其API，使得我们可以通过各种语言编程访问。
面向.Net的API文档：[http://code.google.com/apis/calendar/docs/2.0/developers_guide_dotnet.html](http://code.google.com/apis/calendar/docs/2.0/developers_guide_dotnet.html)
开源类库：[http://code.google.com/p/google-gdata/](http://code.google.com/p/google-gdata/)
下面我们就要利用此类库，及Google日历的免费短信、Email提醒功能，实现程序控制提醒。
## **准备工作**
首先你得有一个Google帐号，并开启了[日历功能](http://www.google.com/calendar)，然后在设置中配置好手机短信功能：
![image](http://images.cnblogs.com/cnblogs_com/SkyD/WindowsLiveWriter/GoogleAPIEmail_3B7/image_952f44d3-b6b4-41c8-a11c-3319fb684555.png)
然后配置你要使用的日历，设置默认支持的通知方式（一个日历帐户下，可以创建或导入多个日历，每个日历的提醒策略都需要单独配置）：
![image](http://images.cnblogs.com/cnblogs_com/SkyD/WindowsLiveWriter/GoogleAPIEmail_3B7/image_ffb5a10b-8b4d-400a-903f-527722b1e06f.png)
这样就OK了。
## **通过代码添加活动并用短消息、邮件提醒**
首先要按这篇[教学文章](http://code.google.com/apis/gdata/articles/dotnet_client_lib.html)的指导，安装类库，然后在项目中引入相关的DLL：
Google.GData.AccessControl.dll
Google.GData.Extensions.dll
Google.GData.Client.dll
Google.GData.Calendar.dll
然后编写代码：
string calendarURI 
="http://www.google.com/calendar/feeds/default/allcalendars/full";
string userName 
="abcdef@gmail.com";
string passWord 
="123321";
//创建日历服务对象
CalendarService service 
=newCalendarService("CalendarTestApp");
//设置用户名密码
service.setUserCredentials(userName, passWord);
//查询找到账户中的名为“测试”的日历
var query 
=newCalendarQuery(calendarURI);
CalendarEntry c 
=null;
foreach (CalendarEntry fin service.Query(query).Entries)
{
if (f.Title.Text=="测试") c= f;
}
//构建新的活动
var q 
=newEventEntry("测试活动标题","测试活动说明\n测试\n测试","北京市朝阳区中央电视台总部大楼大裤衩左裤管下");
q.Times.Add(newWhen(DateTime.Now.AddMinutes(8),DateTime.Now.AddDays(3)));
//设置提前提醒时间及方式
q.Reminder 
=newReminder { Minutes=7, Method=
Reminder.ReminderMethod.all };
//向日历添加此活动
service.Insert(newUri(c.Content.AbsoluteUri), q);
执行此段代码，即可向一个名为“测试”的日历中，添加一个带有全部提醒功能的活动，你将会在1分钟后收到短消息、Email提醒。
除此之外，我还简单封装了一个辅助类，以简化整体操作流程，其发送过程是这样的：
![image](http://images.cnblogs.com/cnblogs_com/SkyD/WindowsLiveWriter/GoogleAPIEmail_3B7/image_0e3dd784-30a1-454e-b387-2070170c63ad.png)
是的，我又使用了神圣的中文来编写代码，我将在文章末尾处提供其源代码及DLL下载，类非常简单，如果你不喜欢中文可以自行修改一下命名。 
注意，那个1.2分钟的提前提醒时间，提交后还是会被认为是1分钟，因为这个参数不支持分钟以下的单位。
## **创建的活动及其发送通知的效果**
执行后查看日历，活动就被添加进来了：
![image](http://images.cnblogs.com/cnblogs_com/SkyD/WindowsLiveWriter/GoogleAPIEmail_3B7/image_192ceabe-e594-46c2-b9c2-bbff04ba883c.png)
1分钟过后，来看一下接收到的提醒吧，这是短信提醒：
![image](http://images.cnblogs.com/cnblogs_com/SkyD/WindowsLiveWriter/GoogleAPIEmail_3B7/image_9f93cd88-fe20-407c-85b9-96b401aee4ff.png)
短信提醒中只包含标题和地点两个信息属性内容，没有说明部分。
这是电子邮件提醒：
![image](http://images.cnblogs.com/cnblogs_com/SkyD/WindowsLiveWriter/GoogleAPIEmail_3B7/image_22fca867-772f-4a0a-82ca-5b48314a7884.png)
## **想想这能干些什么？**
新邮件短信通知：有些专业邮箱有新邮件短信提醒功能，那么我们只要通过程序定期检查邮箱，然后将新邮件信息用此方法提交，就可以实现免费的短信提醒功能啦，嘿嘿。（注意设置提醒的时候只设短信提醒，别把邮件提醒也弄上了，那Google就被你弄进死循环了～）
天气预报通知：不怕气象局见云改口，随时关注实时天气动向，比每天一次的固定天气预报短信服务要好。
网站重要状况通知：网站管理员都希望网站出现重要事件或异常时能收到邮件通知，而现在还能追加一个短信通知，这样就可以近乎实时了解自己网站的状况啦。不仅如此，Google的日历还支持分享，假如有多个管理员的话，就可以通过分享功能共享这一个服务器状况日历，一有突发事件，所有管理员都将收到短信和邮件通知！简直就是太棒了。（别被DDOS攻击的时候把移动服务器也发爆了～Google短信账户赤字～）
RSS更新通知：让RSS阅读器追加上这个功能一定会备受好评哦。
计算机操作监控通知：不在办公室时有没有人偷偷开我电脑？弄个自动运行的程序在开机时发出通知就好，可以顺道把他打开什么程序、读了什么文件也都给记下来，呵呵。
行情走势变动通知：可以通过程序监测某些易波动的商品报价信息，在变化时发出通知，股票、外汇、黄金什么的也适用。“Wroning！胡萝卜大跌！油麦菜涨停。”
团队消息通知：由软件输入并发送消息，继而通过日历共享功能群发短信给团队，省钱的小招：）
游戏外挂通知：“你被一群牛头人踩死了！”“GM正在注视着你！”“与服务器断开连接！”
Twitter发言通知：用程序关注你的目标MM的Twitter，她一说话你就收到短信，然后第一时间回复她吧，那感情持续++。（使用前请确定目标不会在激动时刷版狂喷）
我想到的就这么多了，应该还能发掘出很多不错的点子的，大家一起来想吧。
但是千万别滥用Google好心提供的这个服务，你被封了不要紧，就怕会影响别人的正常使用。
下载源代码：
![](http://msc.wlxrs.com/lQNgXBQYoayUK1wMOhAgVw/images/icons/Large/Zip.png)
下载本文的XPS版本：
![](http://msc.wlxrs.com/lQNgXBQYoayUK1wMOhAgVw/images/icons/Large/Xps.png)
**源代码分流下载地址：**[http://www.uushare.com/user/icesee/file/1822466](http://www.uushare.com/user/icesee/file/1822466)
转载请遵循此协议：[署名 - 非商业用途 - 保持一致](http://creativecommons.org/licenses/by-nc-sa/3.0/deed.zh)
并保留此链接：[http://skyd.cnblogs.com/](http://skyd.cnblogs.com/)

+++++++++++++++++++++++++++++++++++++++++++
## 通过Google Calendar(Google日历)免费获取天气预报短信提醒
2010-11-22
分享到：QQ空间新浪微博腾讯微博人人网开心网邮件搜狐微博豆瓣猜你喜欢0
准备工作:
1.有一个Google帐户.
2.有一部中移动或联通的手机. ([Google SMS支持的移动服务提供商](http://www.google.com/support/calendar/bin/answer.py?hl=cn&answer=37226))
3.将手机与[Google日历](https://www.google.com/accounts/ServiceLogin?service=cl&passive=1209600&continue=http://www.google.com/calendar/render&followup=http://www.google.com/calendar/render)将行绑定. ([如何进行绑定](http://www.google.com/support/calendar/bin/answer.py?hl=cn&answer=45351))
4.访问[免费](http://www.yulaoda.com/tag/%E5%85%8D%E8%B4%B9)[天气预报](http://www.yulaoda.com/tag/%E5%A4%A9%E6%B0%94%E9%A2%84%E6%8A%A5)系统[http://weather.raychou.com](http://weather.raychou.com/),
 在搜索框中输入欲获取[免费](http://www.yulaoda.com/tag/%E5%85%8D%E8%B4%B9)[天气预报](http://www.yulaoda.com/tag/%E5%A4%A9%E6%B0%94%E9%A2%84%E6%8A%A5)的城市名称,拼音或汉字都可以,页面会自动跳转到天气详情页面。如果搜索到的结果不正确（有部分城市的拼音相同），还可通过搜索框下面的城市列表选择.
![image](http://www.yulaoda.com/wp-content/uploads/2010/11/image_thumb.png)
5.搜索页面下方,点击模板”[Google日历](http://www.yulaoda.com/tag/google%E6%97%A5%E5%8E%86)的短信提醒”,最下面的URL就是我们想要的自定义数据源的网址.
![image](http://www.yulaoda.com/wp-content/uploads/2010/11/image_thumb7.png)
现在[免费](http://www.yulaoda.com/tag/%E5%85%8D%E8%B4%B9)[天气预报](http://www.yulaoda.com/tag/%E5%A4%A9%E6%B0%94%E9%A2%84%E6%8A%A5)的数据源部分已经搞定,接下来将其添加到[Google日历](http://www.yulaoda.com/tag/google%E6%97%A5%E5%8E%86)中,并设置短信提醒!
6.登陆[Google日历](https://www.google.com/accounts/ServiceLogin?service=cl&passive=1209600&continue=http://www.google.com/calendar/render&followup=http://www.google.com/calendar/render),点击页面左下角的"添加"在弹出菜单中选择"通过URL添加".
![image](http://www.yulaoda.com/wp-content/uploads/2010/11/image_thumb2.png)![image](http://www.yulaoda.com/wp-content/uploads/2010/11/image_thumb3.png)
在添加页面将之前得到的免费天气预报的数据源填入即可.若提示无法获取网址或其他错误,删除数据源再重复添加即可.
9.在得到页面提示添加成功后,点击左下角的"设置",再点击刚才添加的日历后的"通知"链接,转入日历通知设置。
![image](http://www.yulaoda.com/wp-content/uploads/2010/11/image_thumb4.png)
10.在通知设置中添加通知,选择通知的接受方式和通知方式,如图所示.
![image](http://www.yulaoda.com/wp-content/uploads/2010/11/image_thumb5.png)
根据自己的需要选择通知方式及通知时间.
点击保存,所有的工作就都完成了!
更多设置选项及应用!
![image](http://www.yulaoda.com/wp-content/uploads/2010/11/image_thumb6.png)
0
![正在加载推荐文章](http://static.wumii.com/images/loading.gif)
