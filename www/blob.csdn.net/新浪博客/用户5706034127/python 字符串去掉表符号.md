# python 字符串去掉表符号_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
string 包，自带了标点符号集合，，string.punctuation，不过好像只有英文的符号
字符串自带translate函数，可以去除delset的字符，而且，根据table进行字符串映射转换
delset = string.punctuation
l = line.translate(None,delset)
l =
line.translate(table,delset)
