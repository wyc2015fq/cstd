
# python函数星号参数 - 高科的专栏 - CSDN博客

2011年09月01日 17:35:11[高科](https://me.csdn.net/pbymw8iwm)阅读数：615标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[fun																](https://so.csdn.net/so/search/s.do?q=fun&t=blog)[服务器																](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)[脚本																](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[工作																](https://so.csdn.net/so/search/s.do?q=工作&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)
[
																								](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=fun&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=python&t=blog)
2011-09-01 17:352人阅读[评论](http://blog.csdn.net/pbymw8iwm/article/details/6740369#comments)(0)收藏[编辑](http://write.blog.csdn.net/postedit/6740369)删除今天有个工作是导出一个函数给脚本用
我自已先要测一下
先要客户端发送一个消息给服务器
看了下C++部分的代码,如下
"def onNetMessage(self,playerID, msgName,msgParam):\n" //客户端调用服务器脚本
"    fun = globals()[msgName]\n"
"    args = eval(msgParam)\n"
"    fun(self, playerID, *args)\n"
起初凭自已的理解,写出的代码总是出错,
在网上搜了一下,又作了个实验,得以解决,
原来是参数 msgParam必须得是"[1,2]"类似的形式
三行的含义:
1.先从通过msgName字符找到函数 "fun1" => fun1
2.eval把字符串msgParam变成参数,"[1,2]" => [1,2]
3.等价为fun1(self,playerID,1,2)
引用一篇文章
神奇的星号
传递实参和定义形参（所谓实参就是调用函数时传入的参数，形参则是定义函数是定义的参数）的时候，你还可以使用两个特殊的语法：``*`` ** 。
调用函数时使用 * **
test(*args)* 的作用其实就是把序列 args 中的每个元素，当作位置参数传进去。比如上面这个代码，如果 args 等于 (1,2,3) ，那么这个代码就等价于 test(1, 2, 3) 。
test(**kwargs)** 的作用则是把字典 kwargs 变成关键字参数传递。比如上面这个代码，如果 kwargs 等于 {'a':1,'b':2,'c':3} ，那这个代码就等价于 test(a=1,b=2,c=3) 。
定义函数参数时使用 * **
def test(*args):
...定义函数参数时 * 的含义又要有所不同，在这里 *args 表示把传进来的位置参数都装在元组 args 里面。比如说上面这个函数，调用 test(1, 2, 3) 的话， args 的值就是 (1, 2, 3) 。:
def test(**kwargs):
...类似的， ** 就是针对关键字参数和字典的了。 调用 test(a=1,b=2,c=3) 的话， kwargs 的值就是 {'a':1,'b':2,'c':3} 了。
普通的参数定义和传递方式和 * 们都可以和平共处，不过显然 * 必须放在所有位置参数的最后，而 ** 则必须放在所有关键字参数的最后，否则就要产生歧义了。


