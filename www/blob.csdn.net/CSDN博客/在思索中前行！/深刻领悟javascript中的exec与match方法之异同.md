# 深刻领悟javascript中的exec与match方法之异同 - 在思索中前行！ - CSDN博客





2014年11月27日 20:56:27[_Tham](https://me.csdn.net/txl16211)阅读数：838








阅读本文之前，请先看下面一道题：

题目17：Read the following javascript code: 


var someText="web2.0 .net2.0";

var pattern=/(\w+)(\d)\.(\d)/g;

var outCome_exec=pattern.exec(someText);

var outCome_matc=someText.match(pattern);


What is outCome_exec[1] and outCome_matc[1]? 


Choice A: true 

Choice B: false 

Choice C: null 

Choice D: Web 

Choice E: Web2.0 

Choice F: undefined

Choice G: net2.0


这道题据说是某IT公司的一道笔试题，也是引起我写今天这篇文章的原因，不过题目我稍微修改了一下，如果这道题你答对了，你可以不往下面看了。

-----------------------------------------------------------------

　　javascript中与正则表达式有关的匹配字符串的函数主要有RegExp类的方法exec(string)以及String类的方法match(regex)，当然还有一些其他的方法，这里不作讨论，但是可能不少程序员都会混淆exec和match，这里列举二者的重点特性：
- exec是正则表达式的方法，而不是字符串的方法，它的参数才是字符串，如下所示：

var re=new RegExp(/\d/);

re.exec( "abc4def" );

或者使用perl风格：

/\d/.exec( "abc4def" );

match才是字符串类提供的方法，它的参数是正则表达式对象，如下用法是正确的：

"abc4def".match(\d);

- **exec和match返回的都是数组**如果执行exec方法的正则表达式没有分组（没有括号括起来的内容），那么如果有匹配，他将返回一个只有一个元素的数组，这个数组唯一的元素就是该正则表达式匹配的第一个串;如果没有匹配则返回null。


下面两个alert函数弹出的信息是一样的：

var str= "cat,hat" ;
var p=/at/; //没有g属性
alert(p.exec(str))

alert(str.match(p))

都是"at"。在这种场合下exec等价于match。

但是如果正则表达式是全局匹配（g属性）的，那么以上代码结果不一样了:
var str= "cat,hat" ;
var p=/at/g; //注意g属性
alert(p.exec(str))

alert(str.match(p))

分别是

"at"

"at,at"。

因为exec永远只返回第一个匹配，而match在正则指定了g属性的时候，会返回所有匹配。

- exec如果找到了匹配，而且包含分组的话，返回的数组将包含多个元素，第一个元素是找到的匹配，之后的元素依次为该匹配中的第一、第二...个分组（反向引用）


如下的代码将弹出"cat2,at":
var str= "cat2,hat8" ;
var p=/c(at)\d/;

alert(p.exec(str))


其中第一个元素是匹配的字符串"cat2"，之后的元素是括号中匹配的"at"。

- match函数在满足如下条件下将越俎代庖，实现和exec一样的功能：


1、正则表达式中含有分组（括号）

2、返回唯一的匹配


且看如下的代码：
var str= "cat2,hat8" ;
var p=/c(at)\d/;

alert(p.exec(str))

alert(str.match(p))


都将弹出消息"cat2,at"，是不是觉得很奇怪呢？

现在我们再来回顾文章开头提出的问题：

var someText= "web2.0 .net2.0" ;
var pattern=/(\w+)(\d)\.(\d)/g;
var outCome_exec=pattern.exec(someText);
var outCome_matc=someText.match(pattern);


分析：

outCome_exec的值：pattern中的g属性对exec函数是没有任何作用的，因此exec将匹配第一个可以匹配的字串“web2.0”，作为其返回数组的第一个元素，另外由于pattern中包含三个分组((\w+)、(\d)、(\d))，因此该数组还将包含三个元素，依次是“web”、“2”、“0”，所以该exec执行后的最终结果是:["web2.0","web","2","0"]

outCome_matc的值：由于pattern是全局匹配的，因此match匹配了所有可以匹配的字串，因此结果数组的值outCome_matc为["web2.0","net2.0"]。如果pattern没有g属性，那么它将与outCome_exec结果一样，因为符合本文第4小节所描述的条件：有分组且返回唯一匹配！

总结：

match是返回所有匹配的字符串合成的数组，但是正则表达式必须指定全局g属性才能返回所有匹配，不指定g属性则会返回一个只有一个元素的数组。

exec永远返回与第一个匹配相关的信息，其返回数组包括第一个匹配的字串，所有分组的反向引用。

-------------------------------------------

某些情况下exec返回的结果和match返回的结果一样:
var str= "cat,hat" ;
var p=/at/; //没有g属性
alert(p.exec(str))

alert(str.match(p))


都弹出“at”

-------------------------------------------

某些情况下match返回的结果和exec返回的结果一样:
var str= "cat2,hat8" ;
var p=/c(at)\d/;

alert(p.exec(str))

alert(str.match(p))


都弹出“cat2,at”

[json2form](http://www.ewikisoft.com/?ref=cnblogs)，业界称奇的表单开发框架，极大提高您开发信息系统的效率。[查看演示](http://www.ewikisoft.com/json2form/demo.php?ref=cnblogs)




