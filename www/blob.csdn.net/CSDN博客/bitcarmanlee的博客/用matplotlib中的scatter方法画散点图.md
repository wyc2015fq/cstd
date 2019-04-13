
# 用matplotlib中的scatter方法画散点图 - bitcarmanlee的博客 - CSDN博客


2018年09月14日 18:37:31[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：522



## 1.最简单的绘制方式
绘制散点图是数据分析过程中的常见需求。python中最有名的画图工具是matplotlib，matplotlib中的scatter方法可以方便实现画散点图的需求。下面我们来绘制一个最简单的散点图。
数据格式如下：
```python
0
```
```python
746403
```
```python
1
```
```python
1263043
```
```python
2
```
```python
982360
```
```python
3
```
```python
1202602
```
```python
...
```
其中第一列为X坐标，第二列为Y坐标。下面我们来画图。
```python
#!/usr/bin/env python
```
```python
#coding:utf-8
```
```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
```
```python
def
```
```python
pltpicture
```
```python
()
```
```python
:
```
```python
file =
```
```python
"xxx"
```
```python
xlist = []
    ylist = []
```
```python
with
```
```python
open(file,
```
```python
"r"
```
```python
)
```
```python
as
```
```python
f:
```
```python
for
```
```python
line
```
```python
in
```
```python
f.readlines():
            lines = line.strip().split()
```
```python
if
```
```python
len(lines) !=
```
```python
2
```
```python
or
```
```python
int(lines[
```
```python
1
```
```python
]) <
```
```python
100000
```
```python
:
```
```python
continue
```
```python
x, y = int(lines[
```
```python
0
```
```python
]), int(lines[
```
```python
1
```
```python
])
            xlist.append(x)
            ylist.append(y)
    plt.xlabel(
```
```python
'X'
```
```python
)
    plt.ylabel(
```
```python
'Y'
```
```python
)
    plt.scatter(xlist, ylist)
    plt.show()
```
![这里写图片描述](https://img-blog.csdn.net/20180914181810117?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2.更漂亮一些的画图方式
上面的图片比较粗糙，是最简单的方式，没有任何相关的配置项。下面我们再用另外一份数据集画出更漂亮一点的图。
数据集来自网络的公开数据集，数据格式如下：
```python
40920
```
```python
8.326976
```
```python
0.953952
```
```python
3
```
```python
14488
```
```python
7.153469
```
```python
1.673904
```
```python
2
```
```python
26052
```
```python
1.441871
```
```python
0.805124
```
```python
1
```
```python
75136
```
```python
13.147394
```
```python
0.428964
```
```python
1
```
```python
...
```
第一列每年获得的飞行常客里程数；
第二列玩视频游戏所耗时间百分比；
第三列每周消费的冰淇淋公升数；
第四列为label:
1表示不喜欢的人
2表示魅力一般的人
3表示极具魅力的人
现在将每年获取的飞行里程数作为X坐标，玩视频游戏所消耗的事件百分比作为Y坐标，画出图。
```python
from
```
```python
matplotlib import pyplot
```
```python
as
```
```python
plt
```
```python
file
```
```python
=
```
```python
"/home/mi/wanglei/data/datingTestSet2.txt"
```
```python
label1X, label1Y, label2X, label2Y, label3X, label3Y = [], [], [], [], [], []
```
```python
with
```
```python
open
```
```python
(
```
```python
file
```
```python
,
```
```python
"r"
```
```python
)
```
```python
as
```
```python
f:
```
```python
for
```
```python
line
```
```python
in
```
```python
f:
```
```python
lines
```
```python
=
```
```python
line
```
```python
.strip().
```
```python
split
```
```python
()
```
```python
if
```
```python
len
```
```python
(
```
```python
lines
```
```python
) !=
```
```python
4
```
```python
:
            continue
        distance, rate, label =
```
```python
lines
```
```python
[
```
```python
0
```
```python
],
```
```python
lines
```
```python
[
```
```python
1
```
```python
],
```
```python
lines
```
```python
[
```
```python
3
```
```python
]
```
```python
if
```
```python
label ==
```
```python
"1"
```
```python
:
            label1X.append(distance)
            label1Y.append(rate)
        elif label ==
```
```python
"2"
```
```python
:
            label2X.append(distance)
            label2Y.append(rate)
        elif label ==
```
```python
"3"
```
```python
:
            label3X.append(distance)
            label3Y.append(rate)
plt.figure(figsize=(
```
```python
8
```
```python
,
```
```python
5
```
```python
), dpi=
```
```python
80
```
```python
)
axes = plt.subplot(
```
```python
111
```
```python
)
label1 = axes.scatter(label1X, label1Y, s=
```
```python
20
```
```python
, c=
```
```python
"red"
```
```python
)
label2 = axes.scatter(label2X, label2Y, s=
```
```python
40
```
```python
, c=
```
```python
"green"
```
```python
)
label3 = axes.scatter(label3X, label3Y, s=
```
```python
50
```
```python
, c=
```
```python
"blue"
```
```python
)
plt.xlabel(
```
```python
"every year fly distance"
```
```python
)
plt.ylabel(
```
```python
"play video game rate"
```
```python
)
axes.legend((label1, label2, label3), (
```
```python
"don't like"
```
```python
,
```
```python
"attraction common"
```
```python
,
```
```python
"attraction perfect"
```
```python
), loc=
```
```python
2
```
```python
)
plt.show()
```
最后效果图：
![这里写图片描述](https://img-blog.csdn.net/20180914182624101?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 3.scatter函数详解
我们来看看scatter函数的签名：
```python
def scatter(self, x, y, s=None, c=None, marker=None, cmap=None, norm=None,
                vmin=None, vmax=None, alpha=None, linewidths=None,
                verts=None, edgecolors=None,
                **kwargs):
        """
        Make a scatter plot
```
```python
of
```
```python
`x` vs `y`
        Marker size
```
```python
is
```
```python
scaled
```
```python
by
```
```python
`s`
```
```python
and
```
```python
marker color
```
```python
is
```
```python
mapped to `c`
        Parameters
        ----------
        x, y : array_like, shape (n, )
            Input data
        s : scalar
```
```python
or
```
```python
array_like, shape (n, ), optional
            size
```
```python
in
```
```python
points^2.  Default
```
```python
is
```
```python
`rcParams['lines.markersize'] ** 2`.
        c : color, sequence,
```
```python
or
```
```python
sequence
```
```python
of
```
```python
color, optional, default: 'b'
            `c` can
```
```python
be
```
```python
a single color format string,
```
```python
or
```
```python
a sequence
```
```python
of
```
```python
color
            specifications
```
```python
of
```
```python
length `N`,
```
```python
or
```
```python
a sequence
```
```python
of
```
```python
`N` numbers to
```
```python
be
```
```python
mapped to colors using the `cmap`
```
```python
and
```
```python
`norm` specified via kwargs
            (see below). Note
```
```python
that
```
```python
`c` should
```
```python
not
```
```python
be
```
```python
a single numeric RGB
```
```python
or
```
```python
RGBA sequence because
```
```python
that
```
```python
is
```
```python
indistinguishable
```
```python
from
```
```python
an array
```
```python
of
```
```python
values to
```
```python
be
```
```python
colormapped.  `c` can
```
```python
be
```
```python
a 2-D array
```
```python
in
```
```python
which the
            rows are RGB
```
```python
or
```
```python
RGBA, however, including the case
```
```python
of
```
```python
a single
            row to specify the same color
```
```python
for
```
```python
all points.
        marker : `~matplotlib.markers.MarkerStyle`, optional, default: 'o'
            See `~matplotlib.markers`
```
```python
for
```
```python
more information on the different
            styles
```
```python
of
```
```python
markers scatter supports. `marker` can
```
```python
be
```
```python
either
            an instance
```
```python
of
```
```python
the class
```
```python
or
```
```python
the text shorthand
```
```python
for
```
```python
a particular
            marker.
        cmap : `~matplotlib.colors.Colormap`, optional, default: None
            A `~matplotlib.colors.Colormap` instance
```
```python
or
```
```python
registered name.
            `cmap`
```
```python
is
```
```python
only used if `c`
```
```python
is
```
```python
an array
```
```python
of
```
```python
floats. If None,
            defaults to rc `image.cmap`.
        norm : `~matplotlib.colors.Normalize`, optional, default: None
            A `~matplotlib.colors.Normalize` instance
```
```python
is
```
```python
used to scale
            luminance data to 0, 1. `norm`
```
```python
is
```
```python
only used if `c`
```
```python
is
```
```python
an array
```
```python
of
```
```python
floats. If `None`, use the default :
```
```python
func
```
```python
:`normalize`.
        vmin, vmax : scalar, optional, default: None
            `vmin`
```
```python
and
```
```python
`vmax` are used
```
```python
in
```
```python
conjunction with `norm` to normalize
            luminance data.  If either are `None`, the min
```
```python
and
```
```python
max
```
```python
of
```
```python
the
            color array
```
```python
is
```
```python
used.  Note if you pass a `norm` instance, your
            settings
```
```python
for
```
```python
`vmin`
```
```python
and
```
```python
`vmax` will
```
```python
be
```
```python
ignored.
        alpha : scalar, optional, default: None
            The alpha blending value, between 0 (transparent)
```
```python
and
```
```python
1 (opaque)
        linewidths : scalar
```
```python
or
```
```python
array_like, optional, default: None
            If None, defaults to (lines.linewidth,).
        verts : sequence
```
```python
of
```
```python
(x, y), optional
            If `marker`
```
```python
is
```
```python
None, these vertices will
```
```python
be
```
```python
used to
            construct the marker.  The center
```
```python
of
```
```python
the marker
```
```python
is
```
```python
located
            at (0,0)
```
```python
in
```
```python
normalized units.  The overall marker
```
```python
is
```
```python
rescaled
```
```python
by
```
```python
``s``.
        edgecolors : color
```
```python
or
```
```python
sequence
```
```python
of
```
```python
color, optional, default: None
            If None, defaults to 'face'
            If 'face', the edge color will always
```
```python
be
```
```python
the same
```
```python
as
```
```python
the face color.
            If it
```
```python
is
```
```python
'none', the patch boundary will
```
```python
not
```
```python
be
```
```python
drawn.
            For non-filled markers, the `edgecolors` kwarg
```
```python
is
```
```python
ignored
```
```python
and
```
```python
forced to 'face' internally.
        Returns
        -------
        paths : `~matplotlib.collections.PathCollection`
        Other parameters
        ----------------
        kwargs : `~matplotlib.collections.Collection` properties
        See Also
        --------
        plot : to plot scatter plots when markers are identical
```
```python
in
```
```python
size
```
```python
and
```
```python
color
        Notes
        -----
        * The `plot` function will
```
```python
be
```
```python
faster
```
```python
for
```
```python
scatterplots where markers
          don't vary
```
```python
in
```
```python
size
```
```python
or
```
```python
color.
        * Any
```
```python
or
```
```python
all
```
```python
of
```
```python
`x`, `y`, `s`,
```
```python
and
```
```python
`c` may
```
```python
be
```
```python
masked arrays,
```
```python
in
```
```python
which
          case all masks will
```
```python
be
```
```python
combined
```
```python
and
```
```python
only unmasked points will
```
```python
be
```
```python
plotted.
          Fundamentally, scatter works with 1-D arrays; `x`, `y`, `s`,
```
```python
and
```
```python
`c`
          may
```
```python
be
```
```python
input
```
```python
as
```
```python
2-D arrays, but within scatter they will
```
```python
be
```
```python
flattened. The exception
```
```python
is
```
```python
`c`, which will
```
```python
be
```
```python
flattened only if its
          size matches the size
```
```python
of
```
```python
`x`
```
```python
and
```
```python
`y`.
        Examples
        --------
        .. plot
```
```python
:: mpl_examples/shapes_and_collections/scatter_demo.py
```
```python
"""
```
其中具体的参数含义如下：
x,y是相同长度的数组。
s可以是标量，或者与x,y长度相同的数组，表明散点的大小。默认为20。
c即color，表示点的颜色。
marker 是散点的形状。

