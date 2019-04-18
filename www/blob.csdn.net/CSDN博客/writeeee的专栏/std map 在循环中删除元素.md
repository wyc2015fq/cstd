# std::map 在循环中删除元素 - writeeee的专栏 - CSDN博客
2017年02月17日 15:44:54[writeeee](https://me.csdn.net/writeeee)阅读数：1963
```
for(；iter!=mapStudent.end()；)
{
     if((iter->second)>=aa)
     {
         //满足删除条件，删除当前结点，并指向下面一个结点
              mapStudent.erase(iter++)；
     }
     else
     {
         //条件不满足，指向下面一个结点
         iter++；
     }
}
```
这种删除方式也是STL源码一书中推荐的方式，分析 mapStudent.erase(iter++)语句，map中在删除iter的时候，先将iter做缓存，然后执行iter++使之指向下一个结点，再进入erase函数体中执行删除操作，删除时使用的iter就是缓存下来的iter(也就是当前iter(做了加操作之后的iter)所指向结点的上一个结点)。 
    根据以上分析，可以看出mapStudent.erase(iter++)和map Student.erase(iter)； iter++；这个执行序列是不相同的。前者在erase执行前进行了加操作，在iter被删除(失效)前进行了加操作，是安全的；后者是在erase执行后才进行加操作，而此时iter已经被删除(当前的迭代器已经失效了)，对一个已经失效的迭代器进行加操作，行为是不可预期的，这种写法势必会导致 map操作的失败并引起进程的异常。
