# capistrano自动化部署 - weixin_33985507的博客 - CSDN博客
2010年12月19日 19:51:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5

gem install capistrano
3,加入capistrano支持
[root@master blog]# capify .
[add] writing './Capfile'
[add] writing './config/deploy.rb'
[done] capified!
[http://www.weekface.info/2010/03/12/capistrano](http://www.weekface.info/2010/03/12/capistrano)
5,部署
[root@master blog]# cap setup #初次部署需要运行此命令
[root@master blog]# cap deploy
这就是基本流程了,下篇文章将实际操作一遍rails + git + capistrano实现自动化部署
