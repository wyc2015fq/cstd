# PyTorch之一Fine-tuning / hook - wsp_1138886114的博客 - CSDN博客





2019年02月24日 11:10:42[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：83











### 文章目录
- [一、Fine tuning(微调)](#Fine_tuning_1)
- [二、计算机视觉方向的微调](#_10)
- [三、案例：](#_31)
- [固定层的向量导出](#_261)




### 一、Fine tuning(微调)

针对于某个任务，自己的训练数据不多，那怎么办？ 没关系，我们先找到一个同类的别人训练好的模型，把别人现成的训练好了的模型拿过来，换成自己的数据，调整一下参数，在训练一遍，（此后的训练需要冻结之前网络层的参数，训练之后添加的网络层参数）这就是微调（fine-tune）。

PyTorch里面提供的经典的网络模型都是官方通过Imagenet的数据集与训练好的数据，如果我们的数据训练数据不够，这些数据是可以作为基础模型来使用的。

**为什么要微调**：
- 对于数据集本身很小（几千张图片）的情况，从头开始训练具有几千万参数的大型神经网络是不现实的，因为越大的模型对数据量的要求越大，过拟合无法避免。这时候如果还想用上大型神经网络的超强特征提取能力，只能靠微调已经训练好的模型。
- 可以降低训练成，如果使用导出特征向量的方法进行迁移学习，后期的训练成本非常低，用 CPU 都完全无压力，没有深度学习机器也可以做。
- 前人花很大精力训练出来的模型在大概率上会比你自己从零开始搭的模型要强悍，没有必要重复造轮子。

### 二、计算机视觉方向的微调
- 
ConvNet as fixed feature extractor.： 其实这里有两种做法：

1.使用最后一个fc layer之前的fc layer获得的特征，学习个线性分类器(比如SVM)

2.重新训练最后一个fc layer- 
Fine-tuning the ConvNet

固定前几层的参数，只对最后几层进行fine-tuning,

对于上面两种方案有一些微调的小技巧，比如先计算出预训练模型的卷积层对所有训练和测试数据的特征向量，然后抛开预训练模型，只训练自己定制的简配版全连接网络。 这个方式的一个好处就是节省计算资源，每次迭代都不会再去跑全部的数据，而只是跑一下简配的全连接- 
**Pretrained models**

这个其实和第二种是一个意思，不过比较极端，使用整个pre-trained的model作为初始化，然后fine-tuning整个网络而不是某些层，但是这个的计算量是非常大的,就只相当于做了一个初始化。


**注意事项**
- 新数据集和原始数据集合类似，那么直接可以微调一个最后的FC层或者重新指定一个新的分类器
- 新数据集比较小和原始数据集合差异性比较大，那么可以使用从模型的中部开始训练，只对最后几层进行fine-tuning。**如果上面方法还是不行的化**那么最好是从新训练，只将预训练的模型作为一个新模型初始化的数据
- 新数据集的大小一定要与原始数据集相同，**训练时**CNN中输入的图片大小一定要相同，才不会报错。
- 如果数据集大小不同的话，可以在最后的fc层之前添加卷积或者pool层，使得最后的输出与fc层一致，但这样会导致准确度大幅下降，所以不建议这样做
- 对于不同的层可以设置不同的学习率，一般情况下建议，对于使用的原始数据做初始化的层设置的学习率要小于（一般可设置小于10倍）初始化的学习率，这样保证对于已经初始化的数据不会扭曲的过快，而使用初始化学习率的新层可以快速的收敛。

### 三、案例：

数据预处理，预览

```python
import os
import pandas as pd

DATA_ROOT = './dog_breed_identification'
all_labels_df = pd.read_csv(os.path.join(DATA_ROOT,'labels.csv'))
all_labels_df.head()

# 定义了两个字典，分别以名字和id作为对应，方便后面处理
breeds = all_labels_df.breed.unique()
breed2idx = dict((breed,idx) for idx,breed in enumerate(breeds))
idx2breed = dict((idx,breed) for idx,breed in enumerate(breeds))
len(breeds)

# 添加到列表中
all_labels_df['label_idx'] = [breed2idx[b] for b in all_labels_df.breed]
all_labels_df.head()
```
|all_labels_df.head() # 直接预览||all_labels_df.head() 添加 **[label_idx]列** 到列表中| | | | |
|----|----|----|----|----|----|----|
||id|breed||id|breed|label_idx|
|0|000bec180eb18c7604dcecc8fe0dba07|boston_bull|0|000bec180eb18c7604dcecc8fe0dba07|boston_bull|0|
|1|001513dfcb2ffafc82cccf4d8bbaba97|dingo|1|001513dfcb2ffafc82cccf4d8bbaba97|dingo|1|
|2|001cdf01b096e06d78e9e5112d419397|pekinese|2|001cdf01b096e06d78e9e5112d419397|pekinese|2|
|3|00214f311d5d2247d5dfe4fe24b2303d|bluetick|3|00214f311d5d2247d5dfe4fe24b2303d|bluetick|3|
|4|0021f9ceb3235effd7fcde7f7538ed62|golden_retriever|4|0021f9ceb3235effd7fcde7f7538ed62|golden_retriever|4|

```python
import os
import torch
import torch.utils.data
from torch.utils.data import Dataset, DataLoader
from torchvision import transforms
import pandas as pd
from PIL import Image
import torch.nn as nn
import torchvision.models as models
from sklearn.model_selection import StratifiedShuffleSplit



DATA_ROOT = './dog_breed_identification'
all_labels_df = pd.read_csv(os.path.join(DATA_ROOT,'labels.csv'))


breeds = all_labels_df.breed.unique()
breed2idx = dict((breed,idx) for idx,breed in enumerate(breeds))
idx2breed = dict((idx,breed) for idx,breed in enumerate(breeds))

# 添加到列表中
all_labels_df['label_idx'] = [breed2idx[b] for b in all_labels_df.breed]


class DogDataset(Dataset):
    def __init__(self, labels_df, img_path, transform=None):
        self.labels_df = labels_df
        self.img_path = img_path
        self.transform = transform

    def __len__(self):
        return self.labels_df.shape[0]

    def __getitem__(self, idx):
        image_name = os.path.join(self.img_path, self.labels_df.id[idx]) + '.jpg'
        img = Image.open(image_name)
        label = self.labels_df.label_idx[idx]

        if self.transform:
            img = self.transform(img)
        return img, label

IMG_SIZE = 224         # resnet50的输入是224的所以需要将图片统一大小
BATCH_SIZE= 128        # 这个批次大小需要占用(4.6-5g)/2的显存，如果内存超过10G可以改为512
IMG_MEAN = [0.485, 0.456, 0.406]
IMG_STD = [0.229, 0.224, 0.225]
CUDA=torch.cuda.is_available()
DEVICE = torch.device("cuda" if CUDA else "cpu")


train_transforms = transforms.Compose([
    transforms.Resize(IMG_SIZE),
    transforms.RandomResizedCrop(IMG_SIZE),
    transforms.RandomHorizontalFlip(),
    transforms.RandomRotation(30),
    transforms.ToTensor(),
    transforms.Normalize(IMG_MEAN, IMG_STD)
])

val_transforms = transforms.Compose([
    transforms.Resize(IMG_SIZE),
    transforms.CenterCrop(IMG_SIZE),
    transforms.ToTensor(),
    transforms.Normalize(IMG_MEAN, IMG_STD)
])


dataset_names = ['train', 'valid']
stratified_split = StratifiedShuffleSplit(n_splits=1, test_size=0.1, random_state=0)
train_split_idx, val_split_idx = next(iter(stratified_split.split(all_labels_df.id, all_labels_df.breed)))
train_df = all_labels_df.iloc[train_split_idx].reset_index()
val_df = all_labels_df.iloc[val_split_idx].reset_index()
print(len(train_df))
print(len(val_df))


image_transforms = {'train':train_transforms, 'valid':val_transforms}

train_dataset = DogDataset(train_df, os.path.join(DATA_ROOT,'train'), transform=image_transforms['train'])
val_dataset = DogDataset(val_df, os.path.join(DATA_ROOT,'train'), transform=image_transforms['valid'])
image_dataset = {'train':train_dataset, 'valid':val_dataset}

image_dataloader = {x:DataLoader(image_dataset[x],batch_size=BATCH_SIZE,shuffle=True,num_workers=0) for x in dataset_names}
dataset_sizes = {x:len(image_dataset[x]) for x in dataset_names}




model_ft = models.resnet50(pretrained=True)      # 这里自动下载官方的预训练模型，并且

for param in model_ft.parameters():
    param.requires_grad = False                  # 将所有的参数层进行冻结

print(model_ft.fc)                               # 这里打印下全连接层的信息
num_fc_ftr = model_ft.fc.in_features             # 获取到fc层的输入
model_ft.fc = nn.Linear(num_fc_ftr, len(breeds)) # 定义一个新的FC层
model_ft=model_ft.to(DEVICE)                     # 放到设备中
print("========================================")
print(model_ft)                                  # 最后再打印一下新的模型


criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam([{'params':model_ft.fc.parameters()}],
                             lr=0.001)           # 指定 新加的fc层的学习率

def train(model,device, train_loader, epoch):
    model.train()
    for batch_idx, data in enumerate(train_loader):
        x,y= data
        x=x.to(device)
        y=y.to(device)
        optimizer.zero_grad()
        y_hat= model(x)
        loss = criterion(y_hat, y)
        loss.backward()
        optimizer.step()
    print ('Train Epoch: {}\t Loss: {:.6f}'.format(epoch,loss.item()))


def test(model, device, test_loader):
    model.eval()
    test_loss = 0
    correct = 0
    with torch.no_grad():
        for i,data in enumerate(test_loader):
            x,y= data
            x=x.to(device)
            y=y.to(device)
            optimizer.zero_grad()
            y_hat = model(x)
            test_loss += criterion(y_hat, y).item() # sum up batch loss
            pred = y_hat.max(1, keepdim=True)[1]    # get the index of the max log-probability
            correct += pred.eq(y.view_as(pred)).sum().item()
    test_loss /= len(test_loader.dataset)
    print('\n Test set: Average loss: {:.4f}, Accuracy: {}/{} ({:.0f}%)\n'.format(
        test_loss, correct, len(val_dataset),
        100. * correct / len(val_dataset)))

for epoch in range(1, 10):
    train(model=model_ft,device=DEVICE, train_loader=image_dataloader["train"],epoch=epoch)
    test(model=model_ft, device=DEVICE, test_loader=image_dataloader["valid"])
```

如上代码：只训练了9次就达到了80%的准确率

但是每次训练都需要将一张图片在全部网络中进行计算，而且计算的结果每次都是一样的，这样浪费了很多计算的资源。 下面我们就将这些不进行反向传播或者说不更新网络权重参数层的计算结果保存下来，这样我们以后使用的时候就可以直接将这些结果输入到FC层或者以这些结果构建新的网络层，省去了计算的时间，并且这样如果只训练全连接层，CPU就可以完成了。

##### 固定层的向量导出

采用PyTorch比较高级的API，hook来处理了，我们要先定义一个hook函数

```python
model_ft=model_ft.to(DEVICE)
in_list= [] # 这里存放所有的输出
def hook(module, input, output):
    #input是一个tuple代表顺序代表每一个输入项，我们这里只有一项，所以直接获取
    #需要全部的参数信息可以使用这个打印
    #for val in input:
    #    print("input val:",val)
    for i in range(input[0].size(0)):
        in_list.append(input[0][i].cpu().numpy())

# 在相应的层注册hook函数，保证函数能够正常工作，我们这里直接hook 全连接层前面的pool层，获取pool层的输入数据，这样会获得更多的特征
model_ft.avgpool.register_forward_hook(hook)

# 开始获取输出，这里我们因为不需要反向传播，所以直接可以使用no_grad嵌套
with torch.no_grad():
    for batch_idx, data in enumerate(image_dataloader["train"]):
        x,y= data
        x=x.to(DEVICE)
        y=y.to(DEVICE)
        y_hat = model_ft(x)


features=np.array(in_list)
np.save("features",features)
```







