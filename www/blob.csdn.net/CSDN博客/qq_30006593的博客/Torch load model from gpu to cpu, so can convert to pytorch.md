# Torch load model from gpu to cpu, so can convert to pytorch - qq_30006593的博客 - CSDN博客





2017年09月01日 19:42:46[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：880








# 安装

# load and save

lua -i 开启交互式界面 

require 导入包 

‘torch’ : load 

Sequential :’nn’ ‘cunn’ 根据提示的缺少来添加
[from gpu to cpu]()

m = torch.load(‘xxxx.t7’) 

cudnn.conver(m,nn) 

m = m:float() 

torch.save(‘xxxx.cpu.t7’,m) 
[convert to pytorch](https://discuss.pytorch.org/t/convert-import-torch-model-to-pytorch/37/2)





