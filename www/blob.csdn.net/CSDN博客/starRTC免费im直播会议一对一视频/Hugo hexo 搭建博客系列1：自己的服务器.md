# Hugo hexo 搭建博客系列1：自己的服务器 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月02日 09:45:26[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：433
hexo jekyll
[https://hexo.io/zh-cn/](https://hexo.io/zh-cn/)
[http://theme-next.iissnan.com/getting-started.html](http://theme-next.iissnan.com/getting-started.html)
[Hexo](https://hexo.io/zh-cn/) 是高效的静态站点生成框架，基于 [Node.js](https://nodejs.org/)。
在 Hexo 中有两份主要的配置文件，其名称都是 _config.yml。
 其中，一份位于站点根目录下，主要包含 Hexo 本身的配置；另一份位于主题目录下，这份配置由主题作者提供，主要用于配置主题相关的选项。
只需一条指令即可部署到 GitHub Pages,

安装hexo需要先安装Node.js
apt-get install git-core
yum install git-core
安装 Node.js 的最佳方式是使用 [nvm](https://github.com/creationix/nvm)。
wget -qO- https://raw.github.com/creationix/nvm/master/install.sh | sh
安装完成后，重启终端并执行下列命令即可安装 Node.js。
$ nvm install stable
安装 Hexo。
$ npm install -g hexo-cli
报错：
sh: highlight_alias.json: Permission denied
npm ERR! code ELIFECYCLE
解决：
npm config set user 0
npm config set unsafe-perm true
报错：
npm ERR。。。 integrity checksum failed when using sha1 。。。but got。。。
npm cache clean --force  
npm config
set registry
[http://registry.npm.taobao.org](http://registry.npm.taobao.org)
建站
$ hexo init <folder>
$ cd <folder>
$ npm install
目录如下：
.
├── _config.yml
├── package.json
├── scaffolds
├── source
| ├── _drafts
| └── _posts
└── themes
**package.json**
应用程序的信息。[EJS](http://embeddedjs.com/), [Stylus](http://learnboost.github.io/stylus/) 和 [Markdown](http://daringfireball.net/projects/markdown/) renderer
 已默认安装，您可以自由移除。
package.json
{
"name": "hexo-site",
"version": "0.0.0",
"private": true,
"hexo": {
"version": ""
},
"dependencies": {
"hexo": "^3.0.0",
"hexo-generator-archive": "^0.1.0",
"hexo-generator-category": "^0.1.0",
"hexo-generator-index": "^0.1.0",
"hexo-generator-tag": "^0.1.0",
"hexo-renderer-ejs": "^0.1.0",
"hexo-renderer-stylus": "^0.2.0",
"hexo-renderer-marked": "^0.2.4",
"hexo-server": "^0.1.2"
}
}
**scaffolds**
[模版](https://hexo.io/zh-cn/docs/writing.html) 文件夹。
**source**
资源文件夹
配置_config.yml：
description主要用于SEO，告诉搜索引擎一个关于您站点的简单描述，通常建议在其中包含您网站的关键词。
timezone:
[Asia/Shanghai](https://en.wikipedia.org/wiki/Asia/Shanghai)
如果您的网站存放在子目录中，例如 http://yoursite.com/blog，则请将您的 url 设为 http://yoursite.com/blog 并把 root 设为 /blog/。
指令：
hexo new "postName" #新建文章，如果标题包含空格的话，请使用引号括起来。
hexo g #生成public静态文件 == hexo generate
hexo s #本地预览 == hexo server默认情况下，访问网址为： http://localhost:4000/。
 hexo d #部署网站 == hexo deploy   
 hexo d -g  部署之前预先生成静态文件Hexo
 生成的所有文件都放在 public 文件夹中，您可以将它们复制到您喜欢的地方。
Hexo不支持指定多个同级分类因此，有必要为您的文章选择尽可能准确的分类。
个主题可能会有以下的结构：
.
├── _config.yml
├── languages
├── layout
├── scripts
└── source
主题替换：排名第一的主题
[https://github.com/iissnan/hexo-theme-next](https://github.com/iissnan/hexo-theme-next)
一个主题，三种外观，选您所好通过主题中的主题（亦称 Scheme），您可以方便地改变您博客的外观： 修改主题配置文件，搜索
 scheme 关键字，
只需要将主题文件拷贝至站点目录的 themes 目录下，
 然后修改下配置文件即可
$
**cd** your-hexo-site$ git clone https://github.**com**/iissnan/hexo-theme-**next**
 themes/**next**
修改配置：
theme:next
language:zh-Hans 对应简体中文
运行下hexo clean清除
 Hexo 的缓存。
小结：
部署在自己服务器 hexo clean && hexo g就可以了
