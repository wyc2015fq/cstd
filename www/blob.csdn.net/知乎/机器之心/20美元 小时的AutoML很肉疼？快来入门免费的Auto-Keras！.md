# 20美元/小时的AutoML很肉疼？快来入门免费的Auto-Keras！ - 知乎
# 



选自pyimagesearch，作者：Adrian Rosebrock ，参与：Geek AI、张倩。

> 谷歌 AutoML 的出现使得普通深度学习从业者也能以最少的领域知识来训练模型，但这项服务每小时收费 20 美元，是不是很肉疼？所幸，AutoKeras 为广大从业者带来了福音。这种开源 Python 包为昂贵的 AutoML 提供了免费替代品，它所有的代码都是开源的！本文作者将介绍如何用 Auto-Keras 进行自动化的机器学习和深度学习。

当深度学习从业人员在数据集上训练神经网络时，他们主要有两个目标：
- 定义符合数据集特性的神经网络架构。
- 在许多试验中对一组超参数进行调优，从而使得模型具有较高的准确率并且能够泛化至训练集和测试集之外的数据。举例而言，需要进行调优的典型参数包括优化器算法（随机梯度下降，Adam 等）、学习率、学习率调度以及正则化方法。




针对不同的数据集和问题，深度学习专家需要进行几十至上百次的实验才能找到神经网络架构和超参数之间的平衡。

这些实验可能需要在 GPU 上花费几百到上千小时的计算时间。

这样的情况还只是对于深度学习专家来说，那么对于那些普通的深度学习从业者来说又将如何呢？

下面来看看 Auto-Keras 和 AutoML 这两种工具吧：

Auto-Keras 和 AutoML 的最终目的都是通过使用自动化的神经架构搜索（NAS）算法减少展开机器学习和深度学习工作的障碍。

Auto-Keras 和 AutoML 使非深度学习专家能够以最少的领域知识（深度学习或实际数据）来训练其模型。

通过 AutoML 和 Auto-Keras，一个具备最少机器学习专业知识的程序员也可以应用这些算法，以最少的努力达到最佳水准。

那么，如果你想知道为什么，请继续阅读下文。

想了解更多的关于 AutoML 的信息（以及如何使用 Auto-Keras 自动训练神经网络并进行调优），也请继续读下去。

**Auto-Keras 和 AutoML：入门指南**

接下来，我们将先讨论自动化机器学习（AutoML）以及神经架构搜索（NAS）算法，NAS 使我们可以将 AutoML 应用于神经网络和深度学习。

我们还将简要地讨论谷歌 AutoML，这是一套工具和程序库，它使机器学习专业知识有限的程序员能够在其数据上训练出具备高准确率的模型。

当然，谷歌的 AutoML 是一种专有算法（也有点小贵）。

AutoML 的一种替代方案是开源的 Auto-Keras，它是在 Keras 和 PyTorch 的基础上构建的。

我们将向读者展示如何用 Auto-Keras 自动训练神经网络，并对其进行评估。

**什么是自动化机器学习（AutoML）？**
![](https://pic1.zhimg.com/v2-6296a76f410f0b466d6d545465b2dec8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='184'></svg>)图 1：Auto-Keras 是谷歌 AutoML 的替代方案。这些软件项目可以帮助你在很少干预的情况下自动训练模型。对于从事深度学习工作的新手来说，它们是很好的备选方案。
除了无监督学习（从无标签数据中自动学习模式），对于非专家从业者来说，自动化机器学习一直是一个梦想。

想象一下通过以下方法自动创建机器学习模型：
- 安装一个程序库/使用一个 web 接口
- 将程序库/接口指向数据
- 自动在数据上训练模型，无需调整参数/深入了解驱动它的算法




有些公司正试图创建这样的解决方案——谷歌 AutoML 就是其中一支重要的力量。

谷歌 AutoML 使机器学习经验非常有限的开发者和工程师们能够在自己的数据集上自动训练神经网络。

在谷歌 AutoML 框架下，将进行如下的算法迭代：
- 在一个训练集上训练一个网络
- 在一个测试集上评估上一步训练出的网络
- 修正神经网络架构
- 对超参数调优
- 重复以上过程




使用 AutoML 的程序员或工程师无需定义其神经网络架构或对超参数调优——AutoML 会自动帮他们完成这些工作。

**神经架构搜索（NAS）使得 AutoML 成为可能**
![](https://pic3.zhimg.com/v2-fbf1c3a376bd1a8f7f2d86f4bea6b772_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='357'></svg>)
图 2：图中总结了神经架构搜索（NAS）生成一个网络模型的过程，上图中，这些网络正在搜索适用于 CIFAR-10 数据集的最佳 CNN 架构。（图源：《Learning Transferable Architectures for Scalable Image Recognition》图 4）

谷歌 AutoML 和 Auto-Keras 都是由神经架构搜索（NAS）算法驱动的。给定输入数据集，NAS 算法将自动搜索最优的网络架构以及相应的参数。

本质上来说，NAS 就是用一套能够自动对模型进行调优的算法替代了深度学习工程师/从业者！

在计算机视觉和图像识别的任务中，神经架构搜索算法将：
- 接收输入的训练数据集
- 优化并找到称为「cells」的架构构建模块，这些 cells 是 NAS 自动习得的，看起来可能类似于 Inception 网络、残差网络或 squeeze/fire 网络微型架构
- 持续训练并搜索「NAS 搜索空间」，寻找更优的单元结构




如果 AutoML 系统的用户是经验丰富的深度学习从业者，那他们可以选择：
- 在比训练数据集小得多的子集上运行 NAS
- 找到一组最优的架构构建模块/单元
- 使用这些单元并在架构搜索过程中手动定义一个更深版本的网络
- 利用其专业知识和实践经验在完整的训练集上训练网络




这种方法结合了完全自动化机器学习解决方案和需要专业深度学习从业者参与的方法，它的准确率通常比 NAS 自己发现的网络架构方法更高。

如果你想了解这些算法的工作细节，可以阅读两篇论文：《Neural Architecture Search with Reinforcement Learning》和《Learning Transferable Architectures for Scalable Image Recognition》

**Auto-Keras：谷歌 AutoML 的开源替代方案**
![](https://pic4.zhimg.com/v2-ebfa6f4dcaa5ea0bab1cb4182f6b8d9b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='196'></svg>)图 3：Auto-Keras 程序包由德州农工大学的「DATA Lab」团队开发
由德州农工大学「DATA Lab」团队开发的 Auto-Keras 程序包是谷歌 AutoML 的一种替代方案。Auto-Keras 也使用了神经架构搜索技术，但是应用了「网络态射」（network morphism，在改变网络架构的同时保持网络所具备的功能），并使用贝叶斯优化引导网络态射，以实现更高效的神经网络搜索。

你可以在 Jin 等人 2018 年发表的论文《Auto-Keras: Efficient Neural Architecture Search with Network Morphism》中找到更多关于 Auto-Keras 框架的技术细节。

**项目结构**

接下来，你可以从本文的「Downloads」部分下载所需代码的压缩包，然后解压文件，并通过终端导航至解压的文件夹。

我们可以用「tree」命令查看项目结构：


```
$ tree --dirsfirst
.
├── output
│   ├── 14400.txt
│   ├── 28800.txt
│   ├── 3600.txt
│   ├── 43200.txt
│   ├── 7200.txt
│   └── 86400.txt
└── train_auto_keras.py

1 directory, 7 files
}
```





接下来，我们将以 Python 脚本「train_auto_keras.py」为例展开讨论。

因为会有很多运行结果输出到屏幕上，所以我们选择将分类报告（在 scikit-learn 的「classification_report」工具的帮助下生成）作为文本文件保存到磁盘上。查看上图所示的「output」/文件夹，可以看到一些已经生成的报告。你可以打印一个报告到终端对话框中（例如，cat output/14400.txt），看看运行结果如何。

**安装 Auto-Keras**
![](https://pic2.zhimg.com/v2-0ef002b8ea0aecdff801b0dc4af7f5e1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='400'></svg>)图 4：Auto-Keras 程序包依赖于上图所示的 Python 3.6、TensorFlow 以及 Keras
根据 Github 代码库中的描述，Auto-Keras 目前还处于「预发布」状态，也就是说它还没有正式发布。

其次，Auto-Keras 需要 Python 3.6，并且只与 Python 3.6 兼容。如果你正在使用其它版本的 Python，就无法使用 Auto-Keras 程序包。

你可以通过以下命令查看自己的 Python 版本：

`$ python --version`




当你已经准备好 Python 3.6 时，可以通过下面的「pip」命令安装 Auto-keras：


```
$ pip install tensorflow # or tensorflow-gpu
$ pip install keras
$ pip install autokeras
```





如果你在安装或使用 Auto-Keras 的过程中遇到了任何问题，可以将问题提交到其官方 Github 问题页面，Auto-Keras 的作者可能会为你提供帮助。

**用 Auto-Keras 实现训练脚本**

我们用 Auto-Keras 实现训练脚本。打开「train_auto_keras.py」文件并嵌入以下代码：


```
# import the necessary packages
from sklearn.metrics import classification_report
from keras.datasets import cifar10
import autokeras as ak
import os

def main():
    # initialize the output directory
    OUTPUT_PATH = "output"
```





首先，请引入第 2-5 行的项目所必需的程序包：
- 如前所述，我们将使用 scikit-learn 的「classification_report」来计算需要保存到输出文件中的统计信息。
- 我们将使用 CIFAR-10 数据集，然后轻松地将其集成到「keras.datasets」中。
- 接着，我们将引入最重要的「autokeras」包，将其简写为「ak」。
- 我们还需要引入「os」模块，因为在构建输出文件路径时，我们将在各种操作系统上适配路径分隔符。




在第 7 行，我们定义脚本的「main」函数。由于 Auto-Keras 和 TensorFlow 处理线程的方式，我们需要将代码封装在一个「main」函数中。更多详细信息，请参见 GitHub 问题表单：[https://github.com/jhfjhfj1/autokeras/issues/311](https://link.zhihu.com/?target=https%3A//github.com/jhfjhfj1/autokeras/issues/311)

在第 9 行，我们定义输出路径「OUTPUT_PATH」。

接下来，我们将为 Auto-Keras 初始化一个训练时间列表。


```
# initialize the list of training times that we'll allow
    # Auto-Keras to train for
    TRAINING_TIMES = [
        60 * 60,        # 1 hour
        60 * 60 * 2,    # 2 hours
        60 * 60 * 4,    # 4 hours
        60 * 60 * 8,    # 8 hours
        60 * 60 * 12,   # 12 hours
        60 * 60 * 24,   # 24 hours
    ]
```





第 13-20 行定义了一组训练时间「TRAINING_TIMES」，包括「1, 2, 4, 8, 12, 24」小时。我们将使用 Auto-Keras 探究更长的训练时长对准确率的影响。

接下来，我们将加载 CIFAR-10 数据集，并初始化各个图片的类别名：


```
# load the training and testing data, then scale it into the
    # range [0, 1]
    print("[INFO] loading CIFAR-10 data...")
    ((trainX, trainY), (testX, testY)) = cifar10.load_data()
    trainX = trainX.astype("float") / 255.0
    testX = testX.astype("float") / 255.0

    # initialize the label names for the CIFAR-10 dataset
    labelNames = ["airplane", "automobile", "bird", "cat", "deer",
        "dog", "frog", "horse", "ship", "truck"]
```





本文所使用的 CIFAR-10 数据将被加载、存储到第 25 行所示的训练/测试切片片段中。随后我们会将这些数据进行标准化处理，映射到 [0，1] 的区间上（如第 26、27 行代码所示）。我们在第 30、31 行中初始化类名（labelNames）。CIFAR-10 数据集中包含了这 10 类图片。请注意，这里类名的顺序是十分重要的。

接下来，我们将开始根据「TRAINING_TIMES」进行循环工作，每次都会使用到 Auto-Keras：


```
# loop over the number of seconds to allow the current Auto-Keras
    # model to train for
    for seconds in TRAINING_TIMES:
        # train our Auto-Keras model
        print("[INFO] training model for {} seconds max...".format(
            seconds))
        model = ak.ImageClassifier(verbose=True)
        model.fit(trainX, trainY, time_limit=seconds)
        model.final_fit(trainX, trainY, testX, testY, retrain=True)

        # evaluate the Auto-Keras model
        score = model.evaluate(testX, testY)
        predictions = model.predict(testX)
        report = classification_report(testY, predictions,
            target_names=labelNames)

        # write the report to disk
        p = os.path.sep.join(OUTPUT_PATH, "{}.txt".format(seconds))
        f = open(p, "w")
        f.write(report)
        f.write("\nscore: {}".format(score))
        f.close()
```





上面的代码块是本文项目脚本的核心部分。在第 35 行，我们对每一个「TRAINING_TIMES」定义了一个循环的工作流，我们将在每一轮工作流中：
- 初始化我们的模型（ak.ImageClassifier），并且启动训练（第 39、40 行）。请注意，我们并不会为一类特定的卷积神经网络实例化一个对象，也不需要跟往常一样对超参数进行调优。Auto-Keras 会帮我们处理所有这些工作，并且生成其发现结果的报告。
- 一旦达到了时间限制，工作流将利用 Auto-Keras 寻找到的最佳模型和参数再次训练模型（第 41 行）。
- 对模型进行评价并构建分类报告（第 44-47 行）。
- 将分类报告和准确率得分一同写入磁盘，从而使我们能够评价更长训练时间的效果（第 50-54 行）。




我们将根据「TRAINING_TIMES」中的各个训练时间重复运行这个过程。

最终，我们将检查代码并启动程序运行的主（main）线程：


```
# if this is the main thread of execution then start the process (our
# code must be wrapped like this to avoid threading issues with
# TensorFlow)
if __name__ == "__main__":
    main()
```





在这里，我们需要进行检查，确保这是程序执行的主线程，然后编写主函数。

仅仅使用这 60 行代码，我们就使用 CIFAR-10 数据集完成了 Auto-Keras 的样例脚本编写工作。但我们的工作还没有结束...

**利用 Auto-Keras 训练一个神经网络**

接下来，我们将使用 Auto-Keras 训练我们自己的神经网络。

请一定要使用本教程「Downloads」章节提到的方法下载项目所需的源代码。

接着，请打开一个终端，将工作路径导航至你下载源代码的地方，并执行下面的命令：


```
$ python train_auto_keras.py
[INFO] training model for 3600 seconds max...   
Preprocessing the images.
Preprocessing finished.

Initializing search.
Initialization finished.


+----------------------------------------------+
|               Training model 0               |
+----------------------------------------------+
Using TensorFlow backend.

No loss decrease after 5 epochs.


Saving model.
+--------------------------------------------------------------------------+
|        Model ID        |          Loss          |      Metric Value      |
+--------------------------------------------------------------------------+
|           0            |   4.816269397735596    |         0.5852         |
+--------------------------------------------------------------------------+


+----------------------------------------------+
|               Training model 1               |
+----------------------------------------------+
Using TensorFlow backend.
Epoch-14, Current Metric - 0.83:  28%|██████▊                 | 110/387 [01:02<02:46,  1.67 batch/s]Time is out.
[INFO] training model for 86400 seconds max...  
Preprocessing the images.
Preprocessing finished.

Initializing search.
Initialization finished.


+----------------------------------------------+
|               Training model 0               |
+----------------------------------------------+
Using TensorFlow backend.

No loss decrease after 5 epochs.
...
+----------------------------------------------+
|              Training model 21               |
+----------------------------------------------+
Using TensorFlow backend.

No loss decrease after 5 epochs.


+--------------------------------------------------------------------------+
|    Father Model ID     |                 Added Operation                 |
+--------------------------------------------------------------------------+
|                        |             to_deeper_model 16 ReLU             |
|           16           |               to_wider_model 16 64              |
+--------------------------------------------------------------------------+

Saving model.
+--------------------------------------------------------------------------+
|        Model ID        |          Loss          |      Metric Value      |
+--------------------------------------------------------------------------+
|           21           |   0.8843476831912994   |   0.9316000000000001   |
+--------------------------------------------------------------------------+


+----------------------------------------------+
|              Training model 22               |
+----------------------------------------------+
Using TensorFlow backend.
Epoch-3, Current Metric - 0.9:  80%|████████████████████▊     | 310/387 [03:50<00:58,  1.31 batch/s]Time is out.

No loss decrease after 30 epochs.
```


如上图所示，我们的脚本指导 Auto-Keras 执行了 6 组实验。

在英伟达的 K80 GPU 上，总的训练时间（包括时间限制和模型重新拟合所需的时间）约为 3 天多一点。

**Auto-Keras 的运行结果**
![](https://pic1.zhimg.com/v2-07682bd3801301667b1e81bb6bdb73d0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='371'></svg>)
图 5：使用 Auto-Keras 通常是一个非常耗时的过程。用 Auto-Keras 训练 8-12 个小时将得到适用于 CIFAR-10 的最佳网络模型。在此之后，Auto-Keras 无法进一步进行优化。

在上面的图 5 中，你可以看到在使用 Auto-Keras 时训练时长（x 轴所示）对于模型整体准确率（y 轴所示）的影响。

当训练时间较短时（1-2 小时），模型的准确率约为 73%。训练 4 个小时，模型就能达到 93% 的准确率。

在 8-12 小时训练时间范围内获得的准确率最高，达到了 95%。

超过 8-1 2 小时的训练并不能进一步提高模型的准确率，这意味着我们已经达到了性能的饱和点，Auto-Keras 无法进一步进行优化。

**Auto-Keras 和 AutoML 有意义吗？**
![](https://pic1.zhimg.com/v2-b62acbd116122d75abd82b95736486bc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='450' height='272'></svg>)
图 6：Auto-Keras（或 AutoML）有意义吗？这无疑是业界向前迈出的一大步，尤其是可以帮助那些不具备深度学习领域知识的人。而且，经验丰富的深度学习专家可以在更短的时间内创建网络架构、对其进行训练，并且达到与手动构造网络相当的、甚至更好的模型准确率。

对于非专家的从业者来说，除了无监督学习（从无标签的数据中自动学习模式）之外，自动机器学习被认为是机器学习的「圣杯」。

谷歌的 AutoML 和开源的 Auto-Keras 程序包都试图为大众提供机器学习解决方案，即使使用者不具备重要的技术经验。

Auto-Keras 在 CIFAR-10 上的效果还不错，笔者根据之前撰写的关于深度学习、医学图像和疟疾检测的文章进行了第二组实验。

在那篇文章中，笔者使用一个简化的 ResNet 架构，模型在经过了 1 个小时的训练后获得了 97.1% 的准确率。

然后，笔者让 Auto-Keras 在相同的数据集上运行了 24 小时，结果只获得了 96% 的准确率（低于手工定义的架构）。

谷歌的 AutoML 和 Auto-Keras 都是巨大的技术进步；然而，自动化机器学习的问题还远远没有得到解决。

目前，相对于自动化机器学习技术，更加重要的还是掌握深度学习专业知识。这些领域的专业知识（特别是关于所使用的数据的知识），对于提高模型准确率非常关键。

笔者的建议是：仍然要丰富自己的专业知识，不要依赖自动机器学习算法。

要成为一个成功的深度学习从业者和工程师，你需要在工作中使用恰当的工具。使用 AutoML 和 Auto-Keras 作为工具，然后继续用其它知识充盈你自己的工具箱。

**总结**

本文讨论了 Auto-Keras 和 AutoML，这是一组进行自动化的机器学习和深度学习的工具和程序库。

Auto-Keras 和 AutoML 的最终目标都是通过使用神经网络架构搜索（NAS）算法，减少人们开始进行机器学习和深度学习任务的障碍。

NAS 算法是 Auto-Keras 和 AutoML 的基石，它将自动地：
- 定义并优化一个神经网络架构
- 对模型的超参数进行调优




使用该框架的主要的好处有：
- 可以在具备非常有限的专业知识的情况下，执行机器学习和深度学习任务
- 获得高模型准确率，并且具备泛化到训练集和测试集以外的数据上的能力
- 使用图形化交互界面或一个简单的应用程序接口快速启动、运行项目
- 在不费太大力气的情况下，可能达到目前最好的模型效果




当然，这是要付费的，事实上费用来自于两部分。

首先，谷歌的 AutoML 很贵，大约需要每小时 20 美元。为了节省资金，你可以使用 Auto-Keras，它是谷歌 AutoML 的开源替代方案，但是你仍然需要为 GPU 计算时间付费。用 NAS 算法代替真实的深度学习专家将需要数小时的计算来搜索最优参数。

虽然我们在 CIFAR-10 数据集上寻找到了具备高准确率的模型（~96% 的准确率），但是当笔者将 Auto-Keras 应用到之前关于医学深度学习和疟疾预测的文章上时，Auto-Keras 的准确率仅为 96.1%，比笔者 97% 的准确率低了整整一个百分点（而且 Auto-Keras 需要多用 2300% 的计算时间！

尽管 Auto-Keras 和 AutoML 可能是人们在自动机器学习和深度学习方面朝着正确方向进行的一步探索，但这一领域仍有大量的工作有待完成。

并没有什么用现有的算法解决机器学习、深度学习问题的灵丹妙药。相反，笔者建议深度学习从业者和工程师们不断丰富自己的知识储备。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
原文链接：[https://www.pyimagesearch.com/2019/01/07/auto-keras-and-automl-a-getting-started-guide/](https://link.zhihu.com/?target=https%3A//www.pyimagesearch.com/2019/01/07/auto-keras-and-automl-a-getting-started-guide/)


