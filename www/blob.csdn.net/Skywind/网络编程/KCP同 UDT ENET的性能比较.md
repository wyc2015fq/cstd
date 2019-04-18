# Skywind Inside » KCP同 UDT/ENET的性能比较
## KCP同 UDT/ENET的性能比较
February 18th, 2016[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
如果不丢包那么 KCP（[https://github.com/skywind3000/kcp](https://github.com/skywind3000/kcp)）和 TCP性能差不多，KCP不会有任何优势，但是网络会卡，造成卡的原因就是丢包和抖动，有同学在内网这样好的环境下没有用任何丢包模拟直接跑，跑出来的数据是差不多的，但是放到公网上，放到3G/4G网络情况下，差距就很明显了，公网在高峰期有平均接近10%的丢包，wifi/3g/4g下更糟糕，这正是造成各种网络卡顿的元凶。
感谢 [asio-kcp](https://github.com/libinzhangyuan/asio_kcp) 的作者 zhangyuan 对 KCP 与 enet, udt做过的一次横向评测，结论如下：
- ASIO-KCP has good performace in wifi and phone network(3G, 4G).
- Extra using 20% ~ 50% network flow for speed improvement.
- The kcp is the first choice for realtime pvp game.
- The lag is less than 1 second when network lag happen. 3 times better than enet when lag happen.
- The enet is a good choice if your game allow 2 second lag.
- UDT is a bad idea. It always sink into badly situation of more than serval seconds lag. And the recovery is not expected.
- enet has the problem of lack of doc. And it has lots of functions that you may intrest. kcp’s doc is chinese. Good thing is the function detail which is writen in code is english. And you can use asio_kcp which is a good wrap.
- The kcp is a simple thing. You will write more code if you want more feature.
- UDT has a perfect doc. UDT may has more bug than others as I feeling.
具体见：[横向比较](https://github.com/skywind3000/kcp/wiki/KCP-Benchmark) 和 [这里](https://github.com/skywind3000/kcp/wiki/KCP-Benchmark)。截取一段在网络糟糕时，asio-kcp/enet的延迟数据：
> 
worst network lag happen:     
asio: 10:51.21      
291  295   269   268   231   195   249   230   225   204
enet: 10:51.21     
1563   1520    1470    1482    1438    1454    1412    1637    1588    1540
我当年主要测试了 KCP和 TCP/UDT的比较，扫了一眼 libenet觉得协议实现中规中矩，缺乏很多现代传输协议的技术，所以并没有详细测试。而 asio-kcp的作者同时给出了KCP/enet/udt三者的详细比较，为更多犹豫选择使用那一套的人提供了更多指引。
