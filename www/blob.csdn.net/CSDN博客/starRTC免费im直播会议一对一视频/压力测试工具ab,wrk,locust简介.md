# 压力测试工具ab,wrk,locust简介 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月02日 18:41:41[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1681
[ab](http://httpd.apache.org/docs/current/programs/ab.html) 无疑是目前最常见的压力测试工具。其典型用法如下：
shell> ab -k -c 100 -t 10 http://domain/path
其中，参数「c」表示的是并发，
参数「t」表示的是整个测试持续的时间。
一个很容易被忽视的参数是「k」，它会增加请求头 Connection: Keep-Alive，相当于开启了
 HTTP 长连接，这样做一方面可以降低测试服务器动态端口被耗尽的风险，另一方面也有助于给目标服务器更大的压力，测试出更接近极限的结果。
**再来说说 wrk：**
[wrk](https://github.com/wg/wrk) 相对于
 ab 来说最大的优点是它支持多线程，这样更容易发挥多核 CPU 的能力，从而更容易测试出系统的极限能力，其典型用法如下：
shell> wrk -c 100 -d 10 http://domain/path
其中，参数「c」表示的是并发，参数「d」表示的是整个测试持续的时间。此外，可以通过参数「t」来控制线程数（缺省值为
 2），实际使用中可以视情况酌情增大。
如果顺着 ab 的使用惯性，你可能会纳闷为什么 wrk 没有类似参数「k」这样打开 HTTP 长链接的选项，这是因为 wrk 使用的是
 HTTP/1.1，缺省开启的是长连接，而 ab 使用的是 HTTP/1.0，缺省开启的是短链接。
不过这也引出另一个问题，如何用 wrk 测试短链接？实际上很简单：
shell> wrk -H "Connection: Close" -c 100 -d 10 http://domain/path
也就是说通过参数「H」传递一个自定义的 Connection 请求头来关闭长链接。此外，wrk 支持自定义脚本编程，不过相对较复杂，本文略过不谈。
**最后说说 locust：**
[locust](http://locust.io/) 相对于
 ab、wrk 来说最大的优点是它不再只是测试一个 url，而是可以测试一个自定义的场景，其中可以包含多个有相互关联的 url，此外，它还是分布式可扩展的，可以模拟大量用户的访问，由此可以得到更贴近真实环境的测试数据。
关于 locust 的具体用法有点复杂，大家可以自行查阅[官方文档](http://docs.locust.io/en/latest/)。
**其它**
除了 ab，wrk，locust 等压力测试工具之外，还有很多其它选择，其中比较有代表性的是 [GoReplay](https://github.com/buger/goreplay) 和 [TcpCopy](https://github.com/session-replay-tools/tcpcopy)，它们的特殊之处在于可以拷贝线上服务器的真实流量并转发到测试服务器上去，也就是说，用真实流量来测试，如此得到的数据无疑是更可信的。不过它们的用法更复杂，大家记得有类似需求的时候去哪找就行了。
…
说了这么多压力测试工具，结尾我想说说压力测试过程中一些注意事项：首先，在使用
 ab 或者 wrk 的时候，最好在同一个局域网里独立的服务器上运行它们，因为这些压力测试工具本身也会消耗系统资源，所以如果直接在目标服务器上运行它们，那么测试的结果会有偏差；其次，即便使用了长连接，那么也应该注意端口
TIME_WAIT
问题，视情况可能需要在 sysctl 配置中开启 tcp_tw_recycle 或 tcp_tw_reuse 之类的选项；
最后，在压力测试过程中，最好时刻留意哪些资源成为了瓶颈，比如：CPU 是不是跑满了，IO
 是不是跑满了，带宽是不是跑满了等等，如果什么都没跑满，但是压力测试结果却上不去，那么就需要反思一下是不是压力测试工具使用方法有误，或者是不是在 off-CPU 上消耗了太多时间，只有知道了瓶颈在哪，才能突破系统的极限。
参考：
[https://huoding.com/2017/05/31/620?utm_source=tuicool&utm_medium=referral](https://huoding.com/2017/05/31/620?utm_source=tuicool&utm_medium=referral)
