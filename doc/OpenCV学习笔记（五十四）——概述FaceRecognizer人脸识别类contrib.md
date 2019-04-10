# OpenCV学习笔记（五十四）——概述FaceRecognizer人脸识别类contrib

2012年07月11日 15:11:29

yang_xian521

阅读数：65904

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

在最新版的2.4.2中，文档的更新也是一大亮点，refrence   manual扩充了200多页的内容，添加了contrib部分的文档。contrib就是指OpenCV中新添加的模块，但又不是很稳定，可以认为是一个雏形的部分。这次结合refman的阅读，介绍一下FaceRecognizer这个人脸识别类，这个类也是2.4.2更新日志里着重强调过的更新，配套的文档也是相当充实。这个类的基类也是Algorithm类，对于Algorithm类的简单介绍，请参看我之前的blog[OpenCV学习笔记（五十）——Algorithm类介绍（core）](http://blog.csdn.net/yang_xian521/article/details/7533922)，这个类内的接口函数也是异常简单，人脸识别的任务也就是两大部分，训练和预测，分别对应着train函数和predict函数，还有对应的数据加载保存函数save和load。不过它当然还可以调用其基类Algorithm的函数。特别说明的是，人脸识别中预测的参数也是可以调节的，但这里只给出了train和predict函数，为啥没有setparameter的函数呢，那是因为各中人脸识别方法的参数并不相同，要通过Algorithm的get和set函数实时的调整~~v5啊！

先来说说训练的过程，train函数的两个参数也很简单，训练的图像组vector<Mat>和对应的标签组vector<int>，这个label标签只需保证同一个人的标签相同即可，不需要保证图像的按标签顺序输入，方便极了。对于预测，有两种调用，其中的参数有测试图像、返回的标签值和测试样本和标签样本的相似性。返回的标签值为-1，说明测试样本在训练集中无对应或距离较远。这里用个FisherFace作为示例说明一下如何训练和预测：



```cpp
	vector<Mat> images;
	vector<int> labels;
	// images for first person
	images.push_back(imread("person0/0.jpg", CV_LOAD_IMAGE_GRAYSCALE));
	labels.push_back(0);
	images.push_back(imread("person0/1.jpg", CV_LOAD_IMAGE_GRAYSCALE));
	labels.push_back(0);
	// images for second person
	images.push_back(imread("person1/0.jpg", CV_LOAD_IMAGE_GRAYSCALE));
	labels.push_back(1);
	images.push_back(imread("person1/1.jpg", CV_LOAD_IMAGE_GRAYSCALE));
	labels.push_back(1);
 
	Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
	model->train(images, labels);
 
	Mat img = imread("person1/2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	int predicted = model->predict(img);
```

 当然我们也不需要每次使用都进行一次训练，

可以把训练好的模型通过save函数保存成一个文件，下次使用的时候只需load即可

。 



目前支持的3种人脸识别的方案：特征脸EigenFace、Fisher脸FisherFace、LBP直方图LBPHFace。分别调用函数createEigenFaceRecognizer、createFisherFaceRecognizer、createLBPHFaceRecognizer建立模型。

对于EigenFace两个输入参数，分别为PCA主成分的维数num_components和预测时的阈值threshold，主成分这里没有一个选取的准则，要根据输入数据的大小而决定，通常认为80维主成分是足够的。除了这两个输入参数外，还有eigenvalues和eigenvectors分别代表特征值和特征向量，mean参数为训练样本的平均值，projections为训练数据的预测值，labels为预测时的阈值。

对于FisherFace，和EigenFace非常相似，也有num_components和threshold两个参数和其他5个参数，FisherFace的降维是LDA得到的。默认值为c-1，如果设置的初始值不在（0，c-1]的范围内，会自动设定为c-1。

特别需要强调的是，EigenFace和FisherFace的训练图像和测试图像都必须是灰度图，而且是经过归一化裁剪过的。

对于LBPHFace，我想不用过多介绍，LBP简单和效果是大家都很喜欢的，参数包括半径radius，邻域大小即采样点个数neighbors，x和y方向的单元格数目grid_x,grid_y，还有两个参数histograms为训练数据得到的直方图，labels为直方图对应的标签。这个方法也要求训练和测试的图像是灰度图

接下来应该结合文档进一步研究一下这个人脸识别类。我之前大量的人脸实验都是在matlab下进行的，有了这个利器，我感觉会有越来越多的学生做老师和老板布置的project会选择用OpenCV，而不是Matlab。希望我们都爱的OpenCV越来越好，越来越强大。欢迎交流