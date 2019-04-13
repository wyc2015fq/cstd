
# 文章中插入Visio及Matlab矢量图 - 机器学习的小学生 - CSDN博客


2017年09月16日 16:06:31[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：2954


第一次使用LaTeX，很神奇。有关文章里矢量图的插入，这里作个小小的总结~
### 转载请注明出处：
### [http://blog.csdn.net/xiaowei_cqu/article/details/7699585](http://blog.csdn.net/xiaowei_cqu/article/details/7699585)
我只用到两种绘图工具：Visio和Matlab，最后都是存成pdf插入的。哦，还用绘图板手画了一个实验setup的图，也放到visio里再存的。
## Visio
visio默认存储格式vsd本身就是矢量图，一般可以再word里插vsd的，或者直接全选图ctrl+v过去就是插的vsd。但不同的版本显示可能有问题，所以我一般还是习惯存成png再放到word中。
LaTex个人觉得还是pdf比较好，png，jpg都是位图，放大不清楚。存pdf关键是要调页面大小，页面决定pdf页面，也就是最后插入LaTex图的大小。
比如页面是这样子，存成pdf就是右边的样子：
![](https://img-my.csdn.net/uploads/201206/30/1341021866_1374.jpg)[       ](https://img-my.csdn.net/uploads/201206/30/1341021866_1374.jpg)![](https://img-my.csdn.net/uploads/201206/30/1341022246_1291.jpg)
可以设计里 “纸张方向”、“大小”、“自动调整”几个选项调整图和页面的大小：
![](https://img-my.csdn.net/uploads/201206/30/1341022416_3620.jpg)
最后有这样的效果：
![](https://img-my.csdn.net/uploads/201206/30/1341022437_5753.jpg)[     ](https://img-my.csdn.net/uploads/201206/30/1341022437_5753.jpg)![](https://img-my.csdn.net/uploads/201206/30/1341022478_5945.jpg)
然后将pdf插入到LaTex中：
```python
\begin{figure}
  \centering
  \includegraphics[width=120mm]{fig.pdf}\\
  \caption{System environment.}
  \label{fig:env}
\end{figure}
```

## Matlab
matlab默认的存成格式fig是matlab的格式，可以存成eps是矢量图，也可以在word中插入。LaTex中插入eps好像是需要加宏包，因为需要一些转化操作：eps->ps->pdf (中间可能还有一步)。我直接还是转的pdf再插入的，可能有点麻烦。
先把matlab的图存成eps，再用Photoshop打开eps图片。打开时需要先对eps图进行栅格化操作：
![](https://img-my.csdn.net/uploads/201206/30/1341023370_9679.jpg)
这里“分辨率”的选项直接决定了你打开的图像质量，当然也关系到图像大小。 128的分辨率大概有1M，分辨率越大在pdf中放大也越清楚，当然图像大小也越大，根据论文要求取舍吧。这里PS很强大的在于你能随便定义大小，页面也是随着图像大小存的：
![](https://img-my.csdn.net/uploads/201206/30/1341023655_9954.jpg)
放大一个试试：
![](https://img-my.csdn.net/uploads/201206/30/1341023859_2727.jpg)

还是很清楚的 ！！








