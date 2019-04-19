# python进阶(二) 多进程+协程 - _天枢 - 博客园
## [python进阶(二) 多进程+协程](https://www.cnblogs.com/yhleng/p/8336227.html)
2018-01-23 15:52 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8336227)
　　　　我们大多数的时候使用多线程，以及多进程，但是python中由于GIL全局解释器锁的原因，python的多线程并没有真的实现
      实际上，python在执行多线程的时候，是通过GIL锁，进行上下文切换线程执行，每次真实只有一个线程在运行。所以上边才说，没有真的实现多现程。
      那么python的多线程就没有什么用了吗？
              不是这个样子的，python多线程一般用于IO密集型的程序，那么什么叫做IO密集型呢，举个例子，比如说带有阻塞的。当前线程阻塞等待其它线程执行。
      即然说到适合python多线程的，那么什么样的不适合用python多线程呢？
              答案是CPU密集型的，那么什么样的是CPU密集型的呢？百度一下你就知道。
       现在有这样一项任务：需要从200W个url中获取数据？
       那么我们真心不能用多线程，上下文切换是需要时间的，数据量太大，无法接受。这里我们就要用到多进程+协程
      那么什么是协程呢？
      协程，又称微线程，纤程。英文名Coroutine。
      协程的概念很早就提出来了，但直到最近几年才在某些语言（如Lua）中得到广泛应用。
      协程有什么好处呢，协程只在单线程中执行，不需要cpu进行上下文切换,协程自动完成子程序切换。
      这里没有使用yield协程，这个python自带的并不是很完善，至于为什么有待于你去研究了。
      这里使用比较完善的第三方协程包gevent
      pip  install    gevent
每个进程下N个协程，    
```
#coding=utf-8
from multiprocessing import Process
import gevent
#from gevent import monkey; monkey.patch_socket()
#用于协程的了程序
def yield_execFunc(x):
    print('______________%s'%x)
#yield_clist决定协程的数量
#开始协程操作
def yield_start(yield_clist):
    task=[] #用来存储协程
    for i in yield_clist:
        task.append(gevent.spawn(yield_execFunc,i))
    gevent.joinall(task) #执行协程
if  __name__=="__main__":
    list1=[1,2,3,4,5,6,7,8,9,10] #元素个数决定开起的协程数量
    list2=[1,2,3,4,5,6,7,8,9,10]
    list3=[1,2,3,4,5,6,7,8,9,10]
    process_list =[list1,list2,list3] #元素个数决定进程数量
    for plist in process_list:
        p = Process(target=yield_start,args=(plist,))
        p.start()
```
执行结果：开了三个进程，每个进程下执行10个协程协作任务
```
C:\Python27\python.exe D:/weixin/temp/yield_tmp.py
______________1
______________2
______________3
______________4
______________5
______________6
______________7
______________8
______________9
______________10
______________1
______________1
______________2
______________2
______________3
______________3
______________4
______________4
______________5
______________5
______________6
______________6
______________7
______________7
______________8
______________8
______________9
______________9
______________10
______________10
Process finished with exit code 0
```
