# 用脑电波控制智能假肢：如何利用深度学习技术进行EGG数据分类 - 知乎
# 



选自TowardsDataScience，作者：Norman Di Palo，机器之心编译。

> 脑电图是一种利用电极记录大脑活动的非侵入式技术，但大脑活动和脑电图信号之间的关系非常复杂，如何「解码」成为困扰研究者的一大难题。本文作者利用 Kaggle 竞赛中的脑电图数据探索与特定手势对应的脑电图模式，并以此设计神经网络。
- 代码地址：[https://colab.research.google.com/drive/1lDfmXMo7_mcVBo9EAtgLNIgeJV50GTce#scrollTo=AFoTOqC-4Rjl](https://link.zhihu.com/?target=https%3A//colab.research.google.com/drive/1lDfmXMo7_mcVBo9EAtgLNIgeJV50GTce%23scrollTo%3DAFoTOqC-4Rjl)
- GitHub 地址：[https://github.com/normandipalo/eeg-to-action](https://link.zhihu.com/?target=https%3A//github.com/normandipalo/eeg-to-action)
![](https://pic4.zhimg.com/v2-f6100098d4cad59446e0608e7169656b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='383'></svg>)
神经系统是一个极其复杂的结构。人体内有超过十万公里的神经与脊髓和大脑相连。这种「网格」传输控制每一个运动的电脉冲。每一个指令都从大脑发出，大脑是一个更加神奇的神经元结构，通过电激活信号进行通信。理解和解释大脑的电模式是神经科学家和神经生物学家的研究热点之一，但事实证明这是一项极具挑战性的任务。

记录大脑活动的一种非侵入式技术是「脑电图」（EEG），使用固定在患者头皮上的电极记录脑电压波动。通常在头皮周围固定大约 30 个电极，记录脑电波的总体活动。无论如何，大脑活动和 EEG 信号之间的关系非常复杂，如果不借助具体的实验测试，我们将很难理解。因此，一个巨大的挑战是学习如何「解码」，在某种意义上，这些脑电图扫描可以允许使用非侵入式脑机接口（BCI）来控制机器假肢和其他设备。
![](https://pic1.zhimg.com/v2-b48bfd46b4cf186cf599c7a139e320b8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='478' height='500'></svg>)



利用 EEG 记录脑电波。CC BY-SA 2.0，来源：[https://commons.wikimedia.org/w/index.php?curid=845554](https://link.zhihu.com/?target=https%3A//commons.wikimedia.org/w/index.php%3Fcurid%3D845554)




作为强数据驱动的学科，最近在相关模式识别任务中取得的「深度学习」新突破为使用「神经网络」分析这些电信号创造了一种新方法。在这篇文章中，我们首先介绍这一主题：阅读 Kaggle 竞赛（Grasp-and-Lift EEG Detection）提供的 EEG 数据，该竞赛旨在检测哪些 EEG 模式对应特定的手臂和手势动作，如抓取或提起物体。在以不同的方式预处理数据之后，我们将设计一个神经网络来执行这种分类。此外，我还将展示一些大脑活动的数据可视化，以便大致了解正在使用的数据。这一研究领域的最终目标是开发平价、实用的假肢装置，通过大脑控制假肢，帮助截肢者恢复轻松进行基本活动的能力。类似的技术也可以应用于读取肌肉电激活，从而通过分析激活的肌肉来解码人试图执行的运动类型。




**数据简介**

如果你拥有 Kaggle 帐户，那么你可以在该网址免费下载数据：[https://www.kaggle.com/c/grasp-and-lift-eeg-detection](https://link.zhihu.com/?target=https%3A//www.kaggle.com/c/grasp-and-lift-eeg-detection)。数据由几个.csv 文件组成。这些文件分别是：
- EEG 数据，用作模型的输入，由固定在患者头皮上的 32 个电极记录。数据以 500 Hz 的频率进行记录。
- 人体测试者试图实现的运动的逐帧标签，一共有 6 个标签，每帧的标签是 6 个标签中的一个。

通过记录不同人体测试者在执行简单动作（例如抓取和提升物体）时的脑电图来收集数据。因此，我们按不同的场景、主题对数据集进行分类。我们将在稍后的准确率预测中看到，脑电波可能非常个性化，因为模型可以非常准确地预测同一个人在没见过的场景中的意图，但如果训练不够多样，那么与新的测试人员进行同样的训练会很困难。

因此，目标是创建一个神经网络，该网络将脑电图数据作为输入，并输出测试者试图实现的 6 个可能动作的概率分布。由于「no action」不属于 6 个类别，因此我们可以将其添加为类，或者将所有可能的输出设置为 0 到 1 之间的值，并使用阈值来确定是否检测到动作。如果每一个动作都低于阈值，则我们就认为没有动作。
![](https://pic2.zhimg.com/v2-ab2f9a3b1fa22c9b883fc04fc0d5010d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='575' height='586'></svg>)
电极位置，来源：[https://www.kaggle.com/c/grasp-and-lift-eeg-detection/data](https://link.zhihu.com/?target=https%3A//www.kaggle.com/c/grasp-and-lift-eeg-detection/data)

我制作了这些电极活动的动画数据可视化。由于采样频率相当高（500 Hz），因此我使用了简单的 3 步低通滤波器来平滑数据，并使用前 100 帧（约 1/5 秒）创建动画。
![](https://pic1.zhimg.com/v2-fc91fc94ce7ba8a0da115cb2035db2e0_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='432' height='288'></svg>)32 个电极在前 1/5 秒内的激活状态
我们还可以将时序数据可视化为 2D 热图，其中纵轴代表时间（从上到下递增），横轴表示 32 个电极。
![](https://pic1.zhimg.com/v2-f3d3b7258f5af5b527af5607db8a1250_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='472' height='469'></svg>)EGG 时序热图（时间从上到下增大）
这也非常有用，因为正如我们将看到的，它允许我们使用「时空卷积」。




**数据预处理**

为方便学习阶段，我们应对原始数据进行预处理。例如，与执行动作的较低变化率相比，非常高的 EEG 采样频率会引发许多问题：数据变化非常快，但是动作实际上保持不变，因此波动几乎可以被认为是噪声。此外，时序模型接收大量快速变化的数据，而分类输出从不改变。

第一个可能的步骤是「使用低通滤波器过滤数据」。即使是简单的运行平均值也能起作用：通过这种方式，我们缓解了数据的高频变化，同时保留了更有用的低频结构，因为我们即将分类的动作具有非常低的变化频率（最多 1Hz）。之后，我们可以对数据进行二次采样，即每 10100 个数据点只保留一个数据点。从某种意义上说，这也有助于降低时间维度及数据的相关性，从而使数据更加时间稀疏。

也可以采用其他预处理技术，但是为了简洁起见，我们就此打住，开始设计神经网络。




**神经网络设计与实验**

处理时序数据时，我们首先想到的架构之一是「循环神经网络」。这些网络具备动态结构，其内部状态允许它们对时序数据进行编码，因此这些网络还基于过去的输入计算输出。我在 Keras 中设计了一个 LSTM 网络，并为其提供了具备连续时序结构的训练数据。结果很好，但在这个特定的例子中，我更感兴趣的是展示一个通常用于图像的卷积神经网络如何很好地应用到时序数据上。

如前所述，从某种意义上来说，我们实际上是在处理时空数据：以上热图的纵轴表示时间演化，而横轴表示各种电极，结果相近的电极在人类头皮上的空间位置往往也很接近。这意味着我们可以用卷积提取有用特征：2D 卷积核将同时在时间和空间上编码模式。设想一个 3*3 卷积核：它能够在热图中的矩阵上，通过在 3 个不同时间步（3 个核行）以及在 3 个不同的电极（3 个核列）上进行加权求和，来提取特征。因此，具有许多核的 CNN 可以发现电极的激活在与想要的动作相关的有限时间周期上的变化特征」。

我在 Keras 中实现了一个简单的 CNN，来检查它在这个数据集上的性能。


```
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout
from keras.layers import Embedding
from keras.layers import LSTM, CuDNNLSTM, BatchNormalization, Conv2D, Flatten, MaxPooling2D, Dropout
from keras.optimizers import Adam
model = Sequential()
#model.add(CuDNNLSTM(128, input_shape = (time_steps//subsample, 32)))
model.add(Conv2D(filters = 64, kernel_size = (7,7), padding = "same", activation = "relu", input_shape = (time_steps//subsample, 32, 1)))
model.add(BatchNormalization())
#model.add(MaxPooling2D(pool_size = (3,3)))
model.add(Conv2D(filters = 64, kernel_size = (5,5), padding = "same", activation = "relu", input_shape = (time_steps//subsample, 32, 1)))
model.add(BatchNormalization())
#model.add(MaxPooling2D(pool_size = (3,3)))
model.add(Conv2D(filters = 64, kernel_size = (3,3), padding = "same", activation = "relu", input_shape = (time_steps//subsample, 32, 1)))
model.add(BatchNormalization())
#model.add(MaxPooling2D(pool_size = (3,3)))
model.add(Flatten())
#model.add(Dropout(0.2))
model.add(Dense(32, activation = "relu"))
model.add(BatchNormalization())
# model.add(Dropout(0.2))
model.add(Dense(6, activation = "sigmoid"))
adam = Adam(lr = 0.001)
model.compile(optimizer = adam, loss = "categorical_crossentropy", metrics = ["accuracy"])
model.summary()
```


为了检查模型的性能，如 Kaggle 竞赛中所建议的，我们可以查看 AUC 分数。如果不熟悉 AUC，建议读一下这个清晰直观的解释（[https://datascience.stackexchange.com/questions/806/advantages-of-auc-vs-standard-accuracy](https://link.zhihu.com/?target=https%3A//datascience.stackexchange.com/questions/806/advantages-of-auc-vs-standard-accuracy)）。从代码所在 notebook 中可以看到，我们可以在快速训练阶段达到约 0.85 的 AUC 分数。




**结论**

在这篇文章中，我们介绍了脑电信号与脑电图，后者是一种利用用户头皮上的电极记录有用信号的非侵入式且相对简单的方法。我们看到了一些直观的数据可视化，以及如何使用神经网络从这些数据中提取运动意向等特征。我相信这一领域（机器假肢、脑机接口）将会因为深度学习而得到深入发展。

这些技术的影响将是巨大的。拥有能够以自然方式控制的低成本假肢可以极大地改善数百万人的生活。

建议查看近期启动的 Symbionic Project 项目，参与项目的天才们尝试创造一种低成本的智能手臂假肢，该假肢可以利用肌肉激活控制，目的是实现这种装置的平民化。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[https://towardsdatascience.com/from-brain-waves-to-arm-movements-with-deep-learning-an-introduction-3c2a8b535ece](https://link.zhihu.com/?target=https%3A//towardsdatascience.com/from-brain-waves-to-arm-movements-with-deep-learning-an-introduction-3c2a8b535ece)


