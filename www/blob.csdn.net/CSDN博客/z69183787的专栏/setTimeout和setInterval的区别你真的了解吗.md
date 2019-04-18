# setTimeout和setInterval的区别你真的了解吗? - z69183787的专栏 - CSDN博客
2014年03月08日 09:52:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1503
setTimeout和setInterval这两个函数, 大家肯定都不陌生, 但可能并不是每个用过这两个方法的同学, 都了解其内部的实质
甚至可能会错误的把两个实现定时调用的函数理解成了类似thread一样的东西,
 认为会在一个时间片内, 并发的执行调用的函数, 似乎很好很强大, 但其实并不是如此, 实际的情况是javascript都是以单线程的方式运行于浏览器的javascript引擎中的, setTimeout和setInterval的作用只是把你要执行的代码在你设定的一个时间点插入js引擎维护的一个代码队列中, 插入代码队列并不意味着你的代码就会立马执行的,理解这一点很重要. 而且setTimeout和setInterval还有点不一样. 
先谈谈setTimeout 
复制代码代码如下:
function click() { 
// code block1... 
setTimeout(function() { 
// process ... 
}, 200); 
// code block2 
} 
假设我们给一个button的onclick事件绑定了此方法, 当我们按下按钮后, 肯定先执行block1的内容, 然后运行到setTimeout的地方, setTimeout会告诉浏览器说, "200ms后我会插一段要执行的代码给你的队列中",
 浏览器当然答应了(注意插入代码并不意味着立马执行), setTimeout代码运行后, 紧跟其后的block2代码开始执行, 这里就开始说明问题了, **如果block2的代码执行时间超过200ms, 那结果会是如何? 或许按照你之前的理解, 会理所当然的认为200ms一到, 你的process代码会立马执行...事实是, 在block2执行过程中(执行了200ms后)process代码被插入代码队列, 但一直要等click方法执行结束, 才会执行process代码段, 从代码队列上看process代码是在click后面的, 再加上js以单线程方式执行, 所以应该不难理解. 如果是另一种情况, block2代码执行的时间<200ms, setTimeout在200ms后将process代码插入到代码队列, 而那时执行线程可能已经处于空闲状态了(idle), 那结果就是200ms后, process代码插入队列就立马执行了, 就让你感觉200ms后, 就执行了. ****再看看setInterval **
这里可能会存在两个问题: 
1.时间间隔或许会跳过 
2.时间间隔可能<定时调用的代码的执行时间 
复制代码代码如下:
function click() { 
// code block1... 
setInterval(function() { 
// process ... 
}, 200); 
// code block2 
} 
和上面一样我们假设通过一个click, 触发了setInterval以实现每隔一个时间段执行process代码
![](http://files.jb51.net/upload/201103/20110331002726170.jpg)
比如onclick要300ms执行完, block1代码执行完, 在5ms时执行setInterval, 以此为一个时间点, 在205ms时插入process代码, click代码顺利结束, process代码开始执行(相当于图中的timer code), 然而process代码也执行了一个比较长的时间, 超过了接下来一个插入时间点405ms, 这样代码队列后又插入了一份process代码, process继续执行着, 而且超过了605ms这个插入时间点,
 下面问题来, **可能你还会认为代码队列后面又会继续插入一份process代码...真实的情况是,由于代码队列中已经有了一份未执行的process代码, 所以605ms这个插入时间点将会被"无情"的跳过, 因为js引擎只允许有一份未执行的process代码, 说到这里不知道您是不是会豁然开朗呢...**
为了这种情况你可以用一种更好的代码形式
复制代码代码如下:
setTimeout(function(){ 
//processing 
setTimeout(arguments.callee, interval); 
}, interval); 
这个估计稍微想一下, 就明白其中的好处了, 这样就不会产生时间点被跳过的问题内容就到这里, 希望能有所帮助, 可能我表达的不是很清楚如果觉得自己英语基础不错可以直接看
![](http://files.jb51.net/upload/201103/20110331002726269.jpg)
里有关advanced Timers这节内容,  个人认为这本书真的很不错, 无论是想从零学起, 还是平日没事翻翻参考参考 都很不错, 作者是yahoo里很牛的一位前端开发工程师 : )
