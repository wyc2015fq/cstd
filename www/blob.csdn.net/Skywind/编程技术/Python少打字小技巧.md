# Skywind Inside » Python少打字小技巧
## Python少打字小技巧
June 7th, 2006[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
说明：增加代码的描述力，可以成倍减少你的LOC，做到简单，并且真切有力
观点：少打字＝多思考＋少出错，10代码行比50行更能让人明白，以下技巧有助于提高5倍工作效率
**1. 交换变量值时避免使用临时变量：(cookbook1.1)**
老代码：我们经常很熟练于下面的代码
temp = x
x = y
y = temp
代码一：
u, v, w = w, v, u
有人提出可以利用赋值顺序来简化上面的三行代码成一行
代码二：
u, v = v, u
其实利用Python元组赋值的概念，可更简明 — 元组初始化 + 元组赋值
**2. 读字典时避免判断键值是否存在：(cookbook1.2)**
d = { ‘key’: ‘value’ }
老代码：
if ‘key’ in d: print d[‘key’]
else: print ‘not find’
新代码：
print d.get(‘key’, ‘not find’)
**3. 寻找最小值和位置的代码优化：**
s = [ 4,1,8,3 ]
老代码：
mval, mpos = MAX, 0
for i in xrange(len(s)):
if s[i] < mval: mval, mpos = s[i], i
新代码：
mval, mpos = min([ (s[i], i) for i in xrange(len(s)) ])
元组比较的特性，可以方便的写做一行
观点一：用Python编程，需要有“一字千金”的感觉；既然选择了Python，就不要在意单条语句的效率。
上面几点例子很基础，实际中将原始代码压缩1/5并不是不可能，我们之前一个子项目，C++代码270K
重构后Python代码只有67K，当然使用python的日志模块（logging），读写表格文本（csv）等，也功
不可末，最终代码变成原来的1/4，我觉得自己的寿命延长了三倍。。。下面优化几个常用代码：
**4. 文件读取工作的最简单表达：**
老代码：我们需要将文本文件读入到内存中
line = ”
fp = open(‘text.txt’, ‘r’)
for line in fp: text += line
代码一：
text = string.join([ line for line in open(‘text.txt’)], ”]
代码二：
text = ”.join([ line for line in open(‘text.txt’)])
代码三：
text = file(‘text.txt’).read()
新版本的Python可以让你写出比1，2漂亮的代码（open是file的别名，这里file更直观）
**5. 如何在Python实现三元式：**
老代码：用惯C++,Java,C#不喜欢写下面代码
if n >= 0: print ‘positive’
else: print ‘negitive’
代码一：该技巧在 Lua里也很常见
print (n >= 0) and ‘positive’ or ‘negitive’
说明：这里的’and’和’or’相当于C中的’:’和’?’的作用，道理很简单，因为如果表达式为
真了那么后面的or被短路，取到’positive’；否则，and被短路，取到’negitive’
代码二：
print (n >= 0 and [‘positive’] or [‘negitive])[0]
说明：将两个值组装成元组，即使’positive’是None, ”, 0 之类整句话都很安全
代码三：
print (‘negitive’, ‘positive’)[n >= 0]
说明：(FalseValue, TrueValue)[Condition] 是利用了 元组访问 + True=1 两条原理
**6. 避免字典成员是复杂对象的初始化：(cookbook1.5)**
老代码：
if not y in d: d[y] = { }
d[y][x] = 3
新代码：
d.setdefault(y, { })[x] = 3
如果成员是列表的话也一样: d.setdefault(key, []).append(val)
上面六点技巧加以发挥，代码已经很紧凑了，但是还没有做到“没有一句废话”可能有人怀疑真的能
减少1/5的代码么？？我要说的是1/5其实很保守，Thinking in C++的作者后来用了Python以后
觉得Python甚至提高了10倍的工作效率。下面的例子可以进一步说明：
**例子1：把文本的IP地址转化为整数**
说明：需要将类似’192.168.10.214’的IP地址转化为 0x0C0A80AD6，在不用 inet_aton情况下
当C++/Java程序员正为如何进行文本分析，处理各种错误输入烦恼时，Python程序员已经下班：
f = lambda ip: sum( [ int(k)*v for k, v in zip(ip.split(‘.’), [1<<24, 65536, 256, 1])] )
首先ip.split(‘.’)得到列表[‘192′,’168′,’10’,’214′]，经过zip一组装，就变成
[(‘192’,0x1000000),(‘168′,0x10000),(’10’,0x100),(‘214’,1)]
接着for循环将各个元组的两项做整数乘法，最后将新列表的值用sum求和，得到结果
C++程序员不肖道：“你似乎太相信数据了，根本没有考虑道错误的输入”
Python程序员回答：“外面的try/except已帮我完成所有异常处理，不必担心越界崩溃而无法捕获”
Java程序员得意的看着自己百行代码：“我想知道你如何让你的同事来理解你的杰作？你有没有考虑过将
类似gettoken之类的功能独立处理，让类似问题可以复用？我的代码说明了如何充分发挥Reflection和
interface的优秀特性，在增加重用性的同时，提供清晰可读的代码”
Python无奈道：“这是‘纯粹的代码’，意思是不可修改，类似正则表达式，只要让人明白他的功能就行了，
要修改就重写。再我能用三行代码完成以内绝不会有封装的想法，况且熟悉Python者也不觉得难读啊？”
C++程序员抛出杀手简：“如果让你一秒钟处理10w个ip转化的话怎么办？”
Python程序员觉得想睡觉：“你觉得我会蠢到还用Python做这样的事情么？”
此时C++程序员似乎并没听到，反而开始认真的思考起自己刚才提出问题来，一会只见他轻藐的看了另外两
人一眼，然后胸有成竹的转到电脑前，开始往屏幕上输入：“template <….”
小笑话：封装的陷阱，让人一边喊着“封装”或“复用”，一边在新项目中，全部打破重写，并解释为–重构
观点二：简单即是美，把一个东西设计复杂了，本身就是有问题的
思考题：上面的程序，如果反过来，将ip的整数形式转化为字符串，各位该如何设计呢？？
**例子2：输出一个对象各个成员的名称和值**
g = lambda m: ‘\n’.join([ ‘%s=%s’%(k, repr(v)) for k, v in m.__dict__.iteritems() ])
用法：print g(x)
延伸：上面两个例子熟悉了lambda以后，建议可以尝试使用下 yield
**观点总结**
Q:“怎样才算做到注重What you think多于What you are writing”
A:“就是说你手上打着第1页需求的代码，眼睛却在看着第2页需求的内容，心里想着如何应对5-10页的东西”
国外多年前废除PASCAL改用Python做科研教学是有道理的，关于精简代码的例子举不胜举，用它编码时应该有“一字千金”的感觉，否则最终写出来的，还是“伪装成Python的C++程序”。
编程本来就是快乐的，避免过多的体力劳动，赢得更多思考的时间。
思考题：到底是封装呢？还是放弃封装？
思考题：“more than one way to do it”是不是就是好事？它的反面是什么？
PS: 更多实用方法可以阅读 Daily Python URL 以及《Python Cookbook》
Skywind Inside
http://www.skywind.me/blog/
