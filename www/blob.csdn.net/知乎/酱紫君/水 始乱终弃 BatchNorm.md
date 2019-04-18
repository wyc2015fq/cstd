# 水: 始乱终弃 BatchNorm - 知乎
# 

如果你爱 BN 你就用它, 如果你不爱 BN 你就用了再抛弃他.

**I have a ConvolutionLayer:**

```
Weights<TensorT[C_output, C_input, K, K]>:  w
Biases<VectorT[C_output]>:  b
```

![\mathrm{Convolution::<TensorT(4)> ⟹ <TensorT(4)>}](https://www.zhihu.com/equation?tex=%5Cmathrm%7BConvolution%3A%3A%3CTensorT%284%29%3E+%E2%9F%B9+%3CTensorT%284%29%3E%7D)

![\mathrm{Convolution(x)}=w\cdot x+b](https://www.zhihu.com/equation?tex=%5Cmathrm%7BConvolution%28x%29%7D%3Dw%5Ccdot+x%2Bb)

**I have a BatchNormalizationLayer:**

```
Beta<VectorT[C]>:  \beta
Epsilon<ScalarT>:  \varepsilon
Gamma<VectorT[C]>: \gamma
MovingMean<VectorT[C]>:     \hat{u}
MovingVariance<VectorT[C]>: \sigma
```

There is also a `Momentum<ScalarT>` that is not related to Interface, just Ignore.

![\mathrm{BatchNorm::<TensorT[N,C,H,W]> ⟹ <TensorT[N,C,H,W]>}](https://www.zhihu.com/equation?tex=%5Cmathrm%7BBatchNorm%3A%3A%3CTensorT%5BN%2CC%2CH%2CW%5D%3E+%E2%9F%B9+%3CTensorT%5BN%2CC%2CH%2CW%5D%3E%7D)

![\mathrm{BatchNorm}(x)=\frac{x-\hat{u}}{\sqrt{\sigma^2+\varepsilon}}\cdot \gamma+\beta](https://www.zhihu.com/equation?tex=%5Cmathrm%7BBatchNorm%7D%28x%29%3D%5Cfrac%7Bx-%5Chat%7Bu%7D%7D%7B%5Csqrt%7B%5Csigma%5E2%2B%5Cvarepsilon%7D%7D%5Ccdot+%5Cgamma%2B%5Cbeta)

**Aha, I have The Res Unit!**

ResA: merge(+)(cv1->bn1->relu->cv2->bn2->relu)

![\begin{aligned} \mathrm{Convolution}\circ \mathrm{BatchNorm}(x) &=\frac{(w\cdot x+b)-\hat{u}}{\sqrt{\sigma^2+\varepsilon}}\cdot \gamma+\beta\\ &=\frac{w\cdot \gamma}{\sqrt{\sigma^2+\varepsilon}}\cdot x+\frac{b-\hat{u}}{\sqrt{\sigma^2+\varepsilon}} +\beta\\ &= \tilde{w}\cdot x+\tilde{b} \end{aligned}](https://www.zhihu.com/equation?tex=%5Cbegin%7Baligned%7D+%5Cmathrm%7BConvolution%7D%5Ccirc+%5Cmathrm%7BBatchNorm%7D%28x%29+%26%3D%5Cfrac%7B%28w%5Ccdot+x%2Bb%29-%5Chat%7Bu%7D%7D%7B%5Csqrt%7B%5Csigma%5E2%2B%5Cvarepsilon%7D%7D%5Ccdot+%5Cgamma%2B%5Cbeta%5C%5C+%26%3D%5Cfrac%7Bw%5Ccdot+%5Cgamma%7D%7B%5Csqrt%7B%5Csigma%5E2%2B%5Cvarepsilon%7D%7D%5Ccdot+x%2B%5Cfrac%7Bb-%5Chat%7Bu%7D%7D%7B%5Csqrt%7B%5Csigma%5E2%2B%5Cvarepsilon%7D%7D+%2B%5Cbeta%5C%5C+%26%3D+%5Ctilde%7Bw%7D%5Ccdot+x%2B%5Ctilde%7Bb%7D+%5Cend%7Baligned%7D)

ResB: merge(+)(cv1->relu->bn1->cv2->relu->bn2)

![\begin{aligned} \mathrm{BatchNorm}\circ \mathrm{Convolution}(x) &=w\cdot\left(\frac{x-\hat{u}}{\sqrt{\sigma^2+\varepsilon}}\cdot \gamma+\beta\right)+b\\ &=\frac{w\cdot \gamma}{\sqrt{\sigma^2+\varepsilon}}\cdot x-\frac{w\cdot \gamma\cdot \hat{u}}{\sqrt{\sigma^2+\varepsilon}} +\beta\cdot w+b\\ &= \tilde{w}\cdot x+\tilde{b} \end{aligned}](https://www.zhihu.com/equation?tex=%5Cbegin%7Baligned%7D+%5Cmathrm%7BBatchNorm%7D%5Ccirc+%5Cmathrm%7BConvolution%7D%28x%29+%26%3Dw%5Ccdot%5Cleft%28%5Cfrac%7Bx-%5Chat%7Bu%7D%7D%7B%5Csqrt%7B%5Csigma%5E2%2B%5Cvarepsilon%7D%7D%5Ccdot+%5Cgamma%2B%5Cbeta%5Cright%29%2Bb%5C%5C+%26%3D%5Cfrac%7Bw%5Ccdot+%5Cgamma%7D%7B%5Csqrt%7B%5Csigma%5E2%2B%5Cvarepsilon%7D%7D%5Ccdot+x-%5Cfrac%7Bw%5Ccdot+%5Cgamma%5Ccdot+%5Chat%7Bu%7D%7D%7B%5Csqrt%7B%5Csigma%5E2%2B%5Cvarepsilon%7D%7D+%2B%5Cbeta%5Ccdot+w%2Bb%5C%5C+%26%3D+%5Ctilde%7Bw%7D%5Ccdot+x%2B%5Ctilde%7Bb%7D+%5Cend%7Baligned%7D)

类型可以自动推倒, 不写了.

bn 放在 relu 前还是后是个玄学, 实验表明 ResA 适合中小型网络, 大型网络 ResB 更优

其实还可以有 relu 前后都加 bn 的 ResC, 然后设计一个开关, 看看哪一个BN更重要, 然后吃了不重要的那个.

如果你不喜欢 (0.485, 0.456, 0.406) 这组数据或者不想让别人知道图像均值的话

同理预过程可以等价成一个手动初始化的BatchNorm, 然后被Convolution吃掉

其他Norm有些也能被吃掉, 推倒方式都是差不多的

简化后的模型仍然是可 Fine-tune 的, 因为本来预训练就相当于初始化, 于是 Norms 只是帮助更平滑的找到一个更好的起点, 起点有了, 那...Norms 扔了也没事...

题图出处找不到了, 假装正常Cite了原画师.....

还有大家为啥都喜欢用动态类型 Python....

依赖类型不好吗>>逃

