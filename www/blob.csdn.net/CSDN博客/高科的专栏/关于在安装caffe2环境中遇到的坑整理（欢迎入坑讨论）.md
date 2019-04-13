
# 关于在安装caffe2环境中遇到的坑整理（欢迎入坑讨论） - 高科的专栏 - CSDN博客

2018年09月27日 13:13:16[高科](https://me.csdn.net/pbymw8iwm)阅读数：994


# 1.ImportError: cannot import name caffe2_pb2
测试caffe2的pytorch环境是否正常的时候使用
`root@lxsj-ThinkStation:~/pytorch# python
Python 2.7.12 (default, Dec  4 2017, 14:50:18) 
[GCC 5.4.0 20160609] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> from caffe2.python import workspace
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "caffe2/python/__init__.py", line 2, in <module>
    from caffe2.proto import caffe2_pb2
ImportError: cannot import name caffe2_pb2`**解决方案：**在pytorch路径下，或者root目录下找到.bashrc文件，打开。
1. 在末尾fi后面 写入
`export PYTHONPATH=/root/pytorch/build:/usr/local:$PYTHONPATH
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH`然后
`source .bashrc`重新进入之后不会报错了

# 2.ImportError: No module named _tkinter, please install the python-tk package
安装python-tk即可
apt-get install python-tk

# 3.WARNING:root:This caffe2 python run does not have GPU support. Will run in CPU only mode
`root@lxsj-ThinkStation:~/download#  python -c 'from caffe2.python import core'
WARNING:root:This caffe2 python run does not have GPU support. Will run in CPU only mode.`然后需要在环境变量里添加一行 vim ~/.bashrc
`export PATH=/usr/local/cuda/bin${PATH:+:${PATH}}`然后source ~/.bashrc
# 4.AssertionError: Torch not compiled with CUDA enabled
执行如下脚本
```python
import os
import numpy as np
 
import torch
import torch.nn
import torchvision.models as models
from torch.autograd import Variable 
import torch.cuda
import torchvision.transforms as transforms
 
from PIL import Image
 
img_to_tensor = transforms.ToTensor()
 
def make_model():
    resmodel=models.resnet34(pretrained=True)
    resmodel.cuda()#将模型从CPU发送到GPU,如果没有GPU则删除该行
    return resmodel
 
#分类
def inference(resmodel,imgpath):
    resmodel.eval()#必需，否则预测结果是错误的
    
    img=Image.open(imgpath)
    img=img.resize((224,224))
    tensor=img_to_tensor(img)
    
    tensor=tensor.resize_(1,3,224,224)
    tensor=tensor.cuda()#将数据发送到GPU，数据和模型在同一个设备上运行
            
    result=resmodel(Variable(tensor))
    result_npy=result.data.cpu().numpy()#将结果传到CPU，并转换为numpy格式
    max_index=np.argmax(result_npy[0])
    
    return max_index
    
#特征提取
def extract_feature(resmodel,imgpath):
    resmodel.fc=torch.nn.LeakyReLU(0.1)
    resmodel.eval()
    
    img=Image.open(imgpath)
    img=img.resize((224,224))
    tensor=img_to_tensor(img)
    
    tensor=tensor.resize_(1,3,224,224)
    tensor=tensor.cuda()
            
    result=resmodel(Variable(tensor))
    result_npy=result.data.cpu().numpy()
    
    return result_npy[0]
    
if __name__=="__main__":
    model=make_model()
    imgpath='xx.jpg'
    print inference(model,imgpath)
    print extract_feature(model, imgpath)
```
结果提示
`root@lxsj-ThinkStation:~/example# python index.py 
Traceback (most recent call last):
  File "index.py", line 57, in <module>
    model=make_model()
  File "index.py", line 19, in make_model
    resmodel.cuda()#将模型从CPU发送到GPU,如果没有GPU则删除该行
  File "/usr/local/lib/python2.7/dist-packages/torch/nn/modules/module.py", line 258, in cuda
    return self._apply(lambda t: t.cuda(device))
  File "/usr/local/lib/python2.7/dist-packages/torch/nn/modules/module.py", line 185, in _apply
    module._apply(fn)
  File "/usr/local/lib/python2.7/dist-packages/torch/nn/modules/module.py", line 191, in _apply
    param.data = fn(param.data)
  File "/usr/local/lib/python2.7/dist-packages/torch/nn/modules/module.py", line 258, in <lambda>
    return self._apply(lambda t: t.cuda(device))
  File "/usr/local/lib/python2.7/dist-packages/torch/cuda/__init__.py", line 161, in _lazy_init
    _check_driver()
  File "/usr/local/lib/python2.7/dist-packages/torch/cuda/__init__.py", line 75, in _check_driver
    raise AssertionError("Torch not compiled with CUDA enabled")
AssertionError: Torch not compiled with CUDA enabled`那么我们就来检测cuda是否生效
root@lxsj-ThinkStation:~/example\# cat test.py
import torch
print(torch.cuda.is_available())
返回的是False
所以估计是当时编译的pytorch的方式有问题，所以重新编译
`cd pytorch
git submodule update --init --recursive
python setup.py install`然后运行测试cuda返回True，运行刚才的脚本不再抛出异常
5.系统找不到caffe2的依赖库和头文件
应该是当时只编译了，要进入到build目录下进行make install 然后重新编译即可

