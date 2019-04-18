# 深度炼丹：用 GAN 生成 CG 图片，可以直接在浏览器运行的深度卷积网络生成模型 - 知乎
# 



深度学习最重要的是什么？最重要的是数据。昨天爬了几万张高质量 CG，拿最简单的 DCGAN 跑了几百个 epoch：
![](https://pic2.zhimg.com/v2-0b7b2c53097e9375258d5bf9a19fa2f9_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='665' height='693'></svg>)
虽然不一定看得出在画什么，但是色彩确实已经有些艺术的感觉。如果把模型做细致一点，生成更大的图片，或许某些可以骗进现代艺术展。

我把它做成了 JS 程序，读者可以点击打开下面这个网页，自己生成无穷无尽的图片（建议在电脑打开，因为计算量较大）：

**[深度炼丹：用 GAN 生成 CG 图片 Use GAN to generate CG artworks](https://link.zhihu.com/?target=http%3A//withablink.coding.me/GAN/)**

按目前的经验，如果用 state-of-the-art 的网络来跑，同时配合更多的输入数据，效果会好得多，可以在这个基础上进一步达到“看得出在画什么”的程度。

那么，未来 CG 画师是否会受到冲击？其实也有可能是 AI 提供辅助工具，让作画的过程更方便快捷，就像 “一键补充细节”，“一键转换风格”，“一键生成3D” 等等。

**如果觉得此文有帮助，请记得点个赞噢。**如需转载本文，请先与本人联系，谢谢。

说说过程：
- 先把图片爬下来，剪切缩放到尺寸。
- 然后用 MXNet 的 im2rec 做成可以直接读取的数据文件。
- 然后在 jupyter notebook 运行代码即可。
- 可以用 MXNet 的 amalgamation 做成可以在浏览器运行。

**另外，如果有 MXNet 开发团队的同学看到，很建议加上 amalgamation 后 WebGL 加速的支持。现在 TF 有 TensorFire 了，Keras 早就有 Keras.js 了，这确实是潮流，加个 weBLAS 之类并不会很难。或者也可以给大家一个 MXNet 转 TF 模型的工具，会很方便许多事情。**

训练代码，基本就是 MXNet 现成的：


```python
from __future__ import print_function
import mxnet as mx
import numpy as np
from matplotlib import pyplot as plt
import logging
import cv2
from datetime import datetime

loadG = None #"imagenet_G_2017_08_11-20_24-0090.params"
loadD = None #"imagenet_D_2017_08_11-20_24-0090.params"

def make_dcgan_sym(ngf, ndf, nc, no_bias=True, fix_gamma=True, eps=1e-5 + 1e-12):
    BatchNorm = mx.sym.BatchNorm
    rand = mx.sym.Variable('rand')

    g1 = mx.sym.Deconvolution(rand, name='g1', kernel=(,), num_filter=ngf*, no_bias=no_bias)
    gbn1 = BatchNorm(g1, name='gbn1', fix_gamma=fix_gamma, eps=eps)
    gact1 = mx.sym.Activation(gbn1, name='gact1', act_type='relu')

    g2 = mx.sym.Deconvolution(gact1, name='g2', kernel=(,), stride=(,), pad=(,), num_filter=ngf*, no_bias=no_bias)
    gbn2 = BatchNorm(g2, name='gbn2', fix_gamma=fix_gamma, eps=eps)
    gact2 = mx.sym.Activation(gbn2, name='gact2', act_type='relu')

    g3 = mx.sym.Deconvolution(gact2, name='g3', kernel=(,), stride=(,), pad=(,), num_filter=ngf*, no_bias=no_bias)
    gbn3 = BatchNorm(g3, name='gbn3', fix_gamma=fix_gamma, eps=eps)
    gact3 = mx.sym.Activation(gbn3, name='gact3', act_type='relu')

    g4 = mx.sym.Deconvolution(gact3, name='g4', kernel=(,), stride=(,), pad=(,), num_filter=ngf, no_bias=no_bias)
    gbn4 = BatchNorm(g4, name='gbn4', fix_gamma=fix_gamma, eps=eps)
    gact4 = mx.sym.Activation(gbn4, name='gact4', act_type='relu')

    g5 = mx.sym.Deconvolution(gact4, name='g5', kernel=(,), stride=(,), pad=(,), num_filter=nc, no_bias=no_bias)
    gout = mx.sym.Activation(g5, name='gact5', act_type='tanh')

    data = mx.sym.Variable('data')
    label = mx.sym.Variable('label')

    d_fix_gamma = False

    d1 = mx.sym.Convolution(data, name='d1', kernel=(,), stride=(,), pad=(,), num_filter=ndf, no_bias=no_bias)
    dact1 = mx.sym.LeakyReLU(d1, name='dact1', act_type='leaky', slope=0.2)

    d2 = mx.sym.Convolution(dact1, name='d2', kernel=(,), stride=(,), pad=(,), num_filter=ndf*, no_bias=no_bias)
    dbn2 = BatchNorm(d2, name='dbn2', fix_gamma=d_fix_gamma, eps=eps)
    dact2 = mx.sym.LeakyReLU(dbn2, name='dact2', act_type='leaky', slope=0.2)

    d3 = mx.sym.Convolution(dact2, name='d3', kernel=(,), stride=(,), pad=(,), num_filter=ndf*, no_bias=no_bias)
    dbn3 = BatchNorm(d3, name='dbn3', fix_gamma=d_fix_gamma, eps=eps)
    dact3 = mx.sym.LeakyReLU(dbn3, name='dact3', act_type='leaky', slope=0.2)

    d4 = mx.sym.Convolution(dact3, name='d4', kernel=(,), stride=(,), pad=(,), num_filter=ndf*, no_bias=no_bias)
    dbn4 = BatchNorm(d4, name='dbn4', fix_gamma=d_fix_gamma, eps=eps)
    dact4 = mx.sym.LeakyReLU(dbn4, name='dact4', act_type='leaky', slope=0.2)

    d5 = mx.sym.Convolution(dact4, name='d5', kernel=(,), num_filter=, no_bias=no_bias)
    d5 = mx.sym.Flatten(d5)

    dloss = mx.sym.LogisticRegressionOutput(data=d5, label=label, name='dloss')
    return gout, dloss

class RandIter(mx.io.DataIter):
    def __init__(self, batch_size, ndim):
        self.batch_size = batch_size
        self.ndim = ndim
        self.provide_data = [('rand', (batch_size, ndim, , ))]
        self.provide_label = []

    def iter_next(self):
        return True

    def getdata(self):
        return [mx.random.normal(, 1.0, shape=(self.batch_size, self.ndim, , ))]

class ImagenetIter(mx.io.DataIter):
    def __init__(self, path, batch_size, data_shape):
        self.internal = mx.io.ImageRecordIter(
            path_imgrec = path,
            data_shape  = data_shape,
            batch_size  = batch_size,
            preprocess_threads = ,
            shuffle = True,
            min_img_size = ,
            max_img_size = ,
            rand_crop   = False,
            rand_mirror = False)
        self.provide_data = [('data', (batch_size,) + data_shape)]
        self.provide_label = []

    def reset(self):
        self.internal.reset()

    def iter_next(self):
        return self.internal.iter_next()

    def getdata(self):
        data = self.internal.getdata()
        data = data * (2.0/255.0)
        data -= 
        return [data]

def fill_buf(buf, i, img, shape):
    n = buf.shape[]/shape[]
    m = buf.shape[]/shape[]

    sx = (i%m)*shape[]
    sy = (i/m)*shape[]
    buf[sy:sy+shape[], sx:sx+shape[], :] = img

def visual(title, X):
    assert len(X.shape) == 
    X = X.transpose((, , , ))
    X = np.clip((X+1.0)*(255.0/2.0), , ).astype(np.uint8)
    n = np.ceil(np.sqrt(X.shape[]))
    buff = np.zeros((int(n*X.shape[]), int(n*X.shape[]), int(X.shape[])), dtype=np.uint8)
    for i, img in enumerate(X):
        fill_buf(buff, i, img, X.shape[:])
    #buff = cv2.cvtColor(buff, cv2.COLOR_BGR2RGB)
    plt.imshow(buff)
    plt.title(title)
plt.show()

if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)

    # =============setting============
    dataset = 'imagenet'
    imgnet_path = 'img64.rec'
    ndf = 
    ngf = 
    nc = 
    batch_size = 
    Z = 
    lr = 0.0002
    beta1 = 0.5
    ctx = mx.gpu()
    check_point = True

    symG, symD = make_dcgan_sym(ngf, ndf, nc)
    #mx.viz.plot_network(symG, shape={'rand': (batch_size, 100, 1, 1)}).view()
    #mx.viz.plot_network(symD, shape={'data': (batch_size, nc, 64, 64)}).view()

    # ==============data==============
    train_iter = ImagenetIter(imgnet_path, batch_size, (, , ))
    rand_iter = RandIter(batch_size, Z)
    label = mx.nd.zeros((batch_size,), ctx=ctx)

    # =============module G=============
    modG = mx.mod.Module(symbol=symG, data_names=('rand',), label_names=None, context=ctx)
    modG.bind(data_shapes=rand_iter.provide_data)

    if loadG:
        modG.init_params(initializer=mx.init.Load(loadG))
    else:
        modG.init_params(initializer=mx.init.Normal(0.02))

    modG.init_optimizer(
        optimizer='adam',
        optimizer_params={
            'learning_rate': lr,
            'wd': 0.,
            'beta1': beta1,
        })
    mods = [modG]

    # =============module D=============
    modD = mx.mod.Module(symbol=symD, data_names=('data',), label_names=('label',), context=ctx)
    modD.bind(data_shapes=train_iter.provide_data,
              label_shapes=[('label', (batch_size,))],
              inputs_need_grad=True)

    if loadD:
        modD.init_params(initializer=mx.init.Load(loadD))
    else:
        modD.init_params(initializer=mx.init.Normal(0.02))

    modD.init_optimizer(
        optimizer='adam',
        optimizer_params={
            'learning_rate': lr,
            'wd': 0.,
            'beta1': beta1,
        })
    mods.append(modD)

    # ============printing==============
    def norm_stat(d):
        return mx.nd.norm(d)/np.sqrt(d.size)
    mon = mx.mon.Monitor(, norm_stat, pattern=".*output|d1_backward_data", sort=True)
    mon = None
    if mon is not None:
        for mod in mods:
            pass

    def facc(label, pred):
        pred = pred.ravel()
        label = label.ravel()
        return ((pred > 0.5) == label).mean()

    def fentropy(label, pred):
        pred = pred.ravel()
        label = label.ravel()
        return -(label*np.log(pred+1e-12) + (1.-label)*np.log(1.-pred+1e-12)).mean()

    mG = mx.metric.CustomMetric(fentropy)
    mD = mx.metric.CustomMetric(fentropy)
    mACC0 = mx.metric.CustomMetric(facc)
    mACC1 = mx.metric.CustomMetric(facc)

    print('Training...')
    stamp =  datetime.now().strftime('%Y_%m_%d-%H_%M')

symG.save('%s_G_%s.json'%(dataset, stamp))
symD.save('%s_D_%s.json'%(dataset, stamp))

    # =============train===============
    for epoch in range(+):
        train_iter.reset()
        for t, batch in enumerate(train_iter):
            rbatch = rand_iter.next()

            if mon is not None:
                mon.tic()

            modG.forward(rbatch, is_train=True)
            outG = modG.get_outputs()

            # update discriminator on fake
            label[:] = 
            modD.forward(mx.io.DataBatch(outG, [label]), is_train=True)
            modD.backward()
            #modD.update()
            gradD = [[grad.copyto(grad.context) for grad in grads] for grads in modD._exec_group.grad_arrays]

            modD.update_metric(mD, [label])
            modD.update_metric(mACC0, [label])

            # update discriminator on real
            label[:] = 
            batch.label = [label]
            modD.forward(batch, is_train=True)
            modD.backward()
            for gradsr, gradsf in zip(modD._exec_group.grad_arrays, gradD):
                for gradr, gradf in zip(gradsr, gradsf):
                    gradr += gradf
            modD.update()

            modD.update_metric(mD, [label])
            modD.update_metric(mACC1, [label])

            # update generator
            label[:] = 
            modD.forward(mx.io.DataBatch(outG, [label]), is_train=True)
            modD.backward()
            diffD = modD.get_input_grads()
            modG.backward(diffD)
            modG.update()

            mG.update([label], modD.get_outputs())


            if mon is not None:
                mon.toc_print()

            t += 
            if t %  == :
                print('epoch:', epoch, 'iter:', t, 'metric:', mACC0.get(), mACC1.get(), mG.get(), mD.get())
                mACC0.reset()
                mACC1.reset()
                mG.reset()
                mD.reset()

        visual('gout', outG[].asnumpy())
#        diff = diffD[0].asnumpy()
#        diff = (diff - diff.mean())/diff.std()
#        visual('diff', diff)
        visual('data', batch.data[].asnumpy())

        if check_point and epoch %  == :
            print('Saving...')
            modG.save_params('%s_G_%s-%04d.params'%(dataset, stamp, epoch))
            modD.save_params('%s_D_%s-%04d.params'%(dataset, stamp, epoch))
```


**如果觉得此文有帮助，请记得点个赞噢。**如需转载本文，请先与本人联系，谢谢。

欢迎关注我的专栏： [技术备忘录 - 知乎专栏](https://zhuanlan.zhihu.com/mathNote)，其中有更多文章。 


