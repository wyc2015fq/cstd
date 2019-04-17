# PyQt实现的生产者消费者模型 - alw2009的博客 - CSDN博客





2016年09月16日 17:23:23[traveler_zero](https://me.csdn.net/alw2009)阅读数：216








```python
from PyQt5.QtCore import QCoreApplication, QMutex, QThread, QWaitCondition

DataSize = 1000000
BufferSize = 8192
buffer = ['0' for x in range(BufferSize)]

bufferNotEmpty = QWaitCondition()
bufferNotFull = QWaitCondition()
mutex = QMutex()

numUsedBytes = 0


class Producer(QThread):
    def __init__(self):
        super().__init__()

    def run(self):
        global numUsedBytes
        for i in range(DataSize):
            mutex.lock()
            if numUsedBytes == BufferSize:
                bufferNotFull.wait(mutex)
            mutex.unlock()

            print("produce ", i)
            buffer[i % BufferSize] = i

            mutex.lock()
            numUsedBytes += 1
            bufferNotEmpty.wakeAll()
            mutex.unlock()


class Consumer(QThread):
    def __init__(self):
        super().__init__()

    def run(self):
        global numUsedBytes
        for i in range(DataSize):
            mutex.lock()
            if numUsedBytes == 0:
                bufferNotEmpty.wait(mutex)
            mutex.unlock()

            print(buffer[i % BufferSize])

            mutex.lock()
            numUsedBytes -= 1
            bufferNotFull.wakeAll()
            mutex.unlock()

if __name__ == '__main__':
    import sys
    app = QCoreApplication(sys.argv)
    producer = Producer()
    consumer = Consumer()
    producer.start()
    consumer.start()
    producer.wait()
    consumer.wait()
    print("hello, finished")
    sys.exit(app.exec_())
```




