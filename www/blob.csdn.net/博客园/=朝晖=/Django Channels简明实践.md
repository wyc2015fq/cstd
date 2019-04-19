# Django Channels简明实践 - =朝晖= - 博客园
# [Django Channels简明实践](https://www.cnblogs.com/dhcn/p/7124892.html)
1、安装，如果你已经安装django1.9+，那就不要用官方文档的安装指令了，把-U去掉，直接用：
- sudo pip install channels  
2、自定义的普通Channel的名称只能包含a-z A-Z 0-9 - _ 这些字母，这样就可以和内建Channel完全区别开，内建channel是带点号.的
3、其搭建真是非常简单，安装完成后，在setiings里面加两个地方，一是在Install apps配置处添加channels app如下：
- INSTALLED_APPS = (  
- 'django.contrib.auth',  
- 'django.contrib.contenttypes',  
- 'django.contrib.sessions',  
- 'django.contrib.sites',  
-     ...  
- 'channels',  
- )  
再就是添加Channel_Layer配置：
- # In settings.py  
- CHANNEL_LAYERS = {  
- "default": {  
- "BACKEND": "asgiref.inmemory.ChannelLayer",  
- "ROUTING": "myproject.routing.channel_routing",  
-     },  
- }  
然后配置好你自己的myproject.routing.channel_routing就ok了。
4、我目前这个项目使用channels做后端异步后台任务，很多WebSocket方面的group之类的特性基本用不到，不过Channels这个东西的逻辑实例只能支撑几千并发，所以我建议实时[大数据](http://lib.csdn.net/base/hadoop)量并发程序还是考虑[Node.js](http://lib.csdn.net/base/nodejs)或者[Go](http://lib.csdn.net/base/go)，当然channels还是可以做一些需要WebSocket来实现的普通业务操作，毕竟它还是有一定优势的：和http服务的高整合度。

