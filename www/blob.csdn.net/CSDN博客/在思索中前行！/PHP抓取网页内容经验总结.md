# PHP抓取网页内容经验总结 - 在思索中前行！ - CSDN博客





2014年06月17日 22:19:38[_Tham](https://me.csdn.net/txl16211)阅读数：689








用php 抓取页面的内容在实际的开发当中是非常有用的，如作一个简单的内容采集器，提取网页中的部分内容等等，抓取到的内容在通过正则表达式做一下过滤就得到了你想要的内容，至于如何用正则表达式过滤，在这里就不做介绍了，有兴趣的同学可以参考本站的《正则表达式》板块：[http://phpzixue.cn/articles11.shtml](http://phpzixue.cn/articles11.shtml) ，以下就是几种常用的用php抓取网页中的内容的方法。

1.file_get_contents

PHP代码
|- - $url = "[http://www.phpzixue.cn](http://www.phpzixue.cn/) ";- $contents = file_get_contents($url);- //如果出现中文乱码使用下面代码- //$getcontent = iconv("gb2312", "utf-8",$contents);- echo $contents;- ?>|
|----|

2.curl
PHP代码|- - $url = "[http://www.phpzixue.cn](http://www.phpzixue.cn/) ";- $ch = curl_init();- $timeout = 5;- curl_setopt($ch, CURLOPT_URL, $url);- curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);- curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, $timeout);- //在需要用户检测的网页里需要增加下面两行- //curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);- //curl_setopt($ch, CURLOPT_USERPWD, US_NAME.":".US_PWD);- $contents = curl_exec($ch);- curl_close($ch);- echo $contents;- ?>|
|----|

3.fopen->fread->fclose
PHP代码|- - $handle = fopen ("[http://www.phpzixue.cn](http://www.phpzixue.cn/) ", "rb");- $contents = "";- do {- $data = fread($handle, 1024);- if (strlen($data) == 0) {- break;- }- $contents .= $data;- } while(true);- fclose ($handle);- echo $contents;- ?>|
|----|

注：
1. 使用file_get_contents和fopen必须空间开启allow_url_fopen。方法：编辑php.ini，设置 allow_url_fopen = On，allow_url_fopen关闭时fopen和file_get_contents都不能打开远程文件。
2.使用curl必须空间开启curl。方法：windows下修改php.ini，将extension=php_curl.dll前面的分号去掉，而且需要拷贝ssleay32.dll和libeay32.dll到C:/WINDOWS/system32下；Linux 下要安装curl扩展


