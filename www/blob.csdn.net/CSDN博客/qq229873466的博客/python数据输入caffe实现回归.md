# python数据输入caffe实现回归 - qq229873466的博客 - CSDN博客

2017年05月11日 20:13:19[qq229873466](https://me.csdn.net/qq229873466)阅读数：588


```python
from numpy import *
import caffe

x = random.random((100, 1)).astype(dtype=float32)
y = 2.0 * x + 0.5

caffe.set_mode_cpu()
solver = caffe.SGDSolver('solver.prototxt')
solver.net.blobs['data'].data[...] = x
solver.net.blobs['label'].data[...] = y
solver.solve()

from matplotlib import pyplot as plt
net = caffe.Net('deploy.prototxt','test_iter_300.caffemodel', caffe.TEST)
tx = random.random((100, 1)).astype(dtype=float32)

net.blobs['data'].data[...] = tx

out = net.forward()
ty = out['out']

print('w = %f, b = %f' % (net.params['ip1'][0].data, net.params['ip1'][1].data))
plt.figure()
plt.plot(tx, ty)
plt.show()
```

solver.prototxt

```python
train_net: "train.prototxt"
base_lr: 0.1
display: 10
max_iter: 300
lr_policy: "step"
gamma: 0.1
momentum: 0.9
weight_decay: 0.0005
stepsize: 200
snapshot: 300
snapshot_prefix: "test"
solver_mode: CPU
device_id: 0
```

train.prototxt

```python
layer {
  name: "data"
  type: "Input"
  top: "data"
  top: "label"
  input_param {
    shape{
      dim:100
      dim:1
    }
    shape{
      dim:100
      dim:1
    }
  }
}
layer {
  name: "ip1"
  type: "InnerProduct"
  bottom: "data"
  top: "ip1"
  inner_product_param {
    num_output: 1
    weight_filler {
      type: "xavier"
    }
  }
}
layer {
  name: "loss"
  type: "EuclideanLoss"
  bottom: "ip1"
  bottom: "label"
  top: "loss"
}
```

deploy.prototxt

```python
layer {
  name: "data"
  type: "Input"
  top: "data"
  input_param {
    shape{
      dim:100
      dim:1
    }
  }

}

layer {
  name: "ip1"
  type: "InnerProduct"
  bottom: "data"
  top: "out"
  inner_product_param {
    num_output: 1
    weight_filler {
      type: "xavier"
    }
  }
}
```


