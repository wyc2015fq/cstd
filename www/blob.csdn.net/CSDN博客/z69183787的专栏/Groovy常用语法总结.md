# Groovy常用语法总结 - z69183787的专栏 - CSDN博客
2015年01月05日 19:05:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1320
过去半年都在写Groovy，基本都是在实战中学习，从java转过来，也还没什么问题，毕竟基础，架构都没变。
Groovy的强大特性中，Closure，MetaClass都尝试过，Closure基本上就是当方法或者回调接口使用，再高级的用法就没用过了，没有特别体会什么好处，当回调接口就是省的定义接口，简洁了一些；MetaClass就有专门为一个模块做了些MetaClass，和GORM差不多的，给一些类加了save，update，delete之类的方法，不同的是GORM是存到数据库，我做的那模块是调用Restful Service，还不错，如果有需求是不能改变类而要给类加方法，还是很适用的。但是在绝大部分情况下，这些优势都体现不出来，到目前位置，还是对Groovy的一些语法印象深刻些，总结一些常用的语法：
### **1.List，和Map的初始化**
这个用的最多了，原来java的时候，非要新建一个List或者Map，然后把element一个一个加进去，特繁琐。用Groovy在语法上简洁了很多。
定义一个List： 
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- List intList = [1,3,5,7,9]  
- List stringList = ['a', 'b','']  
定义一个Map：
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- Map map = [a:4,b:'d']  
定义Map的时候，对于key，如果像上面的例子一样，没有特别指示，那所有key都是String类型的，key值就是‘a’,'b'，上面的例子等价于
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- Map map = ['a':4,'b':'d']  
某些时候，map的key可能是存在的一个变量，那这个时候，就要用括号把key抱起来，比如
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- String a = 'I am Key'
- Map map = [(a):'I am Value']  
也可以定义空的List或者Map
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- List empty = []  
- Map empty = [:]  
需要知道的是，这种方式新建的List实例是ArrayList，Map实例是LinkedHashMap
### **2.二元运算符 ?:**
java继承了C语言的问号表达式，二元运算符就是更加精简的问号表达式。形式：
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- def result = a ?: b  
意为，如果条件为真，即a为true或者a有值，则result为a，否则result为b
### **3.安全占位符**
这个很有用，可以避免很多NullPointerException，但是也不能滥用了
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- def result = obj?.property  
代码里，obj是个对象，property是对象的一个熟悉，这行代码的意思，如果obj不为null，则会返回property属性的值，如果obj为null，这会直接返回null。语句可以一直串下去
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- def result = a?.b?.c?.d...  
### **4.字段操作**
按照[Groovy Bean的标准](http://groovy.codehaus.org/Groovy+Beans)，默认的时候类里面的所有字段，Groovy都会帮忙生成一个get方法。在类的外部，即便你直接用了属性名而不用get方法去取值，拿到的也是通过get方法拿到的值。如果想直接拿属性值怎么办呢？ 通过字段运算符：
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- class A {  
-    String b  
- }  
- 
- A a = new A()  
- a.b //通过get方法拿值
- a.getB() //通过get方法拿值
- a.@b//直接拿值
### **5.GString**
Java里有String，Groovy里新加了GString。Groovy的语法是，如果用单引号括起来的就是String，如果是双引号括起来的就是GString。所以Groovy不能直接定义原来Java里的char了。
String
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- String s = 'This is a String'
GString
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- GString s = "This is a GString"
两种方式定义出来的实例类型是不一样的。当然GString不是这么简单的，GString的强大之处在于它可以作为模板使用
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- String name = 'count'
- int value1 = 23
- int value2 = 22
- GString s = "The value of $name is ${value1+value2}"
- println s  
最终的输出是： The value of count is 55
倒数第二行，里面有关键字符 $, $即代表去引用上下文中的变量值，而${}中间不仅可以引用变量值，还可以加上一些运算，作为一个表达式
最后一行这是把GString转换成String，再输出。
String是常量，但是GString是变量，准确的说，在GString中的$仅仅是作为一个模板的占位符存在，GString会保存对那个对象的引用，如果引用的对象值发生改变，GString的值也就跟着改变。
需要注意的是，Map里面String和GString就算最终生成的String值一样，但是还是当作两个key存在的。很容易理解，但是很容易犯的错误，比如手误把单引号写成了双引号等等都会引起这个错误。
### **6.构造函数**
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- class ClassA {  
-     String s1  
- int i1  
-     ClassB cb  
- }  
- class ClassB {  
-     String s2  
- }  
- new ClassA(s1:'pro in A', i1:5, cb:[s2:'pro in B'])  
以上代码是可以工作的，Groovy会创建一个ClassA的实例，并把‘pro in A’ 和 5 分别设到属性s1和i1里。即便有一个复杂类型的熟悉cb，也能正常工作，ClassB的对象会被创建出来，并设到属性cb上。ClassB里的s2熟悉当然就是'pro in B'了。
可以看到构造函数里的参数很像Map的定义，确实，其实传入Map也是可以的
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- new ClassA([s1:'pro in A', i1:5, cb:[s2:'pro in B']])  
这个功能，在一些数据模型转换的时候，比较方便。打个比方，前台来的JSON数据，直接全转成Map，再用构造函数new一下，就全部出来了。注意的是，如果Map存在某个类里没有的属性值，会出错误的。
### **7.asType**
用Groovy的是，可能经常看到这样的代码
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- String a = '78'
- int b = a as int
- print b  
第二行，有个关键字as，可以看出来作用就是把String类型的a转成int类型的b。它是怎么工作的呢，很简单，就是把这个操作代理给了String的asType方法，比如String有如下的方法（仅仅是例子，代码中不是这样）
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- class String {  
-     Object asType(Class clz) {  
- if(clz == Integer) {  
- return Integer.parseInt(this)  
-          } elseif(clz == GString) {  
- return"$this"
-          } else {  
- returnsuper.asType(clz)  
-          }  
-     }  
- }  
那么，String就可以用as运算符转换成int或者GString类型
**[java]**[view
 plain](http://blog.csdn.net/yanical/article/details/7163138#)[copy](http://blog.csdn.net/yanical/article/details/7163138#)
- String a = '45'
- int b = a as int
- GString c = a as GString  
上面的 clz == GString 是合法的，在Groovy里等价于 clz == GString.class
### **8. inspect和eval**
eval就和javascript里的eval一样，就是直接执行一段Groovy脚本，可以用Eval.me(...script here...)直接调用。
inspect就是eval的反相操作了，就是把一个对象转成一个合法的可执行的脚本（我的理解，没仔细看过文档）。没试过其他对象，但是像Map，List之类的都可以转出来。这就够了，Map，List转出来的数据之于Groovy就相当于JSON之于JavaScript。在系统内部的时候就可以直接当作数据传输，虽然大家都不推荐这么用，但是我还是坚持，如果数据只包含Map，List，Number，String等简单类型，为什么不用呢？（如果Number里值是NaN或者Infinite的时候有问题，但是很容易解决，看Eval类里的其他两个方法就知道了）。呵呵，我也就这么用着，直到发现了一个Bug（[http://stackoverflow.com/questions/7410252/groovy-inspect-handle-dollar-sign](http://stackoverflow.com/questions/7410252/groovy-inspect-handle-dollar-sign)）,没法解决，我才换掉了实现。
但是我对这个东西还是有很大期待，如果都是内部集成，不存在安全问题，且没有很好JSON支持的时候，这种方式还是很好的方式。
### 9.is方法
按照Groovy的定义，== 操作等于调用equals方法。这样，我们就失去了直接判断两个对象是不是同一对象的运算符。要判断是不是同一个对象，要调用is方法，比如thisObject.is(anotherObject)。这点非常重要，特别是在overwrite对象的equals方法的时候，eclipse自动生成的代码里面有些地方不能直接用==了，而要用is方法。
就这么多吧，以后如果发现更多新东西的时候再加。
发现有人转载我的博客，连个来源链接都没有。版权所有，转载自由，但是【转载请注明来源】！
