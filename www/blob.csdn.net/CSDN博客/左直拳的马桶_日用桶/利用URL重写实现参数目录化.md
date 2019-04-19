# 利用URL重写实现参数目录化 - 左直拳的马桶_日用桶 - CSDN博客
2013年07月09日 18:34:25[左直拳](https://me.csdn.net/leftfist)阅读数：10838
参数目录化，就是将 类似 http://www.abc.com/store/store.aspx?id=1024 这样的网址，对外改为 http://www.abc.com/1024。
要实现这种功能，可以用以下三种方式（应该不仅限这三种吧！）
**1、用微软的 Microsoft URL 重写模块 2.0，但只能给IIS7使用，IIS6不行。**
64位：
[http://www.microsoft.com/downloads/zh-cn/details.aspx?familyid=1b8c7bd8-8824-4408-b8fc-49dc7f951a00](http://www.microsoft.com/downloads/zh-cn/details.aspx?familyid=1b8c7bd8-8824-4408-b8fc-49dc7f951a00)
32位：
[http://www.microsoft.com/zh-cn/download/details.aspx?id=5747](http://www.microsoft.com/zh-cn/download/details.aspx?id=5747)
**2、isapi_rewrite**
[http://www.helicontech.com/download-isapi_rewrite3.htm](http://www.helicontech.com/download-isapi_rewrite3.htm)
但完全版有日期限制，如果不想掏钱，有大牛破解了，可以用下面这个：
[http://download.csdn.net/detail/keke0307/3867086](http://download.csdn.net/detail/keke0307/3867086)
**3、urlrewriter.net**
这个有源代码，可以自己编译。
机缘巧合之下，我用了方法一 和 二。因为开发机器是WIN7，装了IIS7，所以用微软的重写模块；而服务器是WIN2003，就用了isapi_rewrite。
下面就这两种方法分别做一点心得介绍。
事实上，这两种方法大同小异，IIS基本上不用怎么配置，没有网上说的那么玄妙，又是勾选，又是映射，又是权限之类，关键在于要写对正则表达式。
**1、微软的 Microsoft URL 重写模块 2.0**
安装好之后，就可以改写网站下的web.config了。
```
<configuration>
  <system.web>
    <compilation debug="true" targetFramework="4.0"/>
    <httpRuntime/>
  </system.web>
  <system.webServer>
    <rewrite>
      <rules>
	<!-- 实现http://localhost/1024 ==> http://localhost/store/store.aspx?id=1024 -->
        <rule name="storecode">
          <match url="^([1-9][0-9]*)/?$" ignoreCase="true"/>
          <action type="Rewrite" url="/store/store.aspx?id={R:1}"/>
        </rule>
	<!-- 实现http://localhost/1024/p=1&c=1 ==> http://localhost/store/store.aspx?id=1024&p=1&c=1 -->
        <rule name="storecode with param">
          <match url="^([1-9][0-9]*)/([^/]+)/?$" ignoreCase="true"/>
          <action type="Rewrite" url="/store/store.aspx?id={R:1}&{R:2}"/>
        </rule>
	<!-- 下面这些据说是从Discuz论坛里导出来的，肯定是正确的，供学习借鉴 -->
        <!--
                <rule name="已导入的规则 1">
                    <match url="^userinfo-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="userinfo.aspx?userid={R:1}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 2">
                    <match url="^showforum-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="showforum.aspx?forumid={R:1}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 3">
                    <match url="^showtopic-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="showtopic.aspx?topicid={R:1}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 4">
                    <match url="^showforum-([0-9]+)-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="showforum.aspx?forumid={R:1}&page=
{R:2}" appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 5">
                    <match url="^showtopic-([0-9]+)-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="showtopic.aspx?topicid={R:1}&page=
{R:2}" appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 6">
                    <match url="^archiver/showforum-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="archiver/showforum.aspx?forumid={R:1}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 7">
                    <match url="^archiver/showtopic-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="archiver/showtopic.aspx?topicid={R:1}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 8">
                    <match url="^archiver/showtopic-([0-9]+)-([0-9]+)\.html$" 
ignoreCase="false" />
                    <action type="Rewrite" url="archiver/showtopic.aspx?topicid={R:1}
&page={R:2}" appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 9">
                    <match url="^archiver/showforum-([0-9]+)-([0-9]+)\.html$" 
ignoreCase="false" />
                    <action type="Rewrite" url="archiver/showforum.aspx?forumid={R:1}
&page={R:2}" appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 10">
                    <match url="^tools/rss-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="tools/rss.aspx?forumid={R:1}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 11">
                    <match url="^tools/spacerss-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="tools/rss.aspx?uid={R:1}&type=space" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 12">
                    <match url="^tools/photorss-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="tools/rss.aspx?uid={R:1}&type=photo" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 13">
                    <match url="^space\/((\w|\s)+)((\/?))?$" ignoreCase="false" />
                    <action type="Rewrite" url="space/index.aspx?user={R:1}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 14">
                    <match url="^space\/((\w|\s|-)+)((\/?))?\?((.*)+)$" ignoreCase="false" 
/>
                    <action type="Rewrite" url="space/index.aspx?user={R:1}&{R:5}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 15">
                    <match url="^showdebate-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="showdebate.aspx?topicid={R:1}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 16">
                    <match url="^showbonus-([0-9]+)\.html$" ignoreCase="false" />
                    <action type="Rewrite" url="showbonus.aspx?topicid={R:1}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 17">
                    <match url="^postgoods-(\d+)*.html$" ignoreCase="false" />
                    <action type="Rewrite" url="postgoods.aspx?categoryid={R:1}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 18">
                    <match url="^showgoodslist-(\d+)(-(\d+))?.html$" ignoreCase="false" />
                    <action type="Rewrite" url="showgoodslist.aspx?categoryid={R:1}
&page={R:3}" appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 19">
                    <match url="^showgoods-(\d+)*.html$" ignoreCase="false" />
                    <action type="Rewrite" url="showgoods.aspx?goodsid={R:1}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 20">
                    <match url="^install((\/?))?$" ignoreCase="false" />
                    <action type="Rewrite" url="/install{R:1}" />
                </rule>
                <rule name="已导入的规则 21">
                    <match url="^upgrade((\/?))?$" ignoreCase="false" />
                    <action type="Rewrite" url="/upgrade{R:1}" />
                </rule>
                <rule name="已导入的规则 22">
                    <match url="^aspx/([0-9]+)((\/?))?$" ignoreCase="false" />
                    <action type="Rewrite" url="/aspx/{R:1}{R:2}" />
                </rule>
                <rule name="已导入的规则 23">
                    <match url="^archiver((\/?))?$" ignoreCase="false" />
                    <action type="Rewrite" url="/archiver{R:1}" />
                </rule>
                <rule name="已导入的规则 24">
                    <match url="^space((\/?))?$" ignoreCase="false" />
                    <action type="Rewrite" url="/space{R:1}" />
                </rule>
                <rule name="已导入的规则 25">
                    <match url="^doc((\/?))?$" ignoreCase="false" />
                    <action type="Rewrite" url="/doc{R:1}" />
                </rule>
                <rule name="已导入的规则 26">
                    <match url="^(([\w|\-|_])+)((\/?))?$" ignoreCase="false" />
                    <action type="Rewrite" url="/{R:1}/list.aspx" />
                </rule>
                <rule name="已导入的规则 27">
                    <match url="^(([\w|\-|_])+)((\/?))?\?(.)*$" ignoreCase="false" />
                    <action type="Rewrite" url="/{R:1}/list.aspx?{R:2}" 
appendQueryString="false" />
                </rule>
                <rule name="已导入的规则 28">
                    <match url="^(([\w|\-|_])+)/([0-9]+)((\/?))?$" ignoreCase="false" />
                    <action type="Rewrite" url="/{R:1}/{R:3}/list.aspx" />
                </rule>
                <rule name="已导入的规则 29">
                    <match url="^(([\w|\-|_])+)/([0-9]+)((\/?))?\?(.)*$" ignoreCase="false" 
/>
                    <action type="Rewrite" url="/{R:1}/{R:3}/list.aspx?{R:4}" 
appendQueryString="false" />
                </rule>
-->
      </rules>
    </rewrite>
  </system.webServer>
</configuration>
```
关键是要**写对正则表达式**，否则系统会报无法找到资源的404错误。真他妈的。就不能报点别的错误？害得我在网上疯狂搜索，以为差了哪里没配置，还差点将IIS重装了。网上也不会告诉你这个答案，他们只是建议将文件是否存在的选项去掉，诸如此类。
## HTTP 错误 404.0 - Not Found
### 您要找的资源已被删除、已更名或暂时不可用。
同一站内的，就用Rewrite，目标页面在另一站点的，则用 Redirect。如
```
<rule name="img show">
              <match url="^showimg/([a-zA-Z_0-9]+)/([1-9][0-9]*)/?$" ignoreCase="true"/>
              <action type="Redirect" url="http://localhost:8006/handler.ashx?{R:1}={R:2}"/>
          </rule>
```
2、isapi_rewrite
安装之后，就可以直接使用了。打开IIS6，点击网站-属性，会看到：
![](https://img-blog.csdn.net/20130709175338359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点 EDIT，会在网站根目录下生成一个.htaccess文件。在里面编写我们的正则表达式：
```
# Helicon ISAPI_Rewrite configuration file
# Version 3.1.0.87
RewriteRule ^([1-9][0-9]*)/eurl\.axd/  /store/store.aspx\?id=$1 [NC]
RewriteRule ^([1-9][0-9]*)/([^/]*)?/? /store/store.aspx\?id=$1&$2 [NC]
```
其正则表达式的语法，似乎跟微软的URL重写模块没有多大区别，并且它还自带了一个正则表达式测试器，打开.htaccess文件的时候，在编辑器菜单里可以看到，RegExp test：
![](https://img-blog.csdn.net/20130709181114000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20130709181334750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后IIS6还要将Isapi_rewrite.dll作为筛选器器
![](https://img-blog.csdn.net/20130730155435609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但我发现有以下几点需要注意：
1、正则表达式里，不能出现问号(“\?”)，否则一定匹配失败，但正则表达式绝对正确，放在其他正则表达式测试器里测试，也OK，但在这里就不行。不知道什么原因。翻遍互联网，有个兄弟说：
*这是因为 url 里面 ? 后面的不被 apache 认为是 requested uri 的内容，所以你怎么写都无法取得。建议你修改 url 格式：比如把 ? 去掉；或者改变获取方式：比如改成 RewriteRule /?(.*)$ /reg.php?id=$1&%*
帖子：[http://bbs.csdn.net/topics/300052355](http://bbs.csdn.net/topics/300052355)
但我这个是IIS耶！真搞不懂，不管了。所以我原本想将优化后的地址设为：http://www.abc.com/1024?p=1&c=1的，结果怎么试都不成功，最后只能放弃问号，改成：http://www.abc.com/1024/p=1&c=1
2、IIS6 + .net 4的情况下，IIS会自动在无后缀（什么叫无后缀？不懂）地址后加上 "/eurl.axd/guid号"，刚好跟我机器一样。本来这个在ASP.NET处理过程中，会被自动去掉，但在这里，经过转向后，居然还保留了下来，我在测试过程中，在目标页面显示 Request.QueryString，居然可耻地显示出这种不速之客：
![](https://img-blog.csdn.net/20130709183111562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
网上有文章介绍用修改注册表的方式来屏蔽
[http://www.admin10000.com/document/1244.html](http://www.admin10000.com/document/1244.html)
但我想着以后可能会采用MVC模式，所以只在写正则表达式时将这个因素考虑进去了：
RewriteRule ^([1-9][0-9]*)/eurl\.axd/  /store/store.aspx\?id=$1 [NC]
3、每条规则后面必要时要带上适当的flag。
isapi_rewrite的RewriteRule就是后面常常带上一些古怪的标识，比如[NC]、[L]之类，它们是干什么的？
其实它们很有用，比如：
[NC]，忽略大小写
[L]，在一连串的匹配规则中，当系统找到第一个匹配的规则后，就不再往下匹配其他规则了。这个特别有用，比如：
我有1、2、3三条规则，有个输入情况按照我的设想，是应该规则2起作用，最为匹配，但规则3是规则2的超集，同样也可以匹配。如果规则2后面没有这个[L]标识，那么起作用的，会是规则3！
[R]，立即转向，比如将某个输入，Redirect到别的网站或网址啦，使用这个标识，我感觉速度会快一点。
还有许多标识(flag)，可以查阅这里：
[http://www.helicontech.com/isapi_rewrite/doc/RewriteRule.htm](http://www.helicontech.com/isapi_rewrite/doc/RewriteRule.htm)
是英文的，但作为一个计算机程序员，我们还是要习惯阅读英文文档，这有时比直接阅读那些翻译得奇形怪状的中文文档还好理解。有时候我觉得老外编程那么厉害，常常提出这思想那思想，这模式那模式，是因为他们阅读、交流没有什么障碍，接受东西就比我们快。我们吃了这个亏。
我自己的英文非常烂，很不习惯读英文文档，但从今往后，要慢慢习惯了。与猪君共勉！
