
# Python使用matplotlib填充图形指定区域代码示例 - jiahaowanhao的博客 - CSDN博客


2018年02月15日 13:31:39[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：2563


[Python使用matplotlib填充图形指定区域代码示例](http://cda.pinggu.org/view/24748.html)
本文代码重点在于演示Python扩展库matplotlib.pyplot中fill_between()函数的用法。

importnumpy as np
importmatplotlib.pyplot as plt
\# 生成模拟数据
x=np.arange(0.0,4.0*np.pi,0.01)
y=np.sin(x)
\# 绘制正弦曲线
plt.plot(x, y)
\# 绘制基准水平直线
plt.plot((x.min(),x.max()), (0,0))
\# 设置坐标轴标签
plt.xlabel('x')
plt.ylabel('y')
\# 填充指定区域
plt.fill_between(x,y, where=(2.3<x) & (x<4.3) | (x>10), facecolor='purple')
\# 可以填充多次
plt.fill_between(x,y,where=(7<x) & (x<8), facecolor='green')
plt.show()
演示效果：
![](http://cda.pinggu.org/uploadfile/image/20180215/20180215091853_97596.png)
总结
以上就是本文关于Python使用matplotlib填充图形指定区域代码示例的全部内容，希望对大家有所帮助。

