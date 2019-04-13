
# 在Python中使用异步Socket编程性能测试 - jiahaowanhao的博客 - CSDN博客


2018年07月10日 21:09:17[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：594


[在Python中使用异步Socket编程性能测试](http://cda.pinggu.org/view/26054.html)
异步网络据说能极大的提高网络server的连接速度,所以打算写一个专题,来学习和了解异步网络.因为Python有个非常出名的异步Lib:Twisted,所以就用Python来完成.
OK,首先写一个python socket的server段,对开放三个端口:10000,10001,10002.krondo的例子中是每个server绑定一个端口,测试的时候需要分别开3个shell,分别运行.这太麻烦了,就分别用三个Thread来运行这些services.
import optparse
import os
import socket
import time
from threading import Thread
import StringIO
txt = '''1111
2222
3333
4444
'''
def server(listen_socket):
while True:
buf = StringIO.StringIO(txt)
sock, addr = listen_socket.accept()
print 'Somebody at %s wants poetry!' % (addr,)
while True:
try:
line = buf.readline().strip()
if not line:
sock.close()
break
sock.sendall(line) \# this is a blocking call
print 'send bytes to client:%s' % line
\#sock.close()
except socket.error:
sock.close()
break
time.sleep(1) \#server和client连接后，server会故意每发送一个单词后等待一秒钟后再发送另一个单词
def main():
ports = [10000, 10001, 10002]
for port in ports:
listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
listen_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
addres = (str('127.0.0.1'), port)
listen_socket.bind(addres)
listen_socket.listen(5)
print "start listen at:%s" % (port,)
worker = Thread(target = server, args = [listen_socket])
worker.setDaemon(True)
worker.start()
if __name__ == '__main__':
main()
while True:
time.sleep(0.1) \#如果不sleep的话,CPU会被Python完全占用了
pass
下面是一个client,没有才用异步网络,连接这个三个端口的server:
import socket

if __name__ == '__main__':
ports = [10000, 10001, 10002]
for port in ports:
address = (str('127.0.0.1'), port)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(address)
poem = ''
while True:
data = sock.recv(4)
if not data:
sock.close()
break
poem += data
print poem
下面用异步的client来读取，代码如下：
import datetime, errno, optparse, select, socket
def connect(port):
"""Connect to the given server and return a non-blocking socket."""
address = (str('127.0.0.1'), port)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(address)
sock.setblocking(0)
return sock
def format_address(address):
host, port = address
return '%s:%s' % (host or '127.0.0.1', port)
if __name__ == '__main__':
ports = [10000, 10001, 10002]
start = datetime.datetime.now()
sockets = map(connect, ports)
poems = dict.fromkeys(sockets, '') \# socket -> accumulated poem
\# socket -> task numbers
sock2task = dict([(s, i + 1) for i, s in enumerate(sockets)])
sockets = list(sockets) \# make a copy
while sockets:
\#运用select来确保那些可读取的异步socket可以立即开始读取IO
\#OS不停的搜索目前可以read的socket，有的话就返回rlist
rlist, _, _ = select.select(sockets, [], [])
for sock in rlist:
data = ''
while True:
try:
new_data = sock.recv(1024)
except socket.error, e:
if e.args[0] == errno.EWOULDBLOCK:
break
raise
else:
if not new_data:
break
else:
print new_data
data += new_data
task_num = sock2task[sock]
if not data:
sockets.remove(sock)
sock.close()
print 'Task %d finished' % task_num
else:
addr_fmt = format_address(sock.getpeername())
msg = 'Task %d: got %d bytes of poetry from %s'
print msg % (task_num, len(data), addr_fmt)
poems[sock] += data
elapsed = datetime.datetime.now() - start
print 'Got poems in %s' % elapsed
结果只需要4秒就完成了读取任务。效率是刚才同步socket的三倍。对客户端的异步改造主要有两点：
同步模式下，客户端分别创建socket；而在异步模式下，client开始就创建了所有的socket。
通过“sock.setblocking(0)”设置socket为异步模式。
通过Unix系统的select俩返回可读取IO
最为核心的是26行和29行。尤其是29行的select操作返回待读取socket的列表。

