# jquery源码阅读知识储备(5)call方法和apply方法接触 - z69183787的专栏 - CSDN博客
2013年10月24日 15:31:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1422
**call方法:**
语法：call([thisObj[,arg1[, arg2[,   [,.argN]]]]]) 
定义：调用一个对象的一个方法，以另一个对象替换当前对象。
说明：
call 方法可以用来代替另一个对象调用一个方法。call 方法可将一个函数的对象上下文从初始的上下文改变为由 thisObj 指定的新对象。
如果没有提供 thisObj 参数，那么 Global 对象被用作 thisObj。 
**apply****方法：**
语法：apply([thisObj[,argArray]]) 
定义：应用某一对象的一个方法，用另一个对象替换当前对象。
说明：
如果 argArray 不是一个有效的数组或者不是 arguments 对象，那么将导致一个 TypeError。
如果没有提供 argArray 和 thisObj 任何一个参数，那么 Global 对象将被用作 thisObj， 并且无法被传递任何参数。
**[javascript]**[view
 plain](http://blog.csdn.net/hackerhope/article/details/6174895#)[copy](http://blog.csdn.net/hackerhope/article/details/6174895#)
- 
- <script language="javascript"><!--  
- 
- /**定义一个animal类*/
- function Animal(){     
- this.name = "Animal";     
- this.showName = function(){     
-         alert(this.name);     
-     }     
- }     
- /**定义一个Cat类*/
- function Cat(){     
- this.name = "Cat";     
- }     
- 
- /**创建两个类对象*/
- var animal = new Animal();     
- var cat = new Cat();     
- 
- //通过call或apply方法，将原本属于Animal对象的showName()方法交给当前对象cat来使用了。   
- //输入结果为"Cat"   
- animal.showName.call(cat,",");     
- //animal.showName.apply(cat,[]);   
- 
- 
- 
- / --></script>   
**以上代码无论是采用animal.showName.call或是animal.showName.apply方法，运行的结果都是输出一个"Cat"的字符串。说明showName方法的调用者被换成了cat对象，而不是最初定义它的animal了。这就是call和apply方法的妙用！**
