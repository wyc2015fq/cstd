# Python多线程之：threading模块 - alw2009的博客 - CSDN博客





2017年12月09日 15:33:51[traveler_zero](https://me.csdn.net/alw2009)阅读数：92








废话少说，干货送上：




例子1：如何用threading模块创建一个简单的线程



```python
import threading
 import time
 
 class ReadBook(threading.Thread):
     def run(self):
         print "Start reading book.."
         time.sleep(1)
         print "finished"
 
 readbook = ReadBook()
 readbook.start()
 
 while(1):
     pass
```


例子2：如何传参到线程？



```python
import threading
 import time
 import random
 
 class Worker(threading.Thread):
 
     def __init__(self, ID, name, work):
         threading.Thread.__init__(self)
         self.id = ID
         self.name = name
         self.work = work
 
     def run(self):
         print "ID:", self.id
         print "Name:", self.name
         print "start", self.work
         time.sleep(random.randint(2, 10))
         print "finish", self.work
 
 cook = Worker(1, "cooker", "cooking")
 drive = Worker(2, "driver", "driving")
 
 cook.start()
 drive.start()
 
 cook.join()
```

```python
drive.join()
```





