# python sys.path_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
里面有个 **sys.path**属性。他是一个list.默然情况下python导入文件或者模块的话，他会先在sys.path里找模块的路径。如果没有的话，
程序就会报错。
所以我们一般自己写程序的话。最好把自己的模块路径给加到当前模块扫描的路径里,eg:
sys.path.append('你的模块的名称'),这样程序就不会
因为找不到模块而报错。。
