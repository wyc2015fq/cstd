# 新版caffe 运行指令 - 家家的专栏 - CSDN博客





2015年01月05日 20:48:50[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：6854








**Train：**

/home/caffe/build/tools/caffe train \

    --solver=/home//mnist_solver.prototxt  2>&1 | tee ./mnist_model/cy.txt




**Test：**

/home/caffebuild/tools/caffe test \

   --model=/home//cifar10_traintest.prototxt   --weights=/home/model/cifar10_model/_iter_130500.caffemodel --iterations 100 -gpu 1 2>&1 | tee cy.txt




