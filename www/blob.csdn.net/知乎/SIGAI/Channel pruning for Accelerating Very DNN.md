# Channel pruning for Accelerating Very DNN - 知乎
# 

**PDF全文链接：**[论文解读Channel pruning for Accelerating Very Deep Neural Networks](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_134.html)

本文提出了一种新的裁枝方法，用于加速深层卷积神经网络。对于一个训练好的模型，本文方法通过一个2步迭代的算法逐层裁枝，优化函数是LASSO回归和最小二乘法重建误差。进一步，本文将算法推广到多层的裁枝，和多分枝网络的裁枝。结果上，本文的方法减少了累积误差，且适用于各种网络结构。针对于VGG16网络，本文方法可以在加速5倍的条件下，准确率仅下降0.3%；针对ResNet，Xception网络加速2倍，准确率分别下降1.4%，1.0%

## **一．简介**

加速卷积神经网络的方法主要可以分三个方面：1. 针对卷积操作优化，例如使用FFT实现卷积操作；2. 量化操作，例如网络的二值化（BinaryNet）；3. 在结构上简化，使模型变小。本工作属于第三种。在结构上简化模型也可以分三类：张量分解、连接稀疏化，基于通道的裁枝。首先张量分解是将张量分解成多个小张量，但是输出的通道数并没有变化，因此对于 ![1*1](https://www.zhihu.com/equation?tex=1%2A1) 的卷积层很难通过张量分解的方法做压缩，而当前很多模型结构都用到了大量的 ![1*1](https://www.zhihu.com/equation?tex=1%2A1) 卷积（例如ResNet，GoogleNet， Xception等）。其次连接稀疏化是将两层之间的连接变稀疏，但是这种稀疏化处理通常是没有固定模式规律的，所以尽管理论上有很高的加速效果，但是实际实现很复杂，因为通过稀疏化处理，数据无法再通过原来的张量存储，需要使用稀疏矩阵/稀疏张量来存储，那么卷积操作也变成稀疏卷积。最后相比于前两种方法，基于通道的裁枝既可以减少通道数，又不会改变数据的存储方式，因此其对于CPU和GPU都有很好的加速效果，同时也不需要实现特殊的卷积操作。

虽然基于通道的裁枝有以上的好处，但是也存在相应的难点。首先，改变一层的通道数，对下一层卷积的输入也是有影响的（输入的特征图通道数变少了）。其次，如何选取删掉的通道也是一个问题。【1，49】工作提出在从头训练的时候，就强制参数稀疏化，在训练过程中即确定哪些参数是不重要的，但是问题在于从头训练十分耗时，而且这种加入参数稀疏化的损失函数也不容易优化。【31，3】工作则是直接对训练好的模型做裁枝，着眼于分析单个权重的重要性，但是为了保证很好的准确率，加速效果就有很大的限制。

本文同样也是针对训练好的模型做裁枝，参考张量分解中重建特征图的优化方法，本文不去考虑单个参数的重要性，而是直接最小化输出特征图的重建误差，逐层地做裁枝，如图1所示。我们想要裁掉B层到C层之间卷积的若干通道，但由于通道被裁掉，相应B层的输入对应通道的特征图也需要被裁掉，而产生这些特征图的是A层到B层的对应的filter（图中虚线的filter），一次裁掉B层到C层的若干通道，同样也可以删去A层到B层的若干filter。裁枝后如何保证裁掉的参数是正确的呢？设原本C层特征图为Mc，裁枝后，到C层的特征图为Mc’，则要使Mc’尽可能与Mc相同

![](https://pic1.zhimg.com/v2-490c229e36aade125ef11aff93ba6288_b.jpg)图1 最小化特征图的重建误差

最小化重建误差的过程可以分为2个步骤：通道选择，特征图重建。第一步是需要找出最具代表性的通道，然后将其他的通道删掉，这一步是基于LASSO回归做的；第二部是利用选取的通道重建C层的特征图，这一步是使用最小均方误差来表示重建误差。本文进一步逐层地做裁枝，并计算积累误差，最后本文也讨论了多分枝网络结构的裁枝方法。

通过本文的方法，可以对VGG16实现4倍的加速效果，top-5错误率增加1.0%，若进一步结合张量分解的方法，可以实现5倍的加速效果，二错误率仅增加0.3%。针对ResNet-50和Xception-50网络，实现2倍的加速，错误率分别增加1.4%，1.0%

## **二．相关工作**

大多加速卷积神经网络的方法可以归为3类：1. 针对卷积操作优化【36，48，27，4】；2. 量化操作【8，41】；3. 在结构上简化，使模型变小。第3类方法又可以分3种方法：张量分解【22，28，13，24】、连接稀疏化【17，33，29，15，14，52】、基于通道的裁枝。其中张量分解将整个参数张量由若干小张量表示，针对全连接层，则利用SVD分解；连接稀疏化则通过单个参数的绝对值来衡量参数的重要性，理论上可以达到很好的加速效果，但是实际的加速效果却依赖于实现细节，因为参数的存储和卷积操作都需要额外实现。

基于通道的裁枝方法则是以通道为单位，裁剪掉不重要的参数。【1，49，54】工作通过在训练中加入使参数稀疏化的正则项，来简化后续裁枝步骤的难度，但是训练成本很高，而且不容易拟合。【45，35，19】等工作则直接针对训练好的模型做裁枝，但往往效果并不理想。

## **三．方法介绍**

**单层裁枝**

继续看图1，我们在简介中大致描述了本文单层的裁枝方法：我们想要裁掉B层到C层之间卷积的若干通道，设原本C层特征图为Mc，裁枝后，到C层的特征图为Mc’，则要使Mc’尽可能与Mc相同

第一步是选择通道，第二步是特征图重建，目的是最小化重建误差，本文提出两步迭代的算法：首先选取最具代表性的通道，即裁剪B层到C层的卷积；其次重建特征图，调整B层到C层的参数W，使C层特征图重建误差最小。迭代交替进行以上两步。

假设B层到C层的卷积核参数为W，其尺寸为 ![n \times c \times k_{n} \times k_{w} ](https://www.zhihu.com/equation?tex=n+%5Ctimes+c+%5Ctimes+k_%7Bn%7D+%5Ctimes+k_%7Bw%7D+) ，其中n为输出层C层特征图的通道数，c为输入层B层特征图的通道数， ![k_{n}\times k_{w}](https://www.zhihu.com/equation?tex=k_%7Bn%7D%5Ctimes+k_%7Bw%7D) 为卷积核尺寸，假设输入特征图为X，输出特征图为Y，设batch\_size为N，则X尺寸为 ![N\times H_{in}\times W_{in}\times c](https://www.zhihu.com/equation?tex=N%5Ctimes+H_%7Bin%7D%5Ctimes+W_%7Bin%7D%5Ctimes+c) ，Y尺寸为 ![N\times H_{out}\times W_{out}\times n](https://www.zhihu.com/equation?tex=N%5Ctimes+H_%7Bout%7D%5Ctimes+W_%7Bout%7D%5Ctimes+n) 。我们将通道数从c，裁剪到 ![c^{'}<c](https://www.zhihu.com/equation?tex=c%5E%7B%27%7D%3Cc) ，则可以列一下优化函数

![argmin_{\beta ,W}\frac{1}{2N}\left \| Y'-\sum_{i=1}^{c}\beta _{i}X_{i}W_{i}^{T} \right \|_{F}^{2}\\subject \quad to\left \| \beta  \right \|_{0}\leq c^{'}](https://www.zhihu.com/equation?tex=argmin_%7B%5Cbeta+%2CW%7D%5Cfrac%7B1%7D%7B2N%7D%5Cleft+%5C%7C+Y%27-%5Csum_%7Bi%3D1%7D%5E%7Bc%7D%5Cbeta+_%7Bi%7DX_%7Bi%7DW_%7Bi%7D%5E%7BT%7D+%5Cright+%5C%7C_%7BF%7D%5E%7B2%7D%5C%5Csubject+%5Cquad+to%5Cleft+%5C%7C+%5Cbeta++%5Cright+%5C%7C_%7B0%7D%5Cleq+c%5E%7B%27%7D)

注意： ![ | A |_{F}^{2}=\sum _{ij}\left | A_{ij} \right |^{2},\left \| \beta  \right \|_{0}=\beta](https://www.zhihu.com/equation?tex=+%7C+A+%7C_%7BF%7D%5E%7B2%7D%3D%5Csum+_%7Bij%7D%5Cleft+%7C+A_%7Bij%7D+%5Cright+%7C%5E%7B2%7D%2C%5Cleft+%5C%7C+%5Cbeta++%5Cright+%5C%7C_%7B0%7D%3D%5Cbeta) 中非0项的个数。

上式为一个滑动窗下的优化函数，实际中我们需要将所有滑动窗对应的函数值累加起来，再求解 ![\beta ,W](https://www.zhihu.com/equation?tex=%5Cbeta+%2CW) ,式中 ![X_{i}](https://www.zhihu.com/equation?tex=X_%7Bi%7D) 为 ![N*k_{n}k_{w}](https://www.zhihu.com/equation?tex=N%2Ak_%7Bn%7Dk_%7Bw%7D) 的矩阵，实际是卷积核的滑动窗在输入特征图滑动到某个位置得到的矩阵； ![Y^{'}](https://www.zhihu.com/equation?tex=Y%5E%7B%27%7D) 为 ![N*n](https://www.zhihu.com/equation?tex=N%2An) 的矩阵； ![W_{i}](https://www.zhihu.com/equation?tex=W_%7Bi%7D) 为 ![n*k_{n}k_{w}](https://www.zhihu.com/equation?tex=n%2Ak_%7Bn%7Dk_%7Bw%7D) $$的矩阵，表示第i个channel的所有参数； ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta)  为一维向量，参数个数为𝑐，因此 ![\beta_{i}](https://www.zhihu.com/equation?tex=%5Cbeta_%7Bi%7D) 一个标量，表示是否选择第i个通道。如果 ![\beta_{i}=0](https://www.zhihu.com/equation?tex=%5Cbeta_%7Bi%7D%3D0) 则表示第i个通道被裁掉，那么相应的𝑋中的第i个通道可以被删掉。

但由于（1）式中的约束条件是0-范数，属于 ![l_{0}](https://www.zhihu.com/equation?tex=l_%7B0%7D) 优化问题，求解为NP难问题，因此进一步将0-范数放宽到1-范数，得优化函数为

![argmin_{\beta ,W}\frac{1}{2N}\left \| Y^{'}-\sum_{i=1}^{c}\beta _{i}X_{i}W_{i}^{T} \right \|_{F}^{2}+\lambda \left \| \beta  \right \|_{1}\\subject \quad to\left \| \beta  \right \|_{0}\leq c^{'},\quad \forall i\left \| W_{i} \right \|_{F}=1](https://www.zhihu.com/equation?tex=argmin_%7B%5Cbeta+%2CW%7D%5Cfrac%7B1%7D%7B2N%7D%5Cleft+%5C%7C+Y%5E%7B%27%7D-%5Csum_%7Bi%3D1%7D%5E%7Bc%7D%5Cbeta+_%7Bi%7DX_%7Bi%7DW_%7Bi%7D%5E%7BT%7D+%5Cright+%5C%7C_%7BF%7D%5E%7B2%7D%2B%5Clambda+%5Cleft+%5C%7C+%5Cbeta++%5Cright+%5C%7C_%7B1%7D%5C%5Csubject+%5Cquad+to%5Cleft+%5C%7C+%5Cbeta++%5Cright+%5C%7C_%7B0%7D%5Cleq+c%5E%7B%27%7D%2C%5Cquad+%5Cforall+i%5Cleft+%5C%7C+W_%7Bi%7D+%5Cright+%5C%7C_%7BF%7D%3D1)

上式中对 ![\beta ,W](https://www.zhihu.com/equation?tex=%5Cbeta+%2CW) 的求解，分别对应选择通道、重建特征图两步，下面我们分别来看这两步迭代。

**第一步：选择通道**

在第一步，我们固定参数W不变，解$$\beta$$，则上述优化问题可以进一步转化为LASSO回归问题

![\beta ^{LASSO}(\lambda )_{\beta ,W}=argmin\frac{1}{2N}\left \| Y^{'}-\sum_{i=1}^{c}\beta _{i}Z_{i} \right \|_{F}^{2}+\lambda \left \| \beta  \right \|_{1}\\subject \quad to\left \| \beta \right \|_{0}\leq c^{'}](https://www.zhihu.com/equation?tex=%5Cbeta+%5E%7BLASSO%7D%28%5Clambda+%29_%7B%5Cbeta+%2CW%7D%3Dargmin%5Cfrac%7B1%7D%7B2N%7D%5Cleft+%5C%7C+Y%5E%7B%27%7D-%5Csum_%7Bi%3D1%7D%5E%7Bc%7D%5Cbeta+_%7Bi%7DZ_%7Bi%7D+%5Cright+%5C%7C_%7BF%7D%5E%7B2%7D%2B%5Clambda+%5Cleft+%5C%7C+%5Cbeta++%5Cright+%5C%7C_%7B1%7D%5C%5Csubject+%5Cquad+to%5Cleft+%5C%7C+%5Cbeta+%5Cright+%5C%7C_%7B0%7D%5Cleq+c%5E%7B%27%7D)

式中$ ![Z_{i}=X_{i}W_{i}^{T}\in \mathbb{R}^{N*n}](https://www.zhihu.com/equation?tex=Z_%7Bi%7D%3DX_%7Bi%7DW_%7Bi%7D%5E%7BT%7D%5Cin+%5Cmathbb%7BR%7D%5E%7BN%2An%7D) $,LASSO回归问题，可以通过SGD方法找到最优解，是比较常见的优化问题。

**第二步：重建特征图**

在第二步，我们固定参数β不变，解W，则式（2）可以转化为最小二乘估计问题

![argmin_{W^{'}}\left \| Y^{'}-X^{'}(W^{'})^{T} \right \|_{F}^{2}](https://www.zhihu.com/equation?tex=argmin_%7BW%5E%7B%27%7D%7D%5Cleft+%5C%7C+Y%5E%7B%27%7D-X%5E%7B%27%7D%28W%5E%7B%27%7D%29%5E%7BT%7D+%5Cright+%5C%7C_%7BF%7D%5E%7B2%7D)

式中 ![X^{'}=[\beta _{1}X_{1},\beta_{2}X_{2},...,\beta _{c}X_{c}]\in \mathbb{R}^{N*ck_{n}k_{w}},W^{'}=[W_{1},W_{2},...,W_{c}]\in \mathbb{R}^{n*ck_{n}k_{w}}](https://www.zhihu.com/equation?tex=X%5E%7B%27%7D%3D%5B%5Cbeta+_%7B1%7DX_%7B1%7D%2C%5Cbeta_%7B2%7DX_%7B2%7D%2C...%2C%5Cbeta+_%7Bc%7DX_%7Bc%7D%5D%5Cin+%5Cmathbb%7BR%7D%5E%7BN%2Ack_%7Bn%7Dk_%7Bw%7D%7D%2CW%5E%7B%27%7D%3D%5BW_%7B1%7D%2CW_%7B2%7D%2C...%2CW_%7Bc%7D%5D%5Cin+%5Cmathbb%7BR%7D%5E%7Bn%2Ack_%7Bn%7Dk_%7Bw%7D%7D) ，最小二乘估计问题同样为常见的优化问题，也可以利用SGD的方法得到最优解，但是在原本的式（2）中，要求$ ![\forall i\left \| W_{i} \right \|_{F}=1](https://www.zhihu.com/equation?tex=%5Cforall+i%5Cleft+%5C%7C+W_%7Bi%7D+%5Cright+%5C%7C_%7BF%7D%3D1)因此在求的最优的参数W后，需要做以下调整

![\beta _{i}=\beta _{i}\left \| W_{i} \right \|_{F};W_{i}=W_{i}/\left \| W_{i} \right \|_{F}](https://www.zhihu.com/equation?tex=%5Cbeta+_%7Bi%7D%3D%5Cbeta+_%7Bi%7D%5Cleft+%5C%7C+W_%7Bi%7D+%5Cright+%5C%7C_%7BF%7D%3BW_%7Bi%7D%3DW_%7Bi%7D%2F%5Cleft+%5C%7C+W_%7Bi%7D+%5Cright+%5C%7C_%7BF%7D) ，由于实际优化函数中

![X^{'}(W^{'})^{T}=\sum_{i=1}^{c}\beta _{i}X_{i}W_{i}^{T}](https://www.zhihu.com/equation?tex=X%5E%7B%27%7D%28W%5E%7B%27%7D%29%5E%7BT%7D%3D%5Csum_%7Bi%3D1%7D%5E%7Bc%7D%5Cbeta+_%7Bi%7DX_%7Bi%7DW_%7Bi%7D%5E%7BT%7D)

做上述调整并不会影响最优值和最优解。

本文提出迭代交替地进行上述两步优化过程，第一次迭代中参数W初始化为待裁枝的训练好的模型参数， ![\lambda =0](https://www.zhihu.com/equation?tex=%5Clambda+%3D0) , ![\left \| \beta  \right \|_{0}=c](https://www.zhihu.com/equation?tex=%5Cleft+%5C%7C+%5Cbeta++%5Cright+%5C%7C_%7B0%7D%3Dc) 做第一步优化可以使 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 中0项增加。当 ![\left \| \beta  \right \|_{0}](https://www.zhihu.com/equation?tex=%5Cleft+%5C%7C+%5Cbeta++%5Cright+%5C%7C_%7B0%7D) 趋于稳定后，进行第二步优化，得到相应的W;再增加 ![\lambda](https://www.zhihu.com/equation?tex=%5Clambda) ,反复，直到 ![\left \| \beta  \right \|_{0}\leq c^{'}](https://www.zhihu.com/equation?tex=%5Cleft+%5C%7C+%5Cbeta++%5Cright+%5C%7C_%7B0%7D%5Cleq+c%5E%7B%27%7D) 成立，最后做一次第二步优化，得到的结果与迭代两个优化的结果相似.

**整体模型裁枝**

对于整体模型的裁枝，采用逐层裁枝的方法，但是为了减少累积误差，每次重建特征图误差中的模版特征图$$Y^{'}$$，均选择原模型（未裁枝的模型）中对应层的输出。

**多分枝结构的裁枝**

基于上述的裁枝方法，已经可以对一般的单枝网络做裁枝了（例如VGG，LenNet，AlexNet等）。但是当前很多模型结构例如（ResNet，GoogleNet等）都存在多分枝的情况。以ResNet为例，每一个Bottleneck结构包括正常的分枝（residual branch），和shortcut branch，如图2左侧所示，针对于多分枝结构，作者给出了裁枝方法

首先对于residual branch，由3个卷积构成，其中对于第二个卷积，是可以按之前单枝裁枝方法做的，问题出在第一个和第三个卷积上。

**第一个卷积裁枝**

如图2所示对于第一个卷积（c1），想要裁剪通道，势必会影响之前一层（c0）的卷积核，和c1卷积的输入特征图（相应被裁掉的通道，也会被删去），而该特征图还需要作为shortcut层的输入，这将引起较大的误差。作者提出feature map sampling的解决方法（如图2右侧所示），即在裁剪第一个卷积时，并不删掉其输入特征图的通道，而是新加一层采样层（其用处就是对输入特征图按β来进行采样，同时保留了原本的输入特征图作为shortcut的输入）

此外，作者也提出另一种解决方法：filter-wise pruing，即先对c0的卷积层做一次以filter为单位的裁枝（filter-level，在视频中有讲，filter-level裁枝同时带动下一层channel-level的裁枝），就直接作为第一层的裁枝。但是通过实验，作者发现fine-tune之前，filter-wise pruning的方法比feature map sampling的方法准确率高，但是经过fine-tune后，两种方法准确率相差不多。因此作者最终采用简单而有效的feature map sampling的方法

![](https://pic3.zhimg.com/v2-c68c75eab175efe8be5ea1e36c949a9e_b.png)图2 针对Bottleneck的裁枝方法


**第三个卷积层裁枝**

如图2右侧所示，最后一层的输出本身是![Y_{2}](https://www.zhihu.com/equation?tex=Y_%7B2%7D) 单加上shortcut层才是实际bottleneck的输出，我们的目的实际上是最小误差重建 ![Y_{1}+Y_{2}](https://www.zhihu.com/equation?tex=Y_%7B1%7D%2BY_%7B2%7D) 假设 ![Y_{1},Y_{2}](https://www.zhihu.com/equation?tex=Y_%7B1%7D%2CY_%7B2%7D) 为裁枝之前shortcut branch和residual branch的输出， ![Y_{1}^{'},Y_{2}^{'}](https://www.zhihu.com/equation?tex=Y_%7B1%7D%5E%7B%27%7D%2CY_%7B2%7D%5E%7B%27%7D) 为裁枝之后两个分枝的输出，由于shortcut branch没有卷积，无法重建 ![Y_{1}](https://www.zhihu.com/equation?tex=Y_%7B1%7D) ,因此，我们直接上第三层卷积的重建目标中的模版特征图为 ![Y_{1}-Y_{1}^{'}+Y_{2}](https://www.zhihu.com/equation?tex=Y_%7B1%7D-Y_%7B1%7D%5E%7B%27%7D%2BY_%7B2%7D) ，如此即可保证最小误差重建整个bottleneck的输出特征图 ![Y_{1}+Y_{2}](https://www.zhihu.com/equation?tex=Y_%7B1%7D%2BY_%7B2%7D)

## **四．实验**

本文中的实验也是十分丰富的，作者通过与其他方法的对比，以及本文方法在不同网络结构中的效果，证明了本文方法的有效性

**基于VGG16网络的实验**

**单层裁枝效果实验**

![](https://pic3.zhimg.com/v2-31384b40576a26eb470c20cfa2b2257e_b.png)图3 VGG16单层裁枝效果

蓝色的线first k是直接选取前k个channel；灰虚线max response是选取绝对值和最大的k个channel；红色线为本文提出的方法。横坐标为理论加速效果，纵坐标为错误率的增加，当然斜率越小越好。首先我们可以看到本文提出的方法（红色）效果是最好的，一定程度上证明了本文提出方法的有效性。其次，层数越深，越不容易做裁枝加速，这也是所有加速方法面临的一个瓶颈。

**整体网络裁枝效果**

实验基于ImageNet数据集，首先对比了本文的方法与其他方法的效果，在相同理论加速效果的条件下，比较Top-5错误率的增加量。注意图4中的加速效果都是理论加速效果（本文中理论加速效果由$$c^{'},c$$共同决定）。

![](https://pic1.zhimg.com/v2-735461002699f3f5c89996f910e41ea4_b.png)图4 本文方法与其他方法的比较

前两行分别为张量分解的方法，最后两行分别本文提出的方法在fine-tune前后的效果。

其次，作者比较了本文方法与其他方法结合的效果，注意此时的加速效果仍是理论加速效果（而且是多种方法结合在一起后的加速效果）
![](https://pic3.zhimg.com/v2-79a0d31f1b4f50c84f7473c97241f75a_b.png)图5 本文方法与其他方法结合的效果

图5实际上比较了本文工作和另外一个工作的效果，Asym. 3D使用了通道维（channel）分解和卷积核（spatial，即分解 ![k_{n}*k_{w}](https://www.zhihu.com/equation?tex=k_%7Bn%7D%2Ak_%7Bw%7D) ）分解参见【53】，Our 3C使用了Asym 3D，和基于通道的裁枝，实验发现经过fine-tune后，本文方法有很好的效果。

**实际效果的比较**

验基于ImageNet数据集，效果如图6所示，其中第一列中的（$$4\*$$）表示理论加速效果，最后一列表示实际加速效果

![](https://pic2.zhimg.com/v2-172be5dc51b1c7338ef53b7aa7f38781_b.png)图6 本文方法与其他方法的实际加速效果


**目标检测网络的效果实验**

实验基于PASCAL VOC2007，网络结构使用VGG16-Faster-RCNN，实验效果如图7所示，注意其中的加速效果为理论加速效果

![](https://pic2.zhimg.com/v2-10e5fec4ddf2d56f520cdfc4615a314d_b.png)图7 本文方法裁枝VGG16-Faster-RCNN的加速效果

图中mAP（Mean Average Precisian）表示检测效果，mmAP为COCO

数据集提出的指标。具体可参见工作【32】

**针对多分枝网络结构的实验**

**基于ResNet50网络结构的实验**

实验基于ImageNet数据集，使用ResNet50网络结构，在理论加速2倍的条件下，比较Top-5错误率的增加，实验效果如图8所示

![](https://pic3.zhimg.com/v2-2181f7060e2224acd83c24cd3709259e_b.png)图8 本文方法加速ResNet50网络

**基于Xception50网络结构的实验**

实验基于ImageNet数据集，使用Xception50网络结构，在理论加速2倍的条件下，比较Top-5错误率的增加，实验效果如图9所示

![](https://pic1.zhimg.com/v2-62be181b0c69fbca5e92a6486395b82c_b.png)图9 本文方法加速Xception50网络

参考文献

[1]  J. M. Alvarez and M. Salzmann. Learning the number of neurons in deep networks. In Advances in Neural Informa- tion Processing Systems, pages 2262–2270, 2016. 

[3] S. Anwar and W. Sung. Compact deep convolutional neural networks with coarse pruning. arXiv preprint arXiv:1610.09639, 2016.

[4] H. Bagherinezhad, M. Rastegari, and A. Farhadi. Lcnn: Lookup-based convolutional neural network. arXiv preprint arXiv:1611.06473, 2016.

[8] M. Courbariaux and Y. Bengio. Binarynet: Training deep neural networks with weights and activations constrained to+ 1 or-1. arXiv preprint arXiv:1602.02830, 2016.

[13] Y. Gong, L. Liu, M. Yang, and L. Bourdev. Compress- ing deep convolutional networks using vector quantization. arXiv preprint arXiv:1412.6115, 2014

[14] Y. Guo, A. Yao, and Y. Chen. Dynamic network surgery for efficient dnns. In Advances In Neural Information Process- ing Systems, pages 1379–1387, 2016.

[15] S. Han, X. Liu, H. Mao, J. Pu, A. Pedram, M. A. Horowitz, and W. J. Dally. Eie: efficient inference engine on com- pressed deep neural network. In Proceedings of the 43rd International Symposium on Computer Architecture, pages 243–254. IEEE Press, 2016.

[17] S. Han, J. Pool, J. Tran, and W. Dally. Learning both weights and connections for efficient neural network. In Advances in Neural Information Processing Systems, pages 1135–1143, 2015.

[19] H. Hu, R. Peng, Y.-W. Tai, and C.-K. Tang. Network trim- ming: A data-driven neuron pruning approach towards effi- cient deep architectures. arXiv preprint arXiv:1607.03250, 2016.

[22] M. Jaderberg, A. Vedaldi, and A. Zisserman. Speeding up convolutional neural networks with low rank expansions. arXiv preprint arXiv:1405.3866, 2014.

[24] Y.-D. Kim, E. Park, S. Yoo, T. Choi, L. Yang, and D. Shin. Compression of deep convolutional neural networks for fast and low power mobile applications. arXiv preprint arXiv:1511.06530, 2015.

[27] A. Lavin. Fast algorithms for convolutional neural networks. arXiv preprint arXiv:1509.09308, 2015.

[28] V. Lebedev, Y. Ganin, M. Rakhuba, I. Oseledets, and V. Lempitsky. Speeding-up convolutional neural net- works using fine-tuned cp-decomposition. arXiv preprint arXiv:1412.6553, 2014.

[29] V. Lebedev and V. Lempitsky. Fast convnets using group- wise brain damage. arXiv preprint arXiv:1506.02515, 2015.

[31] H. Li, A. Kadav, I. Durdanovic, H. Samet, and H. P. Graf. Pruning filters for efficient convnets. arXiv preprint arXiv:1608.08710, 2016.

[32] T.-Y. Lin, M. Maire, S. Belongie, J. Hays, P. Perona, D. Ra- manan, P. Dolla ́r, and C. L. Zitnick. Microsoft coco: Com- mon objects in context. In European conference on computer vision, pages 740–755. Springer, Cham, 2014.

[33] B. Liu, M. Wang, H. Foroosh, M. Tappen, and M. Pensky. Sparse convolutional neural networks. In Proceedings of the IEEE Conference on Computer Vision and Pattern Recogni- tion, pages 806–814, 2015.

[35] Z. Mariet and S. Sra. Diversity networks. arXiv preprint arXiv:1511.05077, 2015.

[36] M. Mathieu, M. Henaff, and Y. LeCun. Fast training of convolutional networks through ffts. arXiv preprint arXiv:1312.5851, 2013.

[41] M. Rastegari, V. Ordonez, J. Redmon, and A. Farhadi. Xnor- net: Imagenet classification using binary convolutional neu- ral networks. In European Conference on Computer Vision, pages 525–542. Springer, 2016.

[44] K. Simonyan and A. Zisserman. Very deep convolutional networks for large-scale image recognition. arXiv preprint arXiv:1409.1556, 2014.

[45] S. Srinivas and R. V. Babu. Data-free parameter pruning for deep neural networks. arXiv preprint arXiv:1507.06149, 2015.

[48] N. Vasilache, J. Johnson, M. Mathieu, S. Chintala, S. Pi- antino, and Y. LeCun. Fast convolutional nets with fbfft: A gpu performance evaluation. arXiv preprint arXiv:1412.7580, 2014.

[49] W. Wen, C. Wu, Y. Wang, Y. Chen, and H. Li. Learning structured sparsity in deep neural networks. In Advances In Neural Information Processing Systems, pages 2074–2082, 2016.

[52] T.-J. Yang, Y.-H. Chen, and V. Sze. Designing energy- efficient convolutional neural networks using energy-aware pruning. arXiv preprint arXiv:1611.05128, 2016.

[53] X. Zhang, J. Zou, K. He, and J. Sun. Accelerating very deep convolutional networks for classification and detection. IEEE transactions on pattern analysis and machine intelli- gence, 38(10):1943–1955, 2016.

[54] H.Zhou,J.M.Alvarez,andF.Porikli.Lessismore:Towards compact cnns. In European Conference on Computer Vision, pages 662–677. Springer International Publishing, 2016.

