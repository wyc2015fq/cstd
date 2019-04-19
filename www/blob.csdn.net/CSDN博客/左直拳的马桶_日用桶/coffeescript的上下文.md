# coffeescript的上下文 - 左直拳的马桶_日用桶 - CSDN博客
2014年11月25日 20:07:55[左直拳](https://me.csdn.net/leftfist)阅读数：1595
CoffeeScript代码中，变量，甚至函数前面有时会带上一个@符号，那么翻译到 javascript里，就是 “this.”
这就涉及到运行过程中的上下文。
这个this指什么，网上有专门的文章介绍。按我目前的理解，并不完全等同于面向对象语言里的this，是要区分情况：
1、如果它位于普通函数内部，那么这个this是指这个函数，或表明它的作用域，仅限于这个函数内部
2、如果所在函数属于 prototype，那么这个this就与运行中的上下文有关。
以下代码，可以仔细参详、比较一下：
CoffeeScript:
```
class User
  name:'unknown'
  constructor : (name) ->
    @name = name
  
  sayHello : ->
    "您好！\r\n普通函数：#{_getName()}!\r\nprototype函数：#{@getName()}!"
  _getName = ->
    @name + '先生'
  getName : ->
    @name + '先生'
user = new User('leftfist')
alert user.sayHello()
```
对应javascript:
```java
var User, user;
User = (function() {
  var _getName;
  User.prototype.name = 'unknown';
  function User(name) {
    this.name = name;
  }
  User.prototype.sayHello = function() {
    return "您好！\r\n普通函数：" + (_getName()) + "!\r\nprototype函数：" + (this.getName()) + "!";
  };
  _getName = function() {
    return this.name + '先生';
  };
  User.prototype.getName = function() {
    return this.name + '先生';
  };
  return User;
})();
user = new User('leftfist');
alert(user.sayHello());
```
运行结果就是
您好！
普通函数：先生！
prototype函数：leftfist 先生！
可以看到，在普通函数里，虽然使用了 this.name，但此this指的是_getName自己，非彼user = new User('leftfist')那个user，故而拿不到name，空空焉。
