# MSCOCO数据集分析 - 专注于数据挖掘算法研究和应用 - CSDN博客
2019年01月04日 10:27:27[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：137
MSCOCO数据集是多模态学习领域重要的数据集，网址：[http://cocodataset.org/#home](http://cocodataset.org/#home)
```
COCO is a large image dataset designed for object detection, segmentation, person keypoints detection, stuff segmentation, and caption generation.
```
1、下载数据集 [http://cocodataset.org/#download](http://cocodataset.org/#download)
![](https://img-blog.csdnimg.cn/20190104102658502.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_16,color_FFFFFF,t_70)
2、依赖库：
sudo pip3 install cython
sudo pip3 install scikit-image 或sudo apt-get install python-skimage  看那个有效
3、下载开源库：[https://github.com/cocodataset/cocoapi](https://github.com/cocodataset/cocoapi)
1)git clone [https://github.com/cocodataset/cocoapi](https://github.com/cocodataset/cocoapi)
2)cd cocoapi/PythonAPI
3)编辑Makefile文件如下：
![](https://img-blog.csdnimg.cn/20190104103041764.png)
4)make
4、分析：pycocoDemo.ipynb文件
