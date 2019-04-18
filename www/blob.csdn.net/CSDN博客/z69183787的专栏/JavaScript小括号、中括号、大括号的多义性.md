# JavaScript小括号、中括号、大括号的多义性 - z69183787的专栏 - CSDN博客
2017年03月02日 18:15:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：596
**可根据条件判断 是否展示某标签的 自身属性**
**<Checkbox {...c}  key="zs"/ >**
**![](https://img-blog.csdn.net/20170302181434619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**可设置1为checkbox标签的属性，且值为1**
**小括号**
[JavaScript](http://lib.csdn.net/base/javascript)中小括号有五种语义：
语义1，函数声明时参数表
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- func(arg1,arg2){  
- // ...
- }  
语义2，和一些语句联合使用以达到某些限定作用
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- // 和for in一起使用
- for(var a in obj){  
- // ...
- }  
- // 和if一起使用
- if(boo){  
- //...
- }  
- // 和while一起使用
- while(boo){  
- // ...
- }  
- // 和do while一起使用do{
- // ...
- }  
- while(boo)  
注意：在与if、while及dowhile一起使用时小括号会将其中的表达式结果隐式的转换成布尔值。见无处不在的隐式类型转换。
语义3，和new一起使用用来传值(实参)
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- // 假设已经定义了类Person，它有两个字段姓名（name），年龄（age）
- var p1=new Person('Jack',26);  
语义4，作为函数或对象方法的调用运算符(如果定义了参数也可与语义3一样传实参)
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- // 假设已经定义了函数func
- func();  
- // 假设已经定义了对象obj，且拥有func方法
- obj.func();  
这里提下typeof运算符，有人喜欢这么使用
请注意typeof后的小括号并非语义4(即不是函数调用)，而是后面提到的语义5。我使用typeof一般不加后面的小括号。见具名函数的多种调用方式
语义5，强制表达式运算
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- function strToJson(str){ // eval 中字符串两旁加了强制运算符()
- var json= eval('(' + str + ')');   
- return json;  
- }  
关于语义5，大家最熟悉的莫过于使用eval解析JSON
又如使用较多的是匿名函数自执行
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- (function(){  
- // ...
- })();  
注意，以上代码第1对小括号是语义5，第3对则是语义4。
大括号
JavaScript中大括号有四种语义作用：
语义1，组织复合语句,这是最常见的
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- if( condition ) {  
- //...
- }else {  
- //...
- }for() {  
- //...
- }  
语义2，对象直接量声明
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- var obj= { name : 'jack', age : 23};  
整个是个赋值语句，其中的{name:'jack',age:23}是个表达式。
语义3，声明函数或函数直接量
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- function f1(){  
- //...
- }  
- var f2 = function(){  
- //...
- }  
f1与非f2的区别是前者在语法解释期，后者在运行期。区别在于：如果调用该函数的代码在函数定义之后，则没有区别;如果调用该函数的代码在函数定义之前，则f1仍然可以调用，f2则会报错，提示f2未定义。
语义4，结构化异常处理的语法符号
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- try {  
- //...
- }catch( ex ){  
- //...
- }finally{  
- //...
- }  
这里的大括号与符合语句(语义1)是有区别的，大括号中如果只有一条语句，在if/else/for等中大括号是可以省略的，但try/catch/finally则不能省略。以下代码纠结了偶N久：
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- function(){}() //匿名函数立即执行，语法分析期报
- {}.constructor //获取对象直接量的构造器，语法分析期报错
令人不解的是为何[].constructor这么写却不报错呢，一个是想获取对象直接量的构造器，一个是获取数组直接量的构造器而已。
当然添加个变量接收也不会报错，同样的情况如：
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- var fn= function(){}()，也不会报错。  
实际上是js的“语句优先”在作怪，即{}被理解成复合语句块(语义1)而不是对象直接量(语义2)或声明函数(语义3)的语义。
function(){}()，大括号被理解成复合语句，自然前面的function()声明函数的语法不完整导致语法分析期出错。
{}.constructor，大括号被理解成复合语句，大括号后面是点运算符，点运算符前没有合理的对象自然也报错。
修复方式众所周知，加个强制运算符()：
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- (function(){})()，(function(){});//强制其理解为函数(语义3)，“函数()”表示执行该函数，即声明后立即执行了。
- 
- ({}).constructor //({})强制把大括号理解成对象直接量(语义2)，“对象.xx”表示获取对象的成员，自然后面的点运算符可以正常执行了。
中括号
JavaScript中括号有四种语义：
语义1，声明数组
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- var ary= []; // 声明一个空数组
- var ary= [1,3]; // 声明一个数组，同时赋初值
语义2，取数组成员
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- var ary= [1,2,3];  
- var item= ary[0];  
语义3，定义对象成员(可以不遵循标识符规则)
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- var obj= {};  
- // 为obj添加一个属性name，name是合法的标识符，即也可以通过obj.name方式来定义
- obj['name'] = 'jack';   
- //为obj添加一个属性2a，2a不是合法的标识符（不能以数字开头），不能通过obj.2a来定义
- obj['2a'] = 'test';   
语义4，取对象成员
**[javascript]**[view
 plain](http://blog.csdn.net/zyz511919766/article/details/7258808#)[copy](http://blog.csdn.net/zyz511919766/article/details/7258808#)
- var obj= {name:'jack'};obj['2a'] = 'test';  
- obj['name']; // -->jack
- obj['2a']; // --> test (不能通过obj.2a获取)
====================================篇2======================================================
一、{ } 大括号，表示定义一个对象，大部分情况下要有成对的属性和值，或是函数。
如：var LangShen = {"Name":"Langshen","AGE":"28"};
上面声明了一个名为“LangShen”的对象，多个属性或函数用,（逗号）隔开，因为是对象的属性，
所以访问时，应该用.（点）来层层访问：LangShen.Name、LangShen.AGE，当然我们也可以用数组的方式来访问，如：LangShen["Name"]、LangShen["AGE"]，结果是一样的。
该写法，在JSON[数据结构](http://lib.csdn.net/base/datastructure)中经常用，除此之外，我们平时写函数组的时候，也经常用到，如：
var LangShen = {
Name = function(){
return "LangShen";
},
Age = function(){
return "28";
}
}
调用方式差不多，因为是函数组，所以要加上()，如：alert( LangShen.Name() );
二、[ ]中括号，表示一个数组，也可以理解为一个数组对象。
如：var LangShen = [ "Name","LangShen","AGE","28" ];
很明显，每个值或函数，都是独立的，多个值之间只用,（逗号）隔开，因为是数组对象，所以它等于：
var LangShen = Array( "Name","LangShen","AGE","28" );
访问时，也是和数组一样，alert( LangShen[0] );
三、{ } 和[ ] 一起使用，我们前面说到，{ } 是一个对象，[ ] 是一个数组，我们可以组成一个对象数组，如：
var LangShen = { "Name":"Langshen",
"MyWife":[ "LuLu","26" ],
"MySon":[{"Name":"Son1"},{"Name":"Son2"},{"Name":"Son3"}]
}
从上面的结构来看，是一个对象里面的第一项是个属性，第二项是一个数组，第三个是包含有多个对象的数组。调用起来，也是一层一层访问，对象的属性用.（点）叠加，数组用[下标] 来访问。
如：alert( LangShen.MySon[1].Name ) ;
============篇3==================================================
**一、大括号｛｝表示对象：**javascript供了另外一种简单的方式来创建对象，即大括号（{}）语法：
arr = {
a:5, //对象属性，a是变量名
b:8,
c:function(){return this.a + this.b;}, //对象方法
d:['a':1,'b':2] //对象属性，数组
}
通过大括号括住多个属性或方法及其定义（这些属性或方法用逗号隔开），来实现对象的定义，这段代码就直接定义个了具有n个属性或方法的对象，其中属性名和其定义之间用冒号（:）隔开。
document.write(arr.c());
注意，各属性间用逗号（,）隔开。使用这种方式来定义对象，还可以使用字符串作为属性（方法）名，例如：
var obj={“001”:”abc”}
因为这种简单对象没有通过构造函数创造所以也就没能在对象外添加方法。
**对比构造函数创造的对象**：
function Test(a,b){
this.a = a;
this.b = b;
this.c = function(){return this.a + this.b;}
}
Test.prototype.d = function another(){
return this.a * this.b * Math.PI;
};
arr = new Test(5,8);
document.write(arr.c());
输出：13
document.write(arr.d());
输出：125.66370614359172
**二、中括号 [ ] 表示数组：**
arr = [
[1,2], //数组
['a','b'],
[{c:'a1',d:'b1'},{e:'a2',f:'b2'}] //对象
];
for(key in arr){
for(chikey in arr[key]){
document.write(chikey + "=>" + arr[key][chikey] +"<br />");
}
}
输出：
0=>1
1=>2
0=>a
1=>b
0=>[object Object]
1=>[object Object]
document.write(arr[2][1].e);
输出：a2
这里，数组的第三组是未命名的对象，可以用arr[2][1].e的方式指定访问属性。当然也可以遍历：
for(key in arr[2]){
for(val in arr[2][key]){
document.write(val + "=>" + arr[2][key][val] +"<br />");
}
}
输出：
c=>a1
d=>b1
e=>a2
f=>b2
**三、数组与对象**
在js中，上述的对象与数组的创建很相似，有人称第一种方式为js的关联数组，姑且吧；但在中括号中就不能使用['a':'b']的形式，只能以索引数组的形式，即下标只能是数字，如[1,2]或['a','b']，这点须注意。
