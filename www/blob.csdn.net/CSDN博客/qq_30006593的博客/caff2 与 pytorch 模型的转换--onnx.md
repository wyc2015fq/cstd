# caff2 与 pytorch 模型的转换--onnx - qq_30006593的博客 - CSDN博客





2018年08月18日 15:08:16[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：1436








onnx和caffe2的安装参考另一篇[博客](https://blog.csdn.net/qq_30006593/article/details/81808878)

caffe2新版本中添加了onnx-caffe2,所以无需再安装

# Pytorch to ONNX

```
import torch
from pnasnet import PNASNet5Large
from torch.autograd import Variable

model = PNASNet5Large(50)
m = torch.load('pnas_meitu_001-9708.pt')
model.load_state_dict(m)
model.train(False)

x = Variable(torch.randn(1, 3, 331, 331))
y = model(x)
torch_out = torch.onnx._export(model,  # model being run
                               x,  # model input (or a tuple for multiple inputs)
                               "pnas.onnx",  # where to save the model
                               export_params=True)
```

# ONNX to Caffe2

直接运行

```
import onnx
import caffe2.python.onnx.backend as bc
import numpy as np

x = np.zeros((1, 3, 331, 331)).astype(np.float32)
model = onnx.load("pnas.onnx")
out = bc.run_model(model, [x])
```

 或者直接转成caffe2的model
`convert-onnx-to-caffe2 assets/squeezenet.onnx --output predict_net.pb --init-net-output init_net.pb`




