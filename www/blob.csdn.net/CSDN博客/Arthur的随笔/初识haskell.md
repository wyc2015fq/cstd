# 初识haskell - Arthur的随笔 - CSDN博客
2011年06月24日 23:44:00[largetalk](https://me.csdn.net/largetalk)阅读数：732标签：[haskell																[function																[erlang																[concurrency																[list																[lisp](https://so.csdn.net/so/search/s.do?q=lisp&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=concurrency&t=blog)](https://so.csdn.net/so/search/s.do?q=erlang&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=haskell&t=blog)
个人分类：[haskell](https://blog.csdn.net/largetalk/article/category/837038)
其实很久前（大概还在学校吧）就有学习函数式语言的冲动，但一直由于各种事情没能学成。 现在下定决心了一定要好好学习一下haskell， 虽然目前不知道会拿来作什么用，但学习一种新的编程思想也是很不错的。一开始听说函数式编程是从lisp开始， 应该是室友有阵子搞过这个， 我好像也装过clisp的编译器， 但看着晕乎乎的英文文档也晕乎乎了，就没坚持下来。后来又听说过F＃， erlang，都没研究。 erlang是一个现在非常火的语言，原生支持分布式。我说的不好，还是看erlang官方的简介吧，Erlang is a programming language used to build massively scalable soft real-time systems with requirements on high availability. Some of its uses are in telecoms, banking, e-commerce, computer telephony and instant messaging. Erlang's runtime system has built-in support for concurrency, distribution and fault tolerance. 有个同事也在研究，我这个人不喜欢与别的人一样， 就这样准备开始来学haskell了。
以前下过一本haskell的书， 叫《真实的haskell》吧，但没看，后来在网上发现一个网站 [http://learnyouahaskell.com/](http://learnyouahaskell.com/)， 对haskell初学者来说很好，我就随着这个网站开始我的haskell的学习之旅了。
我使用的系统是ubuntu， 首先是安装askell编译解释器， 这个简单，sudo apt-get install ghc6 ,这就好了然后typein ghci就可以使用haskell解释器动手实践了。
照着这个网站学，前面都很简单， 这里有个概念要牢记，haskell里面是没有变量的， 只有函数， 就连let a = 1 这样的语句也不是定义变量， 而是定义了一个函数名为a的函数，这个函数返回int 1。
在一个文件中定义一个函数是这样的,   doubleMe x = x ＋ x， 然后用 "  :l 文件名 " load to interpretation, so you can use doubleMe 10 to call this funcion.
call a function is like that:  **function_name param1 param2 ... **, use space to separate function name and all params.
you can also use if statement in function: 
doubleSmallNumber x = if x > 100 
                                           then x
                                           else x*2
list: use ++ function to merge two list into one, (in haskell, all operator +, -, *, /  is function, because this function take two params, so you can put the function name between params, `elem` is function for judge elem is in one list or not, so you can call `elem` function like: 
4 `elem` [1,2,3,4] ),  !! return the elem in the index you point, use >, <, >=, <= compare two list. 
more list function include: head, tail, last, init, length, null, reverse, take, drop, maximum, minimum, sum, product, elem
今天就写到这了， 下次接着写
ps。 有专门写博客的软件没，每次写博客还得登上网站，麻烦

