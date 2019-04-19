# 利用github Pages和Jekyll搭建blog实践1 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月15日 08:32:51[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：215
你必须要懂一点git和网页开发。安装了[git](http://git-scm.com/book/en/Getting-Started-Installing-Git)，并且有[github](https://github.com/)账户。
github设计了[Pages功能](http://pages.github.com/)，允许用户自定义项目首页
github提供模板，允许[站内生成](https://help.github.com/articles/creating-pages-with-the-automatic-generator)网页，但也允许用户自己编写网页，然后上传。有意思的是，这种上传并不是单纯的上传，而是会经过Jekyll程序的再处理。
[Jekyll](http://jekyllrb.com/)**（发音/'dʒiːk əl/，"杰克尔"）是一个静态站点生成器，它会根据网页源码生成静态文件。**它提供了模板、变量、插件等功能，所以实际上可以用来编写整个网站。
**先在本地编写符合Jekyll规范的网站源码，然后上传到github，由github生成并托管整个网站。**
**实例:**
mkdir jekyll_demo
cd jekyll_demo
　　$ git init
然后，创建一个没有父节点的分支gh-pages。因为github规定，只有该分支中的页面，才会生成网页文件。
git checkout --orphan gh-pages
选项说明;
用于创建一个全新的分支，不包含原分支的提交历史
假如你的某个分支上，积累了无数次的提交，你也懒得去打理，打印出的log也让你无力吐槽，那么这个命令将是你的神器，它会基于当前所在分支新建一个赤裸裸的分支，没有任何的提交历史，但是当前分支的内容一一俱全。新建的分支，严格意义上说，还不是一个分支，因为HEAD指向的引用中没有commit值，只有在进行一次提交后，它才算得上真正的分支。

以下所有动作，都在该分支下完成。
**第二步，创建设置文件。**
根目录下，建立一个名为_config.yml的文本文件。它是jekyll的设置文件，我们在里面填入如下内容，其他设置都可以用默认选项，具体解释参见[官方网页](https://github.com/mojombo/jekyll/wiki/Configuration)。
　　baseurl: /jekyll_demo
**第三步，创建模板文件。**
创建一个_layouts目录，用于存放模板文件。进入该目录，创建一个default.html文件，作为Blog的默认模板。并在该文件中填入以下内容。
　<!DOCTYPE html>
　　<html>
　　<head>
　　　　<meta http-equiv="content-type" content="text/html; charset=utf-8" />
　　　　<title>{{ page.title }}</title>
　　</head>
　　<body>
　　　　{{ content }}
　　</body>
　　</html>
{{ page.title }}表示文章标题，{{ content }}表示文章内容，更多模板变量请参考[官方文档](https://github.com/mojombo/jekyll/wiki/Template-Data)。
**第四步，创建文章。**
根目录，创建一个_posts目录，用于存放blog文章。
进入该目录，创建第一篇文章。文章就是普通的文本文件，文件名假定为2012-08-25-hello-world.html。(注意，文件名必须为"年-月-日-文章标题.后缀名"的格式。如果网页代码采用html格式，后缀名为html；如果采用[markdown](http://daringfireball.net/projects/markdown/)格式，后缀名为md。）
在该文件中，填入以下内容：（注意，行首不能有空格）
　---
　　layout: default
　　title: 你好，世界
　　---
　　<h2>{{ page.title }}</h2>
　　<p>我的第一篇文章</p>
　　<p>{{ page.date | date_to_string }}</p>
每篇文章的头部，必须有一个[yaml文件头](https://github.com/mojombo/jekyll/wiki/YAML-Front-Matter)，用来设置一些元数据。它用三根短划线"---"，标记开始和结束
{{ page.date }}是嵌入文件名的日期（也可以在文件头重新定义date变量），"| date_to_string"表示将page.date变量转化成人类可读的格式。
**第五步，创建首页**
根目录，创建一个index.html文件，填入以下内容。
　　---
　　layout: default
　　title: 我的Blog
　　---
　　<h2>{{ page.title }}</h2>
　　<p>最新文章</p>
　　<ul>
　　　　{% for post in site.posts %}
　　　　　　<li>{{ post.date | date_to_string }} <a href="{{ site.baseurl }}{{ post.url }}">{{ post.title }}</a></li>
　　　　{% endfor %}
　　</ul>
输出内容使用两层大括号，单纯的命令使用一层大括号。
{{site.baseurl}}就是_config.yml中设置的baseurl变量。
**第六步，发布内容**
$ git add .
　　$ git commit -m "first post"
然后，前往github的网站，在网站上创建一个名为jekyll_demo的库（不要选readme.md）。接着，再将本地内容推送到github上你刚创建的库。注意，下面命令中的username，要替换成你的username。
　　$ git remote add origin https://github.com/**username**/jekyll_demo.git
　　$ git push origin gh-pages
上传成功之后，等10分钟左右，访问**http://username.github.****io****/jekyll_demo/**就可以看到Blog已经生成了。
**第七步，绑定域名**
根目录下面，新建一个名为CNAME的文本文件，里面写入你要绑定的域名，比如example.com或者xxx.example.com。
如果绑定的是顶级域名，则DNS要新建一条A记录，指向204.232.175.78。如果绑定的是二级域名，则DNS要新建一条CNAME记录，指向username.github.com。此外，别忘了将_config.yml文件中的baseurl改成根目录"/"。
其他用Jekyll搭建的[blog](https://github.com/mojombo/jekyll/wiki/Sites)。
效果：
[https://starconnect.github.io/jekyll_demo/](https://starconnect.github.io/jekyll_demo/)
[https://starconnect.github.io/jekyll_demo/2017/07/14/hello-world.html](https://starconnect.github.io/jekyll_demo/2017/07/14/hello-world.html)
参考：
[http://www.ruanyifeng.com/blog/2012/08/blogging_with_jekyll.html](http://www.ruanyifeng.com/blog/2012/08/blogging_with_jekyll.html)
