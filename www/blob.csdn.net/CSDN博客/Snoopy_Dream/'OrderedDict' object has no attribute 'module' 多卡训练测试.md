# 'OrderedDict' object has no attribute 'module' 多卡训练测试 - Snoopy_Dream - CSDN博客





2018年12月17日 09:55:09[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：264
个人分类：[pytorch](https://blog.csdn.net/e01528/article/category/8254835)









加载并跑起来多卡训练的模型来用来检测

```python
device_ids=[0,1]

use_gpu = torch.cuda.is_available()
#创建模型
model = se_resnet20_v2()  
model = model.cuda(device_ids[0])
model = nn.DataParallel(model, device_ids=device_ids)

#加载模型

pretained_model = torch.load('/home/zj/senetial/save_models/SENet/SEnet_model_33.pth')
model.load_state_dict(pretained_model)

#加载test数据
test_file = h5py.File('/home/zj/senetial/data/round1_test_a_20181109.h5', 'r')
test_sen1 = test_file['sen1']
test_sen2 = test_file['sen2']
count=test_sen1.shape[0]
```




f=open('/home/zj/senetial/data/test_33_meanstd_test.csv','w')

for i in tqdm(range(int(math.ceil(count/16.0)))):###不用管这里到底能不能整除，你进行向上取整就好

    onehot = np.zeros((16,17),dtype=int)

    images=mean_std(AddFeatures(np.concatenate((test_sen1[i*16:(i+1)*16,:,:,:],test_sen2[i*16:(i+1)*16,:,:,:]),axis=3)))#列表：可以自动识别是否到了最后
**inputs** = Variable((torch.from_numpy(images)).float().permute(0, 3, 1, 2).**cuda****(device_ids[0]))#一定要加device_ids不能默认（）**

    outputs = model(**inputs**)

    scores, preds = torch.max(outputs.data, 1)#preds

    for i in range(inputs.shape[0]):#注意这里不能用batchsize，只能用列表[:]后的shape

        onehot[i,preds[i]]=1

        f.writelines(','.join(map(str,onehot[i]))+'\n')



**单GPU：**

```python
import os
os.environ["CUDA_VISIBLE_DEVICES"] = "0"
```



**多GPU：**

```python
device_ids = [0,1,2,3]

model = model.cuda(device_ids[0])
model = nn.DataParallel(model, device_ids=device_ids)

optimizer = optim.SGD(model.parameters(), lr=learning_rate, momentum=0.9, weight_decay=0.001)
optimizer = nn.DataParallel(optimizer, device_ids=device_ids)

optimizer.module.step()
for param_lr in optimizer.module.param_groups:  # 同样是要加module
        #     param_lr['lr'] = param_lr['lr'] * 0.999
```



**加载多GPU预训练模型**

```python
model = ft_net()
pretained_model = torch.load('./model/all/8_model.pkl')
pretained_dict = pretained_model.module.state_dict()
model = ft_net()
model.load_state_dict(pretained_dict)
```





