# OpenCV学习笔记（四十九）——号外！OpenCV-2.4.0 release 千呼万唤始出来

2012年05月03日 17:01:03

yang_xian521

阅读数：11662

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

最近一直很忙，没有及时回复网友的咨询和疑问，有些回复也是寥寥数语。其实我也没有办法，面对大量的问题，有大部分都是可以通过网络和图书获得答案，我实在没有精力每个问题都详细具体的回答，希望大家谅解，还是一位老师说的好：“知之为知之，不知百度知”。

前阵子有些朋友已经发现了OpenCV-2.4.0beta已经放出了，又有重大改动，很是让人兴奋，我前一篇blog——[OpenCV学习笔记（三十四）——OpenCV路在何方](http://blog.csdn.net/yang_xian521/article/details/7041566)中提到的OpenCV下一步的改进在这次版本中很多都得到了实现。最让我欣慰的是OpenCV有了自己的主页[code.opencv.org](http://http//code.opencv.org/projects/OpenCV/wiki/WikiStart)，之前的[http://opencv.willowgarage.com](http://opencv.willowgarage.com/wiki/)终将退出历史。其实这个beta版我也下载下来看过了，其中的文档并没有跟上，所以我一直在等。

五一节日，OpenCV也给大家送上了大礼，2.4.0release出炉。下载地址还是在Sourceforge上，之前听说OpenCV要把项目主页挪到ROS上，看来还没有实施。这次的压缩包竟然有201M啊，有木有啊！可见这次版本添加了很多内容啊，由于有重大改动，接下来还是详细给大家说说，其实新主页上都有，大家英文好可以去那里看，免得我给翻译错了~~。还有就是OpenCV许诺大家下一个版本2.4.1将在五个月后出炉，看来只能是活到老学到老了。。。

首先提到的还是网站的事，[code.opencv.org](http://code.opencv.org)现在是OpenCV开发首要的网站，相信日后的最新消息都会从这里放出。而[opencv.org](http://opencv.org)将作为官网（现在还没有投入使用，期待一下）。

还有一个重要的消息是一部分imgproc, video, calib3d, features2d, objdetect模块中的函数将废弃，扔到legacy中去了。兴奋的同时又让我很期待，新版本的整合是如何重整这些模块的呢，看来我又得好好重新学习了。版本的重大变化肯定也导致了CMake脚本的改变，这部分不是太精通，这里不班门弄斧。

一个新的模块cv::Algorithm出炉。这个模块包含了众多算法，也是接下来研究的重点，有新研究再和大家分享，文档我目前才开始看。

之前被大家诟病的视频部分这次又有所加强，大家经常抱怨OpenCV对视频支持的不好，不知道这次的改动会不会打一个翻身仗，还有待大家实践。

feature2d模块之前使用还是很不方便的，现在都被整合到Algorithm模块中了。SURF和SIFT被挪到了nonfree模块，这又是什么模块，看来新版本的惊喜还不少。。。还有一些函数的增添和改进，这里由于没有使用过，还是不乱讲，有兴趣自己去看一下咯。

一个新模块photo出炉，是为了计算摄影学而设计的，目前该模块只有从imgproc中移植出来的inpainting算法，还有一个模块是videostab是用来稳定视频的（这个模块还是beta版的）？这都是神马啊！具体咋用我也不知道啊。。。

之前我blog里提到的对GPU的支持力度要加强，OpenCV兑现了它的诺言，把光流法和特征点的提取检测算法都在GPU模块中实现了。之前我提过的android模块也有所加强，但具体就不介绍了，我本身也不是搞移动开发的。新版本还特意提到了一个函数getBuildInformation，日后也要试试做啥的。

从今日起，我的学习笔记都将使用OpenCV-2.4.0开发，之前的blog都是用2.3.1写的，肯定有些东西，大家看看就乱了，给大家的学习带来了不便，还请大家包涵理解。