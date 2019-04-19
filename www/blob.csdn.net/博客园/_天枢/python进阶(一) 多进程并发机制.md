# python进阶(一) 多进程并发机制 - _天枢 - 博客园
## [python进阶(一) 多进程并发机制](https://www.cnblogs.com/yhleng/p/8334042.html)
2018-01-23 09:41 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8334042)
python多进程并发机制：
这里使用了multprocessing.Pool进程池，来动态增加进程
```
#coding=utf-8
from multiprocessing import Pool
import time
def ft(x):
  #多任务，系统自动化配进程执行
  for i in range(2):
    print i,'-----------',x
    time.sleep(1)
def main_process():
    pool = Pool(processes=4) #控制进程池的大小，为4个进程
    for i in range(10):
        #添加入进程池，apply带_async，单独apply为阻塞版本；函数名target，参数args
        result = pool.apply_async(ft,(i,))
    pool.close()
    pool.join()
    if result.successful():
        print('_____success_____')
if __name__=="__main__":
    main_process()
```
运行结果：
```
C:\Python27\python.exe D:/weixin/temp/testtmp.py
0 ----------- 0
0 ----------- 1
0 ----------- 2
0 ----------- 3
1 ----------- 0
1 ----------- 1
1 ----------- 2
1 ----------- 3
0 ----------- 4
0 ----------- 5
0 ----------- 6
0 ----------- 7
1 ----------- 4
1 ----------- 5
1 ----------- 6
1 ----------- 7
0 ----------- 8
0 ----------- 9
1 ----------- 8
1 ----------- 9
_____success_____
Process finished with exit code 0
```
从以上运行结果可以看出，一次最多执行了我们设定的4个进程。
**Linux and python学习交流1,2群已满.**
**Linux and python学习交流3群新开,欢迎加入,一起学习.qq 3群:563227894**
不前进,不倒退,停止的状态是没有的.
一起进步,与君共勉,
