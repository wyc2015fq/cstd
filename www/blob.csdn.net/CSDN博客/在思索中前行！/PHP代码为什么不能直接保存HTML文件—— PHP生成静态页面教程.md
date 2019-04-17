# PHP代码为什么不能直接保存HTML文件——>PHP生成静态页面教程 - 在思索中前行！ - CSDN博客





2014年10月25日 22:20:05[_Tham](https://me.csdn.net/txl16211)阅读数：5573







1、服务器会根据文件的后缀名去进行解析，如果是HTML文件则服务器不会进行语法解析，而是直接输出到浏览器。
2、如果一个页面中全部都是HTML代码而没有需要解析的PHP语法，则没有必要保存为PHP文件，这样反而会降低运行效率。
3、如果是需要PHP控制HTML代码的输出，比如需要PHP判断用户是否登陆，如果登陆则输出A，未登录则输出B。这就需要PHP来进行控制了。HTML不能实现这样的功能
## PHP生成静态页面教程 ，一些基本的概念


### 一，PHP脚本与动态页面。
　　PHP脚本是一种服务器端脚本程序，可通过嵌入等方法与HTML文件混合，也可以类，函数封装等形式，以模板的方式对用户请求进行处理。无论以何种方式，它的基本原理是这样的。由客户端提出请求，请求某一页面　----->　WEB服务器引入指定相应脚本进行处理　----->　脚本被载入服务器　----->　由服务器指定的PHP解析器对脚本进行解析形成HTML语言形式　---->　将解析后的HTML语句以包的方式传回给浏览器。由此不难看出，在页面发送到浏览器后，PHP就不存在了，已被转化解析为HTML语句。客户请求为一动态文件，事实上并没有真正的文件存在在那里，是PHP解析而成相对应的页面，然后发送回浏览器。这种页面处理方式被称为“动态页面”。 

### 二，静态页面。
　　静态页面是指在服务器端确实存在的仅含HTML以及JS，CSS等客户端运行脚本的页面。它的处理方式是。由客户端提出请求，请求某一页面　---->　WEB服务器确认并载入某一页面　---->　WEB服务器将该页面以包的形式传递回浏览器。由这一过程，我们对比一下动态页面，即可方现。动态页面需由WEB服务器的PHP解析器进行解析，而且通常还需连接数据库，进行数据库存取操作，然后才能形成HTML语言信息包；而静态页面，无须解析，无须连接数据库，直接发送，可大大减轻服务器压力，提高服务器负载能力，大幅提供页面打开速度和网站整体打开速度。但其缺点是，不能动态地对请求进行处理，服务器上必须确实存在该文件。 



### 三，模板及模板解析。
　　模板即尚未填充内容html文件。例如： 
　temp.html 
Code: 



```php
<HTML> 
<TITLE>{ title }</TITLE> 
<BODY> 
this is a { file } file's templets 
</BODY> 
</HTML> 
PHP处理： 
　templetest.php 
Code:   <?
$title = "拓迈国际测试模板"; 
$file = "TwoMax Inter test templet, 
author：Matrix@Two_Max"; 
　$fp = fopen ("temp.html","r"); 
$content = fread ($fp,filesize ("temp.html")); 
$content .= str_replace ("{ file }",$file,$content); 
$content .= str_replace ("{ title }",$title,$content); 
echo $content; 
?>
```

模板解析处理，即将经PHP脚本解析处理后得出的结果填充(content)进模板的处理过程。通常借助于模板类。目前较流行的模板解析类有phplib，smarty，fastsmarty等等。模板解析处理的原理通常为替换。也有些程序员习惯将判断，循环等处理放进模板文件中，用解析类处理，典型应用为block概念，简单来说即为一个循环处理。由PHP脚本指定循环次数，如何循环代入等，再由模板解析类具体实施这些操作。 
　　好了，对比过静态页面与动态页面各自的优劣，现在我们就来说说，如何用PHP生成静态文件。 
　　PHP生成静态页面并不是指PHP的动态解析，输出HTML页面，而是指用PHP创建HTML页面。同时因为HTML的不可写性，我们创建的HTML若有修改，则需删掉重新生成即可。(当然你也可以选择用正则进行修改，但个人认为那样做倒不如删掉重新生成来得快捷，有些得不偿失。) 
　　言归正传。用过PHP文件操作函数的PHP FANS知道，PHP中有一个文件操作函数fopen，即打开文件。若文件不存在，则尝试创建。这即是PHP可以用来创建HTML文件的理论基础。只要用来存放HTML文件的文件夹有写权限(即权限定义0777)，即可创建文件。（针对UNIX系统而言，Win系统无须考虑。）仍以上例为例，若我们修改最后一句，并指定在test目录下生成一个名为test.html的静态文件： 
Code: 



```php
<?php 
$title = "拓迈国际测试模板"; 
$file = "TwoMax Inter test templet, 
author：Matrix@Two_Max"; 
　$fp = fopen ("temp.html","r"); 
$content = fread ($fp,filesize ("temp.html")); 
$content .= str_replace ("{ file }",$file,$content); 
$content .= str_replace ("{ title }",$title,$content); 
// echo $content; 
$filename = "test/test.html"; 
$handle = fopen ($filename,"w"); //打开文件指针，创建文件 
/* 
　检查文件是否被创建且可写 
*/ 
if (!is_writable ($filename)){ 
die ("文件：".$filename."不可写，请检查其属性后重试！"); 
} 
if (!fwrite ($handle,$content)){ //将信息写入文件 
die ("生成文件".$filename."失败！"); 
} 
fclose ($handle); //关闭指针 
die ("创建文件".$filename."成功！"); 
?>
```

## 实际应用中常见问题解决方案参考： 

## 一，文章列表问题：
　　在数据库中创建字段，记录文件名，每生成一个文件，将自动生成的文件名存入数据库，对于推荐文章，只需指向存放静态文件的指定文件夹中的该页面即可。利用PHP操作处理文章列表，存为字符串，生成页面时替换此字符串即可。如，在页面中放置文章列表的表格加入标记{
 articletable }，而在PHP处理文件中： 
Code: 



```php
<?php 
$title = "拓迈国际测试模板"; 
$file = "TwoMax Inter test templet, 
author：Matrix@Two_Max"; 
　$fp = fopen ("temp.html","r"); 
$content = fread ($fp,filesize ("temp.html")); 
$content .= str_replace ("{ file }",$file,$content); 
$content .= str_replace ("{ title }",$title,$content); 
// 生成列表开始 
$list = ''; 
$sql = "select id,title,filename from article"; 
$query = mysql_query ($sql); 
while ($result = mysql_fetch_array ($query)){ 
$list .= ''.$result['title'].''; 
} 
$content .= str_replace ("{ articletable }",$list,$content); 
//生成列表结束 
// echo $content; 
$filename = "test/test.html"; 
$handle = fopen ($filename,"w"); //打开文件指针，创建文件 
/* 
　检查文件是否被创建且可写 
*/ 
if (!is_writable ($filename)){ 
die ("文件：".$filename."不可写，请检查其属性后重试！"); 
} 
if (!fwrite ($handle,$content)){ //将信息写入文件 
die ("生成文件".$filename."失败！"); 
} 
fclose ($handle); //关闭指针 
die ("创建文件".$filename."成功！"); 
?>
```


## 二，分页问题。
　　如我们指定分页时，每页20篇。某子频道列表内文章经数据库查询为45条，则，首先我们通过查询得到如下参数：1，总页数；2，每页篇数。第二步，for
 ($i = 0; $i < allpages; $i++)，页面元素获取，分析，文章生成，都在此循环中执行。不同的是，die ("创建文件".$filename."成功！";这句去掉，放到循环后的显示，因为该语句将中止程序执行。例： 
Code: 



```php
<?  $fp = fopen ("temp.html","r"); 
$content = fread ($fp,filesize ("temp.html")); 
$onepage = '20'; 
$sql = "select id from article where channel='$channelid'"; 
$query = mysql_query ($sql); 
$num = mysql_num_rows ($query); 
$allpages = ceil ($num / $onepage); 
for ($i = 0;$i<$allpages; $i++){ 
if ($i == 0){ 
$indexpath = "index.html"; 
} else { 
$indexpath = "index_".$i."html"; 
} 
$start = $i * $onepage; 
$list = ''; 
$sql_for_page = "select name,filename,title from article where channel='$channelid' limit $start,$onepage"; 
$query_for_page = mysql_query ($sql_for_page); 
while ($result = $query_for_page){ 
$list .= ''.$title.''; 
} 
$content = str_replace ("{ articletable }",$list,$content); 
if (is_file ($indexpath)){ 
@unlink ($indexpath); //若文件已存在，则删除 
} 
$handle = fopen ($indexpath,"w"); //打开文件指针，创建文件 
/* 
　 检查文件是否被创建且可写 
*/ 
if (!is_writable ($indexpath)){ 
echo "文件：".$indexpath."不可写，请检查其属性后重试！"; //修改为echo 
} 
if (!fwrite ($handle,$content)){ //将信息写入文件 
echo "生成文件".$indexpath."失败！"; //修改为echo 
} 
fclose ($handle); //关闭指针 
} 
fclose ($fp); 
die ("生成分页文件完成，如生成不完全，请检查文件权限系统后重新生成！"); 
?>
```
　大致思路如此，其中如其它数据生成，数据输入输出检查，分页内容指向等可酌情在页面中加入。 
　　在实际文章系统处理过程当中，还有许多问题有待考虑，与动态页面不同之处，需注意的地方还有很多。但大致思路即是如此，其它方面可举一反三而得。 
用PHP制作静态网站的模板框架 
　模板能够改善网站的结构。本文阐述如何通过PHP 4的一个新功能和模板类，在由大量静态HTML页面构成的网站中巧妙地运用模板控制页面布局。 



提纲： 
=================================== 
分离功能和布局 
避免页面元素重复 
静态网站的模板框架 
=================================== 

## 分离功能和布局
首先我们来看看应用模板的两个主要目的： 

## 分离功能（PHP）和布局（HTML） 
避免页面元素重复 
　　 第一个目的是谈论得最多的目的，它设想的情形是：一组程序员编写用于生成页面内容的PHP脚本，同时另一组设计人员设计HTML和图形以控制页面的最终外观。分离功能和布局的基本思想就是使得这两组人能够各自编写和使用独立的一组文件：程序员只需关心那些只包含PHP代码的文件，无需关心页面的外观 
；而页面设计人员可以用自己最熟悉的可视化编辑器设计页面布局，无需担心破坏任何嵌入到页面的PHP代码。 
　　 如果你曾经看过几个关于PHP模板的教程，那么你应该已经明白模板的工作机制。考虑一个简单的页面局部：页面的上方是页头，左边是导航条，其余部分是内容区域。这种网站可以拥有如下模板文件： 



```
<!-- main.htm --> 
<html> 
<head><title>模板示例</title></head> 
<body> 
<table><tr><td>{HEADER}</td></tr> 
<tr><td>{LEFTNAV}</td><td>{CONTENT}</td></tr> 
</table> 
</body></html>
```
<!--
 header.htm --> 
<img src="sitelogo.jpg"> 
<!-- leftnav.htm --> 
<br><a href="foo">Foo</a> 
<br><a href="bar">Bar</a> 
　　 可以看出页面如何由这些模板构造而成：main模板控制着整个页面的布局；header模板和leftnav模板控制着页面的公共元素。花括号“{}”里面的标识符是内容占位符。使用模板最主要的好处在于界面设计者能够按照自己的意愿编辑这些文件，比如设置字体、修改颜色和图形，或者完全地改变页面的布局。界面设计者可以用任何普通HTML编辑器或者可视化工具编辑这些页面，因为这些文件都只包含HTML代码，没有任何PHP代码。 
　 PHP代码全部保存到单独的文件中，这个文件也就是由页面URL实际调用的文件。Web服务器通过PHP引擎解析该文件，然后把结果返回给浏览器。一般地，PHP代码总是动态地生成页面内容，比如查询数据库或者执行某种计算等。下面是一个例子： 


```php
<?php 
// example.php 
require('class.FastTemplate.php'); 
$tpl = new FastTemplate('.'); 
$tpl->define( array( 'main' => 'main.htm', 
'header' => 'header.htm', 
'leftnav' => 'leftnav.htm' ) ); 
// 此处的PHP代码设置$content使其包含合适的页面内容 
$tpl->assign('CONTENT', $content); 
$tpl->parse('HEADER', 'header'); 
$tpl->parse('LEFTNAV', 'leftnav'); 
$tpl->parse('MAIN', 'main'); 
$tpl->FastPrint('MAIN'); 
?>
```

       这里我们使用的是流行的FastTemplate模板类，但其基本思路对于其他许多模板类来说都一样。首先你实例化一个类，告诉它到哪里去寻找模板文件以及哪一个模板文件与页面的哪部分对应；接下来是生成页面内容，把结果赋予内容的标识符；然后，依次解析各个模板文件，模板类将执行必要的替换操作；最后把解析结果输出到浏览器。 
　　 这个文件完全由PHP代码构成，不包含任何HTML代码，这是它最大的优点。现在，PHP程序员可以集中精力编写生成页面内容的代码，而不必为了如何生成HTML去正确地格式化最终页面而担心。 
　　 你可以使用这种方法和上面的文件构造出一个完整的网站。如果PHP代码是以URL中的查询字符串为基础生成页面内容，例如http://www.foo.com/example.php?article=099，你可以据此构造出一个完整的杂志网站。 
　　 很容易看出采用模板还有第二个好处。如上例所示，页面左边的导航条单独保存为一个文件，我们只需编辑这一个模板文件就可以改变网站所有页面左边的导航条。 
避免页面元素重复 
　　 “这确实不错”，你也许会想，“我的网站主要就是由大量的静态页面构成。现在我可以从所有页面中删除它们的公共部分，要更新这些公共部分实在太麻烦了。以后我就可以用模板制作出很容易维护的统一页面布局。”但事情并非这么简单，“大量的静态页面”道出了问题的所在。 
　　 请考虑上面的例子。这个例子实际上只有一个example.php页面，它之所以能够生成整个网站的所有页面，是因为它利用了URL中的查询字符串从数据库之类的信息源动态地构造出页面。 
　　 我们之中的大多数人所运行的网站并不一定都有数据库支持。我们的网站大多数由静态页面构成，然后用PHP在这里、那里加上一些动态功能，比如搜索引擎、反馈表单等。那么，如何在这种网站上应用模板呢？
　　 最简单的方法是为每一个页面复制一份PHP文件， 
然后在每一个页面中把PHP代码里代表内容的变量设置成合适的页面内容。例如，假设有三个页面，它们分别是主页（home）、关于（about）和产品（product），我们可以用三个文件分别生成它们。这三个文件的内容都类如： 




```php
<?php 
// home.php 
require('class.FastTemplate.php'); 
$tpl = new FastTemplate('.'); 
$tpl->define( array( 'main' => 'main.htm', 
'header' => 'header.htm', 
'leftnav' => 'leftnav.htm' ) ); 
$content = "<p>欢迎访问</p> 
<img src=\"demo.jpg\"> 
<p>希望你能够喜欢本网站</p>"; 
$tpl->assign('CONTENT', $content); 
$tpl->parse('HEADER', 'header'); 
$tpl->parse('LEFTNAV', 'leftnav'); 
$tpl->parse('MAIN', 'main'); 
$tpl->FastPrint('MAIN'); 
?>
```
 
      显然，这种方法有三个问题：我们必须为每一个页面复制这些复杂的、牵涉到模板的PHP代码，这与重复公共页面元素一样使得页面难以维护；现在文件又混合了HTML和PHP代码；为内容变量赋值将变得非常困难，因为我们必须处理好大量的特殊字符。 
　　 解决这个问题的关键就在于分离PHP代码和HTML内容，虽然我们不能从文件中删除所有的HTML内容，但可以移出绝大多数PHP代码。 
静态网站的模板框架 
　　 首先，我们象前面一样为所有的页面公用元素以及页面整体布局编写模板文件；然后从所有的页面删除公共部分，只留下页面内容；接下来再在每个页面中加上三行PHP代码，如下所示： 

```php
<?php 
<!-- home.php --> 
<?php require('prepend.php'); ?> 
<?php pageStart('Home'); ?> 
<h1>你好</h1> 
<p>欢迎访问</p> 
<img src="demo.jpg"> 
<p>希望你能够喜欢本网站</p> 
<?php pageFinish(); ?> 
?>
```

        这种方法基本上解决了前面提到的各种问题。现在文件里只有三行PHP代码，而且没有任何一行代码直接涉及到模板，因此要改动这些代码的可能性极小。此外，由于HTML内容位于PHP标记之外，所以也不存在特殊字符的处理问题。我们可以很容易地将这三行PHP代码加入到所有静态HTML页面中。 
　　 require函数引入了一个PHP文件，这个文件包含了所有必需的与模板相关的PHP代码。其中pageStart函数设置模板对象以及页面标题，pageFinish函数解析模板然后生成结果发送给浏览器。 
　　 这是如何实现的呢？为什么在调用pageFinish函数之前文件中的HTML不会发送给浏览器？答案就在于PHP 4的一个新功能，这个功能允许把输出到浏览器的内容截获到缓冲区之中。让我们来看看prepend.php的具体代码： 



```php
<?php 
require('class.FastTemplate.php'); 
function pageStart($title = '') { 
GLOBAL $tpl; 
$tpl = new FastTemplate('.'); 
$tpl->define( array( 'main' => 'main.htm', 
'header' => 'header.htm', 
'leftnav'=> 'leftnav.htm' ) ); 
$tpl->assign('TITLE', $title); 
ob_start(); 
} 
function pageFinish() { 
GLOBAL $tpl; 
$content = ob_get_contents(); 
ob_end_clean(); 
$tpl->assign('CONTENT', $content); 
$tpl->parse('HEADER', 'header'); 
$tpl->parse('LEFTNAV', 'leftnav'); 
$tpl->parse('MAIN', 'main'); 
$tpl->FastPrint('MAIN'); 
} 
?>
```
 
       pageStart函数首先创建并设置了一个模板实例，然后启用输出缓存。此后，所有来自页面本身的HTML内容都将进入缓存。pageFinish函数取出缓存中的内容，然后在模板对象中指定这些内容，最后解析模板并输出完成后的页面。 
　　 这就是整个模板框架全部的工作过程了。首先编写包含了网站各个页面公共元素的模板，然后从所有页面中删除全部公共的页面布局代码，代之以三行永远无需改动的PHP代码；再把FastTemplate类文件和prepend.php加入到包含路径，这样你就得到了一个页面布局可以集中控制的网站，它有着更好的可靠性和可维护性，而且网站级的大范围修改也变得相当容易。 

**本文下载包包含 **
了一个可运行的示例网站，它的代码注释要比前面的代码注释更详细一些。FastTemplate类可以在http://www.thewebmasters.net/找到，最新的版本号是1.1.0，那里还有一个用于保证该类在PHP
 4中正确运行的小补丁。本文下载代码中的类已经经过该补丁的修正。 
PHP简易生成静态页面 



```php
<?php 
/* 
* 文件名：index.php 
*/ 
require "conn.php"; 
$query = "select * from news order by datetime desc"; 
$result = mysql_query($query); 
?> 
<html> 
<head> 
<meta http-equiv="Content-Type" content="text/html; charset=??????"> 
<title>NEWS</title> 
</head> 
<body> 
<table width="500" border="1" align="center"> 
<tr> 
<td>标题</td> 
<td width="200">发布时间</td> 
</tr> 
<? 
while($re = mysql_fetch_array($result)){ 
?> 
<tr> 
<td><a href="<?= $re["newsid"].".html"?>"><?= $re["title"]?></a></td> 
<td><?= $re["datetime"]?></td> 
</tr> 
<? 
} 
?> 
<tr> 
<td> </td> 
<td><a href="addnews.php">添加新闻</a></td> 
</tr> 
</table> 
</body> 
</html>
```

```php
<?php 
/* 
文件名：AddNews.php 
简易动态添加生成静态新闻页面 
# 
# 表的结构 `news` 
# 
CREATE TABLE `news` ( 
`newsid` int(11) NOT NULL auto_increment, 
`title` varchar(100) NOT NULL default '', 
`content` text NOT NULL, 
`datetime` datetime NOT NULL default '0000-00-00 00:00:00', 
KEY `newsid` (`newsid`) 
) TYPE=MyISAM AUTO_INCREMENT=11 ; 
*/ 
?>
```

用PHP生成静态网页的两个函数 
在最近几年，万维网（也称环球信息网，即WWW）不断改变信息处理技术的面貌。WEB已经快速地成为一种有效的媒介，并适合人们和商业沟通和协作。几乎所有的信息技术领域都普遍受到WEB的影响。Web访问带来更多用户和更多数据，这意味着给服务器和数据库更多压力和最终用户得到越来越慢的响应速度。与不断靠增加CPU，磁盘驱动器及内存来跟上这种增长的需求相比，
 WEB动态网页面静态化应该是一个更实用，更经济的选择。 
用PHP实现WEB动态网页静态化的具体实现函数如function gen_static_file()所示 



```php
function gen_static_file($program, $filename) 
{ 
$program 1= "/usr/local/apache/htdocs/php/" . $program; 
$filename1 = "/usr/local/apache/htdocs/ static_html/" . $filename; 
$cmd_str = "/usr/local/php4/bin/php " . $program1 . " } " . $filename1 . " "; 
system($cmd_str); 
echo $filename . " generated.〈br〉"; 
}
```

这个函数是实现静态化的关键，即PHP动态页面程序不是被送到浏览器中，而是输入到名为$filename的文件中去（如图2）。两个参数中$program是PHP动态页面程序，$filename是生成的静态页面的名字（可根据需要自己制定命名规则，这一点很重要，见下文），/usr/local/php4/bin/php是PHP中具有把程序输入文件功能的部分，System是PHP中执行外部命令的函数。我们还可以看出所有生成动态页面的php程序需放在/php/目录下，所有新产生的静态页面则会出现在/static_html/目录下（这些路径可以根据具体需要设置）。 
下面让我们举个具体例子，看一下college_static.php的静态页面是怎样生成的。 



```php
function gen_college_static () 
{ 
for ($i = 0; $i 〈= 32; $i++〉 
{ 
putenv("province_id=" . $i); //*.php文件从数据库取数据时要用到。 
$filename = " college_static". $i . ".html"; 
gen_static_file("college_static.php", $filename); 
}
```
从这个函数我们可以看到通过调用函数gen_static_file（）,
 college_static.php经过静态化，变成了33个静态页面college.static0.html~college.static33.html，其中$filename会随着$I的变化而变化。当然也可以从数据库中直接取值，来控制生成的静态页面的个数和名字，其他程序对生成的静态页面的调用应和静态页面的命名规则一致。








