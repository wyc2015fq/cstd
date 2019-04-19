# TCP - 左直拳的马桶_日用桶 - CSDN博客
2018年05月10日 19:41:31[左直拳](https://me.csdn.net/leftfist)阅读数：166
个人分类：[学习笔记](https://blog.csdn.net/leftfist/article/category/1790155)
学习阮一峰多年前的学习笔记[《TCP 协议简介》](http://www.ruanyifeng.com/blog/2017/06/tcp-protocol.html)，受益匪浅。
学习笔记的学习笔记。嗯。
![这里写图片描述](https://img-blog.csdn.net/20180510190248991?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
以太网协议（Ethernet）规定了电子信号如何组成数据包（packet），解决了子网内部的点对点通信。
但是，以太网协议不能解决多个局域网如何互通，这由 IP 协议解决。
IP 协议只是一个地址协议，并不保证数据包的完整。如果路由器丢包（比如缓存满了，新进来的数据包就会丢失），就需要发现丢了哪一个包，以及如何重新发送这个包。这就要依靠 TCP 协议。
![这里写图片描述](https://img-blog.csdn.net/20180510192548185?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所谓的网络协议，就是个软件的概念，在发送端，由应用层向下，表示层、会话层、传输层、网络层、数据链路层、物理层，将数据处理，分包，加上自己的标头，层层加码；到达接收端后，则由物理层向应用层而上，层层剥离、读取属于自己的标头，组装，最后还原出原始完整的信息。
我读书的时候，根本没有理解，被“物理层”这样的字眼误导，以为这个网络分层也包括硬件。其实就是一个纯粹的软件概念。
一晃十多年过去了，至今还在做读书笔记。
2018.06.25 
网络分层是软件概念，不包括硬件，这种说法也不准确。应该说，网络分的层，是个逻辑概念，里面可以包括硬件、协议。
