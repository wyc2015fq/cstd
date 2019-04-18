# PyTorch之—图像分类一（每个类对应一个文件夹） - wsp_1138886114的博客 - CSDN博客





2019年02月27日 17:29:27[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：304











### 文章目录
- [一、数据集准备与训练](#_1)
- [二、预测或推断](#_124)




### 一、数据集准备与训练

数据按文件夹组织，每个类对应一个文件夹。大多数其他的PyTorch教程和示例都希望你先按照训练集和验证集来组织文件夹，然后在训练集和验证集中再按照类别进行组织。但我认为这非常麻烦，必须从每个类别中选择一定数量的图像并将它们从训练集文件夹移动到验证集文件夹。由于大多数人会通过选择一组连续的文件作为验证集，因此选择可能存在很多偏差。

如下将使用随机打乱的方式：将数据集快速分为训练集和测试集的更好的方法。

```python
import matplotlib.pyplot as plt
import numpy as np
import torch
from torch import nn
from torch import optim
from torch.utils.data import DataLoader
from torchvision import datasets, transforms, models


"""
    =========================== 模型训练与验证 ================================
"""
# 定义train/validation数据集加载器

data_dir = 'D:\\AI-img-classifier\\re\\train'

def load_split_train_test(datadir,valid_size = 0.2):
    train_trainsforms = transforms.Compose([transforms.Resize((224,224)),
                                            transforms.ToTensor(),])
    test_trainsforms = transforms.Compose([transforms.Resize((224,224)),
                                           transforms.ToTensor(),])

    train_data = datasets.ImageFolder(datadir,transform=train_trainsforms)
    # print("train_data大小：",train_data[0][0].size())       # 查看resize(确保图像都有3通道)
    test_data = datasets.ImageFolder(datadir,transform=test_trainsforms)

    num_train = len(train_data)                               # 训练集数量
    indices = list(range(num_train))                          # 训练集索引

    split = int(np.floor(valid_size * num_train))             # 获取20%数据作为验证集
    np.random.shuffle(indices)                                # 打乱数据集

    from torch.utils.data.sampler import SubsetRandomSampler
    train_idx, test_idx = indices[split:], indices[:split]    # 获取训练集，测试集
    train_sampler = SubsetRandomSampler(train_idx)            # 打乱训练集，测试集
    test_sampler  = SubsetRandomSampler(test_idx)

    #============数据加载器：加载训练集，测试集===================
    train_loader = DataLoader(train_data,sampler=train_sampler,batch_size=64)
    test_loader = DataLoader(test_data,sampler=test_sampler,batch_size=64)
    return train_loader,test_loader


train_loader,test_loader = load_split_train_test(data_dir, 0.2)
print(train_loader.dataset.classes)


device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model = models.resnet50(pretrained=True)
print(model)

"""
   首先，我们必须冻结预训练过的层，因此在训练期间它们不会进行反向传播。
   然后，我们重新定义最后的全连接层，即使用我们的图像来训练的图层。
        我们还创建了标准（损失函数）并选择了一个优化器（在这种情况下为Adam）和学习率。
"""
for param in model.parameters():
    param.requires_grad = False

model.fc = nn.Sequential(nn.Linear(2048,512),
                         nn.ReLU(),
                         nn.Dropout(0.2),
                         nn.Linear(512,5),
                         nn.LogSoftmax(dim=1))
criterion = nn.NLLLoss()
optimizer = optim.Adam(model.fc.parameters(),lr=0.003)
model.to(device)


epochs = 10
steps = 0
running_loss = 0
train_losses,test_losses = [],[]


for epoch in range(epochs):
    for inputs,labels in train_loader:
        inputs,labels = inputs.to(device),labels.to(device)
        optimizer.zero_grad()
        out = model(inputs)
        loss = criterion(out,labels)
        loss.backward()
        optimizer.step()
        running_loss +=loss.item()
        steps +=1

        if (steps+1)%5 == 0:
            test_loss = 0
            accuracy = 0
            model.eval()
            with torch.no_grad():
                for inputs,labels in test_loader:
                    inputs, labels = inputs.to(device), labels.to(device)
                    out2 = model(inputs)
                    batch_loss = criterion(out2,labels)
                    test_loss +=batch_loss.item()

                    ps = torch.exp(out2)
                    top_pred,top_class = ps.topk(1,dim=1)
                    equals = top_class == labels.view(*top_class.shape)
                    accuracy += torch.mean(equals.type(torch.FloatTensor)).item()

            train_losses.append(running_loss/len(train_loader))
            test_losses.append(test_loss/len(test_loader))

            print(f"Epoch {epoch+1}/{epochs}"
                  f"Train loss: {running_loss/5:.3f}",
                  f"Test loss: {test_loss/len(test_loader):.3f} "
                  f"Test accuracy: {accuracy/len(test_loader):.3f}")
            running_loss = 0
            model.train()
torch.save(model,"./pytorch_Image_Training.pth")

plt.plot(train_losses,label = "Training loss")
plt.plot(test_losses, label='Validation loss')
plt.legend(frameon=False)
plt.show()
```

### 二、预测或推断

代码一：

```python
import matplotlib.pyplot as plt
import numpy as np
import torch
from torch.utils.data import DataLoader
from torch.autograd import Variable
from torchvision import datasets, transforms



"""
    =========================== 模型预测与使用 ================================
"""
# 测试数据集，设置图像大小与训练一项
data_dir2 = 'D:\\AI-img-classifier\\re\\train'
test_trainsforms = transforms.Compose([transforms.Resize((224,224)),
                                       transforms.ToTensor(),])

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model = torch.load('./pytorch_Image_Training.pth')
model.eval()
print(model)


# 预测图像（输入具体图像：eg:"./xxx.jpg"）
def predict_img(image):
    image_tensor = test_trainsforms(image).float()
    image_tensor = image_tensor.unsqueeze_(0)
    input = Variable(image_tensor).to(device)
    output = model(input)
    index = output.data.cpu().numpy().argmax()
    return index


# 获取随机图像
def get_random_images(num):
    data = datasets.ImageFolder(data_dir2,transform=test_trainsforms)
    classes = data.classes
    print("classes:",classes)

    indices = list(range(len(data)))
    np.random.shuffle(indices)
    idx = indices[:num]                 # 获取随机的预测数据

    from torch.utils.data.sampler import SubsetRandomSampler
    sampler = SubsetRandomSampler(idx) # 获取的预测数据再次打乱
    loader = DataLoader(data,sampler=sampler,batch_size=num)
    dataiter = iter(loader)

    images,labels = dataiter.next()
    return images,labels,classes

if __name__ == '__main__':
    path = 'D:\\AI-img-classifier\\re\\train'
    to_pil = transforms.ToPILImage()
    images, labels ,classes= get_random_images(10)
    fig = plt.figure(figsize=(10,10))
    for i in range(len(images)):
        image = to_pil(images[i])
        index = predict_img(image)
        sub = fig.add_subplot(1,len(images),i+1)
        res = int(labels[i]) == index
        str(sub.set_title(str(classes[index])))+":"+str(res)
        plt.axis('off')
        plt.imshow(image)
    plt.show()
```

代码二：

```python
import torch
import matplotlib.pyplot as plt
import numpy as np
from torch.utils.data import DataLoader
from torch.autograd import Variable
from torchvision import datasets, transforms
import torchvision

"""
    =========================== 模型预测与使用 ================================
"""

def loadtestdata(img_path,num):
    testset = datasets.ImageFolder(img_path,
                                   transform=transforms.Compose([
                                       transforms.Resize((224, 224)),
                                       transforms.ToTensor()]))
    testloader = DataLoader(testset, batch_size=num,shuffle=True, num_workers=4)

    dataiter = iter(testloader)
    images,labels = dataiter.next()
    classes = testloader.dataset.classes
    print("classes:",classes)
    return images,labels,classes

def imshow(img):
    np_img = img.numpy()
    plt.imshow(np.transpose(np_img, (1, 2, 0)))
    plt.show()


def test(weghts_path,img_path,num):
    net = torch.load(weghts_path)
    images, labels, classes = loadtestdata(img_path,num)
    imshow(torchvision.utils.make_grid(images, nrow=5))                               # nrow是每行显示的图片数量，缺省值为8
    print('GroundTruth: ', " ".join('%5s' % classes[labels[j]] for j in range(num)))  # 打印（test集里图片的标签）

    outputs = net(Variable(images.cuda()))
    _, predicted = torch.max(outputs.data, 1)
    print('Predicted: ', " ".join('%5s' % classes[predicted[j]] for j in range(num)))

if __name__ == '__main__':
    weghts_path = './pytorch_Image_Training.pth'
    img_path = "D:\\AI-img-classifier\\re\\train"
    test(weghts_path,img_path,num=10)         #请输入测试图像数量
```

代码三：

```python
import torch
from torch.autograd import Variable
from torchvision import transforms
import os
import shutil
from PIL import Image


def get_img(image_path):
    img_list = []
    for i in os.listdir(image_path):
        img_list.append(image_path+"/"+i)
    return img_list

def load_models():
    model = torch.load('./pytorch_Image_Training.pth')
    model.eval()
    return model

def predict_image(image_path):
    image = Image.open(image_path)

    transformation = transforms.Compose([
        transforms.Resize((224, 224)),
        transforms.ToTensor(),
        transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])

    image_tensor = transformation(image).float() # 预处理图像
    image_tensor = image_tensor.unsqueeze_(0)    # 额外添加一个批次维度，因为PyTorch将所有的图像当做批次

    if torch.cuda.is_available():
        image_tensor.cuda()

    model = load_models()

    input = Variable(image_tensor.cuda())   # 将输入变为变量
    output = model(input)                   # 预测图像的类
    index = output.data.cpu().numpy().argmax()
    return index

if __name__ == '__main__':
    image_path = r'E:/pytorch_project/re/test/'
    img_list = get_img(image_path)
    for i in img_list:
        index = predict_image(i)
        id2tag = {0:'bus',1: 'dinosaurs',2: 'elephants', 3:'flowers', 4:'hourse'}
        print(index)
        print(id2tag[index])
        target = 'E:/pytorch_project/re/'+id2tag[index]
        if os.path.exists(target):
            shutil.copy(i, target)
        else:
            os.mkdir(target)
        shutil.copy(i, target)
```

**报错与解决：**

报错1:

```python
Traceback (most recent call last):
  File "E:/pytorch_project/pytorch_classifier/pytorch_Image_Training.py", line 77, in <module>
    for inputs,labels in train_loader:
  File "C:\Anaconda3\envs\AI_PyTorch_GPU\lib\site-packages\torch\utils\data\dataloader.py", line 615, in __next__
    batch = self.collate_fn([self.dataset[i] for i in indices])
  File "C:\Anaconda3\envs\AI_PyTorch_GPU\lib\site-packages\torch\utils\data\dataloader.py", line 232, in default_collate
    return [default_collate(samples) for samples in transposed]
  File "C:\Anaconda3\envs\AI_PyTorch_GPU\lib\site-packages\torch\utils\data\dataloader.py", line 232, in <listcomp>
    return [default_collate(samples) for samples in transposed]
  File "C:\Anaconda3\envs\AI_PyTorch_GPU\lib\site-packages\torch\utils\data\dataloader.py", line 209, in default_collate
    return torch.stack(batch, 0, out=out)
RuntimeError: invalid argument 0: Sizes of tensors must match except in dimension 0. Got 224 and 336 in dimension 2 at c:\a\w\1\s\tmp_conda_3.6_105809\conda\conda-bld\pytorch_1544094150554\work\aten\src\th\generic/THTensorMoreMath.cpp:1333
```

网上关于此类报错：说是图像的通道问题（有的图像没有三个通道），关于此类问题[详情请查看](https://blog.csdn.net/qq_38156052/article/details/81293412)

我的不是图像的问题（你可以打印出训练图像的通道等信息），在大神的帮助下，我查看了train_data中其中一个`train_data[0][0].size()`大小，发现图像只有宽进行l了Resize，高没有进行Resize，修改后代码如上。

报错2:

```python
TypeError: exp(): argument 'input' (position 1) must be Tensor, not float
```

传参数据格式错误

报错3:

```python
Traceback (most recent call last):
  File "E:/pytorch_project/pytorch_classifier/pytorch_Image_Inference.py", line 53, in <module>
    images, labels ,classes= get_random_images(5)
  File "E:/pytorch_project/pytorch_classifier/pytorch_Image_Inference.py", line 48, in get_random_images
    images,labels = dataiter.next()
  File "C:\Anaconda3\envs\AI_PyTorch_GPU\lib\site-packages\torch\utils\data\dataloader.py", line 615, in __next__
    batch = self.collate_fn([self.dataset[i] for i in indices])
  File "C:\Anaconda3\envs\AI_PyTorch_GPU\lib\site-packages\torch\utils\data\dataloader.py", line 615, in <listcomp>
    batch = self.collate_fn([self.dataset[i] for i in indices])
  File "C:\Anaconda3\envs\AI_PyTorch_GPU\lib\site-packages\torchvision\datasets\folder.py", line 103, in __getitem__
    sample = self.transform(sample)
TypeError: 'module' object is not callable
```

传入数据格式错误



