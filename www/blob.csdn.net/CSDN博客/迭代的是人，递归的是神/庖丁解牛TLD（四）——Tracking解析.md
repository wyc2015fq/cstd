# 庖丁解牛TLD（四）——Tracking解析 - 迭代的是人，递归的是神 - CSDN博客





2011年12月17日 13:28:53[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：15767








前几节都是根据作者的程序流程一步步介绍作者的工作，感觉只是对代码的一个注释，这次换一个思路，一部分一部分啃，作者的工作主要就是3部分么，tracking，learning，detection。

这次先介绍Tracking的工作。对于Tracking，作者主要使用的是他提出的Forward-Backward Error的办法，使用Lucas-Kanade光流法跟踪，对跟踪的结果，用Forward-Backward Error做反馈，求FB error的结果与原始位置的欧式距离，把距离过大的跟踪结果舍弃，他把这种利用FB
 error舍弃坏值的跟踪方法叫做Median Flow，是把欧式距离集合中较大的50%的那些跟踪结果舍弃。作者在他的文章Forward-Backward Error：Automatic Detection of Tracking Failures里提到用FB+NCC（交叉验证）的方案，可以使跟踪的结果最佳。作者的Tracking的办法就是根据我以上介绍的流程实现的。接下来结合代码再详细剖析一下

先用bb_points函数在box中均匀采样10*10个点，注意作者这里设置了采样点的区域比box的区域少一圈边界，边界为5，在后面我会介绍作者这里的独到用心。然后调用混合编程的lk函数实现lucas-Kanade光流法跟踪，得到的结果有为这100个点的lk结果，前两个参数为利用l-k方法得到的点当前的跟踪位置坐标，第三个参数是利用NCC把跟踪预测的结果周围取10*10的小图片与原始位置周围10*10（这里取10*10，有心的朋友应该笑了，为什么作者之前在bb_points函数里要设置个边界5，原来是防止越界哦）的小图片（使用函数getRectSubPix得到）进行模板匹配（调用matchTemplate），再对匹配的结果归一化，把这个结果保存在第三个参数中，第四个参数为FB
 error的欧氏距离。这个lk函数过程中有很多参数可以设置，对最终的结果我想应该应该也是有的，有待实验验证。接下来就是利用作者提出的Median Flow，得到NCC和FB error结果的中值，分别去掉中值一半的跟踪结果不好的点，利用这一半（其实不到50%）的跟踪点输入函数bb_predict函数中预测bounding box在当前帧的位置和大小。

这基本就是Tracking工作的主要部分了，至于被遮挡的tracking（tldTrack_occlusion），作者进行了单独处理，下一次再分析。

PS：很感谢最近有些网友与我一起研究TLD，不过本人能力不足，很多东西还是不理解，对于作者detection和learning的工作，感觉那部分的代码实在好比天书，没法拿出来和大家交流了，希望有识之士也能写出来，和大家分享~~



