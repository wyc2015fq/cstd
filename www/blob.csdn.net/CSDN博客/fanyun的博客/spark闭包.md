# spark闭包 - fanyun的博客 - CSDN博客
2016年03月17日 12:29:21[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2551
所属专栏：[大数据](https://blog.csdn.net/column/details/bigdata01.html)
# 定义
关于闭包有太多种解释，但基本上都很难用一两句解释清楚，下面这句简短的定义是我见过的最精炼且准确的解释了：
```
A closure is a function that carries an implicit binding to all the variables referenced within it. In other words, the function (or method) encloses a context around the things it references.
```
首先，闭包是一个函数，然后，也是最本质的地方：这个函数内部会引用（依赖）到一些变量，这些变量既不是全局的也不是局部的，而是在定义在上下文中的（这种变量被称为“自由变量”，我们会在稍后的例子中看到这种变量），闭包的“神奇”之处是它可以“cache”或者说是持续的“trace”它所引用的这些变量。（从语言实现层面上解释就是：这些变量以及它们引用的对象不会被GC释放）。同样是这件事情，换另一种说法就是：闭包是一个函数，但同时这个函数“背后”还自带了一个“隐式”的上下文保存了函数内部引用到的一些（自由）变量。
# 第一个例子
![](http://ww2.sinaimg.cn/large/67a6a651gw1f0amh3dg9mj20xn0jrtnu.jpg)
# 第二个例子
![](http://ww1.sinaimg.cn/large/67a6a651gw1f0amh80aagj20mu0irtg5.jpg)
# 对两个例子的补充
上述两个例子的代码都在解释闭包的概念，但是解释的角度不太一样，相对而言第二个例子揭示地更为深刻一些，它揭示闭包会隐式地持续trace（也就是不会被垃圾回收）它所使用的那些自由变量！
每当我们去调用一个闭包时，脑子里一定要意识到：闭包不单单是定义它的那段代码，同时还有一个绑定在它“后面”（隐式的）的持续保持它所引用的所有自用变量的一个“上下文”（“环境”）！
# 更透彻地理解：闭包产生的根源
某种角度上，我们可以说闭包是函数字面量的一个“衍生品”。函数字面量的存在使得函数的定义与普通变量无异，也就是val 变量名=函数字面量，既然普通变量在赋值时可以引用另一个变量的值，那么定义函数时，在函数字面量里引用其他变量也变成非常自然的事情（而在传统的函数体内是没有办法直接引用函数体外部的变量的），比如,像下面这样定义普通变量是非常常见的：
<code class="hljs cs has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">var</span> a=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>;
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">var</span> b=a+<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>;</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li></ul>
显然变量b的赋值过程中引用了变量a. 同样的，在函数编程语言里，像下面这样定义函数：
<code class="hljs coffeescript has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-reserved" style="box-sizing: border-box;">var</span> a=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>;
val <span class="hljs-function" style="box-sizing: border-box;"><span class="hljs-title" style="box-sizing: border-box;">b</span>=<span class="hljs-params" style="color: rgb(102, 0, 102); box-sizing: border-box;">()</span>=></span>a+<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span></code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li></ul>
又有何不可呢？这时b成了就成了典型的闭包，它所引用的变量的a是定义在上下文的。
# 为什么需要闭包
闭包被创造出来显然是因为有场景需要的。一个最为普遍和典型的使用场合是：推迟执行。我们可以把一段代码封装到闭包里，你可以等到“时机”成熟时去执行它。比如：在Spark里，针对RDD的计算任务都要分布到每个节点（准确的说是executor）上并行处理，Spark就需要封装一个闭包，把相关的操作（方法）和需要的变量引入到闭包中分发给节点执行。
