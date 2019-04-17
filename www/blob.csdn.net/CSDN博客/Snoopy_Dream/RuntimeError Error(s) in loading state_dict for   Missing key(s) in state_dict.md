# RuntimeError: Error(s) in loading state_dict for ***: Missing key(s) in state_dict: - Snoopy_Dream - CSDN博客





2018年12月12日 23:19:55[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：1052








原因，model需要先放到cuda上，根据你的设置，所以是load_state_dict错了。

原来：

model = se_resnet20_v2()

model.load_state_dict(torch.load('/home/zj/senetial/save_models/SENet/SEnet_model_2.pth')

if use_gpu and len(device_ids)>1:#多gpu训练
model = model.cuda(device_ids[0])

    model = nn.DataParallel(model, device_ids=device_ids)

if use_gpu and len(device_ids)==1:#单gpu训练

    model = model.cuda()

print model

#model=TSnet(17)

model = se_resnet20_v2()

改：

if use_gpu and len(device_ids)>1:#多gpu训练
model = model.cuda(device_ids[0])

    model = nn.DataParallel(model, device_ids=device_ids)

if use_gpu and len(device_ids)==1:#单gpu训练

    model = model.cuda()

print model

model.load_state_dict(torch.load('/home/zj/senetial/save_models/SENet/SEnet_model_2.pth'))



**单GPU：**

import os

os.environ["CUDA_VISIBLE_DEVICES"] = "0"

**多GPU：**

device_ids = [0,1,2,3]


model = model.cuda(device_ids[0])

model = nn.DataParallel(model, device_ids=device_ids)


optimizer = optim.SGD(model.parameters(), lr=learning_rate, momentum=0.9, weight_decay=0.001)

optimizer = nn.DataParallel(optimizer, device_ids=device_ids)


optimizer.module.step()


for param_lr in optimizer.module.param_groups:  # 同样是要加module

        #     param_lr['lr'] = param_lr['lr'] * 0.999

**加载多GPU预训练模型**

model = ft_net()

pretained_model = torch.load('./model/all/8_model.pkl')

pretained_dict = pretained_model.module.state_dict()

model = ft_net()

model.load_state_dict(pretained_dict)






