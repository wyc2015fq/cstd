# js入门介绍 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年02月01日 10:51:46[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：339
为什么起名叫JavaScript？原因是当时Java语言非常红火，所以网景公司希望借Java的名气来推广，但事实上JavaScript除了语法上有点像Java，其他部分基本上没啥关系。
为了让JavaScript成为全球标准，几个公司联合ECMA（European Computer Manufacturers Association）组织定制了JavaScript语言的标准，被称为ECMAScript标准。
ECMAScript是一种语言标准，而JavaScript是网景公司对ECMAScript标准的一种实现。
如果你遇到ECMAScript这个词，简单把它替换为JavaScript就行了。
<script
type="text/javascript">
 ...</script>
但这是没有必要的，因为默认的type就是JavaScript，所以不必显式地把type指定为JavaScript。
微软出的[Visual
 Studio Code](https://code.visualstudio.com/)，可以看做迷你版Visual Studio，免费！跨平台！内置JavaScript支持，强烈推荐使用！
chrome的“控制台(Console)“，在这个面板里可以直接输入JavaScript代码，按回车后执行。
如果你对自己还有更高的要求，可以研究开发者工具的“源码(Sources)”，掌握断点、单步执行等高级调试技巧。
JavaScript严格区分大小写，如果弄错了大小写，程序将报错或者运行不正常。
Number
JavaScript不区分整数和浮点数，统一用Number表示
比较
7
 == 7;
//**true**
**false**
 == 0;
//**true****false**
 === 0;
//**false**
第一种是==比较，它会自动转换数据类型再比较，很多时候，会得到非常诡异的结果；
第二种是===比较，它不会自动转换数据类型，如果数据类型不一致，返回false，如果一致，再比较。
不要使用==比较，始终坚持使用===比较。
另一个例外是NaN这个特殊的Number与所有其他值都不相等，包括它自己：
NaN === NaN;
*// false*
唯一能判断NaN的方法是通过isNaN()函数：
isNaN(NaN);
//**true**
要比较两个浮点数是否相等，只能计算它们之差的绝对值，看是否小于某个阈值：
Math.abs(1
 / 3 - (1
 - 2 /
3)) <
0.0000001;
//**true**
null和undefined
null表示一个“空”的值，它和0以及空字符串''不同，0是一个数值，''表示长度为0的字符串，而null表示“空”。
在其他语言中，也有类似JavaScript的null的表示，例如Java也用null，Swift用nil，Python用None表示。
但是，在JavaScript中，还有一个和null类似的undefined，它表示“未定义”。
JavaScript的设计者希望用null表示一个空的值，而undefined表示值未定义。事实证明，这并没有什么卵用，区分两者的意义不大。大多数情况下，我们都应该用null。undefined仅仅在判断函数参数是否传递的情况下有用。
JavaScript的数组可以包括任意数据类型。
[1,
2,
3.14,
'Hello', null, true];
对象是一组由键-值组成的无序集合，例如：
**var**
 person = { name: 'Bob', age:
20, tags: ['js',
'web',
'mobile'], city:
'Beijing', hasCar: true, zipcode:
 null};
JavaScript对象的键都是字符串类型，值可以是任意数据类型。上述person对象一共定义了6个键值对，其中每个键又称为对象的属性，例如，person的name属性为'Bob'。
要获取一个对象的属性，我们用对象变量.属性名的方式：
person.name;
//'Bob'
**var**
 a; *// 申明了变量a，此时a的值为undefined*
**var**
 t = **null**;
*// t的值是null*
**var**
 a = 123;
*// a的值是整数123*a =
'ABC';
*// a变为字符串*
使用console.log()代替alert()的好处是可以避免弹出烦人的对话框。
如果一个变量没有通过var申明就被使用，那么该变量就自动被申明为全局变量：
i =
10; // i现在是全局变量
在同一个页面的不同的JavaScript文件中，如果都不用var申明，恰好都使用了变量i，将造成变量i互相影响，产生难以调试的错误结果。

