# URL 重写新手指南 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Delostik](http://www.jobbole.com/members/delostik) 翻译，[牧师](http://www.jobbole.com/members/q69039280) 校稿。未经许可，禁止转载！
英文出处：[addedbytes](https://www.addedbytes.com/articles/for-beginners/url-rewriting-for-beginners/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
### URL重写是什么？
大多数动态网站的URL中都含有变量，以告知站点哪些信息需要展示给用户。比如像下面这个URL，会通知相关的脚本加载编号为7的产品：

XHTML
```
http://www.example.com/show_a_product.php?product_id=7
```
这种URL结构的问题在于它并不容易记忆。如果是在电话中也很难读出来（令人惊讶的是有很多人通过这种方式传递URL）。搜索引擎和用户都不能从URL中得到有用的内容信息。你没办法从URL看出在这个页面能买挪威的蓝鹦鹉（的羽毛）。这种是相当标准的URL——也就是一般你在CMS网站上看到的那种。相比下面的URL：

XHTML
```
http://www.example.com/products/7/
```
这种URL更清晰，也更短，更容易记忆，也更容易被念出来。尽管如此，它还是不能告诉别人它所指向的内容是什么。但是我们可以更进一步：

XHTML
```
http://www.example.com/parrots/norwegian-blue/
```
这就是我们要的东西了。即使不看上下文，我们也可以从这个URL中看出你要找的东西就在这个页面上。搜索引擎可以将这个URL分割成单词（搜索引擎会将URL中的连字符当做空格，但是下划线不是），然后根据这些信息更好地判断页面内容。这种URL是易于记忆和传递的。
不幸的是，要让服务器理解最后一种URL，需要我们做一番工作。当URL发起一个请求，服务器需要知道如何处理URL，才能知道该返回给用户什么内容。URL重写就是这种将最后一种URL“翻译”成服务器能理解的语言的技术。
### 平台和工具
根据你的服务器上运行的软件，你可能已经有URL重写模块。如果没有，大多主机都提供启用或安装相关模块的功能，你可以尝试启用它。
Apache启用URL重写是最简单的，它通常带有自己的内建URL重写模块——mod_rewrite，启动和使用mod_rewrite就像上传和命名文件一样简单。
IIS是微软的服务器软件，标配并不包含URL重写的能力，但是有很多插件提供了这种功能。[ISAPI_Rewrite](http://www.isapirewrite.com/) 是我比较推荐一款插件，这是我发现的一款功能最接近mod_rewrite的插件。在这篇文章的最后附有ISAPI_Rewrite的安装和配置说明。
下面的代码是一些使用mod_rewrite的例子。
### 基本的URL重写
首先我们来看一个简单的例子。我们有一个网站，含有一个单独的PHP脚本，展示了一个单独的页面，URL如下：

XHTML
```
http://www.example.com/pet_care_info_07_07_2008.php
```
我们想要简化URL，理想的URL像这样：

XHTML
```
http://www.example.com/pet-care/
```
为了让这个URL有效，我们需要让服务器在内部将所有的“pet-care”的请求重定向到“pet_care_info_07_07_2008.php”。我们希望这个工作在内部进行是因为我们不希望用户浏览器的地址栏发生改变。
为了达到这个目的，我们首先需要建立一个名为“.htaccess”的文本文档来存储我们的规则。这个文件必须命名成这样（不能是“.htaccess.txt”或者“rules.htaccess”）。这个文件应该放在服务器的根目录（本例中放在与 “pet_care_info_07_07_2008.php” 相同的目录中）。可能那里已经有一个.htaccess文件了，这时我们就应该编辑这个文件而不是覆盖它。
.htaccess文件是服务器的配置文件。如果文件中有错误，服务器会提示错误信息（通常错误代码是500）。如果使用FTP协议向服务器发送文件，必须使用ASCII编码传输，而不是BINARY。在本例中，这个文件有两个作用：1. 通知Apache启动重写引擎； 2. 把我们的重写规则告诉Apache。因此我们需要在这个文件中加入以下内容：


```
RewriteEngine On # Turn on the rewriting engine RewriteRule ^pet-care/?$ pet_care_info_01_02_2008.php [NC,L] # Handle requests for "pet-care"
```
有几个需要注意的地方：在.htaccess文件中，‘#’之后的文字都会被当做注释忽略，建议大家多使用注释；“RewriteEngine”这一行在每个.htaccess文件中应当只使用一次（请注意在后面的代码中都不包括这一行）。
“RewriteRule”行是见证奇迹的地方。这一行可以分为五个部分：
- RewriteRule —— 通知Apache这是一条独立的RewriteRule。
- ^/pet-care/?$ —— 模式串，服务器会检查每一条URL是否跟这个模式串匹配。如果匹配，Apache会使用后面的Substitution块替换URL。
- pet_care_info_01_02_2003.php —— Substitution。如果请求与前面的模式串匹配，则Apache会使用这个URL替换原来的URL。
- [NC,L] —— 标记，告诉Apache如何应用规则。在本例中，我们使用了两个标记：“NC”告诉Apache这条规则不区分大小写；“L”告诉Apache如果这条规则被应用，则不再应用其他规则。
- # Handle requests for “pet-care” —— 注释，用来解释这条规则做了什么（可选，但是建议这样做）
这种规则是重写一个独立URL的简单方式，也是几乎所有URL重写的基础。
### 模式和替换
前面提到的规则使你可以重定向单独的URL，但是mod_rewrite的强大之处在于根据包涵的模式串，识别和重写成组的URL。
现在我们要把站点上所有的URL改成像前面举得例子中说的那样。你现在的URL是这样的：

XHTML
```
http://www.example.com/show_a_product.php?product_id=7
```
你想把它改成这个样子：

XHTML
```
http://www.example.com/products/7/
```
我们可以只写一条规则来管理所有产品id，而不用对每个id都写一条规则。其实就是你想把这种类型的URL：

XHTML
```
http://www.example.com/show_a_product.php?product_id={a number}
```
改成这种样子的：

XHTML
```
http://www.example.com/products/{a number}/
```
为了达到这个目的，你需要使用正则表达式。正则表达式是一种格式特殊的、方便服务器理解的pattern。一种典型的用来匹配数字的正则表达式像这样：


```
[0-9]+
```
方括号内含有一系列的字符，“0-9”表示所有数字。加号表示将会匹配任何加号前出现的pattern——本例中也就是“一个或多个数字”——也就是我们要在URL中寻找的东西。
整个的模式部分一般都会被当做正则表达式进行处理——你不需要启动或激活它们。


```
RewriteRule ^products/([0-9]+)/?$ show_a_product.php?product_id=$1 [NC,L] # Handle product requests
```
首先应该注意的是用括号括起来的模式串，这样我们可以在接下来的Substitution中使用被括号中pattern匹配的URL进行“back-reference”（向后引用）。Substitution中的“$1”告诉Apache将之前被括号里面的pattern匹配到的URL字串放到这里。你可以有很多back-reference，他们按出现顺序编号。
像上面的RewriteRule语句，将会使Apache把所有domain.com/products/{number}/的请求重定向到show_a_product.php?product_id={same number}。
### 正则表达式
本文要讲的并不是完整的正则表达式的指南。然而，需要注意的重点是整个pattern都会被当做正则表达式处理，要一直注意正则表达式中那些特殊的字符。
最典型的例子是在pattern中使用句号。在一个patter中，’.’ 表示“任意字符”，而不是一个普通的句号，所以当你想要匹配一个句号的时候，你需要对句号进行“转义”——就是在它前面加一个特殊的符号，反斜杠，它会让Apache将下一个字符当做普通字符处理。
比如，下面这条语句 ，将会匹配”rss1xml”、”rss-xml”这样的URL：


```
RewriteRule ^rss.xml$ rss.php [NC,L] # Change feed URL
```
这样做一般不会有什么很严重的问题，但是对字符进行适当转义的习惯对深入学习正则表达式有好处。所以最好的写法应该是这样：


```
RewriteRule ^rss\.xml$ rss.php [NC,L] # Change feed URL
```
这种情况只适用于模式比配，而不能用于替换。此外还有其他的字符（我们称之为“元字符”）用于转义：
- . （任意字符）
- *（零个或多个字符）
- +（一个或多个字符）
- {}（匹配至少、至多个字符）
- ? （非贪婪限制符，跟在任何一个限制符后面表示该
- ! （负向预查，出现在字符串头表示从不匹配该字符串的位置开始匹配）
- ^（匹配输入字符串的开始位置，出现在范围中表示负）
- $（表示字符串结尾）
- []（包含,默认是一个字符长度）
- – （当在方括号中出现时表示范围）
- () （段域）
- | （对两个匹配条件进行逻辑或运算）
- \（将下一个字符转义）
通过使用正则表达式，我们可以匹配任意URL并且重写它们。现在回到我们文章开头提到的例子，我们希望匹配并重写这条URL：

XHTML
```
http://www.example.com/parrots/norwegian-blue/
```
我们想要把这条URL翻译成如下的格式交给服务器：

XHTML
```
http://www.example.com/get_product_by_name.php?product_name=norwegian-blue
```
我们可以用很简单一条规则完成这个工作：


```
RewriteRule ^parrots/([A-Za-z0-9-]+)/?$ get_product_by_name.php?product_name=$1 [NC,L] # Process parrots
```
这个规则让我们可以提取出URL中“parrot/”之后的任意字母、数字和连字符的组合([A-Za-z0-9-])（将连字符放在字符末尾，方括号的最后，使之被当做连字符处理，而不是分隔符），并将匹配到的产品名称替换为$1.
如果需要的话，我们也可以让规则更普适，使得不管产品出现在哪个目录下，都可以发送给相同的脚本，就像：


```
RewriteRule ^[A-Za-z-]+/([A-Za-z0-9-]+)/?$ get_product_by_name.php?product_name=$1 [NC,L] # Process all products
```
像这样，我们将“parrots”替换为任意字母和连字符的组合。现在这条规则可以匹配任意在parrots目录下或任何以一个或多个字母和连字符组成的名称的目录下的产品了。
### 修正符
修正符跟在重写规则的最后，用来告知Apache如何解释和处理规则。比如可以告诉Apache处理规则时不区分大小写，当遇到第一个匹配时终止匹配，或其他更多的选项。修正符用逗号隔开，并写在方括号里。下面是一些修正符和他们的含义（关于这些符号有一份速查表，不需要全部记住）。
- C （与下一条规则关联）
- CO（设置特殊Cookie）
- E=var:value （设置环境变量var: value）
- F （禁用URL,返回403HTTP状态码）
- G （强制URL为GONE，返回410HTTP状态码）
- H=handler （设置handler）
- L （表明当前规则是最后一条规则，停止分析以后规则的重写）
- N （重新从第一条规则开始运行重写过程）
- NC （不区分大小写）
- NE （不在输出转义特殊字符）
- NS （只用于不是内部子请求）
- P （强制使用代理转发）
- PT （移交给下一个处理器– 当使用多个处理器对url进行处理的情况下使用，如mod_alias）
- R （强制外部重定向）
- R=301 （强制重定向到新URL）
- QSA （追加请求字符串）
- S=x （跳过接下来x个规则）
- T=mime-type （强制指定MIME类型）
### 移动内容


```
RewriteRule ^article/?$ http://www.new-domain.com/article/ [R,NC,L] # Temporary Move
```
在修正符段添加R修正符可以改变RewriteRule的工作方式。这种情况下Apache会向浏览器返发送一条信息（一个HTTP头），告诉浏览器内容已经被临时移动到了替换块中的URL处，而不是在内部进行URL重写。替换块内可以是绝对的URL也可以是相对的。HTTP头中还包含了302代码，说明移动是临时的。


```
RewriteRule ^article/?$ http://www.new-domain.com/article/ [R=301,NC,L] # Permanent Move
```
如果移动是永久的，给“R”修饰符添加“=301”字段，Apache会告诉浏览器内容被永久移动。与默认的R修饰符不同的是，“R=301”也会使浏览器地址栏显示新的地址。
这是最常见的一种URL重写的方法来把内容移动到新的 URL（比如在本网站中就被广泛使用，当文章地址改变时，会将用户带到新的地址去）。
### 条件
重写规则可以在一个或多个重写条件下进行，而且可以串联，借此我们可以对一些请求只使用一部分重写。就我个人而言，我最常把这个规则应用到子域或替代域，它可以满足各种各样的标准,不仅仅是URL的。举个例子:


```
RewriteCond %{HTTP_HOST} ^addedbytes\.com [NC] 
RewriteRule ^(.*)$ http://www.addedbytes.com/$1 [L,R=301]
```
上面这条重写规则重定向所有请求到“www.addedbytes.com”。如果没有这个条件，这个规则将会产生一个循环，所有匹配的请求都会被送回给自己。规则的目的是只重定向URL中缺少“www”的请求，重写条件可以帮助我们达成目的。
重写条件和重写规则的使用方法差不多。首先写“RewriteCond”告诉mod_rewrite这一行定义了一个条件。接下来是TestString和测试的模式串。最后是方括号内的修正符，跟RewriteRule的写法差不多。
TestString（条件语句的第二部分）可以表示很多不同的东西。比如在上面的例子中，你可以检测被请求的域，可以检测用户使用的浏览器，可以检测引用URL（通常用来防止盗链），检测用户的IP地址，或者检测其他的东西（参考“服务器变量”一节了解其工作方式）。
模式串跟RewriteRule中的差不多一样，但是有几个小的例外。如果模式串的开始是一个特殊的字符（在“异常”一节定义的），那么模式串将不会被解释成一个匹配模式。这意味着如果你想要在正则表达式中使用用”<“，”>”，或者连字符开头的字符串，你得给他们加一个反斜线用来转义。
重写条件后面也可以像重写规则那样加修正符，但是只有两个。“NC”跟RewriteRule一样，告诉Apache处理条件时忽略大小写。另一个修正符是“OR”，如果你想在有一两条条件匹配时就应用规则，而不是全部都满足，在第一条条件后加入“OR”修正符（只有两个条件的情况下），这样只需要有其中一条满足，规则就会被应用。默认行为是在多个条件下，只有所有都满足的情况下才能够应用规则。
### 异常和特例
重写条件有很多不同的方式进行检测——并不需要当做正则表达式的模式串，虽然正则表达式很常用。下面是一些处理重写条件的方法选项：
- <Pattern（是否测试串比模式串小）
- >Pattern（是否测试串比模式串大）
- =Pattern（测试串和模式串是否相等）
- -d （测试串是否是一个合法的目录）
- -f  （测试串是否是一个合法的文件）
- -s （测试串是否是一个合法的文件且大小不为0）
- -l  （测试串文件是否存在且是一个符号链接）
- -F  （通过subrequest来检查某文件是否可访问）
- -U （通过subrequest来检查URL是否合法且可访问）
### 服务器变量
服务器变量是在重写条件中可以被检测的一些项目。这使得你可以根据所有的请求参数——包括浏览器标识、引用URL或其他的字符串——来应用适当的规则。变量格式如下：
- HTTP Headers
- HTTP_USER_AGENT
- HTTP_REFERER
- HTTP_COOKIE
- HTTP_FORWARDED
- HTTP_HOST
- HTTP_PROXY_CONNECTION
- HTTP_ACCEPT
- Connection Variables
- REMOTE_ADDR
- REMOTE_HOST
- REMOTE_USER
- REMOTE_IDENT
- REQUEST_METHOD
- SCRIPT_FILENAME
- PATH_INFO
- QUERY_STRING
- AUTH_TYPE
- Server Variables
- DOCUMENT_ROOT
- SERVER_ADMIN
- SERVER_NAME
- SERVER_ADDR
- SERVER_PORT
- SERVER_PROTOCOL
- SERVER_SOFTWARE
- Dates and Times
- TIME_YEAR
- TIME_MON
- TIME_DAY
- TIME_HOUR
- TIME_MIN
- TIME_SEC
- TIME_WDAY
- TIME
- Special Items
- API_VERSION
- THE_REQUEST
- REQUEST_URI
- REQUEST_FILENAME
- IS_SUBREQ
### 多规则应用
越复杂的网站，就会有越复杂的规则来管理。当规则产生冲突的时候，行为是不确定的。往往在添加一条新的规则之后，会出现一些莫名其妙的问题，比如根本不起作用。如果这条规则本身是没有问题的，那可能是之前有一条规则匹配到了这个URL，所以这条URL根本没有被匹配到新加入的规则。


```
RewriteRule ^([A-Za-z0-9-]+)/([A-Za-z0-9-]+)/?$ get_product_by_name.php?category_name=$1&product_name=$2 [NC,L] # Process product requests 
RewriteRule ^([A-Za-z0-9-]+)/([A-Za-z0-9-]+)/?$ get_blog_post_by_title.php?category_name=$1&post_title=$2 [NC,L] # Process blog posts
```
在这个例子中，产品页面和blog页面有不同的模式串，但是第二条规则将不会匹配到URL，因为所有能被匹配的模式都已经被第一条规则匹配到了。
解决这个问题的方式有很多。很多CMS（包括wordpress）通过在URL中增加一个表示请求类型的串来供规则匹配，比如：


```
RewriteRule ^products/([A-Za-z0-9-]+)/([A-Za-z0-9-]+)/?$ get_product_by_name.php?category_name=$1&product_name=$2 [NC,L] # Process product requests 
RewriteRule ^blog/([A-Za-z0-9-]+)/([A-Za-z0-9-]+)/?$ get_blog_post_by_title.php?category_name=$1&post_title=$2 [NC,L] # Process blog posts
```
你也可以写一个单独的PHP页面来处理所有请求，它可以检查URL的第二个部分是否能匹配上一个blog或者一个产品。我通常这样做，虽然可能会给服务器带来一些额外的负担，但是他让URL更加简洁。


```
RewriteRule ^([A-Za-z0-9-]+)/([A-Za-z0-9-]+)/?$ get_product_or_blog_post.php?category_name=$1&item_name=$2 [NC,L] # Process product and blog requests
```
还可以通过设计更精确的规则和对规则进行更合理的安排来解决这个问题。想象一个blog有两个分类集——主题和发布年份。


```
RewriteRule ^([A-Za-z0-9-]+)/?$ get_archives_by_topic.php?topic_name=$1 [NC,L] # Get archive by topic 
RewriteRule ^([A-Za-z0-9-]+)/?$ get_archives_by_year.php?year=$1 [NC,L] # Get archive by year
```
上面这两条规则会冲突。当然，年份只由四位数字组成，所以你可以吧规则写的更精确，这样只有在主题名称也是四位数字的时候才会产生冲突。


```
RewriteRule ^([0-9]{4})/?$ get_archives_by_year.php?year=$1 [NC,L] # Get archive by year 
RewriteRule ^([A-Za-z0-9-]+)/?$ get_archives_by_topic.php?topic_name=$1 [NC,L] # Get archive by topic
```
mod_rewrite
Apache的mod_rewrite模块在大多Apache托管中是标配，如果你使用共享的托管服务，你不需要做什么配置。但是如果你是在管理自己的空间，你需要启用mod_rewrite模块。如果你在使用Apache1，你得修改httpd.conf文件，去掉下面这行行首的”#”


```
#LoadModule rewrite_module modules/mod_rewrite.so #AddModule mod_rewrite.c
```
如果在类Debian发行版上用Apache2，你只需要使用一下命令并重启Apache：


```
sudo a2enmod rewrite
```
其他发行版或其他平台可能不太一样。如果上面这两种方法都不适用于你的系统，那就去google一下吧。可能需要修改Apache2的配置文件，把“rewrite”加入到APACHE_MODULES列表里，或者要修改httpd.conf，实在不行就下载mod_rewrite的源码自己编译安装。这些方法都不麻烦的。
### ISAPI_Rewrite
[ISAPI_Rewrite](http://www.isapirewrite.com/)是IIS上一个基于mod_rewrite的插件，它跟mod_rewrite的功能差不多，而且还有一些高质量的[ISAPI_Rewrite论坛](http://www.isapirewrite.com/forum/forum_topics.asp?FID=10)用来交流释疑。因为ISAPI_Rewrite是IIS上的，[安装](http://www.isapirewrite.com/docs/#install)也非常简单。
ISAPI_Rewrite的规则默认写在httpd.ini文件中，错误日志在httpd.parse.errors文件中。
### 正斜杠
在实际中我经常被URL重写中的正斜杠所困扰，不管在模式串中、RewriteRule的替换串中还是RewriteCond的状态中，都会困扰我。这可能是由于我经常面对不同的URL重写引擎，然而我仍然建议大家——当一个规则无效时，先注意一下是不是正斜杠搞的鬼。我通常在mod_rewrite规则中避免使用斜杠，但是在ISAPI_Rewrite中会使用。
### 示例规则
把旧的域重定向到一个新的域：


```
RewriteCond %{HTTP_HOST} old_domain\.com [NC] 
RewriteRule ^(.*)$ http://www.new_domain.com/$1 [L,R=301]
```
重定向缺少“www”的请求（添加“www”）：


```
RewriteCond %{HTTP_HOST} ^domain\.com [NC] 
RewriteRule ^(.*)$ http://www.domain.com/$1 [L,R=301]
```
重定向所有含有“www”的网页（去掉“www”）：


```
RewriteCond %{HTTP_HOST} ^www\.domain\.com [NC] 
RewriteRule ^(.*)$ http://domain.com/$1 [L,R=301]
```
把旧页面重定向到新页面：


```
RewriteRule ^old-url\.htm$ http://www.domain.com/new-url.htm [NC,R=301,L]
```
