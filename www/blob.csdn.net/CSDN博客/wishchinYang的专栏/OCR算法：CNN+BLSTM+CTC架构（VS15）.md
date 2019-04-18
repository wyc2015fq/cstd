# OCR算法：CNN+BLSTM+CTC架构（VS15） - wishchinYang的专栏 - CSDN博客
2018年08月30日 17:03:40[wishchin](https://me.csdn.net/wishchin)阅读数：855
原文链接：[OCR算法](https://blog.csdn.net/forest_world/article/details/78566737)-CNN+BLSTM+CTC架构
由于作者使用了Boost1.57-Vc14，而1.57的VC14版本作者没有给出下载链接，因此需要自行编译，建议换掉作者的第三方库，使用其他的库，比如：这篇文章：[VS编译Caffe非常简单](https://blog.csdn.net/xingchenbingbuyu/article/details/72765612)。网盘：[3rdlibVC14](https://pan.baidu.com/s/1mhZgDeS?errno=0&errmsg=Auth%20Login%20Sucess&&bduss=&ssnerror=0)。
有少量的改动，如有疑问，请移步原文，直接到作者GitHub界面...链接：[https://github.com/senlinuc/caffe_ocr](https://github.com/senlinuc/caffe_ocr)
另外一篇使用TensorFlow-LSTM的方法：[TensorFlow-LSTM+CTC方法](https://blog.csdn.net/c2a2o2/article/details/75844775)；代码在[Github](http://github.com/ilovin/lstm_ctc_ocr)上。
实验结果：
       实测结果inception的准确率要比resnet高一些，ResNet把热情识别为然情，把温柔识别成温采.
![](https://img-blog.csdn.net/20180830170236960?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
主流ocr算法研究实验性的项目，目前实现了CNN+BLSTM+CTC架构
[ocr](https://github.com/topics/ocr)[lstm](https://github.com/topics/lstm)[ctc](https://github.com/topics/ctc)[res-lstm](https://github.com/topics/res-lstm)[densenet](https://github.com/topics/densenet)
- [41 commits](https://github.com/senlinuc/caffe_ocr/commits/master)
- [1 branch](https://github.com/senlinuc/caffe_ocr/branches)
- [0 releases](https://github.com/senlinuc/caffe_ocr/releases)
- [1 contributor](https://github.com/senlinuc/caffe_ocr/graphs/contributors)
- [C++ 87.2%](https://github.com/senlinuc/caffe_ocr/search?l=c%2B%2B)
- [Cuda 12.6%](https://github.com/senlinuc/caffe_ocr/search?l=cuda)
-  Other 0.2%
C++Cuda Other
Clone or download [Find file](https://github.com/senlinuc/caffe_ocr/find/master)
Latest commit [93775fd](https://github.com/senlinuc/caffe_ocr/commit/93775fd94760a6bc63af5e1829c4e54b5125ae09) 4 days ago
![@senlinuc](https://avatars3.githubusercontent.com/u/32592255?s=40&v=4)[senlinuc](https://github.com/senlinuc)[Update README.md](https://github.com/senlinuc/caffe_ocr/commit/93775fd94760a6bc63af5e1829c4e54b5125ae09)
||[3rdparty](https://github.com/senlinuc/caffe_ocr/tree/master/3rdparty)|[1. add openblas; 2.add vs2015 project](https://github.com/senlinuc/caffe_ocr/commit/5a72392febd35eac5f638e8212f455c18c682c05)|a month ago|
|----|----|----|----|
||[caffe-vsproj](https://github.com/senlinuc/caffe_ocr/tree/master/caffe-vsproj)|[remove ctc_loss_layer](https://github.com/senlinuc/caffe_ocr/commit/b84e4b043b8a469e30f08855c80278dc272b7162)|a month ago|
||[examples](https://github.com/senlinuc/caffe_ocr/tree/master/examples)|[add full-res-blstm network](https://github.com/senlinuc/caffe_ocr/commit/a07f162ff7e2693c6fcc516f6f3a9038419e1529)|a month ago|
||[include](https://github.com/senlinuc/caffe_ocr/tree/master/include)|[fix reshape bug (batch size change)](https://github.com/senlinuc/caffe_ocr/commit/b707abf214613b34e4b0bef7a6ae88c556d4bbf7)|5 days ago|
||[src](https://github.com/senlinuc/caffe_ocr/tree/master/src)|[fix reshape bug (batch size change)](https://github.com/senlinuc/caffe_ocr/commit/191fb9fc1106788ff85706b8d8b46ddeef32c129)|6 days ago|
||[tools](https://github.com/senlinuc/caffe_ocr/tree/master/tools)|[commit main code](https://github.com/senlinuc/caffe_ocr/commit/2ba537074924436e67f624c777567d7cba26cb92)|a month ago|
||[README.md](https://github.com/senlinuc/caffe_ocr/blob/master/README.md)|[Update README.md](https://github.com/senlinuc/caffe_ocr/commit/93775fd94760a6bc63af5e1829c4e54b5125ae09)|4 days ago|
  README.md
###  简介
caffe_ocr是一个对现有主流ocr算法研究实验性的项目，目前实现了CNN+BLSTM+CTC的识别架构，并在数据准备、网络设计、调参等方面进行了诸多的实验。代码包含了对lstm、warp-ctc、multi-label等的适配和修改，还有基于inception、restnet、densenet的网络结构。代码是针对windows平台的，linux平台下只需要合并相关的修改到caffe代码中即可。
###  caffe代码修改
 1. data layer增加了对multi-label的支持
 2. lstm使用的是junhyukoh实现的lstm版本（lstm_layer_Junhyuk.cpp/cu），原版不支持变长输入的识别。输入的shape由(TxN)xH改为TxNxH以适应ctc的输入结构。
 3. WarpCTCLossLayer去掉了对sequence indicators依赖（训练时CNN输出的结构是固定的），简化了网络结构（不需要sequence indicator layer）。
 4. densenet修改了对Reshape没有正确响应的bug，实现了对变长输入预测的支持。
 5. 增加transpose_layer、reverse_layer，实现对CNN feature map与lstm输入shape的适配
###  编译
  1. 安装opencv,boost,cuda,其它依赖库在3rdparty下（包含debug版的lib:[http://pan.baidu.com/s/1nvIFojJ）](http://pan.baidu.com/s/1nvIFojJ%EF%BC%89)
  2. caffe-vsproj下为vs2015的工程，配置好依赖库的路径即可编译，编译后会在tools_bin目录下生成训练程序caffe.exe
  3. 相关的依赖dll可从百度网盘下载（[http://pan.baidu.com/s/1boOiscJ）](http://pan.baidu.com/s/1boOiscJ%EF%BC%89)
###  实验
- 数据准备
	 （1）[VGG Synthetic Word Dataset](http://www.robots.ox.ac.uk/~vgg/data/text/)
	 （2）合成的中文数据（[http://pan.baidu.com/s/1c2fHpvE](http://pan.baidu.com/s/1c2fHpvE) ）(包含中文语料库)
![](https://github.com/senlinuc/caffe_ocr/raw/master/examples/ocr/20436312_1683447152.jpg)![](https://github.com/senlinuc/caffe_ocr/raw/master/examples/ocr/20436328_800384098.jpg)![](https://github.com/senlinuc/caffe_ocr/raw/master/examples/ocr/20436765_2556130357.jpg)![](https://github.com/senlinuc/caffe_ocr/raw/master/examples/ocr/20437703_937698549.jpg)![](https://github.com/senlinuc/caffe_ocr/raw/master/examples/ocr/20438234_4066860539.jpg)![](https://github.com/senlinuc/caffe_ocr/raw/master/examples/ocr/20440484_2127180630.jpg)
> 
> -   数据为利用中文语料库（新闻+文言文），通过字体、大小、灰度、模糊、透视、拉伸等变化随机生成。
-   字典中包含汉字、标点、英文、数字共5990个字符（语料字频统计，全角半角合并）
-   每个样本固定10个字符，字符随机截取自语料库中的句子
-   图片分辨率统一为280x32
-   共生成约360万张图片，按9:1分成训练集、验证集（暂时没有分测试集）
- 网络设计：网络结构在examples/ocr目录下
- 主要实验结果
- 英文数据集 VGG Synthetic Word Dataset:
|网格结构|predict-CPU|predict-GPU|准确率-no lexicon|准确率-lexicon-minctcloss|模型大小|
|----|----|----|----|----|----|
|crnn|67.13ms|10.28ms|0.8435|0.9163|32MB|
|inception-bn-res-blstm|41.62ms|8.68ms|0.7353|0.8609|15MB|
|densenet-res-blstm|N/A|6.07ms|0.7548|0.893|11MB|
- 中文数据集:
|网格结构|predict-CPU|predict-GPU|准确率|模型大小|
|----|----|----|----|----|
|inception-bn-res-blstm|65.4ms|11.2ms|0.92|26.9MB|
|resnet-res-blstm|64ms|10.75ms|0.91|23.2MB|
|densenet-res-blstm|N/A|7.73ms|0.965|22.9MB|
|densenet-no-blstm|N/A|2.4ms|0.97|5.6MB|
|densenet-sum-blstm-full-res-blstm|N/A|7.83ms|0.9805|15.5MB|
|densenet-no-blstm-vertical-feature|N/A|3.05ms|0.9816|9.6MB|
中文数据集上训练好的模型：[http://pan.baidu.com/s/1i5d5zdN](http://pan.baidu.com/s/1i5d5zdN)
> 
说明：
> -   CPU是Xeon E3 1230, GPU是1080TI
-   densenet使用的是memory-efficient版本，其CPU代码并没有使用blas库，只是实现了原始的卷积操作，速度非常慢，待优化后再做对比。
-   “res-blstm”表示残差形式的blstm，“no-blstm”表示没有lstm层，CNN直接对接CTC
-   准确率是指整串正确的比例,在验证集上统计,"准确率-no lexicon"表示没用词典的准确率，"准确率-lexicon-minctcloss"指先在词典中查找Edit Distance <=2的单词，再选择ctcloss最小的单词作为识别结果
-   predict-CPU/GPU为单张图片的预测时间，predict-CPU的后端是openblas，MKL比openblas快约一倍。中文数据集上图片分辨率为280x32，英文数据集100x32
-   densenet-sum-blstm-full-res-blstm相对于densenet-res-blstm有两点改动：（1）两个lstm结合成blstm的方式由concat改为sum；（2）两层blstm都采用残差方式连接（CNN最后的Channel数改为与blstm结点数相同），最后得到了最高的准确率。
-   densenet-no-blstm-vertical-feature相对于densenet-no-blstm去掉了1x4的pooling层，使得CNN最后一层的feature maps的高度为4，更好的保留笔画在垂直方向的变化信息，再reshape BxCxHxW --> Bx(CxH)x1xW串成高度为1的feature maps, 最后准确率上略好于之前的最好结果，可见CNN feature方面还有很多可挖掘的地方。
- 一些tricks
	 （1） 残差形式的blstm可显著提升准确率，中文数据集上0.94-->0.965，两层BLSTM都用残差后又提升到了0.9805
	 （2） 网络的CNN部分相对于BLSTM部分可以设置更高的学习率，这样可以显著增加收敛速度
- 疑问
	 （1）去掉blstm，直接用CNN+CTC的结构，在中文数据集上也可以取得很高的准确率（densenet-no-blstm），为什么？
	     可能的原因：a）CNN最后一层得到的特征对于字符级别的建模已经具有很好表征，b)lstm收敛较慢，需要更长的时间才能达到相同的精度。
- 现存的问题
	 （1）宽度较小的数字、英文字符会出现丢字的情况，如“11”、“ll”，应该是因为CNN特征感受野过大没有看到文字间隙的缘故。
###  提高准确率TODO
 1. 数据方面: 增大数据量，语料库均匀采样([https://yq.aliyun.com/articles/109555?t=t1](https://yq.aliyun.com/articles/109555?t=t1))
 2. 网络方面：增大网络（train、test loss很接近，现有网络没有过拟合），Attention,STN,辅助loss
###  引用
- crnn([https://arxiv.org/abs/1507.05717](https://arxiv.org/abs/1507.05717))
- multi-label的支持([http://blog.csdn.net/hubin232/article/details/50960201](http://blog.csdn.net/hubin232/article/details/50960201))
- junhyukoh实现的lstm版本（[https://github.com/junhyukoh/caffe-lstm）](https://github.com/junhyukoh/caffe-lstm%EF%BC%89)
- caffe-warp-ctc([https://github.com/BVLC/caffe/pull/4681](https://github.com/BVLC/caffe/pull/4681))
- memory-efficient densenet([https://github.com/Tongcheng/caffe/](https://github.com/Tongcheng/caffe/))
