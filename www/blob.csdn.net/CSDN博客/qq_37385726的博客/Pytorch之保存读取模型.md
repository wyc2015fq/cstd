# Pytorch之保存读取模型 - qq_37385726的博客 - CSDN博客





2018年08月22日 14:47:26[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：2498








# **目录**

转自[这里](https://cloud.tencent.com/developer/article/1149988)

[pytorch保存数据](#pytorch%E4%BF%9D%E5%AD%98%E6%95%B0%E6%8D%AE)

[pytorch读取数据](#pytorch%E8%AF%BB%E5%8F%96%E6%95%B0%E6%8D%AE)

## pytorch保存数据

pytorch保存数据的格式为**.t7文件**或者**.pth文件**，**t7文件是沿用torch7中读取模型权重的方式。而pth文件是python中存储文件的常用格式**。而在keras中则是使用.h5文件。

```
# 保存模型示例代码
print('===> Saving models...')
state = {
    'state': model.state_dict(),
    'epoch': epoch                   # 将epoch一并保存
}
if not os.path.isdir('checkpoint'):
    os.mkdir('checkpoint')
torch.save(state, './checkpoint/autoencoder.t7')
```

保存用到`torch.save`函数，注意该函数第一个参数可以是单个值也可以是字典，字典可以存更多你要保存的参数（不仅仅是权重数据）。

## pytorch读取数据

pytorch读取数据使用的方法和我们平时使用预训练参数所用的方法是一样的，都是使用`load_state_dict`这个函数。

下方的代码和上方的保存代码可以搭配使用。

```
print('===> Try resume from checkpoint')
if os.path.isdir('checkpoint'):
    try:
        checkpoint = torch.load('./checkpoint/autoencoder.t7')
        model.load_state_dict(checkpoint['state'])        # 从字典中依次读取
        start_epoch = checkpoint['epoch']
        print('===> Load last checkpoint data')
    except FileNotFoundError:
        print('Can\'t found autoencoder.t7')
else:
    start_epoch = 0
    print('===> Start from scratch')
```

以上是pytorch读取的方法汇总，但是要**注意**，在使用官方的预处理模型进行读取时，一般使用的格式是pth，**使用官方的模型读取命令会检查你模型的格式是否正确，如果不是使用官方提供模型通过下面的函数强行读取模型(将其他模型例如caffe模型转过来的模型放到指定目录下)会发生错误。**

```
def vgg19(pretrained=False, **kwargs):
    """VGG 19-layer model (configuration "E")

    Args:
        pretrained (bool): If True, returns a model pre-trained on ImageNet
    """
    model = VGG(make_layers(cfg['E']), **kwargs)
    if pretrained:
        model.load_state_dict(model_zoo.load_url(model_urls['vgg19']))
    return model
```

假如我们有从caffe模型转过来的pytorch模型([0-255,BGR])，我们可以使用：

```
model_dir = '自己的模型地址'
model = VGG()
model.load_state_dict(torch.load(model_dir + 'vgg_conv.pth'))
```

也就是pytorch的读取函数进行读取即可。



