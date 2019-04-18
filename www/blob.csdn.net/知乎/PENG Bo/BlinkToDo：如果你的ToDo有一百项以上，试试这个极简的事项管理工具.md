# BlinkToDo：如果你的ToDo有一百项以上，试试这个极简的事项管理工具 - 知乎
# 



当你的ToDo有近百项时，可能就会发现市面的ToDo工具体验极差，甚至不如用记事本开个txt管理。

我的ToDo有近千项，所以我写了一个基于txt的ToDo工具，纯 JS 前端代码，源码在此：
[BlinkDL/BlinkToDo​github.com![图标](https://pic2.zhimg.com/v2-14ff3f9bdcc35d88f5c4c0b21bacac1d_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/BlinkDL/BlinkToDo)
请点击这里试试效果（在最右边输入txt，然后在中间自动渲染）：
[Blink ToDo​blinkdl.github.io](https://link.zhihu.com/?target=https%3A//blinkdl.github.io/BlinkToDo/)
效果如图：
![](https://pic1.zhimg.com/v2-4026aa5ab23faa328563f2dde9ff8f88_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1062' height='288'></svg>)
上述测试代码：


```
. 用点代表层次关系
.. 用tab可以自动缩进（两个点）
... 这是三个点的效果 // 更多事项 // 更多更多
... 还有什么 // 什么什么
.. 我是标题：具体的事项 // 事项 // !重要事项 // 事项
. 一个点的是大类别
..! 用ctrl+s可以随时保存txt到磁盘
.. 内容保存在localStorage中 // 连接测试 http://baidu.com
.. 数学测试：$\xi(s) = \xi(1-s)$
. 还有更多神秘功能请查看代码
```


事项更多的时候的效果：
![](https://pic2.zhimg.com/v2-8052678081450c9484a28b8314735229_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='442'></svg>)
欢迎大家star和改进。觉得好用请点个赞噢。


