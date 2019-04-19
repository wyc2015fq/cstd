# ISO15693协议的Inventory - xqhrs232的专栏 - CSDN博客
2015年04月23日 09:43:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2200
原文地址::[http://blog.sina.com.cn/s/blog_a20257d80101cjua.html](http://blog.sina.com.cn/s/blog_a20257d80101cjua.html)
相关文章
1、ISO15693百科----[http://www.baike.com/wiki/ISO15693](http://www.baike.com/wiki/ISO15693)
2、ISO15693协议----[http://download.csdn.net/download/layzerlee/2072899](http://download.csdn.net/download/layzerlee/2072899)
3、ISO15693协议中，数据编码采用脉冲位置调制。两种数据编码模式：256选1模式和4选1模式。256选1模式啥意思----[http://zhidao.baidu.com/link?url=BL2vRNMWap6AFJcmsFCEKgHMkXTbEHJiiDrOzIxf-eKqGVUok_I4Rkro6T2C3sPW440tGq83VPEVdbOdc0UAcK](http://zhidao.baidu.com/link?url=BL2vRNMWap6AFJcmsFCEKgHMkXTbEHJiiDrOzIxf-eKqGVUok_I4Rkro6T2C3sPW440tGq83VPEVdbOdc0UAcK)

ISO15693的Inventory指令看起来比较让人迷糊，想明白其流程，我认为以下几点是必须了解的：
第一。ISO15693标签的几种状态，资料上有，这里罗嗦重复一下：
      1.PowerOff状态
      2.就绪态
      3.静默态
      4.选中态
      其中选中态是可选的，而前三种是必须支持的。
第二。卡片操作的几个概念：
      1.寻址模式，所谓寻址模式，就是读卡设备每次操作卡片前必须带上该卡的UID(不要问我UID是什么，我会
        拍死你的）。  这就好比老师再给学生上课，老师喊起立，大家都站起来，如果老师喊某某某起立，那么
        其他人就不用站起来，只需要某某某站起来，读卡设备就像老师，而UID就像每个学生的学号。
第三。指令域
       FLAG域 + Command域 + Parameter域 + Data域 + CRC校验域（这个一般不用软件处理，目前市场上的读卡设
        备其硬件都支持自动添加校验，用户只需要将初值配好即可）
防冲突的过程：
       1.发送Inventory命令，这时FLAG域的Address_flag位应设置为0，选slot16模式。
       2.没有接收到response时，发送EOF命令，切换到下一个slot。收到响应时，判断CRC校验位和冲突检测寄
        存器的值，同时出现CRC错误和冲突检测寄存器的值不为0时表示有冲突，如果冲突检测寄存器的值为0而CRC
        不为0可以断定为校验错误，去调调天线吧！
       3.由于卡片回送UID时是LSB优先回送，所以冲突寄存器的值一定是冲突发生的位（从右向左数)，冲突寄存器
         的值就是MaskLength的值，冲突的位，有两种配置方法，每种配置方法其实都殊路同归，如果将冲突的位配
         置为1，那么防冲突首先选出的卡号将是所有参与防冲突指令的卡片中卡号发最大的，反之则是最小的，无
         论哪种都不影响最终结果。MaskValue的值就是从有至左一直到冲突位截至的字节数，不足一个字节的全部补0.
       4.重新发送Inventory命令，这次发送时需要将FLAG域的Address_flag置位1。即告诉VICC，与MaskValue值中
         由MaskLength确定的有效位相同的VICC回答，有冲突时继续。
       5.没有冲突时发送EOF并切换到下一个slot。直至slot16.
