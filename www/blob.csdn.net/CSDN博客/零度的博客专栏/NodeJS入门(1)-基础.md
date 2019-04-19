# NodeJS入门(1)-基础 - 零度的博客专栏 - CSDN博客
2016年11月02日 11:23:06[零度anngle](https://me.csdn.net/zmx729618)阅读数：288
1、什么是NodeJS
       浏览器可以解析JS，NodeJS也可以解析JS，它就是一个JS的解析器。浏览器利用JS操作DOM，而NodeJS利用JS操作磁盘文件或搭建HTTP服务器。NodeJS的作者创造NodeJS的目的是为了实现高性能的Web服务器，所以他首先看重的是事件机制和异步IO模型的优越性。
        安装nodejs，可参考 [windows下搭建node.js及npm的工作环境](http://blog.csdn.net/fm2005/article/details/41319591) 。可再次进行如下测试，在命令行中输入`node`，进行node命令行，输入
```
console.log('Hello
 World!')
```
,可看到控制台的输出`Hello World!`。也可编写一名为`hello.js`的JS文件，内容如下： 
 function hello() { console.log('Hello World!'); } hello();
        写好后，在命令行输入
```
node
 hello.js
```
运行，也可看到输出。 3.模块化。NodeJS中，将JS代码合理拆分到不同的JS文件中，一个文件就是一个模块，而文件路径就是模块名。在编写模块时，有`require`、`exports`、`module`三个预先定义好的变量可供使用。
2、require :
    require用于在当前模块中加载和使用别的模块，传入一个模块名，返回一个模块导出对象。模块名可使用相对路径（以./开头），或者是绝对路径（以/或C:之类的盘符开头）。另外，模块名中的.js扩展名可以省略。以下是一个例子。 var foo1 = require('./foo'); var foo2 = require('./foo.js'); var foo3 = require('/home/user/foo'); var foo4 =
 require('/home/user/foo.js');
```
// foo1至foo4中保存的是同一个模块的导出对象。
```
另外，可以使用以下方式加载和使用一个JSON文件。
`var data = require('./data.json');`
3、exports
     exports对象是当前模块的导出对象，用于导出模块公有方法和属性。别的模块通过require函数使用当前模块时得到的就是当前模块的exports对象。以下例子中导出了一个公有方法。
      exports.hello = function () { console.log('Hello World!'); };
4、module
      通过module对象可以访问到当前模块的一些相关信息，但最多的用途是替换当前模块的导出对象。例如模块导出对象默认是一个普通对象，如果想改成一个函数的话，可以使用以下方式。
```
module.exports = function () {
    console.log('Hello World!');
};
```
     以上代码中，模块默认导出对象被替换为一个函数。
5、模块
      模块初始化：一个模块中的JS代码仅在模块第一次被使用时执行一次，并在执行过程中初始化模块的导出对象。之后，缓存起来的导出对象被重复利用。
      主模块：直接通过命令行参数传递给NodeJS以启动程序的模块被称为主模块。主模块负责调度组成整个程序的其它模块完成工作。例如通过
```
node
 main.js
```
命令启动程序时，main.js就是主模块。
6、一个完整示例
例如有以下目录。
```
- F:\hello\
    - util\
        counter.js
    main.js
```
其中counter.js内容如下：
```
var i = 0;
function count() {
    return ++i;
}
exports.count = count;
```
该模块内部定义了一个私有变量i，并在exports对象导出了一个公有方法count。
主模块main.js内容如下：
```
var counter1 = require('./util/counter');
var    counter2 = require('./util/counter');
console.log(counter1.count());
console.log(counter2.count());
console.log(counter2.count());
```
运行该程序的结果如下：
```
$ node main.js
1
2
3
```
可以看到，counter.js并没有因为被require了两次而初始化两次。
