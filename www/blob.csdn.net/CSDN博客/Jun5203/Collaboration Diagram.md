# Collaboration Diagram - Jun5203 - CSDN博客
2018年10月06日 15:21:28[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：196
所属专栏：[UML](https://blog.csdn.net/column/details/26918.html)
### 前言
协作图是动态图的另一种表现形式，它强调参加交互的各对象的组织。
### 芝士
- 协作图的组成元素
- 对象（Object）
矩形中的内容代表对象
- 链（Link）
- 消息 （Message）
- 举例——客户取车
取车的动作从客户开始，她向预定申请模块发送出示清单的消息，然后由公司员工向预定申请模块发送核对的消息，预定申请在收到消息核对的信息后，回复公司员工申请存在，然后再回复客户允许客户取车。公司员工收到消息后填写工作记录和登记汽车状态。
![在这里插入图片描述](https://img-blog.csdn.net/20181006144618249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 协作图和时序图的关系
- 二者在语义上等价
- 二者可以互相转化
- 二者侧重点不同
- 时序图侧重时间顺序
- 协作图侧重对象之间的关系，时间顺序从序列号获得
- 协作图与时序图的互换
学生学位评审的流程如下：教务人员将需评审的学生的学号输入学位初评模块，学位初评模块会查询相应写生的成绩和奖惩记录来作为学位评定的依据。学位初评模块将初评的结果打印，学位初评打印稿被提交给教务人员，控制流结束。
**时序图**
![在这里插入图片描述](https://img-blog.csdn.net/20181006151041801?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**协作图**
![在这里插入图片描述](https://img-blog.csdn.net/20181006151054395?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 附加——机房收费系统
![在这里插入图片描述](https://img-blog.csdn.net/2018100615172049?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 小结
把时序图和协作图一比较，自己大概了解了各方面的套路，也大体懂得了其中的画法。送给自己一句话“在漫长崎岖的人生旅途中，人们要想实现自己的理想和事业，就必须走好自己人生的每一步，就必须领悟到成功者的经验和智慧。幸福和快乐存在于自己抗争困难的过程中，决不是对困难的妥协。放弃自己就等于放弃一切。”Fighting！
