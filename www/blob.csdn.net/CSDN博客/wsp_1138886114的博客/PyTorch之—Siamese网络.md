# PyTorch之—Siamese网络 - wsp_1138886114的博客 - CSDN博客





2019年04月15日 15:26:15[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：57标签：[PyTorch之—Siamese网络](https://so.csdn.net/so/search/s.do?q=PyTorch之—Siamese网络&t=blog)
个人分类：[pytorch](https://blog.csdn.net/wsp_1138886114/article/category/8293033)












### 文章目录
- [1. 数据集处理](#1__6)
- [2. 网络与损失函数](#2__10)
- [3. 代码如下：](#3__17)



本来是想做检测图像的相似度的，偶然见到这篇文章。于是写下了这篇博文。

本文参考于：[github](https://github.com/harveyslash/Facial-Similarity-with-Siamese-Networks-in-Pytorch/tree/master) and [PyTorch 中文网人脸相似度对比](http://www.pytorchtutorial.com/pytorch-one-shot-learning/#Contrastive_Loss_function)


关于Siamese网络 [请查看](https://blog.csdn.net/ybdesire/article/details/84072339),或者[查看](https://www.jianshu.com/p/92d7f6eaacf5) 。就是两个共享参数的CNN。每次的输入是一对图像+1个label，共3个值。注意label=0或1（又称正负样本），表示输入的两张图片match（匹配、同一个人）或no-match（不匹配、非同一人）。 下图是Siamese基本结构
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190415151021432.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
##### 1. 数据集处理

数据采用的是AT&T人脸数据。共40个人，每个人有10张脸。

数据集下载：[https://files.cnblogs.com/files/king-lps/att_faces.zip](https://files.cnblogs.com/files/king-lps/att_faces.zip)

解压后文件夹下共40个文件夹，每个文件夹里有10张pgm图片。
##### 2. 网络与损失函数

显然该网络前向传播是两张图同时输入进行。

损失函数公式：m为容忍度，$D_w$ 为两张图之间的欧式距离。
$$\rm loss = (1-Y) \frac{1}{2}D_w^2 +  (Y) \frac{1}{2}\{max(0,m-D_w)\}^2 $$
$$\rm D_w = \sqrt{\{G_w(X_1) - G_w(X_2)\}^2}$$

##### 3. 代码如下：

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import torch
from torch.autograd import Variable
import os
import random
import linecache
import numpy as np
import torchvision
from torch.utils.data import Dataset, DataLoader
from torchvision import transforms
from PIL import Image
import PIL.ImageOps
import matplotlib.pyplot as plt


class Config():
    root = 'E:/pytorch_AI_learning/att_faces'
    txt_root = 'E:/pytorch_AI_learning/att_faces/train.txt'
    train_batch_size = 32
    train_number_epochs = 30


# Helper functions
def imshow(img, text=None, should_save=False):
    npimg = img.numpy()
    plt.axis("off")
    if text:
        plt.text(75, 8, text, style='italic', fontweight='bold',
                 bbox={'facecolor': 'white', 'alpha': 0.8, 'pad': 10})
    plt.imshow(np.transpose(npimg, (1, 2, 0)))
    plt.show()


def show_plot(iteration, loss):
    plt.plot(iteration, loss)
    plt.show()


def convert(train=True):
    if (train):
        f = open(Config.txt_root, 'w')
        data_path = Config.root + '/'
        if (not os.path.exists(data_path)):
            os.makedirs(data_path)
        for i in range(40):
            for j in range(10):
                img_path = data_path + 's' + str(i + 1) + '/' + str(j + 1) + '.pgm'
                f.write(img_path + ' ' + str(i) + '\n')
        f.close()


convert(True)  # 生成train.txt文件

# ready the dataset, Not use ImageFolder as the author did
class MyDataset(Dataset):

    def __init__(self, txt, transform=None, target_transform=None, should_invert=False):

        self.transform = transform
        self.target_transform = target_transform
        self.should_invert = should_invert
        self.txt = txt

    def __getitem__(self, index):

        line = linecache.getline(self.txt, random.randint(1, self.__len__()))
        line.strip('\n')
        img0_list = line.split()
        should_get_same_class = random.randint(0, 1)
        if should_get_same_class:
            while True:
                img1_list = linecache.getline(self.txt, random.randint(1, self.__len__())).strip('\n').split()
                if img0_list[1] == img1_list[1]:
                    break
        else:
            img1_list = linecache.getline(self.txt, random.randint(1, self.__len__())).strip('\n').split()

        img0 = Image.open(img0_list[0])
        img1 = Image.open(img1_list[0])
        img0 = img0.convert("L")
        img1 = img1.convert("L")

        if self.should_invert:
            img0 = PIL.ImageOps.invert(img0)
            img1 = PIL.ImageOps.invert(img1)

        if self.transform is not None:
            img0 = self.transform(img0)
            img1 = self.transform(img1)

        return img0, img1, torch.from_numpy(np.array([int(img1_list[1] != img0_list[1])], dtype=np.float32))

    def __len__(self):
        fh = open(self.txt, 'r')
        num = len(fh.readlines())
        fh.close()
        return num



""" =======Visualising some of the data==========
train_data = MyDataset(txt = Config.txt_root, transform=transforms.ToTensor(),
                       target_transform=transforms.Compose([transforms.Resize((100,100)),transforms.ToTensor()],))
train_loader = DataLoader(dataset=train_data, batch_size=8, shuffle=True)
it = iter(train_loader)
p1, p2, label = it.next()
example_batch = it.next()
concatenated = torch.cat((example_batch[0],example_batch[1]),0)
imshow(torchvision.utils.make_grid(concatenated))
print(example_batch[2].numpy())
"""



# Neural Net Definition, Standard CNNs
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim


class SiameseNetwork(nn.Module):
    def __init__(self):
        super(SiameseNetwork, self).__init__()
        self.cnn1 = nn.Sequential(
            nn.ReflectionPad2d(1),
            nn.Conv2d(1, 4, kernel_size=3),
            nn.ReLU(inplace=True),
            nn.BatchNorm2d(4),
            nn.Dropout2d(p=.2),

            nn.ReflectionPad2d(1),
            nn.Conv2d(4, 8, kernel_size=3),
            nn.ReLU(inplace=True),
            nn.BatchNorm2d(8),
            nn.Dropout2d(p=.2),

            nn.ReflectionPad2d(1),
            nn.Conv2d(8, 8, kernel_size=3),
            nn.ReLU(inplace=True),
            nn.BatchNorm2d(8),
            nn.Dropout2d(p=.2),
        )

        self.fc1 = nn.Sequential(
            nn.Linear(8 * 100 * 100, 500),
            nn.ReLU(inplace=True),

            nn.Linear(500, 500),
            nn.ReLU(inplace=True),

            nn.Linear(500, 5)
        )

    def forward_once(self, x):
        output = self.cnn1(x)
        output = output.view(output.size()[0], -1)
        output = self.fc1(output)
        return output

    def forward(self, input1, input2):
        output1 = self.forward_once(input1)
        output2 = self.forward_once(input2)
        return output1, output2


# Custom Contrastive Loss
class ContrastiveLoss(torch.nn.Module):
    """
    Contrastive loss function.
    Based on: http://yann.lecun.com/exdb/publis/pdf/hadsell-chopra-lecun-06.pdf
    """

    def __init__(self, margin=2.0):
        super(ContrastiveLoss, self).__init__()
        self.margin = margin

    def forward(self, output1, output2, label):
        euclidean_distance = F.pairwise_distance(output1, output2)
        loss_contrastive = torch.mean((1 - label) * torch.pow(euclidean_distance, 2) +
                                      (label) * torch.pow(torch.clamp(self.margin - euclidean_distance, min=0.0), 2))

        return loss_contrastive


# Training
train_data = MyDataset(txt=Config.txt_root, transform=transforms.Compose(
    [transforms.Resize((100, 100)), transforms.ToTensor()]), should_invert=False)
train_dataloader = DataLoader(dataset=train_data, shuffle=True, num_workers=2, batch_size=Config.train_batch_size)

net = SiameseNetwork().cuda()
criterion = ContrastiveLoss()
optimizer = optim.Adam(net.parameters(), lr=0.0005)

counter = []
loss_history = []
iteration_number = 0

if __name__ == '__main__':
    for epoch in range(0, Config.train_number_epochs):
        for i, data in enumerate(train_dataloader, 0):
            img0, img1, label = data
            img0, img1, label = Variable(img0).cuda(), Variable(img1).cuda(), Variable(label).cuda()
            output1, output2 = net(img0, img1)
            optimizer.zero_grad()
            loss_contrastive = criterion(output1, output2, label)
            loss_contrastive.backward()
            optimizer.step()

            if i % 10 == 0:
                print("Epoch:{},  Current loss {}\n".format(epoch, loss_contrastive.data.item()))
                iteration_number += 10
                counter.append(iteration_number)
                loss_history.append(loss_contrastive.data.item())
    show_plot(counter, loss_history)
    torch.save(net.state_dict(), './SiameseNet.pth')
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190415152545632.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)






