
# python进程池：multiprocessing.pool - 机器学习的小学生 - CSDN博客


2017年06月17日 21:12:04[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：498


转载自：
[http://www.cnblogs.com/kaituorensheng/p/4465768.html](http://www.cnblogs.com/kaituorensheng/p/4465768.html)
在利用Python进行系统管理的时候，特别是同时操作多个文件目录，或者远程控制多台主机，并行操作可以节约大量的时间。当被操作对象数目不大时，可以直接利用multiprocessing中的Process动态成生多个进程，十几个还好，但如果是上百个，上千个目标，手动的去限制进程数量却又太过繁琐，此时可以发挥进程池的功效。
Pool可以提供指定数量的进程供用户调用，当有新的请求提交到pool中时，如果池还没有满，那么就会创建一个新的进程用来执行该请求；但如果池中的进程数已经达到规定最大值，那么该请求就会等待，直到池中有进程结束，才会创建新的进程来它。
## 例1：使用进程池
```python
#coding: utf-8
```
```python
import
```
```python
multiprocessing
```
```python
import
```
```python
time
```
```python
def
```
```python
func
```
```python
(msg)
```
```python
:
```
```python
print
```
```python
"msg:"
```
```python
, msg
    time.sleep(
```
```python
3
```
```python
)
```
```python
print
```
```python
"end"
```
```python
if
```
```python
__name__ ==
```
```python
"__main__"
```
```python
:
    pool = multiprocessing.Pool(processes =
```
```python
3
```
```python
)
```
```python
for
```
```python
i
```
```python
in
```
```python
xrange(
```
```python
4
```
```python
):
        msg =
```
```python
"hello %d"
```
```python
%(i)
        pool.apply_async(func, (msg, ))
```
```python
#维持执行的进程总数为processes，当一个进程执行完毕后会添加新的进程进去
```
```python
print
```
```python
"Mark~ Mark~ Mark~~~~~~~~~~~~~~~~~~~~~~"
```
```python
pool.close()
    pool.join()
```
```python
#调用join之前，先调用close函数，否则会出错。执行完close后不会有新的进程加入到pool,join函数等待所有子进程结束
```
```python
print
```
```python
"Sub-process(es) done."
```
一次执行结果
```python
Mark~ Mark~ Mark~~~~~~~~~~~~~~~~~~~~~~
msg: hello
```
```python
0
```
```python
msg: hello
```
```python
1
```
```python
msg: hello
```
```python
2
```
```python
end
```
```python
msg: hello
```
```python
3
```
```python
end
```
```python
end
```
```python
end
```
```python
Sub-
```
```python
process
```
```python
(es) done.
```
**函数解释：**
apply_async(func[, args[, kwds[, callback]]]) 它是非阻塞，apply(func[, args[, kwds]])是阻塞的（理解区别，看例1例2结果区别）
close()    关闭pool，使其不在接受新的任务。
terminate()    结束工作进程，不在处理未完成的任务。
join()    主进程阻塞，等待子进程的退出， join方法要在close或terminate之后使用。
**执行说明：**创建一个进程池pool，并设定进程的数量为3，xrange(4)会相继产生四个对象[0, 1, 2, 4]，四个对象被提交到pool中，因pool指定进程数为3，所以0、1、2会直接送到进程中执行，当其中一个执行完事后才空出一个进程处理对象3，所以会出现输出“msg: hello 3”出现在”end”后。因为为非阻塞，主函数会自己执行自个的，不搭理进程的执行，所以运行完for循环后直接输出“mMsg: hark~ Mark~ Mark~~~~~~~~~~~~~~~~~~~~~~”，主程序在pool.join（）处等待各个进程的结束。
## 例2：使用进程池（阻塞）
```python
#coding: utf-8
```
```python
import
```
```python
multiprocessing
```
```python
import
```
```python
time
```
```python
def
```
```python
func
```
```python
(msg)
```
```python
:
```
```python
print
```
```python
"msg:"
```
```python
, msg
    time.sleep(
```
```python
3
```
```python
)
```
```python
print
```
```python
"end"
```
```python
if
```
```python
__name__ ==
```
```python
"__main__"
```
```python
:
    pool = multiprocessing.Pool(processes =
```
```python
3
```
```python
)
```
```python
for
```
```python
i
```
```python
in
```
```python
xrange(
```
```python
4
```
```python
):
        msg =
```
```python
"hello %d"
```
```python
%(i)
        pool.apply(func, (msg, ))
```
```python
#维持执行的进程总数为processes，当一个进程执行完毕后会添加新的进程进去
```
```python
print
```
```python
"Mark~ Mark~ Mark~~~~~~~~~~~~~~~~~~~~~~"
```
```python
pool.close()
    pool.join()
```
```python
#调用join之前，先调用close函数，否则会出错。执行完close后不会有新的进程加入到pool,join函数等待所有子进程结束
```
```python
print
```
```python
"Sub-process(es) done."
```
一次执行的结果
```python
msg: hello
```
```python
0
```
```python
end
```
```python
msg: hello
```
```python
1
```
```python
end
```
```python
msg: hello
```
```python
2
```
```python
end
```
```python
msg: hello
```
```python
3
```
```python
end
```
```python
Mark~ Mark~ Mark~~~~~~~~~~~~~~~~~~~~~~
Sub-
```
```python
process
```
```python
(es) done.
```
## 例3：使用进程池，并关注结果
```python
import
```
```python
multiprocessing
```
```python
import
```
```python
time
```
```python
def
```
```python
func
```
```python
(msg)
```
```python
:
```
```python
print
```
```python
"msg:"
```
```python
, msg
    time.sleep(
```
```python
3
```
```python
)
```
```python
print
```
```python
"end"
```
```python
return
```
```python
"done"
```
```python
+ msg
```
```python
if
```
```python
__name__ ==
```
```python
"__main__"
```
```python
:
    pool = multiprocessing.Pool(processes=
```
```python
4
```
```python
)
    result = []
```
```python
for
```
```python
i
```
```python
in
```
```python
xrange(
```
```python
3
```
```python
):
        msg =
```
```python
"hello %d"
```
```python
%(i)
        result.append(pool.apply_async(func, (msg, )))
    pool.close()
    pool.join()
```
```python
for
```
```python
res
```
```python
in
```
```python
result:
```
```python
print
```
```python
":::"
```
```python
, res.get()
```
```python
print
```
```python
"Sub-process(es) done."
```
一次执行结果
```python
msg: hello 0
msg: hello 1
msg: hello 2
end
end
end
```
```python
:::
```
```python
donehello 0
```
```python
:::
```
```python
donehello 1
```
```python
:::
```
```python
donehello 2
Sub-process(es) done.
```
注：get()函数得出每个返回结果的值
## 例4：使用多个进程池
```python
#coding: utf-8
```
```python
import
```
```python
multiprocessing
```
```python
import
```
```python
os, time, random
```
```python
def
```
```python
Lee
```
```python
()
```
```python
:
```
```python
print
```
```python
"\nRun task Lee-%s"
```
```python
%(os.getpid())
```
```python
#os.getpid()获取当前的进程的ID
```
```python
start = time.time()
    time.sleep(random.random() *
```
```python
10
```
```python
)
```
```python
#random.random()随机生成0-1之间的小数
```
```python
end = time.time()
```
```python
print
```
```python
'Task Lee, runs %0.2f seconds.'
```
```python
%(end - start)
```
```python
def
```
```python
Marlon
```
```python
()
```
```python
:
```
```python
print
```
```python
"\nRun task Marlon-%s"
```
```python
%(os.getpid())
    start = time.time()
    time.sleep(random.random() *
```
```python
40
```
```python
)
    end=time.time()
```
```python
print
```
```python
'Task Marlon runs %0.2f seconds.'
```
```python
%(end - start)
```
```python
def
```
```python
Allen
```
```python
()
```
```python
:
```
```python
print
```
```python
"\nRun task Allen-%s"
```
```python
%(os.getpid())
    start = time.time()
    time.sleep(random.random() *
```
```python
30
```
```python
)
    end = time.time()
```
```python
print
```
```python
'Task Allen runs %0.2f seconds.'
```
```python
%(end - start)
```
```python
def
```
```python
Frank
```
```python
()
```
```python
:
```
```python
print
```
```python
"\nRun task Frank-%s"
```
```python
%(os.getpid())
    start = time.time()
    time.sleep(random.random() *
```
```python
20
```
```python
)
    end = time.time()
```
```python
print
```
```python
'Task Frank runs %0.2f seconds.'
```
```python
%(end - start)
```
```python
if
```
```python
__name__==
```
```python
'__main__'
```
```python
:
    function_list=  [Lee, Marlon, Allen, Frank]
```
```python
print
```
```python
"parent process %s"
```
```python
%(os.getpid())
    pool=multiprocessing.Pool(
```
```python
4
```
```python
)
```
```python
for
```
```python
func
```
```python
in
```
```python
function_list:
        pool.apply_async(func)
```
```python
#Pool执行函数，apply执行函数,当有一个进程执行完毕后，会添加一个新的进程到pool中
```
```python
print
```
```python
'Waiting for all subprocesses done...'
```
```python
pool.close()
    pool.join()
```
```python
#调用join之前，一定要先调用close() 函数，否则会出错, close()执行后不会有新的进程加入到pool,join函数等待素有子进程结束
```
```python
print
```
```python
'All subprocesses done.'
```
一次执行结果
```python
parent
```
```python
process
```
```python
7704
```
```python
Waiting
```
```python
for
```
```python
all subprocesses done...
Run task Lee-
```
```python
6948
```
```python
Run task Marlon-
```
```python
2896
```
```python
Run task Allen-
```
```python
7304
```
```python
Run task Frank-
```
```python
3052
```
```python
Task Lee, runs
```
```python
1.59
```
```python
seconds
```
```python
.
Task Marlon runs
```
```python
8.48
```
```python
seconds
```
```python
.
Task Frank runs
```
```python
15.68
```
```python
seconds
```
```python
.
Task Allen runs
```
```python
18.08
```
```python
seconds
```
```python
.
All subprocesses done.
```
[multiprocessing pool map](https://dotblogs.com.tw/rickyteng/archive/2012/02/20/69635.aspx)
```python
#coding: utf-8
```
```python
import
```
```python
multiprocessing
```
```python
def
```
```python
m1
```
```python
(x)
```
```python
:
```
```python
print
```
```python
x * x
```
```python
if
```
```python
__name__ ==
```
```python
'__main__'
```
```python
: 
    pool = multiprocessing.Pool(multiprocessing.cpu_count()) 
    i_list = range(
```
```python
8
```
```python
)
    pool.map(m1, i_list)
```
一次执行结果
```python
0
1
4
9
16
25
36
49
```
参考：[http://www.dotblogs.com.tw/rickyteng/archive/2012/02/20/69635.aspx](http://www.dotblogs.com.tw/rickyteng/archive/2012/02/20/69635.aspx)
问题：[http://bbs.chinaunix.net/thread-4111379-1-1.html](http://bbs.chinaunix.net/thread-4111379-1-1.html)
```python
#coding: utf-8
```
```python
import
```
```python
multiprocessing
```
```python
import
```
```python
logging
```
```python
def
```
```python
create_logger
```
```python
(i)
```
```python
:
```
```python
print
```
```python
i
```
```python
class
```
```python
CreateLogger
```
```python
(object)
```
```python
:
```
```python
def
```
```python
__init__
```
```python
(self, func)
```
```python
:
```
```python
self.func = func
```
```python
if
```
```python
__name__ ==
```
```python
'__main__'
```
```python
:
    ilist = range(
```
```python
10
```
```python
)
    cl = CreateLogger(create_logger)
    pool = multiprocessing.Pool(multiprocessing.cpu_count())
    pool.map(cl.func, ilist)
```
```python
print
```
```python
"hello------------>"
```
一次执行结果
```python
0
```
```python
1
```
```python
2
```
```python
3
```
```python
4
```
```python
5
```
```python
6
```
```python
7
```
```python
8
```
```python
9
```
```python
hello
```
```python
-
```
```python
-
```
```python
-
```
```python
-
```
```python
-
```
```python
-
```
```python
-
```
```python
-
```
```python
-
```
```python
-
```
```python
-
```
```python
-
```
```python
>
```
**评论：**
2楼 2016-12-29 14:23 Last_Stardust
您好，我测试了一下
进程池在旧进程的任务执行完毕后，不会创建新的子进程，是刚刚空闲出来的进程去执行新的任务，进程池中的各进程pid是不变的

