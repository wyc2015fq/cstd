# Caffe2：python -m caffe2.python.operator_test.relu_op_test - wishchinYang的专栏 - CSDN博客
2018年01月30日 16:01:29[wishchin](https://me.csdn.net/wishchin)阅读数：694
1. 进行语句测试时候，出现问题，
      设置环境变量*CUDA_VISIBLE_DEVICES*
参考： cuda设置指定可见方法
在/etc/profile文件或者～/.bashrc末尾添加以下行：
       export CUDA_VISIBLE_DEVICES=0,1 ##仅显卡设备0,1GPU可见。可用的GPU可通过nvidia-smi -L命令查看
2. 出现     ImportError: No module named hypothesis
     pip install hypothesis
测试成功
/usr/lib/python2.7/dist-packages/numpy/core/numeric.py:301: FutureWarning: in the future, full(9, True) will return an array of dtype('bool')
  format(shape, fill_value, array(fill_value).dtype), FutureWarning)
Trying example: test_relu(self=<__main__.TestRelu testMethod=test_relu>, X=array([[-0.27628136, -0.27628136, -0.27628136],
       [-0.27628136, -0.27628136, -0.27628136],
       [-0.27628136, -0.27628136, -0.89451963]], dtype=float32), gc=, dc=[], engine=u'')
.
----------------------------------------------------------------------
Ran 1 test in 0.183s
OK
