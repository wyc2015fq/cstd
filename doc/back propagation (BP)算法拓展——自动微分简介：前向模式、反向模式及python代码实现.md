# back propagation (BP)算法拓展——自动微分简介：前向模式、反向模式及python代码实现



## 前言

神经网络的优化多采用诸如梯度下降法的基于一阶导数的优化方法（PS：可参见之前写的一篇文章 <http://www.cnblogs.com/simplex/p/6777705.html>，用下降单纯形优化一个非常简单的神经网络，无需求梯度）。在计算待优化变量的一阶导数时，最常用的方法就是 back propagation (BP)算法。BP算法可以认为是自动微分(Automatic Differentiation )的一个特例，但是很多讲述BP算法在神经网络中应用的文章都是先手工推导出各个参数的导函数表达式，再写代码实现导数的计算，这样不但导致推导十分繁复，而且也没有体现出BP算法的效果，因为BP算法（或者更一般的自动微分算法）的一个重要目的就是避免我们手动计算复杂的解析形式的导函数。本文就自动微分方法做一个介绍，包括比较简单的前向自动微分算法和基于计算图实现的后向自动微分算法（BP算法），并给出了相应的python代码实现。



 

## 自动微分原理简介

自动微分(Automatic Differentiation )的目的是为了求函数在某点的导数值。它可以认为是介于符号计算和数值计算之间的一种求微分的方式，主要原理是利用求导的链式法则：

假设$\mathbf{y}=v_n(v_{n-1}(v_{n-2}\cdots( v_1(\mathbf{x}))))​$，则y关于x的导数$\frac{d\mathbf{y}}{d\mathbf{x}}​$为：
$$
\frac{d\mathbf{y}}{d\mathbf{x}}=\frac{d\mathbf{y}}{dv_n}\frac{dv_n}{d\mathbf{x}}=\frac{d\mathbf{y}}{dv_n}\frac{dv_n}{dv_{n-1}}\frac{dv_{n-1}}{d\mathbf{x}}=\frac{d\mathbf{y}}{dv_{n}}\frac{dv_n}{dv_{n-1}}\cdots \frac{dv_{2}}{dv_1}\frac{dv_1}{d\mathbf{x}}
$$
依据求导的链式法则，为了计算$\frac{d\mathbf{y}}{d\mathbf{x}}$，我们可以依次求出上式中的微分表达式，然后将他们乘起来得到最后的结果。这又有两种实现方式：



1. 前向模式（或tangent-linear mode），即沿着上式乘积的**反向**依次求导，先求$\frac{dv_1}{d\mathbf{x}}$(seed)。它与函数求值的顺序相同，求导所需函数值可以和对应的微分一起求出来。
2. 反向模式（或adjoint mode），即沿着上式乘积的**正向**依次求导，先求$\frac{d\mathbf{y}}{dv_{n}}$(seed)。它与函数求值的顺序相反，求导所需函数值没法和对应的微分一起求出来，得分别计算函数值和到数值。

 

以链式法则的一步计算$\frac{dv_{i+1}}{dv_{i-1}}=\frac{dv_{i+1}}{dv_{i}}\frac{dv_{i}}{dv_{i-1}}$为例，前向和反向模式的区别是：

1. 前向模式中$\frac{dv_{i}}{dv_{i-1}}$是已知的，需要求的是$\frac{dv_{i+1}}{dv_{i}}$，即待求导函数的**自变量**的导数是已知的。
2. 反向模式中$\frac{dv_{i+1}}{dv_{i}}$是已知的，需要求的是$\frac{dv_{i}}{dv_{i-1}}$，即待求导函数的**因变量**的导数是已知的。


注意，由多元微积分可知，MM维空间到NN维空间的函数ff的导数是一个N×M的线性算符（雅可比矩阵），所以在多维情况下，上面涉及到的导数乘积都是矩阵乘积，这也是很多back propagation算法教程中提到的某个节点的导数要**沿所有路径求和**的来历。

两种自动微分模式各有不同的适用范围。对于多变量的情况，设x和y分别是M和N维的变量，当M≪N时，比较适合用前向模式，因为每次自动微分都求出所有相关节点关于一个自变量分量$x_i$的导数$d\mathbf{\cdot}/dx_i$，一共只需要执行M次自动求导便可求出完整的雅克比矩阵。反之，当M≫N时，适合用反向模式，因为每次自动求导都求出函数$y_i(\mathbf{x})$关于所有相关节点的导数$dy_i/d\mathbf{\cdot}$，要求出完成的雅克比矩阵需要执行N次自动求导。在实际问题中，输入x的维度往往远大于输出y的维度(比如在物理问题中x通常表示计算空间的离散网格，以及在神经网络中的应用)，因此反向模式更为常用。
关于自动微分详细的教程可以参见文献 [1] ，从原理到实现都有很详细的介绍。

## python代码实现

上一章节关于前向模式和反向模式的描述比较适合概念上的理解，本节首先提供一种更适合实际代码实现的描述。假设$\mathbf{y}=f(\mathbf{x})$，引入两个辅助变量s和t,并定义：
$$
\mathbf{x}^{(1)}=\frac{\partial \mathbf{x}}{\partial s}\
\mathbf{y}^{(1)}=\frac{\partial \mathbf{y}}{\partial s}
$$
以及
$$
\mathbf{x}_{(1)}=\frac{\partial t}{\partial \mathbf{x}}\\
\mathbf{y}_{(1)}=\frac{\partial t}{\partial \mathbf{y}}
$$
这样，前向微分可以表示成已知$\mathbf{x}^{(1)}$求$\mathbf{y}^{(1)}$，即利用公式$\mathbf{x}^{(1)}=\mathbf{y}^{(1)}\frac{d\mathbf{y}}{d\mathbf{x}}$；而反向微分则是已知$\mathbf{y}^{(1)}$求$\mathbf{x}^{(1)}$，即利用公式$\mathbf{x}^{(1)}=\mathbf{y}^{(1)}\frac{d\mathbf{y}}{d\mathbf{x}}$。两者的共同点都是要求导数$\frac{d\mathbf{y}}{d\mathbf{x}}$，而不同点是一个是已知x的导数，一个则已知y的导数。用有向图来表述就是s->x->y->t，在求出x->y这部分导数之后，前向模式是计算s->x->y这部分导数，而反向模式是计算x->y->t这部分导数。

另外，分别令s=x和t=y，可以发现前向模式中，$\mathbf{x}^{(1)}=1$，$\mathbf{y}^{(1)}=\frac{d\mathbf{y}}{d\mathbf{x}}$为所求导数；反向模式中，$\mathbf{y}_{(1)}=1$，$\mathbf{x}_{(1)}=\frac{d\mathbf{y}}{d\mathbf{x}}$为所求导数。这点在下面的代码实现中可以看得非常清晰。

### 前向模式

实现前向模式自动微分的最好方法是利用运算符重载，把只求函数值的原始运算符改为同时计算函数值以及导数值的算符，然后在计算函数值的过程中就可以同时得到相应导数值。

首先定义ADT类，包含重载的运算符

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```python
import numpy as np
from prettytable import PrettyTable
class ADT:
    x = 0
    dx = 0

    def __init__(self, x, dx):
        self.x = x
        self.dx = dx

    def __str__(self):
        return "value:%s\nderivative:%s" % (self.x, self.dx)

    def __add__(self, other):
        if isinstance(other, ADT):
            x = self.x + other.x
            dx = self.dx + other.dx
        elif isinstance(other, int) or isinstance(other, float):
            x = self.x + other
            dx = self.dx
        else:
            return NotImplemented
        return ADT(x, dx)

    def __radd__(self, other):
        if isinstance(other, int) or isinstance(other, float):
            x = self.x + other
            dx = self.dx
        else:
            return NotImplemented
        return ADT(x, dx)

    def __iadd__(self, other):
        if isinstance(other, ADT):
            self.x += other.x
            self.dx += other.dx
        elif isinstance(other, int) or isinstance(other, float):
            self.x += other
        else:
            return NotImplemented
        return self

    def __sub__(self, other):
        if isinstance(other, ADT):
            x = self.x - other.x
            dx = self.dx - other.dx
        elif isinstance(other, int) or isinstance(other, float):
            x = self.x - other
            dx = self.dx
        else:
            return NotImplemented
        return ADT(x, dx)

    def __rsub__(self, other):
        if isinstance(other, int) or isinstance(other, float):
            x = other - self.x
            dx = -self.dx
        else:
            return NotImplemented
        return ADT(x, dx)

    def __isub__(self, other):
        if isinstance(other, ADT):
            self.x -= other.x
            self.dx -= other.dx
        elif isinstance(other, int) or isinstance(other, float):
            self.x -= other
        else:
            return NotImplemented
        return self

    def __mul__(self, other):
        if isinstance(other, ADT):
            x = self.x * other.x
            dx = self.x * other.dx + self.dx * other.x
        elif isinstance(other, int) or isinstance(other, float):
            x = self.x * other
            dx = self.dx * other
        else:
            return NotImplemented
        return ADT(x, dx)

    def __rmul__(self, other):
        if isinstance(other, int) or isinstance(other, float):
            x = self.x * other
            dx = self.dx * other
        else:
            return NotImplemented
        return ADT(x, dx)

    def __imul__(self, other):
        if isinstance(other, ADT):
            self.x *= other.x
            self.dx = self.x * other.dx + self.dx * other.x
        elif isinstance(other, int) or isinstance(other, float):
            self.x *= other
            self.dx *= other
        else:
            return NotImplemented
        return self

    def __truediv__(self, other):
        if isinstance(other, ADT):
            x = self.x / other.x
            dx = (self.dx * other.x - self.x * other.dx) / other.x**2
        elif isinstance(other, int) or isinstance(other, float):
            x = self.x / other
            dx = self.dx / other
        else:
            return NotImplemented
        return ADT(x, dx)

    def __rtruediv__(self, other):
        if isinstance(other, int) or isinstance(other, float):
            x = other / self.x
            dx = -other / (self.x)**2 * self.dx
        else:
            return NotImplemented
        return ADT(x, dx)

    def __itruediv__(self, other):
        if isinstance(other, ADT):
            self.x /= other.x
            self.dx = (self.dx * other.x - self.x * other.dx) / other.x**2
        elif isinstance(other, int) or isinstance(other, float):
            self.x /= other
            self.dx /= other
        else:
            return NotImplemented
        return ADT(x, dx)

    def __pow__(self, n):
        x = self.x**n
        dx = n * self.x**(n - 1) * self.dx
        return ADT(x, dx)

    # some frequently used function

    def exp(this):
        x = np.exp(this.x)
        dx = this.dx * np.exp(this.x)
        return ADT(x, dx)

    def log(this):
        x = np.log(this.x)
        dx = 1 / this.x * this.dx
        return ADT(x, dx)

    def sin(this):
        x = np.sin(this.x)
        dx = this.dx * np.cos(this.x)
        return ADT(x, dx)

    def cos(this):
        x = np.cos(this.x)
        dx = -this.dx * np.sin(this.x)
        return ADT(x, dx)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

再定义一个辅助函数，用于打印计算结果

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```python
def table_show(rows):
    table = PrettyTable()
    table.field_names = ["Method", "Value", "Derivative"]
    for row in rows:
        table.add_row(row)
    print('\n')
    print(table)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

最后，对代码做测试：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```python
## one-dimensional 1
x = 0.5
dx = 1

z = ADT(x, dx)

result = ADT.exp((z + 2)**2)

y_analytical = np.exp((x + 2)**2)
dy_analytical = 2 * (x + 2) * np.exp((x + 2)**2)

rows = [['auto diff', result.x, result.dx],
        ['analytical', y_analytical, dy_analytical]]
table_show(rows)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

计算结果如下：

```
+------------+---------------+---------------+
|   Method   |     Value     |   Derivative  |
+------------+---------------+---------------+
| auto diff  | 518.012824668 | 2590.06412334 |
| analytical | 518.012824668 | 2590.06412334 |
+------------+---------------+---------------+
```

 

### 反向模式

反向模式相对前向模式来说比较复杂，因为要首先正向求出函数值，然后再反向求导数。这一般用computation graph实现：将函数求值过程中的所有中间节点按求值顺序存储到栈中，得到表达式对应的计算图，然后再挨个弹出栈中的元素，求相应的导数。

首先定义计算图类CGraph和图的节点Node类，其中CGraph包含了正向求值和反向求导函数，Node包含了重载了的计算函数。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```python
import numpy as np
from enum import Enum
from prettytable import PrettyTable
import tensorflow as tf


class CGraph:
    def __init__(self):
        self.Nodec = 0
        self.NodeList = []

    def connect(self):
        Node.cgraph = self
        return self

    def clear(self):
        self.Nodec = 0
        self.NodeList = []

    def disconnect(self):
        Node.cgraph = None
        return self

    def append(self, node):
        self.Nodec += 1
        self.NodeList.append(node)

    def compute_value(self):
        for node in self.NodeList:
            if node.op is Node.operators.add:
                node.value = self.NodeList[node.arg1].value + self.NodeList[
                    node.arg2].value
            elif node.op is Node.operators.sub:
                node.value = self.NodeList[node.arg1].value - self.NodeList[
                    node.arg2].value
            elif node.op is Node.operators.mul:
                node.value = self.NodeList[node.arg1].value * self.NodeList[
                    node.arg2].value
            elif node.op is Node.operators.truediv:
                node.value = self.NodeList[node.arg1].value / self.NodeList[
                    node.arg2].value
            elif node.op is Node.operators.power:
                node.value = self.NodeList[node.arg1].value**self.NodeList[
                    node.arg2].value
            elif node.op is Node.operators.sin:
                node.value = np.sin(self.NodeList[node.arg1].value)
            elif node.op is Node.operators.cos:
                node.value = np.cos(self.NodeList[node.arg1].value)
            elif node.op is Node.operators.log:
                node.value = np.log(self.NodeList[node.arg1].value)
            elif node.op is Node.operators.exp:
                node.value = np.exp(self.NodeList[node.arg1].value)

    def compute_gradient(self, seedID):
        for node in self.NodeList:
            node.derivative = 0.0
        self.NodeList[seedID].derivative = 1.0
        for node in self.NodeList[-1::-1]:
            if node.op is Node.operators.add:
                self.NodeList[node.arg1].derivative += node.derivative
                self.NodeList[node.arg2].derivative += node.derivative
            elif node.op is Node.operators.sub:
                self.NodeList[node.arg1].derivative += node.derivative
                self.NodeList[node.arg2].derivative += -node.derivative
            elif node.op is Node.operators.mul:
                self.NodeList[
                    node.arg1].derivative += node.derivative * self.NodeList[
                        node.arg2].value
                self.NodeList[
                    node.arg2].derivative += node.derivative * self.NodeList[
                        node.arg1].value
            elif node.op is Node.operators.truediv:
                self.NodeList[
                    node.arg1].derivative += node.derivative / self.NodeList[
                        node.arg2].value
                self.NodeList[
                    node.arg2].derivative += -node.derivative * self.NodeList[
                        node.arg1].value / (self.NodeList[node.arg2].value)**2
            elif node.op is Node.operators.power:
                self.NodeList[
                    node.arg1].derivative += node.derivative * self.NodeList[
                        node.arg2].value * self.NodeList[node.arg1].value**(
                            self.NodeList[node.arg2].value - 1)
            elif node.op is Node.operators.sin:
                self.NodeList[
                    node.arg1].derivative += node.derivative * np.cos(
                        self.NodeList[node.arg1].value)
            elif node.op is Node.operators.cos:
                self.NodeList[
                    node.arg1].derivative += -node.derivative * np.sin(
                        self.NodeList[node.arg1].value)
            elif node.op is Node.operators.log:
                self.NodeList[
                    node.arg1].derivative += node.derivative / self.NodeList[
                        node.arg1].value
            elif node.op is Node.operators.exp:
                self.NodeList[
                    node.arg1].derivative += node.derivative * np.exp(
                        self.NodeList[node.arg1].value)


class Node:
    cgraph = None
    operators = Enum('operators', ('add', 'sub', 'mul', 'truediv', 'power',
                                   'sin', 'cos', 'log', 'exp'))

    def __init__(self,
                 value=np.NaN,
                 derivative=0.0,
                 op=None,
                 arg1=None,
                 arg2=None,
                 name=None):
        self.value = value
        self.derivative = derivative
        self.op = op
        self.arg1 = arg1
        self.arg2 = arg2
        self.name = name
        # if graph is trace on, allocate an id for every newly created Node and add it
        # to the graph.
        if Node.cgraph is not None:
            self.ID = Node.allocate_ID()
            Node.cgraph.append(self)

    def __str__(self):
        return '\nvalue:%s\nderivative:%s\nop:%s\narg1:%s\narg2:%s\nname:%s' % (
            self.value, self.derivative, self.op, self.arg1, self.arg2,
            self.name)

    @classmethod
    def allocate_ID(cls):
        if cls.cgraph is not None:
            return cls.cgraph.Nodec
        else:
            return None

    @classmethod
    def tome(cls, x):
        if isinstance(x, cls):
            return x
        else:
            # x is always constant
            return cls(x, op='constant')

    def __add__(self, other):
        other = Node.tome(other)
        return Node(
            self.value + other.value,
            op=Node.operators.add,
            arg1=self.ID,
            arg2=other.ID)

    def __sub__(self, other):
        other = Node.tome(other)
        return Node(
            self.value - other.value,
            op=Node.operators.sub,
            arg1=self.ID,
            arg2=other.ID)

    def __mul__(self, other):
        other = Node.tome(other)
        return Node(
            self.value * other.value,
            op=Node.operators.mul,
            arg1=self.ID,
            arg2=other.ID)

    def __truediv__(self, other):
        other = Node.tome(other)
        return Node(
            self.value / other.value,
            op=Node.operators.truediv,
            arg1=self.ID,
            arg2=other.ID)

    def __radd__(self, other):
        other = Node.tome(other)
        return Node(
            other.value + self.value,
            op=Node.operators.add,
            arg1=other.ID,
            arg2=self.ID)

    def __rsub__(self, other):
        other = Node.tome(other)
        return Node(
            other.value - self.value,
            op=Node.operators.sub,
            arg1=other.ID,
            arg2=self.ID)

    def __rmul__(self, other):
        other = Node.tome(other)
        return Node(
            other.value * self.value,
            op=Node.operators.mul,
            arg1=other.ID,
            arg2=self.ID)

    def __rtruediv__(self, other):
        other = Node.tome(other)
        return Node(
            other.value / self.value,
            op=Node.operators.truediv,
            arg1=other.ID,
            arg2=self.ID)

    def __pow__(self, other):
        other = Node.tome(other)
        return Node(
            self.value**other.value,
            op=Node.operators.power,
            arg1=self.ID,
            arg2=other.ID)

    def sin(self):
        return Node(np.sin(self.value), op=Node.operators.sin, arg1=self.ID)

    def cos(self):
        return Node(np.cos(self.value), op=Node.operators.cos, arg1=self.ID)

    def log(self):
        return Node(np.log(self.value), op=Node.operators.log, arg1=self.ID)

    def exp(self):
        return Node(np.exp(self.value), op=Node.operators.exp, arg1=self.ID)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

然后利用上面定义的类进行求导计算。注意输入变量在初始化时没有赋值，而是等到计算图建立之后再赋值，并利用compute_value函数先进行正向求值。当然，如果已知输入变量的值，那么在初始化时可以直接赋值，后续所有中间节点的值都会自动求出，无需再调用compute_value函数。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```python
graph = CGraph()
graph.connect()
x1 = Node()
x2 = Node()
x3 = Node()
x4 = Node()
y1 = Node.sin(x1 * x2) + Node.exp(x1 / x2) + x3**2 - x4**3
y2 = x3 * x4

# assign value for each input
x1.value = 1.234
x2.value = 2.345
x3.value = 3.456
x4.value = 4.567

# forward pass, compute the function value.
graph.compute_value()

# backward pass, compute the derivative
result = []
graph.compute_gradient(y1.ID)  
result.append(x1.derivative)
result.append(x2.derivative)
result.append(x3.derivative)
result.append(x4.derivative)

graph.compute_gradient(y2.ID)
result.append(x3.derivative)
result.append(x4.derivative)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

接下来，与tensorflow中的求导函数做对比

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
xx1 = tf.Variable(x1.value, dtype=tf.float32)
xx2 = tf.Variable(x2.value, dtype=tf.float32)
xx3 = tf.Variable(x3.value, dtype=tf.float32)
xx4 = tf.Variable(x4.value, dtype=tf.float32)
yy1 = tf.sin(xx1 * xx2) + tf.exp(xx1 / xx2) + xx3**2 - xx4**3
yy2 = xx3 * xx4
dyy1dxx1 = tf.gradients(yy1, xx1)
dyy1dxx2 = tf.gradients(yy1, xx2)
dyy1dxx3 = tf.gradients(yy1, xx3)
dyy1dxx4 = tf.gradients(yy1, xx4)
dyy2dxx3 = tf.gradients(yy2, xx3)
dyy2dxx4 = tf.gradients(yy2, xx4)

sess = tf.Session()
sess.run(tf.global_variables_initializer())
result_tf = sess.run(
    [dyy1dxx1, dyy1dxx2, dyy1dxx3, dyy1dxx4, dyy2dxx3, dyy2dxx4])
result_tf=[a[0] for a in result_tf]
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

最后，打印结果对比：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```python
table = PrettyTable()
table.field_names = ["Method", "dy1/dx1","dy1/dx2","dy1/dx3","dy1/dx4","dy2/dx3","dy2/dx4"]
row1=['this code']
row1.extend(result)
row2=['tensorflow']
row2.extend(result_tf)
table.add_row(row1)
table.add_row(row2)
print('\n')
print(table)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

计算结果如下：

| Method     | dy1/dx1        | dy1/dx2       | dy1/dx3 | dy1/dx4    | dy2/dx3 | dy2/dx4 |
| ---------- | -------------- | ------------- | ------- | ---------- | ------- | ------- |
| this code  | -1.55157212465 | -1.5760978298 | 6.912   | -62.572467 | 4.567   | 3.456   |
| tensorflow | -1.55157       | -1.5761       | 6.912   | -62.5725   | 4.567   | 3.456   |








## 参考文献

[1] The Art of Differentiating Computer Programs- An Introduction toAlgorithmic Differentiation
[2] Deep Learning

 

## 附录

假设$\mathbf{z}=g(\mathbf{y})$,$\mathbf{y}=f(\mathbf{x})$，其中x，y，z分别是M，N，P维变量，则$\frac{d\mathbf{y}}{d\mathbf{x}}$，$\frac{d\mathbf{z}}{d\mathbf{y}}$，$\frac{d\mathbf{y}}{d\mathbf{x}}$分别是N×M，P×N，P×M的矩阵，且有$\frac{d\mathbf{z}}{d\mathbf{x}}=\frac{d\mathbf{z}}{d\mathbf{y}}\frac{d\mathbf{y}}{d\mathbf{x}}$。写成分量形式即为：
$$
\frac{\partial z_i}{\partial x_j}=\sum_{k=1}^{N}\frac{\partial z_i}{\partial y_k}\frac{\partial y_k}{\partial x_j}
$$
在计算图模型中，上式的求和可以认为是对从$x_j$到$z_i$的所有不同路径（即通过不同的$y_k$到达$z_i$）进行的。