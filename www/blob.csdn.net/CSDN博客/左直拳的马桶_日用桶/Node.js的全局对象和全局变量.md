# Node.js的全局对象和全局变量 - 左直拳的马桶_日用桶 - CSDN博客
2014年12月12日 00:00:00[左直拳](https://me.csdn.net/leftfist)阅读数：27770
1、全局对象
所有模块都可以调用
1）global：表示Node所在的全局环境，类似于浏览器中的window对象。
2）process：指向Node内置的process模块，允许开发者与当前进程互动。
例如你在DOS或终端窗口直接输入node，就会进入NODE的命令行方式（REPL环境）。退出要退出的话，可以输入 process.exit();
3）console：指向Node内置的console模块，提供命令行环境中的标准输入、标准输出功能。
通常是写console.log()，无须多言
2、全局函数：
1）定时器函数：共有4个，分别是setTimeout(), clearTimeout(), setInterval(), clearInterval()。
2）require：用于加载模块。岐王宅里寻常见，崔九堂前几度闻。
3、全局变量：
1）_filename：指向当前运行的脚本文件名。
2）_dirname：指向当前运行的脚本所在的目录。
4、准全局变量
模块内部的局部变量，指向的对象根据模块不同而不同，但是所有模块都适用，可以看作是伪全局变量，主要为module, module.exports, exports等。
module变量指代当前模块。module.exports变量表示当前模块对外输出的接口，其他文件加载该模块，实际上就是读取module.exports变量。
module.id 模块的识别符，通常是模块的文件名。
module.filename 模块的文件名。
module.loaded 返回一个布尔值，表示模块是否已经完成加载。
module.parent 返回使用该模块的模块。
module.children 返回一个数组，表示该模块要用到的其他模块。
这里需要特别指出的是，exports变量实际上是一个指向module.exports对象的链接，等同在每个模块头部，有一行这样的命令。
var exports = module.exports;
这造成的结果是，在对外输出模块接口时，可以向exports对象添加方法，但是不能直接将exports变量指向一个函数：
exports = function (x){ console.log(x);};
上面这样的写法是无效的，因为它切断了exports与module.exports之间的链接。但是，下面这样写是可以的。
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/41877439#)[copy](http://blog.csdn.net/leftfist/article/details/41877439#)
- exports.area = function (r) {  
- return Math.PI * r * r;  
- };  
- 
- exports.circumference = function (r) {  
- return 2 * Math.PI * r;  
- };  
如果你觉得，exports与module.exports之间的区别很难分清，一个简单的处理方法，就是放弃使用exports，只使用module.exports。
参考文章：
[http://javascript.ruanyifeng.com/nodejs/basic.html#toc5](http://javascript.ruanyifeng.com/nodejs/basic.html#toc5)
