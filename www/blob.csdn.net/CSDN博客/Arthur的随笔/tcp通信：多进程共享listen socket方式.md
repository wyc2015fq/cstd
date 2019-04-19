# tcp通信：多进程共享listen socket方式 - Arthur的随笔 - CSDN博客
2012年09月03日 17:59:20[largetalk](https://me.csdn.net/largetalk)阅读数：7006
看tornado源码多进程(process.py)那段，发现他的多进程模型和一般常见的模型有点不一样，多见的是主进程bind-> listen -> accept, 将accept返回的socket用子进程处理，而tornado是在bind -> listen -> fork, 在listen之后fork，多个子进程共享listen socket， 每个子进程都accept。 以前没见过这种模型，开始觉的会有 错误或"惊群“现象的发生，网上找了一下也没有找到相关资料，自己就写了个程序测试一下。
```python
import socket
import select
import os
import time
import errno
def child_epoll():
    epoll = select.epoll()
    epoll.register(sock.fileno(), select.EPOLLIN)
    try:
        while True:
            events = epoll.poll(1)
            for fileno, event in events:
                if fileno == sock.fileno():
                    connection, address = sock.accept()
                    print os.getpid(), address
                    time.sleep(1)
                    connection.close()
    finally:
        epoll.unregister(sock.fileno())
        epoll.close()
def start_child(i):
    pid = os.fork()
    if pid == 0:
        child_epoll()
    else:
        children[pid] = i
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.setblocking(0)
sock.bind(("0.0.0.0", 9700))
sock.listen(128)
children = {}
for i in range(4):
    start_child(i)
time.sleep(2)
print children
while children:
    try:
        pid = status = os.wait()
    except OSError, e:
        if e.errno ==  errno.EINTR:
            continue
        raise
    if pid not in children:
        continue
    children.pop(pid)
sys.exit(0)
```
然后用ab简单测测： ab -n 10 -c 5 http://127.0.0.1:9700/
可以发现每个连接都会只有一个进程去处理。虽然逻辑上没有错误，但还是要找相关资料看看，有知道的请告诉我一下。
后补：（20121101）
[http://blog.dccmx.com/2011/02/nginx-conn-handling/](http://blog.dccmx.com/2011/02/nginx-conn-handling/) 介绍nginx多进程模型就是如此工作，nginx使用一个全局锁来避免惊群
[http://static.usenix.org/event/usenix2000/freenix/full_papers/molloy/molloy.pdf](http://static.usenix.org/event/usenix2000/freenix/full_papers/molloy/molloy.pdf) 该论文说linux 2.6内核之后accept不会有惊群现象
