# ios CALayer的Frame,bounds,position,anchorPoint - xqhrs232的专栏 - CSDN博客
2019年03月04日 16:47:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：24
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/ifdefine/article/details/51146490](https://blog.csdn.net/ifdefine/article/details/51146490)
相关文章
1、UIView CALayer frame，position ，anchorPoint的关系----[https://blog.csdn.net/dfman1978/article/details/73773896](https://blog.csdn.net/dfman1978/article/details/73773896)
2、【iOS】菜鸟进阶：浅谈Layer两个属性position和anchorPoint----[https://www.jianshu.com/p/7703e6fc6191](https://www.jianshu.com/p/7703e6fc6191)
记录四者之间的关系
frame.origin.x = position.x - anchorPoint.x * bounds.size.width； 
frame.origin.y = position.y - anchorPoint.y * bounds.size.height；
Frame
描述当前视图在其父视图中的位置和大小
bounds
描述当前视图在其自身坐标系统中的位置和大小
anchorPoint
AnchorPoint 属于 iOS CoreAnimation层。图层的anchorPoint属性是一个CGPoint值，是变换的支点。AnchorPoint值的范围是0~1.
position
anchorPoint的点在superlayer中的坐标。 
position是layer中的anchorPoint在superLayer中的位置坐标，通过公式可以得出。
不同属性改变时的结果
1.修改position 
frame改变，anchorPoint不变
2.修改frame 
position改变，anchorPoint不变
3.修改anchorPoint 
frame改变
参考： 
http://blog.csdn.net/yongyinmg/article/details/37927793 
http://www.jcodecraeer.com/IOS/2015/0204/2413.html
