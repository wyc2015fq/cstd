# Sublime text 入门学习资源篇及其基本使用方法 - 在思索中前行！ - CSDN博客





2014年07月05日 19:05:20[_Tham](https://me.csdn.net/txl16211)阅读数：3477








## Sublime text 学习资源篇

## [史上最性感的编辑器-sublimetext](http://www.sublimetext.com/)，[插件](http://www.sublimetext.com/)，[学习资源](http://www.jianshu.com/p/d1b9a64e2e37)



## 官网
- [http://www.sublimetext.com/](http://www.sublimetext.com/)

## 插件
- [https://packagecontrol.io](https://packagecontrol.io)

## 教程
- [Sublime Text 全程指南](http://zh.lucida.me/blog/sublime-text-complete-guide/)
- [Sublime Text 2 入门及技巧](http://lucifr.com/2011/08/31/sublime-text-2-tricks-and-tips/)
- [Sublime Text 使用介绍、全套快捷键及插件推荐](http://www.daqianduan.com/4820.html)
- [Sublime Text 3 学习及使用](http://blog.csdn.net/idxuanjun/article/details/13292847)
- [Sublime Text 3 使用心得](http://ce.sysu.edu.cn/hope/Item/106853.aspx)
- [Sublime Text 3 新手上路：必要的安裝、設定與基本使用教學](http://blog.miniasp.com/post/2014/01/07/Useful-tool-Sublime-Text-3-Quick-Start.aspx)
- [Perfect Workflow in Sublime 2](http://code.tutsplus.com/courses/perfect-workflow-in-sublime-text-2)
- [6 Ways to Turn Sublime Text Into the Perfect Blogging Tool](http://sublimetexttips.com/sublime-productivityuctivityroductivityuctivity)
- Scotch Series- [Best of Sublime Text 3: Features, Plugins, and Settings](http://scotch.io/bar-talk/best-of-sublime-text-3-features-plugins-and-settings)
- [Sublime Text Keyboard Shortcuts](http://scotch.io/bar-talk/sublime-text-keyboard-shortcuts)
- [THE COMPLETE VISUAL GUIDE TO SUBLIME TEXT 3](http://scotch.io/series/the-complete-visual-guide-to-sublime-text-3)
- [Themes, Color Schemes, and Cool Features](http://scotch.io/bar-talk/the-complete-visual-guide-to-sublime-text-3-themes-color-schemes-and-cool-features)
- [Getting Started and Keyboard Shortcuts](http://scotch.io/bar-talk/the-complete-visual-guide-to-sublime-text-3-getting-started-and-keyboard-shortcuts)
- [Plugins Part 1](http://scotch.io/bar-talk/the-complete-visual-guide-to-sublime-text-3-plugins-part-1)
- [Plugins Part 2](http://scotch.io/bar-talk/the-complete-visual-guide-to-sublime-text-3-plugins-part-2)

- [Working With Code Snippets In Sublime Text](http://www.hongkiat.com/blog/sublime-code-snippets/)
- [The Best Plugins for Sublime Text](http://ipestov.com/the-best-plugins-for-sublime-text/)

## 书籍
- [Sublime Text 手冊](http://docs.sublimetext.tw/)
- [Sublime Text Unofficial Documentation](http://sublime-text-unofficial-documentation.readthedocs.org/en/latest/index.html)
- [Sublime Text Power User](http://ipestov.com/the-best-plugins-for-sublime-text/)
- Instant Sublime Text Starter
- Mastering Sublime Text
- [Sublime Productivity](http://ipestov.com/the-best-plugins-for-sublime-text/)



**Sublime Text 系列**
- [Sublime Text：学习资源篇](http://www.jianshu.com/p/d1b9a64e2e37)
- [Sublime插件：增强篇](http://www.jianshu.com/p/5905f927d01b)
- [Sublime插件：Markdown篇](http://www.jianshu.com/p/aa30cc25c91b)
- [Sublime插件：C语言篇](http://www.jianshu.com/p/595975a2a5f3)


## 基本使用方法

网络文章：http://www.vsnote.com/tag/sublime-text-3

网络教程：http://www.iplaysoft.com/sublimetext.html




Emmet的前身是大名鼎鼎的Zen coding，如果你从事Web前端开发的话，对该插件一定不会陌生。它使用仿CSS选择器的语法来生成代码，大大提高了HTML/CSS代码编写的速度，比如下面的演示： 


![](http://dl.iteye.com/upload/attachment/0083/2327/301ff5c9-3604-3dd3-a206-6d3516861ec4.jpg)

Zen coding下的编码演示


去年年底，该插件已经改名为Emmet。但Emmet不只改名，还带来了一些新特性。本文就来直观地演示给你。

**一、快速编写HTML代码**

1.  初始化

HTML文档需要包含一些固定的标签，比如<html>、<head>、<body>等，现在你只需要1秒钟就可以输入这些标签。比如输入“!”或“html:5”，然后按Tab键： 


![](http://dl.iteye.com/upload/attachment/0083/2329/0070e820-1cbf-3f42-8c5b-838e5774d02b.gif)


- html:5 或!：用于HTML5文档类型
- html:xt：用于XHTML过渡文档类型
- html:4s：用于HTML4严格文档类型
2.  轻松添加类、id、文本和属性

连续输入元素名称和ID，Emmet会自动为你补全，比如输入p#foo： 


![](http://dl.iteye.com/upload/attachment/0083/2331/cb250aef-3b60-3297-86ba-8c3ed36cacad.gif)


连续输入类和id，比如p.bar#foo，会自动生成： 




Html代码 

- <pclass="bar"id="foo"></p>


下面来看看如何定义HTML元素的内容和属性。你可以通过输入h1{foo}和a[href=#]，就可以自动生成如下代码：




Html代码 

- <h1>foo</h1>
- <ahref="#"></a>



![](http://dl.iteye.com/upload/attachment/0083/2333/167e1956-4ac6-3b6d-9025-8bf90fee1250.gif)


3.  嵌套

现在你只需要1行代码就可以实现标签的嵌套。 

- >：子元素符号，表示嵌套的元素
- +：同级标签符号
- ^：可以使该符号前的标签提升一行
效果如下图所示： 


![](http://dl.iteye.com/upload/attachment/0083/2335/47ae82f5-4fd8-36a1-9b4e-d148237537ee.gif)


4.  分组

你可以通过嵌套和括号来快速生成一些代码块，比如输入(.foo>h1)+(.bar>h2)，会自动生成如下代码： 




Html代码 

- <divclass="foo">
- <h1></h1>
- </div>
- <divclass="bar">
- <h2></h2>
- </div>



![](http://dl.iteye.com/upload/attachment/0083/2337/ae3a8b58-244a-3680-9f0c-d0cab459fc41.gif)


5.  隐式标签

声明一个带类的标签，只需输入div.item，就会生成<div class="item"></div>。 

在过去版本中，可以省略掉div，即输入.item即可生成<div class="item"></div>。现在如果只输入.item，则Emmet会根据父标签进行判定。比如在<ul>中输入.item，就会生成<li class="item"></li>。 


![](http://dl.iteye.com/upload/attachment/0083/2339/363af658-650f-38d1-ad0e-9608df9d2873.gif)


下面是所有的隐式标签名称： 

- li：用于ul和ol中
- tr：用于table、tbody、thead和tfoot中
- td：用于tr中
- option：用于select和optgroup中
6.  定义多个元素

要定义多个元素，可以使用*符号。比如，ul>li*3可以生成如下代码： 




Html代码 

- <ul>
- <li></li>
- <li></li>
- <li></li>
- </ul>




![](http://dl.iteye.com/upload/attachment/0083/2341/cf5613ac-4198-326e-a651-a08ad206800a.gif)


7.  定义多个带属性的元素

如果输入 ul>li.item$*3，将会生成如下代码： 




Html代码 

- <ul>
- <liclass="item1"></li>
- <liclass="item2"></li>
- <liclass="item3"></li>
- </ul>




![](http://dl.iteye.com/upload/attachment/0083/2343/a1c5f9d8-d187-3bde-895b-2e14fb428184.gif)


**二、CSS缩写**

1.  值

比如要定义元素的宽度，只需输入w100，即可生成 




Css代码 

- width: 100px;  



![](http://dl.iteye.com/upload/attachment/0083/2345/21a47a7c-2d38-3231-8b4b-7d15f38be57a.gif)


除了px，也可以生成其他单位，比如输入h10p+m5e，结果如下： 




Css代码 

- height: 10%;  
- margin: 5em;  



单位别名列表： 

- p 表示%
- e 表示 em
- x 表示 ex
2.  附加属性

可能你之前已经了解了一些缩写，比如 @f，可以生成： 




Css代码 

- @font-face {  
-   font-family:;  
-   src:url();  
- }  


一些其他的属性，比如background-image、border-radius、font、@font-face,text-outline、text-shadow等额外的选项，可以通过“+”符号来生成，比如输入@f+，将生成： 




Css代码 

- @font-face {  
-   font-family: 'FontName';  
-   src: url('FileName.eot');  
-   src: url('FileName.eot?#iefix') format('embedded-opentype'),  
-      url('FileName.woff') format('woff'),  
-      url('FileName.ttf') format('truetype'),  
-      url('FileName.svg#FontName') format('svg');  
-   font-style: normal;  
-   font-weight: normal;  
- }  



![](http://dl.iteye.com/upload/attachment/0083/2347/2c8cccf4-be54-3190-b1e7-3713717e2a78.gif)


3.  模糊匹配

如果有些缩写你拿不准，Emmet会根据你的输入内容匹配最接近的语法，比如输入ov:h、ov-h、ovh和oh，生成的代码是相同的： 




Css代码 

- overflow: hidden;  



![](http://dl.iteye.com/upload/attachment/0083/2349/4c2c81e2-ebba-367e-a4fd-4f667d08ca84.gif)


4.  供应商前缀

如果输入非W3C标准的CSS属性，Emmet会自动加上供应商前缀，比如输入trs，则会生成： 




Css代码 

- -webkit-transform: ;  
- -moz-transform: ;  
- -ms-transform: ;  
- -o-transform: ;  
- transform: ;  



![](http://dl.iteye.com/upload/attachment/0083/2351/a68eda57-f573-343c-81b8-19e0a48c08fe.gif)


你也可以在任意属性前加上“-”符号，也可以为该属性加上前缀。比如输入-super-foo： 




Css代码 

- -webkit-super-foo: ;  
- -moz-super-foo: ;  
- -ms-super-foo: ;  
- -o-super-foo: ;  
- super-foo: ;  


如果不希望加上所有前缀，可以使用缩写来指定，比如-wm-trf表示只加上-webkit和-moz前缀： 




Css代码 

- -webkit-transform: ;  
- -moz-transform: ;  
- transform: ;  


前缀缩写如下： 

- w 表示 -webkit-
- m 表示 -moz-
- s 表示 -ms-
- o 表示 -o-
5.  渐变

输入lg(left, #fff 50%, #000)，会生成如下代码： 




Css代码 

- background-image: -webkit-gradient(linear, 00, 100% 0, color-stop(0.5, #fff), to(#000));  
- background-image: -webkit-linear-gradient(left, #fff 50%, #000);  
- background-image: -moz-linear-gradient(left, #fff 50%, #000);  
- background-image: -o-linear-gradient(left, #fff 50%, #000);  
- background-image: linear-gradient(left, #fff 50%, #000);  



![](http://dl.iteye.com/upload/attachment/0083/2353/f39213f9-5d68-3728-8cf3-7964aaa02bae.gif)


**三、附加功能**

生成Lorem ipsum文本

Lorem ipsum指一篇常用于排版设计领域的拉丁文文章，主要目的是测试文章或文字在不同字型、版型下看起来的效果。通过Emmet，你只需输入lorem 或 lipsum即可生成这些文字。还可以指定文字的个数，比如lorem10，将生成： 


引用

Lorem ipsum dolor sit amet, consectetur adipisicing elit. Libero delectus.


![](http://dl.iteye.com/upload/attachment/0083/2355/87a3d500-1798-3050-b7e0-df51c94bd203.gif)


**四、定制**

你还可以定制Emmet插件： 

- 添加新缩写或更新现有缩写，可修改[snippets.json](http://docs.emmet.io/customization/snippets/)文件
- 更改Emmet过滤器和操作的行为，可修改[preferences.json](http://docs.emmet.io/customization/preferences/)文件
- 定义如何生成HTML或XML代码，可修改[syntaxProfiles.json](http://docs.emmet.io/customization/syntax-profiles/)文件

**五、针对不同编辑器的插件**

Emmet支持的编辑器如下（链接为针对该编辑器的Emmet插件）： 

- [Sublime Text 2](https://github.com/sergeche/emmet-sublime)
- [TextMate 1.x](https://github.com/emmetio/Emmet.tmplugin)
- [Eclipse/Aptana](https://github.com/emmetio/emmet-eclipse)
- [Coda 1.6 and 2.x](https://github.com/emmetio/Emmet.codaplugin)
- [Espresso](https://github.com/emmetio/Emmet.sugar)
- [Chocolat](https://github.com/sergeche/emmet.chocmixin) （通过“Install Mixin”对话框添加)
- [Komodo Edit/IDE](https://github.com/emmetio/emmet/downloads) （通过Tools → Add-ons菜单添加)
- [Notepad++](https://github.com/emmetio/emmet/downloads)
- [PSPad](https://github.com/emmetio/emmet/downloads)
- [<textarea>](https://github.com/emmetio/emmet/downloads)
- [CodeMirror2/3](https://github.com/emmetio/codemirror)
- [Brackets](https://github.com/emmetio/brackets-emmet)



