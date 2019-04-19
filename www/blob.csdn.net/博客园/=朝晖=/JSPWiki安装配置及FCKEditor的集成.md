# JSPWiki安装配置及FCKEditor的集成 - =朝晖= - 博客园
# [JSPWiki安装配置及FCKEditor的集成](https://www.cnblogs.com/dhcn/p/7100172.html)
   版本:JSPWiki-2.8.2
             FCKeditor_2.6.3
     安装方法我参照：[http://doc.jspwiki.org/2.4/wiki/InstallingJSPWiki](http://doc.jspwiki.org/2.4/wiki/InstallingJSPWiki)
     FCKEditor的集成：[http://www.jspwiki.org/wiki/HowToManuallyIntegrateFCKEditor](http://www.jspwiki.org/wiki/HowToManuallyIntegrateFCKEditor)
     我就说一下我遇到的问题，就是JSPWiki的BaseURL的配置，我刚开始以为是应用域后来看了jspwiki.properties里面这个项设置前面有这么一句话：
     #  Example:
     #  jspwiki.baseURL = http://www.jspwiki.org/
     所以在本地测试的时候，甚至自己得把自己的IP先写在前面。

