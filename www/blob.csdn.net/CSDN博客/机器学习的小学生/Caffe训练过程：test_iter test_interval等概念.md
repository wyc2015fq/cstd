
# Caffe训练过程：test_iter test_interval等概念 - 机器学习的小学生 - CSDN博客


2017年03月24日 17:00:15[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：1314


转自：[http://blog.csdn.net/iamzhangzhuping/article/details/49993899](http://blog.csdn.net/iamzhangzhuping/article/details/49993899)
先上一张图，大家很熟悉的一张图。
![这里写图片描述](https://img-blog.csdn.net/20170324165928171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFieV9neWw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
首先说明一个概念：在caffe中的一次迭代iteration指的是一个batch,而不是一张图片。下面就主要说下2个概念
test_iter: 在测试的时候，需要迭代的次数，即test_iter* batchsize（测试集的）=测试集的大小，测试集batchsize可以在prototx文件里设置
test_interval:interval是区间的意思，所有该参数表示：训练的时候，每迭代500次就进行一次测试。
caffe在训练的过程是边训练边测试的。训练过程中每500次迭代（也就是32000个训练样本参与了计算，batchsize为64），计算一次测试误差。计算一次测试误差就需要包含所有的测试图片（这里为10000），这样可以认为在一个epoch里，训练集中的所有样本都遍历以一遍，但测试集的所有样本至少要遍历一次，至于具体要多少次，也许不是整数次，这就要看代码，大致了解下这个过程就可以了。

