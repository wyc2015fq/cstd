
# LSTM模型分析及对时序数据预测的具体实现（python实现） - OraYang的博客 - CSDN博客

2017年09月30日 10:28:08[OraYang](https://me.csdn.net/u010665216)阅读数：6215所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



> 具体代码看代码云：
> [传送门](https://gitee.com/orayang_admin/LSTM_prediction)
> ，有问题欢迎随时私信~

## 引言
这篇博客衔接上一篇博客：[Holt-Winters模型原理分析及代码实现（python)](http://blog.csdn.net/u010665216/article/details/78051192)，我们在三次指数平滑的基础上，来进一步讨论下对时序数据的预测。
## LSTM原理分析(参考博文：
## [Understanding LSTM Networks](http://colah.github.io/posts/2015-08-Understanding-LSTMs/)
## ）
Long Short Term 网络–LSTM，是神经网络的一种简单延伸，也是一种特殊的RNN模型。可以用来学习长期依赖的信息。LSTM 由[Hochreiter & Schmidhuber (1997)](http://deeplearning.cs.cmu.edu/pdfs/Hochreiter97_lstm.pdf)提出，并在近期被[Alex Graves](https://scholar.google.com/citations?user=DaFHynwAAAAJ&hl=en)进行了改良和推广。LSTM在语言模型，图像捕捉等领域有着极其广泛的应用。
所有 RNN 都具有一种重复神经网络模块的链式的形式。在标准的 RNN 中，这个重复的模块只有一个非常简单的结构，例如一个 tanh 层。如下图所示：
![RNN](https://img-blog.csdn.net/20170930101201925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170930101201925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
LSTM 同样是这样的结构，但是重复的模块拥有一个不同的结构。不同于 单一神经网络层，这里是有四个，以一种非常特殊的方式进行交互。如下图所示：
![LSTM](https://img-blog.csdn.net/20170930101438165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170930101438165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
那么我们可以知道LSTM实现长期记忆的必要条件如下：
[
](https://img-blog.csdn.net/20170930101438165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 增加遗忘机制。例如当一个场景结束是，模型应该重置场景的相关信息，例如位置、时间等。而一个角色死亡，模型也应该记住这一点。所以，我们希望模型学会一个独立的忘记/记忆机制，当有新的输入时，模型应该知道哪些信息应该丢掉。

> 如下图所示：
![LSTM-focus-t](https://img-blog.csdn.net/20170930101833463?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170930101833463?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 增加保存机制。当模型看到一副新图的时候，需要学会其中是否有值得使用和保存的信息。

> 如下图所示：
![LSTM-focus-s](https://img-blog.csdn.net/20170930101927959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170930101927959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 所以当有一个新的输入时，模型首先忘掉哪些用不上的长期记忆信息，然后学习新输入有什么值得使用的信息，然后存入长期记忆中。

> 如下图所示：
![LSTM-c](https://img-blog.csdn.net/20170930102130431?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170930102130431?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 把长期记忆聚焦到工作记忆中。最后，模型需要学会长期记忆的哪些部分立即能派上用场。不要一直使用完整的长期记忆，而要知道哪些部分是重点。

> 如下图所示：
![LSTM-o](https://img-blog.csdn.net/20170930102207295?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> python实现

> 环境

> Python 3.6

> TensorFlow

> Numpy

> Keras

> Matplotlib

> 构造数据

> 构造一个-50~50步长为1，大小为sinx的数据序列，代码如下图所示：

> x = np.arange(-
> 50.0
> ,
> 50.0
> ,
> 1
> )
y1 = np.sin(x)
y1 = np.array(y1)
plt.plot(x, y1,
> 'ko--'
> )
plt.show()
> 如下图所示：
![lstm_data](https://img-blog.csdn.net/20171006140911286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 构造lstm需要的数据格式

> Keras LSTM层的工作方式是通过接收3维（N，W，F）的数字阵列，其中N是训练序列的数目，W是序列长度，F是每个序列的特征数目。我使用了[1,5,20,1]的网络结构，其中我们有1个输入层（由大小为50的序列组成），该输入层喂食5个神经元给LSTM层，接着该LSTM层喂食20个神经元给另一个LSTM层，然后使用一个线性激活函数来喂食一个完全连接的正常层以用于下一个时间步的预测。

> x_train = np.reshape(x_train, (x_train.shape[
> 0
> ], x_train.shape[
> 1
> ],
> 1
> ))
x_test = np.reshape(x_test, (x_test.shape[
> 0
> ], x_test.shape[
> 1
> ],
> 1
> ))  
.
.
.
model = lstm.build_model([
> 1
> ,
> 5
> ,
> 20
> ,
> 1
> ])
model.fit(
            x_train,
            y_train,
            batch_size=
> 512
> ,
            nb_epoch=epochs,
            validation_split=
> 0.05
> )
> 预测及结果

> predicted = lstm.predict_point_by_point(model, x_test)
plot_results(predicted, y_test)
> 如下图所示：
![lstm_pred](https://img-blog.csdn.net/20171006141520567?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

