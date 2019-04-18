# Python os.path.join方法  join方法 - YZXnuaa的博客 - CSDN博客
2018年01月30日 15:54:48[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：3989
一。函数说明
1.join（）函数
语法：‘sep’.join（seq）
参数说明：
sep：分隔符。可以为空
seq：要连接的元素序列、字符串、元组、字典等
上面的语法即：以sep作为分隔符，将seq所有的元素合并成一个新的字符串
返回值：返回一个以分隔符sep连接各个元素后生成的字符串
2、os.path.join()函数
语法：  os.path.join(path1[,path2[,......]])
返回值：将多个路径组合后返回
注：第一个绝对路径之前的参数将被忽略
二。案例
![](https://img-blog.csdn.net/20180130155347449?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWVpYbnVhYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
三。**浅析一句python代码成生九九乘法表**
2011-11-24 11:58
最近在oschina上看到段九九乘法表 的代码，如下：
print'\n'.join([' '.join(['%s*%s=%-2s'%(y,x,x*y) fory in range(1,x+1)]) forx in range(1,10)])
（来至于：http://www.oschina.net/code/snippet_53549_2238）
我稍微调整了一下：
#coding:utf-8
print('\n'.join([' '.join('%sx%s=%-2s'%(x,y,x*y) for x in xrange(1,y+1)) for y in xrange(1,10)]))
恩，其实两代码都参不多，那么我们就来解析一下，这段代码：
首先：要使用的主要知识点：
1 列表解析知识(以及元组知识)
2 range,xrange,join函数的使用
3 字符串格式化输出
**列表解析知识(以及元组知识)**
语法：(转至 猪跑Lazy)
1 [expr for iter_var in iterable]   
2 [expr for iter_var in iterable if cond_expr]  
第一种语法：首先迭代iterable里所有内容，每一次迭代，都把iterable里相应内容放到iter_var中，再在表达式中应用该iter_var的内容，最后用表达式的计算值生成一个列表。
第二种语法：加入了判断语句，只有满足条件的内容才把iterable里相应内容放到iter_var中，再在表达式中应用该iter_var的内容，最后用表达式的计算值生成一个列表。
比如一个简单的：
l =[y for y in xrange(1,10)]
print(l) #输出 [1, 2, 3, 4, 5, 6, 7, 8, 9]
多个for循环的：
l =[(x,y) for x in xrange(1,3) for y in xrange(1,5)]
print(l) #输出：[(1, 1), (1, 2), (1, 3), (1, 4), (2, 1), (2, 2), (2, 3), (2, 4)]
从这里可以看出来：这两个for 是嵌套的循环执行的。
到这里，我们的代码就可以做第一次拆分了：[ '%sx%s=%-2s'%(x,y,x*y) for x in xrange(1,y+1) for y in xrange(1,10)]，（'%sx%s=%-2s'%(x,y,x*y)实际上就是一段表达式，所表示的意义稍后说明）当然这段代码是不会正常运行的，原因我想你应该知道：第一个for在找y时没有找到，因为从没有定义过（NameError: name 'y' is not defined）。
那么我们得让第一个for相对于第二个for而言也是一个运算表达式，那么我们使用**生成器表达式**，即“()”。
[ ('%sx%s=%-2s'%(x,y,x*y) for x in xrange(1,y+1)) for y in xrange(1,10)]
**range,xrange,join函数的使用**
另外使用生成器表达式还有一个好处是：生成器表达式使用了“惰性计算”(lazy evaluation，也有翻译为“延迟求值”，我以为这种按需调用call by need的方式翻译为惰性更好一些)，只有在检索时才被赋值( evaluated)，所以在列表比较长的情况下使用内存上更有效(来至：猪跑Lazy)。实际上：range与xrange的区别也是这一点，所以在需要大序列时使用xrange要好于range，另外xrange,range产生的序列都是左闭右开区间的(y+1的原因)。
[('%sx%s=%-2s'%(x,y,x*y) for x in xrange(1,y+1)) for y in xrange(1,10)]运算后，会产生类似这样的序列：[['1x1=1 '], ['1x2=2 ', '2x2=4 '], ['1x3=3 ', '2x3=6 ', '3x3=9 '].............一个二维数组。
我们进一歩使用join函数来处理最里层的数组让其形成：'1x2=2 2x2=4 ','1x3=3 2x3=6 3x3=9 '........那么代码如下：
[' '.join('%sx%s=%-2s'%(x,y,x*y) for x in xrange(1,y+1)) for y in xrange(1,10)]
运行后产后这样的结果：
['1x1=1 ', '1x2=2  2x2=4 ', '1x3=3  2x3=6  3x3=9 ', '1x4=4  2x4=8  3x4=12 4x4=16', '1x5=5  2x5=10 3x5=15 4x5=20 5x5=25', '1x6=6  2x6=12 3x6=18 4x6=24 5x6=30 6x6=36', '1x7=7  2x7=14 3x7=21 4x7=28 5x7=35 6x7=42 7x7=49', '1x8=8  2x8=16 3x8=24 4x8=32 5x8=40
 6x8=48 7x8=56 8x8=64', '1x9=9  2x9=18 3x9=27 4x9=36 5x9=45 6x9=54 7x9=63 8x9=72 9x9=81']
另外注意join函数的使用方式：join是string的一个成员函数，不是一个普通函数，所以不能直接这样使用join(source)。恩，你也可以这样分割列表：'|'.join(list)，另外也注意[os.path.join()](http://hi.baidu.com/fc_lamp/blog/item/2aa6a0c7c4c56d019d163df4.html)的用法。
**字符串格式化输出**
好了，我们再来说说：'%sx%s=%-2s'%(x,y,x*y)，这里格式化输出的方式，以前我写过一篇[《PHP如何显示(输出)两位小数(prinrf()函数简说)》](http://hi.baidu.com/fc_lamp/blog/item/09555100745c3eda267fb554.html)
里面有一些说明，另外：元组的操作方式，参看：[http://hi.baidu.com/fc_lamp/blog/item/5f943110754369d2a6ef3f68.html](http://hi.baidu.com/fc_lamp/blog/item/5f943110754369d2a6ef3f68.html)
这里主要说说'-'这个符号，这个是一个转换标志，相应的还有：'-' 表示左对齐，+表示在转换值之前要加上正负号，“”空白字符表示正数之前保留空格，0 (零)表示转换值若位数不够则用0填充。
另外，使用' '.join()的原因就是 '%-2s' 这个产生空格分割符。
好了，最后运行：print('\n'.join([' '.join('%sx%s=%-2s'%(x,y,x*y) for x in xrange(1,y+1)) for y in xrange(1,10)])) 产生结果：
1x1=1
1x2=2  2x2=4
1x3=3  2x3=6  3x3=9
1x4=4  2x4=8  3x4=12 4x4=16
1x5=5  2x5=10 3x5=15 4x5=20 5x5=25
1x6=6  2x6=12 3x6=18 4x6=24 5x6=30 6x6=36
1x7=7  2x7=14 3x7=21 4x7=28 5x7=35 6x7=42 7x7=49
1x8=8  2x8=16 3x8=24 4x8=32 5x8=40 6x8=48 7x8=56 8x8=64
1x9=9  2x9=18 3x9=27 4x9=36 5x9=45 6x9=54 7x9=63 8x9=72 9x9=81
