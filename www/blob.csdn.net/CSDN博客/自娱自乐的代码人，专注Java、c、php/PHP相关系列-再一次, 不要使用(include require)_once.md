# PHP相关系列 - 再一次, 不要使用(include/require)_once - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月04日 18:52:56[initphp](https://me.csdn.net/initphp)阅读数：1334








原文：http://www.laruence.com/2012/09/12/2765.html


最近关于apc.include_once_override的去留, 我们做了几次讨论, 这个APC的配置项一直一来就没有被很好的实现过.

在这里, 我想和大家在此分享下, 这个问题的原因, 以及对我们的一些启示.

关于使用include还是include_once(以下,都包含require_once), 这个讨论很长了, 结论也一直有, 就是尽量使用include, 而不是include_once, 以前最多的理由的是, include_once需要查询一遍已加载的文件列表, 确认是否存在, 然后再加载.

诚然, 这个理由是对的, 不过, 我今天要说的, 是另外一个的原因.

我们知道, PHP去判断一个文件是否被加载, 是需要得到这个文件的opened_path的, 意思是说, 比如:






```php
<?php
    set_include_path("/tmp/:/tmp2/");
    include_once("2.php");
    ?>
```




当PHP看到include_once “2.php”的时候, 他并不知道这个文件的实际路径是什么, 也就无法从已加载的文件列表去判断是否已经加载, 所以在include_once的实现中, 会首先尝试解析这个文件的真实路径(对于普通文件这个解析仅仅类似是检查getcwd和文件路径, 所以如果是相对路径, 一般是不会成功), 如果解析成功, 则查找EG(include_files), 如果存在则说明包含过了, 返回, 否则open这个文件, 从而得到这个文件的opened_path. 比如上面的例子, 这个文件存在于
 “/tmp2/2.php”.

然后, 得到了这个opened_path以后, PHP去已加载的文件列表去查找, 是否已经包含, 如果没有包含, 那么就直接compile, 不再需要open file了.

```
1. 尝试解析文件的绝对路径, 如果能解析成功, 则检查EG(included_files), 存在则返回, 不存在继续
    2. 打开文件, 得到文件的打开路径(opened path)
    3. 拿opened path去EG(included_files)查找, 是否存在, 如果存在则返回, 不存在继续
    4. 编译文件(compile_file)
```


这个在大多数情况下, 不是问题, 然而问题出在当你使用APC的时候…

在使用APC的时候, APC劫持了compile_file这个编译文件的指针, 从而直接从cache中得到编译结果, 避免了对实际文件的open, 避免了对open的system call.

然而, 当你在代码中使用include_once的时候, 在compile_file之前, PHP已经尝试去open file了, 然后才进入被APC劫持的compile file中, 这样一来, 就会产生一次额外的open操作. 而APC正是为了解决这个问题, 引入了include_once_override, 在include_once_override开启的情况下, APC会劫持PHP的ZEND_INCLUDE_OR_EVAL opcode handler, 通过stat来确定文件的绝对路径, 然后如果发现没有被加载,
 就改写opcode为include, 做一个tricky解决方案. 

但是, 很可惜, 如我所说, APC的include_once_override实现的一直不好, 会有一些未定义的问题, 比如:

```php
<?php
    set_include_path("/tmp");
    function a($arg = array()) {
        include_once("b.php");
    }
     
    a();
    a();
    ?>
```


然后, 我们的b.php放置在”/tmp/b.php”, 内容如下:



```php
<?php
      class B {}
    ?>
```


那么在打开apc.include_once_override的情况下, 连续访问就会得到如下错误:




`Fatal error - include() : Cannot redeclare class b`



(后记 2012-09-15 02:07:20: 这个APC的bug我已经修复: [#63070](https://bugs.php.net/bug.php?id=63070))

排除这些技术因素, 我也一直认为, 我们应该使用include, 而不是include_once, 因为我们完全能做到自己规划, 一个文件只被加载一次. 还可以借助自动加载, 来做到这一点.

你使用include_once, 只能证明, 你对自己的代码没信心.

所以, 建议大家, 不要再使用include_once



