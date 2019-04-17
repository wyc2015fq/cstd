# pytorch 50行代码搭建ResNet-34（两种方法） - Snoopy_Dream - CSDN博客





2018年10月24日 09:55:21[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：80
所属专栏：[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)[pytorch yolov3](https://blog.csdn.net/column/details/30519.html)









![](https://img-blog.csdnimg.cn/2018112821531712.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181128215349595.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181128215404203.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181128215414495.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

#------------------------------用50行代码搭建ResNet-------------------------------------------

from torch import nn

import torch as t

from torch.nn import functional as F

class ResidualBlock(nn.Module):

    #实现子module: Residual    Block

    def __init__(self,inchannel,outchannel,stride=1,shortcut=None):

        super(ResidualBlock,self).__init__()

        self.left=nn.Sequential(

            nn.Conv2d(inchannel,outchannel,3,stride,1,bias=False),

            nn.BatchNorm2d(outchannel),

            nn.ReLU(inplace=True),

            nn.Conv2d(outchannel,outchannel,3,1,1,bias=False),

            nn.BatchNorm2d(outchannel)

        )


        self.right=shortcut


    def forward(self,x):

        out=self.left(x)

        residual=x if self.right is None else self.right(x)

        out+=residual

        return F.relu(out)



class ResNet(nn.Module):

    #实现主module:ResNet34

    #ResNet34包含多个layer,每个layer又包含多个residual block

    #用子module实现residual block , 用 _make_layer 函数实现layer

    def __init__(self,num_classes=1000):

        super(ResNet,self).__init__()

        self.pre=nn.Sequential(

            nn.Conv2d(3,64,7,2,3,bias=False),

            nn.BatchNorm2d(64),

            nn.ReLU(inplace=True),

            nn.MaxPool2d(3,2,1)

        )

        #重复的layer,分别有3,4,6,3个residual block

        self.layer1=self._make_layer(64,64,3)

        self.layer2=self._make_layer(64,128,4,stride=2)

        self.layer3=self._make_layer(128,256,6,stride=2)

        self.layer4=self._make_layer(256,512,3,stride=2)


        #分类用的全连接

        self.fc=nn.Linear(512,num_classes)


    def _make_layer(self,inchannel,outchannel,block_num,stride=1):

        #构建layer,包含多个residual block

        shortcut=nn.Sequential(

            nn.Conv2d(inchannel,outchannel,1,stride,bias=False),

            nn.BatchNorm2d(outchannel))

        layers=[ ]

        layers.append(ResidualBlock(inchannel,outchannel,stride,shortcut))


        for i in range(1,block_num):

            layers.append(ResidualBlock(outchannel,outchannel))

        return nn.Sequential(*layers)


    def forward(self,x):

        x=self.pre(x)


        x=self.layer1(x)

        x=self.layer2(x)

        x=self.layer3(x)

        x=self.layer4(x)


        x=F.avg_pool2d(x,7)

        x=x.view(x.size(0),-1)

        return self.fc(x)

model=ResNet()

input=t.autograd.Variable(t.randn(1,3,224,224))

o=model(input)

print(o)



大致框架算是理解了，但是细节部分比如卷积层的输入输出的大小之类的，还需要仔细研究。



Pytorch学习系列（一）至（四）均摘自《深度学习框架PyTorch入门与实践》陈云





---------------------  

作者：寻找如意  

来源：CSDN  

原文：https://blog.csdn.net/qq_34447388/article/details/79503643  

版权声明：本文为博主原创文章，转载请附上博文链接！



