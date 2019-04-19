# twisted实现udp广播聊天 - Arthur的随笔 - CSDN博客
2012年04月11日 00:09:52[largetalk](https://me.csdn.net/largetalk)阅读数：2576
刚翻看twisted udp， 看到udp广播那块，就想用这个做局域网聊天很简单得，然后查了下twisted 线程怎么弄， 掺和在一起，没想到能运行， nice
```python
from twisted.internet.protocol import DatagramProtocol
from twisted.internet import reactor
import os
class MulticastPingPong(DatagramProtocol):
    def startProtocol(self):
        self.transport.setTTL(5)
        self.transport.joinGroup("228.0.0.5")
    def datagramReceived(self, datagram, address):
        print "%s"%repr(datagram)
def talk():
    import socket
    pid = os.getpid()
    while True:
        data = raw_input()
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
        sock.sendto("%s say:%s"%(pid, data), ("228.0.0.5", 8005))
reactor.listenMulticast(8005, MulticastPingPong(), listenMultiple=True)
reactor.callInThread(talk)
reactor.run()
```
