# pytorch学习笔记（四）：输入流水线(input pipeline) - Keith - CSDN博客





2017年04月01日 21:54:22[ke1th](https://me.csdn.net/u012436149)阅读数：7480
所属专栏：[pytorch学习笔记](https://blog.csdn.net/column/details/15023.html)









## input-pipeline

### 引包

```python
from torchvision import transforms
from torch.utils.data import Dataset
from torch.utils.data import DataLoader
```

### **图像预处理**

```python
# 创建个transform用来处理图像数据
transform = transforms.Compose([
    transforms.Scale(40),
    transforms.RandomHorizontalFlip(),
    transforms.RandomCrop(32),
    transforms.ToTensor()])
```

### **准备数据**

```python
# 下载数据
train_dataset = dsets.CIFAR10(root='./data/',
                               train=True,
                               transform=transform,#用了之前定义的transform
                               download=True)

image, label = train_dataset[0]
print (image.size())
print (label)
```

```
Files already downloaded and verified
torch.Size([3, 32, 32])
6
```

### **加载数据**

```python
# data loader提供了队列和线程
train_loader = data.DataLoader(dataset=train_dataset,
                               batch_size=100,# 这里定义了batch_size
                               shuffle=True,
                               num_workers=2)
```

```python
# 迭代开始，然后，队列和线程跟着也开始
data_iter = iter(train_loader)

# mini-batch 图像 和 标签
images, labels = next(data_iter)

for images, labels in train_loader:
    # 这里是训练代码
    pass
```



