# 你所不知道的 Python 冷知识！





## 01. 省略号也是对象

------

`...` 这是省略号，在Python中，一切皆对象。它也不例外。

在 Python 中，它叫做 Ellipsis 。

在 Python 3 中你可以直接写…来得到这玩意。

```
>>> ...
Ellipsis
>>> type(...)
<class 'ellipsis'>
```

而在 2 中没有…这个语法，只能直接写Ellipsis来获取。

```
>>> Ellipsis
Ellipsis
>>> type(Ellipsis)
<type 'ellipsis'>
>>>
```

它转为布尔值时为真

```
>>> bool(...)



True



复制代码
```

最后，这东西是一个单例。

```
>>> id(...)



4362672336



>>> id(...)



4362672336



复制代码
```

这东西有啥用呢？据说它是Numpy的语法糖，不玩 Numpy 的人，可以说是没啥用的。

在网上只看到这个 用 `...` 代替 pass ，稍微有点用，但又不是必须使用的。

```
try:



    1/0



except ZeroDivisionError:



    ...



复制代码
```

## 02. 如何修改解释器提示符

------

正常情况下，我们在 终端下 执行Python 命令是这样的。

```
>>> for i in range(2):



...     print (i)



...



0



1



复制代码
```

你是否想过 `>>>` 和 `...` 这两个提示符也是可以修改的呢？

```
>>> import sys                      



>>> sys.ps1                         



'>>> '                              



>>> sys.ps2                         



'... '                              



>>>                                 



>>> sys.ps2 = '---------------- '                 



>>> sys.ps1 = 'Python编程时光>>>'       



Python编程时光>>>for i in range(2):     



----------------    print (i)                    



----------------                                 



0                                   



1                                   



复制代码
```

## 03. 增量赋值的性能更好

------

诸如 `+=` 和 `*=` 这些运算符，叫做 增量赋值运算符。

这里使用用 += 举例，以下两种写法，在效果上是等价的。

```
# 第一种



a = 1 ; a += 1



 



# 第二种



a = 1; a = a + 1



复制代码
```

`+=` 其背后使用的魔法方法是 __iadd__，如果没有实现这个方法则会退而求其次，使用 __add__ 。

这两种写法有什么区别呢？

用列表举例 a += b，使用 _*add*_ 的话就像是使用了a.extend(b),如果使用 _*add*_ 的话，则是 a = a+b,前者是直接在原列表上进行扩展，而后者是先从原列表中取出值，在一个新的列表中进行扩展，然后再将新的列表对象返回给变量，显然后者的消耗要大些。

所以在能使用增量赋值的时候尽量使用它。

## 04. 奇怪的字符串

------

示例一

```
# Python2.7



>>> a = "Hello_Python"



>>> id(a)



32045616



>>> id("Hello" + "_" + "Python")



32045616



 



# Python3.7



>>> a = "Hello_Python"



>>> id(a)



38764272



>>> id("Hello" + "_" + "Python")



32045616



复制代码
```

示例二

```
>>> a = "MING"



>>> b = "MING"



>>> a is b



True



 



# Python2.7



>>> a, b = "MING!", "MING!"



>>> a is b



True



 



# Python3.7



>>> a, b = "MING!", "MING!"



>>> a is b



False



复制代码
```

示例三

```
# Python2.7



>>> 'a' * 20 is 'aaaaaaaaaaaaaaaaaaaa'



True



>>> 'a' * 21 is 'aaaaaaaaaaaaaaaaaaaaa'



False



 



# Python3.7



>>> 'a' * 20 is 'aaaaaaaaaaaaaaaaaaaa'



True



>>> 'a' * 21 is 'aaaaaaaaaaaaaaaaaaaaa'



True



复制代码
```

## 05. and 和 or 的取值顺序

------

and 和 or 是我们再熟悉不过的两个逻辑运算符。而我们通常只用它来做判断，很少用它来取值。

如果一个or表达式中所有值都为真，Python会选择第一个值，而and表达式则会选择第二个。

```
>>>(2 or 3) * (5 and 7)



14  # 2*7



复制代码
```

## 06. 默认参数最好不为可变对象

------

函数的参数分三种 - 可变参数 - 默认参数 - 关键字参数

这三者的具体区别，和使用方法在 廖雪峰的教程 里会详细的解释。这里就不搬运了。

今天要说的是，传递默认参数时，新手很容易踩雷的一个坑。

先来看一个示例

```
def func(item, item_list=[]):



    item_list.append(item)



    print(item_list)



 



func('iphone')



func('xiaomi', item_list=['oppo','vivo'])



func('huawei')



复制代码
```

在这里，你可以暂停一下，思考一下会输出什么？

思考过后，你的答案是否和下面的一致呢

```
['iphone']



['oppo', 'vivo', 'xiaomi']



['iphone', 'huawei']



复制代码
```

如果是，那你可以跳过这部分内容，如果不是，请接着往下看，这里来分析一下。

Python 中的 def 语句在每次执行的时候都初始化一个函数对象，这个函数对象就是我们要调用的函数，可以把它当成一个一般的对象，只不过这个对象拥有一个可执行的方法和部分属性。

对于参数中提供了初始值的参数，由于 Python 中的函数参数传递的是对象，也可以认为是传地址，在第一次初始化 def 的时候，会先生成这个可变对象的内存地址，然后将这个默认参数 item_list 会与这个内存地址绑定。在后面的函数调用中，如果调用方指定了新的默认值，就会将原来的默认值覆盖。如果调用方没有指定新的默认值，那就会使用原来的默认值。

 

![个人理解的记忆方法，不代表官方](https://user-gold-cdn.xitu.io/2018/9/3/165a003b81b1654f?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)

 

 

## 07. 访问类中的私有方法

------

大家都知道，类中可供直接调用的方法，只有公有方法（protected类型的方法也可以，但是不建议）。也就是说，类的私有方法是无法直接调用的。

这里先看一下例子

```
class Kls():



    def public(self):



        print('Hello public world!')



        



    def __private(self):



        print('Hello private world!')



        



    def call_private(self):



        self.__private()



 



ins = Kls()



 



# 调用公有方法，没问题



ins.public()



 



# 直接调用私有方法，不行



ins.__private()



 



# 但你可以通过内部公有方法，进行代理



ins.call_private()



复制代码
```

既然都是方法，那我们真的没有方法可以直接调用吗？

当然有啦，只是建议你千万不要这样弄，这里只是普及，让你了解一下。

```
# 调用私有方法，以下两种等价



ins._Kls__private()



ins.call_private()



复制代码
```

## 08. 类首字母不一定是大写

------

在正常情况下，我们所编写的所见到的代码，好像都默许了类名首字母大写，而实例用小写的这一准则。但这并不是强制性的，即使你反过来的也没有关系。

但有一些内置的类，首字母都是小写，而实例都是大写。

比如 bool 是类名，而 True，False 是其实例； 比如 ellipsis是类名，Ellipsis是实例； 还有 int，string，float，list，tuple，dict等一系列数据类型都是类名，它们都是小写。

## 09. 时有时无的切片异常

------

这是个简单例子

```
my_list = [1, 2, 3, 4, 5]



print(my_list[5])



复制代码
```

执行一下，和我们预期的一样，会抛出索引异常。

```
Traceback (most recent call last):



  File "F:/Python Script/test.py", line 2, in <module>



    print(my_list[5])



IndexError: list index out of range



复制代码
```

但是今天要说的肯定不是这个，而是一个你可能会不知道的冷知识。

来看看，如下这种写法就不会报索引异常，执行my_list[5:]，会返回一个新list：[]。

```
my_list = [1, 2, 3]



print(my_list[5:])



复制代码
```

## 10. 哪些情况下不需要续行符

------

在写代码时，为了代码的可读性，代码的排版是尤为重要的。

为了实现高可读性的代码，我们常常使用到的就是续行符 `\` 。

```
>>> a = 'talk is cheap,'\



...     'show me the code.'



>>>



>>> print(a)



talk is cheap,show me the code.



复制代码
```

那有哪些情况下，是不需要写续行符的呢？

经过总结，在这些符号中间的代码换行可以省略掉续行符：`[]`, `()` , `{}`

```
>>> my_list=[1,2,3,



...          4,5,6]



 



>>> my_tuple=(1,2,3,



...           4,5,6)



 



>>> my_dict={"name": "MING",



...          "gender": "male"}



复制代码
```

另外还有，在多行文本注释中 `'''` ，续行符也是可以不写的。

```
>>> text = '''talk is cheap,



    ...           show me the code'''



复制代码
```

上面只举了一些简单的例子。

但你要学会举一反三。一样的，在以下这些场景也同样适用

- 类，和函数的定义。
- 列表推导式，字典推导式，集合推导式，生成器表达式

## 11. Py2 也可以使用 print()

------

我相信应该有不少人，思维定式，觉得只有 Py3 才可以使用 print()，而 Py2 只能使用print ’’。

其实并不是这样的。

在Python 2.6之前，只支持

```
print "hello"



复制代码
```

在Python 2.6和2.7中，可以支持如下三种

```
print "hello"



print("hello")



print ("hello")



复制代码
```

在Python3.x中，可以支持如下两种

```
print("hello")



print ("hello")



复制代码
```

## 12. 两次 return

------

我们都知道，try…finally… 语句的用法，不管try里面是正常执行还是报异常，最终都能保证finally能够执行。

同时，我们又知道，一个函数里只要遇到 return 函数就会立马结束。

基于以上这两点，我们来看看这个例子，到底运行过程是怎么样的？

```
>>> def func():



...     try:



...         return 'try'



...     finally:



...         return 'finally'



...



>>> func()



'finally'



复制代码
```

惊奇的发现，在`try` 里的return居然不起作用。

原因是，在try…finally…语句中，try中的return会被直接忽视，因为要保证 finally 能够执行。

## 13. for 死循环

------

for 循环可以说是 基础得不能再基础的知识点了。

但是如果让你用 for 写一个死循环，你会写吗？（问题来自群友 陈**）

这是个开放性的问题，在往下看之前，建议你先尝试自己思考，你会如何解答。

好了，如果你还没有思路，那就来看一下 一个海外 MIT 群友的回答:

```
for i in iter(int, 1):pass



复制代码
```

是不是懵逼了。iter 还有这种用法？这为啥是个死循环？

这真的是个冷知识，关于这个知识点，你如果看中文网站，可能找不到相关资料。

还好你可以通过 IDE 看py源码里的注释内容，介绍了很详细的使用方法。

原来iter有两种使用方法，通常我们的认知是第一种，将一个列表转化为一个迭代器。

而第二种方法，他接收一个 callable对象，和一个sentinel 参数。第一个对象会一直运行，直到它返回 sentinel 值才结束。

那`int` 呢，这又是一个知识点，int 是一个内建方法。通过看注释，可以看出它是有默认值0的。你可以在终端上输入`int()`看看是不是返回0。

![img](https://user-gold-cdn.xitu.io/2018/9/3/165a008f27f970f9?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)

 

 

由于int() 永远返回0，永远返回不了1

![img](https://user-gold-cdn.xitu.io/2018/9/3/165a00adebc228f4?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)

 

所以这个 for 循环会没有终点。一直运行下去。

 

这些问题和答案都源自于群友的智慧。如果你也想加入我们的讨论中，请到公众号后台，添加我个人微信。

## 14. 小整数池

------

先看例子。

```
>>> a = -6



>>> b = -6



>>> a is b



False



 



>>> a = 256



>>> b = 256



>>> a is b



True



 



>>> a = 257



>>> b = 257



>>> a is b



False



 



>>> a = 257; b = 257



>>> a is b



True



复制代码
```

为避免整数频繁申请和销毁内存空间，Python 定义了一个小整数池 [-5, 256] 这些整数对象是提前建立好的，不会被垃圾回收。

以上代码请在 终端Python环境下测试，如果你是在IDE中测试，并不是这样的效果。

那最后一个示例，为啥又是True？

因为当你在同一行里，同时给两个变量赋同一值时，解释器知道这个对象已经生成，那么它就会引用到同一个对象。如果分成两成的话，解释器并不知道这个对象已经存在了，就会重新申请内存存放这个对象。

## 15. intern机制

------

字符串类型作为Python中最常用的数据类型之一，Python解释器为了提高字符串使用的效率和使用性能，做了很多优化.

例如：Python解释器中使用了 intern（字符串驻留）的技术来提高字符串效率，什么是intern机制？就是同样的字符串对象仅仅会保存一份，放在一个字符串储蓄池中，是共用的，当然，肯定不能改变，这也决定了字符串必须是不可变对象。

```
>>> s1="hello"



>>> s2="hello"



>>> s1 is s2



True



 



# 如果有空格，默认不启用intern机制



>>> s1="hell o"



>>> s2="hell o"



>>> s1 is s2



False



 



# 如果一个字符串长度超过20个字符，不启动intern机制



>>> s1 = "a" * 20



>>> s2 = "a" * 20



>>> s1 is s2



True



 



>>> s1 = "a" * 21



>>> s2 = "a" * 21



>>> s1 is s2



False



 



>>> s1 = "ab" * 10



>>> s2 = "ab" * 10



>>> s1 is s2



True



 



>>> s1 = "ab" * 11



>>> s2 = "ab" * 11



>>> s1 is s2



False
```

作者：Python编程时光

更多Python视频、源码、资料加群683380553免费获取

转自：https://juejin.im/post/5b8d4f7651882542d14dab2b