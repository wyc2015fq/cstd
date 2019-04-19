# 用Jekyll搭建的Github Pages个人博客实践2 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月15日 08:37:07[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：557
依稀记得之前访问[喵神](http://onevcat.com/)的博客很有feel
感谢喵神git上的提供的主题[Vno-Jekyll](https://github.com/onevcat/vno-jekyll)。
创建代码仓库(你的用户名).github.io
将主题[Vno-Jekyll](https://github.com/onevcat/vno-jekyll)下载到本地，解压到刚刚的代码仓库目录下，可以把文件夹里的文件都删了。
 gem install jekyll
进入到(你的用户名).github.io目录
执行bundle install
如果出现提示：
-bash:bundle: command
not found
就先安装bundle（执行sudo gem install
 bundle）
开启Jekyll环境bundle
 exec jekyll serve，看见下面输出代表开启成功。
在写文章的时候，在终端执行bundle
 exec jekyll serve，开启Jekyll本地环境，可以一边写博文，一边刷新http://127.0.0.1:4000/地址查看实时效果。写完提交git，就完事啦。
用git电脑终端将这些代码都上传到git代码仓库
绑定个人域名
**创建CNAME文件**
评论功能
登录[Disqus](https://disqus.com/)网站注册一个账号
- 点击Setting图标，选择Add
 disqus to site
- 点击Start Using Engage
- 设置自己的Disqus的URL
- 设置根目录下_config.yml文件的disqus的URL
# Commentcomment:
 disqus: joeliu
写文章
一篇文章就是一个文件
放在_posts文件夹里
命名要按YYYY-MM-DD-文章标题.md这种格式来
**书写格式**
文章的开头，我们需要先设置头信息。头信息需要根据[YAML](http://yaml.org/)的格式写在两行三虚线之间。
---layout: posttitle: 这个是标题date: 2016-04-16 11:11:11.000000000 +09:00tags:
 Jekyll Github---
date：发布文章的时间。（后面的一串零零零好像不能省）
layout
用post.html。page.html模板比post.html模板少了更早的文章和评论模块。
tags：标签，一篇文章可以设置多个标签，使用空格分割。
其它的变量可以在[Jekyll网站](http://jekyllrb.com/docs/frontmatter/)上查看。

主题细节修改
博客名、描述、跳转链接的修改的主要文件路径是在根目录下的文件：_config.yml
头像和背景存放路径：代码仓库根目录->assets->image，直接替换文件就好了，不过要保持文件名一样。

Markdown Engine: 用于将Makrdown语法的文本转换成html文件或者输出
- Github支持的Markdown engine: kramdown(default), redcarpet
- jekyll支持的Markdown engine：kramdown(default), redcarpet
建议查阅[kramdown官方文档](http://kramdown.gettalong.org/quickref.html)

参考：
[Github Pages - 官方配置指南](https://help.github.com/categories/github-pages-basics/)
[Github Pages - 自定义页面指南](https://help.github.com/categories/customizing-github-pages/)
[极客学院翻译 - 中文版本指南](http://wiki.jikexueyuan.com/project/github-pages-basics/)
[Jekyll中文指导手册](http://jekyllcn.com/)
[https://onevcat.com/](https://onevcat.com/)
[http://louisly.com/2016/04/used-jekyll-to-create-my-github-blog/](http://louisly.com/2016/04/used-jekyll-to-create-my-github-blog/)
[https://github.com/jekyll/jekyll/wiki/sites](https://github.com/jekyll/jekyll/wiki/sites)
 所有站点
[http://cenalulu.github.io/](http://cenalulu.github.io/)
 这个站也不错
