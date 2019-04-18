# 体验js之美第八课-面向对象创建和继承终结篇 - weixin_33985507的博客 - CSDN博客
2017年04月29日 19:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
### 概述
到这里我们讲说js面向对象的系列部分的最后一个课程，面向对象必须掌握两个东西一个是对象的创建一个是继承。这节课我们重点说说这两个问题最后我们说下在ES6里面面向对象怎么玩。
### 1对象的创建
我们第一节课已经就会用了，单体模式，你可以管它叫json我也不想多说就是这个。
```
var girlFriend = {
      //属性：
      hair:"long",
      eye:"big",
      hobby:"travel",
      //方法
      cook:function(){},
      wash:function(){}
 };
```
下面我说一种更普遍的方式，这个大家看到的情况更普遍，我们一直说js的对象和传统对象不一样它是基于原型的，那么看看创建对象方式。
**只需要记住一句话，属性放在构造函数里面，方法放在原型上。**
好现在我们重新搞一个妹子，用你熟悉的姿势。
```
//属性放在构造函数里
function Girl (hair,eye,hobby){
    this.hair = hair;
    this.eye = eye;
    this.hobby = hobby;
}
//方法放在原型上
Girl.prototype.cook = function(){};
Girl.prototype.wash = function(){}
var girlFriend = new Girl('long','big','travel');
```
这样不挺好吗，我们都这样用啊。
这里我就说下，new关键字的由来。
```
简单的说new关键字并不该出现在js里，或者说并不是技术原因，仅仅是因为为了争取java那部分用户，所以javascript才叫javascript，然后类这一块又使用了new关键字去符合java程序员的习惯，赢得他们的好感，所以new关键字的出现其实是一个市场策略。但是问题来了，javascript基于原型和java这些语言的类跟js根本不是一个概念new怎么用啊？好，我们上例子说明：
```
```
//属性放在构造函数里
function Girl (hair,eye,hobby){
    this.hair = hair;
    this.eye = eye;
    this.hobby = hobby;
}
//方法放在原型上
Girl.prototype.cook = function(){};
Girl.prototype.wash = function(){}
var girlFriend = new Girl('long','big','travel');
```
那new关键字干嘛了？
简单的说做了三件事：
```
1.执行构造函数（别太担心名词，就当成普通函数就行，一nainai区别都没）
2.在Girl这个构造函数里面，创建一个 空白的Object
3.将 this 指向这个空白的Object
```
这里使用我并不推荐使用new，为什么？
有人说使用new可能出现的问题是把new忘掉了就会返回undefined，比如我道哥（Douglas Crockford's，JSON的创造者，《javascript语言精髓》的作者，多说一句，《javascript语言精髓》初学者不要看，你会只有两个问题，这个画的是啥，这特么画的又是啥），但是个人认为关键原因不是这个，毕竟修复忘掉的new至少有两种方式，一种大写构造函数方法首字母作为提醒，一种是用函数方法，我这里就不展开说了，毕竟不重要。
关键原因是JavaScript是一种prototypical类型语言，在创建之初，是为了迎合市场的需要，让人们觉得它和Java是类似的，才引入了new关键字。Javascript本应通过它的Prototypical特性来实现实例化和继承，但new关键字让它变得不伦不类。
不废话怎么解决，
一种方式是使用jquery的方式，返回一个含有new 对象的函数，但是这个并没有从本质上改变使用new.
第二种方式彻底抛弃new.
作为一个面向对象编程无非就关心两件事，**创建和继承**。
那好我们看看创建的时候怎么搞，
第一种方式，单体模式，不用墨迹了。
第二种方式，就不讲了，其实就是jquery那个方式，我们怎么简单怎么来。
单体模式
```
var girlFriend = {
      //属性：
      hair:"long",
      eye:"big",
      hobby:"travel",
      //方法
      cook:function(){},
      wash:function(){}
 };
```
好关键看继承，我们看见你女儿怎么把你女朋友的优良基因继承下来的。
简单的令人发指。
```
var daughter =  Object.create(girlFriend);
```
完了，确实完了，这就继承完了，以前的改this指向，什么修改原型，去你妹的统统的没了。
我知道你想说啥，我擦嘞，兼容性咋样。好吧，本着授人以鱼不如授人以红烧鱼的精神，大彬哥给你搞一把
```
Object.prototype.create = Object.prototype.create||function(o){
  
        function F(){}
        F.prototype = o;
        return new F(); 
  
};
```
好，这样就鸟了，多么蓝的天啊，面向对象的语法就结束了，剩下的就是面向对象的实际案例了，比如组件化开发，后面我们就看看实际库怎么玩的。
