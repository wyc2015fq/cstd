# PHP相关系列 - PHP中关于COOKIE域问题 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月23日 09:40:56[initphp](https://me.csdn.net/initphp)阅读数：953标签：[php																[google																[浏览器																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[PHP相关系列](https://blog.csdn.net/initphp/article/category/2609095)





> 
COOKIE的设置有域的限制

如果你的网站访问路劲是这样的：http://xxxx/mana/test/

如果你这样设置 set_cookie('a', 'a') 的话，在google浏览器下会遇到一个隐藏的BUG，域的问题

解决方案：


```php
<?php
setcookie("名称", '值', NULL, '/')
```](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=google&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)




