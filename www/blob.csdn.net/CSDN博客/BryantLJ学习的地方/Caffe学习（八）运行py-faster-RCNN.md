# Caffe学习（八）运行py-faster-RCNN - BryantLJ学习的地方 - CSDN博客





2016年08月23日 10:43:41[遍地流金](https://me.csdn.net/u012177034)阅读数：4061








**1.下载py-faster-RCNN源码**

```php
git clone --recursive https://github.com/rbgirshick/py-faster-rcnn
```

**2.编译lib库**

```bash
cd $FRCN_ROOT/lib
make
```

**3.编译caffe**

这部分的caffe编译与以前讲述的基本一致，不同的是makefile.config的修改部分，使支持用python定义layer

```
# In your Makefile.config, make sure to have this line uncommented
WITH_PYTHON_LAYER := 1
```

编译caffe与pycaffe

```
cd $FRCN_ROOT/caffe-fast-rcnn
# Now follow the Caffe installation instructions here:
#   http://caffe.berkeleyvision.org/installation.html

# If you're experienced with Caffe and have all of the requirements installed
# and your Makefile.config in place, then simply do:
make -j8 && make pycaffe
```

注意：由于本人所使用的为cuda8.0，py-faster-RCNN所使用的caffe版本比较旧，不支持，编译的时候会出现报错，因此需要对这里的caffe进行更新。

采用以下方式更新新版本caffe

```
cd caffe-fast-rcnn  
git remote add caffe https://github.com/BVLC/caffe.git  
git fetch caffe  
git merge caffe/master
```

更新完后删除include/caffe/layers/python_layer.hpp文件里的**self_.attr(“phase”) = static_cast(this->phase_); **这句话。 

更改完后在编译caffe即可

**4.下载在VOC-2007上训练好的模型**

该模型为在VOC-2007上训练好的VGG16模型

```bash
cd $FRCN_ROOT
./data/scripts/fetch_faster_rcnn_models.sh
```

**5.运行demo**

```bash
cd $FRCN_ROOT
./tools/demo.py
```



