# javascript的prototype继承 - z69183787的专栏 - CSDN博客
2018年02月25日 13:07:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：114
个人分类：[Javascript-概念原理](https://blog.csdn.net/z69183787/article/category/6664411)
基本的用法 把ClassA的一个实例赋值给ClassB ClassB就继承了ClassA的所有属性
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<script>
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassA()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.a='a';
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassB()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.b='b';
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)ClassB.prototype=new ClassA();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)var objB=new ClassB();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)for(var p in objB)document.write(p+"<br>");
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</script>
从原型继承理论的角度去考虑 js的原型继承是引用原型 不是复制原型
所以 修改原型会导致所有B的实例的变化
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<script>
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassA()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.a='a';
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassB()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.b='b';
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)ClassB.prototype=new ClassA();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)var objB=new ClassB();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)alert(objB.a);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)ClassB.prototype.a='changed!!';
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)alert(objB.a);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</script>
然而 子类对象的写操作只访问子类对象中成员 它们之间不会互相影响
因此 写是写子类 读是读原型（如果子类中没有的话）
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<script>
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassA()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.a='a';
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassB()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.b='b';
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)ClassB.prototype=new ClassA();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)var objB1=new ClassB();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)var objB2=new ClassB();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)objB1.a='!!!';
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)alert(objB1.a);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)alert(objB2.a);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</script>
每个子类对象都执有同一个原型的引用 所以子类对象中的原型成员（引用类型或方法，不包括基础值类型的成员变量）实际是同一个
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<script>
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassA()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)this.a=function(){alert();};
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassB()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)this.b=function(){alert();};
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)ClassB.prototype=new ClassA();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)var objB1=new ClassB();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)var objB2=new ClassB();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)alert(objB1.a==objB2.a);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)alert(objB1.b==objB2.b);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</script>
构造子类时 原型的构造函数不会被执行
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<script>
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassA()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    alert("a");
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)this.a=function(){alert();};
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassB()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    alert("b");
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)this.b=function(){alert();};
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)ClassB.prototype=new ClassA();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)var objB1=new ClassB();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)var objB2=new ClassB();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</script>
接下来是致命的，在子类对象中访问原型的成员对象：
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<script>
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassA()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.a=[];
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)function ClassB()
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)this.b=function(){alert();};
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)ClassB.prototype=new ClassA();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)var objB1=new ClassB();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)var objB2=new ClassB();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)objB1.a.push(1,2,3);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)alert(objB2.a);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)//所有b的实例中的a成员全都变了！！
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</script>
所以 在prototype继承中 原型类中不能有成员对象！ 所有成员必须是值类型数据（string也可以）
用prototype继承有执行效率高，不会浪费内存，为父类动态添置方法后子类中马上可见等的优点。
我就非常喜欢用prototype继承。
prototype继承是通过把子类的原型对象（prototype）设置成父类的一个实例来进行继承的。
只简单的这样设置继承的确如楼主所说，有不少缺点。总的来说有四个缺点：
　　缺点一：父类的构造函数不是像JAVA中那样在给子类进行实例化时执行的，而是在设置继承的时候执行的，并且只执行一次。这往往不是我们希望的，特别是父类的构造函数中有一些特殊操作的情况下。
　　缺点二：由于父类的构造函数不是在子类进行实例化时执行，在父类的构造函数中设置的成员变量到了子类中就成了所有实例对象公有的公共变量。由于JavaScript中继承只发生在“获取”属性的值时，对于属性的值是String，Number和Boolean这些数据本身不能被修改的类型时没有什么影响。但是Array和Object类型就会有问题。
　　缺点三：如果父类的构造函数需要参数，我们就没有办法了。
　　缺点四：子类原本的原型对象被替换了，子类本身的constructor属性就没有了。在类的实例取它的constructor属性时，取得的是从父类中继承的constructor属性，从而constructor的值是父类而不是子类。
我也曾经为了这四个缺点头疼过，于是对prototype继承进行改造。
我试了几种方法，下面是我觉得最好的一种。我把它写成Function对象的一个方法，这样用的时候方便。方法如下：
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)//类的继承-海浪版
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)Function.prototype.Extends =function (parentClass)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)var Bs =new Function();
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)  Bs.prototype = parentClass.prototype;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.prototype =new Bs();
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.prototype.Super = parentClass;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.prototype.constructor =this;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
