# Node.js 编程 -- module - cocoonyang的专栏 - CSDN博客





2013年02月03日 22:43:13[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：496








[1]详细介绍了Node.js的模块系统。[2]对这个模块机制作了深入探讨。Node.js使用require方法引入模块。调用起来十分简单。

Node.js的模块分为两类： 核心模块和文件模块。Node.js启动时会自动加载核心模块。核心模块优先级要高于文件模块，因此，即使有与核心模块同名的文件模块存在，Node.js也会对它们置之不理。

Node.js的有三类文件模块，其文件后缀分别为：.js， .json，和 .node。其中，.js文件是JavaScript脚本文件；.json文件是JSON文本文件； .node是编译后的原生扩展文件。

以JavaScript脚本文件为载体的Node.js文件模块定义、封装和使用最方便。[1]给出了一个计算圆的周长和面积的简单样例：circle模块。首先，创建一个名为circle.js的JavaScript脚本文件，其内容如下：



```java
var PI = Math.PI;

exports.area = function (r) {
  return PI * r * r;
};

exports.circumference = function (r) {
  return 2 * PI * r;
}
```
在调用circle模块的代码如下：

```java
var circle = require('./circle.js');
console.log( 'The area of a circle of radius 4 is '
           + circle.area(4));
```

Node.js的在遇到代码  require('./circle.js') 时，就会启动模块机制，加载circle模块。具体的路径、文件搜索算法详见[1][2]。




如果想把模块当做一个类，可将上述circle.js中的代码改写成



```java
var PI = Math.PI;
module.exports = function( radius ) {
    this.r = radius;

    this.area = function() {
	return PI * this.r * this.r;
    };

    this.circumference = function() {
	return 2 * PI * this.r;
    };
};
```


 使用时的代码如下：





```java
var Circle = require('./circle.js');
var circle = new Circle( 4 );
console.log( 'The area of a circle of radius 4 is '
           + circle.area( ));
```




在这里需要注意的一点是exports和module.exports的区别和联系。

exports和module.exports具有相同的功用，JavaScript脚本文件形式的Node.js文件模块被调用时，它返回的是module.exports，exports只是个辅助工具。如果module.exports没有被赋予任何属性或方法，exports则将本身的方法和属性都复制给module.exports。如果module.exports已经被赋予任何属性或方法，则赋给exports属性和方法，都会被忽略。













参考文献

[1] http://nodejs.org/docs/latest/api/modules.html


[2] http://www.infoq.com/cn/articles/nodejs-module-mechanism




