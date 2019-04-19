# 玩转 Markdown，你需要用到这几个工具 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [蓝猫163](http://www.jobbole.com/members/lanmao) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 前言
随着 WordPress 和静态网站的流行，Markdown 被用的越来越多。
Markdown 是一个面向写作的语法引擎，Markdown 的最终目的都是解析成 HTML 用于网页浏览，所以它兼容 HTML 语法，即你可以在 Markdown 文档中使用原生的 HTML 标签。
## Markdown 解析器
开发静态网站生成器的时候都会采用一种叫front matter的格式进行网站内容写在类似下面的格式


```
---
title: 玩转Markdown ，你需要用到这几个工具
date: 2016-08-14 16:44:54
image: /img/pencils-762555_640.jpg
---
## 前言
随着wordpress和静态网站的流行，Markdown 被用的越来越多。...
```
当进行网站生成的时候需要进行Markdown 解析，然后渲染成html页面，那用什么工具进行解析呢？
**[marked](https://github.com/chjj/marked)**
marked是最早用node.js开发的Markdown 解析器，同时提供CLI命令调用和node.js api调用。
CLI调用代码示例


```
$ marked -o hello.html
hello world
^D
$ cat hello.html
<p>hello world</p>
```
API调用示例


```
var marked = require('marked');
console.log(marked('I am using __Markdown __.'));
// Outputs: <p>I am using <strong>Markdown </strong>.</p>
```
这些都是一些通用的功能，但是marked还支持代码高亮，通过使用[highlight.js](https://github.com/isagalaev/highlight.js)。
使用highlight.js进行代码高亮相信大家都用到过，可能大家不知道是highlight.js还支持api方式调用，下面的代码会配置marked使用highlight.js进行代码高亮相信大家都用到过，可能大家不知道是highlight


```
marked.setOptions({
  highlight: function (code, lang) {
    var res;
    if (lang) {
      res = hljs.highlight(lang, code, true).value;
    } else {
      res = hljs.highlightAuto(code).value;
    }
    return res;
  }
});
```
生成的代码已经包含代码高亮标签，最后只需要引入highlight.js的主题就能显示了，highlight.js所有的颜色主题都在[这里](https://github.com/isagalaev/highlight.js/tree/master/src/styles)
**[Markdown -js](https://github.com/evilstreak/Markdown -js)**
Markdown -js也是一款使用node.js开发的Markdown 解析器，基本用法和marked差不多，但是文档里面好像没有提到像marked一样进行代码高亮生成的接口，有兴趣的同学自己找找吧。
## Markdown 生成器
**[to-Markdown](https://github.com/domchristie/to-Markdown )**
什么是Markdown 生成器，就是根据html标签生成Markdown 文件。
github上面Markdown 生成器star数最高的是to-Markdown 。
简单的代码示例


```
var toMarkdown  = require('to-Markdown ');
toMarkdown ('<h1>Hello world!</h1>');
```
to-Markdown 最近进行了更新，增加了对gfm的兼容，gfm就是[git flavored Markdown](https://guides.github.com/features/mastering-Markdown /)的意思， 是github对Markdown 语法进行了扩展。
使用gfm的示例


```
toMarkdown ('<del>Hello world!</del>', { gfm: true });
```
那这个to-Markdown 有什么用呢？
举个简单的例子，假如我想开发一个简单的rss阅读器，但是我又不想跳转到目标网站去阅读，因为不同的网站风格不一，导致不一致的阅读体验。
怎么办呢？那就把网站内容抓取下来，然后用to-Markdown 生成Markdown 文件，然后使用自己的模板样式进行统一渲染。
当然去除广告只是一个side effect。
**[heckyesMarkdown](http://heckyesMarkdown .com/)**
除了to-Markdown 之外还有一个比较好用的api，heckyesMarkdown ，这个项目使用了[php-readability](https://github.com/feelinglucky/php-readability)，提高文章的可读性。
可惜heckyesMarkdown 没有开源出来，这个项目有点古老，估计那个时候github还没流行起来。
heckyesmarkdow对中文的支持不是非常友好，如果想抓取中文站还是使用to-Markdown 比较靠谱一点。
## front matter
Markdown 写文章确实很方便，简单容易上手，但是Markdown 不能保存元数据，例如作者，日期，类型这样的结构化的数据如果都生成html标签的话提取的时候又稍微麻烦了点， 还得借助[cheerio](https://github.com/cheeriojs/cheerio)才能完成。
所以，为了能方便的保存文章的元数据，几乎所有的静态网站生成器都使用front matter格式来保存文章。
front matter文件通常分为头部和正文部分，头部一般使用[yaml](www.yaml.org)、[toml](https://github.com/toml-lang/toml)和json三种格式，front matter 解析工具需要识别这三种格式的文件头。正文部分就是普通的Markdown 内容。
**[front-matter](https://github.com/jxson/front-matter)**
front-matter也是用node.js开发的，相比Markdown 解析器来说，fornt-matter解析器要简单很多。
示例文件 `example.md`


```
---
title: Just hack'n
description: Nothing to see here
---
This is some text about some stuff that happened sometime ago
```
解析代码


```
var fs = require('fs')
  , fm = require('front-matter')
fs.readFile('./example.md', 'utf8', function(err, data){
  if (err) throw err
  var content = fm(data)
  console.log(content)
})
```


```
{
    attributes: {
        title: 'Just hack\'n',
        description: 'Nothing to see here'
    },
    body: '\nThis is some text about some stuff that happened sometime ago',
    frontmatter: 'title: Just hack\'n\ndescription: Nothing to see here'
}
```
front matter虽然格式看起来有点杂乱，却是对Markdown 强有力的补充。
