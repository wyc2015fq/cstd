
# python的异常机制 - 喜欢打酱油的老鸟 - CSDN博客


2019年03月13日 12:39:22[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：17


[https://www.toutiao.com/a6666700570595688963/](https://www.toutiao.com/a6666700570595688963/)

```python
商家确认
finally：
	删除订单
```
上面这个例子是我现编的，不知道真实的公司会不会这么草率地删除一项记录。
不过finally真的非常适合用于确保文件或者网络套接字等关掉。
# 8警告
编程的时候会遇到一些警告，警告类似于异常。但他们通常都是只打印一条错误消息。
例如：
```python
>>> import warnings as win
>>> win.warn("this is a warning")
__main__:1: UserWarning: this is a warning
>>> win.warn("this is second warning")
__main__:1: UserWarning: this is second warning
>>> win.warn("this is a warning")
```
警告只显示一次，如果再次运行一次win.warn("this is a warning")，是不会显示警告内容的。
每当生成一个警告时，都需要将其与注册过的过滤器比较，第一个匹配的过滤器将控制这个警告采取的动作，否则采取默认的动作，其实本质流程上会发现与Exception异常处理类似，看一下常见的过滤器动作：
```python
error：将警告提升为异常
 ignore：忽略警告
 always：总是抛出警告
 default：从各个位置第一次生成警告时输出警告
 module：从各个模块第一次生成警告时输出警告
 once：第一次生成警告时输出警告
```
有时候的一些警告没有什么特殊的意义，我们可以使用函数将他们抑制，例如：
```python
>>> win.filterwarnings("ignore")
>>> win.warn("this is second warning ")
>>> win.warn("this is second warning ")
Traceback (most recent call last):
 File "<stdin>", line 1, in <module>
UserWarning: this is second warning
```
上面分别展示了ignore和error两种状况，分别将警告忽略或者转成错误。
一般而言，一个警告只出现一次，但是使用always可以让他每次执行都出现，
```python
>>> win.warn("this is third warning",)
__main__:1: UserWarning: this is third warning
>>> win.warn("this is third warning",)
__main__:1: UserWarning: this is third warning
```

