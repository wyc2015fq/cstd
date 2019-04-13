
# 自定义 Discuz 样式 - 阳光岛主 - CSDN博客

2013年10月05日 03:15:05[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：13942


本文论坛示例，请访问
IT-Homer 论坛：[http://forum.ithomer.net](http://forum.ithomer.net)


**1、删除注册“会员”**
由于论坛开始，注册会员较少，且会员数是论坛敏感隐私数据，
![](https://img-blog.csdn.net/20131005024732468)

一般不要对外显示，因此需要删除注册会员数，步骤
discuz根目录——》template——》default——》forum——》discuz.htm
查找“<span class="pipe"> |</span>{lang index_members}: <em>$_G['cache']['userstats']['totalmembers']</em>”，并注释掉，如下图
![](https://img-blog.csdn.net/20131005024916671)

修改后，结果：
![](https://img-blog.csdn.net/20131005024958953)


**2、删除title标题上的“- Powered by Discuz!”**
为了尊重版本，discuz在标题（title）上，一般会加上“**- Powered by Discuz!**”，如下图：
![](https://img-blog.csdn.net/20131005030422062)

但是出于美观的考虑，需要把它删除，步骤如下：
打开./template/default/common/header_common.htm，查找 “<title><!--{if !empty($navtitle)}-->$navtitle-<!--{/if}--><!--{if empty($nobbname)}--> $_G['setting']['bbname']**-**<!--{/if}-->Powered by Discuz!</title>”，将红色部分删除，即删除两个横杠“-”和“**Powered by Discuz!**”，不要删除<!--{/if}-->如下图：
![](https://img-blog.csdn.net/20131005030654828)
删掉上图后线部分，标题（title）的“- Powered by Discuz!”字样消除，如下图：
![](https://img-blog.csdn.net/20131005093550015)
“排行榜”有时会出现“站点“名，去掉方法是把”$_G['setting']['bbname']“删掉


**3、删除坐下角“Powered by Discuz! X2”字样**
discuz安装后，页脚左右两侧都会有版权信息，如下图：
![](https://img-blog.csdn.net/20131005103735375)

为了美化，可以删除，依次打开./template/default/common/footer.htm
1) 搜索”<p>Powered by <strong><a href="http://www.discuz.net" target="_blank">Discuz!</a></strong>“，修改或删除discuz版权信息
2) 搜索”Processed in $_G[debuginfo][time] second(s)“，精简或去掉右下角第二行的时间栏信息，注释修改如下图：
![](https://img-blog.csdn.net/20131005102658093)

修改后版式结果如下：
![](https://img-blog.csdn.net/20131005102929828)


**4、Discuz手机版删除版权、替换logo**
discuz安装后，默认手机版也有discuz版权信息，跟PC端类似，体现在title、logo、页脚等处，如下图：
![](https://img-blog.csdn.net/20131005151620625)

1） 删除title版权
类似PC端删除title，依次进入手机版的：.\template\default\mobile\common\header.htm，删除红线标记部分，如下图：
![](https://img-blog.csdn.net/20131005152109906)

2） 替换logo
依次进入手机版：./static/image/mobile/logo.gif，删除logo.gif，替换上自己logo.gif，如下图：
![](https://img-blog.csdn.net/20131005154452531)

3） 删除页脚discuz
类似CP端页脚或手机端title，依次进入手机版：.\template\default\mobile\common\footer.htm，注释掉红线标记部分，如下图：
![](https://img-blog.csdn.net/20131005152625656)

手机版，修改上面三处后的效果，请用手机访问手机版：[http://iforum.sinaapp.com/](http://iforum.sinaapp.com/)
![](https://img-blog.csdn.net/20131005155107968)


**5、隐藏在线会员图标**
discuz安装后，默认会显示会员图标，如下图：
![](https://img-blog.csdn.net/20131005111522562)
上图看上去，很不美观，且管理员、超级版主等属于论坛隐私信息，一般不对外显示，因此需要隐藏，步骤如下：
管理中心——》界面——》页面设置——》论坛首页——》缩略显示在线列表:——》勾选”是“（默认是否），如下图
![](https://img-blog.csdn.net/20131005111847734)

勾选”是“打开此选项后，将会只显示缩略信息，隐藏详情，效果如下：
![](https://img-blog.csdn.net/20131005112126500)
不过，这样点击右侧的展开按钮，仍然会出现在线会员的详细信息，彻底不显示的方法如下：
管理中心——》界面——》页面设置——》论坛首页——》显示在线用户——》选择”空白“，如下图
![](https://img-blog.csdn.net/20131005113113515)
这样，在论坛的任何地方，都不会再显示”在线会员“信息了


**6、论坛版块排版修改**
discuz论坛版块，默认是横排单列，如下：
![](https://img-blog.csdn.net/20131005104431062)

想把它修改成横排三列，步骤如下：
管理中心——》论坛——》版块管理——》选中你要修改的版块，如”博客专题“——》点击”编辑“，如下图：
![](https://img-blog.csdn.net/20131005104739781)

如下图，按照右侧提示，修改”论坛首页下级子版块横排:“为3
![](https://img-blog.csdn.net/20131005105024453)

提交保存，修改后的效果，如下图：
![](https://img-blog.csdn.net/20131005105309125)

整体论坛版块样式，请参见我在SAE搭建的论坛：[iforum](http://iforum.sinaapp.com/)


**7、论坛css样式有时还会乱且论坛发帖编辑栏看不清图标**
![](https://img-blog.csdn.net/20131007020034734)
css错乱和图标看不清，是由于SAE Strorage缓存和路径不对引起的，解决方法是把css的绝对路径修改成相对路径，分析解决步骤如下：
1） css绝对路径，如“[http://iforum-discuzx.stor.sinaapp.com/data/cache/style_1_common.css](http://iforum-discuzx.stor.sinaapp.com/data/cache/style_1_common.css)”修改成相对路径“[data/cache/style_1_common.css](http://iforum-discuzx.stor.sinaapp.com/data/cache/style_1_common.css)”
绝对路径“[http://iforum-discuzx.stor.sinaapp.com/data/cache/style_1_common.css](http://iforum-discuzx.stor.sinaapp.com/data/cache/style_1_common.css)”， 请见我先前写的discuz安装博客[SAE 搭建 Discuz](http://blog.csdn.net/ithomer/article/details/12277065)——》6、 Discuzx 安装——》11）
修改成相对路径，请把[Storage](http://sae.sina.com.cn/?m=storage&&app_id=iforum)»[discuzx](http://sae.sina.com.cn/?m=storage&a=managestor&app_id=iforum&domName=discuzx)»[data](http://sae.sina.com.cn/?m=storage&a=managestor&app_id=iforum&domName=discuzx&last=data)»[cache](http://sae.sina.com.cn/?m=storage&a=managestor&app_id=iforum&domName=discuzx&last=data%2Fcache)» 下的所有js和css拷贝到代码管理——》编辑代码——》data/cache/目录下，如下图：
![](https://img-blog.csdn.net/20131006225953515)
修改如下两处链接：
（1）打开./config/config_global.php注释掉，修改如下：
define('SAE_STORAGE_DOMAIN_NAME','discuzx');
//define('SAE_STORAGE_DOMAIN','http://'.$_SERVER['HTTP_APPNAME'].'-'.SAE_STORAGE_DOMAIN_NAME.'.stor.sinaapp.com/');// 注释掉
define('SAE_STORAGE_DOMAIN','.');// 修改后的
（2） 打开./source/class/class_template.php注释掉，修改如下：

```python
// <link rel="stylesheet" type="text/css" href="data/cache/style_1_css_space.css?WuV" />
        //$template = str_replace('href="data/cache/','href="http://'.$_SERVER['HTTP_APPNAME'].'-'.SAE_STORAGE_DOMAIN_NAME.'.stor.sinaapp.com/data/cache/',$template);
		//$template = str_replace('href=\'data/cache/','href=\'http://'.$_SERVER['HTTP_APPNAME'].'-'.SAE_STORAGE_DOMAIN_NAME.'.stor.sinaapp.com/data/cache/',$template);
		//$template = str_replace('src="data/cache/','src="http://'.$_SERVER['HTTP_APPNAME'].'-'.SAE_STORAGE_DOMAIN_NAME.'.stor.sinaapp.com/data/cache/',$template);
		//$template = str_replace('src=\'data/cache/','src=\'http://'.$_SERVER['HTTP_APPNAME'].'-'.SAE_STORAGE_DOMAIN_NAME.'.stor.sinaapp.com/data/cache/',$template);
		$template = str_replace('href="data/cache/','href="data/cache/',$template);
		$template = str_replace('href=\'data/cache/','href=\'data/cache/',$template);
		$template = str_replace('src="data/cache/','src="data/cache/',$template);
		$template = str_replace('src=\'data/cache/','src=\'data/cache/',$template);
```

修改后的代码，相对路径如下：
![](https://img-blog.csdn.net/20131006230215765)

发帖编辑栏的图标也出现了，如下：
![](https://img-blog.csdn.net/20131006230322265)
更多真实的效果体验，请见我的论坛：[http://iforum.sinaapp.com/](http://iforum.sinaapp.com/)


**8、修正提示内容，明确的引导用户**
注册discuz论坛后，无法看帖，也无法发帖和跟帖，弹出提示框如下：
![](https://img-blog.csdn.net/20131007052122562)

看了这，用户也会迷惑，不知所措，因此需要明确提示用户，引导他去邮件验证、激活，修改提示步骤：
打开 source/language/lang_message.php，查找 ”group_nopermission“，如下图：
![](https://img-blog.csdn.net/20131007052707281)
这是由于discuz论坛设置了“邮箱认证”，需要先进入邮箱验证激活，才能看帖、发帖和跟帖，修后提示后的效果：
![](https://img-blog.csdn.net/20131007053119515)


9、删除友情链接
discuz 默认安装后，左下角会有一个discuz友情链接，如下图
![](https://img-blog.csdn.net/20141217142214026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRob21lcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

友情链接很不友情，也很不美观，那么如何去掉友情链接呢，方法如下：
**方法1)**后台管理 ——》 运营 ——》 友情链接 ——》 删除四个选项
![](https://img-blog.csdn.net/20141217142239969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRob21lcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**方法2)**界面 ——》 界面设置 ——》 论坛首页 ——》 ”显示友情链接“ ——》 选择”否“
![](https://img-blog.csdn.net/20141217142253931?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRob21lcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**10、绑定微博账号**
升级插件下载：[Xweibo for Discuz! X2.0 V2.1.6](http://down.chinaz.com/soft/29905.htm#down)
升级安装教程：[Xweibo For Discuz! X1.5/X2升级到v2.1.6（V2接口）教程](http://bbs.x.weibo.com/forum.php?mod=viewthread&tid=136174)


郑重声明：
本文仅供学习交流，示例论坛请见[forum.ithomer.net](http://forum.ithomer.net)
为了保护软件著作权，修改后也请尊重作者（公司）的版权信息，谢谢。



**参考推荐：**
[怎样不显示discuz注册会员数](http://zhidao.baidu.com/link?url=QpZIWW5wS9nZThjxUhN1bQRLivieSjuQaB74L9SQ-vwWa7IpMWO6a1G4pzyH55KvVSudx-OUg9bg5_5AyIUqFq)
[去掉Powered by Discuz! X2.5这个东西](http://www.cncuz.com/thread-30-1-1.html)



