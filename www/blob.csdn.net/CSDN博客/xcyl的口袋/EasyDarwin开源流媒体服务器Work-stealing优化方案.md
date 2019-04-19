# EasyDarwin开源流媒体服务器Work-stealing优化方案 - xcyl的口袋 - CSDN博客
2016年09月01日 15:33:35[xcyl](https://me.csdn.net/cai6811376)阅读数：745
EasyDarwin团队的Babosa对EasyDarwin流媒体服务器的性能瓶颈进行了分析，并提出了优化方案。[EasyDarwin开源流媒体服务器性能瓶颈分析及优化方案设计](http://blog.csdn.net/xiejiashu/article/details/51874158)
这里，根据Babosa提出的优化方案，以及Work-stealing的算法特性，提出用Work-stealing算法对EasyDarwin进行优化。
## 什么是Work-stealing？
> 
工作窃取（work-stealing）算法是指某个线程从其他队列里窃取任务来执行。
![这里写图片描述](https://img-blog.csdn.net/20160901150439225)
![这里写图片描述](https://img-blog.csdn.net/20160901150453162)
*图片出自 [java7里的fork-join](http://hongjiang.info/java7-fork-join/)*
## EasyDarwin优化方案
- 
不区分TaskThread，将BlockingTaskThread和ShortTaskThread统一；
- 
实现一个双端队列，支持LIFO的push、pop，FIFO的take；
- 
当前TaskThread使用LIFO处理最新的Task，空闲的TadkThread使用FIFO处理尾部任务；
- 
空闲TaskThread窃取任务失败，继续重试，直到所有的TaskThread处理完所有的Task。
参考资料
[http://www.cnblogs.com/qicosmos/archive/2015/11/18/4975454.html](http://www.cnblogs.com/qicosmos/archive/2015/11/18/4975454.html)
[http://gee.cs.oswego.edu/dl/papers/fj.pdf](http://gee.cs.oswego.edu/dl/papers/fj.pdf)
[http://supertech.csail.mit.edu/papers/steal.pdf](http://supertech.csail.mit.edu/papers/steal.pdf)
## 获取更多信息
Github：[https://github.com/easydarwin](https://github.com/easydarwin)
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
