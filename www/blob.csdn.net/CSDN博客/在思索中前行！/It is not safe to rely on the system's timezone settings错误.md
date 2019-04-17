# It is not safe to rely on the system's timezone settings错误 - 在思索中前行！ - CSDN博客





2014年04月29日 21:01:53[_Tham](https://me.csdn.net/txl16211)阅读数：548
个人分类：[php开发](https://blog.csdn.net/txl16211/article/category/2231073)








在写php程序中有时会出现这样的警告：
PHP Warning: date(): It is not safe to rely on the system's timezone settings. You are *required* to use the date.timezone setting or the date_default_timezone_set() function. In case you used
 any of those methods and you are still getting this warning, you most likely misspelled the timezone identifier. We selected 'UTC' for '8.0/no DST' instead in D:\PHPWEB\news\file.php on line 17 。这是因为PHP所取的时间是格林威治标准时间，所以和你当地的时间会有出入格林威治标准时间和北京时间大概差8个小时左右，我们可以按照下面的方法解决： 
1、在页头使用date_default_timezone_set()设置我的默认时区为北京时间，即 <?php date_default_timezone_set("PRC"); ?>就可以了。 

2、在php.ini中设置date.timezone的值为PRC，设置好以后的为：date.timezone=PRC，同时取消这一行代码的注释，即去掉前面的分号就可以了。 

PRC：表示The Republic Country OF China



