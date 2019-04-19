# django manager  use_for_related_fields使用 - Arthur的随笔 - CSDN博客
2011年10月20日 14:20:46[largetalk](https://me.csdn.net/largetalk)阅读数：1085标签：[django																[manager																[文档](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=manager&t=blog)](https://so.csdn.net/so/search/s.do?q=django&t=blog)
个人分类：[django](https://blog.csdn.net/largetalk/article/category/716448)
看django文档看到关于models.manager时候，前面custom manager都还好懂，但这个use_for_related_fields一时没看懂，搜了下，下面这个在stackoverflow上的回答很好，记一下
[http://stackoverflow.com/questions/6067195/how-does-use-for-related-fields-work-in-django](http://stackoverflow.com/questions/6067195/how-does-use-for-related-fields-work-in-django)
简单来说， use_for_related_fields作用就是当另一个model实例通过字段引用该model时，使用该model指定的manager而不是使用django.db.models.Manager
