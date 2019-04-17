# Storm如何保证消息的可靠处理 - zkq_1986的博客 - CSDN博客





2017年05月03日 14:19:22[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：163










**Storm如何保证消息的可靠处理**



Spout通过ack/ fail机制，处理bolt失败或成功的tuple。通常bolt通过output Collecter抛出fail或当tuple超时未处理，都会激发Spout的fail功能，在fail函数上可设置具体的操作，比如重发tuple。





