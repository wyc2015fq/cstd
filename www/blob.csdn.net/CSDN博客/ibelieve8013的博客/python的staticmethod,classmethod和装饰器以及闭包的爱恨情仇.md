# python的staticmethod,classmethod和装饰器以及闭包的爱恨情仇 - ibelieve8013的博客 - CSDN博客





2018年10月13日 14:35:12[ibelieve8013](https://me.csdn.net/ibelieve8013)阅读数：1125








经常都是你去面试，面试官一脸冷漠，听说你会Python，那给你30秒，写一个装饰器，应该很简单吧？然后你一脸蒙蔽，装饰器大家都听过，我们也大致知道什么意思，但是让写一个，你会写吗？

我们在python的程序中，经常看到一些类中，一些函数定义前面，写有@staticmethod,@classmethod等东东，这些是什么鬼呢？且听我细细道来，先来解释一下staticmethod和classmethod。直接一点来说，我们知道对于一个普通的类，我们要使用其中的函数的话，需要对类进行实例化，而一个类中，某个函数前面加上了staticmethod或者classmethod的话，那么这个函数就可以不通过实例化直接调用，什么意思呢？就是说有时候，我们需要把一些具有特定功能的函数放到一起，做成包导入Python程序，最好就是把他们放到一个类中，但是一个类每次我都要去实例化他，我觉得很麻烦，于是我在函数前面加上了@staticmethod,@classmethod，那么我下次用这个函数的功能的时候，可以直接用 *类名.函数名 *的形式了，这样是不是省心多了？

我相信到这里应该把这两个东东大致的功能讲清楚了，但是，这两个方法本身有什么区别呢？
- @staticmethod不需要表示自身对象的self和自身类的cls参数，就跟使用函数一样。
- @classmethod也不需要self参数，但第一个参数需要是表示自身类的cls参数。
- 如果在@staticmethod中要调用到这个类的一些属性方法，只能直接类名.属性名或类名.方法名。
- 而@classmethod因为持有cls参数，可以来调用类的属性，类的方法，实例化对象等，避免硬编码。

简单来说，就是staticmethod不需要传入表示自身对象参数，而classmethod需要传入一个类似于self的cls参数。那么既然classmethod麻烦一点，必然有他麻烦的道理，就是：使用classmethod的话，凡是在该类中的类的属性，方法，实例化对象等，都可以调用出来。

![](https://img-blog.csdn.net/20181013140445661?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

那么说了这么久，staticmethod和classmethod方法本质上是一个什么东西，好像楼主你并没有说清楚啊？

不错，大家看staticmethod和classmethod前面都有一个@符号，我们知道在微信qq里面，要@一个人，必然是有求于他。在这里也是一样的，我们的@xxx这样的形式，表示一个装逼器，哦对不起，说错了，是装饰器。那么这个装饰器又是何方神圣呢？

简而言之，有时候我们觉得这个函数不够完善，我想在这个函数的基础上拓展一些额外的通用的功能，但是由于这个函数又很重要，不能直接侵入代码去改，加上可能其它的函数也需要这样一种功能，那么我们就需要用到装逼器了，就是这样一个@xxx的东西。

一个简单的例子，我们要实现一个对函数运行时间计时的功能，因为我们依稀记得老师说过：一切皆对象。那么我们最简单粗暴的方法当然是，把这个函数当成一个参数，传入另外一个计时函数中，bingo。

![](https://img-blog.csdn.net/20181013141110576?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

但是，如果老板说现在有一千万个函数，都需要这样一个功能呢？好像这个方法不太体面，有没有装B一点的方法呢，当然，就是这个装饰器。

下面写一个最简单的装饰器，在原函数中不需要传入任何的参数的形式，记住，一定要返回函数。

![](https://img-blog.csdn.net/20181013141110713?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

当原函数中含有参数，怎么办呢，于是我们还可以写含有参数的装饰器：

![](https://img-blog.csdn.net/20181013141110742?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

但是这个问题在于也太过于定制化了，什么意思呢，就是说我们并不能保证每个函数都恰好具有两个参数，当有多个参数的时候，我希望这个装饰器同样可以用，那么就需要用到这里的 多参数装饰器。

## 多参数的时候，复习一下多参数传入的方式：

![](https://img-blog.csdn.net/20181013141110709?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**kwargs：（表示的就是形参中按照关键字传值把多余的传值以字典的方式呈现）。

![](https://img-blog.csdn.net/20181013141110741?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

位置参数、*args、**kwargs三者的顺序必须是位置参数、*args、**kwargs，不然就会报错。

言归正传，再说这边：对于一些函数如果都需要这样的装饰器，但是这个函数的参数数量各不相同，那么就需要向下面这样：

![](https://img-blog.csdn.net/20181013141110722?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

那么苛刻的老板又跳出来，这样一个装饰器不能满足我的要求，我想要多个装饰器。

![](https://img-blog.csdn.net/2018101314111110?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

好的，你一下@出来了多个大佬，这些大佬的出场顺序呢，搞不好的话大佬会互相砍起来。

简单一个图就可以把人看晕，噢不对，看懂。考验智商的时候到了同志们。

![](https://img-blog.csdn.net/20181013141111200?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

张无忌，你懂了吗，你真的懂了吗，好的，那你看看下面的程序，应该输出什么内容（答案如箭头所示）：

![](https://img-blog.csdn.net/20181013141111141?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)有人说，我怎么感觉这个东西和闭包有着藕断丝连的、牵扯不清的情感纠葛？

闭包是什么？于是又引入闭包的含义，闭包：简而言之，在函数内部对外部作用域的局部变量x进行引用，但是一通骚操作之后并不会影响局部变量x的值。见下图：

![](https://img-blog.csdn.net/20181013141111126?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注意：闭包中是不能修改外部作用域的局部变量。

![](https://img-blog.csdn.net/20181013141111157?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

请看下面的一个程序，很多人没搞清楚的时候会以为结果是2,3,4，但是不是。

![](https://img-blog.csdn.net/20181013141111176?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

要实现原来的234的功能也很简单，只需要动动手脚：

![](https://img-blog.csdn.net/20181013141111413?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

你问闭包这个脱裤子放屁的功能有什么用呢，还真有用，比如有时候我们希望在函数内部，在实现其他的函数的时候，环境的值不改变，而只是调用这个值；另外闭包可以根据外部作用域的局部变量，得到不同的结果，类似于配置不同的功能。

谈了这么多，我们可以简而言之理解一下@staticmethod了，这就相当于是一个装饰器，当然这个装饰器比我们的对函数计时等helloworld级别的装饰器牛逼闪闪多了，这个装饰器作用在这个函数上，使得该函数很牛逼，然后告诉我们，你不用实例化了，你直接用吧。

好了，生活就是这么有趣，Python也是，当你试图搞懂一个staticmethod的时候，你得搞懂装饰器，要搞懂装饰器，你还要知道闭包，要知道闭包，你还要看懂这篇博文。。。



