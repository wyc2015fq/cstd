# Pytorch之深入torchvision.transforms.ToTensor与ToPILImage - qq_37385726的博客 - CSDN博客





2018年08月18日 18:25:53[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：2287
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









# **目录**

[torchvision.transforms.ToTensor](#torchvision.transforms.ToTensor)

[torchvision.transforms.ToPILImage](#torchvision.transforms.ToPILImage)

# torchvision.transforms.ToTensor

对于一个图片img，调用ToTensor转化成张量的形式，**发生的不是将图片的RGB三维信道矩阵变成tensor**

图片在内存中以bytes的形式存储，转化过程的步骤是：
- **img.tobytes()  将图片转化成内存中的存储格式**
- **torch.BytesStorage.frombuffer(img.tobytes() )  将字节以流的形式输入，转化成一维的张量**
- **对张量进行reshape**
- **对张量进行transpose**
- **将当前张量的每个元素除以255**
- **输出张量**



# torchvision.transforms.ToPILImage

对于一个Tensor的转化过程是：
- **将张量的每个元素乘上255**
- **将张量的数据类型有FloatTensor转化成Uint8**
- **将张量转化成numpy的ndarray类型**
- **对ndarray对象做transpose (1, 2, 0)的操作**
- **利用Image下的fromarray函数，将ndarray对象转化成PILImage形式**
- **输出PILImage**



