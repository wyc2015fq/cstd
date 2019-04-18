# js的Prototype属性 解释及常用方法 - z69183787的专栏 - CSDN博客
2013年11月23日 18:48:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1728
**函数：原型**
每一个构造函数都有一个属性叫做原型(prototype,下面都不再翻译，使用其原文)。这个属性非常有用：为一个特定类声明通用的变量或者函数。
***prototype的定义***
**Javascript中对象的prototype属性的解释是:返回对象类型原型的引用。**
你不需要显式地声明一个prototype属性，因为在每一个构造函数中都有它的存在。你可以看看下面的例子：
**Example PT1**
**CODE:**
function Test()
{
}
alert(Test.prototype); // 输出 "Object"
***给prototype添加属性***
就如你在上面所看到的，prototype是一个对象，因此，你能够给它添加属性。你添加给prototype的属性将会成为使用这个构造函数创建的对象的通用属性。
例如，我下面有一个数据类型Fish，我想让所有的鱼都有这些属性：livesIn="water"和price=20；为了实现这个，我可以给构造函数Fish的prototype添加那些属性。
**Example PT2**
**CODE:**
function Fish(name, color)
{
this.name=name;
this.color=color;
}
Fish.prototype.livesIn="water";
Fish.prototype.price=20;
接下来让我们作几条鱼：
**CODE:**
var fish1=new Fish("mackarel", "gray");
var fish2=new Fish("goldfish", "orange");
var fish3=new Fish("salmon", "white");
再来看看鱼都有哪些属性：
**CODE:**
for (int i=1; i<=3; i++)
{
var fish=eval_r("fish"+i);   // 我只是取得指向这条鱼的指针
alert(fish.name+","+fish.color+","+fish.livesIn+","+fish.price);
}
输出应该是：
**CODE:**
"mackarel, gray, water, 20"
"goldfish, orange, water, 20"
"salmon, white water, 20"
你看到所有的鱼都有属性livesIn和price,我们甚至都没有为每一条不同的鱼特别声明这些属性。这时因为当一个对象被创建时，这个构造函数 将会把它的属性prototype赋给新对象的内部属性__proto__。这个__proto__被这个对象用来查找它的属性。
你也可以通过prototype来给所有对象添加共用的函数。这有一个好处：你不需要每次在构造一个对象的时候创建并初始化这个函数。为了解释这一点，让我们重新来看Example DT9并使用prototype来重写它：
***用prototype给对象添加函数***
**Example PT3**
**CODE:**
function Employee(name, salary)
{
this.name=name;               
this.salary=salary;
}
Employee.prototype.getSalary=function getSalaryFunction()
{
return this.salary;
}
Employee.prototype.addSalary=function addSalaryFunction(addition)
{
this.salary=this.salary+addition;
}
我们可以象通常那样创建对象：
**CODE:**
var boss1=new Employee("Joan", 200000);
var boss2=new Employee("Kim", 100000);
var boss3=new Employee("Sam", 150000);
并验证它：
**CODE:**
alert(boss1.getSalary());   // 输出 200000
alert(boss2.getSalary());   // 输出 100000
alert(boss3.getSalary());   // 输出 150000
这里有一个图示来说明prototype是如何工作的。这个对象的每一个实例(boss1, boss2, boss3)都有一个内部属性叫做__proto__，这个属性指向了它的构造器(Employee)的属性prototype。当你执行 getSalary或者addSalary的时候，这个对象会在它的__proto__找到并执行这个代码。注意这点：这里并没有代码的复制(和 Example DT8的图表作一下对比)。
![js的Prototype属性 <wbr>解释及常用方法](http://www.blueidea.com/articleimg/2006/07/3823/05.gif)
## String.prototype.Trim = function()
利用Javascript中每个对象(Object)的prototype属性我们可以为Javascript中的内置对象添加我们自己的方法和属性。 
以下我们就用这个属性来为String对象添加三个方法：Trim,LTrim,RTrim(作用和VbScript中的同名函数一样) 
CODE:String.prototype.Trim = function() 
{ 
return this.replace(/(^\s*)|(\s*$)/g, ""); 
} 
String.prototype.LTrim = function() 
{ 
return this.replace(/(^\s*)/g, ""); 
} 
String.prototype.Rtrim = function() 
{ 
return this.replace(/(\s*$)/g, ""); 
}
怎么样，简单吧，下面看一个使用的实例：[复制此代码]CODE:
<script language=javascript> 
String.prototype.Trim = function() 
{ 
return this.replace(/(^\s*)|(\s*$)/g, ""); 
} 
var s = " leading and trailing spaces "; 
window.alert(s + " (" + s.length + ")"); 
s = s.Trim(); 
window.alert(s + " (" + s.length + ")"); 
</script>
