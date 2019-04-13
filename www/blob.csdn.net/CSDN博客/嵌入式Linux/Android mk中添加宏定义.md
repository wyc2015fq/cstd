
# Android.mk中添加宏定义 - 嵌入式Linux - CSDN博客

2018年11月01日 14:07:43[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：115


在Boardconfig.mk
中添加一个
**IMAGE_FOR_FACTORY_TEST**:= true
由于Boardconfig.mk和各目录的[Android](http://www.2cto.com/kf/yidong/Android/).mk是相互关联的
所以我们可以在Android.mk 中添加
一个
ifdef**IMAGE_FOR_FACTORY_TEST**
LOCAL_CFLAGS += -**DFACTORY_TEST**
endif
在Android.mk中添加的这行相当于在 该目录下的文件中定义了一个宏
\#define**FACTORY_TEST**
如果我们想要在某个工程中根据工程的不同来对相关的代码中的某段参数进行选择时，可以参考此方法

