# pytorch to caffe - qq_30006593的博客 - CSDN博客





2018年08月18日 15:29:44[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：1857








# 安装caffe, gpu版本的,可以直接通过conda安装
`conda install caffe-gpu`
# pytorch安装:[官网](https://pytorch.org/)

```
conda install pytorch torchvision cuda80 -c pytorch
```

# pytorch to caffe

这里使用一个开源的代码: [pytorchtocaffe](https://github.com/longcw/pytorch2caffe)

```
model_def = os.path.join(caffemodel_dir, 'model.prototxt')
model_weights = os.path.join(caffemodel_dir, 'model.caffemodel')
input_name = 'ConvNdBackward1'
output_name = 'AddmmBackward348'

# pytorch net
model = torchvision.models.resnet18(False)
m = torch.load('xx.pt')
model.load_state_dict(m)
model.eval()

# random input
image = np.random.randint(0, 255, input_size)
input_data = image.astype(np.float32)

# pytorch forward
input_var = Variable(torch.from_numpy(input_data))

output_var = model(input_var)
plot_graph(output_var, os.path.join(caffemodel_dir, 'pytorch_graph'))
pytorch2caffe(input_var, output_var, model_def, model_weights)
```





