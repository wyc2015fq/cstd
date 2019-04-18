# Boost.Coroutine2：学习使用Coroutine（协程） - zwvista - 博客园

## [Boost.Coroutine2：学习使用Coroutine（协程）](https://www.cnblogs.com/zwvista/p/7764277.html)

### function（函数）routine（例程）coroutine （协程）

函数，例程以及协程都是指一系列的操作的集合。

函数（有返回值）以及例程（没有返回值）也被称作subroutine（子例程），因为它们的执行过程通常在父例程之前结束。

协程则有所不同，它是例程一般化的结果。

协程的执行过程允许被
- 中途挂起。（suspend）
- 稍后恢复运行。（resume）

协程通常用于实现
- 生成器。（generators）
- 异步函数。（asynchronous functions）

两者的区别在于：
- 生成器的恢复执行由用户显式调用来决定。
- 异步函数的恢复执行由后台线程来决定。

### boost::coroutines2::coroutine<>

boost::coroutines2::coroutine<>被用来实现协程。

它有两个嵌套类型：pull_type和push_type。

pull_type可以从push_type那里接收并返回数据。

push_type可以把数据传给pull_type。

```
#include <iostream>
#include <boost/coroutine2/coroutine.hpp>
using namespace std;

int main()
{
    typedef boost::coroutines2::coroutine<int> coro_t;
    int max = 8;
    coro_t::pull_type source(
        [&](coro_t::push_type& sink){
            int first=1,second=1;
            sink(first);
            sink(second);
            for(int i=0;i<max;++i){
                int third=first+second;
                first=second;
                second=third;
                sink(third);
            }
        });

    for(auto i:source)
        cout << i <<  " ";
    cout << endl;

    coro_t::push_type sink(
        [&](coro_t::pull_type& source){
            while(source){
                cout << source.get() <<  " ";
                source();
            }
        });

    vector<int> v{1,1,2,3,5,8,13,21,34,55};
    copy(begin(v),end(v),begin(sink));
}

// 1 1 2 3 5 8 13 21 34 55 
// 1 1 2 3 5 8 13 21 34 55
```
- 这是一个使用协程实现斐波那契数列生成器的例子。
- 协程类的类型为boost::coroutines2::coroutine<int>。也就是说协程和主线程间相互传递的数据类型为int。
- pull_type类型对象source在构建时使用一个lambda来初始化。该lambda带有一个push_type的引用参数sink。使用sink可以将数据传回主线程。

C#同等功能的代码如下

```
using System;
using System.Collections.Generic;

namespace Sample
{
    class Fibonacci
    {
        public static void Main(string[] args)
        {
            IEnumerable<int> Source(int max)
            {
                int first = 1, second = 1;
                yield return first;
                yield return second;
                for (int i = 0; i < max; ++i)
                {
                    int third = first + second;
                    first = second;
                    second = third;
                    yield return third;
                }
            }
            foreach (int i in Source(8))
                Console.Write($"{i} ");
            Console.WriteLine();

            var v = new List<int> { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
            void Sink()
            {
                foreach(int i in v)
                {
                    Console.Write($"{i} ");
                }
            }
            Sink();
        }
    }
}   

// 1 1 2 3 5 8 13 21 34 55
// 1 1 2 3 5 8 13 21 34 55
```


