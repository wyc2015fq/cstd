
# COCO  loss （人脸识别损失函数） - intflojx的博客 - CSDN博客


2018年09月06日 10:36:07[paulpanwang](https://me.csdn.net/intflojx)阅读数：464


2017年nips的一篇做分类和识别的工作，其中在**[人脸识别](https://blog.csdn.net/intflojx/article/details/81278330)**任务上也做了实验，Rethinking Feature Discrimination and Polymerization for Large-scale Recognition.Yu Liu, Hongyang Li, XiaogangWang。提出了一个新的损失函数：congenerous cosine，本质上就是一个cosine距离版本的triplet center loss。
**[motivation](https://blog.csdn.net/intflojx/article/details/82377939)****：**
在做分类、识别任务的时候常见的loss就是softmax，pairwise，triplet以及最近新提出的center loss，基本的共识是既学习类内的信息又学习类间的信息是对识别效果最好的监督方式，所以softmax只有类间的监督是比较挫的一种loss，pairwise、triplet都是同时学习类内和类间信息的，但是都受到采样的问题导致训练容易不稳定。center loss作者认为其问题在于他的center是个统计意义值，不是每次迭代update的（这个地方有问题，center loss也是每个iteration在mini-batch里面更新的）。所以作者提出了一个coco loss，其实主要的特性在于：一是使用了cosine距离；二是用了center；三是把distance本身作为输入构建softmax loss，使得distance具有softmax特性。具体的loss对比如下图
![](https://img-blog.csdn.net/20171113161239722?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcl9vbl9haXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**基本原理：**
因为主要是loss的定义，推演的过程围绕上面的三个方面讲，一是使用cosine距离的loss如下：
![](https://img-blog.csdn.net/20171113161226874?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcl9vbl9haXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个loss就是要让同类的cosine距离相比于不同类的更大，但是这个计算复杂度太高所以把和每个样本的计算改成和中心的计算并且写成softmax的形式，并做feature归一化：
![](https://img-blog.csdn.net/20171113161530410?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcl9vbl9haXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171113161646924?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcl9vbl9haXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最终加上交叉熵：
![](https://img-blog.csdn.net/20171113161748140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcl9vbl9haXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里面需要注意的是有个scale的参数alpha，之前在focal loss里提到过对于softmax来说，如果feature归一化，类别数固定的情况下其loss是有个上下限的，这里面的alpha的作用也是为了对不同的任务调整loss的范围使得能够得到更好的效果，对于alpha作者还推导了一下算了个公式：
![](https://img-blog.csdn.net/20171113163454257?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcl9vbl9haXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**实验结果**
在人脸人体上都做了 实验，这里就只列一下人脸的结果，使用了inception-resnet模型，用了ms1M的80k ID\3M人脸图片就把lfw搞到了99.86%应该是目前最好的结果了，不过数据上应该会有重复的可能而且官网上还没更新不知道是为啥，会不会也overfit了错误lable的样本？
![](https://img-blog.csdn.net/20171113164153547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcl9vbl9haXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
megaface上的结果：
![](https://img-blog.csdn.net/20171113164325458?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcl9vbl9haXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**总结**
是个很好的工作，结果也不错。用归一化的特征把**cosine距离和center loss结合**起来，除了只用中心点这个信息不够理想之外其他的都还好，应该跟triplet做些结合或者把中心点变成多个小的中心可能会更好，从信息的层面上还是丢掉了类之间的轮廓信息，也就是制衡量样本到类中心的距离没办法很好的表示这个类的样本空间分布。

