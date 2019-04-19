# dev_t的主次编号 - Koma Hub - CSDN博客
2019年02月27日 21:05:24[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：24
个人分类：[Linux driver](https://blog.csdn.net/Rong_Toa/article/category/8645170)
在内核中, dev_t 类型(在 <linux/types.h>中定义)用来持有设备编号 -- 主次部分都包括. 对于 2.6.0 内核, dev_t 是 32 位的量, 12 位用作主编号, 20 位用作次编号. 你的代码应当, 当然, 对于设备编号的内部组织从不做任何假设; 相反, 应当利用在<linux/kdev_t.h>中的一套宏定义. 为获得一个 dev_t 的主或者次编号, 使用:
```cpp
MAJOR(dev_t dev);
MINOR(dev_t dev);
```
相反, 如果你有主次编号, 需要将其转换为一个 dev_t, 使用:
```cpp
MKDEV(int major, int minor);
```
注意, 2.6 内核能容纳有大量设备, 而以前的内核版本限制在 255 个主编号和 255 个次编号. 有人认为这么宽的范围在很长时间内是足够的, 但是计算领域被这个特性的错误假设搞乱了. 因此你应当希望 dev_t 的格式将来可能再次改变; 但是, 如果你仔细编写你的驱动, 这些变化不会是一个问题.
