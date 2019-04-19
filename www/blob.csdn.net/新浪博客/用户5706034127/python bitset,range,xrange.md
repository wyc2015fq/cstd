# python bitset,range,xrange_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
			xrange 和range 在输入为1234567910的时候会报错，因为不支持那么多的，
xragne说不支持转化为c语言的long，
range说不支持这么大的数组。
python自身可以处理任意大的整数。
所以Python自带bitset，，不需要额外的bitset！！！！
天然的bitset
biset可以有很多用处，比如标志位，
标记成绩大于75分的同学，他们的学号位置设置为1
