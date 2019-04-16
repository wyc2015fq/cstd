# BREW SDK 九大功能之多媒体系统 - 我相信...... - CSDN博客





2010年04月06日 11:43:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2170标签：[brew																[多媒体系统																[数据结构																[图像处理																[框架																[手机](https://so.csdn.net/so/search/s.do?q=手机&t=blog)




对多媒体的支持主要是IMedia接口实现的，从IMedia接口衍生出很多的媒体接口，如图11-3所示：



图11-3： IMedia接口支持的部分媒体格式



IMedia接口为所有BREW多媒体对象提供了一个抽象基类，提供了媒体对象的播放、记录及与播放/记录相关的搜索、停止、暂停和继续等操作功能，开发者通过IMedia接口可以进行所有多媒体对象的操作。BREW中的多媒体框架如图11-4所示。





图11-4： 手机中的多媒体构架



            媒体的播放状态符合有限状态机模型，一般有6种状态：idle，Ready，Play，Play_Pause, Record和Record_Pause, IMedia使用媒体的状态机如图11-5所示。



图11-5： IMedia使用的多媒体状态机



使用IMedia接口的各种方法必须按照此状态机的状态进行转换，这样可以保证最大限度的使用。如果状态出错，必须重新设置状态机。

            IMedia 播放多媒体的流程如图11-6所示，显示了多媒体对象的状态在Ready和Play之间的变化。





图11-6：IMedia 播放多媒体的流程

针对图像处理，BREW3.x也提供了丰富的支持。BREW应用可以使用IBitmap 和IDisplay进行位图操作，IDIB是由IBitmap派生的一个接口和数据结构。通过IDIB的数据结构成员变量可以定位位图中图像点(pixel)的位置，以及如何解析图像点的颜色值，通过IDIB结构可以实现对位图数据的直接操作。对图像的缩放处理一种方法是通过ITransform接口实现，需要通过目标位图获取ITransform句柄，缩放完成的目标位图位置由用户指定，缩放要求通过AEETransformMatrix进行设定，另一种方法是通过IBitmapScale，IBITMAPROT和IBitmapFX结合使用来实现。对于显示屏幕旋转事件处理，可以通过IDISPLAY_SetPrefs来实现。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/03/31/5435864.aspx](http://blog.csdn.net/wireless_com/archive/2010/03/31/5435864.aspx)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=多媒体系统&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




