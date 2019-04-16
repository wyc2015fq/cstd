# PHP相关系列 - PHP in_array函数的一个危险用法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年08月06日 19:37:17[initphp](https://me.csdn.net/initphp)阅读数：1522标签：[php](https://so.csdn.net/so/search/s.do?q=php&t=blog)
个人分类：[PHP相关系列](https://blog.csdn.net/initphp/article/category/2609095)









```php
<?php
if (in_array('', array(0,1))) echo '错误';
if (in_array(false, array(0,1))) echo '错误';
if (in_array(true, array(0,1))) echo '错误';
if (in_array($a['a'], array(0,1))) echo '错误';
```


PHP in_array()函数中如果这样用的话就会出现问题，所以使用的时候要谨慎
            


