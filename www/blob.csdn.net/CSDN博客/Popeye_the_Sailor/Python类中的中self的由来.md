# Python类中的中self的由来 - Popeye_the_Sailor - CSDN博客
2019年03月11日 22:33:46[_Sailor_](https://me.csdn.net/lz0499)阅读数：17
**                                                                   Python****类中的中****self****的由来**
**1.由类创建对象**
要使用一个类需要从类创建对象，这称为对象实例化。Eg:
假设创建的类为(暂时定义为一个空类)：                                             
```python
class CountFromBy:
        pass
```
假设创建两个对象：
```python
a = CountFromBy()
b = CountFromBy()
c = CountFromBy()
```
上面两条语句，我怎么知道CountFromBy()是函数调用还是类的对象实例化呢？
在Python社区有一个普遍的约定：
函数用小写字母命名（可以加下划线来强调），而类是所有单词连在一起，并且每个单词的首写字母需要大写。按照这个约定，我们很容易就知道count_from_by()是一个函数调用；而CountFromBy()会创建一个对象。
**2.对象共享行为，但不共享状态**
行为（方法）：由同一个类创建，所有对象共享
状态（属性）：各个对象独立拥有。不会被同一个类创建的其他对象共享
**3.类的方法调用细节**
假设类中有一方法increase，当我们用点记法调用的时候有:
`c.increase()`
解释器中会把上面的代码转换为如下调用形式：
```python
CountFromBy.increase(c)
```
也就是说，解释器会把对象本身传到increase函数中，即可辨识每一个对象各自的属性了。
所以increase函数应该至少拥有一个参数用来保存传进来的对象。在Python社区中一种约定俗成的做法是在各个方法的第一个参数指定一个特殊的名字：self
所以，在类中定义方法的时候需要指定这个对象传给类实例的参数self。
故类的方法定义为：
```python
def increase(self):
```
...........
总结：在对象调用一个方法的时候，Python总是会把调用的对象实例座位方法的第一个参数。这个对象总是赋值给各个方法的self参数。这就是为什么self要作为每一个对象方法的第一个参数。调用方法时，你不需要为self提供一个值，因为解释器会为你做这个一切。
如果没有在类的方法中增加self参数，解释器会显示一堆的TypeError，告诉你缺少一些东西。而这些缺少的东西就是self。
**4.为类增加方法**
之前我们定义类：
```python
class CountFromBy:
      pass
```
现在我们需要实现其方法。用increase替换掉pass。想回忆一下调用increase的方法。
```python
c.increase（）
```
根据这个调用你可能认为increase方法没有参数，因为小括号中没有任何参数是吧？不过这并不是全部。
解释器会把上述的语句转换为：
`CountFromBy.increase(c)`
基于上述考虑，我们实现increase方法为：
```python
class CountFromBy：
         def increase(self) ->None:
```
increase没有其他参数，所以def行上除了self之外，我们没有传入其他任何参数。在这里self机器重要，因为如果忘记self会导致语法错误。
下面我们实现increase函数。我们假设需要维护两个变量val,incr。val包含当前对象的值，incr包含每次调用increase时val的增量。了解到这点，你可能会写出如下代码：
val += incr
而实际上应该这样写：
self.val += self.incr
因为val incr是两个类的状态，并不对所有类的对象实例共享。每一个类的对象实例会维护其独立的属性。而这恰好就是通过关联属性和对象做到的，也就是说通过self关联到每一个对象的实例属性。
假设我们写的是如下代码：
```python
class CountFromBy：
    def increase(self) ->None:
        val += incr
```
从表面上看这段代码没有任何问题，因为它要做的就是增加val的当前值，增量为incr的当前值。但是当这个increase函数结束的时候会发生什么：val incr都是仅仅存在于increase函数中，当函数结束的时候瞬间它们将会被撤销，这些变量将不再存在。为了保证函数结束的时候属性的赋值仍然有效，属性的值必须赋给函数结束时不会被撤销的某个东西。而这个东西就是属于这个方法的当前对象，它存储在self中。这就解释了为什么每一个方法代码中的每一个属性值前都需要加上self。
