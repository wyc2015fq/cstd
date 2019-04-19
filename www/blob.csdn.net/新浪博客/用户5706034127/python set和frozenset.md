# python set和frozenset_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
转载：http://www.xuebuyuan.com/2041728.html
同多数语言一样，set表示集合，最重要的特性就是无序，所以Python中的set不支持indexing，但支持len（set），x
in set 等操作。
set有两种类型，set和frozenset。
set是可变的，有add（），remove（）等方法。既然是可变的，所以它不存在哈希值。
frozenset是冻结的集合，它是不可变的，存在哈希值，好处是它可以作为字典的key，也可以作为其它集合的元素。缺点是一旦创建便不能更改，没有add，remove方法。
举例如下
fza=frozenset('a')
adict={fza:1,'b':2} #正确
setb=set('a')
bdict={setb:1,'b':2} #错误
还有一点需要注意，不管是set还是frozenset，Python都不支持创建一个整数的集合。
seta=set(1) #错误
setb=set（'1'）#正确
