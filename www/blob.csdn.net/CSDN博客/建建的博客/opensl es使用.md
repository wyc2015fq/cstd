# opensl  es使用 - 建建的博客 - CSDN博客
2018年03月17日 10:21:57[纪建](https://me.csdn.net/u013898698)阅读数：260
　使用opensl es 的前提条件是在[源代码](https://www.baidu.com/s?wd=%E6%BA%90%E4%BB%A3%E7%A0%81&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9PHbsuH-BPhcsmhwhuh7b0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPWfLrjRknHD4P1fYnHf4nj6d)中：
　　#include <[SLES](https://www.baidu.com/s?wd=SLES&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9PHbsuH-BPhcsmhwhuh7b0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPWfLrjRknHD4P1fYnHf4nj6d)/Open[SLES](https://www.baidu.com/s?wd=SLES&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9PHbsuH-BPhcsmhwhuh7b0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPWfLrjRknHD4P1fYnHf4nj6d).h>， 包含 opensl es的头文件
　　在gcc中：
　　-lOpen[SLES](https://www.baidu.com/s?wd=SLES&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9PHbsuH-BPhcsmhwhuh7b0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPWfLrjRknHD4P1fYnHf4nj6d) 链接 opensl es的库文件
　　大休步骤如下：
　　1.创建声音引擎
　　2.创建声音[播放器](https://www.baidu.com/s?wd=%E6%92%AD%E6%94%BE%E5%99%A8&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9PHbsuH-BPhcsmhwhuh7b0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPWfLrjRknHD4P1fYnHf4nj6d)
　　3.设置播放缓冲
　　opensl es 主要操作的是 介面（interface），大体来说有如下几个介面：
　　SLObjectItf ： 对象介面，
　　SLEngineItf ：引擎介面
　　SLPlayItf：播放介面
　　SLBufferQueueItf :缓冲队列介面
　　SLEffectSendItf：音效发送介面
　　SLMuteSoloItf : 声道关闭 /[单声道](https://www.baidu.com/s?wd=%E5%8D%95%E5%A3%B0%E9%81%93&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9PHbsuH-BPhcsmhwhuh7b0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPWfLrjRknHD4P1fYnHf4nj6d)介面
　　SLVolumeItf : 声量介面
