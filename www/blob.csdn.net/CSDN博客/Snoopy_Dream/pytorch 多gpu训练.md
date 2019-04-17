# pytorch 多gpu训练 - Snoopy_Dream - CSDN博客





2018年12月06日 18:19:02[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：96
个人分类：[pytorch](https://blog.csdn.net/e01528/article/category/8254835)









在torch.nn.DataParallel

#多gpu训练

device_ids = [0,1]

....

model=resnetv2sn18()



if use_gpu and len(device_ids)>1:#多gpu训练

    model = model.cuda(device_ids[0])

    model = nn.DataParallel(model, device_ids=device_ids)



if use_gpu and len(device_ids)==1:#单gpu训练

    model = model.cuda(device_ids[0])



