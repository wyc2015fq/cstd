
# python使用socket向客户端发送数据的方法​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年08月04日 20:02:23[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：137


**[python使用socket向客户端发送数据的方法](http://cda.pinggu.org/view/26282.html)**
这篇文章主要介绍了python使用socket向客户端发送数据的方法,涉及Python使用socket实现数据通信的技巧,非常具有实用价值,需要的朋友可以参考下
import socket, sys
port = 55555
host = 'localhost'
data = "test" * 10485760 \# 40MB of data
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))
byteswritten = 0
while byteswritten < len(data):
startpos = byteswritten
endpos = min(byteswritten + 1024, len(data))
byteswritten += s.send(data[startpos:endpos])
sys.stdout.write("Wrote %d bytes\r" % byteswritten)
sys.stdout.flush()
s.shutdown(1)
print "All data sent."
while 1:
buf = s.recv(1024)
if not len(buf):
break
sys.stdout.write(buf)
希望本文所述对大家的Python程序设计有所帮助。

