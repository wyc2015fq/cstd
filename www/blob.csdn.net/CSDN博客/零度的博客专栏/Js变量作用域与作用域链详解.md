# Js变量作用域与作用域链详解 - 零度的博客专栏 - CSDN博客
2017年07月13日 10:36:34[零度anngle](https://me.csdn.net/zmx729618)阅读数：264标签：[javascript](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[Javascript](https://blog.csdn.net/zmx729618/article/category/6156941)
### **一、变量的类型 **
　　Javascript和Java、C这些语言不同，它是一种无类型、弱检测的语言。它对变量的定义并不需要声明变量类型，我们只要通过赋值的形式，可以将各种类型的数据赋值给同一个变量。例如，代码如下: 
```java
i=100;//Number类型 
i="variable";//String类型 
i={x:4};//Object类型 
i=[1,2,3];//Array类型
```
　　JS的这种特性虽然让我们的编码更加灵活，但也带来了一个弊端，不利于Debug，编译器的弱检测让我们维护冗长的代码时相当痛苦。 
### **二、变量的声明 **
　　JS中变量申明分显式申明和隐式申明。 
　　var i=100;//显式申明 
　　i=100;//隐式申明 
　　在函数中使用var关键字进行显式申明的变量是做为局部变量，而没有用var关键字，使用直接赋值方式声明的是全局变量。　　 
　　当我们使用访问一个没有声明的变量时，JS会报错。而当我们给一个没有声明的变量赋值时，JS不会报错，相反它会认为我们是要隐式申明一个全局变量，这一点一定要注意。
### **三、全局变量和局部变量 **
　　当JS解析器执行时，首先就会在执行环境里构建一个全局对象，我们定义的全局属性就是做为该对象的属性读取，在顶层代码中我们使用this关键字和window对象都可以访问到它。而函数体中的局部变量只在函数执行时生成的调用对象中存在，函数执行完毕时局部变量即刻销毁。因此在程序设计中我们需要考虑如何合理声明变量，这样既减小了不必要的内存开销，同时能很大程度地避免变量重复定义而覆盖先前定义的变量所造成的Debug麻烦。
### 四、函数作用域
先看一小段代码：
```java
var scope="global";
function t(){
    console.log(scope);
    var scope="local"
    console.log(scope);
}
t();
```
(PS: console.log()是firebug提供的调试工具，很好用，有兴趣的童鞋可以用下，比浏览器+alert好用多了）
第一句输出的是： "undefined"，而不是 "global"
第二讲输出的是："local"
你可能会认为第一句会输出："global",因为代码还没执行var scope="local",所以肯定会输出“global"。
我说这想法完全没错，只不过用错了对象。我们首先要区分Javascript的函数作用域与我们熟知的C/C++等的块级作用域。
在C/C++中，花括号内中的每一段代码都具有各自的作用域，而且变量在声明它们的代码段之外是不可见的。而Javascript压根没有块级作用域，而是函数作用域.
所谓函数作用域就是说：-》变量在声明它们的函数体以及这个函数体嵌套的任意函数体内都是有定义的。
所以根据函数作用域的意思，可以将上述代码重写如下：
```java
var scope="global";
function t(){
    var scope;
    console.log(scope);
    scope="local"
    console.log(scope);
}
t();
```
```java
var scope="global";
function t(){
    var scope;
    console.log(scope);
    scope="local"
    console.log(scope);
}
t();
```
我们可以看到，由于函数作用域的特性，局部变量在整个函数体始终是由定义的，我们可以将变量声明”提前“到函数体顶部，同时变量初始化还在原来位置。
为什么说Js没有块级作用域呢，有以下代码为证：
```java
var name="global";
if(true){
    var name="local";
    console.log(name)
}
console.log(name);
```
```java
var name="global";
if(true){
    var name="local";
    console.log(name)
}
console.log(name);
```
都输出是“local",如果有块级作用域，明显if语句将创建局部变量name,并不会修改全局name,可是没有这样，所以Js没有块级作用域。
现在很好理解为什么会得出那样的结果了。scope声明覆盖了全局的scope,但是还没有赋值，所以输出：”undefined“。
所以下面的代码也就很好理解了。
```java
function t(flag){
    if(flag){
        var s="ifscope";
        for(var i=0;i<2;i++) 
            ;
    }
    console.log(i);
    console.log(s);
}
t(true);
```
输出：2  ”ifscope"
### 五、变量作用域
还是首先看一段代码：
```java
function t(flag){
    if(flag){
        s="ifscope";
        for(var i=0;i<2;i++) 
            ;
    }
    console.log(i);
}
t(true);
console.log(s);
```
就是上面的翻版，知识将声明s中的var去掉。
程序会报错还是输出“ifscope"呢？
让我揭开谜底吧，会输出：”ifscope"
这主要是Js中没有用var声明的变量都是全局变量，而且是顶层对象的属性。
所以你用console.log(window.s)也是会输出“ifconfig"
当使用var声明一个变量时，创建的这个属性是不可配置的，也就是说无法通过delete运算符删除
var name=1    ->不可删除
sex=”girl“         ->可删除
this.age=22    ->可删除
再看如下代码:
```java
<script language ="javascript" type ="text/javascript" > 
var a = "change"; 
function fun() { 
alert(a);//输出undefined 
var a = "改变了"; 
alert(a);//输出改变了 
} 
alert(a);//输出change 
fun(); 
</script>
```
var定义的是一个作用域上的变量，在第一次输出a之前，JS在预编译分析中已经将a赋值为change，所以第一次输出change，当调用到fun()函数的时候，JS创建一个新的作用域，在输出a之前，初始化所有var变量的值为undefined，所以fun()中第一次输出的是undefined，第二次输出已经给a赋值了，所以输出新的值；两个a在函数里面和外面是不同的两个变量。
再如下代码:
```java
<script language ="javascript" type ="text/javascript" > 
var b; 
function fun() { 
b = "change"; 
} 
alert(b);//输出undefined 
</script>
```
变量b在函数外面已经定义了，在函数中有给b赋值，但外部输出的却是undefined。
### 六、作用域链
先来看一段代码：
```java
name="lwy";
function t(){
    var name="tlwy";
    function s(){
        var name="slwy";
        console.log(name);
    }
    function ss(){
        console.log(name);
    }
    s();
    ss();
}
t();
```
当执行s时，将创建函数s的执行环境(调用对象),并将该对象置于链表开头，然后将函数t的调用对象链接在之后，最后是全局对象。然后从链表开头寻找变量name,很明显
name是"slwy"。
但执行ss()时，作用域链是： ss()->t()->window,所以name是”tlwy"
下面看一个很容易犯错的例子：
```java
<html>
<head>
<script type="text/javascript">
function buttonInit(){
	for(var i=1;i<4;i++){
		var b=document.getElementById("button"+i);
		b.addEventListener("click",function(){ alert("Button"+i);},false);
	}
}
window.onload=buttonInit;
</script>
</head>
<body>
<button id="button1">Button1</button>
<button id="button2">Button2</button>
<button id="button3">Button3</button>
</body>
</html>
```
```
<html>
<head>
<script type="text/javascript">
function buttonInit(){
	for(var i=1;i<4;i++){
		var b=document.getElementById("button"+i);
		b.addEventListener("click",function(){ alert("Button"+i);},false);
	}
}
window.onload=buttonInit;
</script>
</head>
<body>
<button id="button1">Button1</button>
<button id="button2">Button2</button>
<button id="button3">Button3</button>
</body>
</html>
```
当文档加载完毕，给几个按钮注册点击事件，当我们点击按钮时，会弹出什么提示框呢？
很容易犯错，对是的，三个按钮都是弹出："Button4",你答对了吗？
当注册事件结束后，i的值为4，当点击按钮时，事件函数即function(){ alert("Button"+i);}这个匿名函数中没有i,根据作用域链，所以到buttonInit函数中找，此时i的值为4，
所以弹出”button4“。
### 七、with语句
说到作用域链，不得不说with语句。with语句主要用来临时扩展作用域链，将语句中的对象添加到作用域的头部。
看下面代码：
```java
person={name:"yhb",age:22,height:175,wife:{name:"lwy",age:21}};
with(person.wife){
    console.log(name);
}
```
with语句将person.wife添加到当前作用域链的头部，所以输出的就是：“lwy"。with语句结束后，作用域链恢复正常。
### 八、基本类型和引用类型 
　　JS不同于JAVA、C这些语言，在变量申明时并不需要声明变量的存储空间。变量中所存储的数据可以分为两类：基本类型和引用类型。其中数值、布尔值、null和undefined属于基本类型，对象、数组和函数属于引用类型。
　　基本类型在内存中具有固定的内存大小。例如：数值型在内存中占有八个字节，布尔值只占有一个字节。对于引用型数据，他们可以具有任意长度，因此他们的内存大小是不定的，因此变量中存储的实际上是对此数据的引用，通常是内存地址或者指针，通过它们我们可以找到这个数据。
　　引用类型和基本类型在使用行为上也有不同之处： 
代码如下:
```java
<script language="JavaScript" type="text/javascript"> 
//定义一个输出函数 
function outPut(s){ 
document.writeln(s) 
} 
var a = 3; 
var b = a; 
outPut(b); 
//3 
a = 4; 
outPut(a); 
//4 
outPut(b); 
//3 
</script>
```
　　对基本类型b进行赋值时，实际上是又开辟了一块内存空间，因此改变变量a的值对变量b没有任何影响。 
代码如下:
```java
<script language="JavaScript" type="text/javascript"> 
//定义一个输出函数 
function outPut(s){ 
document.writeln(s) 
} 
var a_array = [1,2,3]; 
var b_array = a_array; 
outPut(b_array); //1,2,3 
a_array[3] = 4; 
outPut(b_array);//1,2,3,4 
</script>
```
        上面是对引用类型的变量赋值，实际上他们传递的是对内存地址的引用，因此对a_array和b_array的存取，实际上都是操作的同一块内存区域。如果希望重新分配内存空间存储引用型变量，那么我就需要使用克隆方法或者自定义方法来复制引用变量的数据。

