# ros::spin()和ros::spinOnce()函数 - 站在巨人的肩膀上coding - CSDN博客





2018年03月14日 13:57:14[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：161








看了wiki上面的关于消息发布器和订阅器之后，对于ros::spin()和ros::spinOnce()不是很理解。于是查了很多资料，总结如下：




关于ros消息发布器和订阅器的教程：[http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber(c++)](http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber%28c++%29)





看完链接里面的教程后，我们知道：




消息发布器在一个while里面一直循环发送“hello world”到话题（topic）chatter上。消息订阅器一旦知道chatter上面有data，就会将这data作为参数传入callback函数中，但是此时还没有执行callback函数，而是把callback函数放到了一个回调函数队列中。所以当发布器不断发送data到chatter上面时，就会有相应的callback函数进入队列中，它们函数名一样，只是实参不一样。




那么什么时候才会执行callback函数呢？




就是ros::spin()和ros::spinOnce()的事了。




当spinOnce函数被调用时，spinOnce就会调用回调函数队列中第一个callback函数，此时callback函数才被执行，然后等到下次spinOnce函数又被调用时，回调函数队列中第二个callback函数就会被调用，以此类推。




所以，这会有一个问题。因为回调函数队列的长度是有限的，如果发布器发送数据的速度太快，spinOnce函数调用的频率太少，就会导致队列溢出，一些callback函数就会被挤掉，导致没被执行到。




而对于spin函数，一旦进入spin函数，它就不会返回了，相当于它在自己的函数里面死循环了。只要回调函数队列里面有callback函数在，它就会马上去执行callback函数。如果没有的话，它就会阻塞，不会占用CPU。



这里有一篇博客详细解释，可供参考： [http://www.07net01.com/2016/10/1680311.html](http://blog.csdn.net/http://www.07net01.com/2016/10/1680311.html/)


