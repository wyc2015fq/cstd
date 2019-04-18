# PyTorch faster_rcnn之一—复现代码 - wsp_1138886114的博客 - CSDN博客





2019年03月08日 13:47:21[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：161











### 文章目录
- [一、准备](#_1)
- [1.1 开始准备](#11__2)
- [1.2 准备训练数据 Pascal VOC2007 与模型权重](#12__Pascal_VOC2007__54)
- [1.3 准备模型权重：caffe-pretrained vgg16](#13_caffepretrained_vgg16_70)
- [1.4 训练前](#14__81)
- [二、开始训练](#_91)
- [三、推论或预测](#_97)
- [四、报错与解决](#_98)




### 一、准备

##### 1.1 开始准备

开发运行环境 **`win10 | Anaconda | python=3.6 | pytorch=1.0`**

关于系统运行环境的安装请点击，PyTorch虚拟环境请点击

项目下载地址：[https://github.com/chenyuntc/simple-faster-rcnn-pytorch](https://github.com/chenyuntc/simple-faster-rcnn-pytorch)

预训练模型地址：[https://pan.baidu.com/s/1o87RuXW](https://pan.baidu.com/s/1o87RuXW)  提取码：scxn
安装以下依赖包

```python
conda install pip pyyaml sympy h5py cython numpy scipy
conda install -c menpo opencv3  # 或者：pip install opencv-python
pip install easydict
pip install cupy-cuda80
pip install -r requirements.txt #其他的依赖
```

工程文档结构

```python
.
├── data                       # 训练测试数据准备与处理
|   ├── __init__.py
|   ├── dataset.py
|   ├── util.py
|   └── voc_dataset.py
├── misc
|   ├── convert_caffe_pretain.py
|   └── train_fast.py
├── model
|   ├── utils
|   |   ├── __init__.py
|   |   ├── bbox_tools.py
|   |   ├── creator_tool.py
|   |   ├── roi_cupy.py
|   |   └── nms
|   |       ├── __init__.py
|   |       ├── _nms_gpu_post.py
|   |       ├── build.py
|   |       └── non_maximum_suppression.py
|   ├── __init__.py
|   ├── faster_rcnn.py
|   ├── faster_rcnn_vgg16.py
|   ├── region_proposal_network.py
|   └── roi_module.py
├── utils
|   ├── __init__.py
|   ├── array_tool.py   # 类别转换脚本，实现tensor、numpy、Variable之间的转换。
|   ├── config.py       # 配置文件。包括数据及地址、visdom环境、预训练权重类型、学习率及各超参数。
|   ├── eval_tool.py    # 评估检测结果
|   └── vis_tool.py     # 显示训练中的图像
├── demo.ipynb
├── train.py
└── trainer.py
```

##### 1.2 准备训练数据 Pascal VOC2007 与模型权重

下载训练，验证，测试数据和VOCdevkit

```bash
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtrainval_06-Nov-2007.tar
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtest_06-Nov-2007.tar
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCdevkit_08-Jun-2007.tar

将所有这些tars解压缩到一个名为的目录中 VOCdevkit中数据结构如下：

$ VOCdevkit /                            ＃开发套件
$ VOCdevkit / VOCcode /                    ＃ VOC实用代码
$ VOCdevkit / VOC2007                     ＃图像集，注释等
＃ ...等几个目录...
```

修改voc_data_dircfg项目utils/config.py，或使用参数类似将其传递给程序–voc-data-`dir=/path/to/VOCdevkit/VOC2007/。`

##### 1.3 准备模型权重：caffe-pretrained vgg16

如果您想使用caffe-pretrain模型作为初始重量，您可以在下面运行以获得从caffe转换的vgg16权重，这与原始论文使用的相同。
`python misc / convert_caffe_pretrain.py`

此脚本将下载预训练模型并将其转换为与torchvision兼容的格式。

下载预训练模型地址：[https://pan.baidu.com/s/1DFxRkVBRjbDBDKKXdND47A](https://pan.baidu.com/s/1DFxRkVBRjbDBDKKXdND47A) 密码：u0xf。

然后，您可以通过设置指定vgg16_caffe.pth存储的caffe-pretraind模型的位置。默认路径没问题。`utils/config.pycaffe_pretrain_path`

如果您想使用来自torchvision的预训练模型，您可以跳过此步骤。
注意，caffe预训练模型表现出略微更好的表现。

注意：caffe模型需要BGR 0-255中的图像，而torchvision模型需要RGB和0-1中的图像。有关data/dataset.py详细信息

##### 1.4 训练前

构建cython代码`nms_gpu_post`:

```python
# 由于我是win10系统，编译时报错了（请查看报错与问题解决）
cd model/utils/nms/
python build.py build_ext --inplace
```

启动视觉可视化（确保之前的visdom能正常使用）
`python -m visdom.server`

### 二、开始训练

```
mkdir checkpoints / ＃创建文件夹（用于存放训练权重）
python train.py train --env = ' fasterrcnn-caffe '-- plot-every = 100 --caffe-pretrain
```

### 三、推论或预测

### 四、报错与解决

报错一：python [build.py](http://build.py) build_ext --inplace

```python
C:\Anaconda3\envs\AI_PyTorch_GPU\lib\site-packages\Cython\Compiler\Main.py:367: FutureWarning: Cython directive 'language_level' not set, using 2 for now (Py2). This will change in a later release! File: E:\pytorch_project\simple-faster-rcnn-pytorch\model\utils\nms\_nms_gpu_post.pyx
  tree = Parsing.p_module(s, pxd, full_module_name)
building '_nms_gpu_post' extension
creating build
creating build\temp.win-amd64-3.6
creating build\temp.win-amd64-3.6\Release
C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\BIN\x86_amd64\cl.exe /c /nologo /Ox /W3 /GL /DNDEBUG /MD -IC:\Anaconda3\envs\AI_PyTorch_GPU\include -IC:\Anaconda3\envs\AI_PyTorch_GPU\include "-IC:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\INCLUDE" "-IC:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\ATLMFC\INCLUDE" "-IC:\Program Files (x86)\Windows Kits\10\include\10.0.10240.0\ucrt" "-IC:\Program Files (x86)\Windows Kits\NETFXSDK\4.6\include\um" "-IC:\Program Files (x86)\Windows Kits\8.1\include\\shared" "-IC:\Program Files (x86)\Windows Kits\8.1\include\\um" "-IC:\Program Files (x86)\Windows Kits\8.1\include\\winrt" /Tc_nms_gpu_post.c /Fobuild\temp.win-amd64-3.6\Release\_nms_gpu_post.obj
_nms_gpu_post.c
_nms_gpu_post.c(613): fatal error C1083: 无法打开包括文件: “numpy/arrayobject.h”: No such file or directory
error: command 'C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\VC\\BIN\\x86_amd64\\cl.exe' failed with exit status 2
```

解决方法：修改`\model\utils\nms\build.py`后代码如下：

```python
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import numpy as np
 
setup(
    cmdclass={'build_ext': build_ext},
    ext_modules=[Extension('calculate', ['cythonfnS.pyx'])],
    include_dirs=[np.get_include()])
```





