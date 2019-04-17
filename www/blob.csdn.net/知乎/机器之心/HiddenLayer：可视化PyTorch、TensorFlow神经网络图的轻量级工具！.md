# HiddenLayer：可视化PyTorch、TensorFlow神经网络图的轻量级工具！ - 知乎
# 



选自GitHub，机器之心编译，参与：张倩、王淑婷。

> 本文介绍了一个面向 PyTorch 和 TensorFlow 神经网络计算图和训练度量（metric）的轻量级库——HiddenLayer，它适用于快速实验，且与 Jupyter Notebook 兼容。

GitHub链接：[https://github.com/waleedka/hiddenlayer](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer)

HiddenLayer 非常简单，易于扩展，且与 Jupyter Notebook 完美兼容。开发该工具的目的不是为了取代 TensorBoard 等高级工具，而是用在那些无需使用高级工具的用例中（杀鸡焉用宰牛刀）。HiddenLayer 由 Waleed Abdulla 和 Phil Ferriere 编写，已获得 MIT 许可证。

**1. 可读的图**

使用 HiddenLayer 在 Jupyter Notebook 中渲染你的神经网络图，或者渲染 pdf 或 png 文件。Jupyter notebook 示例请参考以下链接：
- TensorFlow：[https://github.com/waleedka/hiddenlayer/blob/master/demos/tf_graph.ipynb](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer/blob/master/demos/tf_graph.ipynb)
- Pytorch：[https://github.com/waleedka/hiddenlayer/blob/master/demos/pytorch_graph.ipynb](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer/blob/master/demos/pytorch_graph.ipynb)
![](https://pic4.zhimg.com/v2-bb640dd245d97cc618fb45f17db561df_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='591' height='578'></svg>)
这些图用于沟通高级架构。因此，低级细节在默认状态下是隐藏的（如权重初始化 ops、梯度、一般层类型的内部 ops 等）。HiddenLayer 还将常用层序列叠在一起。例如，Convolution -> RELU -> MaxPool 序列比较常用，为简单起见，它们被合并在一个盒子里。

自定义图

隐藏、折叠节点的规则是完全可定制的。你可以用 graph expressions 和 transforms 添加自己的规则。例如，使用以下命令可以将 ResNet101 中 bottleneck 块的所有节点折叠为一个节点


```
# Fold bottleneck blocks
    ht.Fold("((ConvBnRelu > ConvBnRelu > ConvBn) | ConvBn) > Add > Relu", 
            "BottleneckBlock", "Bottleneck Block"),
```







![](https://pic4.zhimg.com/v2-54d89211d83bbf32de6edb984ee003b3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='273' height='561'></svg>)
**2. Jupyter Notebook 中的训练度量**

在 Jupyter Notebook 中运行训练试验非常有用。你可以绘制损失函数和准确率图、权重直方图，或者可视化一些层的激活函数。
![](https://pic4.zhimg.com/v2-523c80e3fa18bf90cd6422d4af301117_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='640'></svg>)
在 Jupyter Notebook 之外：

在 Jupyter Notebook 外同样可以使用 HiddenLayer。在 Python 脚本中运行 HiddenLayer，可以打开度量的单独窗口。如果你使用的服务器没有 GUI，可以将图像截图存储为 png 文件以备后查。该用例示例参见：[https://github.com/waleedka/hiddenlayer/blob/master/demos/history_canvas.py](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer/blob/master/demos/history_canvas.py)。




**3. Hackable**

HiddenLayer 是一个小型库。它覆盖基础元素，但你可能需要为自己的用例进行扩展。例如，如果你想将模型准确率表示为饼图，就需要扩展 Canbas 类，并添加新方法，如下所示：


```
class MyCanvas(hl.Canvas):
    """Extending Canvas to add a pie chart method."""
    def draw_pie(self, metric):
        # set square aspect ratio
        self.ax.axis('equal')
        # Get latest value of the metric
        value = np.clip(metric.data[-1], 0, 1)
        # Draw pie chart
        self.ax.pie([value, 1-value], labels=["Accuracy", ""])
```


示例参见：[https://github.com/waleedka/hiddenlayer/blob/master/demos/pytorch_train.ipynb](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer/blob/master/demos/pytorch_train.ipynb) 或 [https://github.com/waleedka/hiddenlayer/blob/master/demos/tf_train.ipynb](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer/blob/master/demos/tf_train.ipynb)。
![](https://pic4.zhimg.com/v2-64ec672585052abe514977cbd9d8abeb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='427'></svg>)
**Demo**

**PyTorch：**
- pytorch_graph.ipynb：此 notebook 展示了如何为一些流行的 PyTorch 模型生成图。
- 地址：[https://github.com/waleedka/hiddenlayer/blob/master/demos/pytorch_graph.ipynb](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer/blob/master/demos/pytorch_graph.ipynb)
- pytorch_train.ipynb：展示了如何在 PyTorch 中追踪和可视化训练度量。
- 地址：[https://github.com/waleedka/hiddenlayer/blob/master/demos/pytorch_train.ipynb](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer/blob/master/demos/pytorch_train.ipynb)
- history_canvas.py：在没有 GUI 的情况下使用 HiddenLayer 的示例。
- 地址：[https://github.com/waleedka/hiddenlayer/blob/master/demos/history_canvas.py](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer/blob/master/demos/history_canvas.py)




**TensorFlow：**
- tf_graph.ipynb：此 notebook 介绍了如何为不同的 TF SLIM 模型生成图。
- 地址：[https://github.com/waleedka/hiddenlayer/blob/master/demos/tf_graph.ipynb](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer/blob/master/demos/tf_graph.ipynb)
- tf_train.ipynb：展示了如何在 TensorFlow 中追踪和可视化训练度量。
- 地址：[https://github.com/waleedka/hiddenlayer/blob/master/demos/tf_train.ipynb](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer/blob/master/demos/tf_train.ipynb)
- history_canvas.py：在没有 GUI 的情况下使用 HiddenLayer 的示例。
- 地址：[https://github.com/waleedka/hiddenlayer/blob/master/demos/history_canvas.py](https://link.zhihu.com/?target=https%3A//github.com/waleedka/hiddenlayer/blob/master/demos/history_canvas.py)




**安装**

**1. 先决条件**

a. Python3、Numpy、Matplotlib 和 Jupyter Notebook。

b. 用 TensorFlow 或 PyTorch。

c. 用 GraphViz 及其 Python 封装来生成网络图。最简单的安装方式是：

> **If you use Conda:** ```bash conda install graphviz python-graphviz ```  **Otherwise:** * [Install GraphViz]([https://graphviz.gitlab.io/download/](https://link.zhihu.com/?target=https%3A//graphviz.gitlab.io/download/)) * Then install the [Python wrapper for GraphViz]([https://github.com/xflr6/graphviz](https://link.zhihu.com/?target=https%3A//github.com/xflr6/graphviz)) using pip:     ```     pip3 install graphviz     ```

**2. 安装 HiddenLayer**

A. 从 GitHub 中安装（开发者模式）

如果要在本地编辑或自定义库，使用此选项。


```
# Clone the repository
git clone git@github.com:waleedka/hiddenlayer.git
cd hiddenlayer

# Install in dev mode
pip install -e .
```


B. 使用 PIP

`pip install hiddenlayer`




