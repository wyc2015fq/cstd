# flarum论坛系统搭建教程 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月24日 13:38:48[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：509
安装
 PHP 5.5+ and MySQL 5.5+, and install [Composer](https://getcomposer.org/).
composer create-project flarum/flarum . --stability=beta
配置nginx
chmod -R 775 root
chmod -R 775 root/assets 
chmod -R 775 root/storage
好像777才行
访问地址按页面说明安装
相关问题
1，创建版块
Primary Tags ：版块，分类
Secondary Tags：标签，关键词
默认创建的都是Secondary 标签，可以拖动成为Primary版块，或者拖动生成子板块
这2个在前端页面左侧导航栏中没有区别，看起来是一样的
2，关闭邮件激活
修改users表的is_activeated默认值为1
3，公告Banner怎么搞超链接
webrtc官方网站：<a href="https://webrtc.org" target="_blank">https://webrtc.org</a>
[https://flarum.org/docs/installation/](https://flarum.org/docs/installation/)
中文文档：[https://flarum.org.cn/docs/preface/faq.html](https://flarum.org.cn/docs/preface/faq.html)
[https://www.jianshu.com/p/b9c4cfea6e6b](https://www.jianshu.com/p/b9c4cfea6e6b)
[https://blog.liyuans.com/archives/Flarum.html](https://blog.liyuans.com/archives/Flarum.html)
[http://www.lijiaocn.com/%E6%96%B9%E6%B3%95/2017/11/21/flarum-usage.html](http://www.lijiaocn.com/%E6%96%B9%E6%B3%95/2017/11/21/flarum-usage.html)
nginx配置：
[https://discuss.flarum.org/d/7366-how-to-add-nginx-and-lighttpd-your-server-s-configuration-block/2](https://discuss.flarum.org/d/7366-how-to-add-nginx-and-lighttpd-your-server-s-configuration-block/2)
[https://github.com/gwillem/flarum-docs/blob/master/user/ubuntu-server-install.md](https://github.com/gwillem/flarum-docs/blob/master/user/ubuntu-server-install.md)
