
# matplotlib绘图蓝本 - saltriver的专栏 - CSDN博客


2016年10月30日 22:39:09[saltriver](https://me.csdn.net/saltriver)阅读数：482


matplotlib是python最著名的绘图库，它提供了一整套API，用于进行图表绘制、数据显示，其图片输出质量达到了科技论文的印刷质量。
但是，matplotlib绘制图表时，需要配置的东西比较多，每次查起来都比较麻烦，故而编写了一个**蓝本**，用于一般的绘图需要，稍加修改即可使用。
**主要包含了如下功能：**
- 中文显示设置
- 线条图
- 柱状图
- 散点图
- 标题
- XY轴标签
- XY轴坐标范围
- 文本说明
- 文本注释
- 显示网格
- 图例
最终图像示例：
![图像蓝本](https://img-blog.csdn.net/20161030223524147)
```python
# -*- coding:utf-8 -*-
```
```python
import matplotlib.pyplot as plt
from matplotlib import rcParams
import numpy as np
```
```python
# 中文字体设置 宋体中宋
```
```python
rcParams[
```
```python
'font.family'
```
```python
] =
```
```python
'STZhongSong'
```
```python
# 曲线图/折线图数据
```
```python
plot_x = np.linspace(-
```
```python
10
```
```python
,
```
```python
10
```
```python
,
```
```python
1000
```
```python
)
plot_y = np.sin(plot_x)
```
```python
# 柱状图数据
```
```python
bar_x = [-
```
```python
8
```
```python
, -
```
```python
4
```
```python
,
```
```python
0
```
```python
,
```
```python
4
```
```python
,
```
```python
8
```
```python
]
bar_y = [
```
```python
0.5
```
```python
,
```
```python
0.8
```
```python
,
```
```python
1
```
```python
,
```
```python
0.4
```
```python
,
```
```python
0.6
```
```python
]
```
```python
# 散点图数据
```
```python
scatter_x = np.random.uniform(-
```
```python
10
```
```python
,
```
```python
10
```
```python
,
```
```python
30
```
```python
)
scatter_y = np.random.uniform(-
```
```python
1.5
```
```python
,
```
```python
1.5
```
```python
,
```
```python
30
```
```python
)
```
```python
# 图像标题和X轴/Y轴标签
```
```python
plt.title(
```
```python
"这里是标题"
```
```python
, fontsize=
```
```python
16
```
```python
)
plt.xlabel(
```
```python
"这里是X轴"
```
```python
, fontsize=
```
```python
16
```
```python
)
plt.ylabel(
```
```python
"这里是Y轴"
```
```python
, fontsize=
```
```python
16
```
```python
)
```
```python
# 图像XY坐标范围
```
```python
plt.xlim(-
```
```python
10
```
```python
,
```
```python
10
```
```python
)
plt.ylim(-np.pi /
```
```python
2
```
```python
, np.pi /
```
```python
2
```
```python
)
```
```python
# 文本说明(任意位置)
```
```python
plt.text(
```
```python
5
```
```python
,
```
```python
1
```
```python
,
```
```python
"这是说明文字"
```
```python
)
```
```python
# 文本注释
```
```python
plt.annotate(
```
```python
"这是坐标原点"
```
```python
, xy=(
```
```python
0
```
```python
,
```
```python
0
```
```python
), xytext=(-
```
```python
5
```
```python
, -
```
```python
0.5
```
```python
), arrowprops=dict(facecolor=
```
```python
'g'
```
```python
, shrink=
```
```python
0.01
```
```python
))
```
```python
# 显示网格
```
```python
plt.grid(True)
```
```python
# 绘制折线图/曲线图
```
```python
plt.plot(plot_x, plot_y, label=
```
```python
"线条"
```
```python
, linewidth=
```
```python
2
```
```python
, color=
```
```python
'b'
```
```python
, alpha=
```
```python
0.8
```
```python
)
```
```python
# 绘制柱状图
```
```python
plt.bar(bar_x, bar_y, label=
```
```python
"柱状"
```
```python
, color=
```
```python
'r'
```
```python
, alpha=
```
```python
0.8
```
```python
)
```
```python
# 绘制散点图
```
```python
T = np.arctan2(scatter_y, scatter_x)
plt.scatter(scatter_x, scatter_y, label=
```
```python
"散点"
```
```python
, s=
```
```python
20
```
```python
, c=T, alpha=
```
```python
0.8
```
```python
)
```
```python
# 给出图例(位置：左上)
```
```python
plt.legend(loc=
```
```python
"upper left"
```
```python
)
```
```python
# 绘制图像
```
```python
plt.show()
```

