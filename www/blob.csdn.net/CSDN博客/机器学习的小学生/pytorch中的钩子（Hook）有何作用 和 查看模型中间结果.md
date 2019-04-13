
# pytorch中的钩子（Hook）有何作用 和 查看模型中间结果 - 机器学习的小学生 - CSDN博客


2018年04月19日 20:04:48[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：2327



```python
#coding=UTF-8
```
```python
import
```
```python
torch
```
```python
import
```
```python
caffe
```
```python
from
```
```python
PIL
```
```python
import
```
```python
Image
```
```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
from
```
```python
torch.autograd
```
```python
import
```
```python
Variable
```
```python
# caffemodel.
```
```python
# model.pkl
```
```python
import
```
```python
torchvision.models
```
```python
as
```
```python
models
alexnet = models.alexnet()
params = alexnet.load_state_dict(torch.load(
```
```python
'alexnet-owt-4df8aa71.pth'
```
```python
))
print(
```
```python
'The architecture of alexnet: '
```
```python
)
```
```python
#print(alexnet)
```
```python
#params = alexnet.state_dict()
```
```python
imgSize = [
```
```python
224
```
```python
,
```
```python
224
```
```python
]
img = Image.open(
```
```python
'cat.jpg'
```
```python
)
res_img = img.resize((imgSize[
```
```python
0
```
```python
],imgSize[
```
```python
1
```
```python
]))
img = np.double(res_img)
img = img[:,:,(
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
)]
```
```python
# rgb 2 bgr
```
```python
img = np.transpose(img, (
```
```python
2
```
```python
,
```
```python
0
```
```python
,
```
```python
1
```
```python
))
```
```python
# h * w *c==> c*h*w
```
```python
print(img.shape)
```
```python
#plt.imshow(img)
```
```python
#plt.show()
```
```python
'''
for k,v in params.items():
    print(k)
a = params['features.0.weight']
#print(params['features.0.weight'])
print(a.shape)
print(type(a))
print(a)
'''
```
```python
def
```
```python
vis_square
```
```python
(data)
```
```python
:
```
```python
"""Take an array of shape (n, height, width) or (n, height, width, 3)
       and visualize each (height, width) thing in a grid of size approx. sqrt(n) by sqrt(n)"""
```
```python
# normalize data for display
```
```python
data = (data - data.min()) / (data.max() - data.min())
```
```python
# force the number of filters to be square
```
```python
n = int(np.ceil(np.sqrt(data.shape[
```
```python
0
```
```python
])))
    padding = (((
```
```python
0
```
```python
, n **
```
```python
2
```
```python
- data.shape[
```
```python
0
```
```python
]),
               (
```
```python
0
```
```python
,
```
```python
1
```
```python
), (
```
```python
0
```
```python
,
```
```python
1
```
```python
))
```
```python
# add some space between filters
```
```python
+ ((
```
```python
0
```
```python
,
```
```python
0
```
```python
),) * (data.ndim -
```
```python
3
```
```python
))
```
```python
# don't pad the last dimension (if there is one)
```
```python
data = np.pad(data, padding, mode=
```
```python
'constant'
```
```python
, constant_values=
```
```python
1
```
```python
)
```
```python
# pad with ones (white)
```
```python
# tile the filters into an image
```
```python
data = data.reshape((n, n) + data.shape[
```
```python
1
```
```python
:]).transpose((
```
```python
0
```
```python
,
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
3
```
```python
) + tuple(range(
```
```python
4
```
```python
, data.ndim +
```
```python
1
```
```python
)))
    data = data.reshape((n * data.shape[
```
```python
1
```
```python
], n * data.shape[
```
```python
3
```
```python
]) + data.shape[
```
```python
4
```
```python
:])
    plt.imshow(data); plt.axis(
```
```python
'off'
```
```python
)
data_arr = np.zeros(shape=(
```
```python
1
```
```python
,
```
```python
3
```
```python
,imgSize[
```
```python
0
```
```python
],imgSize[
```
```python
1
```
```python
]),dtype=np.float32)
data_arr[
```
```python
0
```
```python
,...] = img
input_data = Variable(torch.from_numpy(data_arr).type(torch.FloatTensor))
feat_result  = []
grad_result = []
```
```python
def
```
```python
get_features_hook
```
```python
(self,input,output)
```
```python
:
```
```python
# number of input:
```
```python
print(
```
```python
'len(input): '
```
```python
,len(input))
```
```python
# number of output:
```
```python
print(
```
```python
'len(output): '
```
```python
,len(output))
    print(
```
```python
'###################################'
```
```python
)
    print(input[
```
```python
0
```
```python
].shape)
```
```python
# torch.Size([1, 3, 224, 224])
```
```python
print(
```
```python
'###################################'
```
```python
)
    print(output[
```
```python
0
```
```python
].shape)
```
```python
# torch.Size([64, 55, 55])
```
```python
feat_result.append(output.data.cpu().numpy())
```
```python
def
```
```python
get_grads_hook
```
```python
(self,input_grad, output_grad)
```
```python
:
```
```python
# number of input:
```
```python
print(
```
```python
'len(input): '
```
```python
, len(input_grad))
```
```python
# number of output:
```
```python
print(
```
```python
'len(output): '
```
```python
, len(output_grad))
    print(
```
```python
'###################################'
```
```python
)
    print(input_grad[
```
```python
0
```
```python
])
```
```python
# None
```
```python
print(input_grad[
```
```python
1
```
```python
].shape)
```
```python
# torch.Size([64, 3, 11, 11])  for weights
```
```python
print(input_grad[
```
```python
2
```
```python
].shape)
```
```python
# torch.Size([64])             for bias
```
```python
print(
```
```python
'###################################'
```
```python
)
    print(output_grad[
```
```python
0
```
```python
].shape)
```
```python
# torch.Size([1, 64, 55, 55]) for x
```
```python
print(
```
```python
'###################################'
```
```python
)
    grad_result.append(output_grad[
```
```python
0
```
```python
].data.cpu().numpy())
handle_feat = alexnet.features[
```
```python
0
```
```python
].register_forward_hook(get_features_hook)
```
```python
# conv1
```
```python
handle_grad = alexnet.features[
```
```python
0
```
```python
].register_backward_hook(get_grads_hook)

num_class =
```
```python
1000
```
```python
a  = alexnet(input_data)
print(
```
```python
'a.shape: '
```
```python
, a.shape)
a.backward(torch.ones(
```
```python
1
```
```python
,num_class))
```
```python
#### remove handle
```
```python
handle_feat.remove()
handle_grad.remove()
feat1 = feat_result[
```
```python
0
```
```python
]
grad1 = grad_result[
```
```python
0
```
```python
]
vis_square(feat1[
```
```python
0
```
```python
,...])
```
```python
#plt.show()
```
```python
plt.savefig(
```
```python
'feat_visual.png'
```
```python
)
vis_square(grad1[
```
```python
0
```
```python
,...])
```
```python
#plt.show()
```
```python
plt.savefig(
```
```python
'grad_x.png'
```
```python
)
print(
```
```python
'save feature and gradx over ...'
```
```python
)
```
第一层卷积后的结果：
![这里写图片描述](https://img-blog.csdn.net/20180419213239476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
关于第一层结果的梯度（好像是）：
![这里写图片描述](https://img-blog.csdn.net/20180419213252624?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
摘自：
1.[https://www.zhihu.com/question/61044004](https://www.zhihu.com/question/61044004)【pytorch中的钩子（Hook）有何作用】
2.[https://blog.csdn.net/manong_wxd/article/details/78720119](https://blog.csdn.net/manong_wxd/article/details/78720119)【PyTorch学习总结(一)——查看模型中间结果】

