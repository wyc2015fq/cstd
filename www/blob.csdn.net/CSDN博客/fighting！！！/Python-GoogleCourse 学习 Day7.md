# Python-GoogleCourse 学习 Day7 - fighting！！！ - CSDN博客
2019年02月27日 12:33:33[dujiahei](https://me.csdn.net/dujiahei)阅读数：18
**目录**
[参考：](#%E5%8F%82%E8%80%83%EF%BC%9A)
[知识点](#%E7%9F%A5%E8%AF%86%E7%82%B9)
## 参考：
[http://www.runoob.com/regexp/regexp-tutorial.html](http://www.runoob.com/regexp/regexp-tutorial.html)
[http://www.runoob.com/regexp/regexp-example.html](http://www.runoob.com/regexp/regexp-example.html)
## 知识点
- 正则表达式
	正则表达式是一个特殊的字符序列，它能帮助你方便的检查一个字符串是否与某种模式匹配。
	Python 自1.5版本起增加了re 模块，它提供 Perl 风格的正则表达式模式。
	re 模块使 Python 语言拥有全部的正则表达式功能。
	compile 函数根据一个模式字符串和可选的标志参数生成一个正则表达式对象。该对象拥有一系列方法用于正则表达式匹配和替换。
	re 模块也提供了与这些方法功能完全一致的函数，这些函数使用一个模式字符串做为它们的第一个参数。
	- 简介
		通过理解*通配符的工作原理，引入正则表达式所依赖的概念。​
		​正则表达式(Regular Expression)是一种文本模式，包括普通字符（例如，a 到 z 之间的字母）和特殊字符（称为"元字符"）。
		正则表达式使用单个字符串来描述、匹配一系列匹配某个句法规则的字符串。
		-为什么使用正则表达式：​1. 测试字符串内的模式。例如，可以测试输入字符串，以查看字符串内是否出现电话号码模式或信用卡号码模式。这称为数据验证。2. 替换文本。可以使用正则表达式来识别文档中的特定文本，完全删除该文本或者用其他文本替换它。
		3. 基于模式匹配从字符串中提取子字符串。可以查找文档内或输入域内特定的文本。
- 语法
		正则表达式(regular expression)描述了一种字符串匹配的模式（pattern），可以用来检查一个串是否含有某种子串、将匹配的子串替换或者从某个串中取出符合某个条件的子串等。
		构造正则表达式的方法和创建数学表达式的方法一样。也就是用多种元字符与运算符可以将小的表达式结合在一起来创建更大的表达式。正则表达式的组件可以是单个的字符、字符集合、字符范围、字符间的选择或者所有这些组件的任意组合。
		正则表达式是由普通字符（例如字符 a 到 z）以及特殊字符（称为"元字符"）组成的文字模式。模式描述在搜索文本时要匹配的一个或多个字符串。正则表达式作为一个模板，将某个字符模式与所搜索的字符串进行匹配。
		​
		- 普通字符
			普通字符包括没有显式指定为元字符的所有可打印和不可打印字符。这包括所有大写和小写字母、所有数字、所有标点符号和一些其他符号。
- 非打印字符
			
![](https://img.mubu.com/document_image/7525bd29-07d9-45d6-add1-31a2f4228be5-2329912.jpg)
- 特殊字符
			
![](https://img.mubu.com/document_image/886bdfc1-a714-49c6-b3d7-529e19c22ddf-2329912.jpg)
			所谓特殊字符，就是一些有特殊含义的字符，如上面说的 runoo*b 中的 *，简单的说就是表示任何字符串的意思。如果要查找字符串中的 * 符号，则需要对 * 进行转义，即在其前加一个 \: runo\*ob 匹配 runo*ob。
			许多元字符要求在试图匹配它们时特别对待。若要匹配这些特殊字符，必须首先使字符"转义"，即，将反斜杠字符\ 放在它们前面。
- 限定符
			
![](https://img.mubu.com/document_image/a05283e3-82a2-4235-9fa5-25cac2ec4053-2329912.jpg)
			限定符用来指定正则表达式的一个给定组件必须要出现多少次才能满足匹配。有 * 或 + 或 ? 或 {n} 或 {n,} 或 {n,m} 共6种。
			由于章节编号在大的输入文档中会很可能超过九，所以您需要一种方式来处理两位或三位章节编号。限定符给您这种能力。
- 定位符
			
![](https://img.mubu.com/document_image/bc5882cc-8d56-4d20-a1c0-2255f318b77b-2329912.jpg)
			定位符使您能够将正则表达式固定到行首或行尾。它们还使您能够创建这样的正则表达式，这些正则表达式出现在一个单词内、在一个单词的开头或者一个单词的结尾。
			定位符用来描述字符串或单词的边界，^ 和 $ 分别指字符串的开始与结束，\b 描述单词的前或后边界，\B 表示非单词边界。
- 选择
			用圆括号将所有选择项括起来，相邻的选择项之间用|分隔。但用圆括号会有一个副作用，使相关的匹配会被缓存，此时可用?:放在第一个选项前来消除这种副作用。
			其中 ?: 是非捕获元之一，还有两个非捕获元是 ?= 和 ?!，这两个还有更多的含义，前者为正向预查，在任何开始匹配圆括号内的正则表达式模式的位置来匹配搜索字符串，后者为负向预查，在任何开始不匹配该正则表达式模式的位置来匹配搜索字符串。
- 反向引用
			对一个正则表达式模式或部分模式两边添加圆括号将导致相关匹配存储到一个临时缓冲区中，所捕获的每个子匹配都按照在正则表达式模式中从左到右出现的顺序存储。缓冲区编号从 1 开始，最多可存储 99 个捕获的子表达式。每个缓冲区都可以使用 \n 访问，其中 n 为一个标识特定缓冲区的一位或两位十进制数。
			可以使用非捕获元字符 ?:、?= 或 ?! 来重写捕获，忽略对相关匹配的保存。
			反向引用的最简单的、最有用的应用之一，是提供查找文本中两个相同的相邻单词的匹配项的能力。
- 元字符
- 运算符优先级
		
![](https://img.mubu.com/document_image/6e025af0-94c0-40da-a8b0-2bfea25767c2-2329912.jpg)
		正则表达式从左到右进行计算，并遵循优先级顺序，这与算术表达式非常类似。
		相同优先级的从左到右进行运算，不同优先级的运算先高后低。
- 匹配规则
		- 基本模式匹配
			一切从最基本的开始。模式，是正则表达式最基本的元素，它们是一组描述字符串特征的字符。模式可以很简单，由普通的字符串组成，也可以非常复杂，往往用特殊的字符表示一个范围内的字符、重复出现，或表示上下文。
			这个模式包含一个特殊的字符^，表示该模式只匹配那些以once开头的字符串。​
- 字符簇
			在INTERNET的程序中，正则表达式通常用来验证用户的输入。当用户提交一个FORM以后，要判断输入的电话号码、地址、EMAIL地址、信用卡号码等是否有效，用普通的基于字面的字符是不够的。
			所以要用一种更自由的描述我们要的模式的办法，它就是字符簇。要建立一个表示所有元音字符的字符簇，就把所有的元音字符放在一个方括号里。
- 确定重复出现
			到现在为止，你已经知道如何去匹配一个字母或数字，但更多的情况下，可能要匹配一个单词或一组数字。一个单词有若干个字母组成，一组数字有若干个单数组成。跟在字符或字符簇后面的花括号({})用来确定前面的内容的重复出现的次数。
- re模块
	- re.match函数
		re.match 尝试从字符串的起始位置匹配一个模式，如果不是起始位置匹配成功的话，match()就返回none。
		函数语法：
		​​re.match(pattern, string, flags=0)
- re.search方法
		re.search 扫描整个字符串并返回第一个成功的匹配。
		函数语法：
		re.search(pattern, string, flags=0)
- re.match与re.search的区别
		re.match只匹配字符串的开始，如果字符串开始不符合正则表达式，则匹配失败，函数返回None；而re.search匹配整个字符串，直到找到一个匹配。
- re.sub——检索合替换
		Python 的re模块提供了re.sub用于替换字符串中的匹配项。
		语法：
		re.sub(pattern, repl, string, count=0)参数：
		pattern : 正则中的模式字符串。
		repl : 替换的字符串，也可为一个函数。
		string : 要被查找替换的原始字符串。
		count : 模式匹配后替换的最大次数，默认 0 表示替换所有的匹配。
		- repl参数作为函数
- compile函数
			compile 函数用于编译正则表达式，生成一个正则表达式（ Pattern ）对象，供 match() 和 search() 这两个函数使用。
			语法格式为：
			re.compile(pattern[, flags])参数：
			pattern : 一个字符串形式的正则表达式
			flags 可选，表示匹配模式，比如忽略大小写，多行模式等，具体参数为：
			re.I 忽略大小写re.L 表示特殊字符集 \w, \W, \b, \B, \s, \S 依赖于当前环境
			re.M 多行模式
			re.S 即为' . '并且包括换行符在内的任意字符（' . '不包括换行符）
			re.U 表示特殊字符集 \w, \W, \b, \B, \d, \D, \s, \S 依赖于 Unicode 字符属性数据库
			re.X 为了增加可读性，忽略空格和' # '后面的注释
- findall
			在字符串中找到正则表达式所匹配的所有子串，并返回一个列表，如果没有找到匹配的，则返回空列表。
			注意： match 和 search 是匹配一次 findall 匹配所有。
			语法格式为：
			findall(string[, pos[, endpos]])参数：
			string 待匹配的字符串。
			pos 可选参数，指定字符串的起始位置，默认为 0。
			endpos 可选参数，指定字符串的结束位置，默认为字符串的长度。
- re.finditer
			和 findall 类似，在字符串中找到正则表达式所匹配的所有子串，并把它们作为一个迭代器返回。
			re.finditer(pattern, string, flags=0)
- re.split
			split 方法按照能够匹配的子串将字符串分割后返回列表，它的使用形式如下：
			re.split(pattern, string[, maxsplit=0, flags=0])
- http请求（主要是涉及用urllib.request这个包，了解一下urllib，post请求与get请求两种方法）---CGI编程
	- CGI
		- 什么是CGI
			
![](https://img.mubu.com/document_image/8f65e30a-3c3f-47cd-a175-c78a612f2a6e-2329912.jpg)
			CGI 目前由NCSA维护，NCSA定义CGI如下：
			CGI(Common Gateway Interface),通用网关接口,它是一段程序,运行在服务器上如：HTTP服务器，提供同客户端HTML页面的接口。
- CGI架构
			
![](https://img.mubu.com/document_image/57fef8b7-3868-410f-99db-bf38ee9acec7-2329912.jpg)
- 网页浏览
		为了更好的了解CGI是如何工作的，我们可以从在网页上点击一个链接或URL的流程：
		1、使用你的浏览器访问URL并连接到HTTP web 服务器。
		2、Web服务器接收到请求信息后会解析URL，并查找访问的文件在服务器上是否存在，如果存在返回文件的内容，否则返回错误信息。
		3、浏览器从服务器上接收信息，并显示接收的文件或者错误信息。
		CGI程序可以是Python脚本，PERL脚本，SHELL脚本，C或者C++程序等。
- Web服务器支持及配置
		在你进行CGI编程前，确保您的Web服务器支持CGI及已经配置了CGI的处理程序。
		​所有的HTTP服务器执行CGI程序都保存在一个预先配置的目录。这个目录被称为CGI目录，并按照惯例，它被命名为/var/www/cgi-bin目录。
		CGI文件的扩展名为.cgi，python也可以使用.py扩展名。
		默认情况下，Linux服务器配置运行的cgi-bin目录中为/var/www。
		如果你想指定其他运行CGI脚本的目录，可以修改httpd.conf配置文件
- HTTP头部
		
![](https://img.mubu.com/document_image/d727b06a-19e8-485b-9c41-0706c6c82ff5-2329912.jpg)
		它会发送给浏览器告诉浏览器文件的内容类型。
		HTTP头部的格式如下：
		HTTP 字段名: 字段内容
- CGI环境变量
- GET和POST方法
		浏览器客户端通过两种方法向服务器传递信息，这两种方法就是 GET 方法和 POST 方法。
		- 使用GET方法传输数据
			GET方法发送编码后的用户信息到服务端，数据信息包含在请求页面的URL上，以"?"号分割, 如下所示：
[http://www.test.com/cgi-bin/hello.py?key1=value1&key2=value2](http://www.test.com/cgi-bin/hello.py?key1=value1&key2=value2)
- 使用POST方法传递数据
			使用POST方法向服务器传递数据是更安全可靠的，像一些敏感信息如用户密码等需要使用POST传输数据。
- 通过CGI程序传递checkbox数据
			checkbox用于提交一个或者多个选项数据。
- 通过CGI程序传递Radio数据
			Radio 只向服务器传递一个数据。
- 通过CGI程序传递 Textarea 数据
			Textarea 向服务器传递多行数据
- 通过CGI程序传递下拉数据
- CGI中使用Cookie
		在 http 协议一个很大的缺点就是不对用户身份的进行判断，这样给编程人员带来很大的不便， 而 cookie 功能的出现弥补了这个不足。
		cookie 就是在客户访问脚本的同时，通过客户的浏览器，在客户硬盘上写入纪录数据 ，当下次客户访问脚本时取回数据信息，从而达到身份判别的功能，cookie 常用在身份校验中。
		- cookie的语法
			http cookie的发送是通过http头部来实现的，他早于文件的传递，头部set-cookie的语法如下：
			Set-cookie:name=name;expires=date;path=path;domain=domain;secure name=name: 需要设置cookie的值(name不能使用";"和","号),有多个name值时用 ";" 分隔，例如：name1=name1;name2=name2;name3=name3。
			expires=date: cookie的有效期限,格式： expires="Wdy,DD-Mon-YYYY HH:MM:SS"
			path=path: 设置cookie支持的路径,如果path是一个路径，则cookie对这个目录下的所有文件及子目录生效，例如： path="/cgi-bin/"，如果path是一个文件，则cookie指对这个文件生效，例如：path="/cgi-bin/cookie.cgi"。
			domain=domain: 对cookie生效的域名，例如：domain="[www.runoob.com](http://www.runoob.com)"
			secure: 如果给出此标志，表示cookie只能通过SSL协议的https服务器来传递。
			cookie的接收是通过设置环境变量HTTP_COOKIE来实现的，CGI程序可以通过检索该变量获取cookie信息。
- Cookie设置
		Cookie的设置非常简单，cookie会在http头部单独发送。
- 检索Cookie信息
		Cookie信息检索页非常简单，Cookie信息存储在CGI的环境变量HTTP_COOKIE中，存储格式如下：
		key1=value1;key2=value2;key3=value3....
- 文件下载对话框
		我们先在当前目录下创建 foo.txt 文件，用于程序的下载。
		文件下载通过设置HTTP头信息来实现
