# UFLDL教程的卷积神经网络(CNN)实现 - 硬曲奇小屋 - CSDN博客





2017年09月08日 17:37:44[HardCookies](https://me.csdn.net/james_616)阅读数：219








[UFLDL](http://ufldl.stanford.edu/tutorial/supervised/ExerciseConvolutionalNeuralNetwork/)中介绍了卷积神经网络，卷积神经网受启发于动物的视觉神经系统，是将研究大脑获得的深刻理解成功用于机器学习应用的关键例子。最早的一个成功应用是在20世纪90年代，LeCun等人开发的读取支票的卷积网络LeNet-5. 

卷积神经网是由几个相互交替的卷积层（convolution layer）和池化层（pooling layer），最后再是一个用作softmax分类的全联接层构成。其设计的两个关键特性就是部分链接和权值共享。 

UFLDL的练习与LeNet-5类似，就是对MNIST (Mixed National Institute of Standards and Technology database)手写数字集进行分类。总共包括0-9共10类数字，每个样本为28x28的归一到[0,1]的图像。基本的网络结构如下图所示： 
![CNN structure](https://img-blog.csdn.net/20170908173608484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



