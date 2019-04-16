# node.js之调试器 - ggabcda的博客 - CSDN博客





2017年08月22日 15:12:13[ggabcda](https://me.csdn.net/ggabcda)阅读数：237








# node.js之调试器

## 1.在命令行窗口中，可以使用"node debug" 命令来启用调试器，代码如下：
`node debug<需要被执行的脚本文件名>`
接下来根据一个实例进行学习调试过程：

编写app.js文件进行调试：

```
console.log('hello,word')
function foo(){
    console.log('hello,foo')
    return 100;
}
var bar = 'This is a pen';
var http = require('http')
var i = foo();
console.log(i)
```

### 1 在命令行窗口使用"node debug"命令，结果如下所示：

```
[root@kuber2 webproject]# node debug app.js
< Debugger listening on [::]:5858
connecting to 127.0.0.1:5858 ... ok
break in app.js:1
> 1 console.log('hello,word')
  2 function foo(){
  3 console.log('hello,foo')
debug>
```

在Node.js中提供了一个可以通过简单TCP协议来访问的调试器，进入调试后，首先连接调试器，如上图所示出现Debugger并后面跟有ok，说明调试器已经成功使用。"break in app.js:1" 表示脚本程序暂停的位置

### 2继续执行可输入"cont"或"c"，继续执行剩余的代码，结果如下所示：

```
[root@kuber2 webproject]# node debug app.js
< Debugger listening on [::]:5858
connecting to 127.0.0.1:5858 ... ok
break in app.js:1
> 1 console.log('hello,word')
  2 function foo(){
  3 console.log('hello,foo')
debug> c
< hello,word
< hello,foo
< 100
```

### 3如果不需要执行剩余的所有脚本代码，输入"next"或"n"将程序执行到下一句可执行代码前，如下所示：

```
[root@kuber2 webproject]# node debug app.js
< Debugger listening on [::]:5858
connecting to 127.0.0.1:5858 ... ok
break in app.js:1
> 1 console.log('hello,word')
  2 function foo(){
  3 console.log('hello,foo')
debug> n
< hello,word
break in app.js:6
  4 return 100;
  5 }
> 6 var bar = 'This is a pen';
  7 var http = require('http')
  8 var i = foo();
debug>
```

### 4如果想进入函数进行调试，可以在函数被调用出输入命令"step"或"s",进入函数体，如图所示：

```
break in app.js:8
  6 var bar = 'This is a pen';
  7 var http = require('http')
> 8 var i = foo();
  9 console.log(i)
 10 
debug> s
break in app.js:3
  1 console.log('hello,word')
  2 function foo(){
> 3 console.log('hello,foo')
  4 return 100;
  5 }
debug>
```

### 5在函数体中可以用"n",逐步进行调试，还可输入"out"或"o" 立即执行完函数内所有代码，程序将暂停到被调用函数代码后的一句代码前，如图所示：

```
break in app.js:3
  1 console.log('hello,word')
  2 function foo(){
> 3 console.log('hello,foo')
  4 return 100;
  5 }
debug> n
< hello,foo
break in app.js:4
  2 function foo(){
  3 console.log('hello,foo')
> 4 return 100;
  5 }
  6 var bar = 'This is a pen';
debug> o
break in app.js:9
  7 var http = require('http')
  8 var i = foo();
> 9 console.log(i)
 10 
 11 });
debug>
```

### 6 利用watch可以观察变量或一些表达式的值，利用unwatch可以取消观看，具体应用如下代码所示：

```
break in app.js:1
> 1 console.log('hello,word')
  2 function foo(){
  3 console.log('hello,foo')
debug> watch("i")
debug> watch("i==100")
debug> n
< hello,word
break in app.js:6
Watchers:
  0: i = undefined
  1: i==100 = false

  4 return 100;
  5 }
> 6 var bar = 'This is a pen';
  7 var http = require('http')
  8 var i = foo();
debug> n
break in app.js:7
Watchers:
  0: i = undefined
  1: i==100 = false

  5 }
  6 var bar = 'This is a pen';
> 7 var http = require('http')
  8 var i = foo();
  9 console.log(i)
debug> n
break in app.js:8
Watchers:
  0: i = undefined
  1: i==100 = false

  6 var bar = 'This is a pen';
  7 var http = require('http')
> 8 var i = foo();
  9 console.log(i)
 10 
debug> n
< hello,foo
break in app.js:9
Watchers:
  0: i = 100
  1: i==100 = true

  7 var http = require('http')
  8 var i = foo();
> 9 console.log(i)
 10 
 11 });
debug> unwatch("i")
debug> unwatch("i==100")
debug> n
< 100
break in app.js:11
  9 console.log(i)
 10 
>11 });
debug>
```

### 7可以利用setBreakpoint(filename,line)或cb(filename,line),设置断点，第一个参数为设置断点的文件名，不填默认为i正在运行的文件，line为需要加断点的行号;可以利用clearBreakPoint(filename,line)或cb(filename,line)取消断点

### 8使用list(n)可以显示所调试语句的前几后与后几行代码

### 9使用repl可以特推出debug模式

### 10使用restart可以重新开始文本调试

### 11 使用kill终止脚本文件调试

### 12利用kill终止文本调试后可利用run重新开始

### 13利用backtrace或bt可以返回运行代码的所处位置

### 14利用script可以查看所有被加载的模块名称



