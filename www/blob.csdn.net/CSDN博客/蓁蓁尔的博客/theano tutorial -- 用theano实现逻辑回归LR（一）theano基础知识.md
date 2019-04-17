# theano tutorial -- 用theano实现逻辑回归LR（一）theano基础知识 - 蓁蓁尔的博客 - CSDN博客





2017年01月07日 12:27:39[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：215








### theano tutorial 可见：[http://deeplearning.net/software/theano/tutorial/adding.html](http://deeplearning.net/software/theano/tutorial/adding.html)

### 下面的内容和theano的计算图 computing graph 概念结合着学习会更容易理解。

### 1.theano定义符号变量：

#### 3种：标量（scalar）、向量（vector）和矩阵（matrix）

#### 可在前面加’b’,’w’,’i’,’l’,’f’,’d’,’c’分别表示8位，16位，32位，64位的整数，float，double以及负数。比如imatrix就是32位整数类型的矩阵，dvector就是单精度浮点数的向量。

#### 举例子，每一句后面有详细解释：

```python
>>> import theano.tensor as T
>>> x = T.dscalar('x') #  定义了一个名字叫“x”的double类型的标量，并将该符号变量的值赋给了变量x。
>>> y = T.dvector('y')
>>> z = T.imatrix（'z'）

'''上面的三句都是定义了theano的符号变量，其中后面括号中的（'x'）等是该符号变量的名字，是给theano看的
（theano的computing graph 中的名字），然后将符号变量的值赋给等号前面的x，此x是python的普通变量。注意区分'''
```

### 2.function 函数，对应着computing graph 中的operation操作

#### 举例子，每一句后面有详细解释：

```python
>>> import numpy
>>> import theano.tensor as T
>>> from theano import function
>>> x = T.dscalar('x')
>>> y = T.dscalar('y')
>>> z = x + y
>>> f = function([x, y], z) # 第一个参数是一个list，对应着该function的输入列表，第二个参数对应着输出。

'''此函数中输入为符号变量名字为x和y，输出为名字为z的符号变量，此操作并没有执行，下面调用函数以后才执行'''

>>> f(2, 3) #  调用函数，对应着computing graph中的Apply，此时函数才会执行。
array(5.0) #  函数执行结果。
```

#### 注：运行函数 f = function([x, y], z) 会花费比较长的时间，theano会将函数构建成计算图，并且做一些优化。详细过程可见计算图部分。

### 3.theano中的共享变量 shared variable：

#### 共享变量的值在多个函数直接可以共享。可以用符号变量的地方都可以用共享变量。但不同的是，共享变量有一个内部状态的值，这个值可以被多个函数共享。所有函数都可以对该值进行更改。我们可以使用get_value（）和set_value（）方法来读取或者修改共享变量的值。

#### （1）举例子1 ，每一句后面有详细解释：

```python
>>> from theano import shared
>>> state = shared(0) #  定义共享变量，名字为state，此时state.get_value()的值为0。
>>> inc = T.iscalar('inc') #  定义符号变量，名字为inc。
>>> accumulator = function([inc], state, updates=[(state, state+inc)])

'''此函数中输入为符号变量名字为inc，输出为名字为state的共享变量，updates参数是一个list，
     其中每个元素是一个tuple（共享变量，新的表达式），s也可以是一个dict{共享变量，新的表达式}。

   不管用哪种方法，它的意思是：当函数运行完成后，把新的表达式的值赋给这个共享变量。上面的accumulator函数的
   updates是把state+inc赋给state，也就是每次调用accumulator函数后state增加inc。--高效'''
```

#### （2）可以通过共享变量来定义了一个公式(函数)，但是你不想用它的值。这种情况下，你可以用givens这个参数。

#### 举例子2 ，每一句后面有详细解释：

```python
>>> fn_of_state = state * 2 + inc
>>> # The type of foo must match the shared variable we are replacing
>>> # with the ``givens``
>>> foo = T.scalar(dtype=state.dtype)
>>> skip_shared = function([inc, foo], fn_of_state, givens=[(state, foo)])

>>> skip_shared(1, 3)  # we're using 3 for the state, not state.value
array(7) #  结果

>>> print(state.get_value())  # old state still there, but we didn't use it
0  #  结果是0，而不是上面调用函数时传入的3，在computing graph中实际上用foo替换了state。
```

#### 注：givens参数可以取代任何符号变量，而不只是共享变量【从计算图的角度就非常容易理解了，后面我们会讲到Theano的计算图】。你也可以用这个参数来替代常量和表达式。不过需要小心的是替代的时候不要引入循环的依赖。【比如a=b+c，你显然不能把c又givens成a，这样循环展开就不是有向无环图了】



