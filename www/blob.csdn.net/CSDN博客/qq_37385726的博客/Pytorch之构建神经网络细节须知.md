# Pytorch之构建神经网络细节须知 - qq_37385726的博客 - CSDN博客





2018年08月19日 13:25:52[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：100
个人分类：[Pytorch](https://blog.csdn.net/qq_37385726/article/category/7938821)

所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)








- **trainset = dsets.CIFAR10(root='./CIFAR10/net2/train',train=True,transform=transform1,download=DOWNLOAD)**


- 
```
trainload = Data.DataLoader(trainset,batch_size=BATCH_SIZE,shuffle=True)                         
for step,data in enumerate(trainload,0):
    inputs,labels = data   
inputs的size是： (batch_size, 3, 32, 32) FloatTensor型  （在下载的时候经过transform的处理）
labels的size是： (batch_size)  FloatTensor型  （在下载的时候经过transform的处理）
```

- 
```
for step,data in enumerate(testset):
    input,label = data
input的size是： (3,32,32) FloatTensor型 （在下载的时候经过transform的处理）
label的size是： 单个整数  int型
```





- 
**output = net(input)   **

- **传入参数可以为tensor也可以为variable，但尽量传variable吧，反正input在测试的时候都要包装成variable，做参数更新**
- **input的size要求为 (*, 3, 32, 32)**
- **output的size是： (1, 10)，FloatTensor型 **


- **loss = loss_func(outputs, labels) **
- **传入参数可以为tensor也可以为variable，但一般传tensor，因为不论是output还是labels都不用包装。数据类型必须为FlaotTensor**
- **返回的loss一个Tensor，但是size为()，只有一个floatTensor型的数**





