# Haml 参考大全 - weixin_33985507的博客 - CSDN博客
2007年10月25日 01:14:19[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
Haml是一种用来描述任何XHTML web document的标记语言，它是干净，简单的。而且也不用内嵌代码。Haml的职能就是替代那些内嵌代码的page page templating systems，比如PHP,ERB(Rails的模板系统)，ASP。不过， haml避免了直接coding XHTML到模板，因为它实际上是一个xhtml的抽象描述，内部使用一些code来生成动态内容。<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />
**它有什么特点呢？**
1.空格标识层次嵌套关系
2.良好的标签格式
3.DRY（Don’t repeat yourself）
4.遵循CSS标准
5.集成了Ruby代码
6.用.haml扩展名代替了rails模板（.rhtml）
**Haml****的使用**
Haml的使用有两种方式：
作为Ruby on Rails的插件来使用。
作为一个独立的Ruby module来使用。
Rails 插件方式：
这是使用Haml最常用的方式。当然，安装Haml的方式，就是Rails里常用的插件安装方式了：
./script/plugin install [[url]http://svn.hamptoncatlin.com/haml/tags/stable[/url]](http://svn.hamptoncatlin.com/haml/tags/stable)
一旦安装好以后，你必须以.haml为扩展名来使用。
你在ERB模板里可以使用的实例变量在Haml里照样可以使用，Helper方法也不例外。比如：
# file: app/controllers/movies_controller.rb
class MoviesController < ApplicationController
def index
@title = "Teen Wolf"
end
end
# file: app/views/movies/index.haml
#content
.title
%h1= @title
= link_to 'Home', home_url
上面的haml代码会被编译为：
<div id='content'>
<div class='title'>
<h1>Teen Wolf</h1>
<a href='/'>Home</a>
</div>
</div>
Ruby Module方式：
Haml可以完全从rails和ActionView里拿出来单独使用。下下面这样做：
geminstallhaml
然后用Haml::Engine：
engine = Haml::Engine.new("%p Haml code!")
engine.render #=> "<p>Haml code!</p>\n"
**XTML Tags****：**
下面这些字符会渲染出相应的xhtml tag
**%**
百分号符号是一行的开始，紧接着一个元素的名字，然后后面跟一个可选的修饰语（见下例），比如一个空格，或一行文本等，就会被渲染到这个元素里成为其内容。它会创建一个这样的形式：<element></element>.。举个例子：
%one
%two
%three Hey there
会被编译为：
<one>
<two>
<three>Hey there</three>
</two>
</one>
对于任何一个有效的标准元素字符，Haml都会自动的为其生成闭合标签。
**{}**
括号内的Ruby hash是用来指名一个元素的属性。它作为一个ruby hash的字面量，局部变量也可以在其中使用。Hash放在被定义好的标签之后，基本上就和Ruby语法一样，看例子：
%head{ :name => "doc_head" }
%script{ 'type' => "text/" + "javascript",
:src=> "javascripts/script_#{2 + 7}" }
编译后为：
<head name="doc_head">
<script src='javascripts/script_9' type='text/javascript'>
</script>
</head>
**[]**
方括号跟在一个标签定义之后，包含一个Ruby 对象，被用来为这个标签设置class和id属性。这个class的值被设置为这个对象的类名（两个单词用下划线形式表示，而不是驼峰表示方法）并且id的值被设置为对象的类名加上这个对象的id，也是下划线连接。因为一个对象的id通常是朦胧的实现细节，这是表现model的实例最有用的元素了（这句是不是翻译的太差？）。看例子：
# file: app/controllers/users_controller.rb
def show
@user = CrazyUser.find(15)
end
# file: app/views/users/show.haml
%div[@user]
%bar[290]/
Hello!
转换为：
<div class="crazy_user" id="crazy_user_15">
<bar class="fixnum" id="fixnum_581" />
Hello!
</div>
这是基于RailsConf Europe 2006 大会上DHH提出的SimpleHelpful语法
**/**
这个斜线字符，放在一个tag定义之后，可以让这个标签自我关闭。例子：
%br/
%meta{'http-equiv' => 'Content-Type', :content => 'text/html'}/
转换为：
<br />
<meta http-equiv='Content-Type' content='text/html' />
有一些标签（meta, img, link, script, br, and hr tags等）当没有内容的时候会自动关闭。看例子：
%br
%meta{'http-equiv' => 'Content-Type', :content => 'text/html'}
转换为：
<br />
<meta http-equiv='Content-Type' content='text/html' />
**. and #**
这两个符号是从CSS里借鉴来的。他们被用来表示一个元素的class和id属性。
看例子：
%div#things
%span#rice Chicken Fried
%p.beans{ :food => 'true' } The magical fruit
%h1.class.otherclass#id <?xml:namespace prefix = st1 ns = "urn:schemas-microsoft-com:office:smarttags" />La La La
转换为：
<div id='things'>
<span id='rice'>Chicken Fried</span>
<p class='beans' food='true'>The magical fruit</p>
<h1 class='class otherclass' id='id'>La La La</h1>
</div>
注意h1标签。两个点连用，第一个表示class属性，第二个则是用来链接那两个字符的空格。
#content
.articles
.article.title
Doogie Howser Comes Out
.article.date
2006-11-05
.article.entry
Neil Patrick Harris would like to dispel any rumors that he is straight
转换为：
<div id="content">
<div class="articles">
<div class="article title">Doogie Howser Comes Out</div>
<div class="article date">2006-11-05</div>
<div class="article entry">
Neil Patrick Harris would like to dispel any rumors that he is straight
</div>
</div>
</div>
**Implicit Div Elements****（隐藏DIV****）**
因为Div这个标签经常被用，所以你仅用.and#这两个符号来定义class和id的时候，一个div元素就会被自动的使用。例如：
#collection
.item
.description What a cool item!
和下面的这个相似：
%div{:id => collection}
%div{:class => 'item'}
%div{:class => 'description'} What a cool item!
都会被转换为：
<div id='collection'>
<div class='item'>
<div class='description'>What a cool item!</div>
</div>
</div>
**=**
等号符号用来插入ruby 代码的值到模板中。
%p= "hello"
和下面的这种形式不太一样：
%p
= "hello"
**XHTML Helpers**
**No Special Character**
如果没有这些特定的字符打头的话，返回的只是一个普通的文本，比如下面的Wow this is cool!
%gee
%whiz
Wow this is cool!
转换为：
<gee>
<whiz>
Wow this is cool!
</whiz>
</gee>
**!!!**
当用haml来表示一个XHTML文档，你可以通过!!!这个符号来自动生成文档类型和XML prolog。比如：
!!! XML
!!!
%html
%head
%title Myspace
%body
%h1 I am the international space station
%p Sign my guestbook
转换为：
<?xml version="1.0" encoding="utf-8" ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<title>Myspace</title>
</head>
<body>
<h1>I am the international space station</h1>
<p>Sign my guestbook</p>
</body>
</html>
你也可以在！！！后面加版本号。比如：
!!! 1.1
转换为：
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
and 
!!! Strict
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
如果你不想用UTF-8的编码，你也可以指定你想要的编码：
!!! XML iso-8859-1
转换为：
<?xml version="1.0" encoding="iso-8859-1" ?>
**/**
如果这个斜线写在打头的位置，则会注释这行。
%billabong
/ This is the billabong element
I like billabongs!
转换为：
<billabong>
<!-- This is the billabong element -->
I like billabongs!
</billabong>
放在代码的上方，则注释整个代码：
/
%p This doesn't render...
%div
%h1 Because it's commented out!
转换为：
<!--
<p>This doesn't render...</p>
<div>
<h1>Because it's commented out!</h1>
</div>
-->
/[if IE]
%a{ :href => 'http://www.mozilla.com/en-US/firefox/' }
%h1 Get Firefox
转换为：
<!--[if IE]>
<a href='http://www.mozilla.com/en-US/firefox/'>
<h1>Get Firefox</h1>
</a>
<![endif]-->
**\**
反斜杠符号允许字符串前面的第一个符号作为纯文本使用。
%title
= @title
\- MySite
转换为：
<title>
MyPage
- MySite
</title>
**|**
管道符可以允许把输出为一行的内容写成多行。
%whoo
%hoo I think this might get |
pretty long so I should |
probably make it |
multiline so it doesn't |
look awful. |
%p This is short.
is compiled to: 
<whoo>
<hoo>
I think this might get pretty long so I should probably make it multiline so it doesn't look awful.
</hoo>
</whoo>
**:**
冒号是指定一个过滤器。冒号后面是你要使用的那个过滤器的名字。For example, 
%p
:markdown
Textile
=======
Hello, *World*
转换为：
<p>
<h1>Textile</h1>
<p>Hello, <em>World</em></p>
</p>
Haml支持的过滤器定义
**plain**
**ruby**
**preserve**
**erb**
**sass**
**redcloth**
**textile**
**markdown**
**Ruby evaluators****（执行Ruby****代码，前面说了）**
**=**
等号允许执行ruby代码并返回一个值作为显示文本。
%p
= ['hi', 'there', 'reader!'].join " "
= "yo"
编译为: 
<p>
hi there reader!
yo
</p>
你也能使用双等号来更容易的嵌入ruby代码。比如：
%p
== 1 + 1 = #{1 + 1}
编译为：
<p>
1 + 1 = 2
</p>
-
横杠符号，很有性格，可以使文本变为”silent script”：意思是，代码可以执行，但并不输出任何东西。
**这里不推荐使用这种扩展，所有的逻辑代码都应该限制在controller****，helper****或partials****里**
For example: 
- foo = "hello"
- foo << " there"
- foo << " you!"
%p= foo
转换为：
<p>
hello there you!
</p>
**Blocks**
Ruby中的块，也不需要明显的去关闭，haml会让它自动关闭。这写都是基于缩进的。千万记住要缩进两个空格。
- (42...47).each do |i|
%p= i
%p See, I can count!
编译为: 
<p>
42
</p>
<p>
43
</p>
<p>
44
</p>
<p>
45
</p>
<p>
46
</p>
Another example: 
%p
- case 2
- when 1
= "1!"
- when 2
= "2?"
- when 3
= "3."
is compiled to: 
<p>
2?
</p>
<
**-#**
相当于一个注释吧，跟在这个符号后面的文本无法输出。
For example: 
%p foo -# This is a comment %p bar 
is compiled to: 
<p>foo</p> <p>bar</p> 
h2>Other Useful Things
**Helpers**
Haml offers a bunch of helpers that are useful for doing stuff like preserving whitespace, creating nicely indented output for user-defined helpers, and other useful things. The helpers are all documented in the Haml::Helpers and Haml::Helpers::ActionViewExtensions modules. 
Haml提供了很多有用的helper方法。比如为用户定义的helper方法保留空格创建漂亮的缩进等其他一些有用的东西。这些helpers方法都在Haml::Helpers和Haml::ActionViewExtensions这两个modules里。
**Haml Options**
Options can be set by setting the hash Haml::Template.options from environment.rb in Rails, or by passing an options hash to Haml::Engine. Available options are: 
可以在Rails的environment.rb文件中通过设置Haml::Template.options的hash来设置Options，或者通过传一个hash到Haml::Engine里来设置。也就是你可以设置如下option来自定义haml，可用的options如下所示：
**:suppress_eval**
**:attr_wrapper**
**:filename**
**:filters**
**:locals**
