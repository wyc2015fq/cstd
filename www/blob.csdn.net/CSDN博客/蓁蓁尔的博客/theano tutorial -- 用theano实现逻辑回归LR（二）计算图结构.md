# theano tutorial -- 用theano实现逻辑回归LR（二）计算图结构 - 蓁蓁尔的博客 - CSDN博客





2017年01月07日 12:49:01[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：287








### 见：[http://deeplearning.net/software/theano/extending/graphstructures.html](http://deeplearning.net/software/theano/extending/graphstructures.html)

### 中文版：[http://geek.csdn.net/news/detail/131362](http://geek.csdn.net/news/detail/131362)

## （1）图的结构

### Theano用图来表示符号数学运算。这些图的点包括：Apply、变量、operation和有向边。Apply代表了op对某些变量的计算【op类比成函数的定义，apply类比成函数的实际调用，变量就是函数的参数】。区分通过op定义的计算和把这个计算apply到某个实际的值是非常重要的。

### 【我们在编程时里定义 x和y，然后定义z=x+y，我们就得到了z的值，但是我们在Theano里定义符号变量x和y，然后定义z=x+y，因为x和y只是一个符号，所以z也只是一个符号，我们需要再定义一个函数，它的输入是x和y输出z。然后”调用“这个函数，传入x和y的实际值，才能得到z的值】。符号变量的类型是通过Type这个类来表示的。下面是一段Theano的代码以及对应的图。

### 代码：

```python
>>> import theano.tensor as T

>>> x = T.dmatrix('x')
>>> y = T.dmatrix('y')
>>> z = x + y
```

### 图：

![这里写图片描述](https://img-blog.csdn.net/20170107123618019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 图中的箭头代表了Python对象的引用。蓝色的框是Apply节点，红色的是变量，绿色的是Op，紫色的是Type。

### 当我们常见符号变量并且用Apply Op来产生更多变量的时候，我们创建了一个二分的有向无环图。如果变量的owner有指向Apply的边，那么说明这个变量是由Apply对应的Op产生的。此外Apply节点的input field和output field分别指向这个Op的输入和输出变量。

### x和y的owner是None，因为它不是由其它Op产生的，而是直接定义的。z的owner是非None的，这个Apply节点的输入是x和y，输出是z，Op是+，Apply的output指向了z，z.owner指向Apply，因此它们 是互相引用的。

## （2）自动求导

### 有了这个图的结构，自动计算导数就很容易了。tensor.grad()唯一需要做的就是从outputs逆向遍历到输入节点【如果您阅读过之前的自动求导部分，就会明白每个Op就是当时我们说的一个Gate，它是可以根据forward阶段的输入值计算出对应的local gradient，然后把所有的路径加起来就得到梯度了】。对于每个Op，它都定义了怎么根据输入计算出偏导数。使用链式法则就可以计算出梯度了。

## （3）优化

### 当编译一个Theano函数的时候，你给theano.function的其实是一个图（从输出变量遍历到输入遍历）。你通过这个图结构来告诉theano怎么从input算出output，同时这也让theano有机会来优化这个计算图【你可以把theano想像成一个编译器，你通过它定义的符号计算语法来定义函数，然后调用函数。而theano会想方设法优化你的函数（当然前提是保证结果是正确的）】。Theano的优化包括发现图里的一些模式(pattern)然后把他替换新的模式，这些新的模式计算的结果和原来是一样的，但是心模式可能更快更稳定。它也会检测图里的重复子图避免重复计算，还有就是把某些子图的计算生成等价的GPU版本放到GPU里执行。

### 比如，一个简单的优化可能是把 xy/y 优化成x。

### 例子：

```python
>>> import theano
>>> a = theano.tensor.vector("a")      # declare symbolic variable
>>> b = a + a ** 10                    # build symbolic expression
>>> f = theano.function([a], b)        # compile function
>>> print(f([0, 1, 2]))                # prints `array([0,2,1026])`
[    0.     2.  1026.]

>>> theano.printing.pydotprint(b, outfile="./pics/symbolic_graph_unopt.png", var_with_name_simple=True)  
The output file is available at ./pics/symbolic_graph_unopt.png
>>> theano.printing.pydotprint(f, outfile="./pics/symbolic_graph_opt.png", var_with_name_simple=True)  
The output file is available at ./pics/symbolic_graph_opt.png
```

### 我们定义 b=a+a**10 ，f 是函数，输入a，输出b。

### 下面是没有优化的图：

![这里写图片描述](https://img-blog.csdn.net/20170107124200656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 没有优化的图有两个Op，power和add【还有一个DimShuffle，这个是Theano自己增加的一个Op，对于常量10，theano会创建一个TensorConstant。它是0维的tensor，也就是一个scalar。但是a我们定义的是一个vector，power是一个elementwise的操作，底数是一个vector，那么指数也要是同样大小的vector。dimshuffle(‘x’)就是给0维tensor增加一个维度变成1维的tensor（也就是vector），这样维数就对上了，但是x的shape可能是(100,)的，而常量是(1,)，大小不一样怎么办呢？这就是broadcasting作的事情了，它会把dimshuffle(‘x’, 10)扩展成(100,)的向量，每一个值都是10【实际numpy不会那么笨的复制100个10，不过我们可以这么理解就好了】。之前我们也学过numpy的broadcasting，theano和numpy的broadcasting使用一些区别的，有兴趣的读者可以参考[http://deeplearning.net/software/theano/library/tensor/basic.html#libdoc-tensor-broadcastable](http://deeplearning.net/software/theano/library/tensor/basic.html#libdoc-tensor-broadcastable)。

### 下面是优化过的图：

![这里写图片描述](https://img-blog.csdn.net/20170107124215381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 优化后变成了一个ElementWise（类似于全局函数，对每一个元素做相同的操作）的操作，其实就是把 b=a+a**10 优化成了 b=a+((a**2)**2)**2+a**2。



