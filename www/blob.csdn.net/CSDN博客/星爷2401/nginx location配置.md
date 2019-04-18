# nginx location配置 - 星爷2401 - CSDN博客
2016年12月02日 14:34:02[星爷2401](https://me.csdn.net/leifukes)阅读数：3597标签：[nginx																[location																[html																[php](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=location&t=blog)](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)
个人分类：[nginx](https://blog.csdn.net/leifukes/article/category/6552866)
location 匹配的内容来源是来自网页的URI,而不是URL
说明：URL代表整个链接如：www.baidu.com/images/search，而URI则是/images/search。所以nginx的location匹配的是URI
= 精确匹配。如果匹配，将不会匹配其他的location 
例如：A : location =/web {
root /home/www/web;     
}
B : location =/ {
root /home/www/web;     
}
当网页输入：www.test.com/web 的时候，匹配A。
当网页输入：www.test.com/的时候，匹配B。
^~ 匹配任何以static开头的地址，不是正则匹配，可以理解为对url路径进行匹配。
例如：location ^~ /static/ {
}
当网页输入：www.test.com/static/static.html 的时候,则匹配。
~为区分大小写匹配(可用正则表达式)，而
~* 则是不区分大小写的正则匹配
例如：A：location ~ \.(jpg|gif|png|jpeg|css|js)${
}
B: location ~* \.(jpg|gif|png|jpeg|css|js)${
}
当网页输入：www.test.com/1.jpg 的时候,则匹配A和B。但是A的优先级比较高，最终匹配A
当网页输入：www.test.com/1.JPG
 的时候,则匹配B。因为A不匹配。
!~和!~* 怎么和上面对应，意思是：符合这个规则但是不会匹配到。
例如：A：location !~ \.xhtml${
}
B: location !~* \.xhtml${
}
当网页输入：www.test.com/1.xhtml 的时候,则不会匹配A和B。
当网页输入：www.test.com/1.XHTML 的时候,则不会匹配B。
/ 当所有的规则都不符合的时候则会匹配这一项
例如：location / {
}
当网页输入：www.test.com/xxxxxx，以上规则都匹配不到，所以会匹配此规则。
https://segmentfault.com/a/1190000002797606
http://blog.sina.com.cn/s/blog_97688f8e0100zws5.html

