# 使用nice 属性 实现 ‘XX分钟前’ 日期显示 - z69183787的专栏 - CSDN博客
2014年03月05日 12:54:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3046
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)

javaeye的XX分钟前 羡慕已久 
今天打算自己实现 
初步打算改装struts2的s:date标签  
一经研究发现 s:date原来支持这个功能 
只需使用nice='true'属性即可 
该标签默认是英语显示时间 如 10 minutes ago 
用国际化资源文件了配置为中文 步骤为 
1 在strtus.xml中加 
Java代码 ![复制代码](http://xuliangyong.iteye.com/images/icon_copy.gif)
- <constant name="struts.custom.i18n.resources" value="globalMessages" />  
2 创建文件globalMessages_zh.txt globalMessages_zh_cn.properties 
globalMessages_zh.txt内容如下 
Java代码 ![复制代码](http://xuliangyong.iteye.com/images/icon_copy.gif)
- struts.date.format.past={0}前   
- struts.date.format.future={0}后   
- struts.date.format.seconds={0}秒   
- struts.date.format.minutes={0,choice,1#1分钟|1<{0} 分钟}
   
- struts.date.format.hours={0,choice,1#1小时|1<{0} 小时}
   
- struts.date.format.days={0,choice,1#1天|1<{0} 天}
   
- struts.date.format.years={0,choice,1#1 年|1<{0} 年}  
3 用native2ascii专程utf8编码 (MyEclepse新版的会自动转码)
ok打完收工 
对照表：
|i18n key|default|
|----|----|
|struts.date.format.past|{0} ago|
|struts.date.format.future|in {0}|
|struts.date.format.seconds|an instant|
|struts.date.format.minutes|{0,choice,1#one minute|1<{0} minutes}|
|struts.date.format.hours|{0,choice,1#one hour|1<{0} hours}{1,choice,0#|1#, one minute|1<, {1} minutes}|
|struts.date.format.days|{0,choice,1#one day|1<{0} days}{1,choice,0#|1#, one hour|1<, {1} hours}|
|struts.date.format.years|{0,choice,1#one year|1<{0} years}{1,choice,0#|1#, one day|1<, {1} days}|
- 
