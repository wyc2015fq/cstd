# OpenCV学习笔记（五十五）——用OpenCV做人脸识别和性别识别contrib

2012年07月15日 11:52:46

yang_xian521

阅读数：63578

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

### 人脸识别的故事说也说不完，调研的事还是交给大家吧。这里说的是用OpenCV做人脸识别。

因为是真正的人脸识别，不是搞笑娱乐的东西，所以数据库一定要强大的，推荐个网站<http://www.face-rec.org/databases/>。这里有最全的人脸库的概述，希望大家能找到自己需要的人脸库（PS：我现在特别需要一个3d的人脸库，不知道哪位大侠知道如何free获取，或者愿意共享给我那更是极好的了，先谢过咯）。这里简单介绍两个库，ORL和Yale，ORL是一个轻量级的库，Yale更为复杂，但并不是免费的。

数据的准备：在我们程序里需要读取图像和对应的标签，这里采用CSV文件，是一种简单的数据交互格式，在我们最常用的Excel里就支持这种格式。这种格式每条信息占一行。信息格式为：文件名；标签，例如C:/ORL/image.jpg;0。只要创建了一个CSV文件和对应的图像，你就可以对任何一个数据库进行训练了。当然，这个CSV文件并不一定要自己手动的创建，也可以Python脚本自己生成自己需要的CSV文件，对应的脚本为\modules\contrib\doc\facerec\src\create_csv.py，唉，谁让咱对这个脚本不熟悉，只能偷懒了，直接调\modules\contrib\doc\facerec\etc\at.txt或者\samples\cpp\facerec_at_t.txt。这个txt对应的是ORL的数据库，其中的路径就按照其改一下就好了。

至于人脸识别的具体实现，我已经在blog[OpenCV学习笔记（五十四）——概述FaceRecognizer人脸识别类contrib](http://blog.csdn.net/yang_xian521/article/details/7735224)提到了如何实现。\modules\contrib\doc\facerec\src很多很好的示例，我这里就不上传代码了。

最后再说一下人脸的标定。因为对Python也不是太熟悉，只给大家一个脚本文件吧，是\modules\contrib\doc\facerec\src\crop_face.py，通过这个脚本可以制作自己需要的人脸图片大小。

性别识别和视频中的识别在对应的demo里都有介绍，我这里就不详细说了，因为性别识别无非就是把训练样本的标签只有两类：男性和女性。需要说明的是，EigenFace是基于PCA的，是一种非监督的模型，不太适合性别识别的任务，这里的demo用的是FisherFace。对于视频的人脸识别，无非就是添加了个VideoCapture和一个人脸检测的CascadeClassifier。

关于训练的模型的保存和调用，就是用save和load函数，好简单的，就不介绍了吧。

最后说一下伪彩色图的这个函数applyColorMap，其中colorMap参数是用来选择伪彩色图的样式。因为人眼对颜色的敏感的程度要比对亮度的敏感程度要高，所以用伪彩色图 的对比效果要更好。这才医学图像处理中用的比较多，以前的B超现在都用彩超了，价格竟然要贵好多，其实就是一个伪彩色处理罢了，医院真黑啊！！