# 手写DCGAN - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//加上了注释，对pytorch又加深了理解
import torch as t
from torch import nn
from torch.autograd import Variable
from torch.optim import Adam
from torchvision import transforms
from torchvision.utils import make_grid
from torch.utils.data import DataLoader
from torchvision.datasets import CIFAR10
from pylab import plt  #pylab结合了pyplot和numpy

class Config:
    lr = 0.0002
    nz = 100 #噪声维度
    image_size = 64
    image_size2 = 64
    nc = 3 #图片是三通道的
    ngf = 64 #G的特征层数
    ndf = 64 #D的特征层数
    beta1 = 0.5
    batch_size = 32
    max_epoch = 10
    workers = 0
    gpu = True

opt = Config()

#载入数据
transform = transforms.Compose([
    transforms.Resize(opt.image_size),
    transforms.ToTensor(),
    transforms.Normalize([0.5]*3,[0.5]*3) #均值&标准差
])

dataset = CIFAR10(root='cifar10/',transform=transform,download=True)
dataloader = DataLoader(dataset,opt.batch_size,shuffle=True,num_workers=opt.workers)

#输入的是噪声图片的维度
netg = nn.Sequential(
    nn.ConvTranspose2d(opt.nz,opt.ngf*8,4,1,0,bias=False),
    nn.BatchNorm2d(opt.ngf*8),
    nn.ReLU(True),

    nn.ConvTranspose2d(opt.ngf*8,opt.ngf*4,4,2,1,bias=False),
    nn.BatchNorm2d(opt.ngf*4),
    nn.ReLU(True),

    nn.ConvTranspose2d(opt.ngf*4,opt.ngf*2,4,2,1,bias=False),
    nn.BatchNorm2d(opt.ngf*2),
    nn.ReLU(True),

    nn.ConvTranspose2d(opt.ngf*2,opt.ngf,4,2,1,bias=False),
    nn.BatchNorm2d(opt.ngf),
    nn.ReLU(True),

    nn.ConvTranspose2d(opt.ngf,opt.nc,4,2,1,bias=False),
    nn.Tanh()    #输出的是FAKE图片的维度
)

netd = nn.Sequential(
    nn.Conv2d(opt.nc,opt.ndf,4,2,1,bias=False),
    nn.LeakyReLU(0.2,inplace=True),

    nn.Conv2d(opt.ndf, opt.ndf*2, 4, 2, 1, bias=False),
    nn.BatchNorm2d(opt.ndf*2),
    nn.LeakyReLU(0.2, inplace=True),

    nn.Conv2d(opt.ndf*2, opt.ndf*4, 4, 2, 1, bias=False),
    nn.BatchNorm2d(opt.ndf * 4),
    nn.LeakyReLU(0.2, inplace=True),

    nn.Conv2d(opt.ndf*4, opt.ndf * 8, 4, 2, 1, bias=False),
    nn.BatchNorm2d(opt.ndf * 8),
    nn.LeakyReLU(0.2, inplace=True),

    nn.Conv2d(opt.ndf*8,1,4,1,0,bias=False),
    nn.Sigmoid()
)

#optimizer
optimizerD = Adam(netd.parameters(),lr = opt.lr,betas=(opt.beta1,0.999))
optimizerG = Adam(netg.parameters(),lr = opt.lr,betas=(opt.beta1,0.999))

#criterion
criterion = nn.BCELoss()

fix_noise = Variable(t.FloatTensor(opt.batch_size,opt.nz,1,1).normal_(0,1))#高斯分布N(0,1)
if opt.gpu:
    fix_noise = fix_noise.cuda()
    netd.cuda()
    netg.cuda()
    criterion.cuda()

print("开始训练")


for epoch in range(opt.max_epoch):
    for ii,data in enumerate(dataloader,start=0):
        real,_ = data
        input = Variable(real)
        label = Variable(t.ones(input.size(0)))#一开始训练DIS用real image 所以给的label都是1，所以这个label大小和batch_size大小一样
        noise = t.randn(input.size(0),opt.nz,1,1)#不是很理解后面两个1是干啥用的
        noise = Variable(noise)

        if opt.gpu:
            noise = noise.cuda()
            input = input.cuda()
            label = label.cuda()

        #____train disc____
        netd.zero_grad()
        #用real image train
        output = netd(input)
        loss_real = criterion(output.squeeze(),label)#output 与 1之间的loss
        loss_real.backward()
        # D_x = output.data.mean()#这是平均loss
        #用fake image train
        fake_pic = netg(noise).detach()#截断反向传播，只影响G不影响D
        output2 = netd(fake_pic)
        label.data.fill_(0) #把label的1改成0，因为是fake image
        loss_fake = criterion(output2.squeeze(),label)
        loss_fake.backward()
        # D_x2 = output2.data.mean()
        error_D = loss_real+loss_fake
        optimizerD.step()

        #_____train generator__
        netg.zero_grad()
        label.data.fill_(1) #要计算的是生存的图片与真实的loss,所以是1
        noise.data.normal_(0,1)#产生0-1的高斯噪声
        fake_pic = netg(noise)
        output = netd(fake_pic)
        loss_G = criterion(output.squeeze(),label)
        loss_G.backward()
        optimizerG.step()
        # D_G_z2 = output.data.mean()

    if epoch%2 == 0:
        fake_u = netg(fix_noise)
        imgs = make_grid(fake_u.data*0.5+0.5).cpu() #chw
        plt.imshow(imgs.permute(1,2,0).numpy())
        plt.show()
```













