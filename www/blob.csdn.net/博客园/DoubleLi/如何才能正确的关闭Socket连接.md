# 如何才能正确的关闭Socket连接 - DoubleLi - 博客园






从[TCP协议](https://www.baidu.com/s?wd=TCP%E5%8D%8F%E8%AE%AE&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YknyfsrHf3Pj7WmhR4PW6L0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4rjR4PWbdn1D4rHc1P1RLrf)角度来看，一个已建立的TCP连接有两种关闭方式，一种是正常关闭，即四次挥手关闭连接；还有一种则是异常关闭，我们通常称之为连接重置（RESET)。
首先说一下正常关闭时四次挥手的状态变迁，关闭连接的主动方状态变迁是FIN_WAIT_1->FIN_WAIT_2->TIME_WAIT，而关闭连接的被动方的状态变迁是CLOSE_WAIT->LAST_ACK->TIME_WAIT。在四次挥手过程中ACK包都是协议栈自动完成的，而FIN包则必须由应用层通过closesocket或shutdown主动发送，通常连接正常关闭后，recv会得到返回值0，send会得到错误码10058。
除此之外，在我们的日常应用中，连接异常关闭的情况也很多。比如应用程序被强行关闭、本地网络突然中断(禁用网卡、网线拔出)、程序处理不当等都会导致连接重置，连接重置时将会产生RST包，同时网络络缓冲区中未接收（发送）的数据都将丢失。连接重置后，本方send或recv会得到错误码10053(closesocket时是10038)，对方recv会得到错误码10054，send则得到错误码10053(closesocket时是10054)。
[操作系统](https://www.baidu.com/s?wd=%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YknyfsrHf3Pj7WmhR4PW6L0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4rjR4PWbdn1D4rHc1P1RLrf)为我们提供了两个函数来关闭一个TCP连接，分别是closesocket和shutdown。通常情况下，closesocket会向对方发送一个FIN包，但是也有例外。比如有一个工作线程正在调用recv接收数据，此时外部调用closesocket，会导致连接重置，同时向对方发送一个RST包，这个RST包是由本方主动产生的。
shutdown可以用来关闭指定方向的连接，该函数接收两个参数，一个是套接字，另一个是关闭的方向，可用值为SD_SEND,SD_RECEIVE和SD_BOTH。方向取值为SD_SEND时，无论socket处于什么状态(recv阻塞，或空闲状态)，都会向对方发送一个FIN包，注意这点与closesocket的区别。此时本方进入FIN_WAIT_2状态，对方进入CLOSE_WAIT状态，本方依然可以调用recv接收数据；方向取值为SD_RECEIVE时，双发连接状态没有改变，依然处于ESTABLISHED状态，本方依然可以send数据，但是，如果对方再调用send方法，连接会被立即重置，同时向对方发送一个RST包，这个RST包是被动产生的，这点注意与closesocket的区别。









