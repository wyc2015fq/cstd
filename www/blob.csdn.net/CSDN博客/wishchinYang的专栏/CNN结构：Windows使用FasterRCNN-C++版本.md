# CNN结构：Windows使用FasterRCNN-C++版本 - wishchinYang的专栏 - CSDN博客
2017年11月28日 10:06:21[wishchin](https://me.csdn.net/wishchin)阅读数：1582
           参考文章：[Windows下VS2013 C++编译测试faster-rcnn。](http://www.cnblogs.com/LaplaceAkuir/p/6445189.html)
           本文与作者的所写方法有些许不同，欲速则不达，没有按照作者的推荐方法，绕了个弯弯。
          Windows版本纯C++版本的FasterRCNN比较难找,且懒得翻译Matlab版本代码，暂时可用的是这个项目：[Windows下VS2013 C++编译测试faster-rcnn。](http://www.cnblogs.com/LaplaceAkuir/p/6445189.html)
           作者上传的faster-rcnn c++代码，链接[https://github.com/zhanglaplace/Faster_rcnn_Cplusplus_vs2013](https://github.com/zhanglaplace/Faster_rcnn_Cplusplus_vs2013)，代码结合Matlab版本的faster-rcnn以及参考[http://blog.csdn.net/oYangZi12/article/details/53290426?locationNum=5&fps=1](http://blog.csdn.net/oYangZi12/article/details/53290426?locationNum=5&fps=1) 提供的代码。下载model，model文件可以从网盘下载，网盘链接：[http://pan.baidu.com/s/1dF88JvV](http://pan.baidu.com/s/1dF88JvV) ，设置新建项目为启动项目，且只编译该项目。                
1.重新编译Caffe库版本：
       作者貌似给出了FasterRCNN的代码，且只给了FasterRCNN的代码，相关的Caffe依赖库并没有在源代码里直接给出。因此，参考：[CNN：Windows下编译使用Caffe和Caffe2](http://blog.csdn.net/wishchin/article/details/77863889)，直接使用Vs13的Caffe版本。
       为什么VS15版本的不行，因为编译Caffe1-Vs15失败了，没能把roi_pooling_layer.cpp 编译进去。
     1.1 可能错误：[fast-r-cnn: caffe.LayerParameter“ has no field named ”roi_pooling_param](https://stackoverflow.com/questions/39099783/fast-r-cnn-caffe-layerparameter-has-no-field-named-roi-pooling-param)。使用RBG版本的FasterRCNN和Caffe版本的源代码[rbgirshick](https://github.com/rbgirshick)/**[caffe-fast-rcnn](https://github.com/rbgirshick/caffe-fast-rcnn)**，仍然不能编译成功，没有包含ROI-Pooling的Win版本的代码。
**  排错方法**：
       1.在Vs13版本的工程中，添加已经存在的   roi_pooling_layer.cu roi_pooling_layer.cpp  roi_pooling_layer.hpp到libCaffe工程，重新编译带RoiPooling的Caffe库。
         2. 在src/proto/caffe.proto文件中，若不存在 [roi_pooling_param](https://stackoverflow.com/questions/39099783/fast-r-cnn-caffe-layerparameter-has-no-field-named-roi-pooling-param) ，  则赋值 option  [roi_pooling_param](https://stackoverflow.com/questions/39099783/fast-r-cnn-caffe-layerparameter-has-no-field-named-roi-pooling-param)  = ID。取ID为一个与其他参数不重复的整数，我取其为152。 
          编译caffe完成后，在libcaffe后面加d，加入工程引用。
2. 测试
 初始化错误，无法注册layer。
      方法：去掉 //#include "register.h"  包含
      程序运行可以完成。
3. 测试模型
      在作者的网盘里面：网盘链接：[http://pan.baidu.com/s/1dF88JvV](http://pan.baidu.com/s/1dF88JvV) ，下载模型。把模型文件添加后缀  .caffemodel， 写入程序网络配置参数文件。
```
E:/DataSet/Caffe/FasterRCnn/models/proposal_test.prototxt
E:/DataSet/Caffe/FasterRCnn/models/proposal_final.caffemodel
E:/DataSet/Caffe/FasterRCnn/models/detection_test.prototxt
E:/DataSet/Caffe/FasterRCnn/models/detection_final.caffemodel
```
出现错误： "Unknown layer type: " << type  input
**修改方法**： 建立一个layer 文件夹，把MsCaffe相应的层的源文件添加到项目中，
![](https://img-blog.csdn.net/20171128095657026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4.重新编译，运行成功
![](https://img-blog.csdn.net/20171128100204980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
总结：
   结果：模型稍小，结果暂时不如YOLO2的检测结果。
  另外，赞原作者![害羞](http://static.blog.csdn.net/xheditor/xheditor_emot/default/shy.gif)


