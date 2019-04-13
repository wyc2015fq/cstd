
# matplotlib 操作子图（subplot，axes） - Zhang's Wikipedia - CSDN博客


2018年04月26日 21:45:50[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：2312


[Matplotlib 中文用户指南 3.3 使用 GridSpec 自定义子图位置](https://blog.csdn.net/wizardforcel/article/details/54232788)
ax：matplotlib.axes._subplots.AxesSubplot，的基本操作
ax.set_xticks([]), ax_set_yticks([])：关闭坐标刻度
ax.axis('off')：关闭坐标轴
ax.set_title()：设置标题
1.  subplots
fig, ax = plt.subplots(nrows=1, ncols=2, figsize=(8,4))
ax[0].plot(...)
ax[0].set_xlabel(...)
ax[0].set_title(...)
ax[1].plot(...)
ax[1].set_xlabel(...)...fig, (ax1, ax2, ax3)=plt.subplots(3, 1, sharex=True)更一般的做法（也是matlab的风格）是这样的：
fig = plt.figure()\# 创建一个figure对象，底下的一切显示均在此figure上完成plt.subplot(121)
plt.imshow(img1)
plt.subplot(122)
plt.imshow(img2)
plt.show()\# 当然放在一个range里边for iinrange(ndim):
    plt.subplot(ndim//5,5, i+1)
    plt.hist()2. subplot2grid
subplot2grid
\# 以下两条语句等价ax = plt.subplot2grid((2,2),(0,0))
ax = plt.subplot(221)\# 下标从 0 开始colspan、rowspan 与 索引的对应关系
importmatplotlib.pyplotaspltdefdisable_axis(ax):ax.set_xticks([])
    ax.set_yticks([])\# ax.axis('off')defset_title(ax, title):ax.set_title(title)defsubplot2grid_demo():ax1 = plt.subplot2grid((3,3), (0,0), colspan=2)
    disable_axis(ax1)
    set_title(ax1,'ax1')
    ax2 = plt.subplot2grid((3,3), (1,0))
    disable_axis(ax2)
    set_title(ax2,'ax2')
    ax3 = plt.subplot2grid((3,3), (1,1))
    disable_axis(ax3)
    set_title(ax3,'ax3')
    ax4 = plt.subplot2grid((3,3), (0,2), rowspan=2)
    disable_axis(ax4)
    set_title(ax4,'ax4')
    ax5 = plt.subplot2grid((3,3), (2,0), colspan=3)
    disable_axis(ax5)
    set_title(ax5,'ax5')\# plt.xticks([])\# plt.yticks([])\# plt.axis('off')plt.show()if__name__ =='__main__':
    subplot2grid_demo()

![](https://img-blog.csdn.net/20180426214321863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[](https://img-blog.csdn.net/20180426214321863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[](https://img-blog.csdn.net/20180426214321863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)3. GridSpec：方便的切片操作
[](https://img-blog.csdn.net/20180426214321863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)GridSpec 提供了十分方便的切片操作，实现上述功能，则只需如下代码：
[](https://img-blog.csdn.net/20180426214321863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)defgridspec_demo():gs = gridspec.GridSpec(3,3)
    ax1 = plt.subplot(gs[0, :2])
    ax2 = plt.subplot(gs[1,0])
    ax3 = plt.subplot(gs[1,1])
    ax4 = plt.subplot(gs[0:2,2])
    ax5 = plt.subplot(gs[2:, :])
    plt.show()[](https://img-blog.csdn.net/20180426214321863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

