# caffe2：conda路径和权限问题 - wishchinYang的专栏 - CSDN博客
2018年03月11日 14:17:23[wishchin](https://me.csdn.net/wishchin)阅读数：1233
         在使用conda之后，总是不能直接使用 conda install 命令，需要把codna添加到系统路径，取代默认Python。
在～/.bashrc中，添加
# added by Anaconda2 installer
export PATH="/home/wishchin/anaconda2/bin:$PATH"
source ～/.bashrc
出现问题：CondaIOError: Missing write permissions in: /home/wishchin/anaconda2 
添加sudo 权限：
sudo 命令对于conda是没有用的，要对conda安装目录添加sudo权限
`   sudo chown -R wishchin /home/wishchin/anaconda2`
或者
sudo chmod 777 -R /home//wishchin/anaconda2
然后，
conda install \
      future \
      numpy \
      protobuf
conda install --channel https://conda.anaconda.org/menpo opencv3
conda install --channel https://conda.anaconda.org/menpo opencv
**完成安装两个版本opencv**
**添加到环境：**
export PATH="/home/wishchin/CNN/caffe2/build:$PATH"
测试：
python -c 'from caffe2.python import core' 2>/dev/null && echo "Success" || echo "Failure"
Success
测试2：
运行测试：
python -m caffe2.python.operator_test.relu_op_test
显示：
WARNING:root:This caffe2 python run does not have GPU support. Will run in CPU only mode.
WARNING:root:Debug message: No module named caffe2_pybind11_state_gpu
Traceback (most recent call last):
  File "/home/wishchin/anaconda2/lib/python2.7/runpy.py", line 174, in _run_module_as_main
    "__main__", fname, loader, pkg_name)
  File "/home/wishchin/anaconda2/lib/python2.7/runpy.py", line 72, in _run_code
    exec code in run_globals
  File "/home/wishchin/CNN/caffe2/build/caffe2/python/operator_test/relu_op_test.py", line 22, in <module>
    from hypothesis import given
ImportError: No module named hypothesis
解决：
conda install \
      flask \
      graphviz \
      hypothesis \
      jupyter \
      matplotlib \
      pydot python-nvd3 \
      pyyaml \
      requests \
      scikit-image \
      scipy \
      setuptools \
      tornado
或者只安装：
conda install hypothesis
测试成功，但CUDNN未配置成功：
wishchin@wishchin-Lenovo-Yoga710-14ISK:~/CNN/caffe2/build$ python -m caffe2.python.operator_test.relu_op_test
WARNING:root:This caffe2 python run does not have GPU support. Will run in CPU only mode.
WARNING:root:Debug message: No module named caffe2_pybind11_state_gpu
INFO:caffe2.python.net_drawer:Cannot import pydot, which is required for drawing a network. This can usually be installed in python with "pip install pydot". Also, pydot requires graphviz to convert dot files to pdf: in ubuntu, this can usually be installed with "sudo apt-get install graphviz".
net_drawer will not run correctly. Please install the correct dependencies.
E0304 15:49:47.591872  9381 init_intrinsics_check.cc:59] CPU feature avx is present on your machine, but the Caffe2 binary is not compiled with it. It means you may not get the full speed of your CPU.
E0304 15:49:47.591897  9381 init_intrinsics_check.cc:59] CPU feature avx2 is present on your machine, but the Caffe2 binary is not compiled with it. It means you may not get the full speed of your CPU.
E0304 15:49:47.591920  9381 init_intrinsics_check.cc:59] CPU feature fma is present on your machine, but the Caffe2 binary is not compiled with it. It means you may not get the full speed of your CPU.
Trying example: test_relu(self=<__main__.TestRelu testMethod=test_relu>, X=array([0.], dtype=float32), gc=, dc=[], engine=u'')
Trying example: test_relu(self=<__main__.TestRelu testMethod=test_relu>, X=array([[-0.20262544],
       [-0.20262544],
       [ 0.22070907],
       [-0.20262544],
       [ 0.5926171 ]], dtype=float32), gc=, dc=[], engine=u'CUDNN')
I0304 15:49:47.650118  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.650454  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.650614  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.650760  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.650899  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.651027  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.651161  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.651305  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.651427  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.651526  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.651644  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.651764  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.651855  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.651952  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.652055  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.652148  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.652263  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.652372  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.652462  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.652550  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.652667  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.652750  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.652838  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
Trying example: test_relu(self=<__main__.TestRelu testMethod=test_relu>, X=array([[ 0.74607545, -0.07783447,  0.9671653 ,  0.15993083]],
      dtype=float32), gc=, dc=[], engine=u'')
Trying example: test_relu(self=<__main__.TestRelu testMethod=test_relu>, X=array([-0.9149518,  0.9264948, -0.9149518, -0.8492674], dtype=float32), gc=, dc=[], engine=u'CUDNN')
I0304 15:49:47.674011  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.674340  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.674460  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.674582  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.674681  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.674784  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.674878  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.675000  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.675127  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.675238  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.675344  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.675437  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.675526  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.675609  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.675698  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.675793  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.675884  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.675992  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.676095  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
Trying example: test_relu(self=<__main__.TestRelu testMethod=test_relu>, X=array([[ 0.90525484, -0.32742193],
       [ 0.68177605, -0.32742193]], dtype=float32), gc=, dc=[], engine=u'')
Trying example: test_relu(self=<__main__.TestRelu testMethod=test_relu>, X=array([[[[-0.03494453, -0.03494453, -0.03494453, -0.03494453,
          -0.03494453],
         [-0.03494453, -0.03494453, -0.77903503, -0.03494453,
          -0.03494453],
         [-0.5263449 , -0.03494453, -0.03494453, -0.03494453,
           0.43200284]]],
       [[[-0.03494453, -0.03494453, -0.03494453, -0.03494453,
          -0.03494453],
         [-0.03494453, -0.03494453, -0.03494453, -0.03494453,
          -0.03494453],
         [-0.03494453, -0.03494453, -0.03494453, -0.03494453,
          -0.03494453]]],
       [[[-0.03494453, -0.03494453, -0.03494453,  0.04599251,
          -0.03494453],
         [-0.03494453, -0.03494453, -0.03494453, -0.03494453,
          -0.03494453],
         [-0.03494453, -0.07541212, -0.03494453, -0.5037976 ,
          -0.03494453]]]], dtype=float32), gc=, dc=[], engine=u'CUDNN')
I0304 15:49:47.697964  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.698221  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.698321  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.698417  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.698498  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.698570  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.698647  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.698724  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.698798  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.698866  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.698940  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.699012  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.699100  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.699167  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.699239  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.699311  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.699381  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.699447  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.699518  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.699616  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.699705  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.699774  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.699848  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.699920  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.699991  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.700057  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.700129  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.700201  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.700278  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.700345  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.700417  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.700487  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.700559  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.700624  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.700695  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.700773  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.700845  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.700909  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.700980  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.701051  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.701122  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.701187  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.701258  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.701328  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.701400  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.701464  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.701535  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.701604  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.701674  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.701740  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.701809  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.701879  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.701949  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.702014  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.702085  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.702155  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.702225  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.702289  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.702360  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.702430  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.702502  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.702565  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.702654  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.702761  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.702862  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.702965  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.703079  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.703193  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.703299  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.703388  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.703495  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.703604  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.703718  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.703829  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.703933  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.704035  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.704141  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.704243  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.704357  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.704460  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.704560  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.704645  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.704735  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.704826  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.704917  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.705000  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.705090  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.705179  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.705269  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.705349  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.705438  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.705528  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.705626  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.705708  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.705807  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.705902  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.705993  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.706079  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.706171  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.706264  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.706365  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.706476  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.706585  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.706691  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.706799  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.706902  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.707012  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.707134  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.707240  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.707337  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.707425  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.707525  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.707625  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.707720  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.707823  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.707932  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.708045  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.708148  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.708281  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.708547  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.708674  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.708763  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.708853  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.708938  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.709025  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.709106  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.709192  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.709273  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.709363  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.709442  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.709527  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.709609  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.709692  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.709769  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.709854  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.709936  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.710021  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.710098  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.710180  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.710263  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.710346  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.710423  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.710507  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.710589  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.710674  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.710750  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.710834  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.710916  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.711000  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.711087  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.711174  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.711256  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.711340  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.711426  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.711527  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.711617  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.711702  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.711779  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.711866  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.711947  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.712030  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.712105  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.712188  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.712270  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.712352  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.712427  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.712510  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.712591  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.712674  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.712749  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.712831  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.712911  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.712993  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.713068  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.713150  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.713232  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.713315  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.713389  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.713472  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.713553  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.713635  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.713711  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.713791  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
Trying example: test_relu(self=<__main__.TestRelu testMethod=test_relu>, X=array([0.01914698, 0.01914698, 0.01914698, 0.01914698], dtype=float32), gc=, dc=[], engine=u'')
Trying example: test_relu(self=<__main__.TestRelu testMethod=test_relu>, X=array([-0.55140615, -0.55140615, -0.55140615, -0.55140615], dtype=float32), gc=, dc=[], engine=u'CUDNN')
I0304 15:49:47.726146  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.726380  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.726486  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.726588  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.726680  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.726758  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.726842  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.726927  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.727010  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.727098  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.727182  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.727265  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.727346  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.727421  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.727502  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.727584  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.727665  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.727740  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.727820  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
Trying example: test_relu(self=<__main__.TestRelu testMethod=test_relu>, X=array([[[ 0.78411776,  0.78411776,  0.78411776,  0.78411776,
          0.78411776]],
       [[ 0.78411776,  0.78411776,  0.78411776, -0.35040766,
          0.78411776]]], dtype=float32), gc=, dc=[], engine=u'')
Trying example: test_relu(self=<__main__.TestRelu testMethod=test_relu>, X=array([0.35586166, 0.35586166, 0.01611305, 0.14803013], dtype=float32), gc=, dc=[], engine=u'CUDNN')
I0304 15:49:47.745573  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.745793  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.745888  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.745997  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.746078  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.746148  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.746222  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.746299  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.746371  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.746438  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.746510  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.746582  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.746652  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.746718  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.746800  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.746896  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.746987  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
I0304 15:49:47.747066  9381 operator.cc:173] Operator with engine CUDNN is not available for operator Relu.
I0304 15:49:47.747159  9381 operator.cc:173] Operator with engine CUDNN is not available for operator ReluGradient.
.
----------------------------------------------------------------------
Ran 1 test in 0.160s
OK
测试3：在ecli中引用caffe2，
出现：
WARNING:root:Debug message: No module named caffe2_pybind11_state_gpu
测试4：
from caffe2.python import core
出现：
python caffe2/python/operator_test/relu_op_test.py
Traceback (most recent call last):
  File "caffe2/python/operator_test/relu_op_test.py", line 21, in <module>
    from caffe2.python import core
ImportError: No module named caffe2.python
解决：添加路径
``#echo$PYTHONPATHexport PYTHONPATH=/usr/local:$PYTHONPATHexport PYTHONPATH=$PYTHONPATH:/home/wishchin/CNN/caffe2/build#echo$LD_LIBRARY_PATHexport LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
之后
测试成功...............
