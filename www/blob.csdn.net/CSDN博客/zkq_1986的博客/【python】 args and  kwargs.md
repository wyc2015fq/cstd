# 【python】*args and **kwargs - zkq_1986的博客 - CSDN博客





2017年12月14日 17:00:09[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：71
个人分类：[程序设计语言](https://blog.csdn.net/zkq_1986/article/category/6534515)









## `*args` and `**kwargs`

`*args用于接收列表（字符串也行）``；``**kwargs用于接收``键值对。`

`1. 函数参数定义时可用；`

`2. 调用函数，传入参数时也可用。`


当你不确定你的函数里将要传递多少参数时你可以用`*args`.例如,它可以传递任意数量的参数:























Python




|12345678|>>>defprint_everything(*args):forcount,thing inenumerate(args):...print'{0}. {1}'.format(count,thing)...>>>print_everything('apple','banana','cabbage')0.apple1.banana2.cabbage|
|----|----|

相似的,`**kwargs`允许你使用没有事先定义的参数名:























Python




|1234567|>>>deftable_things(**kwargs):...forname,valueinkwargs.items():...print'{0} = {1}'.format(name,value)...>>>table_things(apple='fruit',cabbage='vegetable')cabbage=vegetableapple=fruit|
|----|----|




你也可以混着用.命名参数首先获得参数值然后所有的其他参数都传递给`*args`和`**kwargs`.命名参数在列表的最前端.例如:























Python




|1|deftable_things(titlestring,**kwargs)|
|----|----|

`*args`和`**kwargs`可以同时在函数的定义中,但是`*args`必须在`**kwargs`前面.

当调用函数时你也可以用`*`和`**`语法.例如:























Python




|1234567|>>>defprint_three_things(a,b,c):...print'a = {0}, b = {1}, c = {2}'.format(a,b,c)...>>>mylist=['aardvark','baboon','cat']>>>print_three_things(*mylist)a=aardvark,b=baboon,c=cat|
|----|----|




就像你看到的一样,它可以传递列表(或者元组)的每一项并把它们解包.注意必须与它们在函数里的参数相吻合.当然,你也可以在函数定义或者函数调用时用*.





转载自：http://python.jobbole.com/85231/#article-comment



