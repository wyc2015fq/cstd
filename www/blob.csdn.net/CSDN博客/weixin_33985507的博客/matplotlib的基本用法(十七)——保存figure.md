# matplotlib的基本用法(十七)——保存figure - weixin_33985507的博客 - CSDN博客
2017年05月10日 21:06:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
文章作者：Tyan
博客：[noahsnail.com](https://link.jianshu.com?t=http://noahsnail.com)  |  [CSDN](https://link.jianshu.com?t=http://blog.csdn.net/quincuntial)  |  [简书](https://www.jianshu.com/users/7731e83f3a4e/latest_articles)
本文主要介绍matplotlib的一些用法。
- Demo
```
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('Agg')
# 定义数据
x = np.arange(0, 10, 0.1)
y = 0.05 * x ** 2
#绘制图像
plt.plot(x, y)
# 设置坐标轴
plt.xlabel('x data')
plt.ylabel('y data')
# 默认保存为png格式
plt.savefig('test')
```
- 结果
在文件夹下可看到`test.png`，如图：
![3232548-f9409fd16d800baa.png](https://upload-images.jianshu.io/upload_images/3232548-f9409fd16d800baa.png)
图像
