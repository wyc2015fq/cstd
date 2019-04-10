# ARQ

自动重传请求（Automatic Repeat-reQuest，ARQ）是OSI模型中数据链路层的错误纠正协议之一。它包括停止等待ARQ协议和连续ARQ协议，错误侦测（Error Detection）、正面确认（Positive Acknowledgment）、逾时重传（Retransmission after Timeout）与负面确认继以重传（Negative Acknowledgment and Retransmission）等[机制](https://baike.baidu.com/item/%E6%9C%BA%E5%88%B6/1433787)。



- 外文名

  ARQ

- 协议称为

  [PAR](https://baike.baidu.com/item/PAR)

- 传    统

  [自动重传请求](https://baike.baidu.com/item/%E8%87%AA%E5%8A%A8%E9%87%8D%E4%BC%A0%E8%AF%B7%E6%B1%82)

- 优    点

  比较简单

## 目录

1. 1 [定义](https://baike.baidu.com/item/ARQ/7402812?fr=aladdin#1)
2. 2 [分类](https://baike.baidu.com/item/ARQ/7402812?fr=aladdin#2)
3. ▪ [停等式ARQ](https://baike.baidu.com/item/ARQ/7402812?fr=aladdin#2_1)

1. ▪ [回退n帧的ARQ](https://baike.baidu.com/item/ARQ/7402812?fr=aladdin#2_2)
2. ▪ [选择性重传ARQ](https://baike.baidu.com/item/ARQ/7402812?fr=aladdin#2_3)
3. ▪ [混合ARQ](https://baike.baidu.com/item/ARQ/7402812?fr=aladdin#2_4)
4. 3 [应用](https://baike.baidu.com/item/ARQ/7402812?fr=aladdin#3)

1. 4 [优缺点](https://baike.baidu.com/item/ARQ/7402812?fr=aladdin#4)



## 定义

编辑

如果在协议中，发送方在准备下一个数据项目之前先等待一个肯定的确认，则这样的协议称为[PAR](https://baike.baidu.com/item/PAR)（Positive Acknowledgement with Retransmission，支持重传的肯定确认协议）或者ARQ（Automatic Repeat Request，自动重复请求协议）。

[自动重传请求](https://baike.baidu.com/item/%E8%87%AA%E5%8A%A8%E9%87%8D%E4%BC%A0%E8%AF%B7%E6%B1%82)（Automatic Repeat Request），通过接收方请求发送方重传出错的数据[报文](https://baike.baidu.com/item/%E6%8A%A5%E6%96%87)来恢复出错的报文，是通信中用于处理信道所带来差错的方法之一，有时也被称为后向纠错（Backward Error Correction，[BEC](https://baike.baidu.com/item/BEC)）；另外一个方法是[信道](https://baike.baidu.com/item/%E4%BF%A1%E9%81%93)纠错编码。



## 分类

编辑

传统[自动重传请求](https://baike.baidu.com/item/%E8%87%AA%E5%8A%A8%E9%87%8D%E4%BC%A0%E8%AF%B7%E6%B1%82)分成为三种，即停等式(stop-and-wait）ARQ，回退n帧（go-back-n）ARQ，以及选择性重传（selective repeat）ARQ。后两种协议是[滑动窗口](https://baike.baidu.com/item/%E6%BB%91%E5%8A%A8%E7%AA%97%E5%8F%A3)技术与请求重发技术的结合，由于窗口尺寸开到足够大时，帧在线路上可以连续地流动，因此又称其为[连续ARQ协议](https://baike.baidu.com/item/%E8%BF%9E%E7%BB%ADARQ%E5%8D%8F%E8%AE%AE)。三者的区别在于对于出错的数据[报文](https://baike.baidu.com/item/%E6%8A%A5%E6%96%87)的处理机制不同。三种ARQ协议中，复杂性递增，效率也递增。除了传统的ARQ，还有混合ARQ（Hybrid-ARQ）。



### 停等式ARQ

在停等式ARQ中，数据[报文](https://baike.baidu.com/item/%E6%8A%A5%E6%96%87)发送完成之后，发送方等待接收方的状态报告，如果状态报告报文发送成功，发送后续的数据报文，否则重传该报文。

停等式ARQ，[发送窗口](https://baike.baidu.com/item/%E5%8F%91%E9%80%81%E7%AA%97%E5%8F%A3)和接收窗口大小均为1，发送方每发送一帧之后就必须停下来等待接收方的确认返回，仅当接收方确认正确接收后再继续发送下一帧。该方法所需要的缓冲存储空间最小，缺点是信道效率很低。



### 回退n帧的ARQ

发信侧不用等待收信侧的应答，持续的发送多个帧，假如发现已发送的帧中有错误发生，那么从那个发生错误的帧开始及其之后所有的帧全部再重新发送。

特点：（GBN）复杂度低，但是不必要的帧会再重发，所以大幅度范围内使用的话效率是不高的

例：如果序列号有K bits，那么这个ARQ的协议大小为：2^k-1。



### 选择性重传ARQ

发信侧不用等待收信侧的应答，持续的发送多个帧，假如发现已发送的帧中有错误发生，那么发信侧将只重新发送那个发生错误的帧。

特点：相对于GBN 复杂度高，但是不需要发送没必要的帧，所以效率高。

例：如果序列号有K bits，那么这个ARQ的协议大小为：2^(k-1)。



### 混合ARQ

在混合ARQ中，数据报文传送到接收方之后，即使出错也不会被丢弃。接收方指示发送方重传出错[报文](https://baike.baidu.com/item/%E6%8A%A5%E6%96%87)的部分或者全部信息，将再次收到的报文信息与上次收到的报文信息进行合并，以恢复报文信息。



## 应用

编辑

在现代的无线通信中，ARQ主要应用在无线链路层。比如，在WCDMA和cdma2000无线通信中都采用了选择性重传ARQ和混合ARQ。



## 优缺点

编辑

优点：比较简单 。因而被广泛的应用在[分组交换](https://baike.baidu.com/item/%E5%88%86%E7%BB%84%E4%BA%A4%E6%8D%A2)网络中。

缺点：1.通信信道的利用率不高，也就是说，信道还远远没有被数据比特填满。2.是需要接收方发送ACK，这样增加了网络的负担也影响了传输速度。重复发送[数据包](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E5%8C%85)来纠正错误的方法也严重的影响了它的传输速度。