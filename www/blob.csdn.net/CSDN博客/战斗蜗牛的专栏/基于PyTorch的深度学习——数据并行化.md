# 基于PyTorch的深度学习——数据并行化 - 战斗蜗牛的专栏 - CSDN博客





2018年02月08日 21:24:45[vbskj](https://me.csdn.net/vbskj)阅读数：2255









版权声明：本文为博主原创文章，未经博主允许不得转载。[http://blog.csdn.net/zzlyw/article/details/78769012](http://blog.csdn.net/zzlyw/article/details/78769012)



**前言**

本文参考PyTorch官网的教程，分为五个基本模块来介绍PyTorch。为了避免文章过长，这五个模块分别在五篇博文中介绍。



[Part1：PyTorch简单知识](http://blog.csdn.net/zzlyw/article/details/78768991)

[Part2：PyTorch的自动梯度计算](http://blog.csdn.net/zzlyw/article/details/78768996)

[Part3：使用PyTorch构建一个神经网络](http://blog.csdn.net/zzlyw/article/details/78769001)

[Part4：训练一个神经网络分类器](http://blog.csdn.net/zzlyw/article/details/78769006)

[Part5：数据并行化](http://blog.csdn.net/zzlyw/article/details/78769012)








本文是关于Part5的内容。



**Part5：数据并行化**

本文中，将会讲到DataParallel使用多GPU。

在PyTorch中使用GPU比较简单，可以这样把模型放到GPU上。


model.gpu()





还可以复制所有的tensors到GPU上。


mytensor=my_tensor.gpu()





请注意，单纯调用mytensor.gpu()不会拷贝tensor到GPU上。你需要把它分配给一个新的tensor，然后在GPU上使用这个新的tensor。

前向和反向传播可以在多个GPU上运行。但是，PyTorch默认只使用一个GPU。你可以使用DataParallel使得你的模型可以在过个GPU上并行运算。


model=nn.DataParallel(model)





**1 Package导入和参数设置**

导入PyTorch的模块并且设置参数。


importtorchimporttorch.nnasnnfromtorch.autogradimportVariablefromtorch.utils.dataimportDataset,DataLoader# Parameters and DataLoadersinput_size=output_size=batch_size=data_size=





**2 虚拟数据集**

制作虚拟（随机）数据集，只需要执行getitem。


classRandomDataset(Dataset):def__init__(self,size,length):self.len=lengthself.data=torch.randn(length,size)def__getitem__(self,index):returnself.data[index]def__len__(self):returnself.lenrand_loader=DataLoader(dataset=RandomDataset(input_size,),batch_size=batch_size,shuffle=True)





**3 简单模型**

作为实例，我们的模型只是获取输入，进行线性运算，给出结果。但是，你可以把DataParallel应用到任何模型（CNN，RNN，Capsule Net等等）。


classModel(nn.Module):# Our modeldef__init__(self,input_size,output_size):super(Model,self).__init__()self.fc=nn.Linear(input_size,output_size)defforward(self,input):output=self.fc(input)print("  In Model: input size",input.size(),"output size",output.size())returnoutput



**4 创建模型和数据并行**

这是本篇教程的核心内容。我们需要制作一个模型实例，并检查是否有多个GPU。如果有多GPU，可以使用nn.DataParallel打包我们的model。之后，我们可以把利用model.gpu()把模型放到GPU上。


model=Model(input_size,output_size)iftorch.cuda.device_count()>:print("Let's use",torch.cuda.device_count(),"GPUs!")# dim = 0 [30, xxx] -> [10, ...], [10, ...], [10, ...] on 3 GPUsmodel=nn.DataParallel(model)iftorch.cuda.is_available():model.cuda()





**5 运行模型**


fordatainrand_loader:iftorch.cuda.is_available():input_var=Variable(data.cuda())else:input_var=Variable(data)output=model(input_var)print("Outside: input size",input_var.size(),"output_size",output.size())



期望输出：


InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])





**6 结果**

（1）如果有2 GPUs，可以看到


# on 2 GPUsLet's use 2 GPUs!InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])





（2）如果有3 GPUs，可以看到


Let's use 3 GPUs!InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])



（3）如果有8 GPUs，可以看到


Let's use 8 GPUs!InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])InModel:inputsizetorch.Size([,])outputsizetorch.Size([,])Outside:inputsizetorch.Size([,])output_sizetorch.Size([,])




**7 总结**

DataParallel将数据自动分割送到不同的GPU上处理，在每个模块完成工作后，DataParallel再收集整合这些结果返回。




