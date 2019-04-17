# torch学习（四） using GPU - xmdxcsj的专栏 - CSDN博客





2015年10月20日 20:57:54[xmucas](https://me.csdn.net/xmdxcsj)阅读数：7626








一、获取GPU信息

require'cutorch'print( 
 cutorch.getDeviceProperties(cutorch.getDevice()))

二、GPU和CPU之间的数据互通

1.构建GPU数据


t1 = torch.CudaTensor(100):fill(0.5)

t2 = torch.CudaTensor(100):fill(1)
t1:add(t2)


2.GPU=>CPU


t1_cpu = t1:float()

3.CPU=>GPU


t1:zero()

t1[{}]= t1_cpu *-- copies the data back to the GPU, with no new alloc*
t1_new = t1_cpu:cuda()*-- allocates a new tensor*


三、nn使用GPU

Module参数是Torch的默认类型，如果需要Cuda的module，需要把默认类型修改为CUda。

1.构建基于Cuda的nn

require'cunn'

*-- we define an MLP*

mlp = nn.Sequential()

mlp:add(nn.Linear(ninput, 1000))

mlp:add(nn.Tanh())

mlp:add(nn.Linear(1000, 1000))

mlp:add(nn.Tanh())

mlp:add(nn.Linear(1000, 1000))

mlp:add(nn.Tanh())

mlp:add(nn.Linear(1000, noutput))

*-- and move it to the GPU:*
mlp:cuda()


输入数据需要首先转化为cuda

*-- input*

input = torch.randn(ninput)

*-- retype and feed to network:*

result = mlp:forward( input:cuda())

*-- the result is a CudaTensor, if your loss is CPU-based, then you will**-- need to bring it back:*
result_cpu = result:float()


可以增加Copy层自动进行数据类型转化

*-- we put the mlp in a new container:*

mlp_auto = nn.Sequential()

mlp_auto:add(nn.Copy('torch.FloatTensor', 'torch.CudaTensor'))

mlp_auto:add(mlp)

mlp_auto:add(nn.Copy('torch.CudaTensor', 'torch.FloatTensor'))



