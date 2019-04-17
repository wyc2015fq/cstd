# Probabilistic decoder, Bayesian neural network, Probabilistic encoder - Keith - CSDN博客





2017年03月04日 14:09:02[ke1th](https://me.csdn.net/u012436149)阅读数：844








最近在看概率模型，看到这三种模型的时候老是分不开谁是谁，在此做个总结加强记忆。

## Probabilistic decoder


![这里写图片描述](https://img-blog.csdn.net/20170304140332345?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzNjE0OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Probabilistic decoder 的概率图模型
`Probabilistic decoder`是通过`coding theory`对 `likelihood`($p(x^n|z^n)$)进行重新解释。$p(x^n|z^n)$ 是一个条件概率分布，条件$z^n$ 被解释成 $x^n$ 的隐含表示。称作  `Probabilistic decoder` 的原因是，给定 $z^n$，解码出来的 $x^n$ 也是随机的。


对于实数来说： 


$p(x^n|z^n)=Normal\Biggr(x^n\Bigr|[\mu,\sigma^2]=NN(z^n;\theta)\Biggr)$
$NN(z^n;\theta)$ 是一个神经网络，输入是 $z^n$, 参数是 $\theta$。 `Probabilistic decoder` 使用神经网络的输出作为 $p(x^n)$ 的参数。
**NOTE: The neural network which parameterizes the probabilistic decoder is also known as a generative network**

**如何 `inference``Probabilistic decoder`?**

1. variational inference 

2. MAP 

3. ML
## Bayesian neural network

是一个概率模型，`Bayesian neural network` 是一个参数带先验分布的神经网络。即：参数是分布的神经网络。 

![这里写图片描述](https://img-blog.csdn.net/20170304140645724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzNjE0OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Bayesian neural network 的概率图模型
**如何 inference bayesian neural network?**

1. variational inference 

2. …


## Probabilistic encoder

最后一个. 
`probabilistic encoder` 又叫 `inference network`，也叫 `recognition model`。 
`Probabilistic decoder`是概率模型，而 `probabilistic encoder` 是一个变分推断模型，使用神经网络的输出作为 $q(z_i|\beta_i = NN(W, x_i))$ 分布的参数，W 是神经网络的参数。 
![这里写图片描述](https://img-blog.csdn.net/20170304140726500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzNjE0OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**这个和上面那个图不一样的地方是，上面那个图 `W` 是随机变量， 而这个图的 `W` 是模型参数**







