# setTimeout不可靠的修正办法及clearTimeout - 左直拳的马桶_日用桶 - CSDN博客
2016年06月08日 20:00:02[左直拳](https://me.csdn.net/leftfist)阅读数：10613
javascript里的这两个定时器函数，大家一定耳熟能详：
`setTimeout("函数()",毫秒)`就是开启一个计时器，指定毫秒后执行该函数一次。 
有关定时器，javascript还有另一个类似的函数，`setInterval("函数()",毫秒)`。不同的是，setInterval不是指定时间后执行一次该函数，而是每隔指定时间执行该函数，连续不断，直到clearInterval()。
问题是，在实际使用过程中，发现javascript的定时器很不靠谱。说好的多少多少时间后执行，但给人的感觉是忽快忽慢。明明指定3秒后执行，竟然5、6秒后才触发，或者不到1秒就触发了！
查阅资料，说是javascript为单线程，setTimeout之后，就注册了一个事件，进入排队，有空才执行，所以就慢了。大意如此。但快的情况呢？好像没说。
> 
javascript引擎只有一个线程，迫使异步事件只能加入队列去等待执行。 
  在执行异步代码的时候，setTimeout 和setInterval 是有着本质区别的。 
  如果计时器被正在执行的代码阻塞了，它将会进入队列的尾部去等待执行直到下一次可能执行的时间出现（可能超过设定的延时时间）。 
  如果interval回调函数执行需要花很长时间的话(比指定的延时长)，interval有可能没有延迟背靠背地执行。 
  上述这一切对于理解js引擎是如果工作的无疑是很重要的知识，尤其是大量的典型的异步事件发生时，对于构建一个高效的应用代码片段来说是一个非常有利的基础。
[JavaScript的计时器的工作原理](https://segmentfault.com/a/1190000002633108)
那么能不能修正呢？受网上文章启发，可以采用读取时间来应对：
> 
setTimeout的时候，记录当前时间戳 
  函数触发时，将时间戳与当前时间比较，看是否已经经过指定的毫秒数 
  时间未够，则继续setTimeout，步长可改为1秒 
  否则执行
```xml
<html>
<head>
</head>
<body>
<input type="button" value="开启" onclick="start()" />
</body>
</html>
<script type="text/javascript">
    var t;
    var marktime = 0;
    var offset = 3000;
    function start(){
        marktime = new Date().getTime();//1970年1月1日以来的毫秒数
        t = setTimeout("hi()",offset);//3000毫秒后触发
    }
    function hi(){
        if (new Date().getTime() - marktime < offset) {//时间未够
            t = setTimeout("hi()",1000);//一秒后再来看看
            return;
        }
        alert("Hello World!");
    }
</script>
```
以上方法可应对比指定时间快的情况。
至于clearTimeout，经测试
```
var t = setTimeout("hi()",1000);
clearTimeout();//不起任何作用
clearTimeout(t);//将计时器t消除
```
setTimeout的作用，就是注册一个计时器，计时器之间各自独立，不会覆盖和干扰；注册多少遍，就有多少个，执行指定函数后自己释放。所以每次setTimeout,应该获取返回值，以便操控：
```
var t = setTimeout("hi()",1000);
clearTimeout(t);//将计时器t消除
```
