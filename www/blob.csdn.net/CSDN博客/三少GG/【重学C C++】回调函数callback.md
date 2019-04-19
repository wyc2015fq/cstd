# 【重学C/C++】回调函数callback - 三少GG - CSDN博客
2011年10月10日 15:05:40[三少GG](https://me.csdn.net/scut1135)阅读数：967标签：[callback																[电话																[手机																[2010																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=电话&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
原文请看：[http://topic.csdn.net/u/20100316/16/e17fe762-d1c3-47d0-a3e9-ee4005b730ef.html](http://topic.csdn.net/u/20100316/16/e17fe762-d1c3-47d0-a3e9-ee4005b730ef.html)
[其中4楼的比喻非常贴切，这里和大家分享。](http://blog.csdn.net/java2000_net/article/details/5435448)
你饿了,想吃饭,就一会去问你妈一声"开饭没有啊?"这就正常函数调用.
但是今天你妈包饺子,花的时间比较长,你跑啊跑啊,就烦了.于是 你给你妈说,我先出去玩会,开饭的时候打我手机.等过了一阵,你妈给你打电话说"开饭啦,快回来吃饭吧!"
其中,你告诉你妈打手机找你, 就是个你把回调函数句柄保存到你妈的动作.你妈打电话叫你,就是个回调过程.
总结：回调方法，一般会是一个接口，然后调用者提供一个具体的实现类作为参数传递过去就行了。
回调，一般属于异步调用，主线程会继续运行。
++++++++++++++++++++++++
就是把方法A的句柄给方法B 由方法B来决定何时调用方法A
引用吃饭的例子：
A就是回调的函数也就是就是喊你吃饭；B就你妈打电话的动作
[CALLBACK回调函数使用之一](http://blog.csdn.net/phlexii/article/details/726033)
### [CALLBACK回调函数使用之二](http://blog.csdn.net/phlexii/article/details/726061)
### [CALLBACK回调函数使用之三](http://blog.csdn.net/phlexii/article/details/726102)
