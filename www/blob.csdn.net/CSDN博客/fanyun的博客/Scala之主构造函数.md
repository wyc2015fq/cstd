# Scala之主构造函数 - fanyun的博客 - CSDN博客
2016年03月17日 12:25:38[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：951
所属专栏：[大数据](https://blog.csdn.net/column/details/bigdata01.html)
## 主构造函数
       首先，我们必须要非常清晰明确的认识到：主构造函数不是你看到的class后面跟的参数列表，那怎么可能是主构造函数呢？那只是主构造函数的函数列表！那主构造函数的函数体在那里呢？答案是：class body里所有除去字段和方法声明的语句，剩下的一切都是主构造函数的，它们在class实例化时一定会被执行。
所以说，Scala的主构造函数包含这些部分：
         1.The constructor parameters
         2.Methods that are called in the body of the class
        3.Statements and expressions that are executed in the body of the class
请看实例代码中的class Person1以及它的输出。从这个例子上我们可以看出：主构造函数看上去和类的定义已经完全融合在了一起！它的参数列表放到了类名的后面（我们也可以直接叫它类参数列表），它的方法体就是整个类体，实例化一个类时，类体（主构造函数）中所有可行的部分都会被执行，不管是函数调用还是表达式等等，只是对于类的字段和方法声明而言是没有什么可执行的，它们只是声明而已。
## 主构造函数的参数（类参数）
首先，我们还是要非常清晰明确的认识到：
- 在主构造函数的参数列表中声明的参数和在类体中声明的变量本质上没有任何不同！基于前面我们对主构造参数的理解，这就像是：在函数的参数列表中声明的参数和方法体中声明的变量本质上没有任何不同一样！
那么接下来的问题就是要讨论不管是主构造函数列表中的参数还是类体中的字段，它们的可见性到底怎样定义？
首先，要说明的是：var/val限定的是变量是否可读写，与可见性无关，即对外可见；public和private声明的才是可见性，只是说：对于类字段和类参数来说，如果没有特别指定，它们总是public的。
规则如下：
     (1).对于var修饰的参数：外部可读/可改写 （实际上是：编译器为该类参数（字段）自动生成了getter和setter）
     (2.)对于val修饰的参数：外部可读/不可改写（实际上是：编译器为该类参数（字段）只生成了getter没有生成setter）
     (3.)对于private var修饰的参数：内部可读/可改写 （编译器不会为私有类参数（字段）自动生成getter和setter）
     (4.)对于private val修饰的参数：内部可读/不可改写 （编译器不会为该类参数（字段）自动生成getter和setter）
     对于主构造函数的参数列表中声明的参数和在类体中声明的变量是否地位一致以及它们的可见性，请看实例代码中的class Person2和Person3以及它们的输出。
## 类字段的getter/setter
虽然通过val/var,我们已经可以轻松地指定字段的外部可访问性了，但是我们还是要清楚的知道under the hood! 也就是说在scala里getter和setter是怎么写的，虽然绝大多数情况下我们不会再像java那样去手动地编写getter与setter了。
示例代码中的Person4给我们一个很好的示例。虽然参数_age被声明为了私有的，但是通过手动的添加getter和setter，我们还是一样可以在外部改写他们。
## 示例代码
<code class="language-scala hljs  has-numbering" style="display: block; padding: 0px; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-class" style="box-sizing: border-box;"><span class="hljs-keyword" style="box-sizing: border-box;">object</span> <span class="hljs-title" style="box-sizing: border-box;">PrimaryConstructorDemo</span> {</span>
    <span class="hljs-comment" style="box-sizing: border-box;">//Person1 is to show: primary constructor consists of not only class args list but also all runnable part in class body.</span>
    <span class="hljs-class" style="box-sizing: border-box;"><span class="hljs-keyword" style="box-sizing: border-box;">class</span> <span class="hljs-title" style="box-sizing: border-box;">Person1</span><span class="hljs-params" style="box-sizing: border-box;">(var firstName: String, var lastName: String)</span> {</span>
        println(<span class="hljs-string" style="box-sizing: border-box;">"the constructor begins"</span>)
        <span class="hljs-comment" style="box-sizing: border-box;">// some class fields</span>
        <span class="hljs-keyword" style="box-sizing: border-box;">private</span> <span class="hljs-keyword" style="box-sizing: border-box;">val</span> HOME = <span class="hljs-string" style="box-sizing: border-box;">"/root"</span>
        <span class="hljs-keyword" style="box-sizing: border-box;">var</span> age = <span class="hljs-number" style="box-sizing: border-box;">30</span>
        <span class="hljs-comment" style="box-sizing: border-box;">// some methods</span>
        <span class="hljs-keyword" style="box-sizing: border-box;">override</span> <span class="hljs-keyword" style="box-sizing: border-box;">def</span> toString = s<span class="hljs-string" style="box-sizing: border-box;">"$firstName $lastName is $age years old"</span>
        <span class="hljs-keyword" style="box-sizing: border-box;">def</span> printHome { println(s<span class="hljs-string" style="box-sizing: border-box;">"HOME = $HOME"</span>) }
        <span class="hljs-keyword" style="box-sizing: border-box;">def</span> printFullName { println(<span class="hljs-keyword" style="box-sizing: border-box;">this</span>) } <span class="hljs-comment" style="box-sizing: border-box;">// uses toString</span>
        printHome
        printFullName
        println(<span class="hljs-string" style="box-sizing: border-box;">"still in the constructor"</span>)
    }
    <span class="hljs-comment" style="box-sizing: border-box;">// Person2 is to show: the visibility of class fields</span>
    <span class="hljs-class" style="box-sizing: border-box;"><span class="hljs-keyword" style="box-sizing: border-box;">class</span> <span class="hljs-title" style="box-sizing: border-box;">Person2</span> {</span>
        <span class="hljs-keyword" style="box-sizing: border-box;">var</span> age = <span class="hljs-number" style="box-sizing: border-box;">30</span>
        <span class="hljs-keyword" style="box-sizing: border-box;">val</span> gender = <span class="hljs-string" style="box-sizing: border-box;">"male"</span>
        <span class="hljs-keyword" style="box-sizing: border-box;">private</span> <span class="hljs-keyword" style="box-sizing: border-box;">val</span> healthy = <span class="hljs-keyword" style="box-sizing: border-box;">false</span>
    }
    <span class="hljs-comment" style="box-sizing: border-box;">// Person3 is to show: the visibility of primary constructor args</span>
    <span class="hljs-class" style="box-sizing: border-box;"><span class="hljs-keyword" style="box-sizing: border-box;">class</span> <span class="hljs-title" style="box-sizing: border-box;">Person3</span><span class="hljs-params" style="box-sizing: border-box;">(var age:Int,val gender:String,private val healthy:Boolean)</span></span>
    <span class="hljs-comment" style="box-sizing: border-box;">// Person4 is to show: change visibility for primary constructor args</span>
    <span class="hljs-class" style="box-sizing: border-box;"><span class="hljs-keyword" style="box-sizing: border-box;">class</span> <span class="hljs-title" style="box-sizing: border-box;">Person4</span><span class="hljs-params" style="box-sizing: border-box;">(private var _age:Int)</span> {</span>
        <span class="hljs-keyword" style="box-sizing: border-box;">def</span> age = _age <span class="hljs-comment" style="box-sizing: border-box;">// this is getter</span>
        <span class="hljs-keyword" style="box-sizing: border-box;">def</span> age_=(newAge: Int) = _age = newAge <span class="hljs-comment" style="box-sizing: border-box;">//this is setter</span>
    }
    <span class="hljs-keyword" style="box-sizing: border-box;">def</span> main(args: Array[String]) {
        <span class="hljs-keyword" style="box-sizing: border-box;">val</span> p1 = <span class="hljs-keyword" style="box-sizing: border-box;">new</span> Person1(<span class="hljs-string" style="box-sizing: border-box;">"Tome"</span>,<span class="hljs-string" style="box-sizing: border-box;">"White"</span>)
        println(<span class="hljs-string" style="box-sizing: border-box;">"---------------------------------"</span>)
        <span class="hljs-keyword" style="box-sizing: border-box;">val</span> p2 = <span class="hljs-keyword" style="box-sizing: border-box;">new</span> Person2
        println(p2.age)
        p2.age = <span class="hljs-number" style="box-sizing: border-box;">40</span>;
        println(p2.age)
        println(p2.gender)
        <span class="hljs-comment" style="box-sizing: border-box;">//error, no setter for gender.</span>
        <span class="hljs-comment" style="box-sizing: border-box;">//p2.gender = false</span>
        <span class="hljs-comment" style="box-sizing: border-box;">//error, invisible out of class.</span>
        <span class="hljs-comment" style="box-sizing: border-box;">//println(p2.healthy)</span>
        println(<span class="hljs-string" style="box-sizing: border-box;">"---------------------------------"</span>)
        <span class="hljs-keyword" style="box-sizing: border-box;">val</span> p3 = <span class="hljs-keyword" style="box-sizing: border-box;">new</span> Person3(<span class="hljs-number" style="box-sizing: border-box;">30</span>,<span class="hljs-string" style="box-sizing: border-box;">"male"</span>,<span class="hljs-keyword" style="box-sizing: border-box;">false</span>)
        println(p3.age)
        p3.age = <span class="hljs-number" style="box-sizing: border-box;">40</span>;
        println(p3.age)
        println(p3.gender)
        <span class="hljs-comment" style="box-sizing: border-box;">//error, no setter for gender.</span>
        <span class="hljs-comment" style="box-sizing: border-box;">//p3.gender = false</span>
        <span class="hljs-comment" style="box-sizing: border-box;">//error, invisible out of class.</span>
        <span class="hljs-comment" style="box-sizing: border-box;">//println(p3.healthy)</span>
        println(<span class="hljs-string" style="box-sizing: border-box;">"---------------------------------"</span>)
        <span class="hljs-keyword" style="box-sizing: border-box;">val</span> p4 = <span class="hljs-keyword" style="box-sizing: border-box;">new</span> Person4(<span class="hljs-number" style="box-sizing: border-box;">30</span>)
        println(p4.age)
        p4.age = <span class="hljs-number" style="box-sizing: border-box;">40</span>
        println(p4.age)
    }
}</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li><li style="box-sizing: border-box; padding: 0px 5px;">36</li><li style="box-sizing: border-box; padding: 0px 5px;">37</li><li style="box-sizing: border-box; padding: 0px 5px;">38</li><li style="box-sizing: border-box; padding: 0px 5px;">39</li><li style="box-sizing: border-box; padding: 0px 5px;">40</li><li style="box-sizing: border-box; padding: 0px 5px;">41</li><li style="box-sizing: border-box; padding: 0px 5px;">42</li><li style="box-sizing: border-box; padding: 0px 5px;">43</li><li style="box-sizing: border-box; padding: 0px 5px;">44</li><li style="box-sizing: border-box; padding: 0px 5px;">45</li><li style="box-sizing: border-box; padding: 0px 5px;">46</li><li style="box-sizing: border-box; padding: 0px 5px;">47</li><li style="box-sizing: border-box; padding: 0px 5px;">48</li><li style="box-sizing: border-box; padding: 0px 5px;">49</li><li style="box-sizing: border-box; padding: 0px 5px;">50</li><li style="box-sizing: border-box; padding: 0px 5px;">51</li><li style="box-sizing: border-box; padding: 0px 5px;">52</li><li style="box-sizing: border-box; padding: 0px 5px;">53</li><li style="box-sizing: border-box; padding: 0px 5px;">54</li><li style="box-sizing: border-box; padding: 0px 5px;">55</li><li style="box-sizing: border-box; padding: 0px 5px;">56</li><li style="box-sizing: border-box; padding: 0px 5px;">57</li><li style="box-sizing: border-box; padding: 0px 5px;">58</li><li style="box-sizing: border-box; padding: 0px 5px;">59</li><li style="box-sizing: border-box; padding: 0px 5px;">60</li><li style="box-sizing: border-box; padding: 0px 5px;">61</li></ul>
程序输出：
<code class="hljs asciidoc has-numbering" style="display: block; padding: 0px; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background-image: initial; background-attachment: initial; background-color: transparent; background-size: initial; background-origin: initial; background-clip: initial; background-position: initial; background-repeat: initial;">the constructor begins
HOME = /root
Tome White is 30 years old
<span class="hljs-header" style="box-sizing: border-box;">still in the constructor
---------------------------------</span>
30
40
<span class="hljs-header" style="box-sizing: border-box;">male
---------------------------------</span>
30
40
<span class="hljs-header" style="box-sizing: border-box;">male
---------------------------------</span>
30</code>
