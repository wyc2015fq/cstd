# QTimer::singleShot()函数使用 - fanyun的博客 - CSDN博客
2017年06月30日 20:13:56[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：7378标签：[qt](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/fanyun_01/article/category/6429937)
QTimer::singleShot(10, this,SLOT(eventOperation()));
       单singleShot，表示它只会触发一次，发出一次信号，然后来执行槽函数。
       如果把槽函数设置成eventOperation（）,那么当定时器到达时候，就会执行这个函数，在这个函数中定时器又会再次发出信号，然后执行槽函数eventOperation，这样[循环往复](https://www.baidu.com/s?wd=%E5%BE%AA%E7%8E%AF%E5%BE%80%E5%A4%8D&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YzPHPWuADvm1fvmWIhnjmv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPjnvnjTsPWmL)的，所以这是一个每隔10[毫秒](https://www.baidu.com/s?wd=%E6%AF%AB%E7%A7%92&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YzPHPWuADvm1fvmWIhnjmv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPjnvnjTsPWmL)就执行一次的循环。一般我们会在线程定时刷新的操作里面使用这个函数。
