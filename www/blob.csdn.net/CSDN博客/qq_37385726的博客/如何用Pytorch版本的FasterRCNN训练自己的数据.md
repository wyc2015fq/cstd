# 如何用Pytorch版本的FasterRCNN训练自己的数据 - qq_37385726的博客 - CSDN博客





2018年12月20日 08:50:47[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：176








1、从这里下载FasterRCNN的Pytorch版本代码。按照提示完成相关编译。

2、因为默认的数据为VOC2007，最直接的方法我们可以按照这里将自己的数据制作成VOC2007的格式，并建立软连接。

3、修改https://github.com/jwyang/faster-rcnn.pytorch/blob/master/lib/datasets/pascal_voc.py数据的类别，即将self._classes修改为自己的类。注：保留background类，用于训练RPN网络，视作RPN网络的负样本。

4、运行trainval_net.py即可。

5、测试时，需要将test_net.py的类别做相应修改。

注：① 在每次训练前，需要将data下面的cache删掉。

       ② 如果跑demo.py会发现检测不完整，这是因为demo里最多显示10个检测框，修改响应代码即可。




