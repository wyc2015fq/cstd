# gcc4.8+Caffe2(0.8)+onnx(1.2)+onnx-caffe2在anaconda中的安装,不需要root,源码编译. - qq_30006593的博客 - CSDN博客





2018年08月18日 15:02:26[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：380








caffe2的cuda8预编译是gcc5以上的. gcc5以下的只有cuda9和cpu版本

对于gcc4.8和cuda8的caffe2没有预编译. 

# 源码安装caffe2

```
git clone --recursive https://github.com/pytorch/pytorch.git
cd pytorch
git submodule update --init
CONDA_INSTALL_LOCALLY=1 ./scripts/build_anaconda.sh --cuda 8.0 --cudnn 7 -DUSE_CUDA=ON -DUSE_NCCL=ON
```

由于onnx-caffe2已经集成在caffe2中,所以不用再安装

# 源码安装onnx

```
git clone https://github.com/onnx/onnx.git --recurrence
pip install ./onnx
```





