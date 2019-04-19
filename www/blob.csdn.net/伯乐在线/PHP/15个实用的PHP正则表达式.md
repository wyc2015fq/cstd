# 15个实用的PHP正则表达式 - 文章 - 伯乐在线
原文出处： [5iDev]()
对于开发人员来说，正则表达式是一个非常有用的功能，它提供了 查找，匹配，替换 句子，单词，或者其他格式的字符串。这篇文章主要介绍了15个超实用的php正则表达式，需要的朋友可以参考下。在这篇文章里，我已经编写了15个超有用的正则表达式，WEB开发人员都应该将它收藏到自己的工具包。**验证域名**检验一个字符串是否是个有效域名.

PHP
```
$url = "http://komunitasweb.com/"; 
if (preg_match('/^(http|https|ftp)://([A-Z0-9][A-Z0-9_-]*(?:.[A-Z0-9][A-Z0-9_-]*)+):?(d+)?/?/i', $url)) { 
  echo "Your url is ok."; 
} else { 
  echo "Wrong url."; 
}
```
**从一个字符串中 突出某个单词**
这是一个非常有用的在一个字符串中匹配出某个单词 并且突出它，非常有效的搜索结果

PHP
```
$text = "Sample sentence from KomunitasWeb, regex has become popular in web programming. Now we learn regex. According to wikipedia, Regular expressions (abbreviated as regex or 
regexp, with plural forms regexes, regexps, or regexen) are written in a formal language that can be interpreted by a regular expression processor"; 
$text = preg_replace("/b(regex)b/i", '<span style="background:#5fc9f6">1</span>', $text); 
echo $text;
```
突出查询结果在你的 WordPress 博客里就像刚才我说的，上面的那段代码可以很方便的搜索出结果，而这里是一个更好的方式去执行搜索在某个WordPress的博客上打开你的文件 search.php ，然后找到 方法 the_title() 然后用下面代码替换掉它

PHP
```
echo $title; 
Now, just before the modified line, add this code: 
<?php 
  $title   = get_the_title(); 
  $keys= explode(" ",$s); 
  $title   = preg_replace('/('.implode('|', $keys) .')/iu', 
    '<strong>\0</strong>', 
    $title); 
?> 
Save the search.php file and open style.css. Append the following line to it: 
strong.search-excerpt { background: yellow; }
```
**从HTML文档中获得全部图片**
如果你曾经希望去获得某个网页上的全部图片，这段代码就是你需要的，你可以轻松的建立一个图片下载机器人

PHP
```
$images = array(); 
preg_match_all('/(img|src)=("|')[^"'>]+/i', $data, $media); 
unset($data); 
$data=preg_replace('/(img|src)("|'|="|=')(.*)/i',"$3",$media[0]); 
foreach($data as $url) 
{ 
  $info = pathinfo($url); 
  if (isset($info['extension'])) 
  { 
    if (($info['extension'] == 'jpg') || 
    ($info['extension'] == 'jpeg') || 
    ($info['extension'] == 'gif') || 
    ($info['extension'] == 'png')) 
    array_push($images, $url); 
  } 
}
```
**删除重复字母**
经常重复输入字母? 这个表达式正适合.

PHP
```
$text = preg_replace("/s(w+s)1/i", "$1", $text);
```
**删除重复的标点**
功能同上，但只是面对标点，白白重复的逗号

PHP
```
$text = preg_replace("/.+/i", ".", $text);
```
**匹配一个XML或者HTML标签**
这个简单的函数有两个参数：第一个是你要匹配的标签，第二个是包含XML或HTML的变量，再强调下，这个真的很强大

PHP
```
function get_tag( $tag, $xml ) { 
 $tag = preg_quote($tag); 
 preg_match_all('{<'.$tag.'[^>]*>(.*?)</'.$tag.'>.'}', 
          $xml, 
          $matches, 
          PREG_PATTERN_ORDER); 
 return $matches[1]; 
}
```
**匹配具有属性值的XML或者HTML标签**
这个功能和上面的非常相似，但是它允许你匹配的标签内部有属性值，例如你可以轻松匹配 <div id=”header”>

PHP
```
function get_tag( $attr, $value, $xml, $tag=null ) { 
 if( is_null($tag) ) 
  $tag = '\w+'; 
 else 
  $tag = preg_quote($tag); 
 $attr = preg_quote($attr); 
 $value = preg_quote($value); 
 $tag_regex = "/<(".$tag.")[^>]*$attr\s*=\s*". 
        "(['\"])$value\\2[^>]*>(.*?)<\/\\1>/" 
 preg_match_all($tag_regex, 
         $xml, 
         $matches, 
         PREG_PATTERN_ORDER); 
 return $matches[3]; 
}
```
**匹配十六进制颜色值**
web开发者的另一个有趣的工具，它允许你匹配和验证十六进制颜色值.

PHP
```
$string = "#555555"; 
if (preg_match('/^#(?:(?:[a-fd]{3}){1,2})$/i', $string)) { 
echo "example 6 successful."; 
}
```
**查找页面 title**
这段代码方便查找和打印 网页 <title> 和</title> 之间的内容

PHP
```
$fp = fopen("http://www.catswhocode.com/blog","r"); 
while (!feof($fp) ){ 
  $page .= fgets($fp, 4096); 
} 
$titre = eregi("<title>(.*)</title>",$page,$regs); 
echo $regs[1]; 
fclose($fp);
```
**解释 Apache 日志**
大多数网站使用的都是著名的Apache服务器，如果你的网站也是，那么使用PHP正则表达式解析 apache 服务器日志 怎么样？

PHP
```
//Logs: Apache web server 
//Successful hits to HTML files only. Useful for counting the number of page views. 
'^((?#client IP or domain name)S+)s+((?#basic authentication)S+s+S+)s+[((?#date and time)[^]]+)]s+"(?:GET|POST|HEAD) ((?#file)/[^ ?"]+?.html?)??((?#parameters)[^ ?"]+)? HTTP/[0-9.]+"s+(?#status code)200s+((?#bytes transferred)[-0-9]+)s+"((?#referrer)[^"]*)"s+"((?#user agent)[^"]*)"$' 
//Logs: Apache web server 
//404 errors only 
'^((?#client IP or domain name)S+)s+((?#basic authentication)S+s+S+)s+[((?#date and time)[^]]+)]s+"(?:GET|POST|HEAD) ((?#file)[^ ?"]+)??((?#parameters)[^ ?"]+)? HTTP/[0-9.]+"s+(?#status code)404s+((?#bytes transferred)[-0-9]+)s+"((?#referrer)[^"]*)"s+"((?#user agent)[^"]*)"$'
```
**使用智能引号代替双引号**
如果你是一个印刷爱好者，你将喜欢这个允许用智能引号代替双引号的正则表达式，这个正则被WORDPRESS在其内容上使用

PHP
```
preg_replace('B"b([^"x84x93x94rn]+)b"B', '?1?', $text);
```
**检验密码的复杂度**
这个正则表达式将检测输入的内容是否包含6个或更多字母，数字，下划线和连字符. 输入必须包含至少一个大写字母，一个小写字母和一个数字

PHP
```
'A(?=[-_a-zA-Z0-9]*?[A-Z])(?=[-_a-zA-Z0-9]*?[a-z])(?=[-_a-zA-Z0-9]*?[0-9])[-_a-zA-Z0-9]{6,}z'
```
**WordPress: 使用正则获得帖子上的图片 **
我知道很多人是WORDPRESS的使用者，你可能会喜欢并且愿意使用 那些从帖子的内容检索下来的图像代码。使用这个代码在你的BLOG只需要复制下面代码到你的某个文件里

PHP
```
<?php if (have_posts()) : ?> 
<?php while (have_posts()) : the_post(); ?> 
<?php 
$szPostContent = $post->post_content; 
$szSearchPattern = '~<img [^>]* />~'; 
// Run preg_match_all to grab all the images and save the results in $aPics 
preg_match_all( $szSearchPattern, $szPostContent, $aPics ); 
// Check to see if we have at least 1 image 
$iNumberOfPics = count($aPics[0]); 
if ( $iNumberOfPics > 0 ) { 
   // Now here you would do whatever you need to do with the images 
   // For this example the images are just displayed 
   for ( $i=0; $i < $iNumberOfPics ; $i++ ) { 
     echo $aPics[0][$i]; 
   }; 
}; 
endwhile; 
endif; 
?>
```
**自动生成笑脸图案**
被WordPress使用的另一个方法, 这段代码可使你把图像自动更换一个笑脸符号

PHP
```
$texte='A text with a smiley '; 
echo str_replace(':-)','<img src="smileys/souriant.png">',$texte);
```
**移除图片的链接**

PHP
```
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<?php 
  $str = ' 
    <a href="http://www.jobbole.com/">jobbole</a>其他字符 
    <a href="http://www.sohu.com/">sohu</a> 
    <a href="http://www.sohu.com/"><img src="http://www.fashion-press.net/img/news/3176/mot_06.jpg" /></a> 
    <br>'; 
  //echo preg_replace("/(<a.*?>)(<img.*?>)(<\/a>)/", '$2', $str);  
  echo preg_replace("/(<a.*?>)(<img.*?>)(<\/a>)/", '\2', $str);  
?>
```
以上就是15个超实用的php正则表达式，希望对大家的学习有所帮助。
