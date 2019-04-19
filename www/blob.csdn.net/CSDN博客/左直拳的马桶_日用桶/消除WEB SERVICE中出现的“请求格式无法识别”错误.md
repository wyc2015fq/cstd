# 消除WEB SERVICE中出现的“请求格式无法识别”错误 - 左直拳的马桶_日用桶 - CSDN博客
2006年05月09日 18:19:00[左直拳](https://me.csdn.net/leftfist)阅读数：3505
消除WEB SERVICE中出现的“请求格式无法识别”错误
左直拳
在本地机器访问得好好的web service，拿到别的机器上安装就发生了错误。错误信息提示为：请求格式无法识别。
奇了怪了。跑到网上进行搜索，有高手指点迷津：
在fromework1.0和1.1是有区别的,
在1.1的 machine.config （C:/WINNT/Microsoft.NET/Framework/v1.1.4322/CONFIG）,默认webservice去除了post,get方法
你可在你的 machine.config 中加上或者只在 webServices 项目的 Web.config 的<system.web>里加上：
<webServices>
<protocols>
<add name="HttpPost" />
<add name="HttpGet" />
</protocols>
</webServices>
否则，在ie地址栏输入 webServices 访问地址串时，提示：请求格式无法识别。
（摘自[风驰电掣, miuo](http://blog.csdn.net/cyp403/)，（作者：[李天平](http://blog.csdn.net/litp)转载请注明））
幸甚至哉。
