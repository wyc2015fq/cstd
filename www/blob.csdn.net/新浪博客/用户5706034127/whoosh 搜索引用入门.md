# whoosh 搜索引用入门_用户5706034127_新浪博客
||分类：[自然语言处理](http://blog.sina.com.cn/s/articlelist_5706034127_3_1.html)|
## whoosh使用流程
- 
创建schema
- 
索引生成
- 
索引查询
其中whoosh是原生唯一的[Python](http://lib.csdn.net/base/python)写的全文搜索引擎，虽然有说whoosh性能比不上sphinx,xapian等。
不过whoosh本身很小，安装后才2.61M，非常容易集成到django/python里面，而我们的需求又不大
一般小站完全够用了，因此决定使用whoosh来进行全文检索。
