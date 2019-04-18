# HTML相关 - weixin_33985507的博客 - CSDN博客
2017年06月14日 12:27:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
## HTML, XML, XHTML的区别
HTML 指的是超文本标记语言 (Hyper Text Markup Language)，用来**传输和存储数据**。
XML 指可扩展标记语言（EXtensible Markup Language），用来**显示数据**。
XHTML 指可扩展超文本标记语言（EXtensible HyperText Markup Language），是**更严格更纯净的 HTML 代码**。
## HTML语义化
###### 什么是HTML语义化
根据网页结构选择合适的标签便于开发者阅读和写出更优雅的代码，同时让浏览器的爬虫和机器更好的进行解析。
###### 为什么要语义化
- 没有CSS的时候，也可以很好的呈现出页面结构、页面内容；
- 有利于SEO: 和搜索引擎之间建立良好的沟通，有助于爬虫抓取更多有效的信息，爬虫依赖于标签来确定上下文和各个关键字的权重；
- 考虑到代码的可复用性，可移植性，方便其他设备的解析执行；
- 代码可读性强，便于维护；
## 怎样理解内容与样式分离的原则
内容与样式分离，就是让内容（HTML）与样式（CSS） 分开写，不要混着用。这样就不用再写大把的行内样式，而是写成单独的样式文件。这样做的好处是可以提高代码的复用率，而且便于后期维护。
## <meta>
[参考文档](https://link.jianshu.com?t=https://segmentfault.com/a/1190000002407912)
> 
<meta> 元素可提供有关页面的元信息（meta-information），比如针对搜索引擎和更新频度的描述和关键词。
<meta> 标签位于文档的头部，不包含任何内容。<meta> 标签的属性定义了与文档相关联的名称/值对。
##### 必要属性
|属性|值|描述|
|----|----|----|
|content|some text|定义与http-equiv或name属性相关的元信息|
##### 可选属性
|属性|值|描述|
|----|----|----|
|http-equiv|content-type / expire / refresh / set-cookie|把content属性关联到HTTP头部。|
|name|author / description / keywords / generator / revised / others|把 content 属性关联到一个名称。|
|content|some text|定义用于翻译 content 属性值的格式|
##### 常见的<meta>标签
- SEO优化
|标签|功能|
|----|----|
|<meta name="keywords" content="your tags">|为搜索引擎提供参考，网页内容所包含的核心搜索关键词。|
|<meta name="description" content="150 words" />|为搜索引擎提供参考，网页的描述信息;搜索引擎采纳后，作为搜索结果中的页面摘要(snippet)显示|
|<meta http-equiv="refresh" content="0;url=" />|页面重定向和刷新content内的数字代表时间（秒），既多少时间后刷新。如果加url,则会重定向到指定网页（搜索引擎能够自动检测，也很容易被引擎视作误导而受到惩罚）|
|<meta name="author" content="author name" />|定义网页作者|
- 移动设备
|标签|功能|
|----|----|
|<meta name="viewport" content="width=device-width, initial-scale=1.0,maximum-scale=1.0, user-scalable=no"/>|移动端适配，能优化移动浏览器的显示|
|<meta name="apple-mobile-web-app-capable" content="yes" />|WebApp全屏模式：伪装app，离线应用。|
|<meta name="apple-mobile-web-app-status-bar-style" content="black-translucent" />|隐藏状态栏/设置状态栏颜色：只有在开启WebApp全屏模式时才生效。content的值为default / black /black-translucent|
|<meta content="email=no" name="format-detection" />|忽略识别邮箱|
|<meta content="telephone=no" name="format-detection" />|忽略数字自动识别为电话号码|
- 网页相关
|标签|功能|
|----|----|
|<meta charset="utf-8">|声明编码方式|
|meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1" />|优先使用IE的最新版本和Chrome|
|<meta http-equiv="X-UA-Compatible" content="IE=6" >|使用IE6|
|<meta http-equiv="X-UA-Compatible" content="IE=7" >|使用IE7|
|<meta http-equiv="X-UA-Compatible" content="IE=8" >|使用IE8|
## <!DOCTYPE HTML>
告诉浏览器采用哪种规范来解析文档
- 
严格模式：又称标准模式，是指浏览器按照 W3C 标准解析代码。
- 
混杂模式：又称怪异模式或兼容模式，是指浏览器用自己的方式解析代码。
## 浏览器乱码的原因是什么？如何解决
产生乱码的原因一般是文档没有声明自身的编码方式或者文件保存的编码方式与声明的编码方式不一致，导致浏览器错误地使用了与文档不同的解码方式，从而导致内容无法正常显示，一定要清楚文件保存时的编码方式并在文件内声明。
## 常见的浏览器有哪些，什么内核
Chrome浏览器 Webit内核
IE浏览器 自家IE内核
国产诸如QQ浏览器/搜狗浏览器/360浏览器之类均为双内核
